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

Special thanks to: M. Geissler
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_PHYSX_FLUID_EXTERN_H__
#define __PU_PHYSX_FLUID_EXTERN_H__

#include <vector>

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX

	#include "ParticleUniversePhysXExtern.h"
	#include "NxPhysics.h"
	#include "NxActor.h"
	#include "NxScene.h"

	namespace ParticleUniverse
	{
		/////////////////////////////////////////////////////////////////////////////
		/** The PhysXFluidExtern class is responsible for connecting a physical object to a particle that was emitted by Particle Universe.
		This can be either by means of regular actors (one particle relates to one actor with one shape) or a fluid is used, 
		in which case the Particle Universe particles are 'connected' to a fluid particle. As soon as createFluid() is called, fluid
		particles are used.
		*/
		class _ParticleUniverseExport PhysXFluidExtern : public PhysXExtern
		{
		public:
			PhysXFluidExtern(void);
			virtual ~PhysXFluidExtern(void);

			//////////////////////////////////////////////////////////////////////////

			/** See PhysicsExtern
			*/
			virtual void _initParticleForEmission(Particle* particle);

			/** See PhysicsExtern
			*/
			virtual void _initParticleForExpiration(Particle* particle);

			//////////////////////////////////////////////////////////////////////////

			/** This is a specialized create function for fluids
			*/
			PhysicsActor* createPhysXFluidParticle(Particle* particle);

			//////////////////////////////////////////////////////////////////////////

			/** See Extern
			*/
			virtual void _prepare(ParticleTechnique* technique);

			/** See Extern: Override the implementation of Extern
			Here we call ProcessPhysXDeletionIDBuffer to sync the deleted and alive particles
			*/
			virtual void _processParticle(
				ParticleTechnique* particleTechnique, 
				Particle* particle, 
				Real timeElapsed, 
				bool firstParticle);


			/**
			*/
			virtual void _preProcessParticles(ParticleTechnique* technique, Real timeElapsed);

			/** See Extern
			Here we sync the properies of PhysX fluid particles with PU particles (like position, etc)
			*/
			virtual void _postProcessParticles(ParticleTechnique* technique, Real timeElapsed);

			/** See Extern: Ignore
			*/
			virtual void _interface(ParticleTechnique* technique, 
				Particle* particle, 
				Real timeElapsed){};

			/** See Extern.
			*/
			virtual void _notifyStop (void);

			/** Create a fluid using the current fluid descriptor.
			*/
			void createFluid(void);

			/** Create a fluid using the fluid descriptor in the argument.
			*/
			void createFluid(const NxFluidDesc& nxFluidDesc);

			/** Delete a fluid.
			*/
			void destroyFluid(void);

			/** This function is called by the PhysXFluidBridge. Every time it is called, the flag that the physics should be updated is set to true.
			So this says that we should read the values from PhysX particles and set them to the values of PU particles in the next update
			*/
			virtual void synchronize(Real timeElapsed);

			/** 
			*/
			virtual void copyAttributesTo (Extern* externObject);

			//////////////////////////////////////////////////////////////////////////
			NxFluidDesc mFluidDesc;							// used only for creating the fluid
		protected:
			/** This function goes through all elements in the mDeletedParticlesID array and makes the corresponding element
			in mPUParticles to be 0. It also deletes a PU particle corresponding to a PhysX ID if it is still alive. We do 
			this in case the particle dies prematurely (it hits a drain)
			*/
			void processPhysXDeletionIDBuffer();

		protected:

			bool mSynchronize;	// Flag that is set by the PhysXFluidBridge if the scene is simulated.
			
			NxScene* mScene;	// the scene where we should create the fluid
			
			//////////////////////////////////////////////////////////////////////////
			NxFluid* mFluid;								// a pointer to our fluid
			//NxFluidDesc mFluidDesc;							// used only for creating the fluid
			NxU32 mMaxParticles;							// the maximum number of particles in this system

			//////////////////////////////////////////////////////////////////////////
			NxParticleIdData mCreationIDWriteData;	
			NxU32* mCreatedParticlesID; 
			NxU32 mNumCreatedParticles; 

			NxParticleIdData mDeletionIDWriteData;
			NxU32* mDeletedParticlesID; 
			NxU32 mNumDeletedParticles;

			//////////////////////////////////////////////////////////////////////////
			NxU32 mNumAliveParticles;						// the number of active particles (written by PhysX)
			ParticleStructure* mPhysXFluidParticles;		// the data about the alive particles (written by PhysX)
			
			//////////////////////////////////////////////////////////////////////////
			Particle** mPUParticles;							// the visual representation of the particles
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif
