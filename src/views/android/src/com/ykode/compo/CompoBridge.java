package com.ykode.compo;

import android.app.Activity;
import android.content.Context;
import android.view.SurfaceView;

class CompoBridge 
{ 
  public static native void initWithContext(Context context);
  public static native void destroyWithContext(Context context);
  static {
    System.loadLibrary("Compo");
  }
}
