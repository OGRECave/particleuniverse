/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "RendererPropertyWindow/LightRendererPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
LightRendererPropertyWindow::LightRendererPropertyWindow(wxWindow* parent, EditComponent* owner) :
	RendererPropertyWindow(parent, owner)
{
	_initProperties();
}
//-----------------------------------------------------------------------
LightRendererPropertyWindow::LightRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow) :
	RendererPropertyWindow(rendererPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
LightRendererPropertyWindow::LightRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::LightRenderer* renderer) :
	RendererPropertyWindow(parent, owner)
{
	copyAttributesFromRenderer(renderer);
}
//-----------------------------------------------------------------------
void LightRendererPropertyWindow::copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::LightRenderer* renderer = static_cast<ParticleUniverse::LightRenderer*>(mOwner->getPUElement());
	if (!renderer)
		return;

	if (propertyName == PRNL_LIGHT_TYPE)
	{
		// Light Type: List
		wxString lightType = prop->GetValueAsString();
		if (lightType == LT_POINT)
		{
			renderer->setLightType(Ogre::Light::LT_POINT);
		}
		else if (lightType == LT_SPOT)
		{
			renderer->setLightType(Ogre::Light::LT_SPOTLIGHT);
		}
	}
	else if (propertyName == PRNL_SPECULAR_COLOUR + PRNL_POST_RGB ||
		propertyName == PRNL_SPECULAR_COLOUR + PRNL_POST_ALPHA)
	{
		// Specular Colour: Ogre::Colour
		wxString name = prop->GetParent()->GetName();
		wxColor c;
		c = doGetColourWithAlpha(name, c);
		Ogre::ColourValue colour(((ParticleUniverse::Real)c.Red())/255.0f, ((ParticleUniverse::Real)c.Green())/255.0f, ((ParticleUniverse::Real)c.Blue())/255.0f, ((ParticleUniverse::Real)c.Alpha())/255.0f);
		renderer->setSpecularColour(colour);
	}
	else if (propertyName == PRNL_ATT_RANGE)
	{
		// Attenuation Range: ParticleUniverse::Real
		renderer->setAttenuationRange(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_ATT_CONSTANT)
	{
		// Attenuation Constant: ParticleUniverse::Real
		renderer->setAttenuationConstant(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_ATT_LINEAR)
	{
		// Attenuation Linear: ParticleUniverse::Real
		renderer->setAttenuationLinear(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_ATT_QUADRATIC)
	{
		// Attenuation Quadratic: ParticleUniverse::Real
		renderer->setAttenuationQuadratic(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_SPOT_INNER_ANGLE)
	{
		// Spotlight Inner Angle: ParticleUniverse::Real
		renderer->setSpotlightInnerAngle(Ogre::Radian(Ogre::Degree(prop->DoGetValue().GetDouble())));
	}
	else if (propertyName == PRNL_SPOT_OUTER_ANGLE)
	{
		// Spotlight Outer Angle: ParticleUniverse::Real
		renderer->setSpotlightOuterAngle(Ogre::Radian(Ogre::Degree(prop->DoGetValue().GetDouble())));
	}
	else if (propertyName == PRNL_FALLOFF)
	{
		// Falloff: ParticleUniverse::Real
		renderer->setSpotlightFalloff(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_POWER_SCALE)
	{
		// Power Scale: ParticleUniverse::Real
		renderer->setPowerScale(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_FLASH_FREQUENCY)
	{
		// Flash Frequency: ParticleUniverse::Real
		renderer->setFlashFrequency(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_FLASH_LENGTH)
	{
		// Flash Length: ParticleUniverse::Real
		renderer->setFlashLength(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_FLASH_RANDOM)
	{
		// Flash Random: bool
		renderer->setFlashRandom(prop->DoGetValue().GetBool());
	}
	else
	{
		// Update renderer with another attribute
		RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void LightRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer)
{
	RendererPropertyWindow::copyAttributesFromRenderer(renderer);

	// Copy properties from renderer to property window
	ParticleUniverse::LightRenderer* lightRenderer = static_cast<ParticleUniverse::LightRenderer*>(renderer);

	// Light Type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_LIGHT_TYPE);
	Ogre::Light::LightTypes lightType = lightRenderer->getLightType();
	wxString lightTypeString = LT_POINT;
	if (lightType == Ogre::Light::LT_SPOTLIGHT)
	{
		lightTypeString = LT_SPOT;
	}
	propTo->SetValueFromString(lightTypeString);

	// Specular Colour: Ogre::Colour
	wxColour specularColour(255 * lightRenderer->getSpecularColour().r,
		255 * lightRenderer->getSpecularColour().g,
		255 * lightRenderer->getSpecularColour().b,
		255 * lightRenderer->getSpecularColour().a);
	doSetColourWithAlpha(PRNL_SPECULAR_COLOUR, specularColour);

	// Attenuation Range: ParticleUniverse::Real
	doSetDouble(PRNL_ATT_RANGE, lightRenderer->getAttenuationRange());

	// Attenuation Constant: ParticleUniverse::Real
	doSetDouble(PRNL_ATT_CONSTANT, lightRenderer->getAttenuationConstant());

	// Attenuation Linear: ParticleUniverse::Real
	doSetDouble(PRNL_ATT_LINEAR, lightRenderer->getAttenuationLinear());

	// Attenuation Quadratic: ParticleUniverse::Real
	doSetDouble(PRNL_ATT_QUADRATIC, lightRenderer->getAttenuationQuadratic());

	// Spotlight Inner Angle: ParticleUniverse::Real
	doSetDouble(PRNL_SPOT_INNER_ANGLE, lightRenderer->getSpotlightInnerAngle().valueDegrees());

	// Spotlight Outer Angle: ParticleUniverse::Real
	doSetDouble(PRNL_SPOT_OUTER_ANGLE, lightRenderer->getSpotlightOuterAngle().valueDegrees());

	// Falloff: ParticleUniverse::Real
	doSetDouble(PRNL_FALLOFF, lightRenderer->getSpotlightFalloff());

	// Power Scale: ParticleUniverse::Real
	doSetDouble(PRNL_POWER_SCALE, lightRenderer->getPowerScale());

	// Flash Frequency: ParticleUniverse::Real
	doSetDouble(PRNL_FLASH_FREQUENCY, lightRenderer->getFlashFrequency());

	// Flash Length: ParticleUniverse::Real
	doSetDouble(PRNL_FLASH_LENGTH, lightRenderer->getFlashLength());

	// Flash Length: ParticleUniverse::Real
	doSetBool(PRNL_FLASH_RANDOM, lightRenderer->isFlashRandom());
}
//-----------------------------------------------------------------------
void LightRendererPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_RENDERER_TYPE = _("Renderer type");
	PRNL_LIGHT_TYPE = _("Light type");
	LT_POINT = _("Point");
	LT_SPOT = _("Spot");
	PRNL_ATT_RANGE = _("Attenuation range");
	PRNL_ATT_CONSTANT = _("Attenuation constant");
	PRNL_ATT_LINEAR = _("Attenuation linear");
	PRNL_ATT_QUADRATIC = _("Attenuation quadratic");
	PRNL_SPOT_INNER_ANGLE = _("spot inner angle");
	PRNL_SPOT_OUTER_ANGLE = _("spot outer angle");
	PRNL_FALLOFF = _("Falloff");
	PRNL_POWER_SCALE = _("Powerscale");
	PRNL_FLASH_FREQUENCY = _("Flash frequency");
	PRNL_FLASH_LENGTH = _("Flash length");
	PRNL_FLASH_RANDOM = _("Flash random");
	PRNL_SPECULAR_COLOUR = _("Specular");
	PRNL_POST_RGB = _(" RGB");
	PRNL_POST_ALPHA = _(" Alpha");

	mHelpHtml = wxT("RendererLight.html");

	// Light Type: List
	mLightTypes.Add(LT_POINT);
	mLightTypes.Add(LT_SPOT);
	wxPGId pid = Append(wxEnumProperty(PRNL_LIGHT_TYPE, PRNL_LIGHT_TYPE, mLightTypes));

	// Specular Colour: Ogre::Colour
	appendColourWithAlphaProperty(PRNL_SPECULAR_COLOUR, PRNL_SPECULAR_COLOUR, wxColour(255, 255, 255, 255));

	// Attenuation Range: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_ATT_RANGE, PRNL_ATT_RANGE, ParticleUniverse::LightRenderer::DEFAULT_ATT_RANGE));
	SetPropertyEditor(PRNL_ATT_RANGE, wxPG_EDITOR(SpinCtrl));

	// Attenuation Constant: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_ATT_CONSTANT, PRNL_ATT_CONSTANT, ParticleUniverse::LightRenderer::DEFAULT_ATT_CONSTANT));
	SetPropertyEditor(PRNL_ATT_CONSTANT, wxPG_EDITOR(SpinCtrl));

	// Attenuation Linear: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_ATT_LINEAR, PRNL_ATT_LINEAR, ParticleUniverse::LightRenderer::DEFAULT_ATT_LINEAR));
	SetPropertyEditor(PRNL_ATT_LINEAR, wxPG_EDITOR(SpinCtrl));

	// Attenuation Quadratic: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_ATT_QUADRATIC, PRNL_ATT_QUADRATIC, ParticleUniverse::LightRenderer::DEFAULT_ATT_QUADRATIC));
	SetPropertyEditor(PRNL_ATT_QUADRATIC, wxPG_EDITOR(SpinCtrl));

	// Spotlight Inner Angle: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SPOT_INNER_ANGLE, PRNL_SPOT_INNER_ANGLE, 0.0f));
	SetPropertyEditor(PRNL_SPOT_INNER_ANGLE, wxPG_EDITOR(SpinCtrl));

	// Spotlight Outer Angle: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_SPOT_OUTER_ANGLE, PRNL_SPOT_OUTER_ANGLE, 0.0f));
	SetPropertyEditor(PRNL_SPOT_OUTER_ANGLE, wxPG_EDITOR(SpinCtrl));

	// Falloff: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_FALLOFF, PRNL_FALLOFF, ParticleUniverse::LightRenderer::DEFAULT_FALLOFF));
	SetPropertyEditor(PRNL_FALLOFF, wxPG_EDITOR(SpinCtrl));

	// Power Scale: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_POWER_SCALE, PRNL_POWER_SCALE, ParticleUniverse::LightRenderer::DEFAULT_POWER_SCALE));
	SetPropertyEditor(PRNL_POWER_SCALE, wxPG_EDITOR(SpinCtrl));

	// Flash Frequency: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_FLASH_FREQUENCY, PRNL_FLASH_FREQUENCY, 0.0f));
	SetPropertyEditor(PRNL_FLASH_FREQUENCY, wxPG_EDITOR(SpinCtrl));

	// Flash Length: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_FLASH_LENGTH, PRNL_FLASH_LENGTH, 0.0f));
	SetPropertyEditor(PRNL_FLASH_LENGTH, wxPG_EDITOR(SpinCtrl));

	// Flash Random: bool
	Append(wxBoolProperty(PRNL_FLASH_RANDOM, PRNL_FLASH_RANDOM, false));
}