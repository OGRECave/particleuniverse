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

#ifndef __PU_ATLAS_IMAGE_H__
#define __PU_ATLAS_IMAGE_H__

#include "ParticleUniversePrerequisites.h"
#include "OgreImage.h"

namespace ParticleUniverse
{
	/** The AtlasImage class offers the possibility to create one (atlas) image from other images.
	*/
	class _ParticleUniverseExport AtlasImage
	{
		public:
			AtlasImage(void);
			virtual ~AtlasImage(void);

			/** If true, the atlas image if recompiled after each change (i.e. an image is added).
			@remarks
				If false, compilation must be done manually (calling the _compile() function)
			*/
			const bool getAlwaysUpdate (void) const;

			/** See getAlwaysUpdate
			*/
			void setAlwaysUpdate (const bool alwaysUpdate);

			/** Add an Ogre Image to the atlas.
			@remarks
				All added images must have the same format. The AtlasImage tries to optimise its own image size.
			@par
				Internally, a copy is made from each added image. This means that the AtlasImage class isn´t responsible
				for destruction of the the added image pointer.
			*/
			bool addImage (const Ogre::Image* image);

			/** Get a pointer to the Ogre image of the atlas.
			*/
			const Ogre::Image* getImage (void) const;

			/** Construct the atlas image from all added images.
			@remarks
				This internal method is called as soon as an image is added to the atlas, so it can be updated.
			*/
			void _compile (void);

			/** Save the atlas image as a file. The filename must include the extention (png, jpg)
			*/
			void save (const String& filename);

		protected:
			typedef vector<Ogre::Image*> ImageList;
			ImageList mImageList;
			Ogre::Image* mAtlas;
			size_t mIndividualImageWidth;
			size_t mIndividualImageHeight;
			size_t mIndividualImageRowSpan;
			Ogre::PixelFormat mPixelFormat;
			bool mAlwaysUpdate;
	};

}
#endif
