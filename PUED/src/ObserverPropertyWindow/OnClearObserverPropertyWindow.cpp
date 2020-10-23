/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnClearObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
OnClearObserverPropertyWindow::OnClearObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnClearObserverPropertyWindow::OnClearObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnClearObserverPropertyWindow::OnClearObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnClearObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnClearObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update observer with another attribute
	ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
}
//-----------------------------------------------------------------------
void OnClearObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Not own properties
}
//-----------------------------------------------------------------------
void OnClearObserverPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	// TODO

	mHelpHtml = wxT("ObserverOnClear.html");

	// Not own properties
}