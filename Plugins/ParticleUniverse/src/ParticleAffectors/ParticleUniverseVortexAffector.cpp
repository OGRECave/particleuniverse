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

#include "ParticleAffectors/ParticleUniverseVortexAffector.h"

namespace ParticleUniverse
{
	// Constants
	const Vector3 VortexAffector::DEFAULT_ROTATION_VECTOR(0, 0, 0);
	const Real VortexAffector::DEFAULT_ROTATION_SPEED = 1.0f;

	//-----------------------------------------------------------------------
	VortexAffector::VortexAffector(void) : 
		ParticleAffector(),
		mRotation(Quaternion::IDENTITY),
		mRotationVector(DEFAULT_ROTATION_VECTOR)
	{
		mDynRotationSpeed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		(static_cast<DynamicAttributeFixed*>(mDynRotationSpeed))->setValue(DEFAULT_ROTATION_SPEED);
	}
	//-----------------------------------------------------------------------
	VortexAffector::~VortexAffector(void)
	{
		if (mDynRotationSpeed)
		{
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
			mDynRotationSpeed = 0;
		}
	}
	//-----------------------------------------------------------------------
	const Vector3& VortexAffector::getRotationVector(void) const
	{
		return mRotationVector;
	}
	//-----------------------------------------------------------------------
	void VortexAffector::setRotationVector(const Vector3& rotationVector)
	{
		mRotationVector = rotationVector;
	}
	//-----------------------------------------------------------------------
	DynamicAttribute* VortexAffector::getRotationSpeed(void) const
	{
		return mDynRotationSpeed;
	}
	//-----------------------------------------------------------------------
	void VortexAffector::setRotationSpeed(DynamicAttribute* dynRotationSpeed)
	{
		if (mDynRotationSpeed)
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);

		mDynRotationSpeed = dynRotationSpeed;
	}
	//-----------------------------------------------------------------------
	void VortexAffector::_preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed)
	{
		ParticleSystem* sys = mParentTechnique->getParentSystem();
		if (sys)
		{
			mRotation.FromAngleAxis(Radian(_calculateRotationSpeed() * timeElapsed), sys->getDerivedOrientation() * mRotationVector);
		}
		else
		{
			mRotation.FromAngleAxis(Radian(_calculateRotationSpeed() * timeElapsed), mRotationVector);
		}
		getDerivedPosition();
	}
	//-----------------------------------------------------------------------
	Radian VortexAffector::_calculateRotationSpeed(void)
	{
		return Radian(mDynamicAttributeHelper.calculate(mDynRotationSpeed, mParentTechnique->getParentSystem()->getTimeElapsedSinceStart()));
	}
	//-----------------------------------------------------------------------
	void VortexAffector::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		// Explicitly check on 'freezed', because it passes the techniques' validation.
		if (particle->isFreezed())
			return;

		// Rotate position, direction and orientation (visible particle only) and compensate for the affector position
		// Also take the affect specialisation into account
		Vector3 local = particle->position - mDerivedPosition;
		particle->position = mDerivedPosition + mRotation * local;
		particle->direction = mRotation * particle->direction;

		if (particle->particleType == Particle::PT_VISUAL)
		{
			VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
			visualParticle->orientation = mRotation * visualParticle->orientation;
		}
	}
	//-----------------------------------------------------------------------
	void VortexAffector::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		VortexAffector* vortexAffector = static_cast<VortexAffector*>(affector);
		vortexAffector->mRotation = mRotation;
		vortexAffector->mRotationVector = mRotationVector;
		vortexAffector->setRotationSpeed(mDynamicAttributeFactory.cloneDynamicAttribute(getRotationSpeed()));
	}

}
