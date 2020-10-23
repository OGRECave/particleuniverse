/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EmitterPropertyWindow/PointEmitterPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
PointEmitterPropertyWindow::PointEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EmitterPropertyWindow(parent, owner, name)
{
	mHelpHtml = wxT("EmitterPoint.html");
}
//-----------------------------------------------------------------------
PointEmitterPropertyWindow::PointEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) :
	EmitterPropertyWindow(emitterPropertyWindow)
{
	mHelpHtml = wxT("EmitterPoint.html");
}
//-----------------------------------------------------------------------
PointEmitterPropertyWindow::PointEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::PointEmitter* emitter) :
	EmitterPropertyWindow(parent, owner, emitter->getName())
{
	mHelpHtml = wxT("EmitterPoint.html");
	copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void PointEmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	// Update emitter with another attribute
	EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
}
//-----------------------------------------------------------------------
void PointEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	// No additional properties
	EmitterPropertyWindow::copyAttributesFromEmitter(emitter);
}
