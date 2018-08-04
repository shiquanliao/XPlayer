//
// Created by CC000033 on 2018/8/3.
//

#ifndef XPLAYER_FFDECODE_H
#define XPLAYER_FFDECODE_H


#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;

class FFDecode : public IDecode {
public:
    bool Open(XParameter para) override;

    //future模型 发送数据到线程解码
    bool SendPacket(XData pkt) override;

    //从线程中获取解码结果，再次调用会复用上次空间，线程不安全
    XData RecvFrame() override;

protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
};


#endif //XPLAYER_FFDECODE_H
