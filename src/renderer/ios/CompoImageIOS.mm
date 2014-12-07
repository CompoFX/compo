//
//  CompoImage.mm
//  CompoView
//
//  Created by Didiet Noor on 18/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#import <UIKit/UIImage.h>
#import "UIImage+Compo.h"
#import "CompoImage.h"

#import "../Compoimage.inl"

CompoImage::CompoImage( const std::string &relativePath ) :
  d (std::make_shared<Private>())
{
  dispatch_sync(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
    UIImage *image = [UIImage imageFromBundleName:relativePath];
      CGImageRef imageRef = [image CGImage];
    d->width = (int)CGImageGetWidth(imageRef);
    d->height = (int)CGImageGetHeight(imageRef);
    d->bpp = 32;
    d->pixelData = (unsigned char*) calloc(d->dataSize(), sizeof(unsigned char));
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(d->pixelData, d->width, d->height,
                                                 8, d->stride(), colorSpace,
                                                  kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
  
    CGContextDrawImage(context, CGRectMake(0, 0, d->width, d->height), imageRef);
    CGContextRelease(context);
  });
}

CompoImage::CompoImage(void *imagePtr) :
d (std::make_shared<Private>())
{
  dispatch_sync(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
    UIImage *image = (__bridge UIImage*)imagePtr;
    CGImageRef imageRef = [image CGImage];
    d->width = (int)CGImageGetWidth(imageRef);
    d->height = (int) CGImageGetHeight(imageRef);
    d->bpp = 32;
    d->pixelData = (unsigned char*) calloc(d->dataSize(), sizeof(unsigned char));
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(d->pixelData, d->width, d->height,
                                                 8, d->stride(), colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    
    CGContextDrawImage(context, CGRectMake(0, 0, d->width, d->height), imageRef);
    CGContextRelease(context);
  });
}

CompoImage CompoImage::fromPNGData(const unsigned char *data, const size_t dataSize)
{
  return CompoImage( (__bridge void*) [UIImage imageWithPNGRawData:(void*)data
                                                       andDataSize:dataSize]);
}