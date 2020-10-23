/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/TextureRotatorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
TextureRotatorPropertyWindow::TextureRotatorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
TextureRotatorPropertyWindow::TextureRotatorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
TextureRotatorPropertyWindow::TextureRotatorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::TextureRotator* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void TextureRotatorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::TextureRotator* affector = static_cast<ParticleUniverse::TextureRotator*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_USE_OWN_SPEED)
	{
		// Use own rotation speed: bool
		affector->setUseOwnRotationSpeed(prop->DoGetValue().GetBool());
	}
	else if (propertyName.StartsWith(PRNL_ROTATION_SPEED))
	{
		// Rotation Speed: Dynamic Attribute (Must be put before PRNL_ROTATION)
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
	else if (propertyName.StartsWith(PRNL_ROTATION))
	{
		// Rotation: Dynamic Attribute
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_ROTATION, affector->getRotation());
		if (dynAttr)
		{
			affector->setRotation(dynAttr);
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
void TextureRotatorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::TextureRotator* textureRotator = static_cast<ParticleUniverse::TextureRotator*>(affector);

	// Use Own Rotation speed
	doSetBool(PRNL_USE_OWN_SPEED, textureRotator->useOwnRotationSpeed());

	// Rotation: Dynamic Attribute
	doSetDynamicAttribute(PRNL_ROTATION, textureRotator->getRotation());

	// Rotation Speed: Dynamic Attribute
	doSetDynamicAttribute(PRNL_ROTATION_SPEED, textureRotator->getRotationSpeed());
}
//-----------------------------------------------------------------------
void TextureRotatorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_USE_OWN_SPEED = _("Use own speed");
	PRNL_ROTATION = _("Rotation");
	PRNL_ROTATION_SPEED = _("Rotation speed");

	mHelpHtml = wxT("AffectorTextureRotator.html");

	// Use Own Rotation speed
	Append(wxBoolProperty(PRNL_USE_OWN_SPEED, PRNL_USE_OWN_SPEED, ParticleUniverse::TextureRotator::DEFAULT_USE_OWN_SPEED));

	// Rotation: Dynamic Attribute
	ParticleUniverse::DynamicAttributeFixed dynAttr;
	dynAttr.setValue(ParticleUniverse::TextureRotator::DEFAULT_ROTATION);
	appendDynamicAttribute(PRNL_ROTATION, PRNL_ROTATION, dynAttr);

	// Rotation Speed: Dynamic Attribute
	dynAttr.setValue(ParticleUniverse::TextureRotator::DEFAULT_ROTATION_SPEED);
	appendDynamicAttribute(PRNL_ROTATION_SPEED, PRNL_ROTATION_SPEED, dynAttr);
}