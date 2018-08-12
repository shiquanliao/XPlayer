#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "IPlayProxy.h"
#include <android/native_window_jni.h>

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {

    ////////////////////////////////////////
    // 测试代码
    IPlayProxy::Get()->Init(vm);
//    IPlayProxy::Get()->Open("/sdcard/testFFmpeg.mp4");
//    IPlayProxy::Get()->Start();

    ///////////////////////////////////////
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tutk_xplayer_xplayer_XPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {

    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    IPlayProxy::Get()->InitView(win);


}

//extern "C" JNIEXPORT jstring
//
//JNICALL
//Java_com_tutk_xplayer_xplayer_MainActivity_stringFromJNI(
//        JNIEnv *env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}

extern "C"
JNIEXPORT void JNICALL
Java_com_tutk_xplayer_xplayer_OpenUrlActivity_Open(JNIEnv *env, jobject instance, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);

    // TODO
    IPlayProxy::Get()->Open(url);
    IPlayProxy::Get()->Start();

    env->ReleaseStringUTFChars(url_, url);
}