/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/ForceFieldAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
ForceFieldAffectorPropertyWindow::ForceFieldAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ForceFieldAffectorPropertyWindow::ForceFieldAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ForceFieldAffectorPropertyWindow::ForceFieldAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::ForceFieldAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void ForceFieldAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ForceFieldAffector* affector = static_cast<ParticleUniverse::ForceFieldAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_AFFECTOR_FORCEFIELD_FF_TYPE)
	{
		// forcefield_type: List
		wxString type = prop->GetValueAsString();
		if (type == FORCEFIELD_REALTIME)
		{
			affector->setForceFieldType(ParticleUniverse::ForceField::FF_REALTIME_CALC);
		}
		else if (type == FORCEFIELD_MATRIX)
		{
			affector->setForceFieldType(ParticleUniverse::ForceField::FF_MATRIX_CALC);
		}
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_DELTA)
	{
		// delta: Real
		affector->setDelta(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_FORCE)
	{
		// force: Real
		affector->setScaleForce(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_OCTAVES)
	{
		// octaves: uint
		affector->setOctaves(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_FREQUENCY)
	{
		// frequency: Real
		affector->setFrequency(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE)
	{
		// amplitude: Real
		affector->setAmplitude(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE)
	{
		// persistence: Real
		affector->setPersistence(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_FF_SIZE)
	{
		// forcefield_size: uint
		affector->setForceFieldSize(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE + PRNL_X ||
		propertyName == PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE + PRNL_Y ||
		propertyName == PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE + PRNL_Z)
	{
		// worldsize: Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE, v3);
		affector->setWorldSize(v3);
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_IGNORE_X)
	{
		// ignore_negative_x: bool
		affector->setIgnoreNegativeX(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y)
	{
		// ignore_negative_y: bool
		affector->setIgnoreNegativeY(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z)
	{
		// ignore_negative_z: bool
		affector->setIgnoreNegativeZ(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_MOVEMENT + PRNL_X ||
		propertyName == PRNL_AFFECTOR_FORCEFIELD_MOVEMENT + PRNL_Y ||
		propertyName == PRNL_AFFECTOR_FORCEFIELD_MOVEMENT + PRNL_Z)
	{
		// movement: Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_AFFECTOR_FORCEFIELD_MOVEMENT, v3);
		affector->setMovement(v3);
	}
	else if (propertyName == PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY)
	{
		// movement_frequency: Real
		affector->setMovementFrequency(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void ForceFieldAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::ForceFieldAffector* forceFieldAffector = static_cast<ParticleUniverse::ForceFieldAffector*>(affector);

	// forcefield_type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_AFFECTOR_FORCEFIELD_FF_TYPE);
	ParticleUniverse::ForceField::ForceFieldType type = forceFieldAffector->getForceFieldType();
	wxString typeString = FORCEFIELD_REALTIME;
	if (type == ParticleUniverse::ForceField::FF_MATRIX_CALC)
	{
		typeString = FORCEFIELD_MATRIX;
	}
	propTo->SetValueFromString(typeString);

	// delta: Real
	doSetDouble(PRNL_AFFECTOR_FORCEFIELD_DELTA, forceFieldAffector->getDelta());

	// force: Real
	doSetDouble(PRNL_AFFECTOR_FORCEFIELD_FORCE, forceFieldAffector->getScaleForce());

	// octaves: uint
	doSetUint16(PRNL_AFFECTOR_FORCEFIELD_OCTAVES, forceFieldAffector->getOctaves());

	// frequency: Real
	doSetDouble(PRNL_AFFECTOR_FORCEFIELD_FREQUENCY, forceFieldAffector->getFrequency());

	// amplitude: Real
	doSetDouble(PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE, forceFieldAffector->getAmplitude());

	// persistence: Real
	doSetDouble(PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE, forceFieldAffector->getPersistence());

	// forcefield_size: uint
	doSetUint16(PRNL_AFFECTOR_FORCEFIELD_FF_SIZE, forceFieldAffector->getForceFieldSize());

	// worldsize: Vector3
	doSetVector3(PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE, forceFieldAffector->getWorldSize());

	// ignore_negative_x: bool
	doSetBool(PRNL_AFFECTOR_FORCEFIELD_IGNORE_X, forceFieldAffector->getIgnoreNegativeX());

	// ignore_negative_y: bool
	doSetBool(PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y, forceFieldAffector->getIgnoreNegativeY());

	// ignore_negative_z: bool
	doSetBool(PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z, forceFieldAffector->getIgnoreNegativeZ());

	// movement: Vector3
	doSetVector3(PRNL_AFFECTOR_FORCEFIELD_MOVEMENT, forceFieldAffector->getMovement());

	// movement_frequency: Real
	doSetDouble(PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY, forceFieldAffector->getMovementFrequency());
}
//-----------------------------------------------------------------------
void ForceFieldAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_AFFECTOR_FORCEFIELD_FF_TYPE = _("Forcefield type");
	FORCEFIELD_REALTIME = _("Realtime");
	FORCEFIELD_MATRIX = _("Matrix");
	PRNL_AFFECTOR_FORCEFIELD_DELTA = _("Delta");
	PRNL_AFFECTOR_FORCEFIELD_FORCE = _("Force");
	PRNL_AFFECTOR_FORCEFIELD_OCTAVES = _("Octaves");
	PRNL_AFFECTOR_FORCEFIELD_FREQUENCY = _("Frequency");
	PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE = _("Amplitude");
	PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE = _("Persistence");
	PRNL_AFFECTOR_FORCEFIELD_FF_SIZE = _("Forcefield size");
	PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE = _("Worldsize");
	PRNL_AFFECTOR_FORCEFIELD_IGNORE_X = _("Ignore -x");
	PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y = _("Ignore -y");
	PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z = _("Ignore -z");
	PRNL_AFFECTOR_FORCEFIELD_MOVEMENT = _("Movement");
	PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY = _("Movement frequency");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");

	mHelpHtml = wxT("AffectorForceField.html");

	// forcefield_type: List
	wxArrayString type;
	type.Add(FORCEFIELD_REALTIME);
	type.Add(FORCEFIELD_MATRIX);
	Append(wxEnumProperty(PRNL_AFFECTOR_FORCEFIELD_FF_TYPE, PRNL_AFFECTOR_FORCEFIELD_FF_TYPE, type));

	// delta: Real
	Append(wxFloatProperty(PRNL_AFFECTOR_FORCEFIELD_DELTA, PRNL_AFFECTOR_FORCEFIELD_DELTA, ParticleUniverse::ForceFieldAffector::DEFAULT_DELTA));
	SetPropertyEditor(PRNL_AFFECTOR_FORCEFIELD_DELTA, wxPG_EDITOR(SpinCtrl));

	// force: Real
	Append(wxFloatProperty(PRNL_AFFECTOR_FORCEFIELD_FORCE, PRNL_AFFECTOR_FORCEFIELD_FORCE, ParticleUniverse::ForceFieldAffector::DEFAULT_FORCE));
	SetPropertyEditor(PRNL_AFFECTOR_FORCEFIELD_FORCE, wxPG_EDITOR(SpinCtrl));
	
	// octaves: uint
	Append(wxUIntProperty(PRNL_AFFECTOR_FORCEFIELD_OCTAVES, PRNL_AFFECTOR_FORCEFIELD_OCTAVES, ParticleUniverse::ForceFieldAffector::DEFAULT_OCTAVES));
	SetPropertyEditor(PRNL_AFFECTOR_FORCEFIELD_OCTAVES, wxPG_EDITOR(SpinCtrl));
	
	// frequency: Real
	Append(wxFloatProperty(PRNL_AFFECTOR_FORCEFIELD_FREQUENCY, PRNL_AFFECTOR_FORCEFIELD_FREQUENCY, ParticleUniverse::ForceFieldAffector::DEFAULT_FREQUENCY));
	SetPropertyEditor(PRNL_AFFECTOR_FORCEFIELD_FREQUENCY, wxPG_EDITOR(SpinCtrl));
	
	// amplitude: Real
	Append(wxFloatProperty(PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE, PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE, ParticleUniverse::ForceFieldAffector::DEFAULT_AMPLITUDE));
	SetPropertyEditor(PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE, wxPG_EDITOR(SpinCtrl));
	
	// persistence: Real
	Append(wxFloatProperty(PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE, PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE, ParticleUniverse::ForceFieldAffector::DEFAULT_PERSISTENCE));
	SetPropertyEditor(PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE, wxPG_EDITOR(SpinCtrl));
	
	// forcefield_size: uint
	Append(wxUIntProperty(PRNL_AFFECTOR_FORCEFIELD_FF_SIZE, PRNL_AFFECTOR_FORCEFIELD_FF_SIZE, ParticleUniverse::ForceFieldAffector::DEFAULT_FORCEFIELDSIZE));
	SetPropertyEditor(PRNL_AFFECTOR_FORCEFIELD_FF_SIZE, wxPG_EDITOR(SpinCtrl));

	// worldsize: Vector3
	appendVector3(PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE, PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE, ParticleUniverse::ForceFieldAffector::DEFAULT_WORLDSIZE);
	
	// ignore_negative_x: bool
	Append(wxBoolProperty(PRNL_AFFECTOR_FORCEFIELD_IGNORE_X, PRNL_AFFECTOR_FORCEFIELD_IGNORE_X, false));

	// ignore_negative_y: bool
	Append(wxBoolProperty(PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y, PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y, false));

	// ignore_negative_z: bool
	Append(wxBoolProperty(PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z, PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z, false));
	
	// movement: Vector3
	appendVector3(PRNL_AFFECTOR_FORCEFIELD_MOVEMENT, PRNL_AFFECTOR_FORCEFIELD_MOVEMENT, ParticleUniverse::ForceFieldAffector::DEFAULT_MOVEMENT);
	
	// movement_frequency: Real
	Append(wxFloatProperty(PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY, PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY, ParticleUniverse::ForceFieldAffector::DEFAULT_MOVEMENT_FREQUENCY));
	SetPropertyEditor(PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY, wxPG_EDITOR(SpinCtrl));
}