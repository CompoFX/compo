//
//  CompoEffect.cpp
//  CompoView
//
//  Created by Didiet Noor on 20/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#undef GLM_FORCE_RADIANS

#include "CompoEffect.h"

#include "CompoPhotoQuad.h"
#include "CompoShaderProgram.h"

static const glm::mat4 defaultProjection(
    glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));

CompoEffect::CompoEffect( const CompoShaderProgram &program ) :
  mEffectProgram(program), mMVPMatrix(defaultProjection)
{
  LOGI("--->> Effect [0x%" PRIXPTR "] Created With Shader Program %d",
      (intptr_t) this, program.programID());
}

CompoEffect::~CompoEffect()
{
  LOGI("--->> CompoEffect [0x%" PRIXPTR "] destroyed", (intptr_t) this);
}

void CompoEffect::assignTexture( const int samplerID, const CompoTexture &texture)
{
  if (samplerID <= 0 || samplerID > 8) {
    std::cerr << "Sampler ID cannet be zero or negative and due to between 1..8"
                  " due to hardware limitation." << std::endl;
  }
  mSupportingTextures.insert(CompoTexturePair(samplerID, texture));
}

void CompoEffect::activate(const CompoPhotoQuad &quad) const {
  
  mEffectProgram.activate();
  
  const GLint loc = mEffectProgram.getUniformLocation("mvpMatrix");
  
  glUniformMatrix4fv(loc, 1, false, glm::value_ptr(mMVPMatrix));

  quad.mTexture->activateWithProgram(mEffectProgram, 0);
  
  if (!mSupportingTextures.empty()) {
    CompoTextureMap::const_iterator it = mSupportingTextures.begin();
  
    for (; it != mSupportingTextures.end(); ++it )
    {
      it->second.activateWithProgram(mEffectProgram, it->first);
    }
  }
}
