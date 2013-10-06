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

#include "ParticleUniverseCameraDependency.h"

namespace ParticleUniverse
{
	// Constants
	const Real CameraDependency::DEFAULT_DISTANCE_THRESHOLD = 1000000.0f;
	const bool CameraDependency::DEFAULT_INCREASE = false;

	//-----------------------------------------------------------------------
	CameraDependency::CameraDependency(void) :
		IDependency(),
		mThreshold(DEFAULT_DISTANCE_THRESHOLD),
		mIncrease(DEFAULT_INCREASE)
	{
	}
	//-----------------------------------------------------------------------
	CameraDependency::CameraDependency(Real threshold, bool inc) :
		IDependency(),
		mThreshold(threshold),
		mIncrease(inc)
	{
	}
	//-----------------------------------------------------------------------
	bool CameraDependency::affect(Real& baseValue, Real dependencyValue)
	{
		Real f = 0;
		bool changed = false;
		if (mIncrease)
		{
			if (mThreshold != 0)
				f = dependencyValue / mThreshold;
		}
		else
		{
			if (dependencyValue != 0)
				f = mThreshold / dependencyValue;
		}
		if (f > 0 && f <= 1)
		{
			baseValue *= f;
			changed = true;
		}
		
		return changed;
	}
	//-----------------------------------------------------------------------
	void CameraDependency::copyAttributesTo (CameraDependency* cameraDependency)
	{
		cameraDependency->setThreshold(mThreshold);
		cameraDependency->setIncrease(mIncrease);
	}

}
