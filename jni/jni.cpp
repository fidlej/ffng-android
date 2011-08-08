/*
 * jni.cpp
 *
 *  Created on: 5.5.2010
 *      Author: dan
 */

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <android/log.h>
#include <signal.h>
#include "jni.h"
#include "Log.h"
#include "Application.h"
#include "HelpException.h"
#include "BaseException.h"

/*
static struct sigaction old_sa[NSIG];

void android_sigaction(int signal, siginfo_t *info, void *reserved)
{
	__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "crash");
	//(*env)->CallVoidMethod(env, obj, nativeCrashed);
	old_sa[signal].sa_handler(signal);
}
*/

extern "C"
int Java_cz_ger_ffng_FFNGApp_ffngmain(JNIEnv * env, jobject obj)
{
	/*
	// Try to catch crashes...
	struct sigaction handler;
	memset(&handler, 0, sizeof(struct sigaction));
	handler.sa_sigaction = android_sigaction;
	handler.sa_flags = SA_RESETHAND;
#define CATCHSIG(X) sigaction(X, &handler, &old_sa[X])
	CATCHSIG(SIGILL);
	CATCHSIG(SIGABRT);
	CATCHSIG(SIGBUS);
	CATCHSIG(SIGFPE);
	CATCHSIG(SIGSEGV);
	CATCHSIG(SIGSTKFLT);
	CATCHSIG(SIGPIPE);
	*/

	__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "begin");
	JNI::getInstance()->setJavaContext(env, obj);

    try {
        Application app;

        try {
            app.init(0, NULL);
            app.run();
        }
        catch (HelpException &e) {
            printf("%s\n", e.what());
        }
        catch (BaseException &e) {
            LOG_ERROR(e.info());
        }
        app.shutdown();
    	__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "OK");
        return 0;
    }
    catch (BaseException &e) {
        LOG_ERROR(e.info());
    }
    catch (std::exception &e) {
        LOG_ERROR(ExInfo("std::exception")
                .addInfo("what", e.what()));
    }
    catch (...) {
        LOG_ERROR(ExInfo("unknown exception"));
    }

	__android_log_print(ANDROID_LOG_DEBUG, "FFNG", "FAIL");
    return 1;
}

JNI* JNI::instance = NULL;

JNI::JNI()
: m_javaEnv(NULL)
, m_javaObj(NULL)
, m_javaCls(NULL)
{

}

JNI::~JNI() {
	if (m_javaCls)
		m_javaEnv->DeleteLocalRef(m_javaCls);

	if (instance)
		delete instance;
}

JNI* JNI::getInstance() {
	if (!instance)
		instance = new JNI;
	return instance;
}

void JNI::setJavaContext(JNIEnv * env, jobject obj) {
	 m_javaEnv = env;
	 m_javaObj = obj;
	 m_javaCls = m_javaEnv->GetObjectClass(m_javaObj);
}

char *JNI::getString(jobject obj, const char *fieldName)
{
	jclass cls = m_javaEnv->GetObjectClass(obj);
	jfieldID fid = m_javaEnv->GetFieldID(cls, fieldName, "Ljava/lang/String;");
	m_javaEnv->DeleteLocalRef(cls);
	if (!fid) return NULL;

	jstring jstr = (jstring)m_javaEnv->GetObjectField(obj, fid);
    const char *str = m_javaEnv->GetStringUTFChars(jstr, NULL);
    if (!str) return NULL; /* out of memory */

    char *strcopy = new char[strlen(str)+1];
    strcpy(strcopy, str);

    m_javaEnv->ReleaseStringUTFChars(jstr, str);
    return strcopy;
}

jobject JNI::getObject(jobject obj, const char *fieldName, const char *fieldType)
{
	jclass cls = m_javaEnv->GetObjectClass(obj);
	jfieldID fid = m_javaEnv->GetFieldID(cls, fieldName, fieldType);
	m_javaEnv->DeleteLocalRef(cls);
	if (!fid) return NULL;

	jobject object = m_javaEnv->GetObjectField(obj, fid);
    return object;
}

int JNI::getInt(jobject obj, const char *fieldName)
{
	jclass cls = m_javaEnv->GetObjectClass(obj);
	jfieldID fid = m_javaEnv->GetFieldID(cls, fieldName, "I");
	m_javaEnv->DeleteLocalRef(cls);
	if (!fid) return 0;

	return m_javaEnv->GetIntField(obj, fid);
}

float JNI::getFloat(jobject obj, const char *fieldName)
{
	jclass cls = m_javaEnv->GetObjectClass(obj);
	jfieldID fid = m_javaEnv->GetFieldID(cls, fieldName, "F");
	m_javaEnv->DeleteLocalRef(cls);
	if (!fid) return 0;

	return m_javaEnv->GetFloatField(obj, fid);
}

bool JNI::getBoolean(jobject obj, const char *fieldName)
{
	jclass cls = m_javaEnv->GetObjectClass(obj);
	jfieldID fid = m_javaEnv->GetFieldID(cls, fieldName, "Z");
	m_javaEnv->DeleteLocalRef(cls);
	if (!fid) return 0;

	return m_javaEnv->GetBooleanField(obj, fid);
}

int JNI::getArrayLength(jarray arr)
{
	return m_javaEnv->GetArrayLength(arr);
}

jobject JNI::getObjectArrayElement(jobjectArray objarr, int index)
{
	return m_javaEnv->GetObjectArrayElement(objarr, index);
}

jobjectArray JNI::getObjectArray(jobject obj, const char *getMethod, const char *type)
{
	static const int bufferSize = 256;
	char *returnType = new char[bufferSize];
	snprintf(returnType, bufferSize-1, "()[L%s;", type);
	jmethodID mid = NULL;

    jclass refClass = m_javaEnv->GetObjectClass(obj);
	mid = m_javaEnv->GetMethodID(refClass, getMethod, returnType);
    m_javaEnv->DeleteLocalRef(refClass);

	if (mid == NULL) {
		assert("method not found");
		return NULL;
	}

	return (jobjectArray)m_javaEnv->CallObjectMethod(obj, mid);
}
