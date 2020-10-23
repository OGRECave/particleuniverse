/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2014 Henry van Merode. All rights reserved.

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

#ifndef __PUED_LISTENER_H__
#define __PUED_LISTENER_H__

#include "wx/ogre/prerequisites.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemListener.h"

/**	
*/
class ParticleUniverseEditorFrame;
class SystemListener : public ParticleUniverse::ParticleSystemListener
{
	public:
		// Constructor / Destructor
		SystemListener(ParticleUniverseEditorFrame* frame);
		~SystemListener(void);

		/*  See ParticleSystemListener
		*/
		virtual void handleParticleSystemEvent(ParticleUniverse::ParticleSystem* particleSystem, 
			ParticleUniverse::ParticleUniverseEvent& particleUniverseEvent);

	protected:
		ParticleUniverseEditorFrame* mFrame;

	private:
		SystemListener(void){};
};

/**	
*/
class LogListener : public Ogre::LogListener
{
	public:
		// Constructor / Destructor
		LogListener(ParticleUniverseEditorFrame* frame);
		~LogListener(void){};

		/* 
		*/
		void suppressLogging(bool suppress);

		/* 
		*/
		bool errorsLogged(void);

		/*  See Ogre::LogListener
		*/
		virtual void messageLogged (const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName, bool& skipThisMessage);

	protected:
		bool mSuppressLogging;
		bool mErrorsLogged;
		ParticleUniverseEditorFrame* mFrame;

	private:
		LogListener(void){};
};

#endif
