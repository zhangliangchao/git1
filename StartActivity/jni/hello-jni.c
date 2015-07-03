/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
jint
Java_org_crazyit_app_StartActivity_stringFromJNI( JNIEnv* env, jobject thiz, jstring* name, jstring* number)
{
	int i = 0,k1 = 0;;
	char ch;
	const char const *strname = (*env)->GetStringUTFChars(env, name, NULL);
	const char const *strnumber = (*env)->GetStringUTFChars(env, number, NULL);
	int namelength = strlen(strname);
	int numberlength = strlen(strnumber);
	struct android_app *app;
	JavaVM *vm = AndroidGetJavaVM();

	(*vm)->AttachCurrentThread(vm, &env, 0);

	jobject lNativeActivity = app->activity->clazz;
	jclass intentClass = (*env)->FindClass(env, "android/content/Intent");
	jstring actionString =(*env)->NewStringUTF(env, "Package Name. Class Name");
	jmethodID newIntent = (*env)->GetMethodID(env, intentClass, "<init>", "()V");
	jobject intent = (*env)->AllocObject(env, intentClass);
	(*env)->CallVoidMethod(env, intent, newIntent);
	jmethodID setAction = (*env)->GetMethodID(env,intentClass, "setAction","(Ljava/lang/String;)Landroid/content/Intent;");
	(*env)->CallObjectMethod(env,intent, setAction, actionString);
	jclass activityClass = (*env)->FindClass(env, "android/app/Activity");
	jmethodID startActivity = (*env)->GetMethodID(env, activityClass,"startActivity", "(Landroid/content/Intent;)V");
	jobject intentObject = (*env)->NewObject(env,intentClass,newIntent);
	(*env)->CallVoidMethod(env, intentObject, setAction,actionString);
	(*env)->CallVoidMethod(env, lNativeActivity, startActivity, intentObject);
	(*vm)->DetachCurrentThread(vm);

	for (i = 0; i < namelength;i++)
	{
		ch = strname[i];
		if (ch < 'A') break;
		if (ch > 'Z') ch -= 32;
		k1 = k1 + ch;
	}
	k1 = k1 ^ 0x5678;

	int k2 = 0;
	for (i = 0;i < numberlength; i++)
	{
	    ch = strnumber[i];
	    ch -= 48;
	    k2 = k2 * 10 + ch;
	}
	k2 = k2 ^ 0x1234;

	if(k1 == k2)
	{
		//(*env)->startActivity((*env)->(new Intent(StartActivity.this, SecondActivity.class)));
		return 1;
		//return (*env)->NewStringUTF(env, "Hellozhangzhang123!\n");
	}
	else
	{
		return 2;
		//return (*env)->NewStringUTF(env, "111111111111111\n");
	}

}


