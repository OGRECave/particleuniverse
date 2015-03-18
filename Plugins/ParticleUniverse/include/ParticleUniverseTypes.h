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

#ifndef __PU_TYPES_H__
#define __PU_TYPES_H__

#include "OgrePlatform.h"
#include "OgrePrerequisites.h"
#include "OgreString.h"
#include "OgreStringVector.h"
#include "OgreVector2.h"

namespace ParticleUniverse
{
	// Define types
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
	typedef unsigned int uint;
	typedef unsigned short ushort;
	typedef int int32;
	typedef short int16;
	typedef unsigned char uchar;

	/** Define other types. Particle Universe needs its own types to be independent from external resources, such as the Ogre renderer. 
		For now, the types are still borrowed from Ogre. If the Ogre renderer is replaced by another renderer, the types below must be re-implemented.
	*/
	typedef Ogre::Real Real;
	typedef Ogre::Vector3 Vector3;
	typedef Ogre::Vector4 Vector4;
	typedef Ogre::Vector2 Vector2;
	typedef Ogre::Quaternion Quaternion;
	typedef Ogre::ColourValue ColourValue;
	typedef Ogre::String String;
	typedef Ogre::StringUtil StringUtil;
	typedef Ogre::StringConverter StringConverter;
	typedef Ogre::StringVector StringVector;
	typedef Ogre::StringVectorPtr StringVectorPtr;
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR < 10
	typedef StringUtil::StrStreamType StringStream;
#else
	typedef Ogre::StringStream StringStream;
#endif

	// Constants
	const String BLANK_STRING = "";

	// Typedef's for specific 'Ogre' containers.
	// vector container
	template <class T> struct vector : public std::vector<T, Ogre::STLAllocator<T, Ogre::GeneralAllocPolicy> >
	{
	};

	// list container
	template <class T> struct list : public std::list<T, Ogre::STLAllocator<T, Ogre::GeneralAllocPolicy> >
	{ 
	};

	// map container
	template <class T, class M> struct map : public std::map<T, M, std::less<T>, Ogre::STLAllocator<T, Ogre::GeneralAllocPolicy> >
	{ 
	};
}

#endif
