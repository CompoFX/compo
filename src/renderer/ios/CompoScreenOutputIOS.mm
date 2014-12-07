//
//  CompoScreenOutputIOS.mm
//  CompoView
//
//  Created by Didiet Noor on 24/03/14.
//  Copyright (c) 2014 Nomad Studio. All rights reserved.
//


#include <OpenGLES/EAGL.h>
#include <QuartzCore/CAEAGLLayer.h>

#import "CompoScreenOutput.h"
#import "CompoRenderable.h"

struct CompoScreenOutput::Private
{
 
  GLuint frameBuffer, renderBuffer;
  const int width, height;
  
  EAGLContext *context;
  
  inline void bindBuffers() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, renderBuffer);
  }
  
  inline Private(): frameBuffer(0), renderBuffer(0), width(0), height(0), context(nil)
  {
    glGenFramebuffers(1, &frameBuffer);
    glGenRenderbuffers(1, &renderBuffer);
  }
  
  inline Private(EAGLContext *context_) : frameBuffer(0), renderBuffer(0), width(0), height(0),
  context(context_)
  {
    glGenFramebuffers(1, &frameBuffer);
    glGenRenderbuffers(1, &renderBuffer);
  }
  
  inline Private(const int width_, const int height_, EAGLContext *context_):
  frameBuffer(0), renderBuffer(0), width(width_), height(height_),
  context(context_)
  {
    glGenFramebuffers(1, &frameBuffer);
    glGenRenderbuffers(1, &renderBuffer);
  }
  
  inline ~Private()
  {
    glDeleteRenderbuffers(1, &renderBuffer);
    glDeleteFramebuffers(1, &frameBuffer);
  }
  
};

CompoScreenOutput::CompoScreenOutput(const int width, const int height, EAGLContext *context, CAEAGLLayer *gpuLayer) :
  d(new Private(width, height, context))
{
  d->bindBuffers();
  [d->context renderbufferStorage:GL_RENDERBUFFER fromDrawable:gpuLayer];
  
  glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  glClearDepthf(1.0f);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void CompoScreenOutput::render(const CompoRenderable &renderable) const
{
  d->bindBuffers();
  glViewport(0, 0, d->width, d->height);
  renderable.render();
}

void CompoScreenOutput::swap() const
{
  d->bindBuffers();
  [d->context presentRenderbuffer:d->renderBuffer];
}


CompoScreenOutput::~CompoScreenOutput()
{
  
}
