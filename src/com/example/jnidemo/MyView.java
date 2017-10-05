package com.example.jnidemo;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

public class MyView extends View {
	Paint paint;
	int pressure = 0;

	public MyView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		paint = new Paint();
	}

	public MyView(Context context, AttributeSet attrs) {
		super(context, attrs);
		paint = new Paint();
	}

	public MyView(Context context) {
		super(context);
		paint = new Paint();
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		if (pressure < 40) {
			paint.setColor(Color.GREEN);
		} else if (pressure < 80) {
			paint.setColor(Color.YELLOW);
		} else {
			paint.setColor(Color.RED);
		}
		canvas.drawRect(50, 200 - pressure, 100, 200, paint);
		canvas.drawText("当前压力值" + pressure, 50, 50, paint);
		paint.setTextSize(25);
	}

	public void setPressure(int pressure) {
		this.pressure = pressure;
		//invalidate只能在主线程调用 子线程用postInvalidate()
		//invalidate();
		postInvalidate();
	}
}
