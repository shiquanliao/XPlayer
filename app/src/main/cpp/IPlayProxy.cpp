//
// Created by CC000033 on 2018/8/9.
//

#include "IPlayProxy.h"
#include "FFPlayerBuilder.h"

IPlayProxy *IPlayProxy::Get() {
    static IPlayProxy iPlayProxy;
    return &iPlayProxy;
}

void IPlayProxy::Init(void *vm) {
    mutex.lock();
    if (vm) {
        FFPlayerBuilder::Get()->InitHard(vm);
    }
    if (!player)
        player = FFPlayerBuilder::Get()->BuilderPlayer();
    mutex.unlock();
}

bool IPlayProxy::Open(const char *path) {
    bool re = false;
    mutex.lock();
    if (player)
        re = player->Open(path);
    mutex.unlock();
    return re;
}

bool IPlayProxy::Start() {
    bool re = false;
    mutex.lock();
    if (player)
        re = player->Start();
    mutex.unlock();
    return re;
}

void IPlayProxy::InitView(void *win) {
    mutex.lock();
    if (player)
        player->InitView(win);
    mutex.unlock();
}

void IPlayProxy::Close() {
    mutex.lock();
    if (player)
        player->Close();
    mutex.unlock();
}
