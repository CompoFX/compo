//
//  CompoShaderProgram.cpp
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#include "CompoShaderProgram.h"

#include "CompoShader.h"
#include "CompoShaderInfoLog.h"

CompoShaderProgram::CompoShaderProgram(const CompoShader &vertexShader,
                                       const CompoShader &fragmentShader,
                                       const bool prebindAttributes) :
CompoRenderState(),
mpProgramID(COMPO_MAKE_SHARED<GLint>(0))
{
  GLint linkedStatus;
  if (!vertexShader.isValid() || !fragmentShader.isValid()) {
    return;
  }
  *mpProgramID = glCreateProgram();
  glAttachShader(*mpProgramID, vertexShader.shaderID());
  glAttachShader(*mpProgramID, fragmentShader.shaderID());

  if (prebindAttributes) {
    bindAttribute("position", 0);
    bindAttribute("texcoord", 1);
  }

  glLinkProgram(*mpProgramID);

  glGetProgramiv(*mpProgramID, GL_LINK_STATUS, &linkedStatus);

  if (GL_FALSE == linkedStatus) {
    std::cout << CompoShaderInfoLog::fromProgram(*this) << std::endl;
    LOGE("<<---- Error creating shader program");
    glDeleteProgram(*mpProgramID);
    *mpProgramID = 0;
  }
  else {
    LOGI("----->> Shader Program Created %d", *mpProgramID);
  }
  glFlush();
}

CompoShaderProgram::CompoShaderProgram( const CompoShaderProgram &otherProgram ):
CompoRenderState(),
mpProgramID(otherProgram.mpProgramID)
{
}


CompoShaderProgram::~CompoShaderProgram()
{
  if (mpProgramID.unique()) {
    LOGI("<<---- Program %d Deleted", *mpProgramID);
    glDeleteProgram(*mpProgramID);
  }
}

void CompoShaderProgram::bindAttribute(const std::string &attributeName,
                                       const GLint attributeID)
{
  glBindAttribLocation(*mpProgramID, attributeID, attributeName.c_str());
}

void CompoShaderProgram::activate() const
{
  if (isValid()) {
    glUseProgram(*mpProgramID);
  }
}

GLint CompoShaderProgram::getUniformLocation(const std::string &uniformName) const
{
  return glGetUniformLocation(*mpProgramID, uniformName.c_str());
}
