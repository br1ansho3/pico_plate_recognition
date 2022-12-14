clear; 

% load pre-trained weights 
load("weights.mat");
% import bitmap 
A = imread("test_bmps\example2.bmp", "bmp");
% which pixels are RGB 3, 3, 2

% binarize?? 
% imagesc(A)
% pooling to get desried size of 20*20
[row, col] = size(A);
M = 20;
N = 20;

% memory for binarized 20x20 
output = zeros(M, N);
pool_h = row/M;
pool_w = col/N;

for i = 1:M
    for j = 1:N
        tile_h_range = (i-1)*pool_h + (1:pool_h);
        tile_w_range = (j-1)*pool_w + (1:pool_w);
        tile = A(tile_h_range , tile_w_range); 

        % write to pooled cell 
        output(i, j) = min(tile, [], 'all');
    end
end

% imagesc(output) %uncomment to see pooled image
 
% binarize output

digit_index = output ~= 255;
space_index = output == 255;
output(digit_index) = 1;
output(space_index) = 0;
% 
test = reshape(output, [20,20]);
imagesc(test);
% feed output into modele for prediciton 
output = reshape(output, 1, []);
% output = output;
p = predict(Theta1, Theta2, output);
p = mod(p, 10);
fprintf("predicted number %d\n", p);


% create serial port and write pooled data via uart 
device = serialport("COM3", 115200);
% % write(device,1:5,"uint8")
%write 20x20 line by line 
write(device,output,"uint8")





% end program 