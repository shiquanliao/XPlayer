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

bool IPlayer::Open(const char *path) {
    //解封装
    if (!demux || !demux->Open(path)) {
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

    return true;
}

bool IPlayer::Start() {

    if(!demux || !demux->Start())
    {
        XLOGE("demux->Start failed!");
        return false;
    }
    if(aDecode)
        aDecode->Start();
    if(audioPlay)
        audioPlay->StartPlay(outPara);
    if(vDecode)
        vDecode->Start();
    return true;
}

void IPlayer::InitView(void *win) {
    if (videoView) {
        videoView->SetRender(win);
    }
}

IPlayer::IPlayer() {

}
