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

#include "ParticleEventHandlers/ParticleUniverseDoScaleEventHandler.h"

namespace ParticleUniverse
{
	// Constants
	const DoScaleEventHandler::ScaleType DoScaleEventHandler::DEFAULT_SCALE_TYPE = DoScaleEventHandler::ST_TIME_TO_LIVE;
	const Real DoScaleEventHandler::DEFAULT_SCALE_FRACTION = 0.2f;

	//-----------------------------------------------------------------------
	DoScaleEventHandler::DoScaleEventHandler(void) : 
		ParticleEventHandler(),
		mScaleFraction(DEFAULT_SCALE_FRACTION),
		mScaleType(DEFAULT_SCALE_TYPE)
	{
	}
	//-----------------------------------------------------------------------
	const DoScaleEventHandler::ScaleType& DoScaleEventHandler::getScaleType(void) const
	{
		return mScaleType;
	}
	//-----------------------------------------------------------------------
	void DoScaleEventHandler::setScaleType(const DoScaleEventHandler::ScaleType& scaleType)
	{
		mScaleType = scaleType;
	}
	//-----------------------------------------------------------------------
	const Real DoScaleEventHandler::getScaleFraction(void) const
	{
		return mScaleFraction;
	}
	//-----------------------------------------------------------------------
	void DoScaleEventHandler::setScaleFraction(const Real scaleFraction)
	{
		mScaleFraction = scaleFraction;
	}
	//-----------------------------------------------------------------------
	void DoScaleEventHandler::_handle (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		if (!particle)
			return;

		switch (mScaleType)
		{
			case ST_TIME_TO_LIVE:
				particle->timeToLive -= timeElapsed * mScaleFraction * particle->timeToLive;
			break;

			case ST_VELOCITY:
				particle->direction += timeElapsed * mScaleFraction * particle->direction;
			break;
		}
	}
	//-----------------------------------------------------------------------
	void DoScaleEventHandler::copyAttributesTo (ParticleEventHandler* eventHandler)
	{
		ParticleEventHandler::copyAttributesTo(eventHandler);
		DoScaleEventHandler* doScaleEventHandler = static_cast<DoScaleEventHandler*>(eventHandler);
		doScaleEventHandler->setScaleFraction(mScaleFraction);
		doScaleEventHandler->setScaleType(mScaleType);
	}
}
