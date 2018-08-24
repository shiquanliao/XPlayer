//
// Created by CC000033 on 2018/8/2.
//

#include "FFDemux.h"
#include "XLog.h"

extern "C"
{
#include <libavformat/avformat.h>
//#include<libavutil/mathematics.h>
//#include<libavutil/time.h>

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

    //使用TCP连接打开RTSP，设置最大延迟时间
    AVDictionary *avdic = NULL;
    char option_key[] = "rtsp_transport";
    char option_value[] = "tcp";
    av_dict_set(&avdic, option_key, option_value, 0);
    char option_key2[] = "max_delay";
    char option_value2[] = "5000000";
    av_dict_set(&avdic, option_key2, option_value2, 0);

    int re = avformat_open_input(&ic, url, 0, &avdic);
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
    if (ofmt) {
        ofmt = nullptr;
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

    // 推流
    if (outputContext && !isStopPush) {
        AVPacket *outPkt = av_packet_alloc();
        av_packet_ref(outPkt, pkt);
        re = av_write_frame(outputContext, outPkt);
        av_packet_unref(outPkt);
        if (re != 0) {
            char buff[1024] = {0};
            av_strerror(re, buff, sizeof(buff));
            XLOGE("av_interleaved_write_frame failed ---  %s ", buff);
        }
    }


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

bool FFDemux::OpenOutput(const char *fileName, unsigned char i) {
    mutex.lock();
    isStopPush = false;
    char *temp = const_cast<char *>("rtmp://demo.easydss.com:10085/hls/ryyv6UT8X?sign=rygJPpUaUX");
//    char *temp = const_cast<char *>("rtmp://://live-api-s.facebook.com:80/rtmp/147055689548657?ds=1&s_sw=0&s_vt=api-s&a=ATj7H3akQzXhPI3y");
//    char *temp = const_cast<char *>("rtmp://live-api-s.facebook.com:80/rtmp/146986029555623?ds=1&s_sw=0&s_vt=api-s&a=AThsGrqOMkSXn8FQ");
//    char *temp = const_cast<char *>("rtmp://://a.rtmp.youtube.com/live2/j8k7-ak0b-3sg1-4cec");
    if (i) {
        fileName = temp;
    }

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

    if (i) {
        ofmt = outputContext->oformat;
    }

    for (int i = 0; i < ic->nb_streams; ++i) {
        //根据输入流创建输出流
        AVStream *in_stream = ic->streams[i];
        AVStream *stream = avformat_new_stream(outputContext, in_stream->codec->codec);
        //将输出流的编码信息复制到输入流
        re = avcodec_copy_context(stream->codec, ic->streams[i]->codec);
        if (re < 0) {
            char buff[1024] = {0};
            av_strerror(re, buff, sizeof(buff));
            XLOGE("avcodec_copy_context failed ---  %s ", buff);
        }
        stream->codec->codec_tag = 0;
//        if (outputContext->oformat->flags & AVFMT_GLOBALHEADER)
//            stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }

    re = avformat_write_header(outputContext, nullptr);
    if (re < 0) {
        mutex.unlock();
        char buff[1024] = {0};
        av_strerror(re, buff, sizeof(buff));
        XLOGE("avio_open2 failed ---  %s ", buff);
        return false;
    }
    XLOGE("-------------path:   %s",fileName);
    XLOGE("----------------- success   output: %s-------------------------- ", fileName);

    mutex.unlock();
    return true;
}

// 开始推流
void FFDemux::StartPushStream(const char *path, unsigned char i) {
    OpenOutput(path, i);
}

// 停止推流
void FFDemux::StopPushStream() {
    mutex.lock();
    isStopPush = true;
    //写文件尾
    av_write_trailer(outputContext);
    mutex.unlock();

}
