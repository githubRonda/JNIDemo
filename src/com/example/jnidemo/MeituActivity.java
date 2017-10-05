package com.example.jnidemo;

import java.io.IOException;

import com.mt.mtxx.image.JNI;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

/**
 * 美图秀秀之反编译使用so库
 */
public class MeituActivity extends Activity {

	private ImageView imageView;
	private JNI mtxx_jni;
	private Bitmap bitmap;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_meitu);

		imageView = (ImageView) findViewById(R.id.iv);
		mtxx_jni = new JNI();

		try {
			//Bitmap bitmap = BitmapFactory.decodeFile("sdcard/awaiyi.jpg");
			bitmap = BitmapFactory.decodeStream(getAssets().open("ic_launcher.png"));
			imageView.setImageBitmap(bitmap);

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void click(View view) {
		int width = bitmap.getWidth();
		int height = bitmap.getHeight();

		int[] piexels = new int[width * height];// 为什么要用int类型的数组来存储颜色值，因为每个像素颜色是由argb组成的，各占一个字节，一共4字节。而int也刚好是4字节
		//获取图片的像素信息保存至pixels中
		bitmap.getPixels(piexels, 0, width, 0, 0, width, height);//参3类似于逐行扫描，要大于等于bitmap.width

		// 通过美图秀秀的so中的函数就piexels像素信息进行效果处理
		mtxx_jni.StyleLomoB(piexels, width, height);

		Bitmap newBitmap = Bitmap.createBitmap(piexels, width, height, bitmap.getConfig());
		imageView.setImageBitmap(newBitmap);
	}

}
