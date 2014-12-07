//
//  UIImage+ImageFromBundleName.h
//  CompoView
//
//  Created by Didiet Noor on 20/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <string>

class CompoImage;

@interface UIImage (Compo)
+ (UIImage*) imageFromBundleName: (const std::string &) name;
+ (UIImage*) imageFromCompoImage: (const CompoImage &) compoImage;
+ (UIImage*) fixedOrientationImageFromImage: (UIImage*) originalImage;
+ (UIImage*) imageFromImage: (UIImage *) source withCropRect: (CGRect) cropRect;
+ (UIImage*) imageWithPNGRawData: (void*) pngData andDataSize: (size_t) dataSize;
+ (UIImage*) imageWithJPGRawdata: (void*) jpegData andDataSize: (size_t) dataSize;
@end
