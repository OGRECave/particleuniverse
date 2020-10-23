/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EmitterPropertyWindow/BoxEmitterPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
BoxEmitterPropertyWindow::BoxEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EmitterPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BoxEmitterPropertyWindow::BoxEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) :
	EmitterPropertyWindow(emitterPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BoxEmitterPropertyWindow::BoxEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::BoxEmitter* emitter) :
	EmitterPropertyWindow(parent, owner, emitter->getName())
{
	copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void BoxEmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::BoxEmitter* boxEmitter = static_cast<ParticleUniverse::BoxEmitter*>(mOwner->getPUElement());
	if (!boxEmitter)
		return;

	if (propertyName == PRNL_BOX_EMITTER_WIDTH)
	{
		// Update emitter with width
		copyWidthToBoxEmitter(prop, boxEmitter);
	}
	else if (propertyName == PRNL_BOX_EMITTER_HEIGHT)
	{
		// Update emitter with height
		copyHeightToBoxEmitter(prop, boxEmitter);
	}
	else if (propertyName == PRNL_BOX_EMITTER_DEPTH)
	{
		// Update emitter with depth
		copyDepthToBoxEmitter(prop, boxEmitter);
	}
	else
	{
		// Update emitter with another attribute
		EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void BoxEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

	// Copy properties from emitter to property window
	ParticleUniverse::BoxEmitter* boxEmitter = static_cast<ParticleUniverse::BoxEmitter*>(emitter);

	// Width: ParticleUniverse::Real
	doSetDouble(PRNL_BOX_EMITTER_WIDTH, boxEmitter->getWidth());

	// Height: ParticleUniverse::Real
	doSetDouble(PRNL_BOX_EMITTER_HEIGHT, boxEmitter->getHeight());

	// Depth: ParticleUniverse::Real
	doSetDouble(PRNL_BOX_EMITTER_DEPTH, boxEmitter->getDepth());
}
//-----------------------------------------------------------------------
void BoxEmitterPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_BOX_EMITTER_WIDTH = _("Box width");
	PRNL_BOX_EMITTER_HEIGHT = _("Box height");
	PRNL_BOX_EMITTER_DEPTH = _("Box depth");

	mHelpHtml = wxT("EmitterBox.html");

	// Width: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_BOX_EMITTER_WIDTH, PRNL_BOX_EMITTER_WIDTH, ParticleUniverse::BoxEmitter::DEFAULT_WIDTH));
	SetPropertyEditor(PRNL_BOX_EMITTER_WIDTH, wxPG_EDITOR(SpinCtrl));

	// Height: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_BOX_EMITTER_HEIGHT, PRNL_BOX_EMITTER_HEIGHT, ParticleUniverse::BoxEmitter::DEFAULT_HEIGHT));
	SetPropertyEditor(PRNL_BOX_EMITTER_HEIGHT, wxPG_EDITOR(SpinCtrl));

	// Depth: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_BOX_EMITTER_DEPTH, PRNL_BOX_EMITTER_DEPTH, ParticleUniverse::BoxEmitter::DEFAULT_DEPTH));
	SetPropertyEditor(PRNL_BOX_EMITTER_DEPTH, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void BoxEmitterPropertyWindow::copyWidthToBoxEmitter(wxPGProperty* prop, ParticleUniverse::BoxEmitter* boxEmitter)
{
	boxEmitter->setWidth(prop->DoGetValue().GetDouble());
}
//-----------------------------------------------------------------------
void BoxEmitterPropertyWindow::copyHeightToBoxEmitter(wxPGProperty* prop, ParticleUniverse::BoxEmitter* boxEmitter)
{
	boxEmitter->setHeight(prop->DoGetValue().GetDouble());
}
//-----------------------------------------------------------------------
void BoxEmitterPropertyWindow::copyDepthToBoxEmitter(wxPGProperty* prop, ParticleUniverse::BoxEmitter* boxEmitter)
{
	boxEmitter->setDepth(prop->DoGetValue().GetDouble());
}

