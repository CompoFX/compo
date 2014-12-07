//
//  CompoTexture.h
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_TEXTURE_H
#define COMPO_TEXTURE_H

#include "CompoTypes.h"

class CompoTexture
{
private:
  COMPO_SHARED<GLuint> mpTextureID;
public:
  explicit CompoTexture(); // Blank Texture for Buffer Attachments
  CompoTexture( const CompoImage &);
  CompoTexture( const CompoTexture &);
  virtual ~CompoTexture();

  void activateWithProgram(const CompoShaderProgram &program, const GLuint sampler) const;
  bool isValid() const;
};


#endif // COMPO_TEXTURE_H
