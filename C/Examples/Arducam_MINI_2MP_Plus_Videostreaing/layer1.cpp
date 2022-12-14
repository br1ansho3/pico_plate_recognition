#include "stdio.h"
#include "layer1.h"
#include "weights.h"
#include <cmath>
// Append Bias to Dataset
//----------------------------------------------------------
// @param[in] : input - input perceptrons
//              I - number of perceptrons
//              M - number of input fmaps
//              
// @param[out] : output - output perceptrons
void addBias(float* input, int I, int M, float* output) {
    // input matrix is M*F
    int O = I + 1;    // O - number of output perceptrons
    for(int m = 0; m < M; m++) {
        int b_index = m*O;
        output[b_index] = 1;
        for(int i = 0; i < I; i++) {
            int i_index = i + m*I;
            int o_index = (i + 1) + m*O;
            output[o_index] = input[i_index];
        }
    }
}

// Apply activation function for each perceptron (sigmoid)
//----------------------------------------------------------
// @param[in] : input - input perceptrons (M*N)
//              M - number of input fmaps
//              N - number of perceptrons (features)
// @param[out] : output - value after activation (M*N)
// void activation(float* input, float* output, int M, int N) { // input output array can be the same too
//     for(int m = 0; m < M; m++) {
//         for(int n = 0, index = m*N; n < N; n++, index+=1) {
//             // int index = m*N + n;     
//             output[index] = sigmoid(input[index]); // sigmoid function 
//         }
//     }
// }

// Predict Digit by taking max perceptron 
//----------------------------------------------------------
// @param[in] : input - last layer perceptron values (M*I)
//              I - number of perceptrons (digit in order: 1 - 9, 0 )
//              M - number of input fmaps         
// @param[out] : output - predicted digit (M*1)
void getDigit(float* input, int I, int M, int* output) {
    for(int m = 0; m < M; m++) {
        int digit = 0;
        // read in perceptron value representing 0 (technically 1)
        float max_value = input[m*I]; 
        for(int i = 0; i < I; i++) {
            int index = m*I + i;
            float value = input[index];
            if(value > max_value) {
                max_value = value;
                digit = i;
            }
        }
        output[m] = (digit + 1)%10; // digit tranlated
    }
}

float sigmoid( float a ) {
    return (float)(1 / (1 + (float)exp(-a)));
};

// Compute value of output nodes via dot product (z = Xw)
//----------------------------------------------------------
// @param[in] : input - input fmaps (includes bias)
//              weight - weights (includes bias weight)
//              I - number of perceptrons (input)
//              O - number of perceptrons (output)
//              M - number of input fmaps
//              Theta1 (25x401)
//              Theta2 (10x26)
// @param[out] : output - output fmaps
void linearComb(float* input, const float* weight, int I, int O, int M, float* output) {
    for(int m = 0; m < M; m++) {
        for(int o = 0; o < O; o++) { 
            int o_index = (m*O) + o ;
            
            float sum = 0;
            for(int i = 0; i < I; i++) {
                // int w_index = (i*O) + o; 
                int w_index = (o*I) + i;
                int i_index = (m*I) + i;
                sum += input[i_index] * weight[w_index];                       
            }
            output[o_index] = sigmoid(sum);
        }
    }
}



void feedforward(float input[MAXSIZE], int SAMPLES, int* prediction){
  /* Feedforward core */
  float a0wb[SAMPLES*401];
//   float z1[SAMPLES*25];
  float a1[SAMPLES*25];
  float a1wb[SAMPLES*26];
//   float z2[SAMPLES*10];
  float a2[SAMPLES*10];

//   // /* First layer */ 
  addBias(input, LAYER_0, SAMPLES, a0wb); 
  linearComb(a0wb, weight_1, LAYER_0+1, LAYER_1, SAMPLES, a1);
//   activation(z1, a1, SAMPLES, LAYER_1);

  // /* Second layer */
  addBias(a1, LAYER_1, SAMPLES, a1wb);
  linearComb(a1wb, weight_2, LAYER_1+1, LAYER_2, SAMPLES, a2);
//   activation(z2, a2, SAMPLES, LAYER_2);

  getDigit(a2, LAYER_2, SAMPLES, prediction);
}
  