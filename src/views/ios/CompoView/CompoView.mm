//
//  CompoView.m
//  CompoView
//
//  Created by Didiet Noor on 17/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#include <cstdint>

#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>

#import "UIImage+Compo.h"

#import "CompoView.h"
#import "CompoComposition.h"
#import "CompoImage.h"
#import "CompoScreenOutput.h"
#import "CompoRenderBufferOutput.h"

@interface CompoView() {
  
  EAGLContext *context;
  
  float timeStamp;
  CADisplayLink *displayLink;
  
  CompoViewEffectType mEffect;
  CompoComposition *mCompo;
  
  CompoScreenOutput *mScreenOutput;
  
  dispatch_semaphore_t compoSemaphore;
  dispatch_queue_t compoQueue;
  BOOL imageSet;
}

- (BOOL) openGLInit: (CGRect) frame;
- (BOOL) openGLInit: (CGRect) frame withFPS: (float) fps;
- (void) doFrame: (CADisplayLink*) displayLink;
- (void) initGCD;
- (UIImage*) image;

@end

@implementation CompoView

@synthesize frameRate;
@synthesize effect = mEffect;

+ (Class) layerClass
{
  return [CAEAGLLayer class];
}

- (id) init
{
  [NSException raise:NSInternalInconsistencyException
              format:@"CompoView cannot be initialized without frame"];
  return nil;
}


- (void) initGCD
{
  compoSemaphore = dispatch_semaphore_create(1);
  compoQueue = dispatch_queue_create("compoQueue",NULL);
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
      imageSet = NO;
      [self initGCD];
      if (![self openGLInit:frame]) {
        return nil;
      }
      [self doFrame:nil];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
  self = [super initWithCoder:aDecoder];
  if (self) {
    imageSet = NO;
    [self initGCD];
    if (![self openGLInit:self.frame]) {
      return nil;
    }
    [self doFrame:nil];
  }
  return self;
}

- (void) dealloc
{
  if (mCompo) {
    delete mCompo; mCompo = 0;
  }
  
  if (mScreenOutput) {
    delete mScreenOutput; mScreenOutput = 0;
  }
}

- (BOOL) openGLInit:(CGRect)frame
{
  return [self openGLInit:frame withFPS:15];
}

- (BOOL) openGLInit: (CGRect) frame withFPS: (float) fps
{
  self.frameRate = fps;
  self.paused = NO;
  
  CAEAGLLayer *gpuLayer = (CAEAGLLayer*) self.layer;
  gpuLayer.opaque = YES;
  gpuLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithBool:NO],
                                 kEAGLDrawablePropertyRetainedBacking,
                                 kEAGLColorFormatRGBA8,
                                 kEAGLDrawablePropertyColorFormat,
                                 nil];
  
  context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
  
  if (!context || ![EAGLContext setCurrentContext:context]) {
    return NO;
  }
  
  timeStamp = CACurrentMediaTime();
  
  displayLink = [CADisplayLink displayLinkWithTarget:self
                                            selector:@selector(doFrame:)];
  displayLink.frameInterval = 1;
  [displayLink addToRunLoop:[NSRunLoop currentRunLoop]
                    forMode:NSDefaultRunLoopMode];
  
  mScreenOutput = new CompoScreenOutput(CGRectGetWidth(frame), CGRectGetHeight(frame), context, gpuLayer);
  mCompo = new CompoComposition;
  dispatch_semaphore_signal(compoSemaphore);

  return YES;
}

- (void) setHidden:(BOOL)hidden
{
  [self setPaused:hidden];
  [super setHidden:hidden];
}

- (void) setPaused:(BOOL)paused_
{
  if (paused_ != displayLink.paused) {
    if (paused_ == NO) {
      timeStamp = CACurrentMediaTime();
    }
    displayLink.paused = paused_;
  }
}

- (BOOL) paused
{
  return displayLink.paused;
}

- (void) setEffect:(CompoViewEffectType)effect
{
  if (mEffect != effect) {
    dispatch_semaphore_wait(compoSemaphore, DISPATCH_TIME_FOREVER);
    mCompo->setActiveEffect((const CompoComposition::CompoCompositionEffect) effect);
    dispatch_semaphore_signal(compoSemaphore);
  }
  mEffect = effect;
}

- (void) setImage:(UIImage *)image
{
  UIImage *fixedImage = [UIImage fixedOrientationImageFromImage:image];
  dispatch_semaphore_wait(compoSemaphore, DISPATCH_TIME_FOREVER);
    mCompo->setImage(CompoImage((__bridge void*)fixedImage));
  imageSet = YES;
  dispatch_semaphore_signal(compoSemaphore);
}

- (void) setValue:(id)value forUndefinedKey:(NSString *)key
{
  if ([key isEqualToString:@"imageFileName"]) {
    UIImage *setterImage = [UIImage imageFromBundleName:[(NSString*)value UTF8String]];
    [self setImage:setterImage];
  }
  else {
    [super setValue:value forUndefinedKey:key];
  }
}

- (void) setValue:(id)value forKey:(NSString *)key
{
  if ([key isEqualToString:@"imageFileName"]) {
    UIImage *setterImage = [UIImage imageFromBundleName:[(NSString*)value UTF8String]];
    [self setImage:setterImage];
  }
  else {
    [super setValue:value forKey:key];
  }
}

- (UIImage*) resultImage
{
  return [self resultImageWithSize:CGSizeMake(512, 512)];
}

- (UIImage*) resultImageWithSize:(CGSize)size
{
  const int imageWidth  = floor(size.width);
  const int imageHeight = floor(size.height);
  const int imageStride = imageWidth * 4;
  const int imageByteSize = imageHeight * imageStride;

  uint8_t *imageBuffer;
  dispatch_semaphore_wait(compoSemaphore, DISPATCH_TIME_FOREVER);
  {
    imageBuffer = (uint8_t*) malloc( imageByteSize );
    dispatch_sync(compoQueue, ^{
      [EAGLContext setCurrentContext:context];
      CompoRenderBufferOutput offscreenBuffer(imageWidth, imageHeight);
      offscreenBuffer.render(*mCompo);
      glReadPixels(0, 0, imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
    });
  }
  dispatch_semaphore_signal(compoSemaphore);
  CGDataProviderRef ref = CGDataProviderCreateWithData(0, imageBuffer, imageByteSize, 0);
  CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
  CGImageRef imgRef = CGImageCreate(imageWidth,
                                    imageHeight,
                                    8, 32,
                                    imageStride,
                                    colorSpace,
                                    kCGImageAlphaLast | kCGBitmapByteOrderDefault,
                                    ref, 0, true, kCGRenderingIntentDefault);
  CGColorSpaceRelease(colorSpace);
  
  uint8_t *contextBuffer = (uint8_t*) calloc(imageByteSize, sizeof(uint8_t));
  CGContextRef imgContext = CGBitmapContextCreate(contextBuffer,
                              imageWidth, imageHeight,
                              8,
                              imageStride,
                              CGImageGetColorSpace(imgRef),
                              kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
  
  CGContextTranslateCTM(imgContext, 0.0, imageHeight);
  CGContextScaleCTM(imgContext, 1.0, -1.0);
  CGContextDrawImage(imgContext, CGRectMake(0.0, 0.0, imageWidth, imageHeight), imgRef);
  
  CGImageRef outputRef = CGBitmapContextCreateImage(imgContext);

  UIImage *img = [[UIImage alloc] initWithCGImage:outputRef];
  
  CGImageRelease(outputRef);
	CGContextRelease(imgContext);
	CGImageRelease(imgRef);
	CGDataProviderRelease(ref);
  
	free(contextBuffer);
	free(imageBuffer);
  
  return img;
}

- (void) doFrame:(CADisplayLink *)displayLink_
{
  if (displayLink_ != nil) {
    const float deltaTime = displayLink_.timestamp - timeStamp;
    const float frameTime = 1.0f / self.frameRate;
    
    if (deltaTime >= frameTime) {
      timeStamp = displayLink_.timestamp;
      
      if (imageSet) {
        if (dispatch_semaphore_wait(compoSemaphore, DISPATCH_TIME_NOW) != 0) {
          return;
        }
        dispatch_async(compoQueue, ^{
          [EAGLContext setCurrentContext:context];
          mScreenOutput->render(*mCompo);
          mScreenOutput->swap();
          dispatch_semaphore_signal(compoSemaphore);
        });
      }
    }
  }
  else {
    if (dispatch_semaphore_wait(compoSemaphore, DISPATCH_TIME_NOW) != 0) {
      return;
    }
    dispatch_async(compoQueue, ^{
      [EAGLContext setCurrentContext:context];
      glClear(GL_COLOR_BUFFER_BIT);
      mScreenOutput->swap();
      dispatch_semaphore_signal(compoSemaphore);
    });
  }
}

- (UIImage*) image
{
  [NSException raise:NSInternalInconsistencyException format:@"property image from CompoView is not available for reading from CompoView"];
  return nil;
}
@end
