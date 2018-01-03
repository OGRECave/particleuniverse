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

#ifndef __PU_ALLOC_H__
#define __PU_ALLOC_H__

#include "OgreMemoryAllocatorConfig.h"

namespace ParticleUniverse
{
	// Below a trick to use an enum of the Ogre namespace into the ParticleUniverse namespace
	typedef Ogre::MemoryCategory MemoryCategory;
	static const MemoryCategory MEMCATEGORY_GENERAL = Ogre::MEMCATEGORY_GENERAL;
	static const MemoryCategory MEMCATEGORY_SCENE_OBJECTS = Ogre::MEMCATEGORY_SCENE_OBJECTS;
	static const MemoryCategory MEMCATEGORY_SCENE_CONTROL = Ogre::MEMCATEGORY_SCENE_CONTROL;
	static const MemoryCategory MEMCATEGORY_SCRIPTING = Ogre::MEMCATEGORY_SCRIPTING;

	// Define the new and delete macro's
    #define PU_NEW_T OGRE_NEW_T
    #define PU_DELETE_T OGRE_DELETE_T
    #define PU_NEW_ARRAY_T OGRE_NEW_ARRAY_T
    #define PU_DELETE_ARRAY_T OGRE_DELETE_ARRAY_T
    #define PU_NEW OGRE_NEW
    #define PU_DELETE OGRE_DELETE
    #define PU_ALLOC_T OGRE_ALLOC_T
    #define PU_FREE OGRE_FREE
    #define PU_DELETE_ARRAY_BYTES OGRE_FREE

	// Typedef the allocation policy for factories
	typedef Ogre::FactoryAlloc FactoryAlloc;
	typedef Ogre::FXAlloc FXAlloc;
	typedef Ogre::SerializerAlloc SerializerAlloc;

}

#endif
