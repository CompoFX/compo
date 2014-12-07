#ifndef COMPO_NATIVE_VIEW
#define COMPO_NATIVE_VIEW

#include "CompoTypes.h"
#include "CompoComposition.h"
#include "CompoImage.h"
#include "CompoGraphicsContext.h"

struct CompoViewState
{
  CompoImage image;
  CompoComposition::CompoCompositionEffect effect;
  
  inline CompoViewState(const CompoImage &img,
      const CompoComposition::CompoCompositionEffect fx):
    image(img), effect(fx) {}

  inline CompoViewState(const CompoImage &img):
    image(img), effect(CompoComposition::EFFECT_NORMAL) {}

  inline CompoViewState(const CompoViewState &other) :
    image(other.image), effect(other.effect) {}
  
  inline CompoViewState &operator= (const CompoViewState &other)
  {
    image = other.image;
    effect = other.effect;
    return *this;
  }
};

class CompoNativeView
{
private:
  CompoGraphicsContext mMainContext;
  const std::string mViewID;
  CompoComposition *mCompo;
  CompoViewState state;
  
  typedef std::pair< pthread_t, jobject > ThreadPair;
  typedef std::map< const std::string,
    ThreadPair > ThreadMap;

  ThreadMap mRenderingThread;
  pthread_mutex_t mViewLock;
  pthread_mutex_t mFrameLock;

  static void * viewThreadRenderFunc( void * );
  
  bool mJustChanged;  
  bool mResumed;
public:
  CompoNativeView(const std::string &vid,
     const CompoViewState &state);
  virtual ~CompoNativeView();
  
  void startRenderingOnSurface( const std::string &tid, 
      jobject surface);
  void stopRendering(const std::string &tid);

  void setState( const CompoViewState &state );
  
  inline const CompoViewState &getState() const {
    return state;    
  }

  jobject getResultBitmap( JNIEnv *env );

};

#endif // COMPO_NATIVE_VIEW
