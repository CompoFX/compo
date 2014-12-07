#include "CompoScreenOutput.h"
#include "CompoGraphicsContext.h"
#include "CompoRenderable.h"

struct CompoScreenOutput::Private
{
  int width, height;
  GLint frameBuffer;
  inline Private(const int width_, const int height_):
    width(width_), height(height_), frameBuffer(0) {}
};

CompoScreenOutput::CompoScreenOutput(const CompoGraphicsContext &context) :
  d(new Private(context.getWidth(), context.getHeight()))
{
  // glGetIntegerv(GL_FRAMEBUFFER_BINDING, &d->frameBuffer);
  LOGI("Screen Output -> %dx%d", d->width, d->height);
}

CompoScreenOutput::~CompoScreenOutput()
{
  
}

void CompoScreenOutput::render(const CompoRenderable &renderable) const
{
  glBindFramebuffer(GL_FRAMEBUFFER, d->frameBuffer);
  glViewport(0, 0, d->width, d->height);
  renderable.render();
}

void CompoScreenOutput::swap() const
{

}


