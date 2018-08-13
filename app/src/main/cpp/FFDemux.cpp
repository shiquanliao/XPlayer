//
// Created by CC000033 on 2018/8/2.
//

#include "FFDemux.h"
#include "XLog.h"

extern "C"
{
#include <libavformat/avformat.h>
}

//分数转为浮点数
static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0. : (double) r.num / (double) r.den;
}


//打开文件，或者流媒体 rmtp http rtsp
bool FFDemux::Open(const char *url) {
    XLOGI("Open file %s begin", url);
    Close();
    mutex.lock();
    int re = avformat_open_input(&ic, url, 0, 0);
    if (re != 0) {
        mutex.unlock();
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        XLOGE("FFDemux open %s failed!", url);
        return false;
    }

    XLOGI("FFDemux open %s success!", url);

    //读取文件信息
    re = avformat_find_stream_info(ic, 0);
    if (re != 0) {
        mutex.unlock();
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        XLOGE("avformat_find_stream_info %s failed!", url);
        return false;
    }
    this->totalMs = ic->duration / (AV_TIME_BASE / 1000);

    mutex.unlock();
    XLOGI("total ms = %d !", totalMs);

    GetVPara();
    GetAPara();
    return true;
}

void FFDemux::Close() {
    mutex.lock();
    if (ic) {
        avformat_close_input(&ic);
    }
    if (outputContext) {
        avformat_free_context(outputContext);
    }
    mutex.unlock();
}

//读取一帧数据，数据由调用者清理
XData FFDemux::Read() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        return XData();
    }
    XData data;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if (re != 0) { // 读取帧失败
        av_packet_free(&pkt);
        mux.unlock();
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
        mux.unlock();
        return XData();
    }

    //转换pts
    pkt->pts = pkt->pts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    pkt->dts = pkt->dts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    data.pts = (int) pkt->pts;
    //XLOGE("demux pts %d", data.pts);
//    if (outputContext) {
//        av_interleaved_write_frame(outputContext, pkt);
//    }

    mux.unlock();
    return data;
}

XParameter FFDemux::GetVPara() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        XLOGE("GetVPara failed! ic is NULL！");
        return XParameter();
    }
    //获取了视频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    mux.unlock();
    return para;
}

XParameter FFDemux::GetAPara() {
    mux.lock();
    if (!ic) {
        mux.unlock();
        XLOGE("GetVPara failed! ic is NULL！");
        return XParameter();
    }
    //获取了音频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        XLOGE("av_find_best_stream failed!");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    mux.unlock();
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

bool FFDemux::OpenOutput(char *fileName) {
    mutex.lock();
    char *temp = const_cast<char *>("rtmp://www.easydss.com:10085/live/librtmp");
    fileName = temp;

    //创建输出上下文
    int re = 0;
    re = avformat_alloc_output_context2(&outputContext, nullptr, "flv", fileName);
    if (re < 0) {
        mutex.unlock();
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        XLOGE("avformat_alloc_output_context2 failed ---  %s ", buff);
        return false;
    }
    re = avio_open2(&outputContext->pb, fileName, AVIO_FLAG_READ_WRITE, nullptr, nullptr);
    if (re < 0) {
        mutex.unlock();
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        XLOGE("avio_open2 failed ---  %s ", buff);
        return false;
    }

//    for (int i = 0; i < ic->nb_streams; ++i) {
//        AVStream *stream = avformat_new_stream(outputContext, nullptr);
//        re = avcodec_parameters_to_context(stream->codec, context->streams[i]->codec);
//    }

    re = avformat_write_header(outputContext, nullptr);
    if (re < 0) {
        mutex.unlock();
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        XLOGE("avio_open2 failed ---  %s ", buff);
        return false;
    }

    mutex.unlock();
    return true;
}
