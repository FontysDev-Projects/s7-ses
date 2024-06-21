% Read an image and convert to grayscale
img = imread('samples\flower-image.jpg');
grayImg = rgb2gray(img);

% Create a high-pass filter (Sobel)
h = fspecial('sobel');

% Apply the filter to the image
edges = imfilter(grayImg, h);

% Display the original image and edges
figure;
subplot(1, 2, 1);
imshow(grayImg);
title('Original Image');
subplot(1, 2, 2);
imshow(edges);
title('Edges Detected');

%% Frequency Domain Analysis

% FFT of original and edge images
fftOriginal = fftshift(fft2(grayImg));
fftEdges = fftshift(fft2(edges));

% Compute magnitude spectra
fftOriginal_mag = abs(fftOriginal);
fftEdges_mag = abs(fftEdges);

% Display magnitudes of FFTs (using log scale for better visualization)
figure;
subplot(1,2,1);
imshow(log(1 + fftOriginal_mag), []); 
title('FFT Original Image');
subplot(1,2,2);
imshow(log(1 + fftEdges_mag), []);
title('FFT Edges Detected');

%% Time Domain Analysis

% Select a row/column for line profile (e.g., center row)
centerLine = round(size(grayImg,1)/2);  % Center row
originalProfile = grayImg(centerLine,:);
edgesProfile = edges(centerLine,:);

figure;
plot(originalProfile, 'b', 'DisplayName', 'Original');
hold on;
plot(edgesProfile, 'r', 'DisplayName', 'Edges');
title('Pixel Intensity Profile (Center Row)');
xlabel('Pixel Position');
ylabel('Intensity');
legend show;
