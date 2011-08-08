#include <assert.h>
#include "jni.h"
#include "FFNGFiles.h"
#include <android/log.h>

bool FFNGFiles::exists(const std::string &path, int type)
{
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGFiles");
		mid = javaEnv->GetStaticMethodID(cls, "exists", "(Ljava/lang/String;I)Z");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGFiles::exists 1 %p %p %p", javaEnv, cls, mid);

	if (mid == NULL) {
		assert("method not found");
		return false;
	}

	jstring pathString = javaEnv->NewStringUTF(path.c_str());

    bool result = javaEnv->CallStaticBooleanMethod(cls, mid, pathString, type);

    javaEnv->DeleteLocalRef(pathString);

    return result;
}

std::string FFNGFiles::read(const std::string &path, int type) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGFiles");
		mid = javaEnv->GetStaticMethodID(cls, "read", "(Ljava/lang/String;I)Ljava/lang/String;");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGFiles::read 1 %p %p %p", javaEnv, cls, mid);

	jstring pathString = javaEnv->NewStringUTF(path.c_str());

	jstring resultString = (jstring)javaEnv->CallStaticObjectMethod(cls, mid, pathString, type);

	jboolean isCopy;
	const char *cstr = javaEnv->GetStringUTFChars(resultString,	&isCopy);
    std::string result(cstr);

    javaEnv->DeleteLocalRef(pathString);
    javaEnv->DeleteLocalRef(resultString);
    if (isCopy) delete cstr;

    //__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGFiles::read 2 %s", result.c_str());
    return result;
}

void FFNGFiles::createPath(const std::string &path) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGFiles");
		mid = javaEnv->GetStaticMethodID(cls, "createPath", "(Ljava/lang/String;)V");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGFiles::createPath 1 %p %p %p", javaEnv, cls, mid);

	jstring pathString = javaEnv->NewStringUTF(path.c_str());

	javaEnv->CallStaticVoidMethod(cls, mid, pathString);

	javaEnv->DeleteLocalRef(pathString);
}

bool FFNGFiles::write(const std::string &path, const std::string &data) {
	static JNIEnv *javaEnv = NULL;
	static jclass cls = NULL;
	static jmethodID mid = NULL;

	if (!mid) {
		javaEnv = JNI::getInstance()->getJavaEnv();
		cls = javaEnv->FindClass("cz/ger/ffng/FFNGFiles");
		mid = javaEnv->GetStaticMethodID(cls, "write", "(Ljava/lang/String;Ljava/lang/String;)Z");
	}
	//__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FFNGFiles::write 1 %p %p %p", javaEnv, cls, mid);

	jstring pathString = javaEnv->NewStringUTF(path.c_str());
	jstring dataString = javaEnv->NewStringUTF(data.c_str());

	bool result = javaEnv->CallStaticBooleanMethod(cls, mid, pathString, dataString);

	javaEnv->DeleteLocalRef(pathString);
	javaEnv->DeleteLocalRef(dataString);

	return result;
}
