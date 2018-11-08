//
// Created by 刘松洋 on 2018/6/21.
//

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "fft.h"
#include "wavread.h"

int main ()
{
    if(!FFT_wave())
        cout << "fail" << endl;
    /*if(!headreader())
        cout << "fail" << endl;*/
    //FFT_test();

    return 0;
}  