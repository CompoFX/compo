//
//  CompoGraphicsOutput.h
//  CompoView
//
//  Created by Didiet Noor on 24/03/14.
//  Copyright (c) 2014 Nomad Studio. All rights reserved.
//

#ifndef COMPO_GRAPHICS_OUTPUT
#define COMPO_GRAPHICS_OUTPUT
#include "CompoTypes.h"

class CompoRenderable;
class CompoGraphicsOutput
{
public:
  virtual void render(const CompoRenderable&) const = 0;
  inline virtual ~CompoGraphicsOutput() {}
};


#endif // COMPO_GRAPHICS_OUTPUT
