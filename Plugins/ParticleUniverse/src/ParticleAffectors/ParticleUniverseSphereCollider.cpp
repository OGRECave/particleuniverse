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

#include "ParticleAffectors/ParticleUniverseSphereCollider.h"
#include "ParticleUniverseAxisAlignedBox.h"

namespace ParticleUniverse
{
	// Constants
	const Real SphereCollider::DEFAULT_RADIUS = 100.0f;

	//-----------------------------------------------------------------------
	SphereCollider::SphereCollider(void) : 
		BaseCollider(),
		mPredictedPosition(Vector3::ZERO),
		mRadius(DEFAULT_RADIUS),
		mInnerCollision(false)
	{
	}
	//-----------------------------------------------------------------------
	const Real SphereCollider::getRadius(void) const
	{
		return mRadius;
	}
	//-----------------------------------------------------------------------
	void SphereCollider::setRadius(const Real radius)
	{
		mRadius = radius;
		mSphere.setRadius(mRadius);
	}
	//-----------------------------------------------------------------------
	bool SphereCollider::isInnerCollision(void) const
	{
		return mInnerCollision;
	}
	//-----------------------------------------------------------------------
	void SphereCollider::setInnerCollision(bool innerCollision)
	{
		mInnerCollision = innerCollision;
	}
	//-----------------------------------------------------------------------
	void SphereCollider::_preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed)
	{
		// Call parent
		BaseCollider::_preProcessParticles(particleTechnique, timeElapsed);

		// Calculate the affectors' center position.
		mSphere.setCenter(getDerivedPosition());
	}
	//-----------------------------------------------------------------------
	void SphereCollider::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		mPredictedPosition = particle->position + mVelocityScale * particle->direction;
		bool collision = false;
		Vector3 distance = particle->position - mDerivedPosition;
		Real distanceLength = distance.length();
		Real scaledRadius = 0.3333f * (_mAffectorScale.x + _mAffectorScale.y + _mAffectorScale.z) * mRadius; // Scaling changed in V 1.3.1

		switch(mIntersectionType)
		{
			case BaseCollider::IT_POINT:
			{
				// Validate for a point-sphere intersection
				//if (mInnerCollision == (distanceLength > mRadius))
				if (mInnerCollision == (distanceLength > scaledRadius))
				{
					// Collision detected (re-position the particle)
					particle->position -= mVelocityScale * particle->direction;
					collision = true;
				}
				else
				{
					distance = mPredictedPosition - mDerivedPosition;
					distanceLength = distance.length();
					//if (mInnerCollision == (distanceLength > mRadius))
					if (mInnerCollision == (distanceLength > scaledRadius))
					{
						// Collision detected
						collision = true;
					}
				}
			}
			break;

			case BaseCollider::IT_BOX:
			{
				// Validate for a box-sphere intersection
				if (particle->particleType != Particle::PT_VISUAL)
					break;

				VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
				AxisAlignedBox box;
				populateAlignedBox(box,
					visualParticle->position, 
					visualParticle->width, 
					visualParticle->height,
					visualParticle->depth);

					if (mInnerCollision != box.intersects(mSphere))
					{
						// Collision detected (re-position the particle)
						particle->position -= mVelocityScale * particle->direction;
						collision = true;
					}
					else
					{
						AxisAlignedBox box;
						populateAlignedBox(box,
							mPredictedPosition, 
							visualParticle->width, 
							visualParticle->height,
							visualParticle->depth);
						if (mInnerCollision != box.intersects(mSphere))
						{
							// Collision detected
							collision = true;
						}
					}
			}
			break;
		}

		if (collision)
		{
			calculateDirectionAfterCollision(particle, distance, distanceLength);
			calculateRotationSpeedAfterCollision(particle);
			particle->addEventFlags(Particle::PEF_COLLIDED);
		}
	}
	//-----------------------------------------------------------------------
	void SphereCollider::calculateDirectionAfterCollision(Particle* particle, Vector3 distance, Real distanceLength)
	{
		switch (mCollisionType)
		{
			case BaseCollider::CT_BOUNCE:
			{
				/** If the particle is on the surface (or just inside the sphere); bounce it
					Make use of formula R = 2 * (-I dot N) * N + I, where
					R = the new direction vector
					I = the old (unit) direction vector before the collision
					N = the Normal at the collision point
				*/
				Real directionLength = particle->direction.length();
				particle->direction.normalise();
				distance.normalise();
				particle->direction = 2 * (-particle->direction.dotProduct(distance)) * distance + particle->direction;

				// Adjust to original speed
				particle->direction *= directionLength;

				// Accelerate/slow down, using the bounce value
				particle->direction *= mBouncyness;
			}
			break;
			case BaseCollider::CT_FLOW:
			{
				/** Reset the position (on the sphere), but keep the direction.
					This doesn't really work good for box-type collisions, because it doesn't take the particle
					dimensions into account.
				*/
				//particle->position = mDerivedPosition + distance * (mRadius / distanceLength);
				Real scaledRadius = 0.3333f * (_mAffectorScale.x + _mAffectorScale.y + _mAffectorScale.z) * mRadius;
				particle->position = mDerivedPosition + distance * (scaledRadius / distanceLength);
			}
			break;
		}
	}
	//-----------------------------------------------------------------------
	void SphereCollider::copyAttributesTo (ParticleAffector* affector)
	{
		BaseCollider::copyAttributesTo(affector);

		SphereCollider* sphereCollider = static_cast<SphereCollider*>(affector);
		sphereCollider->mRadius = mRadius;
		sphereCollider->mSphere = mSphere;
		sphereCollider->mInnerCollision = mInnerCollision;
	}
}
