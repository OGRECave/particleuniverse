/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EmitterPropertyWindow/CircleEmitterPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
CircleEmitterPropertyWindow::CircleEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EmitterPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
CircleEmitterPropertyWindow::CircleEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) :
	EmitterPropertyWindow(emitterPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
CircleEmitterPropertyWindow::CircleEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::CircleEmitter* emitter) :
	EmitterPropertyWindow(parent, owner, emitter->getName())
{
	copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void CircleEmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::CircleEmitter* circleEmitter = static_cast<ParticleUniverse::CircleEmitter*>(mOwner->getPUElement());
	if (!circleEmitter)
		return;

	if (propertyName == PRNL_CIRCLE_EMITTER_ANGLE)
	{
		// Update emitter with angle
		copyAngleToCircleEmitter(prop, circleEmitter);
	}
	else if (propertyName == PRNL_CIRCLE_EMITTER_NORMAL + PRNL_X ||
		propertyName == PRNL_CIRCLE_EMITTER_NORMAL + PRNL_Y ||
		propertyName == PRNL_CIRCLE_EMITTER_NORMAL + PRNL_Z)
	{
		// Update emitter with normal
		copyNormalToCircleEmitter(prop, circleEmitter);
	}
	else if (propertyName == PRNL_CIRCLE_EMITTER_RADIUS)
	{
		// Update emitter with radius
		copyRadiusToCircleEmitter(prop, circleEmitter);
	}
	else if (propertyName == PRNL_CIRCLE_EMITTER_RANDOM)
	{
		// Update emitter with random
		copyRandomToCircleEmitter(prop, circleEmitter);
	}
	else if (propertyName == PRNL_CIRCLE_EMITTER_STEP)
	{
		// Update emitter with step
		copyStepToCircleEmitter(prop, circleEmitter);
	}
	else
	{
		// Update emitter with another attribute
		EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void CircleEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

	// Copy properties from emitter to property window
	ParticleUniverse::CircleEmitter* circleEmitter = static_cast<ParticleUniverse::CircleEmitter*>(emitter);

	// Radius: ParticleUniverse::Real
	doSetDouble(PRNL_CIRCLE_EMITTER_RADIUS, circleEmitter->getRadius());

	// Step: ParticleUniverse::Real
	doSetDouble(PRNL_CIRCLE_EMITTER_STEP, circleEmitter->getStep());

	// Angle: ParticleUniverse::Real
	doSetDouble(PRNL_CIRCLE_EMITTER_ANGLE, circleEmitter->getCircleAngle());

	// Random: bool
	doSetBool(PRNL_CIRCLE_EMITTER_RANDOM, circleEmitter->isRandom());

	// Normal: Vector3
	doSetVector3(PRNL_CIRCLE_EMITTER_NORMAL, circleEmitter->getNormal());
}
//-----------------------------------------------------------------------
void CircleEmitterPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_CIRCLE_EMITTER_RADIUS = _("Circle Radius");
	PRNL_CIRCLE_EMITTER_STEP = _("Circle Step");
	PRNL_CIRCLE_EMITTER_ANGLE = _("Circle angle");
	PRNL_CIRCLE_EMITTER_RANDOM = _("Circle Random");
	PRNL_CIRCLE_EMITTER_NORMAL = _("Circle normal");

	mHelpHtml = wxT("EmitterCircle.html");

	// Radius: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_CIRCLE_EMITTER_RADIUS, PRNL_CIRCLE_EMITTER_RADIUS, ParticleUniverse::CircleEmitter::DEFAULT_RADIUS));
	SetPropertyEditor(PRNL_CIRCLE_EMITTER_RADIUS, wxPG_EDITOR(SpinCtrl));

	// Step: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_CIRCLE_EMITTER_STEP, PRNL_CIRCLE_EMITTER_STEP, ParticleUniverse::CircleEmitter::DEFAULT_STEP));
	SetPropertyEditor(PRNL_CIRCLE_EMITTER_STEP, wxPG_EDITOR(SpinCtrl));

	// Angle: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_CIRCLE_EMITTER_ANGLE, PRNL_CIRCLE_EMITTER_ANGLE, ParticleUniverse::CircleEmitter::DEFAULT_ANGLE));
	SetPropertyEditor(PRNL_CIRCLE_EMITTER_ANGLE, wxPG_EDITOR(SpinCtrl));

	// Random: bool
	Append(wxBoolProperty(PRNL_CIRCLE_EMITTER_RANDOM, PRNL_CIRCLE_EMITTER_RANDOM, ParticleUniverse::CircleEmitter::DEFAULT_RANDOM));

	// Normal: Vector3
	appendVector3(PRNL_CIRCLE_EMITTER_NORMAL, PRNL_CIRCLE_EMITTER_NORMAL, ParticleUniverse::CircleEmitter::DEFAULT_NORMAL);
}
//-----------------------------------------------------------------------
void CircleEmitterPropertyWindow::copyAngleToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter)
{
	circleEmitter->setCircleAngle(prop->DoGetValue().GetDouble());
}
//-----------------------------------------------------------------------
void CircleEmitterPropertyWindow::copyNormalToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter)
{
	//wxString name = prop->GetParent()->GetName();
	Ogre::Vector3 v3;
	//v3 = doGetVector3(name, v3);
	v3 = doGetVector3(PRNL_CIRCLE_EMITTER_NORMAL, v3);
	circleEmitter->setNormal(v3);
}
//-----------------------------------------------------------------------
void CircleEmitterPropertyWindow::copyRadiusToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter)
{
	circleEmitter->setRadius(prop->DoGetValue().GetDouble());
}
//-----------------------------------------------------------------------
void CircleEmitterPropertyWindow::copyRandomToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter)
{
	circleEmitter->setRandom(prop->DoGetValue().GetBool());
}
//-----------------------------------------------------------------------
void CircleEmitterPropertyWindow::copyStepToCircleEmitter(wxPGProperty* prop, ParticleUniverse::CircleEmitter* circleEmitter)
{
	circleEmitter->setStep(prop->DoGetValue().GetDouble());
}
