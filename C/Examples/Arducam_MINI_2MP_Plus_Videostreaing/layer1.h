#ifndef LAYER_H
#define LAYER_H

const int MAXSIZE = 800;

const int image_d = 20;
const int W_1 = 25*401; 
const int W_2 = 10*26;
// Hyperparameters for 2 layer neural net
const int LAYER_0 = 400; // input layer
const int LAYER_1 = 25;
const int LAYER_2 = 10; // output layer 


/// functions 
void addBias(float* input, int I, int M, float* output);
// void activation(float* input, float* output, int M, int N);
void getDigit(float* input, int I, int M, int* output);
float sigmoid( float a ); 
void linearComb(float* input, const float* weight, int I, int O, int M, float* output);
void feedforward(float input[MAXSIZE], int SAMPLES, int* prediction);
#endif