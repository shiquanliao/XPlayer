//
// Created by CC000033 on 2018/8/3.
//

#ifndef XPLAYER_XPARAMETER_H
#define XPLAYER_XPARAMETER_H


struct AVCodecParameters;

class XParameter {
public:
    AVCodecParameters *para = 0;
    int channels = 2;
    int sample_rate = 44100;

};


#endif //XPLAYER_XPARAMETER_H
