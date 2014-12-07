//
//  CompoRenderBufferOutput.h
//  CompoView
//
//  Created by Didiet Noor on 26/03/14.
//  Copyright (c) 2014 Nomad Studio. All rights reserved.
//

#ifndef COMPO_RENDERBUFFER_OUTPUT_H
#define COMPO_RENDERBUFFER_OUTPUT_H

#include "CompoTypes.h"
#include "CompoGraphicsOutput.h"

class CompoRenderable;
class CompoRenderBufferOutput : public CompoGraphicsOutput
{
private:
  GLuint mFrameBuffer;
  GLuint mRenderBuffer;
  int mWidth, mHeight;
public:
  CompoRenderBufferOutput(const int width, const int height);
  virtual void render(const CompoRenderable &) const;
  virtual ~CompoRenderBufferOutput();
  
private:
  CompoRenderBufferOutput(const CompoRenderBufferOutput&);
};

#endif //COMPO_RENDERBUFFER_OUTPUT_H
