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

#include "ParticleAffectors/ParticleUniverseJetAffector.h"

namespace ParticleUniverse
{
	// Constants
	const Real JetAffector::DEFAULT_ACCELERATION = 1.0f;

	//-----------------------------------------------------------------------
	JetAffector::JetAffector (void) : 
		ParticleAffector(),
		mScaled(0.0f)
	{
		mDynAcceleration = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		(static_cast<DynamicAttributeFixed*>(mDynAcceleration))->setValue(DEFAULT_ACCELERATION);
	}
	//-----------------------------------------------------------------------
	JetAffector::~JetAffector (void)
	{
		if (!mDynAcceleration)
			return;

		PU_DELETE_T(mDynAcceleration, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
		mDynAcceleration = 0;
	}
	//-----------------------------------------------------------------------
	void JetAffector::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		JetAffector* jetAffector = static_cast<JetAffector*>(affector);
		jetAffector->setDynAcceleration(mDynamicAttributeFactory.cloneDynamicAttribute(getDynAcceleration()));
	}
	//-----------------------------------------------------------------------
	void JetAffector::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		mScaled = timeElapsed * (mDynAcceleration->getValue(particle->timeFraction));
		if (particle->direction == Vector3::ZERO)
		{
			// Existing direction is zero, so use original direction
			particle->direction += (particle->originalDirection * mScaled);
		}
		else
		{
			particle->direction += (particle->direction * mScaled);
		}
	}
	//-----------------------------------------------------------------------
	void JetAffector::setDynAcceleration(DynamicAttribute* dynAcceleration)
	{
		if (mDynAcceleration)
			PU_DELETE_T(mDynAcceleration, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);

		mDynAcceleration = dynAcceleration;
	}
}
