package com.example.jnidemo;

import java.util.Arrays;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

/**
 * java 与 C 之间的数据传递
 */
public class MainActivity extends Activity {

	private JNI jni;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		jni = new JNI(getApplicationContext());
	}

	public void hello(View view) {
		String str = jni.hello();
		showToast(str);
	}

	//两个int相加返回int
	public void add(View view) {
		int total = jni.add(3, 4);
		showToast("3 + 4 = " + total);
	}

	//传递一个String，每个字符加1再返回
	public void hello2(View view) {
		String str = jni.hello2("abc");
		showToast(str);
	}

	//传递int[]，每个元素加10，然后再把这个int[]返回。本质上就是对这个int[]数组的修改，并没有涉及到int[]类型的创建与转换
	public void changeArr(View view) {
		int[] arr_t = { 1, 2, 3, 4 };
		int[] arr = jni.changeArr(arr_t); // 传递过去的是地址，本质上就是通过地址对存储的值进行修改

		System.out.println(Arrays.toString(arr_t));// [11, 12, 13, 14]
		System.out.println(Arrays.toString(arr)); // [11, 12, 13, 14]
		System.out.println("arr_t == arr: " + (arr_t == arr));//true
	}
	
	
	//========================C中反射调用java中的方法=============================
	public void callbackVoidMethod(View view){
		jni.callBackHelloFromJava();
	}
	
	public void callbackIntMethod(View view){
		jni.callbackPlus();
	}
	
	public void callbackStringMethod(View view){
		jni.callbackStringMethod();
	}
	
	public void callbackShowToast(View view){
		jni.callbackShowToast();
	}
	
	//================美图秀秀======================
	public void jumpMeitu(View view){
		startActivity(new Intent(this, MeituActivity.class));
	}
	
	// 压力表
	public void jumpPressure(View view){
		startActivity(new Intent(this, PressureActivity.class));
	}
	
	//fork进程
	public void fork(View view){
		jni.cfork();
	}
	

	public void showToast(String msg) {
		Toast.makeText(this, msg, 0).show();
	}

}
