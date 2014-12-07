//
//  CompoPhotoQuad.cpp
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#undef GLM_FORCE_RADIANS

#include "CompoPhotoQuad.h"
#include "CompoShader.h"
#include "CompoShaderProgram.h"
#include "CompoTexture.h"
#include "CompoEffect.h"
#include "CompoImage.h"
#include "CompoTexture.h"

void CompoPhotoQuad::initialize() {
  const float CompoQuadVertices[] = {
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
  };

  const unsigned char CompoQuadIndices[] = {
    0, 1, 3, 0, 3, 2
  };


  glGenBuffers(2, mGPUBuffers);

  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CompoQuadVertices),
      CompoQuadVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CompoQuadIndices),
      CompoQuadIndices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

CompoPhotoQuad::CompoPhotoQuad()
{
  initialize();
}

CompoPhotoQuad::CompoPhotoQuad(const CompoImage &image)
  : mTexture( new CompoTexture(image))
{
  initialize();
}

CompoPhotoQuad::~CompoPhotoQuad()
{
  glDeleteBuffers(2, mGPUBuffers);
}

void CompoPhotoQuad::setImage(const CompoImage &img)
{
  COMPO_UNIQUE<CompoTexture> newTexture(new CompoTexture(img));
  mTexture.swap(newTexture);
}

void CompoPhotoQuad::drawWithEffect(const CompoEffect &effect) const
{
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * 4, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * 4, (GLvoid*)12);

  if (mTexture) {
    effect.activate(*this);
  }

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
}

