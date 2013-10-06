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

#include "ParticleUniverseAttachable.h"

namespace ParticleUniverse
{
	// Constants
	String Attachable::PU_ATTACHABLE = "PUAttachable";
	//-----------------------------------------------------------------------
	Attachable::Attachable(void) :
		Extern(),
		MovableObject(),
		mAABB(),
		mDistanceThreshold(std::numeric_limits<float>::max()),
		mDistanceThresholdSet(false)
	{
		// The Movableobject name is auto generated to prevent a clash in Ogre
		mBoundingRadius = 1;
		std::stringstream ss; 
		ss << this;
		MovableObject::mName = "PUExtern" + ss.str();
	}
	//-----------------------------------------------------------------------
	void Attachable::_notifyAttached(Ogre::Node* parent, bool isTagPoint)
	{
		MovableObject::_notifyAttached(parent, isTagPoint);
	}
	//-----------------------------------------------------------------------
	void Attachable::_notifyCurrentCamera(Camera* cam)
	{
		Ogre::MovableObject::_notifyCurrentCamera(cam);
	}
	//-----------------------------------------------------------------------
	const String& Attachable::getMovableType(void) const
	{
		return PU_ATTACHABLE;
	}
	//-----------------------------------------------------------------------
	const AxisAlignedBox& Attachable::getBoundingBox(void) const
	{
		return mAABB;
	}
	//-----------------------------------------------------------------------
	Real Attachable::getBoundingRadius(void) const
	{
		return mBoundingRadius;
	}
	//-----------------------------------------------------------------------
	const Real Attachable::getDistanceThreshold(void) const
	{
		return mDistanceThreshold;
	}
	//-----------------------------------------------------------------------
	void Attachable::setDistanceThreshold(const Real distanceThreshold)
	{
		mDistanceThresholdSet = true;
		mDistanceThreshold = distanceThreshold;
	}
	//-----------------------------------------------------------------------
	void Attachable::_updateRenderQueue(Ogre::RenderQueue* queue)
	{
		// Nothing to do here.
	}
	//-----------------------------------------------------------------------
	void Attachable::copyAttributesTo (Extern* externObject)
	{
		Extern::copyAttributesTo(externObject);
		Attachable* attachable = static_cast<Attachable*>(externObject);
		attachable->setDistanceThreshold(mDistanceThreshold);
	}
	//-----------------------------------------------------------------------
	void Attachable::_prepare(ParticleTechnique* technique)
	{
		// Nothing to do here.
	}
	//-----------------------------------------------------------------------
	void Attachable::_unprepare(ParticleTechnique* technique)
	{
		// Nothing to do here.
	}
	//-----------------------------------------------------------------------
	void Attachable::_interface(ParticleTechnique* technique, 
		Particle* particle, 
		Real timeElapsed)
	{
		// Nothing to do here.
	}

}
