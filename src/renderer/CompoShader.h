//
//  CompoShader.h
//  CompoView
//
//  Created by Didiet Noor on 18/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_SHADER_H
#define COMPO_SHADER_H
#include "CompoTypes.h"

class CompoShader {
public:
  enum CompoShaderType {
    COMPO_SHADER_VERTEX = GL_VERTEX_SHADER,
    COMPO_SHADER_FRAGMENT = GL_FRAGMENT_SHADER,
  };
private:
  GLint mShaderID;
  bool isCompileSuccess() const;
  
public:
  CompoShader( const std::string &source, const CompoShaderType kind );
  CompoShader( const CompoShader &otherShader );
  virtual ~CompoShader();
  
  /* property getter */
  inline GLint shaderID() const { return mShaderID; }
  
  /* methods */
  bool isValid() const;
};

#endif /* defined(__CompoView__CompoShader__) */
