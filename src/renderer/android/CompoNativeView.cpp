#include "CompoNativeView.h"
#include "CompoGraphicsContext.h"
#include "CompoImage.h"
#include "CompoJNIBridge.h"
#include "CompoRenderBufferOutput.h"
#include "CompoScreenOutput.h"
#include "CompoAndroidUtil.h"
#include "CompoScopedJNI.h"

#include <errno.h>

CompoNativeView::CompoNativeView(const std::string &vid,
    const CompoViewState &state_) : 
  mViewID(vid), mCompo(0),
  state(state_), mJustChanged(false),
  mResumed(false)
{
  pthread_mutex_init(&mViewLock, NULL);
  pthread_mutex_init(&mFrameLock, NULL);
}

CompoNativeView::~CompoNativeView()
{
  pthread_mutex_destroy(&mFrameLock);
  pthread_mutex_destroy(&mViewLock);
}

void CompoNativeView::setState( const CompoViewState &state_)
{
  state = state_;
  mJustChanged = true;
  LOGI("Changed State to %d", (int) state.effect);
}

struct _ThreadParam
{
  CompoNativeView *view;
  const std::string tid;
  jobject surfaceRef;

  inline _ThreadParam(CompoNativeView *view_,
      const std::string &tid_,
      jobject surfaceRef_):
    view(view_), tid(tid_), surfaceRef(surfaceRef_) {}

  inline _ThreadParam(const _ThreadParam &other):
    view(other.view), tid(other.tid), surfaceRef(other.surfaceRef) {}
};

void *CompoNativeView::viewThreadRenderFunc( void *p )
{

  _ThreadParam *param = reinterpret_cast<_ThreadParam*>(p);
  CompoNativeView *thisView = param->view;

  pthread_mutex_lock(&thisView->mViewLock);

  jobject surfaceRef = param->surfaceRef;

  LOGI("Rendering Thread for [%s] on Surface [0x%X] Started", 
      thisView->mViewID.c_str(), (unsigned) surfaceRef);

  LOGI("---> Creating Rendering Context on thread [%s]", param->tid.c_str());
  CompoGraphicsContext &context = thisView->mMainContext;
  CompoScreenOutput screenOutput(context);
  context.grabContext();

  if (!thisView->mResumed) {
    const CompoViewState newState( CompoImage("demo.jpg"),
      CompoComposition::EFFECT_NORMAL);
    thisView->setState(newState);
    thisView->mResumed = true; 
  }

  CompoComposition *compo = new CompoComposition(thisView->state.image);
  thisView->mCompo = compo;
  compo->setActiveEffect(thisView->state.effect);

  static const struct timespec spec = {0, 66666666};
  
  ThreadMap &mapRef = param->view->mRenderingThread;
  ThreadMap::iterator it = mapRef.find(param->tid);

  thisView->mResumed = true;
  LOGI((thisView->mResumed ? "<RESUMED>" : "<NON RESUMED>"));
  while (it != mapRef.end()) {
    nanosleep(&spec, 0);
    pthread_mutex_lock(&thisView->mFrameLock);
    context.grabContext();
    it = mapRef.find(param->tid);
    if (it == mapRef.end()) {
      pthread_mutex_unlock(&thisView->mFrameLock);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (thisView->mJustChanged) {
      compo->setImage(thisView->state.image);
      compo->setActiveEffect(thisView->state.effect);
      thisView->mJustChanged = false;
    }
    screenOutput.render(*compo);
    
    context.swap();
    context.releaseContext();
    pthread_mutex_unlock(&thisView->mFrameLock);
  }

  pthread_mutex_unlock(&thisView->mViewLock);

  {

    LOGI("-->> Rendering Thread Finished %s <<--", param->tid.c_str());
    JNIEnv *env;
    CompoScopedJNI startJNI(CompoBridge->getVM(), &env);
    env->DeleteGlobalRef(surfaceRef);
  }

  //-- 
  delete compo; compo = 0;
  delete param; param = 0;
  
  return 0;
}

jobject CompoNativeView::getResultBitmap( JNIEnv *env )
{
  if (mCompo != 0) {    
    const int imageWidth  = 512;
    const int imageHeight = 512;
    const int imageStride = imageWidth * 4;
    const int imageByteSize = imageHeight * imageStride;
    uint8_t *imageBuffer = (uint8_t*) malloc(imageByteSize);
    
    pthread_mutex_lock(&mFrameLock);
    mMainContext.grabContext();
    CompoRenderBufferOutput offScreen(imageWidth, imageHeight);
    offScreen.render(*mCompo);
    LOGI(" ---->> Grabbing Frame...");
    glReadPixels(0, 0, imageWidth, imageHeight, GL_RGBA, 
        GL_UNSIGNED_BYTE, imageBuffer);
    mMainContext.releaseContext();
    pthread_mutex_unlock(&mFrameLock);

    const CompoImage resultImage(imageWidth, imageHeight, imageBuffer);
    return imageToAndroidBitmap(env, resultImage);
  }
  else {
    LOGE(" <<---- Compo [0x%X] is NULL", (unsigned) mCompo); 
  }
  return NULL;
}

void CompoNativeView::stopRendering(const std::string &tid)
{
  LOGI("---->>> STOPPING RENDERING AT %s", tid.c_str());
  ThreadMap::iterator it = mRenderingThread.find(tid);
  if (it != mRenderingThread.end()) {
    mRenderingThread.erase(it);
  }

  pthread_mutex_lock(&mViewLock);
  mMainContext.destroy();
  pthread_mutex_unlock(&mViewLock);

}

void CompoNativeView::startRenderingOnSurface( const std::string &tid, 
    jobject surfaceRef)
{
  LOGI("---> Creating Rendering Context on main thread");
  pthread_mutex_lock(&mViewLock); 
  mMainContext.init(surfaceRef);
  mMainContext.releaseContext();
  pthread_mutex_unlock(&mViewLock);
  
  pthread_t renderThread;
  _ThreadParam *param = new _ThreadParam(this, tid, surfaceRef);
  pthread_create(&renderThread, 0,
      CompoNativeView::viewThreadRenderFunc,
     param);
  mRenderingThread[tid] = std::make_pair(renderThread, surfaceRef);
  LOGI("<THREAD> [START] Rendering Thread Count %d", mRenderingThread.size());
}
