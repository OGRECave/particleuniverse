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
#include "OgreVector.h"
#include "OgreLogManager.h"
#include "OgreAny.h"
#include "OgreAxisAlignedBox.h"
#include "OgrePlane.h"
#include "OgreMath.h"
#include "OgreController.h"
#include "OgreControllerManager.h"
#include "OgreCamera.h"
#include "OgreRadixSort.h"
#include "OgreMemoryAllocatorConfig.h"
#include "OgreSimpleSpline.h"
#include "OgreSphere.h"
#include "OgreException.h"
#include "OgreConfigFile.h"

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

#define EXCEPT(num, desc, src) OGRE_EXCEPT(num, desc, src);

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
	typedef Ogre::ControllerValueRealPtr ControllerValueRealPtr;
	typedef Ogre::ControllerManager ControllerManager;
	typedef Ogre::ConfigFile ConfigFile;
	typedef Ogre::String String;
	typedef Ogre::StringUtil StringUtil;
	typedef Ogre::StringConverter StringConverter;
	typedef Ogre::StringVector StringVector;
	typedef Ogre::StringVectorPtr StringVectorPtr;
	typedef Ogre::StringStream StringStream;
	typedef Ogre::LogManager LogManager;
	typedef Ogre::Any Any;
	typedef Ogre::AxisAlignedBox AxisAlignedBox;
	typedef Ogre::Plane Plane;
	typedef Ogre::Radian Radian;
	typedef Ogre::Angle Angle;
	typedef Ogre::SimpleSpline SimpleSpline;
	typedef Ogre::Sphere SimpleSphere;
	typedef Ogre::Exception Exception;
	typedef Ogre::Math Math;
	// To be re-implemented:
	// ====================
	// Math::Abs
	// Math::Ceil
	// Math::Cos
	// Math::RangeRandom
	// Math::Sin
	// Math::Sign
	// Math::Sqrt
	// Math::UnitRandom
	// Math::SymmetricRandom

	// Math::POS_INFINITY
	// Math::NEG_INFINITY
	// Math::TWO_PI
	// Math::PI

	using Ogre::Controller;
	template <typename T> using ControllerValue = Ogre::ControllerValue<T>;
	typedef Ogre::Camera Camera;

	template <class TContainer, class TContainerValueType, typename TCompValueType>
	using RadixSort = Ogre::RadixSort<TContainer, TContainerValueType, TCompValueType>;

	typedef Ogre::SortMode SortMode;
	static const SortMode SM_DIRECTION = Ogre::SM_DIRECTION;
	static const SortMode SM_DISTANCE = Ogre::SM_DISTANCE;

	// Typedef the allocation policy for factories
	typedef Ogre::FactoryAlloc FactoryAlloc;
	typedef Ogre::FXAlloc FXAlloc;
	typedef Ogre::SerializerAlloc SerializerAlloc;

	typedef Ogre::MemoryCategory MemoryCategory;
	enum {
		MEMCATEGORY_GENERAL = Ogre::MEMCATEGORY_GENERAL,
		MEMCATEGORY_SCENE_OBJECTS = Ogre::MEMCATEGORY_SCENE_OBJECTS,
		MEMCATEGORY_SCENE_CONTROL = Ogre::MEMCATEGORY_SCENE_CONTROL,
		MEMCATEGORY_SCRIPTING = Ogre::MEMCATEGORY_SCRIPTING
	};


	// Constants
	const String BLANK_STRING = "";

	// Typedef's for containers
	template<class T> using vector = std::vector<T>;
	template <class T> using list = std::list<T>;
	template <class T, class M> using map = std::map<T, M>;
	template <typename T> T* any_cast(Any* operand)
	{
		return Ogre::any_cast<T>(operand);
	}
	template <typename T> using Singleton = Ogre::Singleton<T>;
}

#endif
