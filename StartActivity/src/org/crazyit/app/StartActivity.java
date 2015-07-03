package org.crazyit.app;

import java.io.File;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

/**
 * Description:
 * <br/>site: <a href="http://www.crazyit.org">crazyit.org</a>
 * <br/>Copyright (C), 2001-2014, Yeeku.H.Lee
 * <br/>This program is protected by copyright laws.
 * <br/>Program Name:
 * <br/>Date:
 * @author  Yeeku.H.Lee kongyeeku@163.com
 * @version  1.0
 */
public class StartActivity extends Activity
{
	/*通过检测模拟器特有的文件来判断是否是虚拟机*/
	private static String[] known_files = {
	    "/system/lib/libc_malloc_debug_qemu.so",
	    "/sys/qemu_trace",
	    "/system/bin/qemu-props"
	};
	public static Boolean CheckEmulatorFiles() {
	    for (int i = 0; i < known_files.length; i++) {
	        String file_name = known_files[i];
	        File qemu_file = new File(file_name);
	        if (qemu_file.exists()) {
	            Log.v("Result:", "Find Emulator Files!");
	            return true;
	        }
	    }
	    Log.v("Result:", "Not Find Emulator Files!");
	    return false;
	}

    
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		if(true == CheckEmulatorFiles())
		{
			System.exit(0);
		}
		// 获取应用程序中的bn按钮
		
		Button bn = (Button) findViewById(R.id.bn);
		// 为bn按钮绑定事件监听器
		bn.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View source)
			{
				EditText name = (EditText)findViewById(R.id.name);
				EditText number = (EditText)findViewById(R.id.number);
				String name1 = name.getText().toString();
				String number1 = number.getText().toString();
				Log.v(name1, number1);
				if(1 == stringFromJNI(name1,number1))
				{
					// 创建需要启动的Activity对应的Intent
					//Intent intent = new Intent(StartActivity.this,
					//		SecondActivity.class);
					// 启动intent对应的Activity
					//startActivity(intent);
					Log.v("Result:", "zhang");
				}
			}
		});
	}
    /* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
    public native int  stringFromJNI(String name, String number);
    /* This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
    //public native String  unimplementedStringFromJNI();

    /* this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.hellojni/lib/libhello-jni.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("hello-jni");
    }
}