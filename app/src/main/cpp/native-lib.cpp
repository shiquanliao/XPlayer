#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "XLog.h"


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
    de->Start();
    XSleep(4000);
    de->Stop();


//    for (;;) {
//        XData data = de->Read();
//        XLOGI("Read packet data size is %d",data.size);
//    }
    ///////////////////////////////////////



    return env->NewStringUTF(hello.c_str());
}
