//
//  CompoComposition.cpp
//  CompoView
//
//  Created by Didiet Noor on 20/02/14.
//  Copyright (c) 2014 YKode Studio. All rights reserved.
//

#include "CompoComposition.h"
#include "CompoPhotoQuad.h"
#include "CompoEffect.h"
#include "CompoShader.h"
#include "CompoShadersSources.h"
#include "CompoImage.h"
#include "CompoImageSources.h"

// These static shaders are only initialised during Composition
// Composition needs to be created in the same thread as graphic context

static CompoEffect *sNormalEffect = 0;
static CompoShader *sSAQVertexShader = 0;

CompoComposition::CompoComposition() :
  mWatermarkFrame(new CompoPhotoQuad(
    CompoImage::fromPNGData(CompoPreRelease_png,CompoPreRelease_png_len))),
  mIsFramed(false),
  mActiveEffect(EFFECT_NONE),
  mCurrentEffect(0)
{
  if (sSAQVertexShader == 0) {
    sSAQVertexShader = new CompoShader( VS_ScreenAlignedQuad, CompoShader::COMPO_SHADER_VERTEX);
  }

  const CompoShader pixelShader( PS_EffectNormal,
      CompoShader::COMPO_SHADER_FRAGMENT);

  if (sNormalEffect == 0) {
    sNormalEffect = new CompoEffect(
      CompoShaderProgram(*sSAQVertexShader, pixelShader));
  }

  mCurrentEffect = sNormalEffect;
}

CompoComposition::CompoComposition( const CompoImage &image ) :
  mMainImage(new CompoPhotoQuad(image)),
  mWatermarkFrame(new CompoPhotoQuad(
    CompoImage::fromPNGData(CompoPreRelease_png,CompoPreRelease_png_len))),
  mIsFramed(false),
  mActiveEffect(EFFECT_NONE),
  mCurrentEffect(0)
{
  if (sSAQVertexShader == 0) {
    sSAQVertexShader = new CompoShader( VS_ScreenAlignedQuad, CompoShader::COMPO_SHADER_VERTEX);
  }

  const CompoShader pixelShader( PS_EffectNormal,
                                CompoShader::COMPO_SHADER_FRAGMENT);

  if (sNormalEffect == 0) {
    sNormalEffect = new CompoEffect(
      CompoShaderProgram(*sSAQVertexShader, pixelShader));
  }

  mCurrentEffect = sNormalEffect;
}

void CompoComposition::setImage(const CompoImage &image)
{
  if (mMainImage.get() != 0) {
    mMainImage.reset();
  }
  COMPO_UNIQUE<CompoPhotoQuad> newQuad(new CompoPhotoQuad(image));
  mMainImage.swap(newQuad);
}

CompoComposition::~CompoComposition()
{
  glUseProgram(0);
  LOGI("---->> Deleting Composition [0x%" PRIXPTR "]", (intptr_t) this);
  if (mCurrentEffect != sNormalEffect) {
    delete mCurrentEffect; mCurrentEffect = 0;
  }

  if (sNormalEffect != 0)
  {
    delete sNormalEffect; sNormalEffect = 0;
  }

  if (sSAQVertexShader) {
    delete sSAQVertexShader; sSAQVertexShader = 0;
  }
}

void CompoComposition::setActiveEffect(const CompoComposition::CompoCompositionEffect effectKind)
{
  const CompoShader &vertexShader = *sSAQVertexShader;
  CompoEffect *newEffect = 0, *oldEffect = 0;

  if (effectKind != mActiveEffect) {
    switch (effectKind) {
      default:
      case EFFECT_NONE: {
        mCurrentEffect = sNormalEffect;
        goto setFinalEffect;
      }
      break;
      case EFFECT_GREYSCALE: {
        const CompoShader pixelShader(PS_EffectGreyScale, CompoShader::COMPO_SHADER_FRAGMENT);
        newEffect = new CompoEffect(CompoShaderProgram(vertexShader, pixelShader));
      }
      break;
      case EFFECT_LIGHTEN: {
        const CompoShader pixelShader(PS_EffectBleach, CompoShader::COMPO_SHADER_FRAGMENT);
        newEffect = new CompoEffect(CompoShaderProgram(vertexShader, pixelShader));
      }
        break;
      case EFFECT_SEPIA: {
        const CompoShader pixelShader( PS_EffectSepia, CompoShader::COMPO_SHADER_FRAGMENT);
        newEffect = new CompoEffect(CompoShaderProgram(vertexShader, pixelShader));
      }
      break;
    }

    oldEffect = mCurrentEffect;
    mCurrentEffect = newEffect;
    if (oldEffect != sNormalEffect) {
      delete oldEffect; oldEffect = 0;
    }

  setFinalEffect:
    mActiveEffect = effectKind;
  }
}

void CompoComposition::render() const
{
  if (mMainImage != 0 && mCurrentEffect != 0) {
    mMainImage->drawWithEffect(*mCurrentEffect);
  }
  if (mIsFramed && mFrame) {
    // *do nothing* --> mFrame->drawWithEffect(*sNormalEffect);
  }
  if (mWatermarkFrame) {
    mWatermarkFrame->drawWithEffect(*sNormalEffect);
  }
}
