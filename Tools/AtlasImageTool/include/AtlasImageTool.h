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

#ifndef __ATLAS_IMAGE_TOOL_H__
#define __ATLAS_IMAGE_TOOL_H__

#include "ParticleUniverseAtlasImage.h"
#include "OgreConfigFile.h"
#include "OgreString.h"

class AtlasImageTool
{
	public:

		AtlasImageTool(void) : 
			mAtlasImage(), 
			mConfigFile(),
			mInputFileNames(),
			mInputFrames(),
			mAlpha(),
			mOutputImage("atlas.jpg"),
			mImagePath("") {};
		virtual ~AtlasImageTool(void){};

		/** Read the configuration file.
		*/
		void initialise (const Ogre::String& configFileName);

		/** Create the atlas image.
		*/
		void process (void);

		/** Create an interpolated image.
		*/
		void interpolate (Ogre::Image& interpolatedImage, 
			Ogre::Image& firstImage, 
			Ogre::Image& nextImage,
			Ogre::Real fraction);

		/** Correct the alpha component.
		*/
		void correctAlpha (Ogre::Image& image, Ogre::Real alphaCorrection);

	protected:
		ParticleUniverse::AtlasImage mAtlasImage;
		Ogre::ConfigFile mConfigFile;
		Ogre::StringVector mInputFileNames;
		Ogre::StringVector mInputFrames;
		Ogre::StringVector mAlpha;
		Ogre::String mOutputImage;
		Ogre::String mImagePath;

};

#endif
