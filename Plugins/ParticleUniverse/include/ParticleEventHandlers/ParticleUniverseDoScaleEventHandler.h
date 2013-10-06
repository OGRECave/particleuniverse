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

#ifndef __PU_DO_SCALE_EVENT_HANDLER_H__
#define __PU_DO_SCALE_EVENT_HANDLER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseEventHandler.h"

namespace ParticleUniverse
{
	/** The DoScaleEventHandler scales different particle attributes.
    */
	class _ParticleUniverseExport DoScaleEventHandler : public ParticleEventHandler
	{
		public:
			enum ScaleType
			{
				ST_TIME_TO_LIVE,
				ST_VELOCITY
			};

			// Constants
			static const ScaleType DEFAULT_SCALE_TYPE;
			static const Real DEFAULT_SCALE_FRACTION;

			DoScaleEventHandler(void);
	        virtual ~DoScaleEventHandler(void) {};

			/** Returns the scale type
	        */
			const ScaleType& getScaleType(void) const;

			/** Set the scale type. This scale type identifies to which attribute the scale factor is applied.
	        */
			void setScaleType(const ScaleType& scaleType);

			/** Returns the scale fraction
	        */
			const Real getScaleFraction(void) const;

			/** Set the scale fraction. This scale fraction value is used to scale different attributes if the 
				event handler is called.
	        */
			void setScaleFraction(const Real scaleFraction);

			/** 
	        */
			virtual void _handle (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** Copy attributes to another event handler.
	        */
			virtual void copyAttributesTo (ParticleEventHandler* eventHandler);

		protected:
			Real mScaleFraction;
			ScaleType mScaleType;
	};

}
#endif
