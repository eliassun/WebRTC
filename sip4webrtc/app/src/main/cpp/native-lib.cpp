#include <jni.h>
#include <string>

#include "wrapper/include/logger.h"

JavaVM* gvm = NULL;


extern "C" JNIEXPORT jstring JNICALL
Java_com_voipeye_sip4webrtc_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    if (gvm == NULL) {
        env->GetJavaVM(&gvm);
    }
    espjlog(6, "file", 1, "func", "tag", "desc", 400, "from c++ to java");
    return env->NewStringUTF(hello.c_str());
}