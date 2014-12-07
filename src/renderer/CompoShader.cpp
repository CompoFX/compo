//
//  CompoShader.cpp
//  CompoView
//
//  Created by Didiet Noor on 18/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//


#include "CompoShader.h"
#include "CompoShaderInfoLog.h"

CompoShader::CompoShader(const std::string &source, const CompoShaderType kind )
:mShaderID(-1)
{
  const char *src = source.c_str();
  const int  length = static_cast<int>(source.length());
  
  GLint shaderType = (GLint) kind;
  mShaderID = glCreateShader(shaderType);
  
  glShaderSource(mShaderID, 1, &src , &length);
  glCompileShader(mShaderID);
  
  if (!isCompileSuccess()) {
    const std::string err = 
      CompoShaderInfoLog::fromShader(*this).getLogString();
    LOGE("Error creating shader: %s", err.c_str());
    glDeleteShader(mShaderID);
    mShaderID = -1;
  }
  else {
    if (kind == COMPO_SHADER_VERTEX) {
      LOGI("---->> Success creating vertex shader; %d", mShaderID);
    } 
    else if (kind == COMPO_SHADER_FRAGMENT) {
      LOGI("---->> Success creating fragment shader; %d", mShaderID);
    }
  }
  glFlush();
}

CompoShader::CompoShader( const CompoShader &other )
: mShaderID(other.mShaderID)
{
}

CompoShader::~CompoShader() {
  if (isValid()) {
    glDeleteShader(mShaderID);
    LOGI("---->> Shader %d destroyed", mShaderID);
    mShaderID = -1;
  }
}

bool CompoShader::isCompileSuccess() const
{
  if (mShaderID <= 0) {
    return false;
  }
  else {
    GLint result;
    glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &result);
    return (result != GL_FALSE);
  }
}

bool CompoShader::isValid() const
{
  if (mShaderID == -1) {
    return false;
  }
  else {
    return (glIsShader(mShaderID) == GL_TRUE);
  }
}
