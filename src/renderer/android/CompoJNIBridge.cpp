#include <ctime>
#include <cpu-features.h>
#include "CompoJNIBridge.h"
#include "CompoImage.h"
#include "CompoNativeView.h"

static inline const std::string getCPUString();

static inline const std::string jstringToString(JNIEnv *env, jstring str) 
{
  const char *nativeString = env->GetStringUTFChars(str, 0);
  const std::string myString(nativeString);
  env->ReleaseStringUTFChars(str, nativeString);

  return myString;
}

static inline void throwNotImplementedException(JNIEnv *env, 
    const std::string &message)
{
  static const char *clazzName = "java/lang/UnsupportedOperationException";
  jclass exClazz = env->FindClass(clazzName); 
  if (exClazz != NULL) {
    env->ThrowNew(exClazz, message.c_str());
  }
}

#define STRJ(env, str) (jstringToString(env, str))

COMPOAPI jint JNI_OnLoad( JavaVM *vm, void * )
{
  LOGI("============== COMPO NATIVE LIBS =============");
  LOGI("= COPYRIGHT (c) 2014 YKode Studio            -");
  LOGI("----------------------------------------------");
  LOGI("Loading Native Libs...");
  JNIEnv *env;
  if (vm->GetEnv((void**)(&env), JNI_VERSION_1_6) != JNI_OK) {
    LOGE("Native Library failed to be loaded");
    return -1;
  }

  CompoBridge->initBridge( vm, env );
  LOGI("%s - on %s", "Native Library Loaded", getCPUString().c_str()); 
  return JNI_VERSION_1_6;
}


static CompoAndroidBridge *g_AndroidBridge = 0;

CompoAndroidBridge * CompoAndroidBridge::instance()
{  
  if (0 == g_AndroidBridge) {
    g_AndroidBridge = new CompoAndroidBridge;
  }
  return g_AndroidBridge;
}

void CompoAndroidBridge::initBridge( JavaVM *vm, JNIEnv *env ) 
{
  mVM = vm;
  this->initNativeMethods(env);

  LOGI("Android Native Bridge initialised");
}

void CompoAndroidBridge::registerView( const std::string &vid,
    CompoNativeView *nativeView)
{
  mNativeViews[vid] = nativeView;
  LOGI("-->> Register View [%s] view count %d", vid.c_str(), mNativeViews.size());
}

void CompoAndroidBridge::releaseView( const std::string &vid )
{
  ViewMap::iterator it = mNativeViews.find(vid);
  if (it != mNativeViews.end()) {
    LOGI("<<-- Unregister View [%s]", vid.c_str());
    delete it->second;
    mNativeViews.erase(it);
  }
  LOGI("-->> View Released [%s] view count %d", vid.c_str(), mNativeViews.size());
}

void CompoAndroidBridge::startRenderThreadOnSurface(const std::string &vid,
    const std::string &tid, jobject surfaceRef)
{
  ViewMap::iterator it = mNativeViews.find(vid);  
  if (it != mNativeViews.end()) {
    CompoNativeView *v = it->second;
    v->startRenderingOnSurface(tid, surfaceRef);  
  }
}

void CompoAndroidBridge::stopRenderThread(const std::string &vid,
    const std::string &tid, jobject surfaceRef)
{
  ViewMap::iterator it = mNativeViews.find(vid);
  if (it != mNativeViews.end()) {
    CompoNativeView *v = it->second;
    v->stopRendering(tid);
  }
}

void CompoAndroidBridge::setNativeEffectOnView(const std::string &vid,
    const CompoComposition::CompoCompositionEffect effect)
{
  ViewMap::iterator it = mNativeViews.find(vid);
  if (it != mNativeViews.end()) {
    CompoNativeView *v = it->second;
    const CompoViewState newState(v->getState().image, effect);  
    v->setState(newState);
  }
}

void CompoAndroidBridge::setBitmapOnView(const std::string &vid,
    const CompoImage &image)
{
  ViewMap::iterator it = mNativeViews.find(vid);
  if (it != mNativeViews.end()) {
    CompoNativeView *v = it->second;
    const CompoViewState newState(image, v->getState().effect);
    v->setState(newState);
  }
}

jobject CompoAndroidBridge::getResultBitmapOnView( const std::string &vid, 
    JNIEnv *env)
{
  ViewMap::iterator it = mNativeViews.find(vid);
  if (it != mNativeViews.end()) {
    CompoNativeView *v = it->second;
    return v->getResultBitmap(env);
  }
  else {
    return 0;
  }
}

bool CompoAndroidBridge::isViewExists( const std::string &vid ) const
{
  return (mNativeViews.find(vid) != mNativeViews.end());
}

COMPOAPI void JNICALL
AndroidBridge_CV_registerView(JNIEnv *env, jobject, jstring jvid)
{
  const std::string vid = STRJ(env, jvid);
  const CompoViewState 
    state(CompoImage("demo.jpg"), CompoComposition::EFFECT_NORMAL);
  CompoNativeView *newView = new CompoNativeView(vid, state);
  CompoBridge->registerView(vid, newView);
}

COMPOAPI void JNICALL
AndroidBridge_CV_releaseView(JNIEnv *env, jobject, jstring jvid)
{
  CompoBridge->releaseView(STRJ(env, jvid));
}

COMPOAPI void JNICALL
AndroidBridge_CV_startRenderThreadOnSurface(JNIEnv *env, jobject,
    jstring vid, jstring tid, jobject surface)
{
  jobject surfaceRef = env->NewGlobalRef(surface);
  LOGI("Starting Rendering Thread... on GR [0x%X]", (unsigned) surfaceRef);
  CompoBridge->startRenderThreadOnSurface(STRJ(env, vid),
      STRJ(env, tid), surfaceRef);
}

COMPOAPI void JNICALL
AndroidBridge_CV_stopRenderThread(JNIEnv *env, jobject,
    jstring vid, jstring tid, jobject surface)
{
  CompoBridge->stopRenderThread(STRJ(env, vid),
      STRJ(env, tid), surface);
}

COMPOAPI void JNICALL
AndroidBridge_CV_setRenderBitmap(JNIEnv *env, jobject,
    jstring vid, jobject bitmap)
{
  
  CompoBridge->setBitmapOnView(STRJ(env, vid),
      CompoImage((void*) bitmap));

}

COMPOAPI void JNICALL
AndroidBridge_CV_setNativeEffect(JNIEnv *env, jobject,
    jstring vid, jobject effect)
{
  jclass effectClazz = env->GetObjectClass(effect);
  jmethodID effectOrdinal_MID = env->GetMethodID(effectClazz,
      "ordinal", "()I");
  jint effectID = env->CallIntMethod(effect, effectOrdinal_MID);
  CompoBridge->setNativeEffectOnView(STRJ(env, vid), 
      (CompoComposition::CompoCompositionEffect) effectID); 
}

COMPOAPI jobject JNICALL
AndroidBridge_CV_getResultBitmap(JNIEnv *env, jobject, jstring jvid)
{
  const std::string vid = STRJ(env, jvid);
  return CompoBridge->getResultBitmapOnView(vid, env);
}

//-- Demarcation for Native APIs --
COMPOAPI void JNICALL 
AndroidBridge_initWithContext(JNIEnv *, jclass, jobject);

COMPOAPI void JNICALL
AndroidBridge_destroyWithContext(JNIEnv *, jclass, jobject);

COMPOAPI void JNICALL
AndroidBridge_bitmapTest(JNIEnv *env, jclass);

COMPOAPI void JNICALL
AndroidBridge_CV_registerView(JNIEnv *env, jobject,
    jstring);

COMPOAPI void JNICALL
AndroidBridge_CV_releaseView(JNIEnv *env, jobject,
    jstring);

COMPOAPI void JNICALL
AndroidBridge_CV_startRenderThreadOnSurface(JNIEnv *env, jobject,
    jstring, jstring, jobject);

COMPOAPI void JNICALL
AndroidBridge_CV_stopRenderThread(JNIEnv *env, jobject,
    jstring, jstring, jobject);

COMPOAPI void JNICALL
AndroidBridge_CV_setRenderBitmap(JNIEnv *, jobject,
    jstring, jobject);

COMPOAPI void JNICALL
AndroidBridge_CV_setNativeEffect(JNIEnv *env, jobject,
    jstring, jobject);

COMPOAPI jobject JNICALL
AndroidBridge_CV_getResultBitmap(JNIEnv *, jobject, jstring);
//--

#define COMPO_CLASS(c) ("com/ykode/compo/"#c)

void CompoAndroidBridge::initNativeMethods( JNIEnv *env )
{
  const int count = 2;
  JNINativeMethod bridgeMethods [count]  = {
    {"initWithContext",
      "(Landroid/content/Context;)V",
      (void*) AndroidBridge_initWithContext},

    {"destroyWithContext",
      "(Landroid/content/Context;)V",
      (void*) AndroidBridge_destroyWithContext}
  };

  jclass clazzCAN = env->FindClass(COMPO_CLASS(CompoBridge));

  if(env->RegisterNatives(clazzCAN, bridgeMethods, count) == JNI_OK) { 
    LOGI("Native Methods registered");
  }
  else {
    LOGE("Native Methods Registration Failed");
  }

  const int viewMethodCounts = 7;
  JNINativeMethod viewMethods [viewMethodCounts] = {
    {"registerView",
      "(Ljava/lang/String;)V",
      (void*) AndroidBridge_CV_registerView},
    {"releaseView",
      "(Ljava/lang/String;)V",
      (void*) AndroidBridge_CV_releaseView},
    {"startRenderThreadOnSurface",
      "(Ljava/lang/String;Ljava/lang/String;Landroid/view/Surface;)V",
      (void*) AndroidBridge_CV_startRenderThreadOnSurface},
    {"stopRenderThread",
      "(Ljava/lang/String;Ljava/lang/String;Landroid/view/Surface;)V",
      (void*) AndroidBridge_CV_stopRenderThread},
    {"setRenderBitmap",
      "(Ljava/lang/String;Landroid/graphics/Bitmap;)V",
      (void*) AndroidBridge_CV_setRenderBitmap},
    {"setNativeEffect",
      "(Ljava/lang/String;Lcom/ykode/compo/CompoView$Effect;)V",
      (void*) AndroidBridge_CV_setNativeEffect},
    {"getResultBitmap",
      "(Ljava/lang/String;)Landroid/graphics/Bitmap;",
      (void*) AndroidBridge_CV_getResultBitmap}
  };

  jclass CompoView_Clazz = env->FindClass(COMPO_CLASS(CompoView));

  if(env->RegisterNatives(CompoView_Clazz, viewMethods, viewMethodCounts) 
      == JNI_OK) 
  { 
    LOGI("CompoView Native Methods registered");
  }
  else {
    LOGE("CompoView Native Methods Registration Failed");
  }  
}

  CompoAndroidBridge::CompoAndroidBridge() 
: mVM(0), mAssetManager(0), mJavaAssetManager(0)
{
  pthread_mutex_init(&mBridgeMutex, 0);
}

CompoAndroidBridge::~CompoAndroidBridge()
{
  JNIEnv *env;
  mVM->AttachCurrentThread(&env, NULL);

  env->DeleteGlobalRef(mJavaAssetManager);
  mJavaAssetManager = 0;
  LOGI("Java Bridge Released and Destructed!");
  pthread_mutex_destroy(&mBridgeMutex);
  mVM->DetachCurrentThread();
}

void CompoAndroidBridge::attachAssetManager( jobject manager )
{
  JNIEnv *env;
  mVM->AttachCurrentThread(&env, NULL);

  if (mJavaAssetManager != 0) {
    LOGI("AssetManager already attached in [0x%X]", 
        (unsigned) mJavaAssetManager);
    // prevent recreation of bound activity
    return;
  }
  mJavaAssetManager = env->NewGlobalRef(manager);
  mAssetManager = AAssetManager_fromJava(env, manager);
  if (manager != 0) {
    LOGI("Global Asset Manager Attached from [0x%X] to [0x%X]", (unsigned) manager,
        (unsigned) mJavaAssetManager);
  }
  else {
    LOGE("Asset Manager Attachments Failed %x", (unsigned) manager);
  }
}

void CompoAndroidBridge::detachCurrentAssetManager()
{
  JNIEnv *env;
  mVM->AttachCurrentThread(&env, NULL);

  if (mJavaAssetManager == 0) {
    LOGE("No Asset Manager already detached!");
    return;
  }
  env->DeleteGlobalRef(mJavaAssetManager);

  LOGI("Global Asset Manager on [0x%X] detached!", (unsigned)
      mJavaAssetManager);

  mJavaAssetManager = 0;
  mAssetManager = 0;
}


void JNICALL AndroidBridge_initWithContext( JNIEnv *env,
    jclass,
    jobject contextObject)
{
  static jclass Context_Clazz = env->FindClass("android/content/Context");
  static jmethodID Context_getAssets_mid = env->GetMethodID(Context_Clazz,
      "getAssets", "()Landroid/content/res/AssetManager;");
  jobject assetManagerObject = env->CallObjectMethod(contextObject,
      Context_getAssets_mid);
  CompoBridge->attachAssetManager(assetManagerObject);
}

void JNICALL AndroidBridge_destroyWithContext( JNIEnv *env,
    jclass,
    jobject activityObject )
{
  CompoBridge->detachCurrentAssetManager(); 
}

static inline const std::string getCPUString() {
  static AndroidCpuFamily cpuFamily = android_getCpuFamily();
  static uint64_t features = android_getCpuFeatures();
  std::string cpuLog= "-> CPU:";

  if (cpuFamily == ANDROID_CPU_FAMILY_X86) {
    cpuLog += "x86";

    if (features & ANDROID_CPU_X86_FEATURE_SSSE3) {
      cpuLog += " ssse3";
    }

    if (features & ANDROID_CPU_X86_FEATURE_POPCNT) {
      cpuLog += " popcnt";
    }
    if (features & ANDROID_CPU_X86_FEATURE_MOVBE) {
      cpuLog += " movbe";
    }

  }
  else if (cpuFamily == ANDROID_CPU_FAMILY_MIPS) {
    cpuLog += "MIPS"; 
  }
  else if (cpuFamily == ANDROID_CPU_FAMILY_ARM) {
    if (features & ANDROID_CPU_ARM_FEATURE_ARMv7) {
      cpuLog += "ARMv7";
    }
    else {
      cpuLog += "ARM";
    }

    if (features & ANDROID_CPU_ARM_FEATURE_VFPv3) {
      cpuLog += " vfpv3";
    }

    if (features & ANDROID_CPU_ARM_FEATURE_NEON) {
      cpuLog += " none";
    }

  }
  else {
    cpuLog += "UNKNOWN";
  }

  return cpuLog;
}

