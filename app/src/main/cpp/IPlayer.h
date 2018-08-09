//
// Created by CC000033 on 2018/8/9.
//

#ifndef XPLAYER_IPLAYER_H
#define XPLAYER_IPLAYER_H


#include "XThread.h"
#include "XParameter.h"

class IDemux;

class IDecode;

class IAudioPlay;

class IVideoView;

class IResample;

class IPlayer : public XThread {
public:
    static IPlayer *Get(unsigned char index = 0);

    virtual bool Open(const char *path);

    virtual void InitView(void *win);

    bool Start() override;


    //是否视频硬解码
    bool isHardDecode = true;

    //音频输出参数配置
    XParameter outPara;

    IDemux *demux = 0;
    IDecode *vDecode = 0;
    IDecode *aDecode = 0;
    IResample *resample = 0;
    IAudioPlay *audioPlay = 0;
    IVideoView *videoView = 0;

protected:
    IPlayer();

};


#endif //XPLAYER_IPLAYER_H
