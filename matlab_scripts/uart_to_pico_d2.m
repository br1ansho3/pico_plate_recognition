clear; close all;

% load pre-trained weights 
load("weights.mat");
% import bitmap 
A = imread("test_bmps\example.bmp", "bmp");

% view image
imagesc(A)
% pooling to get desried size of 20*20
[row, col] = size(A);
M = 20;
N = 20;

% there are 2 digits (split bmp vertically down the middle)
% memory for binarized 20x20 
output1 = zeros(M, N);
output2 = zeros(M,N);

% pooling parameters
pool_h = row/M;
pool_w = (col/N)/2;

% split image into two
A1 = A(:,1:col/2);
A2 = A(:,col/2+1:end);
imagesc(A1)
imagesc(A2)

% pooling
for i = 1:M
    for j = 1:N
        tile_h_range = (i-1)*pool_h + (1:pool_h);
        tile_w_range = (j-1)*pool_w + (1:pool_w);
        tile = A1(tile_h_range , tile_w_range); 
        % write to pooled cell 
        output1(i, j) = min(tile, [], 'all');

        tile = A2(tile_h_range, tile_w_range);
        output2(i,j) = min(tile, [], 'all');
    end
end

% imagesc(output) %uncomment to see pooled image
 
% binarize output
digit_index = output1 ~= 255;
space_index = output1 == 255;
output1(digit_index) = 1;
output1(space_index) = 0;

digit_index = output2 ~= 255;
space_index = output2 == 255;
output2(digit_index) = 1;
output2(space_index) = 0;


% feed output into modele for prediciton 
output1 = reshape(output1, 1, []);
output2 = reshape(output2, 1, []);

% predict digit
[z1,h1,p] = predict(Theta1, Theta2, output1);
p = mod(p, 10);
fprintf("predicted number 1 is %d\n", p);
[z2,h2,p1] = predict(Theta1, Theta2, output2);
p1 = mod(p1, 10);
fprintf("predicted number 2 is %d\n", p1);

% create serial port and write pooled data via uart 
device = serialport("COM3", 115200);
write(device,[output1, output2],"uint8")


% end program 