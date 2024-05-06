% Define filter parameters
order = 10; % Filter order
cutoff_freq = 0.2; % Cutoff frequency (normalized frequency)

% Generate low-pass filter coefficients using the window method
b = fir1(order, cutoff_freq);

% Generate a test signal (e.g., a noisy sine wave)
t = 0:0.01:100; % Time vector
x = sin(2*pi*1*t) + 0.5*randn(size(t)); % Noisy sine wave

% Apply the FIR filter to the signal
y = filter(b, 1, x);

% Plot original and filtered signals
figure;
plot(t, x, 'g', t, y, 'r');
title('Original vs Filtered Signal');
legend('Original', 'Filtered');
xlabel('Time');
ylabel('Amplitude');