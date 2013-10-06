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

#ifndef __PU_BOX_EMITTER_H__
#define __PU_BOX_EMITTER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseEmitter.h"

namespace ParticleUniverse
{
	/** The BoxEmitter is a ParticleEmitter that emits particles within a box shape.
    */
	class _ParticleUniverseExport BoxEmitter : public ParticleEmitter
	{
		protected:
			Real mHeight;
			Real mWidth;
			Real mDepth;

			Real mXRange;
			Real mYRange;
			Real mZRange;

		public:
			// Constants
			static const Real DEFAULT_WIDTH;
			static const Real DEFAULT_HEIGHT;
			static const Real DEFAULT_DEPTH;

			BoxEmitter(void);
	        virtual ~BoxEmitter(void) {};

			/** 
	        */
			const Real getHeight(void) const;
			void setHeight(const Real height);

			/** 
	        */
			const Real getWidth(void) const;
			void setWidth(const Real width);

			/** 
	        */
			const Real getDepth(void) const;
			void setDepth(const Real depth);

			/** 
	        */
			virtual void _initParticlePosition(Particle* particle);

			/** 
	        */
			virtual void copyAttributesTo (ParticleEmitter* emitter);
	};

}
#endif
