#include "CompoGraphicsContext.h"

CompoGraphicsContext::CompoGraphicsContext(const CompoGraphicsContext &other ):
  mConfig(other.mConfig),
  mContext(EGL_NO_CONTEXT),
  mDisplay(other.mDisplay),
  mSurface(other.mSurface),
  mWidth(other.mWidth), mHeight(other.mHeight),
  mShared(true)
{
  EGLint contextAttributes [] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
  }; 
  
  if ((mContext = eglCreateContext(mDisplay, mConfig, other.mContext, contextAttributes))
     == EGL_NO_CONTEXT)
  {
    LOGE("eglCreateContext Shared returned error %d", eglGetError());
  }
  else
  {
    LOGI("eglContext Created [0x%X]", (unsigned) mContext);
  }
  grabContext(); 
  glDisable(GL_DITHER);
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClearDepthf(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
}

CompoGraphicsContext::CompoGraphicsContext():
  mContext(EGL_NO_CONTEXT), 
  mDisplay(EGL_NO_DISPLAY),
  mSurface(EGL_NO_SURFACE),
  mWidth(0), mHeight(0),
  mShared(false)
{
}

CompoGraphicsContext::~CompoGraphicsContext()
{
  if (mContext != 0) {
    this->destroy();
  }
}

void CompoGraphicsContext::destroy()
{
  LOGI("--- Destroying %s Context [0x%X] ---", 
      (mShared ? "Shared" : "Main"),
      (unsigned) mContext);
  if (mShared) {
    mContext = EGL_NO_CONTEXT;
    return;
  }


  eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  
  if (mContext != EGL_NO_CONTEXT) {
    LOGI("Destroying Context [0x%X]", (unsigned) mContext);
    eglDestroyContext(mDisplay, mContext);
  }
  mContext = EGL_NO_CONTEXT;

  if (mSurface != EGL_NO_SURFACE) {
    LOGI("Destroying Surface [0x%X]", (unsigned) mSurface);
    eglDestroySurface(mDisplay, mSurface);
  }
  mSurface = EGL_NO_SURFACE;

  if (mDisplay != EGL_NO_DISPLAY)
  {
    LOGI("Terminating Display [0x%X]", (unsigned) mDisplay);
    eglTerminate(mDisplay);
  }
  mDisplay = EGL_NO_DISPLAY;
}

void CompoGraphicsContext::grabContext(bool retry) const
{
  if (retry) {
    EGLint err;
    do {
      eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);
      
      err = eglGetError();
    } while(err == EGL_BAD_ACCESS);
  }
  else {
  eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);
  }
}

void CompoGraphicsContext::releaseContext() const
{
  eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

void CompoGraphicsContext::resize( const int width, const int height )
{
  grabContext();
  glViewport(0,0, width, height);
  mWidth = width;
  mHeight = height;

  LOGI("Viewport now %dx%d", width, height);
}

void CompoGraphicsContext::swap()
{
  if(mContext != 0) {
    if(!eglSwapBuffers(mDisplay, mSurface)) {
      LOGE("eglSwapBuffers() returned error %d", eglGetError());
    }
  }
}
