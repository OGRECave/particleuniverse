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

#include "ParticleAffectors/ParticleUniverseBaseForceAffector.h"

namespace ParticleUniverse
{
	// Constants
	const Vector3 BaseForceAffector::DEFAULT_FORCE_VECTOR(0, 0, 0);
	const BaseForceAffector::ForceApplication BaseForceAffector::DEFAULT_FORCE_APPL = BaseForceAffector::FA_ADD;

	//-----------------------------------------------------------------------
	BaseForceAffector::BaseForceAffector(void) : 
		ParticleAffector(),
		mForceVector(DEFAULT_FORCE_VECTOR),
		mScaledVector(Vector3::ZERO),
		mForceApplication(DEFAULT_FORCE_APPL)
	{
	}
	//-----------------------------------------------------------------------
	void BaseForceAffector::copyAttributesTo (ParticleAffector* affector)
	{
		ParticleAffector::copyAttributesTo(affector);

		BaseForceAffector* baseForceAffector = static_cast<BaseForceAffector*>(affector);
		baseForceAffector->mForceVector = mForceVector;
		baseForceAffector->mForceApplication = mForceApplication;
	}
	//-----------------------------------------------------------------------
	const Vector3& BaseForceAffector::getForceVector(void) const
	{
		return mForceVector;
	}
	//-----------------------------------------------------------------------
	void BaseForceAffector::setForceVector(const Vector3& forceVector)
	{
		mForceVector = forceVector;
	}
	//-----------------------------------------------------------------------
	BaseForceAffector::ForceApplication BaseForceAffector::getForceApplication(void) const
	{
		return mForceApplication;
	}
	//-----------------------------------------------------------------------
	void BaseForceAffector::setForceApplication(BaseForceAffector::ForceApplication forceApplication)
	{
		mForceApplication = forceApplication;
	}
}
