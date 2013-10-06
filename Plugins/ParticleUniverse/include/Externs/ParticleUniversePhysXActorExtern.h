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

#ifndef __PU_PHYSX_ACTOR_EXTERN_H__
#define __PU_PHYSX_ACTOR_EXTERN_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX

	#include "ParticleUniversePhysXExtern.h"
	#include "NxPhysics.h"
	#include "NxActor.h"
	#include "NxScene.h"

	namespace ParticleUniverse
	{
		/** The PhysXActorExtern class is responsible for connecting a physical object to a particle that was emitted by Particle Universe.
		*/
		class _ParticleUniverseExport PhysXActorExtern : public PhysXExtern
		{
			public:
				PhysXActorExtern(void);
				virtual ~PhysXActorExtern(void);

				/** See PhysicsExtern
				*/
				virtual void _initParticleForEmission(Particle* particle);

				/** See PhysicsExtern
				*/
				virtual void _initParticleForExpiration(Particle* particle);

				/** See PhysicsExtern
				*/
				virtual PhysicsActor* createPhysicsActor(PhysicsActorDesc* physicsActorDesc, PhysicsShapeDesc* physicsShapeDesc);

				/** See PhysicsExtern
				*/
				virtual void destroyPhysicsActor(PhysicsActor* physicsActor);

				/** See Extern
				*/
				virtual void _prepare(ParticleTechnique* technique);

				/** See Extern
				*/
				virtual void _unprepare(ParticleTechnique* technique);

				/** See Extern: Override the implementation of Extern
				*/
				virtual void _processParticle(
					ParticleTechnique* particleTechnique, 
					Particle* particle, 
					Real timeElapsed, 
					bool firstParticle);

				/** See Extern: Ignore
				*/
				virtual void _interface(ParticleTechnique* technique, 
					Particle* particle, 
					Real timeElapsed){};

				/** See Extern
				*/
				virtual void _postProcessParticles(ParticleTechnique* technique, Real timeElapsed);

				/** See PhysXEntern
				*/
				virtual void synchronize(Real timeElapsed);

				/** 
				*/
				virtual void copyAttributesTo (Extern* externObject);

				/** Set the physics shape.
				*/
				void setPhysicsShape(PhysicsShapeDesc* physicsShapeDesc);

				/** Public attributes.
				*/
				PhysicsShapeDesc* mPhysicsShapeDesc; // Defines the physical shape of the particles; only one shape is allowed.

			protected:

				NxScene* mScene;
				bool mSynchronize; // Flag that is set by the PhysXBridge if the scene is simulated.
				Real mTimeElapsedSynchronize;
				NxU32 mMaxParticles;
				ParticleStructure* mPhysXParticles;
				NxU32 mPhysXParticlesPointer;
				NxU32 mStart;
				NxParticleData mParticles;
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif
