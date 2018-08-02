//
// Created by CC000033 on 2018/8/2.
//

#ifndef XPLAYER_XDATA_H
#define XPLAYER_XDATA_H


struct XData {
    unsigned char *data = 0;
    int size = 0;

    void Drop();
};

#endif //XPLAYER_XDATA_H
