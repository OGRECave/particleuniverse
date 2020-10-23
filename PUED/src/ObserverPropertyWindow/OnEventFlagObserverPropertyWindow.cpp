/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ObserverPropertyWindow/OnEventFlagObserverPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
OnEventFlagObserverPropertyWindow::OnEventFlagObserverPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	ObserverPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnEventFlagObserverPropertyWindow::OnEventFlagObserverPropertyWindow(ObserverPropertyWindow* observerPropertyWindow) :
	ObserverPropertyWindow(observerPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
OnEventFlagObserverPropertyWindow::OnEventFlagObserverPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::OnEventFlagObserver* observer) :
	ObserverPropertyWindow(parent, owner, observer->getName())
{
	copyAttributesFromObserver(observer);
}
//-----------------------------------------------------------------------
void OnEventFlagObserverPropertyWindow::copyAttributeToObserver(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::OnEventFlagObserver* observer = static_cast<ParticleUniverse::OnEventFlagObserver*>(mOwner->getPUElement());
	if (!observer)
		return;

	if (propertyName == PRNL_ON_EVENTFLAG_FLAG)
	{
		// Event Flag: ParticleUniverse::uint32
		observer->setEventFlag(prop->DoGetValue().GetLong());
	}
	else
	{
		// Update observer with another attribute
		ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void OnEventFlagObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver* observer)
{
	ObserverPropertyWindow::copyAttributesFromObserver(observer);

	// Copy properties from observer to property window
	ParticleUniverse::OnEventFlagObserver* onEventFlagObserver = static_cast<ParticleUniverse::OnEventFlagObserver*>(observer);

	// Event Flag: ParticleUniverse::uint32
	doSetLong(PRNL_ON_EVENTFLAG_FLAG, onEventFlagObserver->getEventFlag());
}
//-----------------------------------------------------------------------
void OnEventFlagObserverPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_ON_EVENTFLAG_FLAG = _("Event Flag");

	mHelpHtml = wxT("ObserverOnEventFlag.html");

	// Event Flag: ParticleUniverse::uint32
	Append(wxUIntProperty(PRNL_ON_EVENTFLAG_FLAG, PRNL_ON_EVENTFLAG_FLAG, ParticleUniverse::OnEventFlagObserver::DEFAULT_EVENT_FLAG));
	SetPropertyEditor(PRNL_ON_EVENTFLAG_FLAG, wxPG_EDITOR(SpinCtrl));
}
