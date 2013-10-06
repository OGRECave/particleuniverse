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

#ifndef __PU_PHYSX_EXTERN_H__
#define __PU_PHYSX_EXTERN_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX

	#include "ParticleUniversePhysicsExtern.h"
	#include "NxPhysics.h"
	#include "NxActor.h"
	#include "NxScene.h"

	namespace ParticleUniverse
	{
		class _ParticleUniverseExport PhysXActor : public PhysicsActor
		{
			public:
				// Constructor
				PhysXActor(void) : 
					PhysicsActor(),
					nxActor(0),
					nxParticleData(0){};

				// Destructor
				virtual ~PhysXActor(void) {};

				NxActor* nxActor;
				NxParticleData* nxParticleData;
		};
		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------
		class ParticleStructure
		{
			public:
				// Constructor and Destructor
				ParticleStructure(void) : 
					lifetime(0),
					id(0),
					density(0),
					bufferFlag(0),
					particle(0){};
				virtual ~ParticleStructure(void) {};

				// Public attributes
				NxVec3	position;
				NxVec3  velocity;
				NxReal  lifetime;
				NxVec3	collisionNormal;
				NxU32	id;
				NxU32   bufferFlag;
				NxReal	density;
				Particle* particle;
		};
		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------
		/** The PhysXExtern class is a base class for PhysX.
		*/
		class _ParticleUniverseExport PhysXExtern : public PhysicsExtern
		{
			public:
				PhysXExtern(void) : mCollisionGroup(0){};
				virtual ~PhysXExtern(void){};

				/** This function is called by the PhysXBridge. Every time it is called, the physics are updated.
				*/
				virtual void synchronize(Real timeElapsed) = 0;

				/** Get the collision group.
				*/
				virtual uint16 getCollisionGroup(void) const
				{
					return mCollisionGroup;
				};
				
				/** Set the collision group.
				*/
				virtual void setCollisionGroup(uint16 collisionGroup)
				{
					mCollisionGroup = collisionGroup;
				};

			protected:
				uint16 mCollisionGroup;
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif
