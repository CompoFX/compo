#ifndef COMPO_ANDROID_BRIDGE
#define COMPO_ANDROID_BRIDGE

#include "CompoTypes.h"
#include "CompoComposition.h"

class CompoNativeView;
class CompoImage;

class CompoAndroidBridge
{
private:
  JavaVM *mVM;
  AAssetManager *mAssetManager;
  jobject mJavaAssetManager;

  pthread_mutex_t mBridgeMutex;

  typedef std::map<const std::string,
          CompoNativeView *> ViewMap;

  ViewMap mNativeViews;

public:
  void initBridge( JavaVM *, JNIEnv *);
  void attachAssetManager( jobject javaAssetManager );
  void detachCurrentAssetManager();

  static CompoAndroidBridge *instance();
  
  inline jobject getJavaAssetManager() const;
  inline AAssetManager *getAssetManager() const;
  inline pthread_mutex_t *getLock(); // abstraction leak

  inline JavaVM *getVM() const;
 
  void registerView(const std::string &vid,
      CompoNativeView *);

  void releaseView(const std::string &vid);
  
  void startRenderThreadOnSurface(const std::string &vid,
      const std::string &tid, jobject surfaceRef);

  void stopRenderThread(const std::string &vid,
      const std::string &tid, jobject surfaceRef );

  void setNativeEffectOnView( const std::string &vid,
      const CompoComposition::CompoCompositionEffect effect );

  void setBitmapOnView( const std::string &vid,
      const CompoImage &image );

  bool isViewExists( const std::string &vid ) const;

  jobject getResultBitmapOnView( const std::string &vid, JNIEnv *env);

private:
  void initNativeMethods(JNIEnv *env); 
  CompoAndroidBridge();
  virtual ~CompoAndroidBridge(); 
};


#define CompoBridge (CompoAndroidBridge::instance())
#define CompoJVM (CompoBridge->getVM())
#define CompoAssetManager (CompoBridge->getAssetManager())
#define CompoBridgeLock (CompoBridge->getLock())

inline AAssetManager *CompoAndroidBridge::getAssetManager() const
{
  return CompoBridge->mAssetManager;
}

inline jobject CompoAndroidBridge::getJavaAssetManager() const
{
  return CompoBridge->mJavaAssetManager;
}

inline pthread_mutex_t *CompoAndroidBridge::getLock()
{
  return &mBridgeMutex;
}

inline JavaVM *CompoAndroidBridge::getVM() const 
{
  return CompoBridge->mVM;
}

#endif
