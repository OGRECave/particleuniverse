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

#ifndef __PU_SLAVE_EMITTER_H__
#define __PU_SLAVE_EMITTER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseTechniqueListener.h"

namespace ParticleUniverse
{
	/** The SlaveEmitter makes the particle act as a slave, so it follows another particle to which it is related. This only applies
		at the creation (initialisation) of the particle. For further slave behaviour during the lifespan of the particle, the 
		SlaveBehaviour must be used also.
    */
	class _ParticleUniverseExport SlaveEmitter : public ParticleEmitter, public TechniqueListener
	{
		protected:
			Particle* mMasterParticle;
			String mMasterTechniqueName;
			String mMasterEmitterName;
			Vector3 mMasterPosition;
			Vector3 mMasterDirection;
			bool mMasterEmitterNameSet;

		public:
			SlaveEmitter(void);
	        virtual ~SlaveEmitter(void){};

			/** 
	        */
			const String& getMasterTechniqueName(void) const;

			/** 
	        */
			void setMasterTechniqueName(const String& masterTechniqueName);

			/** 
	        */
			const String& getMasterEmitterName(void) const;

			/** See ParticleEmitter.
	        */
			virtual void _initParticlePosition(Particle* particle);

			/** See ParticleEmitter.
	        */
			virtual void _initParticleDirection(Particle* particle);

			/** See ParticleEmitter.
	        */
			virtual void _prepare(ParticleTechnique* particleTechnique);

			/** See ParticleEmitter.
	        */
			virtual void _unprepare(ParticleTechnique* particleTechnique);

			/** See ParticleEmitter.
			*/
			virtual void _notifyStart (void);

			/** Initialise the emitted particle. This means that its position is set.
	        */
			virtual void particleEmitted(ParticleTechnique* particleTechnique, Particle* particle);

			/** No implementation.
	        */
			virtual void particleExpired(ParticleTechnique* particleTechnique, Particle* particle){};

			/** 
	        */
			void setMasterEmitterName(const String& masterEmitterName);

			/** 
	        */
			virtual void copyAttributesTo (ParticleEmitter* emitter);
	};

}
#endif
