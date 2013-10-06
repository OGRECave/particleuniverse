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

#ifndef __PU_INTERPARTICLE_COLLIDER_H__
#define __PU_INTERPARTICLE_COLLIDER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseBaseCollider.h"

namespace ParticleUniverse
{
	/** The InterParticleCollider is used to perform particle-particle collision.
    */
	class _ParticleUniverseExport InterParticleCollider : public BaseCollider
	{
		public:
			enum InterParticleCollisionResponse
			{
				IPCR_AVERAGE_VELOCITY,
				IPCR_ANGLE_BASED_VELOCITY
			};

			// Constants
			static const Real DEFAULT_ADJUSTMENT;
			static const InterParticleCollisionResponse DEFAULT_COLLISION_RESPONSE;

			InterParticleCollider(void);
	        virtual ~InterParticleCollider(void){};

			/** Todo
			*/
			Real getAdjustment(void) const;

			/** Todo
			*/
			void setAdjustment(Real adjustment);

			/** Todo
			*/
			InterParticleCollisionResponse getInterParticleCollisionResponse(void) const;

			/** Todo
			*/
			void setInterParticleCollisionResponse(InterParticleCollisionResponse interParticleCollisionResponse);

			/** @copydoc ParticleAffector::_prepare */
			virtual void _prepare(ParticleTechnique* particleTechnique);

			/** @copydoc ParticleAffector::_unprepare */
			virtual void _unprepare(ParticleTechnique* particleTechnique);

			/** @copydoc ParticleAffector::_affect */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);
		
		protected:
			Real mAdjustment;
			InterParticleCollisionResponse mInterParticleCollisionResponse;

			/** Todo
			*/
			bool _validateAndExecuteSphereCollision (Particle* particle1, Particle* particle2, Real timeElapsed);
	};

}
#endif
