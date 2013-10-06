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

#ifndef __PU_BASE_ATTRACTOR_H__
#define __PU_BASE_ATTRACTOR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"

namespace ParticleUniverse
{
	/** This is a baseclass for other Force Affector classes.
    */
	class _ParticleUniverseExport BaseForceAffector : public ParticleAffector
	{
		public:
			enum ForceApplication
			{
				FA_AVERAGE,
				FA_ADD
			};

			// Constants
			static const Vector3 DEFAULT_FORCE_VECTOR;
			static const ForceApplication DEFAULT_FORCE_APPL;

			BaseForceAffector(void);
	        virtual ~BaseForceAffector(void) {};

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);

			/** 
	        */
			const Vector3& getForceVector(void) const;
			void setForceVector(const Vector3& forceVector);

			/** 
	        */
			ForceApplication getForceApplication(void) const;
			void setForceApplication(ForceApplication forceApplication);

		protected:
			Vector3 mForceVector;
			Vector3 mScaledVector;
			ForceApplication mForceApplication;
	};

}
#endif
