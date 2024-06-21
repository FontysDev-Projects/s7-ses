% Load audio signal
[audioSignal, fs] = audioread('samples\piano_hum.wav');
numChannels = size(audioSignal, 2);  % Check number of channels

% Design filters
lpFilt = designfilt('lowpassiir', 'PassbandFrequency', 3000, 'StopbandFrequency', 5000, 'PassbandRipple', 1, 'StopbandAttenuation', 60, 'SampleRate', fs); 
hpFilt = designfilt('highpassiir', 'StopbandFrequency', 100, 'PassbandFrequency', 200, 'StopbandAttenuation', 60, 'PassbandRipple', 1, 'SampleRate', fs);

% Apply filters to simulate limited frequency response
filteredAudio = filter(lpFilt, filter(hpFilt, audioSignal));
noise = 0.01 * randn(size(filteredAudio)); % White noise
filteredAudio = filteredAudio + noise;

%% Wow and Flutter Simulation (Resampling)

% LFO parameters
lfoRate = 0.2; % Hz (adjust for wow speed)
lfoDepth = 0.005; % Semitones (adjust for wow depth)
t = (0:length(filteredAudio)-1)' / fs; % Time vector
lfo = lfoDepth * sin(2*pi*lfoRate*t);

% Resample each channel separately
shiftedAudio = zeros(size(filteredAudio));
for channel = 1:numChannels
    % Create time vector for resampling with LFO modulation
    t_resampled = t .* (1 + lfo);

    % Resample the audio using linear interpolation
    shiftedAudio(:, channel) = interp1(t, filteredAudio(:, channel), t_resampled, 'linear', 'extrap');
end

%% Surface Noise Emulation (Crackling and Popping)

% Parameters
crackleDensity = 0.01; % Probability of a crackle per sample (adjust to taste)
popDensity = 0.001;    % Probability of a pop per sample (adjust to taste)
crackleAmplitude = 0.1; % Amplitude of crackle sound (adjust to taste)
popAmplitude = 0.2;    % Amplitude of pop sound (adjust to taste)

% Create crackling and popping sounds
crackles = crackleAmplitude * (rand(size(shiftedAudio)) < crackleDensity) .* sign(randn(size(shiftedAudio)));
pops = popAmplitude * (rand(size(shiftedAudio)) < popDensity) .* randn(size(shiftedAudio));

% Add surface noise to filtered audio
shiftedAudio = shiftedAudio + crackles + pops;

% Add a subtle amount of white noise for background hiss
shiftedAudio = shiftedAudio + 0.005 * randn(size(shiftedAudio));

% Normalize the audio to prevent clipping (optional)
shiftedAudio = shiftedAudio / max(abs(shiftedAudio(:)));

%% Frequency Domain Analysis

% FFT of original and processed audio
NFFT = 2^nextpow2(length(audioSignal));
fftOriginal = fft(audioSignal(:, 1), NFFT);  % Analyze first channel if stereo
fftProcessed = fft(shiftedAudio(:, 1), NFFT);

% Compute magnitude spectra in dB
f = Fs/2 * linspace(0, 1, NFFT/2+1);
fftOriginalMag = 20*log10(abs(fftOriginal(1:NFFT/2+1)));
fftProcessedMag = 20*log10(abs(fftProcessed(1:NFFT/2+1)));

% Plot magnitude spectra
figure;
plot(f, fftOriginalMag, 'b', 'DisplayName', 'Original'); hold on;
plot(f, fftProcessedMag, 'r', 'DisplayName', 'Processed');
title('Frequency Spectrum Comparison');
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
legend;
xlim([0 8000]); % Adjust as needed

%% Time Domain Analysis (Zoom)

% Zoom into a segment for detailed comparison (adjust range as needed)
startSample = 1;
endSample = 5000; % Adjust to desired zoom level
t_zoom = t(startSample:endSample);
originalZoom = audioSignal(startSample:endSample, 1);  % First channel if stereo
processedZoom = shiftedAudio(startSample:endSample, 1);

% Plot time-domain waveforms
figure;
plot(t_zoom, originalZoom, 'b', 'DisplayName', 'Original'); hold on;
plot(t_zoom, processedZoom, 'r', 'DisplayName', 'Processed');
title('Zoomed Time Domain Comparison');
xlabel('Time (s)');
ylabel('Amplitude');

% Play audio
sound(shiftedAudio, fs);
