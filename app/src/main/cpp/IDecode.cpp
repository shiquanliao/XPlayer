//
// Created by CC000033 on 2018/8/3.
//

#include "IDecode.h"

void IDecode::Update(XData data) {
    if (data.isAudio != isAudio) {
        return;
    }
    packsMutex.lock();

    while (!isExit) {
        //阻塞
        if (packs.size() < maxList) {
            packs.push_back(data);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        XSleep(1);
    }


}

void IDecode::Main() {

    while (!isExit) {
        packsMutex.lock();
        if (packs.empty()) {
            packsMutex.unlock();
            XSleep(1);
            continue;
        }
        // 取出packet
        XData pack = packs.front();
        packs.pop_front();  //把头部的数据从列表中删除


        packsMutex.unlock();
    }
}
