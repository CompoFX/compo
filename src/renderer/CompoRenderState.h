//
//  CompoState.h
//  CompoView
//
//  Created by Didiet Noor on 19/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_RENDER_STATE_H
#define COMPO_RENDER_STATE_H

struct CompoRenderState
{
  virtual void activate() const = 0;
  virtual inline ~CompoRenderState() {};
};
#endif
