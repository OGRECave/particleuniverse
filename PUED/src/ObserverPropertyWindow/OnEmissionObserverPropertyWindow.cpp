/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnEmissionObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
OnEmissionObserverPropertyWindow::OnEmissionObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnEmissionObserverPropertyWindow::OnEmissionObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnEmissionObserverPropertyWindow::OnEmissionObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnEmissionObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnEmissionObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update observer with another attribute
	ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
}
//-----------------------------------------------------------------------
void OnEmissionObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Not own properties
}
//-----------------------------------------------------------------------
void OnEmissionObserverPropertyWindow::_initProperties(void)
{
	mHelpHtml = wxT("ObserverOnEmission.html");

	// Not own properties
}