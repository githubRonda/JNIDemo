package com.example.jnidemo;

import android.content.Context;
import android.widget.Toast;

public class JNI {
	static {
		System.loadLibrary("jnidemo");
	}

	private Context mContext;
	
	public JNI(Context context){
		mContext = context;
	}

	/**
	 * 无形参，有返回
	 */
	public native String hello();

	/**
	 * 有形参, 有返回
	 */
	// 多形参
	public native int add(int i, int j);

	// 单形参
	public native String hello2(String s);

	// 形参数组，返回数组
	public native int[] changeArr(int[] arr);

	//=====================下面是在C中反射调用java类中的方法========================

	/**
	 * MainActivity中调用本地方法callBackHelloFromJava(), 而 C 中实现该方法且反射调用该类中的helloFromJava()方法。绕了一个圈！
	 */
	public native void callBackHelloFromJava();

	public void helloFromJava() {
		System.out.println("hello from java!");
	}

	/**
	 * MainActivity中调用本地方法callbackPlus(), 而 C 中实现该方法且反射调用该类中的plus()方法。绕了一个圈！
	 */
	public native void callbackPlus();

	public int plus(int i, int j) {
		return i + j;
	}
	
	/**
	 * MainActivity中调用本地方法callbackStringMethod(), 而 C 中实现该方法且反射调用该类中的printString()方法。绕了一个圈！
	 */
	public native void callbackStringMethod();
	
	private void printString(String str){
		System.out.println(str);
	}
	
	/**
	 * MainActivity中调用本地方法callbackShowToast(), 而 C 中实现该方法且反射调用MainActivity中的showToast()方法。和上面几个反射调用的方法不一样！！！
	 * 但是最后发现不幸的是：MainActivity是一个Context对象，直接在C中创建Activity对象使用时会报错的，所以最后还是把反射调用的方法移到本类中，这样就不用为 创建/获取 反射方法所在的对象而烦恼了
	 */
	public native void callbackShowToast();
	
	public void showToast(String msg) {
		Toast.makeText(mContext, msg, 0).show();
	}
	
	/**
	 * fork进程(程序中止时重新打开程序，程序卸载时弹出网页)
	 */
	public native void cfork();
}
