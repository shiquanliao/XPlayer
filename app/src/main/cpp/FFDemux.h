//
// Created by CC000033 on 2018/8/2.
//

#ifndef XPLAYER_FFDEMUX_H
#define XPLAYER_FFDEMUX_H


#include "XData.h"
#include "IDemux.h"

struct AVFormatContext;
struct AVOutputFormat;

class FFDemux : public IDemux {
public:
    //打开文件，或者流媒体 rmtp http rtsp
    virtual bool Open(const char *url) override;

    bool OpenOutput(const char *fileName, unsigned char i) override;

    void Close() override;

    //获取视频参数
    virtual XParameter GetVPara() override;

    //获取音频参数
    XParameter GetAPara() override;

    //读取一帧数据，数据由调用者清理
    virtual XData Read() override;

    void StartPushStream(const char *path, unsigned char i) override;

    void StopPushStream() override;

    FFDemux();

private:
    //输入上下文
    AVFormatContext *ic = 0; // 只有c++11以上,并且只有无参构造函数时才生效
    //输出上下文
    AVFormatContext *outputContext = 0;
    //输出上下文
    AVOutputFormat *ofmt = NULL;
    std::mutex mutex;
    int audioStream = 1;
    int videoStream = 0;

};


#endif //XPLAYER_FFDEMUX_H
