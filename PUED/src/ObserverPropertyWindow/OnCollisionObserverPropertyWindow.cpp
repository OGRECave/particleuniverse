/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnCollisionObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
OnCollisionObserverPropertyWindow::OnCollisionObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnCollisionObserverPropertyWindow::OnCollisionObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnCollisionObserverPropertyWindow::OnCollisionObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnCollisionObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnCollisionObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update observer with another attribute
	ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
}
//-----------------------------------------------------------------------
void OnCollisionObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Not own properties
}
//-----------------------------------------------------------------------
void OnCollisionObserverPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	// TODO

	mHelpHtml = wxT("ObserverOnCollision.html");

	// Not own properties
}