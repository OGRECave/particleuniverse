/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "BehaviourPropertyWindow/SlaveBehaviourPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
SlaveBehaviourPropertyWindow::SlaveBehaviourPropertyWindow(wxWindow* parent, EditComponent* owner) :
	BehaviourPropertyWindow(parent, owner)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SlaveBehaviourPropertyWindow::SlaveBehaviourPropertyWindow(BehaviourPropertyWindow* behaviourPropertyWindow) :
	BehaviourPropertyWindow(behaviourPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SlaveBehaviourPropertyWindow::SlaveBehaviourPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::SlaveBehaviour* behaviour) :
	BehaviourPropertyWindow(parent, owner)
{
	copyAttributesFromBehaviour(behaviour);
}
//-----------------------------------------------------------------------
void SlaveBehaviourPropertyWindow::copyAttributeToBehaviour(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update behaviour with another attribute
	BehaviourPropertyWindow::copyAttributeToBehaviour(prop, propertyName);
}
//-----------------------------------------------------------------------
void SlaveBehaviourPropertyWindow::copyAttributesFromBehaviour(const ParticleUniverse::ParticleBehaviour* behaviour)
{
	BehaviourPropertyWindow::copyAttributesFromBehaviour(behaviour);
}
//-----------------------------------------------------------------------
void SlaveBehaviourPropertyWindow::_initProperties(void)
{
	mHelpHtml = wxT("BehaviourSlave.html");

	// There are no properties or connections needed.
}