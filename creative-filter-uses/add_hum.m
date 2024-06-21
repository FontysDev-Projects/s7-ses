% MATLAB script to add a 50 Hz hum to an audio file

% Read the audio file
[input_audio, Fs] = audioread('samples\piano_clean.wav'); % Replace with your audio file name
input_audio = input_audio(:,1); % If stereo, convert to mono

% Parameters
hum_frequency = 50; % Frequency of the hum in Hz
hum_amplitude = 0.05; % Amplitude of the hum (adjust as needed)
duration = length(input_audio) / Fs; % Duration of the audio file

% Generate the 50 Hz hum
t = (0:length(input_audio)-1)'/Fs; % Time vector
hum = hum_amplitude * sin(2 * pi * hum_frequency * t);

% Add the hum to the original audio
output_audio = input_audio + hum;

% Normalize the output to prevent clipping
output_audio = output_audio / max(abs(output_audio));

% Write the resulting audio to a new file
audiowrite('samples\piano_hum.wav', output_audio, Fs);

% Plot the original and resulting audio for comparison (optional)
figure;
subplot(2,1,1);
plot(t, input_audio);
title('Original Audio');
xlabel('Time (s)');
ylabel('Amplitude');

subplot(2,1,2);
plot(t, output_audio);
title('Audio with 60 Hz Hum');
xlabel('Time (s)');
ylabel('Amplitude');
