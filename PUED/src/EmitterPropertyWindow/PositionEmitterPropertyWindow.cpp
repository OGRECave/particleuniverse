/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "EmitterPropertyWindow/PositionEmitterPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
PositionEmitterPropertyWindow::PositionEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EmitterPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PositionEmitterPropertyWindow::PositionEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) :
	EmitterPropertyWindow(emitterPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
PositionEmitterPropertyWindow::PositionEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::PositionEmitter* emitter) :
	EmitterPropertyWindow(parent, owner, emitter->getName())
{
	copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void PositionEmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::PositionEmitter* positionEmitter = static_cast<ParticleUniverse::PositionEmitter*>(mOwner->getPUElement());
	if (!positionEmitter)
		return;

	if (propertyName == PRNL_POSITION_EMITTER_POSITION || 
		(propertyName.StartsWith(PRNL_POSITION) && propertyName.EndsWith(PRNL_POSITION_X)) ||
		(propertyName.StartsWith(PRNL_POSITION) && propertyName.EndsWith(PRNL_POSITION_Y)) ||
		(propertyName.StartsWith(PRNL_POSITION) && propertyName.EndsWith(PRNL_POSITION_Z)))
	{
		if (propertyName.StartsWith(PRNL_POSITION))
		{
			prop = prop->GetParent()->GetParent();
		}

		// Update emitter with Positions
		copyPositionsToPositionEmitter(prop, positionEmitter);
	}
	else if (propertyName == PRNL_POSITION_EMITTER_RANDOMIZE)
	{
		// Update emitter with Randomize
		copyRandomizeToPositionEmitter(prop, positionEmitter);
	}
	else
	{
		// Update emitter with another attribute
		EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void PositionEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

	// Copy properties from emitter to property window
	ParticleUniverse::PositionEmitter* positionEmitter = static_cast<ParticleUniverse::PositionEmitter*>(emitter);

	// Positions: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_POSITION_EMITTER_POSITION);
	ParentPropertyWithButtonAndPositions* parentPropertyWithButtonAndPositions = static_cast<ParentPropertyWithButtonAndPositions*>(propTo);
	ParticleUniverse::vector<Ogre::Vector3> positions = positionEmitter->getPositions();
	ParticleUniverse::vector<Ogre::Vector3>::iterator it;
	ParticleUniverse::vector<Ogre::Vector3>::iterator itEnd = positions.end();
	parentPropertyWithButtonAndPositions->reset();
	for (it = positions.begin(); it != itEnd; ++it)
	{
		Ogre::Vector3 position = *it;
		parentPropertyWithButtonAndPositions->addPosition(this, position);
	}

	// Randomize: bool
	doSetBool(PRNL_POSITION_EMITTER_RANDOMIZE, positionEmitter->isRandomized());
}
//-----------------------------------------------------------------------
void PositionEmitterPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_POSITION_EMITTER_POSITION = _("Add particle position");
	PRNL_POSITION_EMITTER_RANDOMIZE = _("Position randomize");
	PRNL_POSITION = _("Position");
	PRNL_POSITION_X = _(".x");
	PRNL_POSITION_Y = _(".y");
	PRNL_POSITION_Z = _(".z");

	mHelpHtml = wxT("EmitterPosition.html");

	// Add Position: Button, with wich new positions can be added
	// Todo: Is the memory cleaned up by wxWidgets?
	wxPGId pid = Append(new ParentPropertyWithButtonAndPositions(PRNL_POSITION_EMITTER_POSITION, PRNL_POSITION_EMITTER_POSITION));
	SetPropertyEditor(pid, wxPG_EDITOR(TextCtrlAndButton)); // Add a button

	// Randomize: bool
	Append(wxBoolProperty(PRNL_POSITION_EMITTER_RANDOMIZE, PRNL_POSITION_EMITTER_RANDOMIZE, ParticleUniverse::PositionEmitter::DEFAULT_RANDOMIZE));
}
//-----------------------------------------------------------------------
void PositionEmitterPropertyWindow::copyPositionsToPositionEmitter(wxPGProperty* prop, ParticleUniverse::PositionEmitter* positionEmitter)
{
	ParentPropertyWithButtonAndPositions* parentPropertyWithButtonAndPositions = static_cast<ParentPropertyWithButtonAndPositions*>(prop);
	unsigned int size = parentPropertyWithButtonAndPositions->getNumberOfPositions();
	positionEmitter->removeAllPositions();
	for (unsigned int i = 0; i < size; ++i)
	{
		Ogre::Vector3 vec = Ogre::Vector3::ZERO;
		vec = parentPropertyWithButtonAndPositions->getPosition(this, i, vec);
		positionEmitter->addPosition(vec);
	}
}
//-----------------------------------------------------------------------
void PositionEmitterPropertyWindow::copyRandomizeToPositionEmitter(wxPGProperty* prop, ParticleUniverse::PositionEmitter* positionEmitter)
{
	positionEmitter->setRandomized(prop->DoGetValue().GetBool());
}
