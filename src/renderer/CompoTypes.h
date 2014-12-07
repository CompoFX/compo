//
//  CompoTypes.h
//  CompoView
//
//  Created by Didiet Noor on 17/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_TYPES_H
#define COMPO_TYPES_H

#ifdef __cplusplus
#include <memory>
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <functional>
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#if __APPLE__
#include "TargetConditionals.h"
#  if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
#    include <OpenGLES/ES2/gl.h>
#    include <OpenGLES/ES2/glext.h>
#    define COMPO_TARGET_IOS 1
#  endif
#  define LOG_TAG "[COMPOIOS]"
#  define QUOTEME_(x) #x
#  define QUOTEME(x) QUOTEME_(x)
#  define LOGI(...) printf("\nI/" LOG_TAG " (" __FILE__ ":" QUOTEME(__LINE__) "): " __VA_ARGS__ )
#  define LOGE(...) printf("\nE/" LOG_TAG "(" ")" __VA_ARGS__)
#elif defined (__ANDROID__) || defined(ANDROID)
#define COMPO_TARGET_ANDROID 1
#define COMPO_USE_EGL 1
#include <jni.h>
#include <semaphore.h>
#define COMPOAPI EXTERN_C
#define LOG_TAG "[COMPOBRIDGE]"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <inttypes.h>

#  define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#  define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#  define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#  define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#  define LOGNS(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#elif defined(__QNXNTO_) || defined(__QNX__)
#define COMPO_TARGET_QNX 1
#define COMPO_USE_EGL 1

#include <inttypes.h>

#include "CompoSlogger.h"

#define LOGI(...) CompoSlogger::instance().log( SLOG2_INFO, __VA_ARGS__ )
#define LOGE(...) CompoSlogger::instance().log( SLOG2_ERROR, __VA_ARGS__ )
#define LOGD(...) CompoSlogger::instance().log( SLOG2_DEBUG1, __VA_ARGS__ )
#define LOGV(...) CompoSlogger::instance().log( SLOG2_DEBUG2, __VA_ARGS__)
#define LOGNS(...) CompoSlogger::instance().log( SLOG2_DEBUG2, __VA_ARGS__)
#include <cstddef>
#include <cstdarg>
#include <cstdlib>

using std::size_t;
using std::malloc;
using std::free;
using std::realloc;
using std::ptrdiff_t;
#elif defined(JOLLA) || defined(__JOLLA__)
#define COMPO_TARGET_JOLLA 1
#define COMPO_USE_EGL 1
#define LOGI(...)
#define LOGE(...)
#define LOGD(...)
#define LOGV(...)
#define LOGNS(...)
#endif

#if COMPO_TARGET_QNX || COMPO_TARGET_JOLLA
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/swap.hpp>
#include <boost/make_shared.hpp>
#define COMPO_SHARED boost::shared_ptr
#define COMPO_UNIQUE boost::scoped_ptr
#define COMPO_MAKE_SHARED boost::make_shared
#else
#define COMPO_SHARED std::shared_ptr
#define COMPO_UNIQUE std::unique_ptr
#define COMPO_MAKE_SHARED std::make_shared
#endif


#if COMPO_USE_EGL
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

//-- Forward Declarations of all Referenced Types
class CompoShader;
class CompoShaderProgram;
class CompoShaderInfoLog;
class CompoImage;
class CompoTexture;
class CompoPhotoQuad;
class CompoEffect;
class CompoScreenOutput;
class CompoRenderBufferOutput;
//--
#endif
