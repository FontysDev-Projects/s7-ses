% Load audio signal
[audioSignal, Fs] = audioread('samples\piano_hum.wav');

% Design a notch filter to remove 50 Hz hum
f0 = 50; % Notch frequency
Q = 30; % Quality factor
[b, a] = iirnotch(f0/(Fs/2), f0/(Fs*Q));

% Apply the notch filter
filteredAudio = filter(b, a, audioSignal);

% Plot the original and resulting audio
t = (0:length(audioSignal)-1)'/Fs; % Time vector
figure;
subplot(2,1,1);
plot(t, audioSignal);
title('Original Audio');
xlabel('Time (s)');
ylabel('Amplitude');
subplot(2,1,2);
plot(t, filteredAudio);
title('Filtered Audio (50 Hz Hum Removed)');
xlabel('Time (s)');
ylabel('Amplitude');

%% Frequency Domain Analysis

% FFT of original and filtered audio
NFFT = 2^nextpow2(length(audioSignal)); % Next power of 2 for efficient FFT
fftOriginal = fft(audioSignal, NFFT);
fftFiltered = fft(filteredAudio, NFFT);

% Compute magnitude spectra
fftOriginal_mag = abs(fftOriginal(1:NFFT/2+1));
fftFiltered_mag = abs(fftFiltered(1:NFFT/2+1));

% Frequency vector for plotting
f = Fs/2*linspace(0,1,NFFT/2+1);

% Find the index corresponding to the frequency closest to 50 Hz
[~, idx_50Hz] = min(abs(f - 50));

% Extract magnitudes at closest frequency to 50 Hz
mag_original_50Hz = fftOriginal_mag(idx_50Hz);
mag_filtered_50Hz = fftFiltered_mag(idx_50Hz);

% Calculate the difference in dB
hum_reduction_dB = 20 * log10(mag_original_50Hz / mag_filtered_50Hz);

% Display the result
fprintf('Hum reduction at closest frequency to 50 Hz (%.2f Hz): %.2f dB\n', f(idx_50Hz), hum_reduction_dB);

% Plot magnitude spectra (using dB scale for better visualization)
figure;
plot(f, 20*log10(fftOriginal_mag), 'b', 'DisplayName', 'Original'); hold on;
plot(f, 20*log10(fftFiltered_mag), 'r', 'DisplayName', 'Filtered');
title('Frequency Spectrum');
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
xlim([0 200]);  % Zoom in to relevant frequency range
legend show;

%% Time Domain Analysis (Zoom In)

% Zoom into a short segment for detailed comparison (adjust time window as needed)
startSample = 1;
endSample = 5000; % Adjust to zoom in on a specific region
t_zoom = t(startSample:endSample);
audioSignal_zoom = audioSignal(startSample:endSample);
filteredAudio_zoom = filteredAudio(startSample:endSample);

figure;
plot(t_zoom, audioSignal_zoom, 'b', 'DisplayName', 'Original'); hold on;
plot(t_zoom, filteredAudio_zoom, 'r', 'DisplayName', 'Filtered');
title('Zoomed Time Domain Comparison');
xlabel('Time (s)');
ylabel('Amplitude');
legend show;

% Play the original and filtered audio
sound(audioSignal, Fs);
pause(length(audioSignal)/Fs + 1);
sound(filteredAudio, Fs);
