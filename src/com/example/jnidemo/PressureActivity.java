package com.example.jnidemo;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;

public class PressureActivity extends Activity {
	static {
		System.loadLibrary("jnidemo");
	}
	//ProgressBar pb;
	MyView pb;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_pressure);
		//pb = (ProgressBar) findViewById(R.id.pb);
		pb = (MyView) findViewById(R.id.pb);
	}

	public void start(View v) {
		new Thread() {
			public void run() {
				startMoniter(); // 子线程中执行
			};
		}.start();

	}

	public void stop(View v) {
		stopMoniter();
	}

	public native void startMoniter();

	public native void stopMoniter();

	public void setPressure(int pressure) {
		//pb.setProgress(pressure);
		pb.setPressure(pressure);
	}
}
