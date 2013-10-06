/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniversePCH.h"

#ifndef PARTICLE_UNIVERSE_EXPORTS
#define PARTICLE_UNIVERSE_EXPORTS
#endif

#include "ParticleAffectors/ParticleUniverseTextureAnimator.h"

namespace ParticleUniverse
{
	// Constants
	const Real TextureAnimator::DEFAULT_TIME_STEP = 0.0f;
	const uint16 TextureAnimator::DEFAULT_TEXCOORDS_START = 0;
	const uint16 TextureAnimator::DEFAULT_TEXCOORDS_END = 0;
	const TextureAnimator::TextureAnimationType TextureAnimator::DEFAULT_ANIMATION_TYPE = TextureAnimator::TAT_LOOP;
	const bool TextureAnimator::DEFAULT_START_RANDOM = true;

	//-----------------------------------------------------------------------
	TextureAnimator::TextureAnimator(void) : 
		ParticleAffector(),
		mAnimationTimeStep(DEFAULT_TIME_STEP),
		mAnimationTimeStepSet(false),
		mNextIndex(false),
		mTextureAnimationType(DEFAULT_ANIMATION_TYPE),
		mTextureCoordsStart(DEFAULT_TEXCOORDS_START),
		mTextureCoordsEnd(DEFAULT_TEXCOORDS_END),
		mStartRandom(DEFAULT_START_RANDOM),
		mAnimationTimeStepCount(0.0f)
	{
	}
	//-----------------------------------------------------------------------
	TextureAnimator::~TextureAnimator(void)
	{
	}
	//-----------------------------------------------------------------------
	Real TextureAnimator::getAnimationTimeStep(void) const
	{
		return mAnimationTimeStep;
	}
	//-----------------------------------------------------------------------
	void TextureAnimator::setAnimationTimeStep(Real animationTimeStep)
	{
		mAnimationTimeStep = animationTimeStep;
		mAnimationTimeStepSet = true;
	}
	//-----------------------------------------------------------------------
	TextureAnimator::TextureAnimationType TextureAnimator::getTextureAnimationType(void) const
	{
		return mTextureAnimationType;
	}
	//-----------------------------------------------------------------------
	void TextureAnimator::setTextureAnimationType(TextureAnimator::TextureAnimationType textureAnimationType)
	{
		mTextureAnimationType = textureAnimationType;
	}
	//-----------------------------------------------------------------------
	uint16 TextureAnimator::getTextureCoordsStart(void) const
	{
		return mTextureCoordsStart;
	}
	//-----------------------------------------------------------------------
	void TextureAnimator::setTextureCoordsStart(uint16 textureCoordsStart)
	{
		mTextureCoordsStart = textureCoordsStart;
	}
	//-----------------------------------------------------------------------
	uint16 TextureAnimator::getTextureCoordsEnd(void) const
	{
		return mTextureCoordsEnd;
	}
	//-----------------------------------------------------------------------
	void TextureAnimator::setTextureCoordsEnd(uint16 textureCoordsEnd)
	{
		mTextureCoordsEnd = textureCoordsEnd;
	}
	//-----------------------------------------------------------------------
	bool TextureAnimator::isStartRandom(void) const
	{
		return mStartRandom;
	}
	//-----------------------------------------------------------------------
	void TextureAnimator::setStartRandom(bool startRandom)
	{
		mStartRandom = startRandom;
	}
	//-----------------------------------------------------------------------
	void TextureAnimator::_initParticleForEmission(Particle* particle)
	{
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL)
			return;

		VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);

		// Set first image
		if (mStartRandom)
		{
			visualParticle->textureCoordsCurrent = (uint16)Math::RangeRandom((Real)mTextureCoordsStart, (Real)mTextureCoordsEnd + 0.999f);
		}
		else
		{
			visualParticle->textureCoordsCurrent = mTextureCoordsStart;
		}

		// Calculate the animationTimeStep
		if (!mAnimationTimeStepSet)
		{
			// Set the animation time step for each particle
			switch(mTextureAnimationType)
			{
				case TAT_LOOP:
				{
					visualParticle->textureAnimationTimeStep = visualParticle->timeToLive / (mTextureCoordsEnd - mTextureCoordsStart + 1);
				}
				break;

				case TAT_UP_DOWN:
				{
					visualParticle->textureAnimationTimeStep = visualParticle->timeToLive / (2 * (mTextureCoordsEnd - mTextureCoordsStart) + 1);
				}
				break;

				case TAT_RANDOM:
				{
					visualParticle->textureAnimationTimeStep = visualParticle->timeToLive;
				}
				break;
			}
		}
	}
	//-----------------------------------------------------------------------
	void TextureAnimator::_preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed)
	{
		// Determine the next texture coords index (global)
		if (mAnimationTimeStepSet)
		{
			mNextIndex = false;
			mAnimationTimeStepCount += timeElapsed;
			if (mAnimationTimeStepCount > mAnimationTimeStep)
			{
				mAnimationTimeStepCount -= mAnimationTimeStep;
				mNextIndex = true;
			}
		}
	}
	//-----------------------------------------------------------------------
	void TextureAnimator::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL)
			return;

		VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);

		// Determine the next texture coords index
		if (mAnimationTimeStepSet)
		{
			if (mNextIndex)
			{
				// Use the global one for all particles
				_determineNextTextureCoords(visualParticle);
			}
		}
		else
		{
			visualParticle->textureAnimationTimeStepCount += timeElapsed;
			if (visualParticle->textureAnimationTimeStepCount > visualParticle->textureAnimationTimeStep)
			{
				visualParticle->textureAnimationTimeStepCount -= visualParticle->textureAnimationTimeStep;
				_determineNextTextureCoords(visualParticle);
			}
		}
	}
	//-----------------------------------------------------------------------
	void TextureAnimator::_determineNextTextureCoords(VisualParticle* visualParticle)
	{
		switch(mTextureAnimationType)
		{
			case TAT_LOOP:
			{
				if (visualParticle->textureCoordsCurrent >= mTextureCoordsEnd)
				{
					visualParticle->textureCoordsCurrent = mTextureCoordsStart;
				}
				else
				{
					(visualParticle->textureCoordsCurrent)++;
				}
			}
			break;

			case TAT_UP_DOWN:
			{
				if (visualParticle->textureAnimationDirectionUp == true)
				{
					// Going up
					if (visualParticle->textureCoordsCurrent >= mTextureCoordsEnd)
					{
						(visualParticle->textureCoordsCurrent)--;
						visualParticle->textureAnimationDirectionUp = false;
					}
					else
					{
						(visualParticle->textureCoordsCurrent)++;
					}
				}
				else
				{
					// Going down
					if (visualParticle->textureCoordsCurrent <= mTextureCoordsStart)
					{
						(visualParticle->textureCoordsCurrent)++;
						visualParticle->textureAnimationDirectionUp = true;
					}
					else
					{
						(visualParticle->textureCoordsCurrent)--;
					}
				}
			}
			break;

			case TAT_RANDOM:
			{
				// Generate a random texcoord index
				visualParticle->textureCoordsCurrent = (uint16)Math::RangeRandom((Real)mTextureCoordsStart, (Real)mTextureCoordsEnd + 0.999f);
			}
			break;
		}
	}

	//-----------------------------------------------------------------------
	void TextureAnimator::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);
		TextureAnimator* textureAnimator = static_cast<TextureAnimator*>(affector);
		textureAnimator->mAnimationTimeStep = mAnimationTimeStep;
		textureAnimator->mAnimationTimeStepSet = mAnimationTimeStepSet;
		textureAnimator->mTextureAnimationType = mTextureAnimationType;
		textureAnimator->mTextureCoordsStart = mTextureCoordsStart;
		textureAnimator->mTextureCoordsEnd = mTextureCoordsEnd;
		textureAnimator->mStartRandom = mStartRandom;
	}

}
