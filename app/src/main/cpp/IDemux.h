//
// Created by CC000033 on 2018/8/2.
//

#ifndef XPLAYER_IDEMUX_H
#define XPLAYER_IDEMUX_H


#include "XData.h"
#include "IObserver.h"
#include "XParameter.h"

class IDemux : public IObserver {
public:
    //打开文件，或者流媒体 rmtp http rtsp
    virtual bool Open(const char *url) = 0;

    //打开输出流,可以输入到rtmp,文件等地方
    virtual bool OpenOutput(char *fileName) = 0;

    virtual void Close() = 0;

    //获取视频参数
    virtual XParameter GetVPara() = 0;

    //获取音频参数
    virtual XParameter GetAPara() = 0;

    //读取一帧数据，数据由调用者清理
    virtual XData Read() = 0;

    //总时长（毫秒）
    int totalMs = 0;

protected:
    virtual void Main() override;

    virtual void Update(XData data) override;

};


#endif //XPLAYER_IDEMUX_H
