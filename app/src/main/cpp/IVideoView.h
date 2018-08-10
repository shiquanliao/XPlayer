//
// Created by CC000033 on 2018/8/6.
//

#ifndef XPLAYER_IVIDEOVIEW_H
#define XPLAYER_IVIDEOVIEW_H


#include "IObserver.h"

class IVideoView : public IObserver {
public:
    virtual void SetRender(void *win) = 0;

    virtual void Render(XData data) = 0;

    virtual void Close() = 0;

    virtual void Update(XData data);

};


#endif //XPLAYER_IVIDEOVIEW_H
