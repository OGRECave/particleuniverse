/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnExpireObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
OnExpireObserverPropertyWindow::OnExpireObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnExpireObserverPropertyWindow::OnExpireObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnExpireObserverPropertyWindow::OnExpireObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnExpireObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnExpireObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update observer with another attribute
	ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
}
//-----------------------------------------------------------------------
void OnExpireObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Not own properties
}
//-----------------------------------------------------------------------
void OnExpireObserverPropertyWindow::_initProperties(void)
{
	mHelpHtml = wxT("ObserverOnExpire.html");

	// Not own properties
}