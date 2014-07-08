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

#ifndef __PU_EVENT_HANDLER_H__
#define __PU_EVENT_HANDLER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseIAlias.h"
#include "ParticleUniverseIElement.h"

namespace ParticleUniverse
{
	/** A ParticleEventHandlers is used to perform a task in case a certain event happens.
	@remarks
		A ParticleEventHandler is associated with a ParticleObserver; The ParticleObserver signals what event occurs,
		while the ParticleEventHandler performs some action.
	*/
	class _ParticleUniverseExport ParticleEventHandler : public IAlias, public IElement
	{
		protected:
			// Observer to which the eventhandler is associated.
			ParticleObserver* mParentObserver;

			// Type of event handler
			String mEventHandlerType;

			// Name of the eventHandler (optional)
			String mName;

			/** Although the scale is on a Particle System level, the event handler can also be scaled.
			*/
			Vector3 _mEventHandlerScale;

		public:
			ParticleEventHandler(void) : 
				IElement(),
				mParentObserver(0),
				mName(BLANK_STRING),
				_mEventHandlerScale(Vector3::UNIT_SCALE)
			{
				mAliasType = AT_HANDLER;
			};
			virtual ~ParticleEventHandler(void) {};

			/** Todo
	        */
			const String& getName(void) const {return mName;};
			void setName(const String& name) {mName = name;};

			/** Todo
	        */
			ParticleObserver* getParentObserver(void) const {return mParentObserver;};
			void setParentObserver(ParticleObserver* parentObserver) {mParentObserver = parentObserver;};

			/** Todo
	        */
			const String& getEventHandlerType(void) const {return mEventHandlerType;};
			void setEventHandlerType(const String& eventHandlerType) {mEventHandlerType = eventHandlerType;};

			/** Notify that the event handler is rescaled.
	        */
			virtual void _notifyRescaled(const Vector3& scale){_mEventHandlerScale = scale;};

			/** Todo
	        */
			virtual void _handle(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed) = 0;

			/** Copy attributes to another event handler.
	        */
			virtual void copyAttributesTo (ParticleEventHandler* eventHandler)
			{
				copyParentAttributesTo(eventHandler);
			};

			/** Copy parent attributes to another event handler.
	        */
			virtual void copyParentAttributesTo (ParticleEventHandler* eventHandler)
			{
				eventHandler->setName(mName);
				eventHandler->mParentObserver = mParentObserver;
				eventHandler->_mEventHandlerScale = _mEventHandlerScale;
			};
	};

}
#endif
