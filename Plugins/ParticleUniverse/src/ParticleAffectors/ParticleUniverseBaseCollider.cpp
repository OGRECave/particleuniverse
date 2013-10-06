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

#include "ParticleAffectors/ParticleUniverseBaseCollider.h"
#include "ParticleUniverseAxisAlignedBox.h"

namespace ParticleUniverse
{
	// Constants
	const Real BaseCollider::DEFAULT_BOUNCYNESS = 1.0f;
	const Real BaseCollider::DEFAULT_FRICTION = 0.0f;
	const BaseCollider::IntersectionType BaseCollider::DEFAULT_INTERSECTION_TYPE = BaseCollider::IT_POINT;
	const BaseCollider::CollisionType BaseCollider::DEFAULT_COLLISION_TYPE = BaseCollider::CT_BOUNCE;

	//-----------------------------------------------------------------------
	BaseCollider::BaseCollider(void) :
		ParticleAffector(),
		mBouncyness(DEFAULT_BOUNCYNESS),
		mFriction(DEFAULT_FRICTION),
		mIntersectionType(DEFAULT_INTERSECTION_TYPE),
		mCollisionType(DEFAULT_COLLISION_TYPE),
		mVelocityScale(1.0f)
	{
	}
	//-----------------------------------------------------------------------
	void BaseCollider::_preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed)
	{
		// Take scaled velocity into account
		mVelocityScale = 1.0f;
		if (mParentTechnique)
		{
			mVelocityScale = timeElapsed * mParentTechnique->getParticleSystemScaleVelocity();
		}
	}
	//-----------------------------------------------------------------------
	const BaseCollider::IntersectionType BaseCollider::getIntersectionType(void) const
	{
		return mIntersectionType;
	}
	//-----------------------------------------------------------------------
	void BaseCollider::setIntersectionType(const BaseCollider::IntersectionType& intersectionType)
	{
		mIntersectionType = intersectionType;
	}
	//-----------------------------------------------------------------------
	const BaseCollider::CollisionType BaseCollider::getCollisionType(void) const
	{
		return mCollisionType;
	}
	//-----------------------------------------------------------------------
	void BaseCollider::setCollisionType(const BaseCollider::CollisionType& collisionType)
	{
		mCollisionType = collisionType;
	}
	//-----------------------------------------------------------------------
	const Real BaseCollider::getFriction(void) const
	{
		return mFriction;
	}
	//-----------------------------------------------------------------------
	void BaseCollider::setFriction(const Real friction)
	{
		mFriction = friction;
	}
	//-----------------------------------------------------------------------
	const Real BaseCollider::getBouncyness(void) const
	{
		return mBouncyness;
	}
	//-----------------------------------------------------------------------
	void BaseCollider::setBouncyness(const Real bouncyness)
	{
		mBouncyness = bouncyness;
	}
	//-----------------------------------------------------------------------
	void BaseCollider::populateAlignedBox(AxisAlignedBox& box,
		const Vector3& position, 
		const Real width,
		const Real height,
		const Real depth)
	{
		Real halfWidth = 0.5f * width;
		Real halfHeight = 0.5f * height;
		Real halfDepth = 0.5f * depth;
		box.setMinimum(position.x - halfWidth, 
			position.y - halfHeight, 
			position.z - halfDepth);
		box.setMaximum(position.x + halfWidth, 
			position.y + halfHeight, 
			position.z + halfDepth);
	}
	//-----------------------------------------------------------------------
	void BaseCollider::calculateRotationSpeedAfterCollision(Particle* particle)
	{
		if (particle->particleType != Particle::PT_VISUAL)
			return;

		VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
		Real signedFriction = Math::UnitRandom() > 0.5 ? -(mFriction - 1) : (mFriction - 1);
		visualParticle->rotationSpeed *= signedFriction;
		visualParticle->zRotationSpeed *= signedFriction;
	}
	//-----------------------------------------------------------------------
	void BaseCollider::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		BaseCollider* baseCollider = static_cast<BaseCollider*>(affector);
		baseCollider->mBouncyness = mBouncyness;
		baseCollider->mFriction = mFriction;
		baseCollider->mIntersectionType = mIntersectionType;
		baseCollider->mCollisionType = mCollisionType;
	}

}
