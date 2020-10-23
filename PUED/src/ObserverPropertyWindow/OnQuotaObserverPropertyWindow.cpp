/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnQuotaObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
OnQuotaObserverPropertyWindow::OnQuotaObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnQuotaObserverPropertyWindow::OnQuotaObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnQuotaObserverPropertyWindow::OnQuotaObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnQuotaObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnQuotaObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update observer with another attribute
	ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
}
//-----------------------------------------------------------------------
void OnQuotaObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Not own properties
}
//-----------------------------------------------------------------------
void OnQuotaObserverPropertyWindow::_initProperties(void)
{
	mHelpHtml = wxT("ObserverOnQuota.html");

	// Not own properties
}