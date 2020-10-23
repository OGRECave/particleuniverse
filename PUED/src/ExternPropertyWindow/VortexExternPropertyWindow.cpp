/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ExternPropertyWindow/VortexExternPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "AffectorPropertyWindow/VortexAffectorPropertyWindow.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
VortexExternPropertyWindow::VortexExternPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ExternPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
VortexExternPropertyWindow::VortexExternPropertyWindow(ExternPropertyWindow* externPropertyWindow) :
	ExternPropertyWindow(externPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
VortexExternPropertyWindow::VortexExternPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::VortexExtern* externObject) :
	ExternPropertyWindow(parent, owner, (static_cast<ParticleUniverse::Extern*>(externObject))->getName())
{
	copyAttributesFromExtern(externObject);
}
//-----------------------------------------------------------------------
void VortexExternPropertyWindow::copyAttributeToExtern(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::Extern* ext = static_cast<ParticleUniverse::Extern*>(mOwner->getPUElement());
	ParticleUniverse::VortexExtern* externObject = static_cast<ParticleUniverse::VortexExtern*>(ext);

	if (propertyName == PRNL_EXTERN_THRESHOLD)
	{
		// Distance Threshold: ParticleUniverse::Real
		ParticleUniverse::Attachable* attachable = static_cast<ParticleUniverse::Attachable*>(mOwner->getPUElement());
		attachable->setDistanceThreshold(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_ROTATION_AXIS + PRNL_X ||
		propertyName == PRNL_ROTATION_AXIS + PRNL_Y ||
		propertyName == PRNL_ROTATION_AXIS + PRNL_Z)
	{
		// Rotation Vector: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_ROTATION_AXIS, v3);
		externObject->setRotationVector(v3);
	}
	else if (propertyName.StartsWith(PRNL_ROTATION_SPEED))
	{
		// Rotation speed: DynamicAttribute
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_ROTATION_SPEED, externObject->getRotationSpeed());
		if (dynAttr)
		{
			externObject->setRotationSpeed(dynAttr);
		}
	}
	else
	{
		// Update extern with another attribute
		ExternPropertyWindow::copyAttributeToExtern(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void VortexExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern* externObject)
{
	ExternPropertyWindow::copyAttributesFromExtern(externObject);

	// Copy properties from Extern to property window
	ParticleUniverse::VortexExtern* vortexExtern = static_cast<ParticleUniverse::VortexExtern*>(externObject);

	// Distance Threshold: ParticleUniverse::Real
	doSetDouble(PRNL_EXTERN_THRESHOLD, vortexExtern->getDistanceThreshold());

	// Rotation Vector: Ogre::Vector3
	doSetVector3(PRNL_ROTATION_AXIS, vortexExtern->getRotationVector());

	// Rotation Speed: Dynamic Attribute
	doSetDynamicAttribute(PRNL_ROTATION_SPEED, vortexExtern->getRotationSpeed());
}
//-----------------------------------------------------------------------
void VortexExternPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	// TODO
	PRNL_ROTATION_SPEED = _("Rotation speed");
	PRNL_ROTATION_AXIS = _("Rotation axis");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_EXTERN_THRESHOLD = _("Distance threshold");

	mHelpHtml = wxT("ExternVortex.html");

	// Distance Threshold: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_EXTERN_THRESHOLD, PRNL_EXTERN_THRESHOLD, std::numeric_limits<float>::max()));
	SetPropertyEditor(PRNL_EXTERN_THRESHOLD, wxPG_EDITOR(SpinCtrl));

	// Rotation Vector: Ogre::Vector3
	appendVector3(PRNL_ROTATION_AXIS, PRNL_ROTATION_AXIS, ParticleUniverse::VortexAffector::DEFAULT_ROTATION_VECTOR);

	// Rotation Speed: Dynamic Attribute
	ParticleUniverse::DynamicAttributeFixed dynAttr;
	dynAttr.setValue(ParticleUniverse::VortexAffector::DEFAULT_ROTATION_SPEED);
	appendDynamicAttribute(PRNL_ROTATION_SPEED, PRNL_ROTATION_SPEED, dynAttr);
}