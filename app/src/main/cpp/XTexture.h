//
// Created by CC000033 on 2018/8/6.
//

#ifndef XPLAYER_XTEXTURE_H
#define XPLAYER_XTEXTURE_H


class XTexture {

public:
    static XTexture *Create();

    virtual bool Init(void *win) = 0;

    virtual void Draw(unsigned char *data[], int width, int height) = 0;

};


#endif //XPLAYER_XTEXTURE_H
