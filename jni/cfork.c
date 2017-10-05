#include <jni.h>
#include <stdio.h>
#include<unistd.h>
#include <android/log.h>

#define TAG "mytag" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型  __VA_ARGS__ 是固定写法，表示可变参数。用法和printf()是一样的。
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

JNIEXPORT void JNICALL Java_com_example_jnidemo_JNI_cfork
  (JNIEnv * env, jobject obj){

	/**
	 * fork()函数特点：
	 *  1. 在语句pid=fork()之前，只有一个进程在执行这段代码，但在这条语句之后，就变成两个进程在执行了，这两个进程的几乎完全相同，将要执行的下一条语句都是if语句
	 *  2. 在fork函数执行完毕后，如果创建新进程成功，则出现两个进程，一个是子进程，一个是父进程。在子进程中，fork函数返回0，在父进程中，fork返回新创建子进程的进程ID。我们可以通过fork返回的值来判断当前进程是子进程还是父进程。
	 *    fork()有一个特性就是：它仅仅被调用一次，却能够返回两次(两个进程)，它可能有三种不同的返回值：
	 *    1）在父进程中，fork返回新创建子进程的进程ID；
	 *    2）在子进程中，fork返回0；
	 *    3）如果出现错误，fork返回一个负值；
	 *  3.如果父进程先结束会导致子进程的PPID(parent process id)=1. 并且子进程中有无限循环的话会一直执行(在设置中强制终止程序甚至直接卸掉程序只能终止父进程, 不能停止该子进程，除非在终端kill掉子进程)
	 *
	 *  http://blog.csdn.net/jason314/article/details/5640969
	 */
	int pid = fork();
	FILE * file;
	if (pid > 0) {
		LOGD("pid = %d", pid);
	} else if (pid == 0) {
		LOGD("pid == 0");
		//在子进程中无线循环
		while (1) {
			LOGD("sub progress is running");
			// 获取父进程的进程id
			int ppid = getppid();

			//父进程进程id变成1时，说明父进程被杀死了
			if(ppid == 1){
				file = fopen("/data/data/com.example.jnidemo", "r"); //只读方式打开
				if(file == NULL){//程序已卸载, 弹出一个卸载原因的网页
					execlp("am", "am", "start", "--user", "0", "-a", "android.intent.action.VIEW", "-d", "http://www.baidu.com", (char *) NULL);
				}
				else{//终止了程序，则使用execlp再打开这个程序
					execlp("am", "am", "start", "--user", "0", "-n", "com.example.jnidemo/com.example.jnidemo.MainActivity", (char *) NULL);
					break;
				}
			}

			sleep(2);
		}
	} else {
		LOGD("pid < 0  error");
	}
}
