//
// Created by CC000033 on 2018/8/6.
//

#ifndef XPLAYER_SLAUDIOPLAY_H
#define XPLAYER_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay {
public:
    bool StartPlay(XParameter out) override;
    void PlayCall(void *bufq);


};


#endif //XPLAYER_SLAUDIOPLAY_H
