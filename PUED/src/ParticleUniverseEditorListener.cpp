/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseEditor.h"
#include "ParticleUniverseEditorListener.h"
#include "ParticleUniverseCommon.h"

//-----------------------------------------------------------------------
SystemListener::SystemListener(ParticleUniverseEditorFrame* frame) : 
	ParticleSystemListener(),
	mFrame(frame)
{
}
//-----------------------------------------------------------------------
SystemListener::~SystemListener(void)
{
}
//-----------------------------------------------------------------------
void SystemListener::handleParticleSystemEvent(ParticleUniverse::ParticleSystem* particleSystem, 
	ParticleUniverse::ParticleUniverseEvent& particleUniverseEvent)
{
	if (particleUniverseEvent.eventType == ParticleUniverse::PU_EVT_SYSTEM_STOPPED)
	{
		// Info the frame that the particle system was stopped
		mFrame->particleSystemStopped();
	}
}
//-----------------------------------------------------------------------
LogListener::LogListener(ParticleUniverseEditorFrame* frame) : 
	Ogre::LogListener(),
	mSuppressLogging(true),
	mErrorsLogged(false),
	mFrame(frame)
{
}
//-----------------------------------------------------------------------
void LogListener::suppressLogging(bool suppress)
{
	mSuppressLogging = suppress;
	mErrorsLogged = false;
}
//-----------------------------------------------------------------------
bool LogListener::errorsLogged(void)
{
	return mErrorsLogged;
}
//-----------------------------------------------------------------------
void LogListener::messageLogged (const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName, bool& skipThisMessage)
{
	if (mSuppressLogging)
		return;

	// Every logmessage is routed to this callback function. Scan it for compiler errors!
	if (message.find("PU Compiler") != Ogre::String::npos || message.find("error") != Ogre::String::npos)
	{
		wxString wxMessage = ogre2wxTranslate(message);
		mFrame->showMessage(wxMessage, wxOK | wxICON_ERROR);
		mErrorsLogged = true;
	}
}
