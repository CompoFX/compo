//
//  CompoRenderable.h
//  CompoView
//
//  Created by Didiet Noor on 24/03/14.
//  Copyright (c) 2014 Nomad Studio. All rights reserved.
//

#ifndef COMPO_RENDERABLE_H
#define COMPO_RENDERABLE_H

class CompoRenderable
{
public:
  virtual void render() const= 0;
  inline virtual ~CompoRenderable() {}
};

#endif // COMPO_RENDERABLE_H
