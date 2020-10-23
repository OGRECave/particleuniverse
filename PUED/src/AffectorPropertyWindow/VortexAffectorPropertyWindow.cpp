/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/VortexAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
VortexAffectorPropertyWindow::VortexAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
VortexAffectorPropertyWindow::VortexAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
VortexAffectorPropertyWindow::VortexAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::VortexAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void VortexAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::VortexAffector* affector = static_cast<ParticleUniverse::VortexAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_ROTATION_AXIS + PRNL_X ||
		propertyName == PRNL_ROTATION_AXIS + PRNL_Y ||
		propertyName == PRNL_ROTATION_AXIS + PRNL_Z)
	{
		// Rotation Vector: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_ROTATION_AXIS, v3);
		affector->setRotationVector(v3);
	}
	else if (propertyName.StartsWith(PRNL_ROTATION_SPEED))
	{
		// Rotation speed: DynamicAttribute
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_ROTATION_SPEED, affector->getRotationSpeed());
		if (dynAttr)
		{
			affector->setRotationSpeed(dynAttr);
		}

		if (affector->_isMarkedForEmission())
		{
			_unprepare(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
		}
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void VortexAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::VortexAffector* vortexAffector = static_cast<ParticleUniverse::VortexAffector*>(affector);

	// Rotation Vector: Ogre::Vector3
	doSetVector3(PRNL_ROTATION_AXIS, vortexAffector->getRotationVector());

	// Rotation Speed: Dynamic Attribute
	doSetDynamicAttribute(PRNL_ROTATION_SPEED, vortexAffector->getRotationSpeed());
}
//-----------------------------------------------------------------------
void VortexAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_ROTATION = _("Rotation");
	PRNL_ROTATION_AXIS = _("Rotation axis");
	PRNL_ROTATION_SPEED = _("Rotation speed");

	mHelpHtml = wxT("AffectorVortex.html");

	// Rotation Vector: Ogre::Vector3
	appendVector3(PRNL_ROTATION_AXIS, PRNL_ROTATION_AXIS, ParticleUniverse::VortexAffector::DEFAULT_ROTATION_VECTOR);

	// Rotation Speed: Dynamic Attribute
	ParticleUniverse::DynamicAttributeFixed dynAttr;
	dynAttr.setValue(ParticleUniverse::VortexAffector::DEFAULT_ROTATION_SPEED);
	appendDynamicAttribute(PRNL_ROTATION_SPEED, PRNL_ROTATION_SPEED, dynAttr);
}