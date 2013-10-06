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

#ifndef __PU_NOISE_H__
#define __PU_NOISE_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse
{
	class _ParticleUniverseExport Noise3D
	{
		public:
			/* Constructor / Destructor */
			Noise3D(void);
			virtual ~Noise3D(void);

			/* Inititialises the noise function */
			void initialise(ushort octaves, double frequency = 1.0, double amplitude = 1.0, double persistence = 1.0);

			/* Returns a noise value between [0, 1]
			@remarks
				The noise is calculated in realtime
			*/
			double noise(double x, double y, double z);

			/* Returns a noise value between [0, 1]
			@remarks
				The noise is calculated in realtime
			*/
			double noise(const Vector3& position);

			/* Creates an image file to test the noise */
			void noise2img(ushort dimension = 255);

		protected:
			int p[512];
			ushort mOctaves;
			double mFrequency;
			double mAmplitude;
			double mPersistence;

			/* Returns a noise value between [0, 1]
			@remarks
				The noise is calculated in realtime
			*/
			double _noise(double x, double y, double z);

			double _fade(double t);
			double _lerp(double t, double a, double b);
			double _grad(int hash, double x, double y, double z);
	};
}

#endif
