#include "CompoAndroidUtil.h"

jobject imageToAndroidBitmap( JNIEnv *env, const CompoImage &image )
{
  const char * const Bitmap_Clazz_Name="android/graphics/Bitmap";
  const char * const Bitmap_Config_Clazz_Name="android/graphics/Bitmap$Config";

  jclass Bitmap_Clazz = env->FindClass(Bitmap_Clazz_Name);
  jclass BitmapConfig_Clazz = env->FindClass(Bitmap_Config_Clazz_Name);

  jmethodID createBitmapID = env->GetStaticMethodID(Bitmap_Clazz, "createBitmap",
      "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");

  jfieldID ARGB888ConfigID = env->GetStaticFieldID(BitmapConfig_Clazz,
      "ARGB_8888", "Landroid/graphics/Bitmap$Config;");

  jobject ARGB_8888 = env->GetStaticObjectField(BitmapConfig_Clazz,
      ARGB888ConfigID);

  jobject Bitmap_obj = env->CallStaticObjectMethod(Bitmap_Clazz,
      createBitmapID, image.width(), image.height(), ARGB_8888);

  jmethodID copyFromBufferID = env->GetMethodID(Bitmap_Clazz,
      "copyPixelsFromBuffer", "(Ljava/nio/Buffer;)V");

  jobject byteBuffer = env->NewDirectByteBuffer((void*) image.pixels(), 
                             image.dataSize());

  env->CallVoidMethod(Bitmap_obj, copyFromBufferID, byteBuffer); 
  
  return Bitmap_obj;
}

