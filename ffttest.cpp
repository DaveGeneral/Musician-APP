//
// Created by 刘松洋 on 2018/6/21.
//

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "fft.h"

void FFT_test ()
{
    char inputfile [] = "../input.txt";    // 从文件 input.txt 中读入原始数据
    char outputfile [] = "../output.txt";    // 将结果输出到文件 output.txt 中
    float xreal [N] = {}, ximag [N] = {};
    int n, i;
    FILE *input, *output;

    if (!(input = fopen (inputfile, "r")))
    {
        printf ("Cannot open file. ");
        exit (1);
    }
    if (!(output = fopen (outputfile, "w")))
    {
        printf ("Cannot write file. ");
        exit (1);
    }

    i = 0;
    while ((fscanf (input, "%f%f", xreal + i, ximag + i)) != EOF)
    {
        i ++;
    }
    n = i;    // 要求 n 为 2 的整数幂
    while (i > 1)
    {
        if (i % 2)
        {
            fprintf (output, "%d is not a power of 2! ", n);
            exit (1);
        }
        i /= 2;
    }

    FFT (xreal, ximag, n);
    fprintf (output, "FFT:    i     real    imag \n");
    for (i = 0; i < n; i ++)
    {
        fprintf (output, "%4d    %8.4f    %8.4f \n", i, xreal [i], ximag [i]);
    }
    fprintf (output, "================================= \n");

    IFFT (xreal, ximag, n);
    fprintf (output, "IFFT:    i        real    imag \n");
    for (i = 0; i < n; i ++)
    {
        fprintf (output, "%4d    %8.4f    %8.4f \n", i, xreal [i], ximag [i]);
    }

    if ( fclose (input))
    {
        printf ("File close error. ");
        exit (1);
    }
    if ( fclose (output))
    {
        printf ("File close error. ");
        exit (1);
    }
}