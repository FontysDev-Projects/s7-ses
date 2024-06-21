% Load and display original image
originalImage = imread('samples\flower-image.jpg');  
imshow(originalImage); 
title('Original Image');

% Blur the image (low-pass filter for creating the mask)
blurredImage = imgaussfilt(originalImage, 2); 
figure; imshow(blurredImage); 
title('Blurred Image (Mask)');

% Create the unsharp mask
mask = originalImage - blurredImage;
figure; imshow(mask); 
title('Unsharp Mask');

% Apply the mask (scaling factor can be adjusted for desired sharpness)
k = 1.5; 
sharpenedImage = originalImage + k * mask;
figure; imshow(sharpenedImage); 
title('Sharpened Image');

%% Frequency Domain Analysis

% Convert images to double precision for FFT calculations
originalImage = double(originalImage);
blurredImage = double(blurredImage);
sharpenedImage = double(sharpenedImage);

% FFT of original, blurred, and sharpened images
fftOriginal = fft2(originalImage);
fftBlurred = fft2(blurredImage);
fftSharpened = fft2(sharpenedImage);

% Shift FFT for better visualization
fftOriginal = fftshift(fftOriginal);
fftBlurred = fftshift(fftBlurred);
fftSharpened = fftshift(fftSharpened);

% Compute magnitude spectra for each color channel separately
fftOriginal_mag = abs(fftOriginal);
fftBlurred_mag = abs(fftBlurred);
fftSharpened_mag = abs(fftSharpened);

% Display magnitudes of FFTs for each channel (Red, Green, Blue)
figure;
for channel = 1:3 % Loop over RGB channels
    subplot(1,3,channel);
    imshow(log(fftOriginal_mag(:,:,channel) + 1), []); % Scale and display for each channel
    title(sprintf('FFT Original (Channel %d)', channel));
end

figure;
for channel = 1:3
    subplot(1,3,channel);
    imshow(log(fftBlurred_mag(:,:,channel) + 1), []);
    title(sprintf('FFT Blurred (Channel %d)', channel));
end

figure;
for channel = 1:3
    subplot(1,3,channel);
    imshow(log(fftSharpened_mag(:,:,channel) + 1), []);
    title(sprintf('FFT Sharpened (Channel %d)', channel));
end

%% Time Domain Analysis

% Plot a line profile through the center of the images
centerLine = size(originalImage,1)/2;
originalProfile = originalImage(centerLine,:);
blurredProfile = blurredImage(centerLine,:);
sharpenedProfile = sharpenedImage(centerLine,:);

figure; 
plot(originalProfile, 'b'); hold on;
plot(blurredProfile, 'r');
plot(sharpenedProfile, 'g'); 
title('Pixel Intensity Profile at Center Line');
xlabel('Pixel Position'); ylabel('Intensity');
legend('Original', 'Blurred', 'Sharpened');
