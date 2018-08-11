//
// Created by CC000033 on 2018/8/9.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IVideoView.h"
#include "IAudioPlay.h"
#include "IResample.h"
#include "XLog.h"

IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

void IPlayer::Close() {
    mutex.lock();
    //1 先关闭主体线程，再清理观察者
    //同步线程
    XThread::Stop();
    //解封装
    if(demux)
        demux->Stop();
    //解码
    if(vDecode)
        vDecode->Stop();
    if(aDecode)
        aDecode->Stop();

    //2 清理缓冲队列
    if(vDecode)
        vDecode->Clear();
    if(aDecode)
        aDecode->Clear();
    if(audioPlay)
        audioPlay->Clear();

    //3 清理资源
    if(audioPlay)
        audioPlay->Close();
    if(videoView)
        videoView->Close();
    if(vDecode)
        vDecode->Close();
    if(aDecode)
        aDecode->Close();
    if(demux)
        demux->Close();
    mutex.unlock();

}

bool IPlayer::Open(const char *path) {
    Close();
    mutex.lock();
    //解封装
    if (!demux || !demux->Open(path)) {
        mutex.unlock();
        XLOGE("demux->Open %s failed!", path);
        return false;
    }

    //解码 解码可能不需要，如果是解封之后就是原始数据
    if (!vDecode || !vDecode->Open(demux->GetVPara(), isHardDecode)) {
        XLOGE("vDecode->Open %s failed!", path);
        //return false;
    }
    if (!aDecode || !aDecode->Open(demux->GetAPara())) {
        XLOGE("adecode->Open %s failed!", path);
        //return false;
    }
    //重采样 有可能不需要，解码后或者解封后可能是直接能播放的数据
    if (outPara.sample_rate <= 0)
        outPara = demux->GetAPara();
    if (!resample || !resample->Open(demux->GetAPara(), outPara)) {
        XLOGE("resample->Open %s failed!", path);
    }

    mutex.unlock();
    return true;
}

bool IPlayer::Start() {

    mutex.lock();

    if(audioPlay)
        audioPlay->StartPlay(outPara);
    if(vDecode)
        vDecode->Start();

    if(aDecode)
        aDecode->Start();

    if(!demux || !demux->Start())
    {
        mutex.unlock();
        XLOGE("demux->Start failed!");
        return false;
    }


    XThread::Start();
    mutex.unlock();
    return true;
}


void IPlayer::InitView(void *win) {
    if (videoView) {
        videoView->SetRender(win);
    }
}

void IPlayer::Main() {
    while (!isExit) {
        mutex.lock();
        if (!audioPlay || !vDecode) {
            mutex.unlock();
            XSleep(2);
            continue;
        }

        //同步
        //获取音频的pts 告诉视频
        int apts = audioPlay->pts;
        XLOGE("apts = %d", apts);
        vDecode->synPts = apts;

        mutex.unlock();
        XSleep(200);
    }
}

IPlayer::IPlayer() {

}
