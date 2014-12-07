//
//  CompoEffect.h
//  CompoView
//
//  Created by Didiet Noor on 20/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_EFFECT_H
#define COMPO_EFFECT_H

#include "CompoTypes.h"
#include "CompoTexture.h"
#include "CompoRenderState.h"
#include "CompoShaderProgram.h"

#include "glm/glm.hpp"

class CompoEffect {
  typedef std::map<int, CompoTexture> CompoTextureMap;
  typedef std::pair<int, CompoTexture> CompoTexturePair;
  
  CompoShaderProgram mEffectProgram;
  CompoTextureMap mSupportingTextures;
  glm::mat4 mMVPMatrix;
  
public:
  CompoEffect( const CompoShaderProgram &program );
  virtual ~CompoEffect();
  void assignTexture( const int samplerID, const CompoTexture &texture);
  virtual void activate( const CompoPhotoQuad &quad) const;
  void setMVPMatrix( const glm::mat4 &mvpMatrix);
};

#endif /* defined(__CompoView__CompoEffect__) */
