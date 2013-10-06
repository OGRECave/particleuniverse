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
	#if OGRE_DEBUG_MODE
		#define PU_NEW_T(T, category) new (::Ogre::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
		#define PU_DELETE_T(ptr, T, category) if(ptr){(ptr)->~T(); ::Ogre::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}		
		#define PU_NEW_ARRAY_T(T, count, category) ::Ogre::constructN(static_cast<T*>(::Ogre::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 
		#define PU_DELETE_ARRAY_T(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Ogre::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}
		#define PU_NEW new (__FILE__, __LINE__, __FUNCTION__)
		#define PU_DELETE delete
		#define PU_ALLOC_T(T, count, category) static_cast<T*>(::Ogre::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))
		#define PU_FREE(ptr, category) ::Ogre::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)
		#define PU_DELETE_ARRAY_BYTES(ptr, category) ::Ogre::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)

	#else
		#define PU_NEW_T(T, category) new (::Ogre::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T))) T
		#define PU_DELETE_T(ptr, T, category) if(ptr){(ptr)->~T(); ::Ogre::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}
		#define PU_NEW_ARRAY_T(T, count, category) ::Ogre::constructN(static_cast<T*>(::Ogre::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count))), count)
		#define PU_DELETE_ARRAY_T(ptr, T, count, category) if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Ogre::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr);}
		#define PU_NEW new 
		#define PU_DELETE delete
		#define PU_ALLOC_T(T, count, category) static_cast<T*>(::Ogre::CategorisedAllocPolicy<category>::allocateBytes(sizeof(T)*(count)))
		#define PU_FREE(ptr, category) ::Ogre::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)
		#define PU_DELETE_ARRAY_BYTES(ptr, category) ::Ogre::CategorisedAllocPolicy<category>::deallocateBytes((void*)ptr)
	#endif

	// Typedef the allocation policy for factories
	typedef Ogre::FactoryAlloc FactoryAlloc;
	typedef Ogre::FXAlloc FXAlloc;
	typedef Ogre::SerializerAlloc SerializerAlloc;

}

#endif
