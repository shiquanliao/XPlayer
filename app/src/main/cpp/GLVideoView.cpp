//
// Created by CC000033 on 2018/8/6.
//

#include "GLVideoView.h"
#include "XTexture.h"

void GLVideoView::SetRender(void *win) {
    view = win;
}

void GLVideoView::Render(XData data) {
    if (!view) return;
    if (!txt) {
        txt = XTexture::Create();
        txt->Init(view, static_cast<XTextureType>(data.format));
    }
    txt->Draw(data.datas, data.width, data.height);


}

void GLVideoView::Close() {
    mutex.lock();
    if (txt) {
        txt->Drop();
        txt = 0;
    }
    mutex.unlock();
}
