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

#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.h"

namespace ParticleUniverse
{
	// Constants
	const bool DoAffectorEventHandler::DEFAULT_PRE_POST = false;

	//-----------------------------------------------------------------------
	DoAffectorEventHandler::DoAffectorEventHandler(void) : 
		ParticleEventHandler(),
		mPrePost(DEFAULT_PRE_POST),
		mAffectorName(BLANK_STRING)
	{
	}
	//-----------------------------------------------------------------------
	void DoAffectorEventHandler::_handle (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		/** Search for the affector.
		*/
		ParticleTechnique* technique = 0;
		ParticleAffector* affector = particleTechnique->getAffector(mAffectorName);
		if (!affector)
		{
			// Search all techniques in this ParticleSystem for an affector with the correct name
			ParticleSystem* system = particleTechnique->getParentSystem();
			size_t size = system->getNumTechniques();
			for(size_t i = 0; i < size; ++i)
			{
				technique = system->getTechnique(i);
				affector = technique->getAffector(mAffectorName);
				if (affector)
				{
					break;
				}
			}
		}

		if (affector)
		{
			// Call the affector even if it has enabled set to 'false'.
			if (mPrePost)
			{
				affector->_preProcessParticles(particleTechnique, timeElapsed);
				affector->_affect(particleTechnique, particle, timeElapsed);
				affector->_postProcessParticles(particleTechnique, timeElapsed);
			}
			else
			{
				affector->_affect(particleTechnique, particle, timeElapsed);
			}
		}
	}
	//-----------------------------------------------------------------------
	void DoAffectorEventHandler::copyAttributesTo (ParticleEventHandler* eventHandler)
	{
		ParticleEventHandler::copyAttributesTo(eventHandler);
		DoAffectorEventHandler* doAffectorEventHandler = static_cast<DoAffectorEventHandler*>(eventHandler);
		doAffectorEventHandler->setAffectorName(mAffectorName);
		doAffectorEventHandler->setPrePost(mPrePost);
	}

}
