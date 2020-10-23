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
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniversePropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseControlPointDialog.h"
#include "ParticleUniverseUtils.h"
#include "wx/ogre/utils.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
EnumPropertyWithButton::EnumPropertyWithButton(
	const wxString& label, 
	const wxString& name,
	const wxArrayString& choices) :
	wxEnumPropertyClass(label, name, choices)
{
}
//-----------------------------------------------------------------------
bool EnumPropertyWithButton::OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
	return true;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ParentPropertyWithButtonAndPositions::ParentPropertyWithButtonAndPositions(
	const wxString& label, 
	const wxString& name) :
	wxParentPropertyClass(label, name),
	mPosition(0)
{
}
//-----------------------------------------------------------------------
bool ParentPropertyWithButtonAndPositions::OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
	if (event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED)
	{
		// Insert a position
		addPosition(propgrid);
	}
	return true;
}
//-----------------------------------------------------------------------
wxPGId* ParentPropertyWithButtonAndPositions::addPosition(wxPropertyGrid* propgrid, Ogre::Vector3 vec3)
{
	// Insert a position
	wxString postFix = ogre2wx(Ogre::StringConverter::toString(mPosition));
	wxString name = PRNL_POSITION + postFix;
	wxPGId pid = propgrid->AppendIn(this, wxParentProperty(PRNL_POSITION, name));
	wxPGId* xid = &(propgrid->AppendIn(pid, wxFloatProperty(PRNL_POSITION + PRNL_POSITION_X, name + PRNL_POSITION_X, vec3.x)));
	propgrid->SetPropertyEditor(*xid, wxPG_EDITOR(SpinCtrl));
	wxPGId yid = propgrid->AppendIn(pid, wxFloatProperty(PRNL_POSITION + PRNL_POSITION_Y, name + PRNL_POSITION_Y, vec3.y));
	propgrid->SetPropertyEditor(yid, wxPG_EDITOR(SpinCtrl));
	wxPGId zid = propgrid->AppendIn(pid, wxFloatProperty(PRNL_POSITION + PRNL_POSITION_Z, name + PRNL_POSITION_Z, vec3.z));
	propgrid->SetPropertyEditor(zid, wxPG_EDITOR(SpinCtrl));
	mPosition++;
	propgrid->GetPrevProperty(*xid);
	return xid;
}
//-----------------------------------------------------------------------
unsigned int ParentPropertyWithButtonAndPositions::getNumberOfPositions(void)
{
	return mPosition;
}
//-----------------------------------------------------------------------
const Ogre::Vector3& ParentPropertyWithButtonAndPositions::getPosition(wxPropertyGrid* propgrid, unsigned int index, Ogre::Vector3& vector)
{
	if (index >= mPosition)
		return vector;

	// Use 'parent.child1.child2' construction, where child 1 is the parent of child 2
	wxString postFix = ogre2wx(Ogre::StringConverter::toString(index));
	wxString name = GetName() + wxT(".") + PRNL_POSITION + postFix + wxT(".") + PRNL_POSITION + postFix + PRNL_POSITION_X;
	wxPGProperty* prop = propgrid->GetPropertyPtr(name);
	if (prop)
	{
		vector.x = prop->DoGetValue().GetDouble();
	}
	name = GetName() + wxT(".") + PRNL_POSITION + postFix + wxT(".") + PRNL_POSITION + postFix + PRNL_POSITION_Y;
	prop = propgrid->GetPropertyPtr(name);
	if (prop)
	{
		vector.y = prop->DoGetValue().GetDouble();
	}
	name = GetName() + wxT(".") + PRNL_POSITION + postFix + wxT(".") + PRNL_POSITION + postFix + PRNL_POSITION_Z;
	prop = propgrid->GetPropertyPtr(name);
	if (prop)
	{
		vector.z = prop->DoGetValue().GetDouble();
	}
	
	return vector;
}
//-----------------------------------------------------------------------
void ParentPropertyWithButtonAndPositions::reset(void)
{
	mPosition = 0;
	this->Empty();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ParentPropertyWithButtonAndFloats::ParentPropertyWithButtonAndFloats(
	const wxString& label, 
	const wxString& name) :
	wxParentPropertyClass(label, name),
	mFloat(0)
{
}
//-----------------------------------------------------------------------
bool ParentPropertyWithButtonAndFloats::OnEvent (wxPropertyGrid* propgrid, wxWindow* wnd_primary, wxEvent& event)
{
	if (event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED)
	{
		// Insert a float
		addFloat(propgrid);
	}
	return true;
}
//-----------------------------------------------------------------------
wxPGId* ParentPropertyWithButtonAndFloats::addFloat(wxPropertyGrid* propgrid, float value)
{
	// Insert a float
	wxString postFix = ogre2wx(Ogre::StringConverter::toString(mFloat));
	wxString name = PRNL_FLOAT + postFix;
	wxPGId* xid = &(propgrid->AppendIn(this, wxFloatProperty(PRNL_FLOAT, name, value)));
	propgrid->SetPropertyEditor(*xid, wxPG_EDITOR(SpinCtrl));
	mFloat++;
	propgrid->GetPrevProperty(*xid);
	return xid;
}
//-----------------------------------------------------------------------
unsigned int ParentPropertyWithButtonAndFloats::getNumberOfFloats(void)
{
	return mFloat;
}
//-----------------------------------------------------------------------
const ParticleUniverse::Real ParentPropertyWithButtonAndFloats::getFloat(wxPropertyGrid* propgrid, unsigned int index)
{
	if (index >= mFloat)
		return 0;

	wxString postFix = ogre2wx(Ogre::StringConverter::toString(index));
	wxString name = this->GetName() + wxT(".") + PRNL_FLOAT + postFix;
	wxPGProperty* prop = propgrid->GetPropertyPtr(name);
	if (prop)
	{
		return prop->DoGetValue().GetDouble();
	}
	
	return 0;
}
//-----------------------------------------------------------------------
void ParentPropertyWithButtonAndFloats::reset(void)
{
	mFloat = 0;
	this->Empty();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PropertyWindow::PropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) : wxPropertyGrid(
	parent, 
	wxID_ANY, 
	wxPoint(0, 99999),
	wxSize(PROPERTIES_WIDTH, PROPERTIES_HEIGHT), 
	wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE,
	wxT("")),
	mName(name),
	mOwner(owner)
{
	/* Fix Version 1.3.1: Set y-coord by default to infinite (99999) to prevent that it is displayed over the listbox with templates. This prevents
	   jittering of the listbox (= wxTreeCtrl from  version 1.4).
	*/ 
	_initProperties();
	Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) &wxPropertyGrid::OnCustomEditorEvent);
	Connect(wxID_ANY, wxEVT_PG_CHANGED, wxPropertyGridEventHandler(PropertyWindow::onPropertyChanged));
}
//-----------------------------------------------------------------------
const Ogre::String& PropertyWindow::getComponentName() const
{
	return mName;
}
//-----------------------------------------------------------------------
void PropertyWindow::setComponentName(const Ogre::String& name)
{
	mName = name;
	wxPGProperty* prop = GetPropertyPtr(PRNL_NAME);
	if (prop)
	{
		wxString n = ogre2wx(name); 
		prop->SetValueFromString(n);
	}
}
//-----------------------------------------------------------------------
void PropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_NAME = _("Name");
	PRNL_POST_RGB = _(" RGB");
	PRNL_POST_ALPHA = _(" Alpha");
	PRNL_W = _(".w");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_TYPE = _("Attribute type");
	PRNL_VALUE = _("Value");
	PRNL_MIN_VALUE = _("Minimum");
	PRNL_MAX_VALUE = _("Maximum");
	PRNL_OSC_TYPE = _("Oscillation type");
	PRNL_OSC_SINE = _("Sine");
	PRNL_OSC_SQUARE = _("Square");
	PRNL_OSC_FREQUENCY = _("Frequency");
	PRNL_OSC_PHASE = _("Phase");
	PRNL_OSC_BASE = _("Base");
	PRNL_OSC_AMPLITUDE = _("Amplitude");
	PRNL_CURVED_INTERPOLATION = _("Interpolation");
	PRNL_CONTROL_POINT = _("Control Point");
	PRNL_POSITION = _("Position");
	PRNL_CONTROL_POINT_X = _(".x");
	PRNL_CONTROL_POINT_Y = _(".y");
	PRNL_POSITION_X = _(".x");
	PRNL_POSITION_Y = _(".y");
	PRNL_POSITION_Z = _(".z");
	PRNL_FLOAT = _("Value");
	DYN_FIXED = _("Fixed");
	DYN_RANDOM = _("Random");
	DYN_CURVED = _("Curved");
	DYN_OSCILLATE = _("Oscillate");
	DYN_CURVED_LINEAR = _("Linear");
	DYN_CURVED_SPLINE = _("Spline");
	PT_ALL = _("All types");
	PT_VISUAL = _("Visual");
	PT_EMITTER = _("Emitter");
	PT_AFFECTOR = _("Affector");
	PT_SYSTEM = _("System");
	PT_TECHNIQUE = _("Technique");

	mHelpHtml = wxT("index.html");
	if (mName.length() > 0)
	{
		Append (wxStringProperty(PRNL_NAME, PRNL_NAME, ogre2wx(mName)));
	}
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetBool(const wxString& name, bool boolValue)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return;

	prop->SetValueFromInt(boolValue);
}
//-----------------------------------------------------------------------
bool PropertyWindow::doGetBool(const wxString& name)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return false;
	return prop->DoGetValue().GetBool();
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetDouble(const wxString& name, double doubleValue)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return;
	prop->DoSetValue(doubleValue);
}
//-----------------------------------------------------------------------
double PropertyWindow::doGetDouble(const wxString& name)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return 0;
	return prop->DoGetValue().GetDouble();
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetLong(const wxString& name, long longValue)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return;
	prop->DoSetValue(longValue);
}
//-----------------------------------------------------------------------
long PropertyWindow::doGetLong(const wxString& name)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return 0;
	return prop->DoGetValue().GetLong();
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetUint16(const wxString& name, wxUint16 uInt16Value)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return;
	prop->DoSetValue(uInt16Value);
}
//-----------------------------------------------------------------------
wxUint16 PropertyWindow::doGetUint16(const wxString& name)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return 0;
	return prop->DoGetValue().GetLong();
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetString(const wxString& name, const wxString& stringValue)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return;
	prop->DoSetValue(stringValue);
}
//-----------------------------------------------------------------------
const wxString& PropertyWindow::doGetString(const wxString& name)
{
	wxPGProperty* prop = GetPropertyPtr(name);
	if (!prop)
		return WX_STRING_BLANK;
	return prop->DoGetValue().GetString();
}
//-----------------------------------------------------------------------
void PropertyWindow::appendColourWithAlphaProperty(const wxString& label, const wxString& name, const wxColour& colour)
{
	wxPGId pid = Append(wxParentProperty(label, name));
	AppendIn(pid, wxColourProperty(name + PRNL_POST_RGB, name + PRNL_POST_RGB, colour));
	wxPGId aid = AppendIn(pid, wxIntProperty(name + PRNL_POST_ALPHA, name + PRNL_POST_ALPHA, colour.Alpha()));
	SetPropertyEditor(aid, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void PropertyWindow::appendInColourWithAlphaProperty(wxPGId& id, const wxString& label, const wxString& name, const wxColour& colour)
{
	wxPGId pid = AppendIn(id, wxParentProperty(label, name));
	AppendIn(pid, wxColourProperty(name + PRNL_POST_RGB, name + PRNL_POST_RGB, colour));
	wxPGId aid = AppendIn(pid, wxIntProperty(name + PRNL_POST_ALPHA, name + PRNL_POST_ALPHA, colour.Alpha()));
	SetPropertyEditor(aid, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetColourWithAlpha(const wxString& name, const wxColour& colour, bool nameIsBaseName)
{
	wxString baseName;
	if (nameIsBaseName)
	{
		baseName = name;
	}
	else
	{
		// Child accessable with name "parent.string"
		baseName = name + wxT(".") + name;
	}
	wxPGProperty* prop = GetPropertyPtr(baseName + PRNL_POST_RGB);
	if (!prop)
		return;
	prop->DoSetValue(colour);

	prop = GetPropertyPtr(baseName + PRNL_POST_ALPHA);
	if (!prop)
		return;

	prop->DoSetValue(colour.Alpha());
}
//-----------------------------------------------------------------------
const wxColour& PropertyWindow::doGetColourWithAlpha(const wxString& name, wxColour& colour, bool nameIsBaseName)
{
	wxString baseName;
	if (nameIsBaseName)
	{
		baseName = name;
	}
	else
	{
		// Child accessable with name "parent.string"
		baseName = name + wxT(".") + name;
	}
	wxPGProperty* prop = GetPropertyPtr(baseName + PRNL_POST_RGB);
	if (!prop)
		return colour;
	wxVariant value = prop->GetValueAsVariant();
	wxColour* txcol = wxGetVariantCast(value, wxColour);
	colour = wxColour(txcol->Red(), txcol->Green(), txcol->Blue());

	prop = GetPropertyPtr(baseName + PRNL_POST_ALPHA);
	if (!prop)
		return colour;
	unsigned char alpha = prop->DoGetValue().GetLong();
	colour = wxColor(colour.Red(), colour.Green(), colour.Blue(), alpha);
	return colour;
}
//-----------------------------------------------------------------------
void PropertyWindow::appendVector3(const wxString& label, const wxString& name, const Ogre::Vector3& vector3)
{
	wxPGId pid = Append(wxParentProperty(label, name));
	wxPGId xid = AppendIn(pid, wxFloatProperty(name + PRNL_X, name + PRNL_X, vector3.x));
	SetPropertyEditor(xid, wxPG_EDITOR(SpinCtrl));
	wxPGId yid = AppendIn(pid, wxFloatProperty(name + PRNL_Y, name + PRNL_Y, vector3.y));
	SetPropertyEditor(yid, wxPG_EDITOR(SpinCtrl));
	wxPGId zid = AppendIn(pid, wxFloatProperty(name + PRNL_Z, name + PRNL_Z, vector3.z));
	SetPropertyEditor(zid, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetVector3(const wxString& name, const Ogre::Vector3& vector3)
{
	// Child accessable with name "parent.string"
	wxString baseName = name + wxT(".") + name;
	wxPGProperty* prop = GetPropertyPtr(baseName + PRNL_X);
	if (!prop)
		return;
	prop->DoSetValue(vector3.x);
	prop = GetPropertyPtr(baseName + PRNL_Y);
	if (!prop)
		return;
	prop->DoSetValue(vector3.y);
	prop = GetPropertyPtr(baseName + PRNL_Z);
	if (!prop)
		return;
	prop->DoSetValue(vector3.z);
}
//-----------------------------------------------------------------------
const Ogre::Vector3& PropertyWindow::doGetVector3(const wxString& name, Ogre::Vector3& vector3)
{
	// Child accessable with name "parent.string"
	wxString baseName = name + wxT(".") + name;
	wxPGProperty* prop = GetPropertyPtr(baseName + PRNL_X);
	if (!prop)
		return Ogre::Vector3::ZERO;
	vector3.x = prop->DoGetValue().GetDouble();
	prop = GetPropertyPtr(baseName + PRNL_Y);
	if (!prop)
		return Ogre::Vector3::ZERO;
	vector3.y = prop->DoGetValue().GetDouble();
	prop = GetPropertyPtr(baseName + PRNL_Z);
	if (!prop)
		return Ogre::Vector3::ZERO;
	vector3.z = prop->DoGetValue().GetDouble();
	return vector3;
}
//-----------------------------------------------------------------------
void PropertyWindow::appendVector4(const wxString& label, const wxString& name, const Ogre::Vector4& vector4)
{
	wxPGId pid = Append(wxParentProperty(label, name));
	wxPGId xid = AppendIn(pid, wxFloatProperty(name + PRNL_X, name + PRNL_X, vector4.x));
	SetPropertyEditor(xid, wxPG_EDITOR(SpinCtrl));
	wxPGId yid = AppendIn(pid, wxFloatProperty(name + PRNL_Y, name + PRNL_Y, vector4.y));
	SetPropertyEditor(yid, wxPG_EDITOR(SpinCtrl));
	wxPGId zid = AppendIn(pid, wxFloatProperty(name + PRNL_Z, name + PRNL_Z, vector4.z));
	SetPropertyEditor(zid, wxPG_EDITOR(SpinCtrl));
	wxPGId wid = AppendIn(pid, wxFloatProperty(name + PRNL_W, name + PRNL_W, vector4.w));
	SetPropertyEditor(wid, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetVector4(const wxString& name, const Ogre::Vector4& vector4)
{
	// Child accessable with name "parent.string"
	wxString baseName = name + wxT(".") + name;
	wxPGProperty* prop = GetPropertyPtr(baseName + PRNL_X);
	if (!prop)
		return;
	prop->DoSetValue(vector4.x);
	
	prop = GetPropertyPtr(baseName + PRNL_Y);
	if (!prop)
		return;
	prop->DoSetValue(vector4.y);
	
	prop = GetPropertyPtr(baseName + PRNL_Z);
	if (!prop)
		return;
	prop->DoSetValue(vector4.z);
	
	prop = GetPropertyPtr(baseName + PRNL_W);
	if (!prop)
		return;
	prop->DoSetValue(vector4.w);
}
//-----------------------------------------------------------------------
const Ogre::Vector4& PropertyWindow::doGetVector4(const wxString& name, Ogre::Vector4& vector4)
{
	// Child accessable with name "parent.string"
	wxString baseName = name + wxT(".") + name;
	wxPGProperty* prop = GetPropertyPtr(baseName + PRNL_X);
	if (!prop)
		return Ogre::Vector4::ZERO;
	vector4.x = prop->DoGetValue().GetDouble();
	prop = GetPropertyPtr(baseName + PRNL_Y);
	if (!prop)
		return Ogre::Vector4::ZERO;
	vector4.y = prop->DoGetValue().GetDouble();
	prop = GetPropertyPtr(baseName + PRNL_Z);
	if (!prop)
		return Ogre::Vector4::ZERO;
	vector4.z = prop->DoGetValue().GetDouble();
	prop = GetPropertyPtr(baseName + PRNL_W);
	if (!prop)
		return Ogre::Vector4::ZERO;
	vector4.w = prop->DoGetValue().GetDouble();
	return vector4;
}
//-----------------------------------------------------------------------
void PropertyWindow::appendQuaternion(const wxString& label, const wxString& name, const Ogre::Quaternion& quaternion)
{
	wxPGId pid = Append(wxParentProperty(label, name));
	wxPGId wid = AppendIn(pid, wxFloatProperty(name + PRNL_W, name + PRNL_W, quaternion.w));
	SetPropertyEditor(wid, wxPG_EDITOR(SpinCtrl));
	wxPGId xid = AppendIn(pid, wxFloatProperty(name + PRNL_X, name + PRNL_X, quaternion.x));
	SetPropertyEditor(xid, wxPG_EDITOR(SpinCtrl));
	wxPGId yid = AppendIn(pid, wxFloatProperty(name + PRNL_Y, name + PRNL_Y, quaternion.y));
	SetPropertyEditor(yid, wxPG_EDITOR(SpinCtrl));
	wxPGId zid = AppendIn(pid, wxFloatProperty(name + PRNL_Z, name + PRNL_Z, quaternion.z));
	SetPropertyEditor(zid, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetQuaternion(const wxString& name, const Ogre::Quaternion& quaternion)
{
	wxString baseName = name + wxT(".") + name;
	wxPGProperty* prop = GetPropertyPtr(baseName + PRNL_W);
	if (!prop)
		return;
	prop->DoSetValue(quaternion.w);
	prop = GetPropertyPtr(baseName + PRNL_X);
	if (!prop)
		return;
	prop->DoSetValue(quaternion.x);
	prop = GetPropertyPtr(baseName + PRNL_Y);
	if (!prop)
		return;
	prop->DoSetValue(quaternion.y);
	prop = GetPropertyPtr(baseName + PRNL_Z);
	if (!prop)
		return;
	prop->DoSetValue(quaternion.z);
}
//-----------------------------------------------------------------------
const Ogre::Quaternion& PropertyWindow::doGetQuaternion(const wxString& name, Ogre::Quaternion& quaternion)
{
	wxString baseName = name + wxT(".") + name;
	wxPGProperty* prop = GetPropertyPtr(baseName + PRNL_W);
	if (!prop)
		return Ogre::Quaternion::IDENTITY;
	quaternion.w = prop->DoGetValue().GetDouble();
	prop = GetPropertyPtr(baseName + PRNL_X);
	if (!prop)
		return Ogre::Quaternion::IDENTITY;
	quaternion.x = prop->DoGetValue().GetDouble();
	prop = GetPropertyPtr(baseName + PRNL_Y);
	if (!prop)
		return Ogre::Quaternion::IDENTITY;
	quaternion.y = prop->DoGetValue().GetDouble();
	prop = GetPropertyPtr(baseName + PRNL_Z);
	if (!prop)
		return Ogre::Quaternion::IDENTITY;
	quaternion.z = prop->DoGetValue().GetDouble();
	return quaternion;
}
//-----------------------------------------------------------------------
void PropertyWindow::appendDynamicAttribute(const wxString& label, const wxString& name, ParticleUniverse::DynamicAttribute& dynamicAttribute)
{
	// Remove previous property and replace it with the new values
	unsigned int index = 0;
	wxPGId pid = GetPropertyByName(name);
	if (pid.IsOk())
	{
		index = pid.GetPropertyPtr()->GetIndexInParent();
		wxPGId prop = wxPropertyContainerMethods::GetNextSibling(pid); // To avoid ambiguity
		Delete(name);
		if (prop)
		{
			pid = Insert(prop, wxParentProperty(label, name));
		}
		else
		{
			// This means that the Dynamic Attribute was at the end of all properties
			pid = Append(wxParentProperty(label, name));
		}
	}
	else
	{
		pid = Append(wxParentProperty(label, name));
	}

	// Add properties
	wxArrayString types;
	types.Add(DYN_FIXED);
	types.Add(DYN_RANDOM);
	types.Add(DYN_CURVED);
	types.Add(DYN_OSCILLATE);
	//AppendIn(pid, wxEnumProperty(PRNL_TYPE, name + PRNL_TYPE, types));

	// Show properties of certain type
	switch (dynamicAttribute.getType())
	{
		case ParticleUniverse::DynamicAttribute::DAT_FIXED:
		{
			// Show fixed value
			AppendIn(pid, wxEnumProperty(PRNL_TYPE, name + PRNL_TYPE, types));
			ParticleUniverse::DynamicAttributeFixed* df = static_cast<ParticleUniverse::DynamicAttributeFixed*>(&dynamicAttribute);
			wxPGId id = AppendIn(pid, wxFloatProperty(PRNL_VALUE, name + PRNL_VALUE, df->getValue()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
		}
		break;

		case ParticleUniverse::DynamicAttribute::DAT_RANDOM:
		{
			// Show min and max
			AppendIn(pid, wxEnumProperty(PRNL_TYPE, name + PRNL_TYPE, types));
			ParticleUniverse::DynamicAttributeRandom* dr = static_cast<ParticleUniverse::DynamicAttributeRandom*>(&dynamicAttribute);
			wxPGId id = AppendIn(pid, wxFloatProperty(PRNL_MIN_VALUE, name + PRNL_MIN_VALUE, dr->getMin()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
			id = AppendIn(pid, wxFloatProperty(PRNL_MAX_VALUE, name + PRNL_MAX_VALUE, dr->getMax()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
		}
		break;

		case ParticleUniverse::DynamicAttribute::DAT_CURVED:
		{
			EnumPropertyWithButton* enumProperty = new EnumPropertyWithButton(PRNL_TYPE, name + PRNL_TYPE, types);
			wxPGId id = AppendIn(pid, enumProperty);
			SetPropertyEditor(id, wxPG_EDITOR(ChoiceAndButton)); // Add a button

			// Copy controlpoints from dynamicAttribute to internal mControlPointListMap
			ParticleUniverse::DynamicAttributeCurved* dynCurved = static_cast<ParticleUniverse::DynamicAttributeCurved*>(&dynamicAttribute);
			ParticleUniverse::DynamicAttributeCurved::ControlPointList list = dynCurved->getControlPoints();
			mControlPointListMap.insert(std::make_pair(name, list));

			// Copy linear / spline interpolation
			ParticleUniverse::InterpolationType type = dynCurved->getInterpolationType();
			mInterpolationTypeMap.insert(std::make_pair(name, type));
		}
		break;

		case ParticleUniverse::DynamicAttribute::DAT_OSCILLATE:
		{
			// Show oscillation type, frequency, phase, base and amplitude
			AppendIn(pid, wxEnumProperty(PRNL_TYPE, name + PRNL_TYPE, types));
			ParticleUniverse::DynamicAttributeOscillate* dosc = static_cast<ParticleUniverse::DynamicAttributeOscillate*>(&dynamicAttribute);
			wxArrayString oscillationTypes;
			oscillationTypes.Add(PRNL_OSC_SINE);
			oscillationTypes.Add(PRNL_OSC_SQUARE);
			AppendIn(pid, wxEnumProperty(PRNL_OSC_TYPE, PRNL_OSC_TYPE, oscillationTypes));
			wxPGId id = AppendIn(pid, wxFloatProperty(PRNL_OSC_FREQUENCY, name + PRNL_OSC_FREQUENCY, dosc->getFrequency()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
			id = AppendIn(pid, wxFloatProperty(PRNL_OSC_PHASE, name + PRNL_OSC_PHASE, dosc->getPhase()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
			id = AppendIn(pid, wxFloatProperty(PRNL_OSC_BASE, name + PRNL_OSC_BASE, dosc->getBase()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
			id = AppendIn(pid, wxFloatProperty(PRNL_OSC_AMPLITUDE, name + PRNL_OSC_AMPLITUDE, dosc->getAmplitude()));
			SetPropertyEditor(id, wxPG_EDITOR(SpinCtrl));
		}
		break;
	}
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetDynamicAttribute(const wxString& name, PropertyWindow* propertyWindow)
{
	// Get the type
	wxString baseName = name + wxT(".") + name;
	wxPGProperty* propFrom = propertyWindow->GetPropertyPtr(baseName + PRNL_TYPE);
	wxPGProperty* propTo = GetPropertyPtr(baseName + PRNL_TYPE);
	if (propFrom && propTo)
	{
		propTo->DoSetValue(propFrom->DoGetValue());
	}
	else
	{
		return;
	}

	if (propFrom->GetValueAsString() == DYN_FIXED)
	{
		// Be sure that both types are the same, so replace the existing one
		ParticleUniverse::DynamicAttributeFixed dynAttr;
		appendDynamicAttribute(name, name, dynAttr);
		propTo = GetPropertyPtr(baseName + PRNL_TYPE);
		if (!propTo) return;
		propTo->SetValueFromString(DYN_FIXED);

		propFrom = propertyWindow->GetPropertyPtr(baseName + PRNL_VALUE);
		propTo = GetPropertyPtr(baseName + PRNL_VALUE);
		if (propFrom && propTo)
		{
			propTo->DoSetValue(propFrom->DoGetValue());
		}
	}
	else if (propFrom->GetValueAsString() == DYN_RANDOM)
	{
		// Be sure that both types are the same, so replace the existing one
		ParticleUniverse::DynamicAttributeRandom dynAttr;
		appendDynamicAttribute(name, name, dynAttr);
		propTo = GetPropertyPtr(baseName + PRNL_TYPE);
		if (!propTo) return;
		propTo->SetValueFromString(DYN_RANDOM);

		propFrom = propertyWindow->GetPropertyPtr(baseName + PRNL_MIN_VALUE);
		propTo = GetPropertyPtr(baseName + PRNL_MIN_VALUE);
		if (propFrom && propTo)
		{
			propTo->DoSetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyPtr(baseName + PRNL_MAX_VALUE);
		propTo = GetPropertyPtr(baseName + PRNL_MAX_VALUE);
		if (propFrom && propTo)
		{
			propTo->DoSetValue(propFrom->DoGetValue());
		}
	}
	else if (propFrom->GetValueAsString() == DYN_CURVED)
	{
		ParticleUniverse::DynamicAttributeCurved dynAttr;
		appendDynamicAttribute(name, name, dynAttr);
		propTo = GetPropertyPtr(baseName + PRNL_TYPE);
		if (!propTo) return;
		propTo->SetValueFromString(DYN_CURVED);

		// Copy controlpoints from other property window to mControlPointListMap
		mControlPointListMap = propertyWindow->mControlPointListMap;

		// Copy linear / spline interpolation
		mInterpolationTypeMap = propertyWindow->mInterpolationTypeMap;
	}
	else if (propFrom->GetValueAsString() == DYN_OSCILLATE)
	{
		// Be sure that both types are the same, so replace the existing one
		ParticleUniverse::DynamicAttributeOscillate dynAttr;
		appendDynamicAttribute(name, name, dynAttr);
		propTo = GetPropertyPtr(baseName + PRNL_TYPE);
		if (!propTo) return;
		propTo->SetValueFromString(DYN_OSCILLATE);

		propFrom = propertyWindow->GetPropertyPtr(baseName + PRNL_OSC_TYPE);
		propTo = GetPropertyPtr(baseName + PRNL_OSC_TYPE);
		if (propFrom && propTo)
		{
			propTo->DoSetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyPtr(baseName + PRNL_OSC_FREQUENCY);
		propTo = GetPropertyPtr(baseName + PRNL_OSC_FREQUENCY);
		if (propFrom && propTo)
		{
			propTo->DoSetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyPtr(baseName + PRNL_OSC_PHASE);
		propTo = GetPropertyPtr(baseName + PRNL_OSC_PHASE);
		if (propFrom && propTo)
		{
			propTo->DoSetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyPtr(baseName + PRNL_OSC_BASE);
		propTo = GetPropertyPtr(baseName + PRNL_OSC_BASE);
		if (propFrom && propTo)
		{
			propTo->DoSetValue(propFrom->DoGetValue());
		}
		propFrom = propertyWindow->GetPropertyPtr(baseName + PRNL_OSC_AMPLITUDE);
		propTo = GetPropertyPtr(baseName + PRNL_OSC_AMPLITUDE);
		if (propFrom && propTo)
		{
			propTo->DoSetValue(propFrom->DoGetValue());
		}
	}
}
//-----------------------------------------------------------------------
void PropertyWindow::doSetDynamicAttribute(const wxString& name, ParticleUniverse::DynamicAttribute* dynamicAttribute)
{
	if (!dynamicAttribute)
		return;

	wxString baseName = name + wxT(".") + name;
	wxPGProperty* propTo;
	appendDynamicAttribute(name, name, *dynamicAttribute);

	switch (dynamicAttribute->getType())
	{
		case ParticleUniverse::DynamicAttribute::DAT_FIXED:
		{
			propTo = GetPropertyPtr(baseName + PRNL_TYPE);
			propTo->SetValueFromString(DYN_FIXED);
		}
		break;

		case ParticleUniverse::DynamicAttribute::DAT_RANDOM:
		{
			propTo = GetPropertyPtr(baseName + PRNL_TYPE);
			propTo->SetValueFromString(DYN_RANDOM);
		}
		break;

		case ParticleUniverse::DynamicAttribute::DAT_CURVED: 
		{
			// Copy controlpoints from dynamicAttribute to internal mControlPointListMap
			//ParticleUniverse::DynamicAttributeCurved* dynCurved = static_cast<ParticleUniverse::DynamicAttributeCurved*>(dynamicAttribute);
			//ParticleUniverse::DynamicAttributeCurved::ControlPointList list = dynCurved->getControlPoints();
			//mControlPointListMap.insert(std::make_pair(wx2ogre(name), list));
			propTo = GetPropertyPtr(baseName + PRNL_TYPE);
			propTo->SetValueFromString(DYN_CURVED);
		}
		break;

		case ParticleUniverse::DynamicAttribute::DAT_OSCILLATE:
		{
			propTo = GetPropertyPtr(baseName + PRNL_TYPE);
			propTo->SetValueFromString(DYN_OSCILLATE);
		}
		break;
	}
}
//-----------------------------------------------------------------------
EditComponent* PropertyWindow::getOwner(void)
{
	return mOwner;
}
//-----------------------------------------------------------------------
void PropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	// Inform others that a property has been changed.
	notifyPropertyChanged();

	// Handle other changes
	wxString name = event.GetPropertyName();
	wxString label = event.GetPropertyLabel();

	// Perform additional validations
	if (mName.length() > 0 && !_validatePropertyStringNoSpaces(event.GetPropertyPtr(), PRNL_NAME))
		return;

	if (name == PRNL_NAME)
	{
		Ogre::String newName = wx2ogre(event.GetPropertyValueAsString());
		mOwner->notifyAdjustNames(newName); // Must perform this first if the name changes (may not be included in setComponentName())
		mOwner->setComponentName(newName);
		mOwner->setCaption();
	}
	else if (label == PRNL_TYPE)
	{
		// Dynamic attribute changed.
		wxPGProperty* prop = event.GetPropertyPtr()->GetMainParent();
		if (!prop) return;
		name = prop->GetName(); // Reuse of 'name'
		label = prop->GetLabel(); // Reuse of 'label'
		if (event.GetPropertyValueAsString() == DYN_FIXED)
		{
			ParticleUniverse::DynamicAttributeFixed dynAttr;
			appendDynamicAttribute(label, name, dynAttr);
			prop = GetPropertyByName(name); // Property has been replaced, so search it again to get the new pointer
			prop->SetValueFromString(DYN_FIXED);
		}
		else if (event.GetPropertyValueAsString() == DYN_RANDOM)
		{
			ParticleUniverse::DynamicAttributeRandom dynAttr;
			appendDynamicAttribute(label, name, dynAttr);
			prop = GetPropertyByName(name); // Property has been replaced, so search it again to get the new pointer
			prop->SetValueFromString(DYN_RANDOM);
		}
		else if (event.GetPropertyValueAsString() == DYN_CURVED)
		{
			// Show dialog to create a curve with control points
			ParticleUniverse::DynamicAttributeCurved dynAttr;
			appendDynamicAttribute(label, name, dynAttr);
			prop = GetPropertyByName(name); // Property has been replaced, so search it again to get the new pointer
			prop->SetValueFromString(DYN_CURVED);

			// Open dialog
			ControlPointListMap::iterator itCp = mControlPointListMap.find(name);
			ParticleUniverse::DynamicAttributeCurved::ControlPointList list;
			if (itCp != mControlPointListMap.end())
			{
				// Found controlpoint list
				list = itCp->second;
			}
			InterpolationTypeMap::iterator itIt = mInterpolationTypeMap.find(name);
			ParticleUniverse::InterpolationType type = ParticleUniverse::IT_LINEAR;
			if (itIt != mInterpolationTypeMap.end())
			{
				// Found interpolation type
				type = itIt->second;
			}

			wxString s = label + _(" - Curved");
			ControlPointDialog dlg(type, list, this, wxID_ANY, s, wxDefaultPosition, wxSize(800, 640));
			if (dlg.ShowModal() == wxID_OK)
			{
				ParticleUniverse::DynamicAttributeCurved::ControlPointList list = dlg.getControlPointList();
				mControlPointListMap.erase(name);
				mControlPointListMap.insert(std::make_pair(name, list));
				ParticleUniverse::InterpolationType type = dlg.getInterpolationType();
				mInterpolationTypeMap.erase(name);
				mInterpolationTypeMap.insert(std::make_pair(name, type));
			}
		}
		else if (event.GetPropertyValueAsString() == DYN_OSCILLATE)
		{
			ParticleUniverse::DynamicAttributeOscillate dynAttr;
			appendDynamicAttribute(label, name, dynAttr);
			prop = GetPropertyByName(name); // Property has been replaced, so search it again to get the new pointer
			prop->SetValueFromString(DYN_OSCILLATE);
		}
		wxPGId pid = GetPropertyByName(name);
		Expand(pid);
	}
}
//-----------------------------------------------------------------------
void PropertyWindow::notifyPropertyChanged(void)
{
	// Inform the owner
	if (mOwner)
	{
		mOwner->notifyPropertyChanged();
	}
}
//-----------------------------------------------------------------------
void PropertyWindow::copyAttributeToComponent(wxPGProperty* prop, wxString propertyName)
{
	// Nothing to do right now

	return;
}
//-----------------------------------------------------------------------
bool PropertyWindow::_validatePropertyStringNoSpaces(wxPGProperty* prop, const wxString& name)
{
	if (!prop)
		return true;

	if (prop->GetName() == name)
	{
		if (prop->GetValueAsString().Contains(wxT(" ")))
		{
			return _setPropertyError(prop, _("No spaces allowed"));
		}
	}
	return true;
}
//-----------------------------------------------------------------------
bool PropertyWindow::_validatePropertyColourWithAlpha(wxPGProperty* prop, const wxString& name)
{
	return _validatePropertyIntMinMax(prop, name + PRNL_POST_ALPHA, 0, 255);
}
//-----------------------------------------------------------------------
bool PropertyWindow::_validatePropertyIntMinMax(wxPGProperty* prop, const wxString& name, int min, int max)
{
	if (!prop)
		return true;

	if (prop->GetName() == name)
	{
		if (prop->DoGetValue().GetLong() < min || prop->DoGetValue().GetLong() > max)
		{
			return _setPropertyError(prop, _("Value must be between ") + 
				ogre2wx(Ogre::StringConverter::toString(min)) + 
				_(" and ") +
				ogre2wx(Ogre::StringConverter::toString(max)));
		}
	}
	return true;
}
//-----------------------------------------------------------------------
bool PropertyWindow::_validatePropertyIntPositive(wxPGProperty* prop, const wxString& name)
{
	if (!prop)
		return true;

	if (prop->GetName() == name)
	{
		if (prop->DoGetValue().GetLong() < 0L)
		{
			return _setPropertyError(prop, _("Value must be positive"));
		}
	}
	return true;
}
//-----------------------------------------------------------------------
bool PropertyWindow::_validatePropertyFloatMinMax(wxPGProperty* prop, const wxString& name, float min, float max)
{
	if (!prop)
		return true;

	if (prop->GetName() == name)
	{
		if (prop->DoGetValue().GetDouble() < min || prop->DoGetValue().GetDouble() > max)
		{
			return _setPropertyError(prop, _("Value must be between ") + 
				ogre2wx(Ogre::StringConverter::toString(min)) + 
				_("and ") +
				ogre2wx(Ogre::StringConverter::toString(max)));
		}
	}
	return true;
}
//-----------------------------------------------------------------------
bool PropertyWindow::_validatePropertyFloatPositive(wxPGProperty* prop, const wxString& name)
{
	if (!prop)
		return true;

	if (prop->GetName() == name)
	{
		if (prop->DoGetValue().GetDouble() < 0.0f)
		{
			return _setPropertyError(prop, _("Value must be positive"));
		}
	}
	return true;
}
//-----------------------------------------------------------------------
bool PropertyWindow::_validatePropertyDynamicAttribute(wxPGProperty* prop, const wxString& name)
{
	if (!prop)
		return true;

	// Validate on positive values
	if (!_validatePropertyFloatPositive(prop, name + PRNL_VALUE))
		return false;
	if (!_validatePropertyFloatPositive(prop, name + PRNL_MIN_VALUE))
		return false;
	if (!_validatePropertyFloatPositive(prop, name + PRNL_MAX_VALUE))
		return false;

	return true;
}
//-----------------------------------------------------------------------
bool PropertyWindow::_setPropertyError(wxPGProperty* prop, const wxString& message)
{
	_showMessage(_("'") + prop->GetValueAsString() + _("': ") + message);
	prop->SetValueToUnspecified();
	SelectProperty(prop->GetId(), true);
	return false;
}
//-----------------------------------------------------------------------
void PropertyWindow::_showMessage(const wxString& message, long style)
{
	wxMessageDialog m(this, message, _("Message"), style);
	m.ShowModal();
}
//-----------------------------------------------------------------------
ParticleUniverse::DynamicAttribute* PropertyWindow::copyValuesToDynamicAttribute(wxString name, wxPGProperty* prop, wxString baseName, ParticleUniverse::DynamicAttribute* dynAttr)
{
	if (name == baseName + PRNL_TYPE)
	{
		// Change the type of dynamic attribute. Note that the prop that is passed is invalid. because it has been changed. So search it again.
		prop = GetPropertyByName(baseName + wxT(".") + name);
		if (prop)
		{
			wxString type = prop->GetValueAsString();
			ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
			if (type == DYN_FIXED)
			{
				return particleSystemManager->createDynamicAttribute(ParticleUniverse::DynamicAttribute::DAT_FIXED);
			}
			else if (type == DYN_RANDOM)
			{
				return particleSystemManager->createDynamicAttribute(ParticleUniverse::DynamicAttribute::DAT_RANDOM);
			}
			else if (type == DYN_CURVED)
			{	
				// This one does not have properties, because all editting is done in the dialog
				ParticleUniverse::DynamicAttributeCurved* dynCurved = 
					static_cast<ParticleUniverse::DynamicAttributeCurved*>(particleSystemManager->createDynamicAttribute(ParticleUniverse::DynamicAttribute::DAT_CURVED));

				// Copy the interpolation type
				InterpolationTypeMap::iterator it = mInterpolationTypeMap.find(baseName);
				ParticleUniverse::InterpolationType type = ParticleUniverse::IT_LINEAR;
				if (it != mInterpolationTypeMap.end())
				{
					// Found interpolation type
					type = it->second;
				}
				dynCurved->setInterpolationType(type);

				// Copy controlpoints and process them
				copyControlPointsToDynamicAttribute(baseName, dynCurved);
				return dynCurved;
			}
			else if (type == DYN_OSCILLATE)
			{
				return particleSystemManager->createDynamicAttribute(ParticleUniverse::DynamicAttribute::DAT_OSCILLATE);
			}
		}
	}
	else if (name == baseName + PRNL_VALUE)
	{
		// Change the value of the fixed dyn attr.
		ParticleUniverse::DynamicAttributeFixed* dynFixed = static_cast<ParticleUniverse::DynamicAttributeFixed*>(dynAttr);
		dynFixed->setValue(prop->DoGetValue().GetDouble());
	}
	else if (name == baseName + PRNL_MIN_VALUE)
	{
		// Change the value of the random dyn attr.
		ParticleUniverse::DynamicAttributeRandom* dynRandom = static_cast<ParticleUniverse::DynamicAttributeRandom*>(dynAttr);
		dynRandom->setMin(prop->DoGetValue().GetDouble());
	}
	else if (name == baseName + PRNL_MAX_VALUE)
	{
		// Change the value of the random dyn attr.
		ParticleUniverse::DynamicAttributeRandom* dynRandom = static_cast<ParticleUniverse::DynamicAttributeRandom*>(dynAttr);
		dynRandom->setMax(prop->DoGetValue().GetDouble());
	}
	else if (name == baseName + PRNL_OSC_TYPE)
	{
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate* dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate*>(dynAttr);
		wxString type = prop->GetValueAsString();
		if (type == PRNL_OSC_SQUARE)
		{
			dynOscillate->setOscillationType(ParticleUniverse::DynamicAttributeOscillate::OSCT_SQUARE);
		}
		else
		{
			dynOscillate->setOscillationType(ParticleUniverse::DynamicAttributeOscillate::OSCT_SINE);
		}
	}
	else if (name == baseName + PRNL_OSC_FREQUENCY)
	{
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate* dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate*>(dynAttr);
		dynOscillate->setFrequency(prop->DoGetValue().GetDouble());
	}
	else if (name == baseName + PRNL_OSC_PHASE)
	{
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate* dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate*>(dynAttr);
		dynOscillate->setPhase(prop->DoGetValue().GetDouble());
	}
	else if (name == baseName + PRNL_OSC_BASE)
	{
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate* dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate*>(dynAttr);
		dynOscillate->setBase(prop->DoGetValue().GetDouble());
	}
	else if (name == baseName + PRNL_OSC_AMPLITUDE)
	{
		// Change the value of the oscillating dyn attr.
		ParticleUniverse::DynamicAttributeOscillate* dynOscillate = static_cast<ParticleUniverse::DynamicAttributeOscillate*>(dynAttr);
		dynOscillate->setAmplitude(prop->DoGetValue().GetDouble());
	}
	return 0;
}
//-----------------------------------------------------------------------
void PropertyWindow::copyControlPointsToDynamicAttribute(wxString baseName, ParticleUniverse::DynamicAttribute* dynAttr)
{
	// Controlpoints to be copied from internal mControlPointListMap
	if (dynAttr->getType() != ParticleUniverse::DynamicAttribute::DAT_CURVED)
		return;
	
	ParticleUniverse::DynamicAttributeCurved* dynCurved = static_cast<ParticleUniverse::DynamicAttributeCurved*>(dynAttr);
	dynCurved->removeAllControlPoints();
	ControlPointListMap::iterator it = mControlPointListMap.find(baseName);
	ParticleUniverse::DynamicAttributeCurved::ControlPointList list;
	if (it != mControlPointListMap.end())
	{
		list = it->second;
	}

	ParticleUniverse::DynamicAttributeCurved::ControlPointList::iterator itCp;
	ParticleUniverse::DynamicAttributeCurved::ControlPointList::iterator itCpEnd = list.end();
	for (itCp = list.begin(); itCp != itCpEnd; ++itCp)
	{
		dynCurved->addControlPoint((*itCp).x, (*itCp).y);
	}

	dynCurved->processControlPoints();
}
//-----------------------------------------------------------------------
bool PropertyWindow::_mustStopParticleSystem(ParticleUniverse::ParticleSystem* system)
{
	if (!system)
		return false;

	bool wasStarted = false;
	if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED)
	{
		wasStarted = true;
		system->stop();
	}
	return wasStarted;
}
//-----------------------------------------------------------------------
void PropertyWindow::_mustRestartParticleSystem(ParticleUniverse::ParticleSystem* system, bool wasStarted)
{
	if (!system)
		return;

	// Start the system if needed
	if (wasStarted)
	{
		if (system)
		{
			system->start();
		}
	}
}
//-----------------------------------------------------------------------
void PropertyWindow::_unprepare(ParticleUniverse::IElement* element, 
								ParticleUniverse::Particle::ParticleType elementType, 
								ParticleUniverse::Particle::ParticleType unprepareType)
{
	if (!element)
		return;

	ParticleUniverse::ParticleTechnique* technique;
	switch (elementType)
	{
		case ParticleUniverse::Particle::PT_TECHNIQUE:
		{
			technique = static_cast<ParticleUniverse::ParticleTechnique*>(element);
		}
		break;
		case ParticleUniverse::Particle::PT_EMITTER:
		{
			ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(element);
			technique = emitter->getParentTechnique();
		}
		break;
		case ParticleUniverse::Particle::PT_AFFECTOR:
		{
			ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(element);
			technique = affector->getParentTechnique();
		}
		break;
	}

	if (!technique)
	{
		return;
	}

	ParticleUniverse::ParticleSystem* system = technique->getParentSystem();
	bool wasStarted = _mustStopParticleSystem(system);
	switch (unprepareType)
	{
		case  ParticleUniverse::Particle::PT_SYSTEM:
		{
			technique->_unprepareSystem();
		}
		case  ParticleUniverse::Particle::PT_TECHNIQUE:
		{
			technique->_unprepareTechnique();
		}
		break;
		case  ParticleUniverse::Particle::PT_EMITTER:
		{
			technique->_unprepareEmitters();
		}
		break;
		case  ParticleUniverse::Particle::PT_AFFECTOR:
		{
			technique->_unprepareAffectors();
		}
		break;
		case  ParticleUniverse::Particle::PT_VISUAL:
		{
			technique->_unprepareVisualParticles();
		}
		break;
	}
	_mustRestartParticleSystem(system, wasStarted);
}
//-----------------------------------------------------------------------
void PropertyWindow::propertyDump(wxPropertyGrid* propgrid)
{
	if (!propgrid)
		return;

	wxPGId prop;
	Ogre::LogManager* log = Ogre::LogManager::getSingletonPtr();
	log->setLogDetail(Ogre::LL_BOREME);
	log->createLog("Dump.log", true);
	wxPGId propId = propgrid->GetFirstProperty();
	while (propId.IsOk())
	{
		wxPGProperty* prop = propId.GetPropertyPtr();
		wxString s = prop->GetName();
		log->logMessage("Property name" + wx2ogre(s));
		propId = propgrid->GetNextProperty(propId);
	}
	log->destroyLog("Dump.log");
}
//-----------------------------------------------------------------------
const wxString& PropertyWindow::getHelpHtml(void) const
{
	return mHelpHtml;
}