#include <jni.h>
#include <string>
#include "FFDemux.h"
#include "XLog.h"
#include "IDecode.h"
#include "FFDecode.h"


class TestObs:public IObserver {
public:
    void Update(XData data){
        //XLOGI("TestObs Updata size is %d", data.size);
    }
};

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
    vDecode->Open(de->GetAPara());

    de->AddObs(vDecode);
    de->AddObs(aDecode);

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
