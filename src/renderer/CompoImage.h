//
//  CompoImage.h
//  CompoView
//
//  Created by Didiet Noor on 18/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_IMAGE_H
#define COMPO_IMAGE_H

#include "CompoTypes.h"

// image class for RGBA 32-bit images
class CompoImage {
  struct Private;
  COMPO_SHARED<Private> d;
public:
  CompoImage( const std::string &relativePath );
  CompoImage( void *nativeImageObject );
  // data will be copied
  CompoImage( const int width, const int height, void *imageData, bool isCopied = false);
  int width() const;
  int height() const;
  int bpp() const;
  const unsigned char *pixels() const;
  size_t dataSize() const;
  virtual ~CompoImage();
  CompoImage( const CompoImage& );

  CompoImage &operator=( const CompoImage & );

  static CompoImage fromPNGData( const unsigned char * data, const size_t dataSize);
  static CompoImage fromJPGData( const unsigned char * data, const size_t dataSize);
private:
  CompoImage();
};
#endif
