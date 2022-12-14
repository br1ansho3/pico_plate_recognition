#ifndef MODEL_CONV
#define MODEL_CONV

#include"layer1.h"

const float weight_1[W_1] = {
    // #include "weights/init_theta1_test"
    #include "weights/ex4weight_theta1"
};

const float weight_2[W_2] = {
    #include "weights/ex4weight_theta2"
};

#endif