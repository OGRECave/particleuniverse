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

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	VisualParticle::VisualParticle(void) : 
		Particle(),
		colour(ColourValue::White),
		originalColour(ColourValue::White),
		zRotation(Radian(0)),
		zRotationSpeed(Radian(0)),
		orientation(Quaternion::IDENTITY),
		originalOrientation(Quaternion::IDENTITY),
		rotationSpeed(0.0f),
		rotationAxis(Vector3::UNIT_Z),
		ownDimensions(false),
		width(1.0f),
		height(1.0f),
		depth(1.0f),
		radius(0.87f),
		textureAnimationTimeStep(0.1f),
		textureAnimationTimeStepCount(0.0f),
		textureCoordsCurrent(0),
		textureAnimationDirectionUp(true)
	{
		particleType = PT_VISUAL;
		mMarkedForEmission  = true; // Default is false, but visual particles are always emitted.
	}
	//-----------------------------------------------------------------------
	void VisualParticle::setOwnDimensions(Real newWidth, Real newHeight, Real newDepth)
	{
		ownDimensions = true;
		if (newWidth)
			width = newWidth;
		if (newHeight)
			height = newHeight;
		if (newDepth)
			depth = newDepth;
		_calculateBoundingSphereRadius();
		parentEmitter->getParentTechnique()->_notifyParticleResized();
	}
	//-----------------------------------------------------------------------
	void VisualParticle::_initForEmission(void)
	{
		Particle::_initForEmission();
		textureAnimationTimeStep = 0.1f;
		textureAnimationTimeStepCount = 0.0f;
		textureCoordsCurrent = 0;
		textureAnimationDirectionUp = true;
	}
	//-----------------------------------------------------------------------
	void VisualParticle::_initForExpiration(ParticleTechnique* technique, Real timeElapsed)
	{
		Particle::_initForExpiration(technique, timeElapsed);
		if (visualData)
		{
			visualData->setVisible(false);
		}
	}
	//-----------------------------------------------------------------------
	void VisualParticle::_calculateBoundingSphereRadius(void)
	{
		//radius = 0.5 * Math::Sqrt(width*width + height*height + depth*depth);
		radius = 0.5f * std::max(depth, std::max(width, height)); // approximation
	}

}
