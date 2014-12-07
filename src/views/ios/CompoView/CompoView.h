//
//  CompoView.h
//  CompoView
//
//  Created by Didiet Noor on 17/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//
//

#import <UIKit/UIKit.h>

/** 
 * The possible effects can be used as Composition
 *
 * @since v1.0
 */
typedef NS_ENUM(NSUInteger, CompoViewEffectType){
  kCompoEffectNone = 0,
  kCompoEffectNormal = 0,
  kCompoEffectGreyScale,
  kCompoEffectLighten,
  kCompoEffectSepia,
  kCompoEffectHitamPutih,
  kCompoEffectScribe,
  kCompoEffectHike,
  kCompoEffectCinta,
  kCompoEffectAnalog,
  kCompoEffectThermo,
  kCompoEffectDesaku,
  kCompoEffectKelud,
  kCompoEffectHero,
  kCompoEffectPencil,
  kCompoEffectStylish,
  kCompoEffectLordKelvin,
  kCompoEffectHudson,
  kCompoEffectPixelate
};

/**
 * The CompoView class provides an implementation of `UIView` with
 * custom rendering implemented in OpenGL ES 2.0
 *
 * @since v1.0
 */
@interface CompoView : UIView

/**
 The rendering framerate of the CompoView
 */
@property (nonatomic) float frameRate;

/**
 To stop rendering routines to be executed and save battery life
 */
@property (nonatomic) BOOL paused;

/**
 The image attached to CompoView as base image
 */
@property (weak, nonatomic) UIImage *image;

/**
 The resulting composited image default to 512x512
 */
@property (weak, atomic, readonly) UIImage *resultImage;

/**
 Active effect
 */
@property (nonatomic) CompoViewEffectType effect;

/**
 Get result image with specific with and height
 */

- (UIImage*) resultImageWithSize: (CGSize) size;

@end
