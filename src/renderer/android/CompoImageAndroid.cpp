
//  CompoImage.cpp
//  CompoView
//
//  Created by Didiet Noor on 18/02/14.
//  Copyright (c) 2014 Nomad Studio. All rights reserved.
//

#include "CompoImage.h"
#include "../CompoImage.inl"
#include "CompoJNIBridge.h"
#include "CompoScopedJNI.h"

/* On Android these only executed on rendering thread thus the
 * AttachCurrentThread
 * Don't even try to access this on main thread
 * Will show error
 * RENDERING IS NOT IN THE MAIN THREAD
 */

CompoImage::CompoImage( const std::string &relativePath )  : d(new Private)
{
  JNIEnv *env;
  
  CompoScopedJNI startJNI(CompoBridge->getVM(), &env);

  jclass BFC_Clazz = env->FindClass("android/graphics/BitmapFactory");

  if (BFC_Clazz == 0) {
    LOGE("Error Loading BitmapFactory Class");
    return;
  }

  jmethodID BFC_decodeStreamMethod = env->GetStaticMethodID(BFC_Clazz, "decodeStream",
      "(Ljava/io/InputStream;)Landroid/graphics/Bitmap;");

  if (BFC_decodeStreamMethod == 0) {
    LOGE("Error loading BitmapFactory.decodeStream(InputStream) method");
    return;
  }
  
  jobject amj = CompoBridge->getJavaAssetManager();
   
  if (amj == 0) {
    LOGE("Asset Manager is not yet attached!");
    return;
  }
  
  jclass AM_Clazz = env->GetObjectClass(amj);
  jmethodID AM_openMID = env->GetMethodID(AM_Clazz, "open", "(Ljava/lang/String;)Ljava/io/InputStream;");

  if (AM_openMID == 0) {
    LOGE("Error Getting open Method from AssetManager");
    return;
  }

  const std::string fullPath = std::string("Compo.bundle/") + relativePath;
  jstring fullPathStr = env->NewStringUTF(fullPath.c_str());
  
  jobject IS_Object = env->CallObjectMethod(amj, AM_openMID, fullPathStr);
  if (env->ExceptionCheck()) {
    LOGE("Error loading file %s from asset", fullPath.c_str());
    env->ExceptionClear();
     return;
  }

  jobject BMP_Object = env->CallStaticObjectMethod(BFC_Clazz, BFC_decodeStreamMethod, IS_Object);

  if (BMP_Object == 0) {
    LOGE("Error Getting Bitmap from Stream");
    return;
  }
  else {
    LOGI("Asset %s loaded!", fullPath.c_str());
  }
  
  // getting bitmapInfo

  AndroidBitmapInfo nativeInfo;
  AndroidBitmap_getInfo(env, BMP_Object, &nativeInfo);

  if (nativeInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
    LOGE("Format of %s not Supported! Should be 32 bit format", fullPath.c_str());
  }

  d->width = nativeInfo.width;
  d->height = nativeInfo.height;
  d->bpp = 32;
  
  LOGI("Bitmap Info: %dx%d %d bit", d->width, d->height, d->bpp);
  d->pixelData = (unsigned char*) calloc(d->dataSize(), sizeof(unsigned char));
  void *np = 0; int ret;
  if ( ( ret = AndroidBitmap_lockPixels(env, BMP_Object, (void**) &np)) < 0 ) {
    LOGE("AndroidBitmap_lockPixels failed error %d", ret);
    return;
  }
  memcpy(d->pixelData, np, d->dataSize());
  LOGI("Data Size: %d", d->dataSize()); 
  AndroidBitmap_unlockPixels(env, BMP_Object);
}

CompoImage::CompoImage( void *imagePtr ) : d(new Private) {
  JNIEnv *env;
  CompoScopedJNI startJNI(CompoBridge->getVM(), &env);

  if (imagePtr == 0) {
    LOGI("Passed Image Pointer is NULL, no pixels allocated");
    return;
  }
  jobject BMP_Object = (jobject) imagePtr;
  
  // getting bitmapInfo

  AndroidBitmapInfo nativeInfo;
  AndroidBitmap_getInfo(env, BMP_Object, &nativeInfo);

  if (nativeInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
    LOGE("Format of jobject [0x%X] not Supported! Should be 32 bit format",
        (unsigned)imagePtr);
    return; 
  }

  d->width = nativeInfo.width;
  d->height = nativeInfo.height;
  d->bpp = 32;
  
  LOGI("Bitmap Info: %dx%d %d bit", d->width, d->height, d->bpp);
  d->pixelData = (unsigned char*) calloc(d->dataSize(), sizeof(unsigned char));
  void *np = 0; int ret;
  if ( ( ret = AndroidBitmap_lockPixels(env, BMP_Object, (void**) &np)) < 0 ) {
    LOGE("AndroidBitmap_lockPixels failed error %d", ret);
    return;
  }
  memcpy(d->pixelData, np, d->dataSize());
  LOGI("Data Size: %d", d->dataSize());
  AndroidBitmap_unlockPixels(env, BMP_Object);
}

static inline CompoImage decodeFromBytes(const unsigned char *data,
    const size_t dataSize);

CompoImage CompoImage::fromPNGData( const unsigned char *data,
    const size_t dataSize)
{
  return decodeFromBytes(data, dataSize);
}

CompoImage CompoImage::fromJPGData( const unsigned char *data,
    const size_t dataSize)
{
  return decodeFromBytes(data, dataSize);
}

static inline CompoImage decodeFromBytes(const unsigned char *data,
    const size_t dataSize)
{
  JNIEnv *env;
  CompoScopedJNI startJNI(CompoBridge->getVM(), &env);
   
  jclass BFC_Clazz = env->FindClass("android/graphics/BitmapFactory");

  if (BFC_Clazz == 0) {
    LOGE("Error Loading BitmapFactory Class");
    
  }

  jmethodID BFC_decodeByteArrayMethod = env->GetStaticMethodID(BFC_Clazz, 
      "decodeByteArray",
      "([BII)Landroid/graphics/Bitmap;");

  if (BFC_decodeByteArrayMethod == 0) {
    LOGE("Error loading BitmapFactory.decodeByteArray(InputStream) method");
  }
    
  jbyteArray imageBuf = env->NewByteArray(dataSize);

  env->SetByteArrayRegion(imageBuf, 0, dataSize, (jbyte*) data);
  
  jobject BMP_Object = env->CallStaticObjectMethod(BFC_Clazz, 
      BFC_decodeByteArrayMethod, imageBuf, 0, dataSize);
  
  CompoImage resultImage((void*) BMP_Object);

  env->DeleteLocalRef(imageBuf);

  return resultImage;
}


