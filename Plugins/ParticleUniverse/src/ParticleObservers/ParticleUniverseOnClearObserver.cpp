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

#include "ParticleObservers/ParticleUniverseOnClearObserver.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	void OnClearObserver::_notifyStart (void)
	{
		ParticleObserver::_notifyStart();
		mContinue = false;
	}
	//-----------------------------------------------------------------------
	bool OnClearObserver::_observe (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		// This function always returns false, but is never called (but it must be implemented).
		return false;
	}
	//-----------------------------------------------------------------------
	void OnClearObserver::_processParticle(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed, bool firstParticle)
	{
		// Override the _processParticle() function, because we don't observe individual particles.
		if (firstParticle)
		{
			// Only continue to validate if there was at least 1 particle emitted, otherwise the event-
			// handlers are already called at the very start (when there are no particle yet).
			mContinue = true;
		}
	}
	//-----------------------------------------------------------------------
	void OnClearObserver::_postProcessParticles(ParticleTechnique* technique, Real timeElapsed)
	{
		if (mContinue)
		{
			if (!(technique->getNumberOfEmittedParticles() > 0))
			{
				/** Handle the event. Use 0 as the particle pointer. This means that not all eventhandlers
					are suitable. If they expect a particle (and most eventhandlers do), it could
					result in an exception. Handlers such as the DoEnableComponentEventHandler and the
					DoStopSystemEventHandler however can be used without any problem.
				*/
				_handleEvent (technique, 0, timeElapsed);
				mContinue = false;
			}
		}
	}

}
