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

#ifndef __PU_VISUAL_PARTICLE_H__
#define __PU_VISUAL_PARTICLE_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseParticle.h"
#include "ParticleUniverseMath.h"

namespace ParticleUniverse
{
	/** A VisualParticle is the most obvious implementation of a particle. It represents that particles that can be
		visualised on the screen.
    */
	class _ParticleUniverseExport VisualParticle : public Particle
	{
		public:
			VisualParticle(void);
			virtual ~VisualParticle(void) {};

			/** Current and original colour */
			ColourValue colour;
			ColourValue originalColour;

			/** zRotation is used to rotate the particle in 2D (around the Z-axis)
			@remarks
				There is no relation between zRotation and orientation.
				rotationSpeed in combination with orientation are used for 3D rotation of the particle, while
				zRotation means the rotation around the Z-axis. This type of rotation is typically used for 
				rotating textures. This also means that both types of rotation can be used together.
			*/
			Radian zRotation;

			/** The zRotationSpeed is used in combination with zRotation and defines tha actual rotationspeed
				in 2D. */
			Radian zRotationSpeed;

			/*  Orientation of the particle.
			@remarks
				The orientation of the particle is only visible if the Particle Renderer - such as the Box renderer - 
				supports orientation.
			*/
			Quaternion orientation;
			Quaternion originalOrientation;

			/** The rotation is used in combination with orientation. Because the rotation speed is part
				of the particle itself, it can be changed independently. */
			Real rotationSpeed;

			/** The rotation axis is used in combination with orientation. Because the rotation axis is part
				of the particle itself, it can be changed independently. */
			Vector3 rotationAxis;

			/** Does this particle have it's own dimensions? */
			bool ownDimensions;

			/** Own width
			*/
			Real width;
        
			/** Own height
			*/
			Real height;

			/** Own depth
			*/
			Real depth;

			/** Radius of the particle, to be used for inter-particle collision and such.
			*/
			Real radius;

			/** Animation attributes
			*/
			Real textureAnimationTimeStep;
			Real textureAnimationTimeStepCount;
			uint16 textureCoordsCurrent;
			bool textureAnimationDirectionUp;

			/** Set own dimensions
			*/
			void setOwnDimensions(Real newWidth, Real newHeight, Real newDepth);

			/** @see Particle::_initForEmission()
			*/
			virtual void _initForEmission(void);

			/** @see Particle::_initForExpiration()
			*/
			virtual void _initForExpiration(ParticleTechnique* technique, Real timeElapsed);

			/** Calculate the bounding sphere radius
			*/
			void _calculateBoundingSphereRadius(void);

	};

}
#endif
