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

#include "Externs/ParticleUniverseSphereColliderExtern.h"
#include "OgreNode.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	void SphereColliderExtern::_preProcessParticles(ParticleTechnique* particleTechnique, Real timeElapsed)
	{
		if (isAttached())
		{
			// Use the position of the parent node in this case.
			position = getParentNode()->_getDerivedPosition();
			mDerivedPosition = position;
			mSphere.setCenter(mDerivedPosition);
		}
	}
	//-----------------------------------------------------------------------
	void SphereColliderExtern::_interface(ParticleTechnique* technique, 
		Particle* particle, 
		Real timeElapsed)
	{
		if (isAttached())
		{
			if (mDistanceThresholdSet && mDerivedPosition.distance(particle->position) > mDistanceThreshold)
				return;

			// Use the derived SphereCollider functionality here
			_affect(technique, particle, timeElapsed);
		}
	}
	//-----------------------------------------------------------------------
	void SphereColliderExtern::copyAttributesTo (Extern* externObject)
	{
		Attachable::copyAttributesTo(externObject);

		SphereColliderExtern* sphereColliderExtern = static_cast<SphereColliderExtern*>(externObject);
		SphereCollider* sphereCollider = static_cast<SphereCollider*>(sphereColliderExtern);
		SphereCollider::copyAttributesTo(sphereCollider);
	}
}
