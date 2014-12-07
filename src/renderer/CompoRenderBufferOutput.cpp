//
//  CompoRenderBufferOutput.cpp
//  CompoView
//
//  Created by Didiet Noor on 26/03/14.
//  Copyright (c) 2014 Nomad Studio. All rights reserved.
//

#include "CompoRenderBufferOutput.h"
#include "CompoRenderable.h"

CompoRenderBufferOutput::CompoRenderBufferOutput(const int width, const int height) :
mWidth(width), mHeight(height)
{  
  glGenFramebuffers(1, &mFrameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
  
  glGenRenderbuffers(1, &mRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, mRenderBuffer);
  
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8_OES, width, height);
  
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, mRenderBuffer);
  
  const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    LOGE("Failed to make complete framebuffer object [%X]", status);
  }
 
}

void CompoRenderBufferOutput::render(const CompoRenderable &renderable) const
{
  glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, mRenderBuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER ,
                            GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, mRenderBuffer);
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClearDepthf(1.0f);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glViewport(0, 0,mWidth, mHeight);
  
  LOGI("--->> RENDERING FROM OFFSCREEN BUFFER");
  
  renderable.render();
  
  LOGI("--->> END RENDERING FROM OFFSCREEN BUFFER");
}

CompoRenderBufferOutput::~CompoRenderBufferOutput()
{
  glDeleteRenderbuffers(1, &mRenderBuffer);
  glDeleteFramebuffers(1, &mFrameBuffer);
}
