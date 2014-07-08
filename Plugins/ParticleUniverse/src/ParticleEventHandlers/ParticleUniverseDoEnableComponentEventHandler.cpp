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

#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	DoEnableComponentEventHandler::DoEnableComponentEventHandler(void) : 
		ParticleEventHandler(),
		mComponentName(BLANK_STRING),
		mComponentType(CT_EMITTER),
		mComponentEnabled(true)
	{
	}
	//-----------------------------------------------------------------------
	void DoEnableComponentEventHandler::_handle (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		/** Search for the component.
		*/
		ParticleTechnique* technique = 0;
		switch (mComponentType)
		{
			case CT_EMITTER:
			{
				ParticleEmitter* emitter = particleTechnique->getEmitter(mComponentName);
				if (!emitter)
				{
					// Search all techniques in this ParticleSystem for an emitter with the correct name
					ParticleSystem* system = particleTechnique->getParentSystem();
					size_t size = system->getNumTechniques();
					for(size_t i = 0; i < size; ++i)		
					{
						technique = system->getTechnique(i);
						emitter = technique->getEmitter(mComponentName);
						if (emitter)
						{
							break;
						}
					}
				}
				if (emitter)
				{
					emitter->setEnabled(mComponentEnabled);
				}
			}
			break;

			case CT_AFFECTOR:
			{
				ParticleAffector* affector = particleTechnique->getAffector(mComponentName);
				if (!affector)
				{
					// Search all techniques in this ParticleSystem for an affector with the correct name
					ParticleSystem* system = particleTechnique->getParentSystem();
					size_t size = system->getNumTechniques();
					for(size_t i = 0; i < size; ++i)
					{
						technique = system->getTechnique(i);
						affector = technique->getAffector(mComponentName);
						if (affector)
						{
							break;
						}
					}
				}
				if (affector)
				{
					affector->setEnabled(mComponentEnabled);
				}
			}
			break;

			case CT_OBSERVER:
			{
				ParticleObserver* observer = particleTechnique->getObserver(mComponentName);
				if (!observer)
				{
					// Search all techniques in this ParticleSystem for an observer with the correct name
					ParticleSystem* system = particleTechnique->getParentSystem();
					size_t size = system->getNumTechniques();
					for(size_t i = 0; i < size; ++i)		
					{
						technique = system->getTechnique(i);
						observer = technique->getObserver(mComponentName);
						if (observer)
						{
							break;
						}
					}
				}
				if (observer)
				{
					observer->setEnabled(mComponentEnabled);
				}
			}
			break;

			case CT_TECHNIQUE:
			{
				// Search in this ParticleSystem for a technique with the correct name
				ParticleSystem* system = particleTechnique->getParentSystem();
				technique = system->getTechnique(mComponentName);
				if (technique)
				{
					technique->setEnabled(mComponentEnabled);
				}
			}
			break;
		}
	}
	//-----------------------------------------------------------------------
	void DoEnableComponentEventHandler::copyAttributesTo (ParticleEventHandler* eventHandler)
	{
		ParticleEventHandler::copyAttributesTo(eventHandler);
		DoEnableComponentEventHandler* doEnableComponentEventHandler = static_cast<DoEnableComponentEventHandler*>(eventHandler);
		doEnableComponentEventHandler->setComponentType(mComponentType);
		doEnableComponentEventHandler->setComponentName(mComponentName);
		doEnableComponentEventHandler->setComponentEnabled(mComponentEnabled);
	}

}
