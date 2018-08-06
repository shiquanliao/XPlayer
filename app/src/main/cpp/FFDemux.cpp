//
// Created by CC000033 on 2018/8/2.
//

#include "FFDemux.h"
#include "XLog.h"

extern "C"
{
#include <libavformat/avformat.h>
}

//打开文件，或者流媒体 rmtp http rtsp
bool FFDemux::Open(const char *url) {
    XLOGI("Open file %s begin", url);
    int re = avformat_open_input(&ic, url, 0, 0);
    if (re != 0) {
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        XLOGE("FFDemux open %s failed!", url);
        return false;
    }
    GetVPara();
    GetAPara();
    XLOGI("FFDemux open %s success!", url);

    //读取文件信息
    re = avformat_find_stream_info(ic, 0);
    if (re != 0) {
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        XLOGE("avformat_find_stream_info %s failed!", url);
        return false;
    }
    this->totalMs = ic->duration / (AV_TIME_BASE / 1000);
    XLOGI("total ms = %d !", totalMs);

    return true;
}

//读取一帧数据，数据由调用者清理
XData FFDemux::Read() {
    if (!ic)
        return XData();
    XData data;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if (re != 0) { // 读取帧失败
        av_packet_free(&pkt);
        return XData();
    }
    //XLOGI("pack size is %d , pts is %lld ", pkt->size, pkt->pts);
    data.data = reinterpret_cast<unsigned char *>(pkt);
    data.size = pkt->size;
    if (pkt->stream_index == AVMEDIA_TYPE_AUDIO) {
        data.isAudio = true;
    } else if (pkt->stream_index == AVMEDIA_TYPE_VIDEO) {
        data.isAudio = false;
    } else { //销毁数据, 防止内存泄漏
        av_packet_free(&pkt);
        return XData();
    }
    return data;
}

XParameter FFDemux::GetVPara() {
    if (!ic) {
        XLOGE("GetVPara failed! ic is NULL！");
        return XParameter();
    }
    //获取了视频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    return para;
}

XParameter FFDemux::GetAPara() {
    if (!ic) {
        XLOGE("GetVPara failed! ic is NULL！");
        return XParameter();
    }
    //获取了音频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    return para;
}

FFDemux::FFDemux() {
    static bool isFirst = true;
    if (isFirst) {
        isFirst = false;
        //注册所有封装器 -- 4.0以上废弃
        //av_register_all();

        //注册所有的解码器
        //avcodec_register_all();

        //初始化网络
        avformat_network_init();
        XLOGE("register ffmpeg!");
    }
}
