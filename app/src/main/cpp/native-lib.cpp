#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "XLog.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "XEGL.h"
#include "XShader.h"
#include "IVideoView.h"
#include "GLVideoView.h"
#include "FFResample.h"
#include "SLAudioPlay.h"
#include <android/native_window_jni.h>


IVideoView *view = NULL;

extern "C" JNIEXPORT jstring

JNICALL
Java_com_tutk_xplayer_xplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    ////////////////////////////////////////
    // 测试代码
//    TestObs *obs = new TestObs();
    IDemux *de = new FFDemux();
    de->Open("/sdcard/testFFmpeg.mp4");

    IDecode *vDecode = new FFDecode();
    vDecode->Open(de->GetVPara());

    IDecode *aDecode = new FFDecode();
    aDecode->Open(de->GetAPara());

    de->AddObs(vDecode);
    de->AddObs(aDecode);

    view = new GLVideoView();
    vDecode->AddObs(view);

    IResample *resample = new FFResample();
    XParameter outPara = de->GetAPara();

    resample->Open(de->GetAPara(),outPara);
    aDecode->AddObs(resample);

    IAudioPlay *audioPlay = new SLAudioPlay();
    audioPlay->StartPlay(outPara);
    resample->AddObs(audioPlay);

    //de->AddObs(obs);
    de->Start();
    vDecode->Start();
    aDecode->Start();

//    XSleep(3000);
//    de->Stop();


//    for (;;) {
//        XData data = de->Read();
//        XLOGI("Read packet data size is %d",data.size);
//    }
    ///////////////////////////////////////



    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_tutk_xplayer_xplayer_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {

    // TODO
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    view->SetRender(win);
//    XEGL::Get()->Init(win);
//    XShader shader;
//    shader.Init();


}