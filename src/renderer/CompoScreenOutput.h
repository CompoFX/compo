//
//  CompoScreenOutput.h
//  CompoView
//
//  Created by Didiet Noor on 24/03/14.
//  Copyright (c) 2014 Nomad Studio. All rights reserved.
//

#ifndef COMPO_SCREEN_OUTPUT_H
#define COMPO_SCREEN_OUTPUT_H

#include "CompoTypes.h"
#include "CompoGraphicsOutput.h"

#if defined(COMPO_TARGET_IOS)
@class EAGLContext;
@class CAEAGLLayer;
#elif COMPO_USE_EGL
class CompoGraphicsContext;
#endif

class CompoScreenOutput : public CompoGraphicsOutput
{
  struct Private;
public:
#if defined(COMPO_TARGET_IOS)
  CompoScreenOutput(const int width, const int height, EAGLContext *context, CAEAGLLayer *gpuLayer);
#elif COMPO_USE_EGL
  CompoScreenOutput(const CompoGraphicsContext &);
#else
  CompoScreenOutput();
#endif
  virtual void render(const CompoRenderable&) const;
  virtual ~CompoScreenOutput();
  void swap() const;

private:
  COMPO_UNIQUE<Private> d;
};

#endif
