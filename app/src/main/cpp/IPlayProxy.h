//
// Created by CC000033 on 2018/8/9.
//

#ifndef XPLAYER_IPLAYPROXY_H
#define XPLAYER_IPLAYPROXY_H


#include <mutex>
#include "IPlayer.h"

class IPlayProxy : public IPlayer {

public:
    static IPlayProxy *Get();

    void Init(void *vm = 0);

    virtual void Close();

    bool Open(const char *path) override;


    void InitView(void *win) override;

    bool Start() override;

protected:
    IPlayProxy() {};
    IPlayer *player = 0;
    std::mutex mutex;
};


#endif //XPLAYER_IPLAYPROXY_H
