//
//  CompoTexture.cpp
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#include "CompoTexture.h"

#include "CompoImage.h"
#include "CompoShaderProgram.h"

const static std::string texBaseName = "tex";

static inline GLuint initTexture()
{
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  return texID;
}

CompoTexture::CompoTexture() :
mpTextureID(COMPO_MAKE_SHARED<GLuint>(initTexture()))
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

CompoTexture::CompoTexture(const CompoImage &image):
mpTextureID(COMPO_MAKE_SHARED<GLuint>(initTexture()))
{
  const GLint width = image.width();
  const GLint height = image.height();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, image.pixels());
  glFlush();

  if (glGetError() != GL_NO_ERROR) {
    LOGE("Error on transferring data");
    LOGE("<<----Failed Creating Texture");
    return;
  }
  else {
    LOGI("---->>Texture Created %d", *mpTextureID);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
}

CompoTexture::CompoTexture( const CompoTexture &other )
{
  mpTextureID = other.mpTextureID;
}

CompoTexture::~CompoTexture()
{
  if (mpTextureID.unique()) {
    LOGI("<<---- Texture %d destroyed", *mpTextureID);
    glDeleteTextures(1, mpTextureID.get());
  }
}

bool CompoTexture::isValid() const
{
  return (*mpTextureID != 0);
}

void CompoTexture::activateWithProgram(const CompoShaderProgram &program, const GLuint sampler) const
{
  if (!isValid()) {
    std::cerr << "[OpenGL Texture Invalid ID]" << std::endl;
    return;
  }
  char idStr[2] ={'0','\0'};
  idStr[0] += sampler;
  const std::string texUniformName = texBaseName + idStr;
  const GLuint texUniform = program.getUniformLocation(texUniformName);

  GLint err = glGetError();
  if (err == GL_NO_ERROR) {
    glActiveTexture(GL_TEXTURE0 + sampler);
    glBindTexture(GL_TEXTURE_2D, *mpTextureID);
    glUniform1i(texUniform, sampler);
  }
  else {
    LOGE("Error Activating Texture %s", texUniformName.c_str());
  }

}
