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

#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitter.h"

namespace ParticleUniverse
{
	// Constants
	const Real SphereSurfaceEmitter::DEFAULT_RADIUS = 10.0f;

	//-----------------------------------------------------------------------
	SphereSurfaceEmitter::SphereSurfaceEmitter(void) : 
		ParticleEmitter(),
		mRadius(DEFAULT_RADIUS),
		mRandomVector(Vector3::ZERO)
	{
	}
	//-----------------------------------------------------------------------
	const Real SphereSurfaceEmitter::getRadius(void) const
	{
		return mRadius;
	}
	//-----------------------------------------------------------------------
	void SphereSurfaceEmitter::setRadius(const Real radius)
	{
		mRadius = radius;
	}
	//-----------------------------------------------------------------------
	void SphereSurfaceEmitter::_initParticlePosition(Particle* particle)
	{
		// Generate a random unit vector to calculate a point on the sphere. This unit vector is
		// also used as direction vector if mAutoDirection has been set.
		mRandomVector = Vector3(Math::RangeRandom(-1, 1), 
			Math::RangeRandom(-1, 1),
			Math::RangeRandom(-1, 1));
		mRandomVector.normalise();
		ParticleSystem* sys = mParentTechnique->getParentSystem();
		if (sys)
		{
			particle->position = getDerivedPosition() + sys->getDerivedOrientation() * (_mEmitterScale * mRandomVector * mRadius);
		}
		else
		{
			particle->position = getDerivedPosition() + (_mEmitterScale * mRandomVector * mRadius);
		}
		particle->originalPosition = particle->position;
	}
	//-----------------------------------------------------------------------
    void SphereSurfaceEmitter::_initParticleDirection(Particle* particle)
    {
		if (mAutoDirection)
		{
			// The value of the direction vector that has been set does not have a meaning for
			// the sphere surface emitter.
			Radian angle;
			_generateAngle(angle);
			if (angle != Radian(0))
			{
				particle->direction = mRandomVector.randomDeviant(angle, mUpVector);
				particle->originalDirection = particle->direction;
			}
			else
			{
				particle->direction = mRandomVector;
				particle->originalDirection = particle->direction;
			}
		}
		else
		{
			// Use the standard way
			ParticleEmitter::_initParticleDirection(particle);
		}
    }
	//-----------------------------------------------------------------------
	void SphereSurfaceEmitter::copyAttributesTo (ParticleEmitter* emitter)
	{
		ParticleEmitter::copyAttributesTo(emitter);

		SphereSurfaceEmitter* sphereSurfaceEmitter = static_cast<SphereSurfaceEmitter*>(emitter);
		sphereSurfaceEmitter->mRadius = mRadius;
	}
}
