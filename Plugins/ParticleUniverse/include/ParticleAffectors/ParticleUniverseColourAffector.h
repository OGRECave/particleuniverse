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

#ifndef __PU_COLOUR_AFFECTOR_H__
#define __PU_COLOUR_AFFECTOR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"

namespace ParticleUniverse
{
	/** This affector is typically used to change the colour of a particle during its lifetime.
    */
	class _ParticleUniverseExport ColourAffector : public ParticleAffector
	{
		public:
			typedef map<Real, ColourValue> ColourMap;
			typedef map<Real, ColourValue>::iterator ColourMapIterator;
			enum ColourOperation
			{
				CAO_MULTIPLY,
				CAO_SET
			};

			// Constants
			static const ColourOperation DEFAULT_COLOUR_OPERATION;

			// Constructor
			ColourAffector(void);

			// Destructor
	        virtual ~ColourAffector(void) {};

			/** 
	        */
			void addColour(Real timeFraction, const ColourValue& colour);

			/** 
	        */
			const ColourMap& getTimeAndColour(void) const;

			/** 
	        */
			void clearColourMap (void);
				
			/** 
	        */
			inline ColourMapIterator _findNearestColourMapIterator(Real timeFraction);

			/** 
	        */
			const ColourOperation& getColourOperation (void) const;

			/** 
	        */
			void setColourOperation (const ColourOperation& colourOperation);

			/** 
	        */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);

		protected:
			ColourMap mColourMap;
			ColourOperation mColourOperation;
	};

}
#endif
