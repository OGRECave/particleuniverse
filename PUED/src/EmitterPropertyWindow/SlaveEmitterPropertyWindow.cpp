/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EmitterPropertyWindow/SlaveEmitterPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
SlaveEmitterPropertyWindow::SlaveEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EmitterPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SlaveEmitterPropertyWindow::SlaveEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) :
	EmitterPropertyWindow(emitterPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SlaveEmitterPropertyWindow::SlaveEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::SlaveEmitter* emitter) :
	EmitterPropertyWindow(parent, owner, emitter->getName())
{
	copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void SlaveEmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	// Update emitter with another attribute
	EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
}
//-----------------------------------------------------------------------
void SlaveEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	// Nothing yet
	EmitterPropertyWindow::copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void SlaveEmitterPropertyWindow::_initProperties(void)
{
	mHelpHtml = wxT("EmitterSlave.html");

	// Nothing yet
}