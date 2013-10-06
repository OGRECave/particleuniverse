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

#ifndef __PU_HOOK_H__
#define __PU_HOOK_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse
{
	/** The Hook class is a template helper class that is used to call the hook functions of objects (Affectors, 
		Externs, ...) registered at the ParticleSystem. This prevents that the same code is duplicated 
		too much.
    */
	template <typename T>
	class _ParticleUniverseExport Hook
	{
		public:

			typedef vector<T*> HookList;

			/** Templated version to prepare objects.
			*/
			static inline void _prepare(HookList* list, ParticleTechnique* technique)
			{
				if (list->empty())
					return;

				typename HookList::iterator it;
				typename HookList::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_prepare(technique);
				}
			}

			/** Templated version to unprepare objects.
			*/
			static inline void _unprepare(HookList* list, ParticleTechnique* technique)
			{
				if (list->empty())
					return;

				typename HookList::iterator it;
				typename HookList::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_unprepare(technique);
				}
			}

			/** Templated version to notify objects that the ParticleSystem starts
			*/
			static inline void _notifyStart(HookList* list)
			{
				if (list->empty())
					return;

				typename HookList::iterator it;
				typename HookList::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_notifyStart();
				}
			}

			/** Templated version to notify objects that the ParticleSystem pauses
			*/
			static inline void _notifyPause(HookList* list)
			{
				if (list->empty())
					return;

				typename HookList::iterator it;
				typename HookList::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_notifyPause();
				}
			}

			/** Templated version to notify objects that the ParticleSystem resumes
			*/
			static inline void _notifyResume(HookList* list)
			{
				if (list->empty())
					return;

				typename HookList::iterator it;
				typename HookList::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_notifyResume();
				}
			}

			/** Templated version to notify objects that the ParticleSystem stops
			*/
			static inline void _notifyStop(HookList* list)
			{
				if (list->empty())
					return;

				typename HookList::iterator it;
				typename HookList::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_notifyStop();
				}
			}

			/** Templated version of _preProcessParticles.
			@remarks
				_preProcessParticles allows actions before the individual particles are processed.
			*/
			static inline void _preProcessParticles(HookList* list, 
				ParticleTechnique* technique, 
				Real timeElapsed)
			{
				if (list->empty())
					return;

				typename HookList::iterator it;
				typename HookList::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_preProcessParticles(technique, timeElapsed);
				}
			}

			/** Templated version to process a particle.
			*/
			static inline void _processParticle(HookList* list, 
				ParticleTechnique* particleTechnique, 
				Particle* particle, 
				Real timeElapsed, 
				bool firstParticle)
			{
				if (list->empty())
					return;

				typename HookList::iterator it;
				typename HookList::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_processParticle(particleTechnique, particle, timeElapsed, firstParticle);
				}
			}

			/** Templated version of _postProcessParticles.
			@remarks
				_postProcessParticles allows actions after the individual particles are processed.
			*/
			static inline void _postProcessParticles(HookList* list, 
				ParticleTechnique* technique, 
				Real timeElapsed)
			{
				if (list->empty())
					return;

				typename HookList::iterator it;
				typename HookList::iterator itEnd = list->end();
				for (it = list->begin(); it != itEnd; ++it)
				{
					(*it)->_postProcessParticles(technique, timeElapsed);
				}
			}
	};

}
#endif
