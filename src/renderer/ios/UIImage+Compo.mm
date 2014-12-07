//
//  UIImage+ImageFromBundleName.m
//  CompoView
//
//  Created by Didiet Noor on 20/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#import "UIImage+Compo.h"
#import "CompoUtil.h"
#import "CompoImage.h"

@implementation UIImage (Compo)
+ (UIImage*) imageFromBundleName: (const std::string &) name
{
  const std::string resPath = getFullPathFromResource(name);
  NSString *fullPath = [NSString stringWithUTF8String:resPath.c_str()];
  
  return [UIImage imageWithContentsOfFile:fullPath];
}

+ (UIImage*) fixedOrientationImageFromImage: (UIImage*) img
{
  if (img.imageOrientation == UIImageOrientationUp) {
    return img;
  }
  
  CGAffineTransform transform = CGAffineTransformIdentity;
  const CGSize &sz = img.size;
  
  switch (img.imageOrientation) {
    case UIImageOrientationDown:
    case UIImageOrientationDownMirrored:
      transform = CGAffineTransformTranslate(transform, sz.width, sz.height);
      transform = CGAffineTransformRotate(transform, M_PI);
      break;
    case UIImageOrientationLeft:
    case UIImageOrientationLeftMirrored:
      transform = CGAffineTransformTranslate(transform, sz.width, 0);
      transform = CGAffineTransformRotate(transform, M_PI_2);
      break;
      
    case UIImageOrientationRight:
    case UIImageOrientationRightMirrored:
      transform = CGAffineTransformTranslate(transform, 0, sz.height);
      transform = CGAffineTransformRotate(transform, -M_PI_2);
      break;
    case UIImageOrientationUp:
    case UIImageOrientationUpMirrored:
    default:
      break;
  }
  
  switch (img.imageOrientation) {
    case UIImageOrientationUpMirrored:
    case UIImageOrientationDownMirrored:
      transform = CGAffineTransformTranslate(transform, sz.width, 0);
      transform = CGAffineTransformScale(transform, -1, 1);
      break;
      
    case UIImageOrientationLeftMirrored:
    case UIImageOrientationRightMirrored:
      transform = CGAffineTransformTranslate(transform, sz.height, 0);
      transform = CGAffineTransformScale(transform, -1, 1);
      break;
    case UIImageOrientationUp:
    case UIImageOrientationDown:
    case UIImageOrientationLeft:
    case UIImageOrientationRight:
      break;
  }
  
  CGImageRef imageRef = img.CGImage;
  CGContextRef ctx = CGBitmapContextCreate(NULL, sz.width, sz.height,
                                           CGImageGetBitsPerComponent(imageRef),
                                           0,
                                           CGImageGetColorSpace(imageRef),
                                           CGImageGetBitmapInfo(imageRef));
  CGContextConcatCTM(ctx, transform);
  switch (img.imageOrientation) {
    case UIImageOrientationLeft:
    case UIImageOrientationLeftMirrored:
    case UIImageOrientationRight:
    case UIImageOrientationRightMirrored:
      // Grr...
      CGContextDrawImage(ctx, CGRectMake(0,0,sz.height,sz.width), imageRef);
      break;
      
    default:
      CGContextDrawImage(ctx, CGRectMake(0,0,sz.width,sz.height), imageRef);
      break;
  }
  
  // And now we just create a new UIImage from the drawing context
  CGImageRef cgimg = CGBitmapContextCreateImage(ctx);
  UIImage *result = [UIImage imageWithCGImage:cgimg];
  CGImageRelease(cgimg);
  CGContextRelease(ctx);
  return result;
}

+ (UIImage*) imageFromCompoImage:(const CompoImage &) img
{
  static const int bitsPerComponent = 8;
  static const int bitsPerPixel = 32;
  const int bytesPerRow = (img.bpp() >> 3) * img.width();
  
  CGDataProviderRef provider =
    CGDataProviderCreateWithData(NULL, img.pixels(), img.dataSize(), NULL);
  CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
  CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
  CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
  
  CGImageRef imageRef = CGImageCreate(img.width(),
                                      img.height(),
                                      bitsPerComponent,
                                      bitsPerPixel,
                                      bytesPerRow,
                                      colorSpaceRef,
                                      bitmapInfo,
                                      provider,
                                      NULL,
                                      NO, renderingIntent);
  
  UIImage *resultImage = [UIImage imageWithCGImage:imageRef];
  
  CGImageRelease(imageRef);
  CGColorSpaceRelease(colorSpaceRef);
  CGDataProviderRelease(provider);
  
  return resultImage;
}

+ (UIImage*) imageFromImage:(UIImage *)source withCropRect:(CGRect)cropRect
{
  CGImageRef sourceRef = source.CGImage;
  CGImageRef resultRef = CGImageCreateWithImageInRect(sourceRef, cropRect);
  
  UIImage *resultImage = [UIImage imageWithCGImage:resultRef];
  CGImageRelease(resultRef);
  
  return resultImage; 
}

+ (UIImage*) imageWithPNGRawData:(void *)pngData andDataSize:(size_t)dataSize
{
  CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, pngData, dataSize, NULL);
  CGImageRef resultRef = CGImageCreateWithPNGDataProvider(provider, NULL, NO,
                                                          kCGRenderingIntentDefault);
  UIImage *resultImage = [UIImage imageWithCGImage:resultRef];
  CGImageRelease(resultRef);
  CGDataProviderRelease(provider);
  
  return resultImage;
}

+ (UIImage*) imageWithJPGRawdata: (void*) jpegData andDataSize: (size_t) dataSize;
{
  CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, jpegData, dataSize, NULL);
  CGImageRef resultRef = CGImageCreateWithJPEGDataProvider(provider, NULL, NO,
                                                          kCGRenderingIntentDefault);
  UIImage *resultImage = [UIImage imageWithCGImage:resultRef];
  CGImageRelease(resultRef);
  CGDataProviderRelease(provider);
  
  return resultImage;
}

@end
