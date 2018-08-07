//
// Created by CC000033 on 2018/8/3.
//

#include "FFDecode.h"
#include "XLog.h"

extern "C"
{
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
#include <libswscale/swscale.h>
}


void FFDecode::InitHard(void *vm) {
    av_jni_set_java_vm(vm, 0);
}

bool FFDecode::Open(XParameter para, bool isHard) {
    if (!para.para) return false;
    AVCodecParameters *p = para.para; // 弄清楚从哪里过来
    //1 查找解码器
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if (isHard) {
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    }
    if (!cd) {
        XLOGE("avcodec_find_decoder %d failed -- %d", p->codec_id, isHard);
        return false;
    }
    XLOGI("avcodec_find_decoder success! --- %d", isHard);
    //2 创建解码上下文，并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);

    codec->thread_count = 8;
    //3 打开解码器
    int re = avcodec_open2(codec, 0, 0);
    if (re != 0) {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf) - 1);
        XLOGE("avcodec_open2 failed because : %s", buf);
        return false;
    }
    XLOGI("avcodec_open2 success");

    if (codec->codec_type == AVMEDIA_TYPE_AUDIO) {
        this->isAudio = true;
    } else {
        this->isAudio = false;
    }
    return true;
}

bool FFDecode::SendPacket(XData pkt) {
    if (pkt.size <= 0 || !pkt.data) {
        return false;
    }
    if (!codec) {
        return false;
    }
    int re = avcodec_send_packet(codec, (AVPacket *) pkt.data);
    if (re != 0) {
        XLOGE("decode msg:[%s].", av_err2str(re));
        return false;
    }
    return true;
}

XData FFDecode::RecvFrame() {
    if(!codec)
    {
        return XData();
    }
    if(!frame)
    {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codec,frame);
    if(re != 0)
    {
        return XData();
    }
    XData d;
    d.data = (unsigned char *)frame;
    if(codec->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2])*frame->height;
        d.width = frame->width;
        d.height = frame->height;
    }
    else
    {
        //样本字节数 * 单通道样本数 * 通道数
        d.size = av_get_bytes_per_sample((AVSampleFormat)frame->format)*frame->nb_samples*2;
    }
    d.format = frame->format;
    if(!isAudio)
        XLOGE("data format is %d",frame->format);
    memcpy(d.datas,frame->data,sizeof(d.datas));

    return d;
}
