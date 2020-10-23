/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2014 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseEditor.h"
#include "ParticleUniverseSystemPropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseSystemManager.h"
#include "wx/ogre/utils.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
SystemPropertyWindow::SystemPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) : 
	PropertyWindow(parent, owner, name),
	mRootFrame(0)
{
	_initProperties();
}
//-----------------------------------------------------------------------
void SystemPropertyWindow::copyAttributesFromSystem(const ParticleUniverse::ParticleSystem* system)
{
	// Name: Ogre::String
	wxString templateName = ogre2wx(system->getTemplateName());
	doSetString(PRNL_NAME, templateName);

	// Category: Ogre::String
	wxString category = ogre2wx(system->getCategory());
	doSetString(PRNL_SYSTEM_CATEGORY, category);

	// Keep local: Bool
	doSetBool(PRNL_SYSTEM_KEEP_LOCAL, system->isKeepLocal());

	// Iteration interval: ParticleUniverse::Real
	doSetDouble(PRNL_SYSTEM_ITERATION_INTERVAL, system->getIterationInterval());

	// Fixed timeout: ParticleUniverse::Real
	doSetDouble(PRNL_SYSTEM_FIXED_TIMEOUT, system->getFixedTimeout());

	// Non-visible update timeout: ParticleUniverse::Real
	doSetDouble(PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT, system->getNonVisibleUpdateTimeout());

	// Lod distances: List of ParticleUniverse::Real
	wxPGProperty* prop = GetPropertyPtr(PRNL_SYSTEM_LOD_DISTANCES);
	if (prop)
	{
		ParentPropertyWithButtonAndFloats* propLodDistances = static_cast<ParentPropertyWithButtonAndFloats*>(prop);
		propLodDistances->reset();
		ParticleUniverse::ParticleSystem::LodDistanceList list = system->getLodDistances();
		ParticleUniverse::ParticleSystem::LodDistanceIterator it;
		ParticleUniverse::ParticleSystem::LodDistanceIterator itEnd = list.end();
		for (it = list.begin(); it != itEnd; ++it)
		{
			propLodDistances->addFloat(this, Ogre::Math::Sqrt(*it));
		}
	}

	// Smooth lod: bool
	doSetBool(PRNL_SYSTEM_SMOOTH_LOD, system->isSmoothLod());

	// Fast forward: ParticleUniverse::Real (time) + ParticleUniverse::Real (interval)
	wxString name = PRNL_SYSTEM_FAST_FORWARD + wxT(".") + PRNL_SYSTEM_FAST_FORWARD_TIME;
	prop = GetPropertyPtr(name);
	if (prop)
	{
		prop->DoSetValue(system->getFastForwardTime());
	}

	name = PRNL_SYSTEM_FAST_FORWARD + wxT(".") + PRNL_SYSTEM_FAST_FORWARD_INTERVAL;
	prop = GetPropertyPtr(name);
	if (prop)
	{
		prop->DoSetValue(system->getFastForwardInterval());
	}

	// Main camera name: Ogre::String
	wxString cameraName = ogre2wx(system->getMainCameraName());
	doSetString(PRNL_SYSTEM_MAIN_CAMERA_NAME, cameraName);

	// Scale velocity: ParticleUniverse::Real
	doSetDouble(PRNL_SYSTEM_SCALE_VELOCITY, system->getScaleVelocity());

	// Scale time: ParticleUniverse::Real
	doSetDouble(PRNL_SYSTEM_SCALE_TIME, system->getScaleTime());

	// Scale dimensions: Ogre::Vector3
	doSetVector3(PRNL_SYSTEM_SCALE, system->getScale());

	// Use tight bounding box: bool
	doSetBool(PRNL_SYSTEM_TIGHT_BOUNDING_BOX, system->hasTightBoundingBox());
}
//-----------------------------------------------------------------------
void SystemPropertyWindow::copyAttributeToSystem(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ParticleSystem* system = static_cast<ParticleUniverse::ParticleSystem*>(mOwner->getPUElement());
	if (!system)
		return;

	if (propertyName == PRNL_NAME)
	{
		// Name: String
		Ogre::String name = wx2ogre(prop->GetValueAsString());
		system->setTemplateName(name);

		// Update everything
		if (mRootFrame)
		{
			ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			particleSystemManager->createParticleSystemTemplate(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			mRootFrame->replaceTemplateName(name);
		}
	}
	else if (propertyName == PRNL_SYSTEM_CATEGORY)
	{
		// Category: String
		system->setCategory(wx2ogre(prop->GetValueAsString()));
		if (mRootFrame)
		{
			mRootFrame->notifyCategoryChanged(system);
		}
	}
	else if (propertyName == PRNL_SYSTEM_KEEP_LOCAL)
	{
		// Keep local: Bool
		system->setKeepLocal(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_SYSTEM_ITERATION_INTERVAL)
	{
		// Iteration interval: ParticleUniverse::Real
		system->setIterationInterval(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SYSTEM_FIXED_TIMEOUT)
	{
		// Fixed timeout: ParticleUniverse::Real
		system->setFixedTimeout(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT)
	{
		// Non-visible update timeout: ParticleUniverse::Real
		system->setNonVisibleUpdateTimeout(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SYSTEM_LOD_DISTANCES || propertyName.StartsWith(PRNL_FLOAT))
	{
		// Lod distances: List of ParticleUniverse::Real
		// Todo: Checking on PRNL_FLOAT only is not sufficient if more lists of floats are added to this propertygrid.
		ParentPropertyWithButtonAndFloats* parentPropertyWithButtonAndFloats = 0;
		if (propertyName == PRNL_SYSTEM_LOD_DISTANCES)
		{
			parentPropertyWithButtonAndFloats = static_cast<ParentPropertyWithButtonAndFloats*>(prop);
		}
		else
		{
			wxPGProperty* p = prop->GetParent();
			parentPropertyWithButtonAndFloats = static_cast<ParentPropertyWithButtonAndFloats*>(p);
		}
		unsigned int size = parentPropertyWithButtonAndFloats->getNumberOfFloats();
		system->clearLodDistances();
		for (unsigned int i = 0; i < size; ++i)
		{
			ParticleUniverse::Real f = parentPropertyWithButtonAndFloats->getFloat(this, i);
			system->addLodDistance(f);
		}
	}
	else if (propertyName == PRNL_SYSTEM_SMOOTH_LOD)
	{
		// Smooth lod: bool
		system->setSmoothLod(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_SYSTEM_FAST_FORWARD ||
		propertyName == PRNL_SYSTEM_FAST_FORWARD_TIME ||
		propertyName == PRNL_SYSTEM_FAST_FORWARD_INTERVAL)
	{
		// Fast forward: ParticleUniverse::Real (time) + ParticleUniverse::Real (interval)
		wxPGProperty* prop = GetPropertyPtr(PRNL_SYSTEM_FAST_FORWARD + wxT(".") + PRNL_SYSTEM_FAST_FORWARD_TIME);
		if (!prop)
			return;
		ParticleUniverse::Real time = prop->DoGetValue().GetDouble();
		prop = GetPropertyPtr(PRNL_SYSTEM_FAST_FORWARD + wxT(".") + PRNL_SYSTEM_FAST_FORWARD_INTERVAL);
		if (!prop)
			return;
		ParticleUniverse::Real interval = prop->DoGetValue().GetDouble();
		system->setFastForward(time, interval);
	}
	else if (propertyName == PRNL_SYSTEM_MAIN_CAMERA_NAME)
	{
		// Main camera name: Ogre::String
		wxString name = prop->GetValueAsString();
		Ogre::String cameraName = wx2ogre(name);
		try
		{
			system->setMainCameraName(cameraName);
		}
		catch (Ogre::Exception e)
		{
			// Ignore the exception
		}
	}
	else if (propertyName == PRNL_SYSTEM_SCALE_VELOCITY)
	{
		// Scale velocity: ParticleUniverse::Real
		system->setScaleVelocity(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SYSTEM_SCALE_TIME)
	{
		// Scale time: ParticleUniverse::Real
		system->setScaleTime(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SYSTEM_SCALE + PRNL_X ||
		propertyName == PRNL_SYSTEM_SCALE + PRNL_Y ||
		propertyName == PRNL_SYSTEM_SCALE + PRNL_Z)
	{
		// Scale dimensions: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_SYSTEM_SCALE, v3);
		system->setScale(v3);
	}
	else if (propertyName == PRNL_SYSTEM_TIGHT_BOUNDING_BOX)
	{
		// Use tight bounding box: bool
		system->setTightBoundingBox(prop->DoGetValue().GetBool());
	}
	else
	{
		PropertyWindow::copyAttributeToComponent(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void SystemPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_NAME = _("Name");
	PRNL_SYSTEM_CATEGORY = _("Category");
	PRNL_SYSTEM_KEEP_LOCAL = _("Keep local");
	PRNL_SYSTEM_ITERATION_INTERVAL = _("Iteration interval");
	PRNL_SYSTEM_FIXED_TIMEOUT = _("Fixed timeout");
	PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT = _("Nonvisible update timeout");
	PRNL_SYSTEM_LOD_DISTANCES = _("Lod distances");
	PRNL_SYSTEM_SMOOTH_LOD = _("Smooth lod");
	PRNL_SYSTEM_FAST_FORWARD = _("Fast forward");
	PRNL_SYSTEM_FAST_FORWARD_TIME = _("Time");
	PRNL_SYSTEM_FAST_FORWARD_INTERVAL = _("Interval");
	PRNL_SYSTEM_MAIN_CAMERA_NAME = _("Main camera name");
	PRNL_SYSTEM_SCALE_VELOCITY = _("Scale velocity");
	PRNL_SYSTEM_SCALE_TIME = _("Scale time");
	PRNL_SYSTEM_SCALE = _("Scale dimensions");
	PRNL_SYSTEM_TIGHT_BOUNDING_BOX = _("Use tight bounding box");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_FLOAT = _("Value");

	mHelpHtml = wxT("ParticleSystem.html");

	// Category: String
	Append(wxStringProperty(PRNL_SYSTEM_CATEGORY, PRNL_SYSTEM_CATEGORY, WX_STRING_BLANK));

	// Keep local: Bool
	SetBoolChoices (_("True"), _("False")); // Forces Internationalization
	Append(wxBoolProperty(PRNL_SYSTEM_KEEP_LOCAL, PRNL_SYSTEM_KEEP_LOCAL, ParticleUniverse::ParticleSystem::DEFAULT_KEEP_LOCAL));

	// Iteration interval: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SYSTEM_ITERATION_INTERVAL, PRNL_SYSTEM_ITERATION_INTERVAL, ParticleUniverse::ParticleSystem::DEFAULT_ITERATION_INTERVAL));
	SetPropertyEditor(PRNL_SYSTEM_ITERATION_INTERVAL, wxPG_EDITOR(SpinCtrl));

	// Fixed timeout: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SYSTEM_FIXED_TIMEOUT, PRNL_SYSTEM_FIXED_TIMEOUT, ParticleUniverse::ParticleSystem::DEFAULT_FIXED_TIMEOUT));
	SetPropertyEditor(PRNL_SYSTEM_FIXED_TIMEOUT, wxPG_EDITOR(SpinCtrl));

	// Non-visible update timeout: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT, PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT, ParticleUniverse::ParticleSystem::DEFAULT_NON_VISIBLE_UPDATE_TIMEOUT));
	SetPropertyEditor(PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT, wxPG_EDITOR(SpinCtrl));

	// Lod distances: List of ParticleUniverse::Real
	wxPGId pid = Append(new ParentPropertyWithButtonAndFloats(PRNL_SYSTEM_LOD_DISTANCES, PRNL_SYSTEM_LOD_DISTANCES));
	SetPropertyEditor(pid, wxPG_EDITOR(TextCtrlAndButton)); // Add a button

	// Smooth lod: bool
	Append(wxBoolProperty(PRNL_SYSTEM_SMOOTH_LOD, PRNL_SYSTEM_SMOOTH_LOD, ParticleUniverse::ParticleSystem::DEFAULT_SMOOTH_LOD));

	// Fast forward: ParticleUniverse::Real (time) + ParticleUniverse::Real (interval)
	pid = Append(wxParentProperty(PRNL_SYSTEM_FAST_FORWARD, PRNL_SYSTEM_FAST_FORWARD));
	AppendIn(pid, wxFloatProperty(PRNL_SYSTEM_FAST_FORWARD_TIME, PRNL_SYSTEM_FAST_FORWARD_TIME, ParticleUniverse::ParticleSystem::DEFAULT_FAST_FORWARD_TIME));
	AppendIn(pid, wxFloatProperty(PRNL_SYSTEM_FAST_FORWARD_INTERVAL, PRNL_SYSTEM_FAST_FORWARD_INTERVAL, 0.0f));
	SetPropertyEditor(PRNL_SYSTEM_FAST_FORWARD + wxT(".") + PRNL_SYSTEM_FAST_FORWARD_TIME, wxPG_EDITOR(SpinCtrl));
	SetPropertyEditor(PRNL_SYSTEM_FAST_FORWARD + wxT(".") + PRNL_SYSTEM_FAST_FORWARD_INTERVAL, wxPG_EDITOR(SpinCtrl));

	// Main camera name: Ogre::String
	Append(wxStringProperty(PRNL_SYSTEM_MAIN_CAMERA_NAME, PRNL_SYSTEM_MAIN_CAMERA_NAME, WX_STRING_BLANK));

	// Scale velocity: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SYSTEM_SCALE_VELOCITY, PRNL_SYSTEM_SCALE_VELOCITY, ParticleUniverse::ParticleSystem::DEFAULT_SCALE_VELOCITY));
	SetPropertyEditor(PRNL_SYSTEM_SCALE_VELOCITY, wxPG_EDITOR(SpinCtrl));

	// Scale time: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SYSTEM_SCALE_TIME, PRNL_SYSTEM_SCALE_TIME, ParticleUniverse::ParticleSystem::DEFAULT_SCALE_TIME));
	SetPropertyEditor(PRNL_SYSTEM_SCALE_TIME, wxPG_EDITOR(SpinCtrl));

	// Scale dimensions: Ogre::Vector3
	appendVector3(PRNL_SYSTEM_SCALE, PRNL_SYSTEM_SCALE, Ogre::Vector3::UNIT_SCALE);

	// Use tight bounding box: bool
	Append(wxBoolProperty(PRNL_SYSTEM_TIGHT_BOUNDING_BOX, PRNL_SYSTEM_TIGHT_BOUNDING_BOX, ParticleUniverse::ParticleSystem::DEFAULT_TIGHT_BOUNDINGBOX));
}
//-----------------------------------------------------------------------
void SystemPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	// Perform additional validations.
	if (!_validatePropertyStringNoSpaces(event.GetPropertyPtr(), PRNL_SYSTEM_MAIN_CAMERA_NAME))
		return;

	wxString propertyName = event.GetPropertyName();
	wxPGProperty* prop = event.GetPropertyPtr();
	PropertyWindow::onPropertyChanged(event);
	copyAttributeToSystem(prop, propertyName);
	ParticleUniverse::ParticleSystem* system = static_cast<ParticleUniverse::ParticleSystem*>(mOwner->getPUElement());
	if (system && system->_isMarkedForEmission())
	{
		// Force recreation of new emitted systems
		// Todo: Implement later, because this version of the editor doesn't support emitted particle systems.
	}
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
ParticleUniverseEditorFrame* SystemPropertyWindow::getRootFrame(void)
{
	return mRootFrame;
}
//-----------------------------------------------------------------------
void SystemPropertyWindow::setRootFrame(ParticleUniverseEditorFrame* rootFrame)
{
	mRootFrame = rootFrame;
}
