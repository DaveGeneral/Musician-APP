//
// Created by 刘松洋 on 2018/6/21.
//

#include "wavread.h"
#include "makeunique.h"
#include <iostream>
#include <memory>
#include <fstream>
#include <iomanip>
using namespace std;

int sp_curr = 44;  //默认DATA数据从44字节起始
unique_ptr<Wave_header> header = make_unique<Wave_header>();

bool headreader(fstream &ifs)
{
    // Read RIFF chunk
    FOURCC fourcc;
    ifs.read((char*)&fourcc, sizeof(FOURCC));
    if (fourcc != MakeFOURCC<'R', 'I', 'F', 'F'>::value) // 判断是不是RIFF
        return false;
    Base_chunk riff_chunk(fourcc);
    ifs.read((char*)&riff_chunk.cb_size, sizeof(uint32_t));
    header->riff = make_shared<Base_chunk>(riff_chunk);

    // Read WAVE FOURCC
    ifs.read((char*)&fourcc, sizeof(FOURCC));
    if (fourcc != MakeFOURCC<'W', 'A', 'V', 'E'>::value)
        return false;
    header->wave_fcc = fourcc;

    //Skip JUNK chunk
    ifs.read((char*)&fourcc,sizeof(FOURCC));
    if(fourcc == MakeFOURCC<'J', 'U', 'N', 'K'>::value){
        ifs.seekg(JUNK_SIZE,ios::cur);
        cout << "JUNK Block skipped." << endl;
        sp_curr=80;    //有JUNK块的时候，DATA数据从80字节开始
    }
    else{
        ifs.seekg(-sizeof(FOURCC),ios::cur); //退回FOURCC前
    }

    //Read fmt chunk
    ifs.read((char*)&fourcc,sizeof(FOURCC));
    if (fourcc != MakeFOURCC<'f', 'm', 't', ' '>::value)
        return false;
    Base_chunk fmt_chunk(fourcc);
    ifs.read((char*)&fmt_chunk.cb_size, sizeof(uint32_t));
    header->fmt = make_shared<Base_chunk>(fmt_chunk);

    //Read wave format
    Wave_format WaveFmt;
    ifs.read((char*)&WaveFmt.format_tag, sizeof(uint16_t));
    if (WaveFmt.format_tag != 0x01)
        return false;
    ifs.read((char*)&WaveFmt.channels, sizeof(uint16_t));
    ifs.read((char*)&WaveFmt.sample_per_sec, sizeof(uint32_t));
    ifs.read((char*)&WaveFmt.bytes_per_sec, sizeof(uint32_t));
    ifs.read((char*)&WaveFmt.block_align, sizeof(uint16_t));
    ifs.read((char*)&WaveFmt.bits_per_sample, sizeof(uint16_t));
    header->fmt_data = make_shared<Wave_format>(WaveFmt);

    //Read data chunk
    ifs.read((char*)&fourcc, sizeof(FOURCC));
    if(fourcc != MakeFOURCC<'d', 'a', 't', 'a'>::value)
        return false;
    Base_chunk wave_chunk(fourcc);
    ifs.read((char*)&wave_chunk.cb_size, sizeof(uint32_t));
    header->data = make_shared<Base_chunk>(wave_chunk);

    cout << dec << header->riff->cb_size << " " << header->fmt->cb_size << " " << header->data->cb_size << endl;

    return true;
}

//取左声道数据
bool datareader(SAMPLEPOINT* SPP, fstream &difs)
{
    difs.seekg(sp_curr);
    for(int i=0; i<sp_amount; i++){
        difs.read((char*)&SPP[i], sizeof(SAMPLEPOINT));
        difs.seekg(sizeof(SAMPLEPOINT), ios::cur);   //指针向后移动两个字节，跳过右声道
    }
    sp_curr += sp_amount*sizeof(SAMPLEPOINT)*2;
    return true;
}

