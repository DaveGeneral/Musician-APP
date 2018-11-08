//
// Created by 刘松洋 on 2018/6/21.
//

#ifndef UNTITLED_WAVREAD_H
#define UNTITLED_WAVREAD_H

#include <iostream>
#include <cstdint>
#include <memory>
using namespace std;

#define SAMPLEPOINT uint16_t
#define JUNK_SIZE 32  //不算ID时JUNK块一共32字节
extern int sp_curr; //当前采样点读取位置
static const int sp_amount = 4096; //每次FFT采样点数 2^12

//MakeFOURCC
#define FOURCC uint32_t
#define MAKE_FOURCC(a,b,c,d) \
( ((uint32_t)d) | ( ((uint32_t)c) << 8 ) | ( ((uint32_t)b) << 16 ) | ( ((uint32_t)a) << 24 ) )
template <char ch0, char ch1, char ch2, char ch3>
struct MakeFOURCC{
    enum { value = (ch0 << 0) + (ch1 << 8) + (ch2 << 16) + (ch3 << 24) };
};

// The basic chunk of RIFF file format
struct Base_chunk{

    FOURCC fcc;    // FourCC id
    uint32_t cb_size; // 数据域的大小

    Base_chunk(FOURCC fourcc)
            : fcc(fourcc)
    {
        cb_size = 0;
    }
};

// Format chunk data field
struct Wave_format{

    uint16_t format_tag;      // WAVE的数据格式，PCM数据该值为1
    uint16_t channels;        // 声道数
    uint32_t sample_per_sec;  // 采样率
    uint32_t bytes_per_sec;   // 码率，channels * sample_per_sec * bits_per_sample / 8
    uint16_t block_align;     // 音频数据块，每次采样处理的数据大小，channels * bits_per_sample / 8
    uint16_t bits_per_sample; // 量化位数，8、16、32等
    uint16_t ex_size;         // 扩展块的大小，附加块的大小

    Wave_format()
    {
        format_tag      = 1; // PCM format data
        ex_size         = 0; // don't use extesion field

        channels        = 0;
        sample_per_sec  = 0;
        bytes_per_sec   = 0;
        block_align     = 0;
        bits_per_sample = 0;
    }

    Wave_format(uint16_t nb_channel, uint32_t sample_rate, uint16_t sample_bits)
            :channels(nb_channel), sample_per_sec(sample_rate), bits_per_sample(sample_bits)
    {
        format_tag    = 0x01;                                            // PCM format data
        bytes_per_sec = channels * sample_per_sec * bits_per_sample / 8; // 码率
        block_align   = channels * bits_per_sample / 8;
        ex_size       = 0;                                               // don't use extension field
    }
};

/*

    数据格式为PCM的WAV文件头
    --------------------------------
    | Base_chunk | RIFF |
    ---------------------
    | WAVE              |
    ---------------------
    | Base_chunk | fmt  |   Header
    ---------------------
    | Wave_format|      |
    ---------------------
    | Base_chunk | data |
    --------------------------------
*/
struct Wave_header{

    shared_ptr<Base_chunk> riff;
    FOURCC wave_fcc;
    shared_ptr<Base_chunk> fmt;
    shared_ptr<Wave_format>  fmt_data;
    shared_ptr<Base_chunk> data;

    Wave_header(uint16_t nb_channel, uint32_t sample_rate, uint16_t sample_bits)
    {
        riff      = make_shared<Base_chunk>(MakeFOURCC<'R', 'I', 'F', 'F'>::value);
        fmt       = make_shared<Base_chunk>(MakeFOURCC<'f', 'm', 't', ' '>::value);
        fmt->cb_size = 18;

        fmt_data  = make_shared<Wave_format>(nb_channel, sample_rate, sample_bits);
        data      = make_shared<Base_chunk>(MakeFOURCC<'d', 'a', 't', 'a'>::value);

        wave_fcc = MakeFOURCC<'W', 'A', 'V', 'E'>::value;
    }

    Wave_header()
    {
        riff         = nullptr;
        fmt          = nullptr;

        fmt_data     = nullptr;
        data         = nullptr;

        wave_fcc     = 0;
    }
};


bool headreader(fstream &ifs);
bool datareader(SAMPLEPOINT* SPP,fstream &difs);
extern unique_ptr<Wave_header> header;

#endif //UNTITLED_WAVREAD_H
