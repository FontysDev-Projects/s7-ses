%% Financial Data Analysis (Apple Stock Prices)
% Read financial data from CSV
data = readtable('samples\apple-stock-price-2023.csv');

dates = data.Date;
prices = data.Close;

% Convert dates to serial date numbers for plotting
t = datenum(dates);

% Calculate moving averages
smaShort = movmean(prices, 10);    % 10-day SMA
smaLong = movmean(prices, 50);     % 50-day SMA

% Calculate EMA using the filter function
alpha = 0.2; 
ema = filter(alpha, [1 alpha-1], prices);

% Plot data and moving averages
figure;
plot(t, prices, 'b', 'DisplayName', 'Price'); hold on;
plot(t, smaShort, 'r', 'DisplayName', '10-day SMA');
plot(t, smaLong, 'g', 'DisplayName', '50-day SMA');
plot(t, ema,  'k', 'DisplayName', 'EMA'); 
datetick('x', 'yyyy-mm'); % Format x-axis to show years and months
title('Apple Stock Price and Moving Averages (2023)');
xlabel('Date');
ylabel('Price (USD)'); % Assuming prices are in USD
legend;