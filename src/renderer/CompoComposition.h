//
//  CompoComposition.h
//  CompoView
//
//  Created by Didiet Noor on 20/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#ifndef COMPO_COMPOSITION_H
#define COMPO_COMPOSITION_H
#include "CompoTypes.h"
#include "CompoRenderable.h"

class CompoGraphicsOutput;
class CompoComposition : public CompoRenderable
{
public:
  enum CompoCompositionEffect {
    EFFECT_NONE = 0,
    EFFECT_NORMAL = 0,
    EFFECT_GREYSCALE,
    EFFECT_LIGHTEN,
    EFFECT_SEPIA,
    EFFECT_HITAMPUTIH,
    EFFECT_SCRIBE,
    EFFECT_HIKE,
    EFFECT_CINTA,
    EFFECT_ANALOG,
    EFFECT_THERMO,
    EFFECT_DESAKU,
    EFFECT_KELUD,
    EFFECT_HERO,
    EFFECT_PENCIL,
    EFFECT_STYLISH,
    EFFECT_LORDKEVIN,
    EFFECT_HUDSON,
    EFFECT_PIXELATE
  };

  CompoComposition();
  CompoComposition(const CompoImage &);
  ~CompoComposition();

  virtual void render() const;
  void setActiveEffect( const CompoCompositionEffect effectKind);
  void setImage(const CompoImage &);

private:
  COMPO_UNIQUE<CompoPhotoQuad> mMainImage,
  mFrame, mWatermarkFrame;

  bool mIsFramed;

  CompoCompositionEffect mActiveEffect;
  CompoEffect *mCurrentEffect;
};

#endif /* defined(__CompoView__CompoComposition__) */
