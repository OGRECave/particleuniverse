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

#ifndef __PU_DO_ENABLE_COMPONENT_EVENT_HANDLER_H__
#define __PU_DO_ENABLE_COMPONENT_EVENT_HANDLER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseEventHandler.h"
#include "ParticleUniverseSystem.h"

namespace ParticleUniverse
{
	/** This class makes it possible to enable or disable a Component. This component is an 
		named emitter, affector or technique.
    */
	class _ParticleUniverseExport DoEnableComponentEventHandler : public ParticleEventHandler
	{
		protected:
			// Identifies the name of component
			String mComponentName;

			// Identifies the type of component
			ComponentType mComponentType;

			/** Determines whether the Component must be enabled or disabled.
	        */
			bool mComponentEnabled;

		public:
			DoEnableComponentEventHandler(void);
	        virtual ~DoEnableComponentEventHandler(void) {};

			/** Get the name of the component that must be enabled or disabled.
	        */
			const String& getComponentName(void) const {return mComponentName;};

			/** Set the name of the component that must be enabled or disables.
	        */
			void setComponentName(const String& componentName){mComponentName = componentName;};

			/** Get the value that identifies whether the component must be enabled or disabled.
	        */
			bool isComponentEnabled(void) const {return mComponentEnabled;};

			/** Set the value that identifies whether the component must be enabled or disabled.
	        */
			void setComponentEnabled(bool enabled){mComponentEnabled = enabled;};

			/** Get the value that identifies whether the component must be enabled or disabled.
	        */
			ComponentType getComponentType(void) const {return mComponentType;};

			/** Set the value that identifies whether the component must be enabled or disabled.
	        */
			void setComponentType(ComponentType componentType){mComponentType = componentType;};

			/** If the _handle() function of this class is invoked (by an Observer), it searches the 
				ParticleEmitter, ParticleAffector or ParticleTechnique defined by the its name. 
				The ParticleEmitter/Affector is either part of the ParticleTechnique in which the 
				DoEnableComponentEventHandler is defined, or if the ParticleEmitter/Affector is not 
				found, other ParticleTechniques are searched.
	        */
			virtual void _handle (ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** Copy attributes to another event handler.
	        */
			virtual void copyAttributesTo (ParticleEventHandler* eventHandler);
	};

}
#endif
