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

#ifndef __PU_TEXTURE_ANIMATION_H__
#define __PU_TEXTURE_ANIMATION_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"
#include "ParticleUniverseDynamicAttribute.h"
#include "ParticleUniverseDynamicAttributeFactory.h"

namespace ParticleUniverse
{
	/** The TextureAnimator makes it possible to have an animated texture for each individual particle. It relies on the uv coordinate 
	    settings in the ParticleRenderer.
    */
	class _ParticleUniverseExport TextureAnimator : public ParticleAffector
	{
		public:
			enum TextureAnimationType
			{
				TAT_LOOP,
				TAT_UP_DOWN,
				TAT_RANDOM
			};

			// Constants
			static const Real DEFAULT_TIME_STEP;
			static const uint16 DEFAULT_TEXCOORDS_START;
			static const uint16 DEFAULT_TEXCOORDS_END;
			static const TextureAnimationType DEFAULT_ANIMATION_TYPE;
			static const bool DEFAULT_START_RANDOM;

			TextureAnimator(void);
	        virtual ~TextureAnimator(void);

			/** Returns the AnimationTimeStep. The AnimationTimeStep defines the time between each animation frame. */
			Real getAnimationTimeStep(void) const;

			/** Set the  AnimationTimeStep */
			void setAnimationTimeStep(Real animationTimeStep);

			/** Returns the type of texture animation. */
			TextureAnimationType getTextureAnimationType(void) const;

			/** Set the type of texture animation. */
			void setTextureAnimationType(TextureAnimationType textureAnimationType);

			/** Todo */
			uint16 getTextureCoordsStart(void) const;

			/** Todo */
			void setTextureCoordsStart(uint16 textureCoordsStart);

			/** Todo */
			uint16 getTextureCoordsEnd(void) const;

			/** Todo */
			void setTextureCoordsEnd(uint16 textureCoordsEnd);

			/** Todo */
			bool isStartRandom(void) const;

			/** Todo */
			void setStartRandom(bool startRandom);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);

			/** @copydoc ParticleAffector::_preProcessParticles */
			virtual void _preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed);

			/** @copydoc ParticleAffector::_initParticleForEmission */
			virtual void _initParticleForEmission(Particle* particle);

			/** @copydoc ParticleAffector::_affect */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

		protected:
			Real mAnimationTimeStep;
			Real mAnimationTimeStepCount;
			bool mStartRandom;
			bool mAnimationTimeStepSet;
			bool mNextIndex;
			TextureAnimationType mTextureAnimationType;
			uint16 mTextureCoordsStart;
			uint16 mTextureCoordsEnd;

			/** Todo.
			*/
			void _determineNextTextureCoords(VisualParticle* visualParticle);
	};

}
#endif
