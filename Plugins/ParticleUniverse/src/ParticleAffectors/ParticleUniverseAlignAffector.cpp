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

#include "ParticleAffectors/ParticleUniverseAlignAffector.h"

namespace ParticleUniverse
{
	// Constants
	const bool AlignAffector::DEFAULT_RESIZE = false;
	
	//-----------------------------------------------------------------------
	AlignAffector::AlignAffector(void) : 
		ParticleAffector(),
		mPreviousParticle(0),
		mResize(DEFAULT_RESIZE)
	{
	}
	//-----------------------------------------------------------------------
	void AlignAffector::_firstParticle(ParticleTechnique* particleTechnique, 
		Particle* particle, 
		Real timeElapsed)
	{
		mPreviousParticle = particle;
	}
	//-----------------------------------------------------------------------
	bool AlignAffector::isResize(void) const
	{
		return mResize;
	}
	//-----------------------------------------------------------------------
	void AlignAffector::setResize(bool resize)
	{
		mResize = resize;
	}
	//-----------------------------------------------------------------------
	void AlignAffector::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (particle->particleType == Particle::PT_VISUAL)
		{
			// Set the orientation towards the previous particle, but rotation is undetermined.
			VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);

			// Get difference
			Vector3 diff = (mPreviousParticle->position - particle->position);
			if (mResize)
			{
				visualParticle->setOwnDimensions (visualParticle->width, diff.length(), visualParticle->depth);
			}
			diff.normalise();
			visualParticle->orientation.x = diff.x;
			visualParticle->orientation.y = diff.y;
			visualParticle->orientation.z = diff.z;
		}

		mPreviousParticle = particle;
	}
	//-----------------------------------------------------------------------
	void AlignAffector::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		AlignAffector* alignAffector = static_cast<AlignAffector*>(affector);
		alignAffector->mResize = mResize;
	}
}
