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

    bool SendPacket(XData pkt) override;

    XData RecvFrame() override;

protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
};


#endif //XPLAYER_FFDECODE_H
