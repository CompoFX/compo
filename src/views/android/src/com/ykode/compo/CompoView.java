package com.ykode.compo;

import android.view.SurfaceView;
import android.view.SurfaceHolder;
import android.view.Surface;

import android.content.Context;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Matrix;

import android.util.AttributeSet;
import android.util.Log;

import java.util.UUID;

class CompoView extends SurfaceView { 
  
  public enum Effect {
    NORMAL, GREYSCALE, LIGHTEN, SEPIA, HITAMPUTIH,
    SCRIBE, HIKE, CINTA, ANALOG, THERMO, DESAKU,
    KELUD, HERO, PENCIL, STYLISH, LORDKEVIN, HUDSON,
    PIXELATE
  }
  
  public CompoView (Context context) {
    super(context);
    mViewUUID = UUID.randomUUID();
  }

  public CompoView (Context context, AttributeSet attrs) {
    super(context, attrs);
    mViewUUID = UUID.randomUUID();
  }

  @Override
  public void onAttachedToWindow()
  {
    super.onAttachedToWindow();
    CompoBridge.initWithContext(this.getContext());   
    registerView(mViewUUID.toString());
    initSurfaceView();
  }

  @Override
  public void onDetachedFromWindow()
  {
    releaseView(mViewUUID.toString());
    CompoBridge.destroyWithContext(this.getContext());
    super.onDetachedFromWindow();
  }

  private void initSurfaceView() {
    registerView(mViewUUID.toString());
    SurfaceHolder.Callback cvsc = new SurfaceHolder.Callback() {
      private UUID mUUID;
      public void surfaceChanged( SurfaceHolder holder,
          int format, int width, int height)
      {
        Log.v("[COMPOJAVASIDE]", "Surface Changed to " +
            width + "x" + height);
      }

      public void surfaceCreated( SurfaceHolder holder )
      {
        mUUID = UUID.randomUUID();
        startRenderThreadOnSurface( mViewUUID.toString(),
            mUUID.toString(),
            holder.getSurface());
      }

      public void surfaceDestroyed( SurfaceHolder holder )
      {
        stopRenderThread( mViewUUID.toString(),
            mUUID.toString(),
            holder.getSurface());
        
      }
    };
    this.getHolder().addCallback(cvsc);
  }

  @Override
  protected void onDraw(Canvas c)
  {
    // this is used for testing the view on designer
    c.drawARGB(255, 255, 0, 0);
  }


  @Override
  protected void onMeasure(int widthMeasureSpec, 
      int heightMeasureSpec) 
  {
    final int minDimension = 100;

    int width, height;

    int widthMode = MeasureSpec.getMode(widthMeasureSpec);
    int widthSize = MeasureSpec.getSize(widthMeasureSpec);
    int heightMode = MeasureSpec.getMode(heightMeasureSpec);
    int heightSize = MeasureSpec.getSize(heightMeasureSpec);

    int squareDimension = Math.min(widthSize, 
        heightSize);

    if (widthMode == MeasureSpec.EXACTLY) {
      width = squareDimension;
    }
    else if (widthMode == MeasureSpec.AT_MOST) {
      width = Math.max(minDimension, squareDimension);
    }
    else {
      width = minDimension;
    }

    if (heightMode == MeasureSpec.EXACTLY) {
      height = squareDimension;
    }
    else if (heightMode == MeasureSpec.AT_MOST) {
      height = Math.max(minDimension, squareDimension);
    }
    else {
      height = minDimension;
    }
    setMeasuredDimension(width, height);
  }

  private native void registerView(String vid);

  private native void releaseView(String vid);

  private native void startRenderThreadOnSurface( String vid, 
      String tid,
      Surface surface);
  private native void stopRenderThread( String vid,
      String tid,
      Surface surface);

  private native void setRenderBitmap( String vid, Bitmap bitmap);
  private native void setNativeEffect( String vid, Effect effect);
  private synchronized native Bitmap getResultBitmap( String vid ); 

  private Bitmap mBitmap;
  private Effect mEffect;  
  private SurfaceHolder.Callback mSurfaceCallback;
  private UUID mViewUUID;  
  
  public void setEffect(Effect fx) {
    synchronized(this) {
      mEffect = fx;
      this.setNativeEffect(mViewUUID.toString(), fx);
    }
  }

  public synchronized Bitmap getResultImage()
  {
    Bitmap flipped = this.getResultBitmap(mViewUUID.toString());
    Matrix flipMatrix = new Matrix();
    flipMatrix.preScale(1.0f, -1.0f);
    return Bitmap.createBitmap(flipped, 0, 0, flipped.getWidth(), 
        flipped.getHeight(), flipMatrix, true);
  }
  public Effect getEffect() {
    return mEffect;
  }

  public void setBitmap(Bitmap bitmap) {
    synchronized(this) {
      mBitmap = bitmap;
      this.setRenderBitmap(mViewUUID.toString(), mBitmap);
    }
  }

  public Bitmap getBitmap() {
    return mBitmap;
  }
}


