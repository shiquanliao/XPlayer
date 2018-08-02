#include <jni.h>
#include <string>
#include "FFDemux.h"




extern "C" JNIEXPORT jstring

JNICALL
Java_com_tutk_xplayer_xplayer_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    ////////////////////////////////////////
    // 测试代码
    IDemux *de = new FFDemux();
    de->Open("/sdcard/testFFmpeg.mp4");
    for (;;) {
        XData data = de->Read();
    }
    ///////////////////////////////////////



    return env->NewStringUTF(hello.c_str());
}
