#ifndef COMPO_GRAPHICS_CONTEXT
#define COMPO_GRAPHICS_CONTEXT
#include "CompoTypes.h"

class CompoGraphicsContext
{
private:
  EGLConfig  mConfig;
  EGLContext mContext;
  EGLDisplay mDisplay;
  EGLSurface mSurface;
  int mWidth, mHeight;
  bool mShared;
public:
  CompoGraphicsContext();
  virtual ~CompoGraphicsContext();

  bool init( void *nativeWindow );
  void resize( const int width, const int height );
  void destroy();
  void swap();
  
  void grabContext(bool retry = false) const;
  void releaseContext() const;

  inline int getWidth() const { return mWidth; }
  inline int getHeight() const { return mHeight; }

  CompoGraphicsContext(const CompoGraphicsContext &);
};

#endif
