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

#ifndef __PU_DO_AFFECTOR_EVENT_HANDLER_H__
#define __PU_DO_AFFECTOR_EVENT_HANDLER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseEventHandler.h"

namespace ParticleUniverse
{
	/** This class explicitly calls an affector to affect the particles. There are several reasons why
		this is appropriate. One reason is, that you only want to affect a particle if a certain event
		occurs. Disable an affector and call it using this event handler, is the method (calling the affector
		from the event handler doesn't take into consideration that the affector is disabled).
    */
	class _ParticleUniverseExport DoAffectorEventHandler : public ParticleEventHandler
	{
		protected:
			// Identifies the name of affector
			String mAffectorName;

			// Determines whether the pre- and post processing activities must be executed also
			bool mPrePost;

		public:
			// Constants
			static const bool DEFAULT_PRE_POST;

			DoAffectorEventHandler(void);
	        virtual ~DoAffectorEventHandler(void) {};

			/** Get the indication whether pre- and postprocessing must be done.
	        */
			const bool getPrePost(void) const {return mPrePost;};

			/** Set the indication whether pre- and postprocessing must be done.
	        */
			void setPrePost(const bool prePost){mPrePost = prePost;};

			/** Get the name of the affector that must be enabled or disabled.
	        */
			const String& getAffectorName(void) const {return mAffectorName;};

			/** Set the name of the affector.
	        */
			void setAffectorName(const String& affectorName){mAffectorName = affectorName;};

			/** If the _handle() function of this class is invoked (by an Observer), it searches the 
				ParticleAffector defined by the its name. 
				The ParticleAffector is either part of the ParticleTechnique in which the 
				DoAffectorEventHandler is defined, or if the Affector is not found, other 
				ParticleTechniques are searched.
	        */
			virtual void _handle (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** Copy attributes to another event handler.
	        */
			virtual void copyAttributesTo (ParticleEventHandler* eventHandler);
	};

}
#endif
