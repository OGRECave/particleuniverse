/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/ScaleAffectorPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
ScaleAffectorPropertyWindow::ScaleAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ScaleAffectorPropertyWindow::ScaleAffectorPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
ScaleAffectorPropertyWindow::ScaleAffectorPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::ScaleAffector* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void ScaleAffectorPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ScaleAffector* affector = static_cast<ParticleUniverse::ScaleAffector*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName.StartsWith(PRNL_XYZ_SCALE))
	{
		// ScaleXYZ: DynamicAttribute
		affector->resetDynScaleXYZ(false); // Value has changed, so the flag is set internally in PU.
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_XYZ_SCALE, affector->getDynScaleXYZ());
		if (dynAttr)
		{
			affector->setDynScaleXYZ(dynAttr);
		}
		dynAttr = affector->getDynScaleXYZ();
		if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && dynAttr->getValue() == ParticleUniverse::ScaleAffector::DEFAULT_XYZ_SCALE)
		{
			// Force default state; internal flag in PU must be reset, which isn't done if the value is changed.
			affector->resetDynScaleXYZ(true);
		}

		if (affector->_isMarkedForEmission())
		{
			_unprepare(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
		}
	}
	else if (propertyName.StartsWith(PRNL_X_SCALE))
	{
		// ScaleX: DynamicAttribute
		affector->resetDynScaleX(false);
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_X_SCALE, affector->getDynScaleX());
		if (dynAttr)
		{
			affector->setDynScaleX(dynAttr);
		}
		dynAttr = affector->getDynScaleX();
		if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && dynAttr->getValue() == ParticleUniverse::ScaleAffector::DEFAULT_X_SCALE)
		{
			// Force default state
			affector->resetDynScaleX(true);
		}

		if (affector->_isMarkedForEmission())
		{
			_unprepare(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
		}
	}
	else if (propertyName.StartsWith(PRNL_Y_SCALE))
	{
		// ScaleY: DynamicAttribute
		affector->resetDynScaleY(false);
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_Y_SCALE, affector->getDynScaleY());
		if (dynAttr)
		{
			affector->setDynScaleY(dynAttr);
		}
		dynAttr = affector->getDynScaleY();
		if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && dynAttr->getValue() == ParticleUniverse::ScaleAffector::DEFAULT_Y_SCALE)
		{
			// Force default state
			affector->resetDynScaleY(true);
		}

		if (affector->_isMarkedForEmission())
		{
			_unprepare(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
		}
	}
	else if (propertyName.StartsWith(PRNL_Z_SCALE))
	{
		// ScaleZ: DynamicAttribute
		affector->resetDynScaleZ(false);
		ParticleUniverse::DynamicAttribute* dynAttr = copyValuesToDynamicAttribute(propertyName, prop, PRNL_Z_SCALE, affector->getDynScaleZ());
		if (dynAttr)
		{
			affector->setDynScaleZ(dynAttr);
		}
		dynAttr = affector->getDynScaleZ();
		if (dynAttr->getType() == ParticleUniverse::DynamicAttribute::DAT_FIXED && dynAttr->getValue() == ParticleUniverse::ScaleAffector::DEFAULT_Z_SCALE)
		{
			// Force default state
			affector->resetDynScaleZ(true);
		}

		if (affector->_isMarkedForEmission())
		{
			_unprepare(affector, ParticleUniverse::Particle::PT_AFFECTOR, ParticleUniverse::Particle::PT_AFFECTOR);
		}
	}
	else if (propertyName == PRNL_SINCE_START_SYSTEM)
	{
		// Since Start System: bool
		affector->setSinceStartSystem(prop->DoGetValue().GetBool());
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void ScaleAffectorPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::ScaleAffector* scaleAffector = static_cast<ParticleUniverse::ScaleAffector*>(affector);

	// ScaleXYZ: DynamicAttribute
	doSetDynamicAttribute(PRNL_XYZ_SCALE, scaleAffector->getDynScaleXYZ());

	// ScaleX: DynamicAttribute
	doSetDynamicAttribute(PRNL_X_SCALE, scaleAffector->getDynScaleX());

	// ScaleY: DynamicAttribute
	doSetDynamicAttribute(PRNL_Y_SCALE, scaleAffector->getDynScaleY());

	// ScaleZ: DynamicAttribute
	doSetDynamicAttribute(PRNL_Z_SCALE, scaleAffector->getDynScaleZ());

	// Since Start System: bool
	doSetBool(PRNL_SINCE_START_SYSTEM, scaleAffector->isSinceStartSystem());
}
//-----------------------------------------------------------------------
void ScaleAffectorPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_XYZ_SCALE = _("Scale x, y and z");
	PRNL_X_SCALE = _("Scale x");
	PRNL_Y_SCALE = _("Scale y");
	PRNL_Z_SCALE = _("Scale z");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_SINCE_START_SYSTEM = _("Since start system");

	mHelpHtml = wxT("AffectorScale.html");

	// ScaleXYZ: DynamicAttribute
	ParticleUniverse::DynamicAttributeFixed dynAttr;
	dynAttr.setValue(ParticleUniverse::ScaleAffector::DEFAULT_XYZ_SCALE);
	appendDynamicAttribute(PRNL_XYZ_SCALE, PRNL_XYZ_SCALE, dynAttr);

	// ScaleX: DynamicAttribute
	dynAttr.setValue(ParticleUniverse::ScaleAffector::DEFAULT_X_SCALE);
	appendDynamicAttribute(PRNL_X_SCALE, PRNL_X_SCALE, dynAttr);

	// ScaleY: DynamicAttribute
	dynAttr.setValue(ParticleUniverse::ScaleAffector::DEFAULT_Y_SCALE);
	appendDynamicAttribute(PRNL_Y_SCALE, PRNL_Y_SCALE, dynAttr);

	// ScaleZ: DynamicAttribute
	dynAttr.setValue(ParticleUniverse::ScaleAffector::DEFAULT_Z_SCALE);
	appendDynamicAttribute(PRNL_Z_SCALE, PRNL_Z_SCALE, dynAttr);

	// Since Start System: bool
	Append(wxBoolProperty(PRNL_SINCE_START_SYSTEM, PRNL_SINCE_START_SYSTEM, false));
}