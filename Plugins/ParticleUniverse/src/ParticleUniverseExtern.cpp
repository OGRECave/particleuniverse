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

#include "ParticleUniverseExtern.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	void Extern::_processParticle(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed, bool firstParticle)
	{
		// Call the _firstParticle() function if the first particle in the update loop is encountered.
		if (firstParticle)
		{
			// Perform a precalculation at the first particle
			_firstParticle(particleTechnique, particle, timeElapsed);
		}

		_interface(particleTechnique, particle, timeElapsed);
	}
	//-----------------------------------------------------------------------
	void Extern::_notifyRescaled(const Vector3& scale)
	{
		_mExternScale = scale;
	}
	//-----------------------------------------------------------------------
	void Extern::copyAttributesTo(Extern* externObject)
	{
		copyParentAttributesTo(externObject);
	}
	//-----------------------------------------------------------------------
	void Extern::copyParentAttributesTo(Extern* externObject)
	{
		externObject->setName(mName);
		externObject->mParentTechnique = mParentTechnique;
		externObject->_mExternScale = _mExternScale;
	}

}
