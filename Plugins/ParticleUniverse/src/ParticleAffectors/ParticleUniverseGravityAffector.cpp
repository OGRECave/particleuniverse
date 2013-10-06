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

#include "ParticleAffectors/ParticleUniverseGravityAffector.h"

namespace ParticleUniverse
{
	// Constants
	const Real GravityAffector::DEFAULT_GRAVITY = 1.0f;

	//-----------------------------------------------------------------------
	GravityAffector::GravityAffector(void) : 
		ParticleAffector(),
		mGravity(DEFAULT_GRAVITY)
	{
	}
	//-----------------------------------------------------------------------
	void GravityAffector::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		GravityAffector* gravityAffector = static_cast<GravityAffector*>(affector);
		gravityAffector->mGravity = mGravity;
	}
	//-----------------------------------------------------------------------
	void GravityAffector::_preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed)
	{
		// Calculate the affectors' position so it is also safe to use mDerivedPosition.
		getDerivedPosition();
	}
	//-----------------------------------------------------------------------
	inline void GravityAffector::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		 // Applied scaling in V1.3.1
		/** Applying Newton's law of universal gravitation.	*/
		Vector3 distance = mDerivedPosition - particle->position;
		Real length = distance.squaredLength();
		Real scaleVelocity = 1.0f;
		if (mParentTechnique)
		{
			scaleVelocity = mParentTechnique->getParticleSystemScaleVelocity();
		}
		if (length > 0 && mParentTechnique)
		{
			//Real force = (mGravity * particle->mass * mass) / length;
			Real force = (scaleVelocity * mGravity * particle->mass * mass) / length;
			particle->direction += force * distance * timeElapsed * _calculateAffectSpecialisationFactor(particle);
		}
	}
	//-----------------------------------------------------------------------
	Real GravityAffector::getGravity(void) const
	{
		return mGravity;
	}
	//-----------------------------------------------------------------------
	void GravityAffector::setGravity(Real gravity)
	{
		mGravity = gravity;
	}
}
