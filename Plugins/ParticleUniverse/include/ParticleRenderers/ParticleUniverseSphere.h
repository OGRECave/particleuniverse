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

#ifndef __PU_SPHERE_H__
#define __PU_SPHERE_H__

#include "ParticleUniversePrerequisites.h"
#include "OgreMemoryAllocatorConfig.h"

namespace ParticleUniverse
{
	class SphereSet;

	/** 
    */
	class _ParticleUniverseExport Sphere : public FXAlloc
	{
		friend class SphereSet;
		friend class SphereRenderer;

		protected:
			bool mOwnRadius;
			Real mRadius;
			bool mRadiusChanged;

		public:
			Vector3 mPosition;
			ColourValue mColour;
			Quaternion mOrientation;
			SphereSet* mParentSet;
			
			/** Default constructor.
			*/
			Sphere(void);

			/** Default destructor.
			*/
	        virtual ~Sphere(void);

			/** Constructor as called by SphereSet.
			*/
			Sphere(const Vector3& position, SphereSet* owner);

			/** 
			*/
			void setPosition(const Vector3& position);

			/** 
			*/
			void setPosition(Real x, Real y, Real z);

			/** 
			*/
			const Vector3& getPosition(void) const;

			/** Sets the colour of this sphere.
			*/
			void setColour(const ColourValue& colour);

			/** Get the colour of this sphere.
			*/
			const ColourValue& getColour(void) const;
		
			/** Resets this sphere to use the parent SphereSet's radius instead of it's own.
			*/
			void resetRadius(void) { mOwnRadius = false; }

			/** Sets the radius for this sphere.
			*/
			void setRadius(Real radius);

			/** Returns true if this box deviates from the SphereSet's default radius.
			*/
			bool hasOwnRadius(void) const;

			/** Retrieves the sphere's personal radius, if hasOwnRadius is true.
			*/
			Real getOwnRadius(void) const;

			/** Internal method for notifying the box of it's owner.
			*/
			void _notifyOwner(SphereSet* owner);
	};

}
#endif
