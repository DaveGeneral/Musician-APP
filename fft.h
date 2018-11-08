//
// Created by 刘松洋 on 2018/6/21.
//

#ifndef UNTITLED_FFT_H
#define UNTITLED_FFT_H

#include <cstdio>
#include <cstdlib>
#include <cmath>

const int N = 262144;
const float PI = 3.1416;
const float normalization = 0.00001526; //归一化：1/65536

inline void swap (float &a, float &b)
{
    float t;
    t = a;
    a = b;
    b = t;
}

void bitrp (float xreal [], float ximag [], int n);
void FFT(float xreal [], float ximag [], int n);
void  IFFT (float xreal [], float ximag [], int n);

void FFT_test ();
bool FFT_wave ();

#endif //UNTITLED_FFT_H
