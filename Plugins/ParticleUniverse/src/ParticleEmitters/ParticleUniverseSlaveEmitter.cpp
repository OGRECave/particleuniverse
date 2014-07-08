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

#include "ParticleEmitters/ParticleUniverseSlaveEmitter.h"
#include "ParticleBehaviours/ParticleUniverseSlaveBehaviour.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	SlaveEmitter::SlaveEmitter(void) : 
		ParticleEmitter(),
		TechniqueListener(),
		mMasterParticle(0),
		mMasterTechniqueName(BLANK_STRING),
		mMasterEmitterName(BLANK_STRING),
		mMasterPosition(Vector3::ZERO),
		mMasterDirection(Vector3::ZERO),
		mMasterEmitterNameSet(false)
	{
	}
	//-----------------------------------------------------------------------
	const String& SlaveEmitter::getMasterTechniqueName(void) const
	{
		return mMasterTechniqueName;
	}
	//-----------------------------------------------------------------------
	void SlaveEmitter::setMasterTechniqueName(const String& masterTechniqueName)
	{
		mMasterTechniqueName = masterTechniqueName;
	}
	//-----------------------------------------------------------------------
	const String& SlaveEmitter::getMasterEmitterName(void) const
	{
		return mMasterEmitterName;
	}
	//-----------------------------------------------------------------------
	void SlaveEmitter::setMasterEmitterName(const String& masterEmitterName)
	{
		mMasterEmitterName = masterEmitterName;
		mMasterEmitterNameSet = true;
	}
	//-----------------------------------------------------------------------
	void SlaveEmitter::particleEmitted(ParticleTechnique* particleTechnique, Particle* particle)
	{
		if (mMasterEmitterNameSet && mMasterEmitterName != particle->parentEmitter->getName())
		{
			// Ignore particle
			return;
		}

		/** Keep the data of the emitted particle from the master technique/emitter.
			Emission if a particle in this emitter may NOT be done in the main _update() method of the ParticleSystem,
			but only from here (it´s a slave afterall). That is why the emitter is enabled and disabled again.
		*/
		mMasterPosition = particle->position;
		mMasterDirection = particle->direction;
		mMasterParticle = particle;
		mEnabled = true;
		mParentTechnique->forceEmission(this, 1); // Just emit one, to be in sync with the master.
		mEnabled = false;
	}
	//-----------------------------------------------------------------------
	void SlaveEmitter::_initParticlePosition(Particle* particle)
	{
		// Remark: Don't take the orientation of the node into account, because the position of the master particle is leading.
		particle->position = mMasterPosition;
		particle->originalPosition = particle->position;
	}
	//-----------------------------------------------------------------------
    void SlaveEmitter::_initParticleDirection(Particle* particle)
    {
		particle->direction = mMasterDirection;
		particle->originalDirection = particle->direction;
		particle->originalDirectionLength = particle->direction.length();

		// Make use of the opportunity to set the master particle in the behaviour object (if available)
		SlaveBehaviour* behaviour = static_cast<SlaveBehaviour*>(particle->getBehaviour("Slave"));
		if (behaviour)
		{
			behaviour->masterParticle = mMasterParticle;
		}
    }
	//-----------------------------------------------------------------------
	void SlaveEmitter::_prepare(ParticleTechnique* particleTechnique)
	{
		ParticleSystem* system = particleTechnique->getParentSystem();
		if (system)
		{
			ParticleTechnique* masterTechnique = system->getTechnique(mMasterTechniqueName);
			if (masterTechnique)
			{
				masterTechnique->addTechniqueListener(this);
			}
			mEnabled = false;
		}
	}
	//-----------------------------------------------------------------------
	void SlaveEmitter::_unprepare(ParticleTechnique* particleTechnique)
	{
		ParticleSystem* system = particleTechnique->getParentSystem();
		if (system)
		{
			ParticleTechnique* masterTechnique = system->getTechnique(mMasterTechniqueName);
			if (masterTechnique)
			{
				masterTechnique->removeTechniqueListener(this);
			}
		}
	}
	//-----------------------------------------------------------------------
	void SlaveEmitter::_notifyStart (void)
	{
		ParticleEmitter::_notifyStart();
		setEnabled(false);
	}
	//-----------------------------------------------------------------------
	void SlaveEmitter::copyAttributesTo (ParticleEmitter* emitter)
	{
		ParticleEmitter::copyAttributesTo(emitter);

		SlaveEmitter* slaveEmitter = static_cast<SlaveEmitter*>(emitter);
		slaveEmitter->mMasterTechniqueName = mMasterTechniqueName;
		slaveEmitter->mMasterEmitterName = mMasterEmitterName;
		slaveEmitter->mMasterEmitterNameSet = mMasterEmitterNameSet;
	}
}
