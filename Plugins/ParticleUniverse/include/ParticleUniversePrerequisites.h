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

#ifndef __PU_PREREQUISITES_H__
#define __PU_PREREQUISITES_H__

/** If PU_RENDERER_OGRE is defined, Ogre is used as the primary renderer.
	This is to abstract the renderer from the particle plugin. This is work in progress.
*/
#define PU_RENDERER_OGRE 1

#include "ParticleUniverseTypes.h"
#include "ParticleUniverseAlloc.h"

#if (PU_RENDERER_OGRE)
	#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32) && !defined(OGRE_STATIC_LIB)
	#   ifdef PARTICLE_UNIVERSE_EXPORTS
	#       define _ParticleUniverseExport __declspec(dllexport)
	#   else
	#       if defined( __MINGW32__ )
	#           define _ParticleUniverseExport
	#       else
	#    		define _ParticleUniverseExport __declspec(dllimport)
	#       endif
	#   endif
	#elif defined ( OGRE_GCC_VISIBILITY )
	#    define _ParticleUniverseExport  __attribute__ ((visibility("default")))
	#else
	#   define _ParticleUniverseExport
	#endif
#else
	#ifdef PARTICLE_UNIVERSE_EXPORTS
	#   define _ParticleUniverseExport __declspec(dllexport)
	#else
	#   if defined( __MINGW32__ )
	#       define _ParticleUniverseExport
	#   else
	# 		define _ParticleUniverseExport __declspec(dllimport)
	#   endif
	#endif
#endif


/** If PU_PHYSICS is defined, a physics engine is used. Uncomment the line if a physics engine is used. 
	Comment it if you don't want to use a physics engine.
*/
//#define PU_PHYSICS

/** If the PhysX engine of NVidia is NOT used, comment the line "#define PU_PHYSICS_PHYSX", so it skips compilation of the PhysX-specific
	code and linking of PhysXLoader.lib. Of course you have to provide the code to include another physics engine yourself.
*/
#ifdef PU_PHYSICS
	#define PU_PHYSICS_PHYSX
#endif // PU_PHYSICS


/** If you want to log additional debug info, uncomment the line below (PU_LOG_DEBUG). Comment it if you don't want to use debug logging.
*/
//#define PU_LOG_DEBUG


/** If REPLACE_TEMPLATE_IF_EXISTING is defined and you want to create a new particle system template with a name that already exists
	(using ParticleSystemManager::createParticleSystemTemplate()), the old template is deleted first (beware of pointers to the old template).
	Comment it if you don't want that. By commenting it, a new particle system template is created with the name prefixed with "CopyOf". 
*/
#define REPLACE_TEMPLATE_IF_EXISTING


// 4217 - reference to an imported symbol that has been defined locally
// 4251 - class 'std::list<T*>' needs to have dll-interface to be used by clients of class
// 4800 - 'unsigned int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning (disable : 4217 4251 4800)

namespace ParticleUniverse
{
	class ParticleSystem;
	class ParticleTechnique;
	class ParticleEmitter;
	class ParticleAffector;
	class ParticleObserver;
	class Particle;
	class VisualParticle;
	class Extern;
	class IDependency;
	class IToken;
	class BoxColliderExternFactory;
}

#endif
