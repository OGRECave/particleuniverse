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

#include "ParticleAffectors/ParticleUniverseColourAffector.h"

namespace ParticleUniverse
{
	// Constants
	const ColourAffector::ColourOperation ColourAffector::DEFAULT_COLOUR_OPERATION = ColourAffector::CAO_SET;

	//-----------------------------------------------------------------------
	ColourAffector::ColourAffector(void) : 
		ParticleAffector(),
		mColourOperation(DEFAULT_COLOUR_OPERATION)
	{
	}
	//-----------------------------------------------------------------------
	void ColourAffector::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		ColourAffector* colourAffector = static_cast<ColourAffector*>(affector);
		colourAffector->mColourMap = mColourMap;
		colourAffector->mColourOperation = mColourOperation;
	}
	//-----------------------------------------------------------------------
	const ColourAffector::ColourOperation& ColourAffector::getColourOperation (void) const
	{
		return mColourOperation;
	}
	//-----------------------------------------------------------------------
	void ColourAffector::setColourOperation (const ColourAffector::ColourOperation& colourOperation)
	{
		mColourOperation = colourOperation;
	}
	//-----------------------------------------------------------------------
	void ColourAffector::addColour (Real timeFraction, const ColourValue& colour)
	{
		mColourMap[timeFraction] = colour;
	}
    //-----------------------------------------------------------------------
	const ColourAffector::ColourMap& ColourAffector::getTimeAndColour(void) const
	{
		return mColourMap;
	}
	//-----------------------------------------------------------------------
	void ColourAffector::clearColourMap (void)
	{
		mColourMap.clear();
	}
	//-----------------------------------------------------------------------
	void ColourAffector::_affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed)
	{
		// Fast rejection
		if (mColourMap.empty())
			return;

		// Only continue if the particle is a visual particle
		if (particle->particleType != Particle::PT_VISUAL)
			return;

		VisualParticle* visualParticle = static_cast<VisualParticle*>(particle);
		
		// Linear interpolation of the colour
		ColourValue colour = ColourValue::White;
		Real timeFraction = (visualParticle->totalTimeToLive - visualParticle->timeToLive) / visualParticle->totalTimeToLive;
		ColourMapIterator it1 = _findNearestColourMapIterator(timeFraction);
		ColourMapIterator it2 = it1;
		it2++;
		if (it2 != mColourMap.end())
		{
			// Interpolate colour
			colour = it1->second + ((it2->second - it1->second) * ((timeFraction - it1->first)/(it2->first - it1->first)));
		}
		else
		{
			colour = it1->second;
		}

		// Determine operation
		if (mColourOperation == CAO_SET)
		{
			// No operation, so just set the colour
			visualParticle->colour = colour;
		}
		else
		{
			// Multiply
			visualParticle->colour = colour * visualParticle->originalColour;
		}
	}
    //-----------------------------------------------------------------------
    ColourAffector::ColourMapIterator ColourAffector::_findNearestColourMapIterator(Real timeFraction)
    {
		ColourMapIterator it;
		for (it = mColourMap.begin(); it != mColourMap.end(); ++it)
		{
			if (timeFraction < it->first)
			{
				if (it == mColourMap.begin())
					return it;
				else
					return --it;
			}
		}

		// If not found return the last valid iterator
		return --it;
    }
}
