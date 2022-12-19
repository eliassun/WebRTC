//
// Created by Elias Sun on 12/01/22.
//

#ifndef SIP4WEBRTC_LOGGER_CPP
#define SIP4WEBRTC_LOGGER_CPP

#include <string>
#include "./include/logger.h"
#include "./include/sip.h"

#if defined(__ANDROID__)
#include <jni.h>
#include <android/log.h>
#define APP_NAME_PATH "com/voipeye/sip4webrtc"

JavaVM* gjvm;

jmethodID	gjavaLog = NULL;
jclass		gjavaLogClass = NULL;

#define LOGCATWARN(...) __android_log_print(ANDROID_LOG_WARN, SIPTAG, __VA_ARGS__)

jclass getJavaLogClass() {
    if (NULL == gjavaLogClass) {
        JNIEnv *jni_env;
        int env_status = gjvm->GetEnv((void **)&jni_env, JNI_VERSION_1_6);
        if( env_status == JNI_EDETACHED )
            jint attachResult = gjvm->AttachCurrentThread(&jni_env , NULL );
        std::string cls_name = std::string(APP_NAME_PATH) + "/AppLog";
        jclass tmp =NULL;
        tmp = jni_env->FindClass(cls_name.c_str());
        if (tmp == NULL) {
            gjavaLogClass = NULL;
            return NULL;
        }
        gjavaLogClass = (jclass)jni_env->NewGlobalRef(tmp);
        if (env_status == JNI_EDETACHED){
            gjvm->DetachCurrentThread();
        }
    }

    if (NULL == gjavaLogClass) {
        LOGCATWARN("AppLog is not defined in Java App");
    }
    return gjavaLogClass;
}

jmethodID getJavaLog() {
    jclass cls = NULL;
    if (NULL != (cls = getJavaLogClass())) {
        if (gjavaLog == NULL) {
            JNIEnv *jni_env;
	        int env_status = gjvm->GetEnv((void **)&jni_env, JNI_VERSION_1_6);
	        if(env_status == JNI_EDETACHED)
		        jint attachResult = gjvm->AttachCurrentThread(&jni_env , NULL );
            gjavaLog  = jni_env->GetStaticMethodID(cls, "logToJava", "(IILjava/lang/String;)V");
            if (env_status == JNI_EDETACHED){
                gjvm->DetachCurrentThread();
            }
        }
        if (gjavaLog == NULL) {
            LOGCATWARN("logToJava func is not defined in Java App");
        }
    }
    return gjavaLog;
}

void addlog(int level, const char *file, int line, const char *func, const char *tag, const char *desc,
                 int code, char *info)
{
    if (level == LOG_LEVEL_LOCAL) {
        __android_log_print(ANDROID_LOG_INFO, tag, "%s", info);
        return;
    }

    jmethodID method = NULL;
    jclass cls = NULL;

    method = getJavaLog();
    cls = getJavaLogClass();
    if (method && cls) {
        JNIEnv *jni_env;
        int env_status = gjvm->GetEnv((void **)&jni_env, JNI_VERSION_1_6);
        if( env_status == JNI_EDETACHED )
            jint attachResult = gjvm->AttachCurrentThread(&jni_env , NULL );
        jstring msg = jni_env->NewStringUTF(info);
        switch(level) {
            case 5:
                jni_env->CallStaticVoidMethod(cls, method, 0, 4, msg);
                break;
            case 4:
                jni_env->CallStaticVoidMethod(cls, method, 0, 4, msg);
                break;
            case 3:
                jni_env->CallStaticVoidMethod(cls, method, 0, 0, msg);
                break;
            case 2:
                jni_env->CallStaticVoidMethod(cls, method, 0, 2, msg);
                break;
            default:
                jni_env->CallStaticVoidMethod(cls, method, 0, 1, msg);
        }
        jni_env->DeleteLocalRef(msg);
        if (env_status == JNI_EDETACHED){
            gjvm->DetachCurrentThread();
        }
    }
}
#endif

#endif //SIP4WEBRTC_LOGGER_CPP
