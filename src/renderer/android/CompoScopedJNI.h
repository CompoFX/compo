#ifndef COMPO_SCOPED_JNI_H
#define COMPO_SCOPED_JNI_H

#include <jni.h>

class CompoScopedJNI
{
public:
  inline CompoScopedJNI(JavaVM *vm, JNIEnv ** env);
  inline ~CompoScopedJNI();
private:
  // no copy constructor and default constructor
  CompoScopedJNI();
  CompoScopedJNI(const CompoScopedJNI &);
 
private: 
  JavaVM *mVM;
  const int mStat;
};

inline CompoScopedJNI::CompoScopedJNI(JavaVM *vm, JNIEnv ** env):
  mVM(vm),
  mStat(mVM->GetEnv(reinterpret_cast<void**>(env), JNI_VERSION_1_6))
{
  if (JNI_EDETACHED == mStat) {
    mVM->AttachCurrentThread(env, NULL);
    LOGI("<JNI> Thread Attached from Detached state");
  }
  else {
    LOGI("<JNI> Thread already Attached");
  }
}

inline CompoScopedJNI::~CompoScopedJNI()
{
  if (JNI_EDETACHED == mStat) {
    mVM->DetachCurrentThread();
    LOGI("<JNI> Detached back");
  }
  else {
    LOGI("<JNI> Thread already JNI Thread NO-OP");
  }
}

#endif // COMPO_SCOPED_JNI_H


