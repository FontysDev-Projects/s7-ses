% Load and display original image
img = imread('samples\image-static-noise.png');
grayImg = rgb2gray(img);

% Define different filters and sizes
filters = {fspecial('average', [3 3]), ...
           fspecial('average', [5 5]), ...
           fspecial('gaussian', [3 3], 0.5), ...
           fspecial('gaussian', [5 5], 1)};
filterNames = {'Average 3x3', 'Average 5x5', 'Gaussian 3x3', 'Gaussian 5x5'};

% Display original image and FFT
figure;
subplot(2, length(filters) + 1, 1);
imshow(grayImg);
title('Original Image');

fftOriginal = fftshift(fft2(grayImg));
fftOriginal_mag = abs(fftOriginal);
subplot(2, length(filters) + 1, length(filters) + 2);
imshow(log(1 + fftOriginal_mag), []);
title('FFT Original Image');

% Apply filters and display results
for i = 1:length(filters)
    h = filters{i};
    filterName = filterNames{i};

    % Apply filter to the whole image
    smoothedImg = imfilter(grayImg, h); 
    
    % Display smoothed image
    subplot(2, length(filters) + 1, i+1);
    imshow(smoothedImg);
    title(filterName);

    % FFT of smoothed image
    fftSmoothed = fftshift(fft2(smoothedImg));
    fftSmoothed_mag = abs(fftSmoothed);

    % Display magnitude spectrum of smoothed image
    subplot(2, length(filters) + 1, i+1+length(filters)+1);
    imshow(log(1 + fftSmoothed_mag), []);
    title(['FFT ' filterName]);
end

%% Time Domain Analysis (for all filters)

centerLine = round(size(grayImg,1)/2);
originalProfile = grayImg(centerLine,:);

figure; 
plot(originalProfile, 'b', 'DisplayName', 'Original'); hold on;

for i = 1:length(filters)
    h = filters{i};
    filterName = filterNames{i};

    % Apply filter to the whole image
    smoothedImg = imfilter(grayImg, h); 
    
    % Extract center line profile
    smoothedProfile = smoothedImg(centerLine,:);

    plot(smoothedProfile, 'DisplayName', filterName);
end

title('Pixel Intensity Profile at Center Line (All Filters)');
xlabel('Pixel Position'); ylabel('Intensity');
legend show;
