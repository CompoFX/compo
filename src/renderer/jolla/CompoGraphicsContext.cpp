#include "CompoGraphicsContext.inl"
#include <X11/Xlib.h> 

bool CompoGraphicsContext::init( void *screenPtr )
{
  Window win = (Window) screenPtr;

  LOGI("-- Initializing OpenGL Context --");

  if ((mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY)
  {
    LOGE("eglGetDisplay returned error %d", eglGetError());
    return false;
  }
  else {
    LOGI("eglGetDisplay SUCCEDED [0x%X]", (unsigned) mDisplay);
  }

  const EGLint attributes [] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_DEPTH_SIZE, 16,
    EGL_NONE
  };

  if (!eglInitialize(mDisplay, 0, 0)) {
    LOGE("eglInitialze returned error %d", eglGetError());
    return false;
  }

  if (EGL_TRUE != eglBindAPI(EGL_OPENGL_ES_API)) {
    LOGE("eglBindAPI returned error %d", eglGetError());
    return false;
  }

  EGLint numConfigs;
  EGLint format;
  if (!eglChooseConfig(mDisplay, attributes, &mConfig, 1, &numConfigs)) {
    LOGE("eglChooseConfig() returned error %d", eglGetError());
    this->destroy();
    return false;
  } 

  if (!eglGetConfigAttrib(mDisplay, mConfig, EGL_NATIVE_VISUAL_ID, &format))
  {
    LOGE("eglGetConfigAttrib returned error %d", eglGetError());
    this->destroy();
    return false;
  }

  if ((mSurface = eglCreateWindowSurface(mDisplay, mConfig, win, 0))
      == EGL_NO_SURFACE) 
  { 
    LOGE("eglCreateWindowSurface returned error %d", eglGetError()); 
    this->destroy();
    return false;
  }
  EGLint contextAttributes [] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
  }; 

  if ((mContext = eglCreateContext(mDisplay, mConfig, 0, contextAttributes))
      == EGL_NO_CONTEXT)
  {
    LOGE("eglCreateContext returned error %d", eglGetError());
    this->destroy();
    return false;
  }
  else
  {
    LOGI("eglCreateContext create [0x%X]", (unsigned) mContext);
  }

  if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)) {
    LOGE("eglMakeCurrent() returned error %d", eglGetError());
    this->destroy();
    return false;
  }

  EGLint width, height;
  if (!eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &width) ||
      !eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &height)) {
    LOGE("eglQuerySurface() returned error %d", eglGetError());
    this->destroy();
    return false;
  }
  else {
    mWidth = width;
    mHeight = height;
    LOGI("Surface Initialized %dx%d", width, height);
  }

  glDisable(GL_DITHER);
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClearDepthf(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return false;

}
