#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "XLog.h"
#include "FFPlayerBuilder.h"


//IVideoView *view = NULL;
static IPlayer *player = NULL;

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    //FFDecode::InitHard(vm);
    FFPlayerBuilder::InitHard(vm);

    ////////////////////////////////////////
    // 测试代码
    player = FFPlayerBuilder::Get()->BuilderPlayer();

    player->Open("/sdcard/testFFmpeg.mp4");
    player->Start();


    ///////////////////////////////////////


    return JNI_VERSION_1_4;
}


#include <android/native_window_jni.h>

extern "C" JNIEXPORT jstring

JNICALL
Java_com_tutk_xplayer_xplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_tutk_xplayer_xplayer_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {

    // TODO
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    if (player)
        player->InitView(win);
    //view->SetRender(win);
//    XEGL::Get()->Init(win);
//    XShader shader;
//    shader.Init();


}