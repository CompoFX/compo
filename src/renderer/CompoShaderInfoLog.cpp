//
//  CompoShaderInfoLog.cpp
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#include "CompoShaderInfoLog.h"

#include "CompoShader.h"
#include "CompoShaderProgram.h"

static const size_t COMPO_SHADER_LOG_GROW_SIZE = 1024;

CompoShaderInfoLog CompoShaderInfoLog::instance;

CompoShaderInfoLog::CompoShaderInfoLog():
  mCurrentLogSize(COMPO_SHADER_LOG_GROW_SIZE),
  mLogBuffer((char*)malloc(mCurrentLogSize))
{
    mLogBuffer[0] = '\0';
}

void CompoShaderInfoLog::reallocTo(const size_t targetSize)
{
  if (targetSize > mCurrentLogSize) {
    while (targetSize > mCurrentLogSize) {
      mCurrentLogSize += COMPO_SHADER_LOG_GROW_SIZE;
    }
    mLogBuffer = (char*) realloc(mLogBuffer, mCurrentLogSize);
  }
}

void CompoShaderInfoLog::getFromShader(const CompoShader &shader)
{
  GLint logSize;
  const GLint shaderID = shader.shaderID();
  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logSize);
  reallocTo(logSize);
  glGetShaderInfoLog(shaderID, (GLsizei)mCurrentLogSize, &logSize, mLogBuffer);
  mLogBuffer[logSize] = '\0';
}

void CompoShaderInfoLog::getFromProgram(const CompoShaderProgram &program)
{
  GLint logSize;
  const GLint programID = program.programID();
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);
  reallocTo(logSize);
  glGetProgramInfoLog(programID, (GLsizei)mCurrentLogSize, &logSize, mLogBuffer);
  mLogBuffer[logSize] = '\0';
}

CompoShaderInfoLog::~CompoShaderInfoLog()
{
  if(mLogBuffer) {
    free(mLogBuffer); mLogBuffer = 0;
  }
}

const CompoShaderInfoLog &CompoShaderInfoLog::fromShader(const CompoShader &shader) {
  instance.getFromShader(shader);
  return instance;
}

const CompoShaderInfoLog &CompoShaderInfoLog::fromProgram(const CompoShaderProgram &program)
{
  instance.getFromProgram(program);
  return instance;
}
