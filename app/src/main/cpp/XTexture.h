//
// Created by CC000033 on 2018/8/6.
//

#ifndef XPLAYER_XTEXTURE_H
#define XPLAYER_XTEXTURE_H

enum XTextureType {
    XTEXTURE_YUV420P = 0,  // Y 4  u 1 v 1
    XTEXTURE_NV12 = 23,    // Y4   uv1
    XTEXTURE_NV21 = 24     // Y4   vu1
};

class XTexture {

public:
    static XTexture *Create();

    virtual bool Init(void *win,XTextureType type=XTEXTURE_YUV420P) = 0;

    virtual void Draw(unsigned char *data[], int width, int height) = 0;

};


#endif //XPLAYER_XTEXTURE_H
