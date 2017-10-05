#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>

#define LOG_TAG "mytag"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__) //"__VA_ARGS__"是固定写法，表示可变参数。和 "..."意思是一样的
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

/**
 * 获取ABI类型
 */
void getABI(){
	#if defined(__arm__)
	  #if defined(__ARM_ARCH_7A__)
		#if defined(__ARM_NEON__)
		  #if defined(__ARM_PCS_VFP)
			#define ABI "armeabi-v7a/NEON (hard-float)"
		  #else
			#define ABI "armeabi-v7a/NEON"
		  #endif
		#else
		  #if defined(__ARM_PCS_VFP)
			#define ABI "armeabi-v7a (hard-float)"
		  #else
			#define ABI "armeabi-v7a"
		  #endif
		#endif
	  #else
	   #define ABI "armeabi"
	  #endif
	#elif defined(__i386__)
	   #define ABI "x86"
	#elif defined(__x86_64__)
	   #define ABI "x86_64"
	#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
	   #define ABI "mips64"
	#elif defined(__mips__)
	   #define ABI "mips"
	#elif defined(__aarch64__)
	   #define ABI "arm64-v8a"
	#else
	   #define ABI "unknown"
	#endif
}


/**
 * 把一个jstring转换成一个c语言的char* 类型.
 * 本质上就是在C中反射调用java中String类型的getBytes(String)方法，转成byte数组。因为C中的char类型占一个字节
 * 然后申请内存，把byte数组拷贝到char*中，
 * 最后返回
 */
char* _JString2CStr(JNIEnv* env, jstring jstr) {
	 char* rtn = NULL;
	 jclass clsstring = (*env)->FindClass(env, "java/lang/String");//获取java 中String类的字节码对象
	 jstring strencode = (*env)->NewStringUTF(env,"GB2312"); // 把char* 类型的字符串 "GB2312" 转成 jstring 类型
	 jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes", "(Ljava/lang/String;)[B"); //获取getBytes(String)方法
	 jbyteArray barr = (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid, strencode); // String .getByte("GB2312");返回类型是byte[], 也是Object, 所以使用CallObjectMethod方法
	 jsize alen = (*env)->GetArrayLength(env, barr);// 获取byte[] 长度
	 jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);//获取 byte数组首地址。jbyte就是char
	 if(alen > 0) {
		rtn = (char*)malloc(alen+1); //"\0"
		memcpy(rtn, ba, alen); // 进行内存拷贝. 把字节数组拷贝到char*中
		rtn[alen]=0; //最后一个为数组结束符
	 }
	 (*env)->ReleaseByteArrayElements(env, barr, ba, 0);//释放byte数组的内存
	 return rtn;
}


/*
 * Class:     com_example_jnidemo_JNI
 * Method:    add
 * Signature: (II)I
 *
 * 有形参：2个int, 返回int
 */
JNIEXPORT jint JNICALL Java_com_example_jnidemo_JNI_add
  (JNIEnv * env, jobject obj, jint i, jint j){

	// 因为 java中的int(jint) 和 C 中的int都是占4字节,所以不需要转换
	return i+j;
}

/*
 * Class:     com_example_jnidemo_JNI
 * Method:    hello
 * Signature: ()Ljava/lang/String;
 *
 * 无形参，返回String
 */
JNIEXPORT jstring JNICALL Java_com_example_jnidemo_JNI_hello
  (JNIEnv * env, jobject obj){

	getABI();
	// 用c定义一个字符串
	char* cstr = "Hello from JNI !  Compiled with ABI " ABI "."; //必须要在getABI();执行否则 ABI 没有值

	//return cstr; // 不能直接把字符串返回。因为函数的返回类型是jstring,本质上就是c中的 void* 类型, 而cstr是char*类型，所以类型不匹配
	//jstring  (*NewStringUTF)(JNIEnv*, const char*);
	return (*env) -> NewStringUTF(env, cstr); // C中的env是二级指针
}

/*
 * Class:     com_example_jnidemo_JNI
 * Method:    hello2
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 *
 * 把传递进来的String 中的每个字符加1之后，再返回
 */
JNIEXPORT jstring JNICALL Java_com_example_jnidemo_JNI_hello2
  (JNIEnv * env, jobject clazz, jstring jstr){
	// 调用工具方法，把java中的String转成C中的char*
	char * cstr = _JString2CStr(env, jstr);

	//extern size_t strlen(const char *);
	int len = strlen(cstr);//获取cstr的长度

	int i; // 必须要在for外部声明
	for(i = 0; i<len; i++){
		*(cstr+i) += 1;
	}

	//jstring     (*NewStringUTF)(JNIEnv*, const char*);
	return (*env)->NewStringUTF(env, cstr);//把char*转成java中的String类型，并返回
}

/*
 * Class:     com_example_jnidemo_JNI
 * Method:    changeArr
 * Signature: ([I)[I
 *
 * 传递jintArray，每个元素加10，然后再把这个jintArray返回。本质上就是对这个jintArray的修改，并没有涉及到jintArray类型的创建与转换
 * 注意： jintArray 和  jint[] 类型是不一样的。
 */
JNIEXPORT jintArray JNICALL Java_com_example_jnidemo_JNI_changeArr
  (JNIEnv * env, jobject clazz, jintArray jarray){

	//jsize (*GetArrayLength)(JNIEnv*, jarray); //typedef jint  jsize;  // jsize,jint,int都是一样的
	int len = (*env)->GetArrayLength(env, jarray); // 获取java数组的长度

	//向logcat中输出日志 下面两种方式本质上是一样的。LOGD()是对__android_log_print的封装，写法更简单
	LOGD("len=%d\n", len);
	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "test = %d", len);//直接调用log.h中声明的原型方法

	jboolean isCopy;
	//jint*  (*GetIntArrayElements)(JNIEnv*, jintArray, jboolean*); //typedef unsigned char   jboolean;  //所以java中的boolean和C中的char都是占用1个字节的
	//(*env)->GetIntArrayElements(env, jarray, &isCopy); // 第三个参数是jboolean*类型，调用GetIntArrayElements()时会自动给isCopy赋值 0或1，表示是否生成了一份数组的拷贝。我们一般不关注这个结果，可以直接传递NULL值

	jint* p = (*env)->GetIntArrayElements(env, jarray, NULL);

	int i;
	for(i = 0; i<len; i++){
		*(p+i) +=10;
	}

	//int myarr[] = { 5, 6, 7, 8 }; // 若是直接返回myarr，则程序会崩溃。说明jintArray 和 jint[]不是一样的。
	return jarray;
}


//=======================C中反射调用java中的方法========================


/*
 * Class:     com_example_jnidemo_JNI
 * Method:    callBackHelloFromJava
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_jnidemo_JNI_callBackHelloFromJava
  (JNIEnv * env, jobject obj){

	/**
	 * 1.获取字节码对象
	 * jclass (*FindClass)(JNIEnv*, const char*);
	 *   参2 --> 要回调的java方法所在的类路径. 即把类的全限定名中的  "." 换成 "/"即可
	 */
	jclass clazz = (*env)->FindClass(env, "com/example/jnidemo/JNI");

	/**
	 * 2.获取method对象
	 * jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
	 *   参2 --> 字节码对象
	 *   参3 --> 方法名
	 *   参4 --> 方法签名(因为java有重载方法,单独依靠方法名不能唯一确定是哪一个方法. 而这个方法签名可以通过javap -s 包名.类名查看(bin/classes运行),  javah 生成头文件中的Signature不是准确的)
	 */
	jmethodID methodID = (*env)->GetMethodID(env, clazz, "helloFromJava", "()V");

	/**
	 * 3.通过字节码创建一个Object
	 * 此步骤暂时不需要,因为helloFromJava()方法刚好就在本地方法调用者JNI对象中, 而调用者就是形参中的jobject
	 * 根据字节码创建对象：jobject (*AllocObject)(JNIEnv*, jclass); 相当于java中的 clazz.newInstance()
	 */

	/**
	 * 4.通过对象调用方法
	 * void (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...); 调用返回类型为void的方法
	 *   参2 --> 方法调用者对象
	 *   参3 --> method对象
	 *   参4 --> 可变参数, 就是形参列表. 无形参时，可不传
	 */
	(*env)->CallVoidMethod(env, obj, methodID);
}

/*
 * Class:     com_example_jnidemo_JNI
 * Method:    callbackPlus
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_jnidemo_JNI_callbackPlus
  (JNIEnv * env, jobject obj){

	//jclass      (*FindClass)(JNIEnv*, const char*);
	jclass clazz = (*env)->FindClass(env, "com/example/jnidemo/JNI");

	//jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
	jmethodID methodID = (*env)->GetMethodID(env, clazz, "plus", "(II)I");

	//jint        (*CallIntMethod)(JNIEnv*, jobject, jmethodID, ...);
	jint total = (*env)->CallIntMethod(env, obj, methodID, 3, 4);

	LOGD("total = %d", total);
}


/*
 * Class:     com_example_jnidemo_JNI
 * Method:    callbackStringMethod
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_jnidemo_JNI_callbackStringMethod
  (JNIEnv * env, jobject obj){
		//jclass      (*FindClass)(JNIEnv*, const char*);
		jclass clazz = (*env)->FindClass(env, "com/example/jnidemo/JNI");

		//jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);
		jmethodID methodID = (*env)->GetMethodID(env, clazz, "printString", "(Ljava/lang/String;)V");

		//void        (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
		//jstring     (*NewStringUTF)(JNIEnv*, const char*);
		jstring result = (*env)->NewStringUTF(env, "printString --> hello");
		(*env)->CallVoidMethod(env, obj, methodID, result);// 这里不能直接写 (*env)->NewStringUTF(); 必须先用result来接收
}

/*
 * Class:     com_example_jnidemo_JNI
 * Method:    callbackShowToast
 * Signature: ()V
 *
 * 实现JNI.java类中的native方法，然后反射调用MainActivity中showToast()的方法. showToast()方法不在调用者这个类中
 */
JNIEXPORT void JNICALL Java_com_example_jnidemo_JNI_callbackShowToast
  (JNIEnv * env, jobject obj){

	//jclass clazz = (*env)->FindClass(env, "com/example/jnidemo/MainActivity;");
	jclass clazz = (*env)->FindClass(env, "com/example/jnidemo/JNI");
	jmethodID methodID = (*env)->GetMethodID(env, clazz, "showToast", "(Ljava/lang/String;)V");

	/**
	 * 此时的问题就是：如何获取反射方法showToast(String)所在类的对象？
	 * jobject  (*AllocObject)(JNIEnv*, jclass); 作用就是创建字节码所对应的对象。
	 * 但是注意：此时传递进来的obj是JNI对应的对象，而showToast(String)方法是位于MainActivity中的并且这个方法内部要使用Context上下文
	 * 此时要调用showToast()方法就必须要获取MainActivity对象，直接使用AllocObject()是肯定不行的，因为Activity对象是系统创建的，我们这样直接创建的Activity连Context都不是，肯定不行！
	 *
	 * 那么如何解决呢？
	 * 解决方法：还是把showToast(String)方法声明在JNI类中，然后在MainActivity中创建JNI对象时，传递Context对象
	 * 1. 反射调用的方法就处于调用者JNI类中，使得传递过来的obj就是反射方法所在的对象
	 * 2. showToast()方法中也可以使用传递过来的context弹出信息，而不报错
	 */
	//jobject object = (*env)->AllocObject(env, clazz);
	jstring msg = (*env)->NewStringUTF(env, "my toast msg!");
	(*env)->CallVoidMethod(env, obj, methodID, msg);
}

//==============压力表================

int getPressure(){
	return rand()%100;
}
int flag = 0;
JNIEXPORT void JNICALL Java_com_example_jnidemo_PressureActivity_startMoniter
  (JNIEnv * env, jobject obj){
	flag = 1;
	while(flag){
		sleep(1);
		//拿到字节码对象
		jclass clazz =(*env)->FindClass(env,"com/example/jnidemo/PressureActivity");
		jmethodID methodID = (*env)->GetMethodID(env,clazz,"setPressure","(I)V");
		(*env)->CallVoidMethod(env,obj,methodID,getPressure());
	}

}

JNIEXPORT void JNICALL Java_com_example_jnidemo_PressureActivity_stopMoniter
  (JNIEnv * env, jobject obj){
	flag = 0;
}

