//
//  CompoShaderProgram.h
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_SHADER_PROGRAM_H
#define COMPO_SHADER_PROGRAM_H

#include "CompoTypes.h"
#include "CompoRenderState.h"

class CompoShaderProgram : public CompoRenderState
{
private:
  COMPO_SHARED<GLint> mpProgramID;
public:
  CompoShaderProgram(const CompoShader &vertexShader, const CompoShader &fragmentShader,
                     const bool prebindAttributes = true);

  CompoShaderProgram( const CompoShaderProgram &otherProgram );

  virtual ~CompoShaderProgram();
  virtual void activate() const;
  GLint getUniformLocation( const std::string &uniformName ) const;
  void bindAttribute( const std::string &attributeName, const GLint attributeID);
  inline bool isValid() const;
  inline GLint programID() const;
};


inline bool CompoShaderProgram::isValid() const {
  return (glIsProgram(*mpProgramID) == GL_TRUE);
}

inline GLint CompoShaderProgram::programID() const {
  return *mpProgramID;
}

#endif // COMPO_SHADER_PROGRAM_H
