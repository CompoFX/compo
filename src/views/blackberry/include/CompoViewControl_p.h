#ifndef COMPOVIEW_CONTROL_PRIVATE_P_H
#define COMPOVIEW_CONTROL_PRIVATE_P_H

#include <screen/screen.h>
#include "CompoGraphicsContext.h"
#include "CompoComposition.h"

#include <pthread.h>
struct CompoViewControlPrivate
{
  screen_context_t mScreenContext;
  screen_window_t  mScreenWindow;
  
  CompoGraphicsContext mGLContext;
  CompoComposition *mCompo;
  
  bool bRenderDone;

  pthread_mutex_t mFrameLock, mViewLock;

  inline CompoViewControlPrivate() : bRenderDone(false)
  {
    pthread_mutex_init(&mFrameLock, 0);
    pthread_mutex_init(&mViewLock, 0);
  }

  inline ~CompoViewControlPrivate()
  {
    pthread_mutex_destroy(&mViewLock);
    pthread_mutex_destroy(&mFrameLock);
  }
};

#endif //COMPOVIEW_CONTROL_PRIVATE_P_H


