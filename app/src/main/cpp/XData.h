//
// Created by CC000033 on 2018/8/2.
//

#ifndef XPLAYER_XDATA_H
#define XPLAYER_XDATA_H
enum XDataType {
    AVPACKET_TYPE = 0,
    UCHAR_TYPE = 1
};

struct XData {
    int type = 0; // 数据类型
    unsigned char *data = 0;
    unsigned char *datas[8] = {0}; // 解码后的数据
    int size = 0;
    bool isAudio = false;
    int width = 0;
    int height = 0;

    bool Alloc(int size, const char *data = 0);
    void Drop();
};

#endif //XPLAYER_XDATA_H
