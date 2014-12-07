//
//  CompoPhotoQuad.h
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_PHOTO_QUAD
#define COMPO_PHOTO_QUAD
#include "CompoTypes.h"

class CompoPhotoQuad
{
  friend class CompoEffect;
private:
  union {
      GLuint mGPUBuffers[2];
    struct {
      GLuint mVBO;
      GLuint mIBO;
    };
  };
  COMPO_UNIQUE<CompoTexture> mTexture;
  void initialize();
  CompoPhotoQuad( const CompoPhotoQuad &);
public:
  CompoPhotoQuad();
  CompoPhotoQuad(const CompoImage &);
  virtual ~CompoPhotoQuad();

  void setImage( const CompoImage &);

  void drawWithEffect( const CompoEffect &effect ) const;
};

#endif /* defined(__CompoView__CompoPhotoQuad__) */
