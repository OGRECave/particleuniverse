/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnRandomObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
OnRandomObserverPropertyWindow::OnRandomObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnRandomObserverPropertyWindow::OnRandomObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnRandomObserverPropertyWindow::OnRandomObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnRandomObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnRandomObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::OnRandomObserver* observer = static_cast<ParticleUniverse::OnRandomObserver*>(mOwner->getPUElement());
	if (!observer)
		return;

	if (propertyName == PRNL_ON_RANDOM_VALUE)
	{
		// Random Threshold: ParticleUniverse::Real
		observer->setThreshold(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update observer with another attribute
		ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void OnRandomObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Copy properties from observer to property window
	ParticleUniverse::OnRandomObserver* onRandomObserver = static_cast<ParticleUniverse::OnRandomObserver*>(observer);

	// Random Threshold
	doSetDouble(PRNL_ON_RANDOM_VALUE, onRandomObserver->getThreshold());
}
//-----------------------------------------------------------------------
void OnRandomObserverPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ON_RANDOM_VALUE = _("Random threshold");

	mHelpHtml = wxT("ObserverOnRandom.html");

	// Random Threshold: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_ON_RANDOM_VALUE, PRNL_ON_RANDOM_VALUE, ParticleUniverse::OnRandomObserver::DEFAULT_THRESHOLD));
	SetPropertyEditor(PRNL_ON_RANDOM_VALUE, wxPG_EDITOR(SpinCtrl));
}