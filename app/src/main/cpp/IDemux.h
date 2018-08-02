//
// Created by CC000033 on 2018/8/2.
//

#ifndef XPLAYER_IDEMUX_H
#define XPLAYER_IDEMUX_H


#include "XData.h"
#include "XThread.h"

class IDemux : public XThread {
public:
    //打开文件，或者流媒体 rmtp http rtsp
    virtual bool Open(const char *url) = 0;

    //读取一帧数据，数据由调用者清理
    virtual XData Read() = 0;

    //总时长（毫秒）
    int totalMs = 0;

protected:
    virtual void Main();

};


#endif //XPLAYER_IDEMUX_H
