/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "AffectorPropertyWindow/PathFollowerPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
PathFollowerPropertyWindow::PathFollowerPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	AffectorPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PathFollowerPropertyWindow::PathFollowerPropertyWindow(AffectorPropertyWindow* affectorPropertyWindow) :
	AffectorPropertyWindow(affectorPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PathFollowerPropertyWindow::PathFollowerPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::PathFollower* affector) :
	AffectorPropertyWindow(parent, owner, affector->getName())
{
	copyAttributesFromAffector(affector);
}
//-----------------------------------------------------------------------
void PathFollowerPropertyWindow::copyAttributeToAffector(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::PathFollower* affector = static_cast<ParticleUniverse::PathFollower*>(mOwner->getPUElement());
	if (!affector)
		return;

	if (propertyName == PRNL_PATHFOLLOWER_POSITION ||
		(propertyName.StartsWith(PRNL_POSITION) && propertyName.EndsWith(PRNL_POSITION_X)) ||
		(propertyName.StartsWith(PRNL_POSITION) && propertyName.EndsWith(PRNL_POSITION_Y)) ||
		(propertyName.StartsWith(PRNL_POSITION) && propertyName.EndsWith(PRNL_POSITION_Z)))
	{
		// Positions: List
		if (propertyName.StartsWith(PRNL_POSITION))
		{
			prop = prop->GetParent()->GetParent();
		}
		ParentPropertyWithButtonAndPositions* parentPropertyWithButtonAndPositions = static_cast<ParentPropertyWithButtonAndPositions*>(prop);
		unsigned int size = parentPropertyWithButtonAndPositions->getNumberOfPositions();
		affector->clearPoints();
		for (unsigned int i = 0; i < size; ++i)
		{
			Ogre::Vector3 vec = Ogre::Vector3::ZERO;
			vec = parentPropertyWithButtonAndPositions->getPosition(this, i, vec);
			affector->addPoint(vec);
		}
	}
	else
	{
		// Update affector with another attribute
		AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void PathFollowerPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector* affector)
{
	AffectorPropertyWindow::copyAttributesFromAffector(affector);

	// Copy properties from affector to property window
	ParticleUniverse::PathFollower* pathFollower = static_cast<ParticleUniverse::PathFollower*>(affector);
	
	// Positions: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_PATHFOLLOWER_POSITION);
	ParentPropertyWithButtonAndPositions* parentPropertyWithButtonAndPositions = static_cast<ParentPropertyWithButtonAndPositions*>(propTo);
	unsigned short numPositions = pathFollower->getNumPoints();
	parentPropertyWithButtonAndPositions->reset();
	for (unsigned short i = 0; i < numPositions; ++i)
	{
		Ogre::Vector3 position = pathFollower->getPoint(i);
		parentPropertyWithButtonAndPositions->addPosition(this, position);
	}
}
//-----------------------------------------------------------------------
void PathFollowerPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_PATHFOLLOWER_POSITION = _("Add Position");
	PRNL_POSITION = _("Position");
	PRNL_POSITION_X = _(".x");
	PRNL_POSITION_Y = _(".y");
	PRNL_POSITION_Z = _(".z");

	mHelpHtml = wxT("PathFollower.html");

	// Positions
	wxPGId pid = Append(new ParentPropertyWithButtonAndPositions(PRNL_PATHFOLLOWER_POSITION, PRNL_PATHFOLLOWER_POSITION));
	SetPropertyEditor(pid, wxPG_EDITOR(TextCtrlAndButton)); // Add a button
}