//
// Created by CC000033 on 2018/8/3.
//

#include "IDecode.h"
#include "XLog.h"

void IDecode::Update(XData data) {
    if (data.isAudio != isAudio) {
        return;
    }
    packsMutex.lock();

    while (!isExit) {
        //阻塞
        if (packs.size() < maxList) {
            //生产者
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
        // 取出packet 消费者
        bool bIsInput = false;
        XData pack = packs.front();
        packs.pop_front();  //把头部的数据从列表中删除

        do{
            bIsInput = this->SendPacket(pack);
            do{
                // 获取解码数据
                XData frame = RecvFrame();
                // XLOGE("frame %d",frame.size);
                if (!frame.data) break;
                // XLOGE("RecvFrame %d", frame.size);
                // 发送数据给观察者(视频播放观察者,音频播放器观察者)
                this->Notify(frame);
            } while (!isExit);
        } while (!bIsInput);

        pack.Drop();
        packsMutex.unlock();
    }
}
