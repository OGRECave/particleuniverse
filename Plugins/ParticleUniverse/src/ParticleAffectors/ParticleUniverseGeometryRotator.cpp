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

#include "ParticleAffectors/ParticleUniverseGeometryRotator.h"

namespace ParticleUniverse
{
	// Constants
	const bool GeometryRotator::DEFAULT_USE_OWN = false;
	const Real GeometryRotator::DEFAULT_ROTATION_SPEED = 10.0f;
	const Vector3 GeometryRotator::DEFAULT_ROTATION_AXIS(0, 0, 0);

	//-----------------------------------------------------------------------
	GeometryRotator::GeometryRotator(void) : 
		ParticleAffector(),
		mScaledRotationSpeed(0.0f),
		mUseOwnRotationSpeed(DEFAULT_USE_OWN),
		mQ(Quaternion::IDENTITY),
		mRotationAxis(DEFAULT_ROTATION_AXIS),
		mRotationAxisSet(false)
	{
		mDynRotationSpeed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		(static_cast<DynamicAttributeFixed*>(mDynRotationSpeed))->setValue(DEFAULT_ROTATION_SPEED);
	};
	//-----------------------------------------------------------------------
	GeometryRotator::~GeometryRotator(void)
	{
		if (mDynRotationSpeed)
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);
	}
	//-----------------------------------------------------------------------
	const Vector3& GeometryRotator::getRotationAxis(void) const
	{
		return mRotationAxis;
	}
	//-----------------------------------------------------------------------
	void GeometryRotator::setRotationAxis(const Vector3& rotationAxis)
	{
		mRotationAxis = rotationAxis;
		mRotationAxisSet = true;
	}
	//-----------------------------------------------------------------------
	void GeometryRotator::resetRotationAxis(void)
	{
		mDynRotationSpeed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
		(static_cast<DynamicAttributeFixed*>(mDynRotationSpeed))->setValue(DEFAULT_ROTATION_SPEED);
		mRotationAxisSet = false;
	}
	//-----------------------------------------------------------------------
	DynamicAttribute* GeometryRotator::getRotationSpeed(void) const
	{
		return mDynRotationSpeed;
	}
	//-----------------------------------------------------------------------
	void GeometryRotator::setRotationSpeed(DynamicAttribute* dynRotationSpeed)
	{
		if (mDynRotationSpeed)
			PU_DELETE_T(mDynRotationSpeed, DynamicAttribute, MEMCATEGORY_SCENE_OBJECTS);

		mDynRotationSpeed = dynRotationSpeed;
	}
	//-----------------------------------------------------------------------
	bool GeometryRotator::useOwnRotationSpeed (void) const
	{
		return mUseOwnRotationSpeed;
	}
	//-----------------------------------------------------------------------
	void GeometryRotator::setUseOwnRotationSpeed (bool useOwnRotationSpeed)
	{
		mUseOwnRotationSpeed = useOwnRotationSpeed;
	}
	//-----------------------------------------------------------------------
	Real GeometryRotator::_calculateRotationSpeed(Particle* particle)
	{
		return mDynamicAttributeHelper.calculate(mDynRotationSpeed, particle->timeFraction);
	}
	//-----------------------------------------------------------------------
	void GeometryRotator::_initParticleForEmission(Particle* particle)
	{
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL)
			return;

		VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
		if (!mRotationAxisSet)
		{
			// Set initial random rotation axis and orientation(PU 1.4)
			visualParticle->orientation.x = Ogre::Math::RangeRandom(-1, 1);
			visualParticle->orientation.y = Ogre::Math::RangeRandom(-1, 1);
			visualParticle->orientation.z = Ogre::Math::RangeRandom(-1, 1);
			visualParticle->orientation.w = Ogre::Math::RangeRandom(-1, 1);
			visualParticle->orientation.normalise();
			visualParticle->rotationAxis.x = Math::UnitRandom();
			visualParticle->rotationAxis.y = Math::UnitRandom();
			visualParticle->rotationAxis.z = Math::UnitRandom();
			visualParticle->rotationAxis.normalise();
		}

		if (mUseOwnRotationSpeed)
		{
			// Use the rotation speed of the particle itself
			visualParticle->rotationSpeed = _calculateRotationSpeed(particle);
		}
	}
	//-----------------------------------------------------------------------
	void GeometryRotator::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL)
			return;

		// Rotate the geometry
		VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);

		if (mUseOwnRotationSpeed)
		{
			// Use scaled rotationspeed and adjust the speed according to the velocity
			mScaledRotationSpeed = visualParticle->rotationSpeed * timeElapsed;
		}
		else
		{
			// Scale speed (beware that dynamic values don't result in a rotation; use a fixed value instead)
			mScaledRotationSpeed = _calculateRotationSpeed(particle) * timeElapsed;
		}

		mQ = Quaternion::IDENTITY;
		if (mRotationAxisSet)
		{
			mQ.FromAngleAxis(Radian(mScaledRotationSpeed), mRotationAxis);
		}
		else
		{
			mQ.FromAngleAxis(Radian(mScaledRotationSpeed), visualParticle->rotationAxis);
		}

		visualParticle->orientation = mQ * visualParticle->orientation;
	}
	//-----------------------------------------------------------------------
	void GeometryRotator::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		GeometryRotator* geometryRotator = static_cast<GeometryRotator*>(affector);
		geometryRotator->setRotationSpeed(mDynamicAttributeFactory.cloneDynamicAttribute(getRotationSpeed()));
		geometryRotator->mUseOwnRotationSpeed = mUseOwnRotationSpeed;
		geometryRotator->mRotationAxis = mRotationAxis;
		geometryRotator->mRotationAxisSet = mRotationAxisSet;
	}

}
