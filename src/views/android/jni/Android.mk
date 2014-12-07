LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := ../../renderer/ \
										../../renderer/egl \
 										../../renderer/android	\
										../../../3rdparty/glm-0.9.5/ 

LOCAL_MODULE    := Compo
LOCAL_CPP_FEATURES += exceptions
LOCAL_CFLAGS    := -Wall -Wfatal-errors # Warning as fatal errors
LOCAL_CFLAGS    += -D__STDC_FORMAT_MACROS
LOCAL_CXXFLAGS  := -std=c++11
LOCAL_LDLIBS    := -llog -landroid -lGLESv2 -ljnigraphics -lEGL

# Android-specific sources
LOCAL_SRC_FILES := ../../../renderer/android/CompoJNIBridge.cpp \
	../../../renderer/android/CompoImageAndroid.cpp \
	../../../renderer/android/CompoGraphicsContext.cpp \
	../../../renderer/android/CompoNativeView.cpp \
 	../../../renderer/android/CompoAndroidUtil.cpp 

# EGL sources
LOCAL_SRC_FILES += ../../../renderer/egl/CompoScreenOutput.cpp

# Cross-platform sources
LOCAL_SRC_FILES +=	../../../renderer/CompoShader.cpp \
	../../../renderer/CompoShaderInfoLog.cpp \
	../../../renderer/CompoShaderProgram.cpp \
	../../../renderer/CompoTexture.cpp \
	../../../renderer/CompoEffect.cpp \
	../../../renderer/CompoComposition.cpp \
	../../../renderer/CompoPhotoQuad.cpp \
	../../../renderer/CompoRenderBufferOutput.cpp

LOCAL_STATIC_LIBRARIES := cpufeatures

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/cpufeatures)
