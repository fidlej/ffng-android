#ifndef JNI_H_
#define JNI_H_

#include <jni.h>

/* android interface singleton */
class JNI {
public:
	static JNI* getInstance();

	/**
	 * Stores java context.
	 * @param env java environment
	 * @param obj java parent, the parent object/class
	 */
	void setJavaContext(JNIEnv * env, jobject obj);
	JNIEnv *getJavaEnv() const { return m_javaEnv; }
	jobject getJavaObj() const { return m_javaObj; }
	jclass getJavaCls() const { return m_javaCls; }


	char *getString(jobject obj, const char *fieldName);
	jobject getObject(jobject obj, const char *fieldName, const char *fieldType);
	int getInt(jobject obj, const char *fieldName);
	float getFloat(jobject obj, const char *fieldName);
	bool getBoolean(jobject obj, const char *fieldName);
	int getArrayLength(jarray arr);
	jobject getObjectArrayElement(jobjectArray objarr, int index);
	jobjectArray getObjectArray(jobject obj, const char *getMethod, const char *type);

private:
	static JNI* instance;

	JNI();
	~JNI();

	/**
	 * Java context
	 */
	JNIEnv * m_javaEnv;
	jobject m_javaObj;
	jclass m_javaCls; // can be computed from m_javaObj
};

#endif /* JNI_H_ */
