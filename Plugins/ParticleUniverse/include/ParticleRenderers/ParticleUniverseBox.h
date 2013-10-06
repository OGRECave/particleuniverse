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

#ifndef __PU_BOX_H__
#define __PU_BOX_H__

#include "ParticleUniversePrerequisites.h"
#include "OgreMemoryAllocatorConfig.h"

namespace ParticleUniverse
{
	class BoxSet;

	/** 
    */
	class _ParticleUniverseExport Box : public FXAlloc
	{
		friend class BoxSet;
		friend class BoxRenderer;

		protected:
			bool mOwnDimensions;
			Real mWidth;
			Real mHeight;
			Real mDepth;
			Real mHalfWidth;
			Real mHalfHeight;
			Real mHalfDepth;
			bool mDimensionsChanged;


			/** Corners of the box in local space.
			*/
			Vector3 mCorners[8];
			Vector3 mTempCorner;

			/** Calculates the corners.
			*/
			void _calculateCorners(void);

		public:
			Vector3 mPosition;
			ColourValue mColour;
			Quaternion mOrientation;
			BoxSet* mParentSet;
			
			/** Default constructor.
			*/
			Box(void);

			/** Default destructor.
			*/
	        virtual ~Box(void);

			/** Constructor as called by BoxSet.
			*/
			Box(const Vector3& position, BoxSet* owner);

			/** Return the boxcorner in local space of a specified corner.
			*/
			const Vector3& getCorner(size_t index);

			/** Return the boxcorner in world space of a specified corner.
			*/
			const Vector3& getWorldspaceCorner(size_t index);

			/** 
			*/
			void setPosition(const Vector3& position);

			/** 
			*/
			void setPosition(Real x, Real y, Real z);

			/** 
			*/
			const Vector3& getPosition(void) const;

			/** Sets the colour of this box.
			*/
			void setColour(const ColourValue& colour);

			/** Get the colour of this box.
			*/
			const ColourValue& getColour(void) const;
		
			/** Resets this box to use the parent BoxSet's dimensions instead of it's own.
			*/
			void resetDimensions(void) { mOwnDimensions = false; }

			/** Sets the width, height and depth for this box.
			*/
			void setDimensions(Real width, Real height, Real depth);

			/** Returns true if this box deviates from the BoxSet's default dimensions.
			@see
				Box::setDimensions
			*/
			bool hasOwnDimensions(void) const;

			/** Retrieves the box's personal width, if hasOwnDimensions is true.
			*/
			Real getOwnWidth(void) const;

			/** Retrieves the box's personal height, if hasOwnDimensions is true.
			*/
			Real getOwnHeight(void) const;

			/** Retrieves the box's personal depth, if hasOwnDimensions is true.
			*/
			Real getOwnDepth(void) const;

			/** Internal method for notifying the box of it's owner.
			*/
			void _notifyOwner(BoxSet* owner);
	};

}
#endif
