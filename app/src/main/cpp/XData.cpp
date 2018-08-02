//
// Created by CC000033 on 2018/8/2.
//

#include "XData.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}


void XData::Drop() {
    if (!data) return;
    av_packet_free((AVPacket **) &data);
    data = 0;
    size = 0;
}
