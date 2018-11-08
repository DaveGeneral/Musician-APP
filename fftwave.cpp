//
// Created by 刘松洋 on 2018/6/22.
//

#include "fft.h"
#include "wavread.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <fstream>
#include <iomanip>
using namespace std;

bool FFT_wave()
{
    //open file
    fstream fs("../Source-File/xk3.wav", ios::in);

    //file processing
    if(!headreader(fs)){
        cout << "fail in head reading" << endl;
    }
    int fft_amount = ((header->data->cb_size-8)/2)/sp_amount;//((header->data->cb_size-8)/2)/sp_amount;变换次数
    cout << "FFAM " << dec << fft_amount << endl;
    cout << "SPAM " << dec << sp_amount << endl;

    SAMPLEPOINT *sp = new SAMPLEPOINT[sp_amount];
    float xreal[sp_amount*64] = {}, ximag[sp_amount*64] = {};
    float mod[sp_amount] = {};
    int max = 0;
    int freq = 0;

    int count=0;
    while(count < fft_amount){
        memset(sp, 0, sp_amount);  //每一个循环清空采样点数组
        memset(xreal, 0, sp_amount*64);
        memset(ximag, 0, sp_amount*64);  //清空xreal, ximag
        memset(mod, 0, sp_amount);
        max = 0;
        freq = 0;
        if(!datareader(sp,fs)){
            cout << "fail in data reading" << endl;
            return false;
        }

        //归一化
        for(int i=0; i<sp_amount; i++){
            xreal[i]=sp[i]*normalization;
        }

        //fft
        FFT(xreal,ximag,sp_amount);

        //取模求最大值
        for(int j=1; j<sp_amount; j++){
            mod[j] = (xreal[j]*xreal[j])+(ximag[j]*ximag[j]);
            if(mod[j] > mod[max]){
                max = j;
            }
        }

        //得到基频
        freq = max*5*1.08;  //频率分辨率为5Hz
        cout << freq << endl;

        count++;
    }

    delete[] sp;
    fs.close();

    return true;

}