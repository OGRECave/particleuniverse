/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EmitterPropertyWindow/LineEmitterPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
LineEmitterPropertyWindow::LineEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EmitterPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
LineEmitterPropertyWindow::LineEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) :
	EmitterPropertyWindow(emitterPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
LineEmitterPropertyWindow::LineEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::LineEmitter* emitter) :
	EmitterPropertyWindow(parent, owner, emitter->getName())
{
	copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void LineEmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::LineEmitter* lineEmitter = static_cast<ParticleUniverse::LineEmitter*>(mOwner->getPUElement());
	if (!lineEmitter)
		return;

	if (propertyName == PRNL_LINE_END + PRNL_X ||
		propertyName == PRNL_LINE_END + PRNL_Y ||
		propertyName == PRNL_LINE_END + PRNL_Z)
	{
		// Update emitter with end
		copyEndToLineEmitter(prop, lineEmitter);
	}
	else if (propertyName == PRNL_LINE_MAX_INCREMENT)
	{
		// Update emitter with Max increment
		copyMaxIncrementToLineEmitter(prop, lineEmitter);
	}
	else if (propertyName == PRNL_LINE_MIN_INCREMENT)
	{
		// Update emitter with Min increment
		copyMinIncrementToLineEmitter(prop, lineEmitter);
	}
	else if (propertyName == PRNL_LINE_DEVIATION)
	{
		// Update emitter with Deviation
		copyDeviationToLineEmitter(prop, lineEmitter);
	}
	else
	{
		// Update emitter with another attribute
		EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void LineEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

	// Copy properties from emitter to property window
	ParticleUniverse::LineEmitter* lineEmitter = static_cast<ParticleUniverse::LineEmitter*>(emitter);

	// End: Ogre::Vector3
	doSetVector3(PRNL_LINE_END, lineEmitter->getEnd());

	// Max increment: ParticleUniverse::Real
	doSetDouble(PRNL_LINE_MAX_INCREMENT, lineEmitter->getMaxIncrement());

	// Min increment: ParticleUniverse::Real
	doSetDouble(PRNL_LINE_MIN_INCREMENT, lineEmitter->getMinIncrement());

	// Deviation: ParticleUniverse::Real
	doSetDouble(PRNL_LINE_DEVIATION, lineEmitter->getMaxDeviation());
}
//-----------------------------------------------------------------------
void LineEmitterPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_LINE_END = _("Line end");
	PRNL_LINE_MAX_INCREMENT = _("Line max increment");
	PRNL_LINE_MIN_INCREMENT = _("Line min increment");
	PRNL_LINE_DEVIATION = _("Line deviation");

	mHelpHtml = wxT("EmitterLine.html");

	// End: Ogre::Vector3
	appendVector3(PRNL_LINE_END, PRNL_LINE_END, ParticleUniverse::LineEmitter::DEFAULT_END);

	// Max increment: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_LINE_MAX_INCREMENT, PRNL_LINE_MAX_INCREMENT, ParticleUniverse::LineEmitter::DEFAULT_MAX_INCREMENT));
	SetPropertyEditor(PRNL_LINE_MAX_INCREMENT, wxPG_EDITOR(SpinCtrl));

	// Min increment: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_LINE_MIN_INCREMENT, PRNL_LINE_MIN_INCREMENT, ParticleUniverse::LineEmitter::DEFAULT_MIN_INCREMENT));
	SetPropertyEditor(PRNL_LINE_MIN_INCREMENT, wxPG_EDITOR(SpinCtrl));

	// Deviation: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_LINE_DEVIATION, PRNL_LINE_DEVIATION, ParticleUniverse::LineEmitter::DEFAULT_MAX_DEVIATION));
	SetPropertyEditor(PRNL_LINE_DEVIATION, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void LineEmitterPropertyWindow::copyEndToLineEmitter(wxPGProperty* prop, ParticleUniverse::LineEmitter* lineEmitter)
{
	Ogre::Vector3 v3;
	v3 = doGetVector3(PRNL_LINE_END, v3);
	lineEmitter->setEnd(v3);
}
//-----------------------------------------------------------------------
void LineEmitterPropertyWindow::copyMaxIncrementToLineEmitter(wxPGProperty* prop, ParticleUniverse::LineEmitter* lineEmitter)
{
	lineEmitter->setMaxIncrement(prop->DoGetValue().GetDouble());
}
//-----------------------------------------------------------------------
void LineEmitterPropertyWindow::copyMinIncrementToLineEmitter(wxPGProperty* prop, ParticleUniverse::LineEmitter* lineEmitter)
{
	lineEmitter->setMinIncrement(prop->DoGetValue().GetDouble());
}
//---------------------	--------------------------------------------------
void LineEmitterPropertyWindow::copyDeviationToLineEmitter(wxPGProperty* prop, ParticleUniverse::LineEmitter* lineEmitter)
{
	lineEmitter->setMaxDeviation(prop->DoGetValue().GetDouble());
}
