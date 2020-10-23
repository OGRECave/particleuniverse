/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "RendererPropertyWindow/BillboardRendererPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
BillboardRendererPropertyWindow::BillboardRendererPropertyWindow(wxWindow* parent, EditComponent* owner) :
	RendererPropertyWindow(parent, owner)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BillboardRendererPropertyWindow::BillboardRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow) :
	RendererPropertyWindow(rendererPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BillboardRendererPropertyWindow::BillboardRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::BillboardRenderer* renderer) :
	RendererPropertyWindow(parent, owner)
{
	copyAttributesFromRenderer(renderer);
}
//-----------------------------------------------------------------------
void BillboardRendererPropertyWindow::copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::BillboardRenderer* renderer = static_cast<ParticleUniverse::BillboardRenderer*>(mOwner->getPUElement());
	if (!renderer)
		return;

	if (propertyName == PRNL_BILLBOARD_TYPE)
	{
		// Billboard Type: List
		wxString bilboardType = prop->GetValueAsString();
		if (bilboardType == BBT_ORIENTED_COMMON)
		{
			renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_ORIENTED_COMMON);
		}
		else if (bilboardType == BBT_ORIENTED_SELF)
		{
			renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_ORIENTED_SELF);
		}
		else if (bilboardType == BBT_ORIENTED_SHAPE)
		{
			renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_ORIENTED_SHAPE);
		}
		else if (bilboardType == BBT_PERPENDICULAR_COMMON)
		{
			renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_PERPENDICULAR_COMMON);
		}
		else if (bilboardType == BBT_PERPENDICULAR_SELF)
		{
			renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_PERPENDICULAR_SELF);
		}
		else if (bilboardType == BBT_POINT)
		{
			renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_POINT);
		}
	}
	else if (propertyName == PRNL_ORIGIN)
	{
		// Origin: List
		wxString origin = prop->GetValueAsString();
		if (origin == ORG_BOTTOM_CENTER)
		{
			renderer->setBillboardOrigin(Ogre::BBO_BOTTOM_CENTER);
		}
		else if (origin == ORG_BOTTOM_LEFT)
		{
			renderer->setBillboardOrigin(Ogre::BBO_BOTTOM_LEFT);
		}
		else if (origin == ORG_BOTTOM_RIGHT)
		{
			renderer->setBillboardOrigin(Ogre::BBO_BOTTOM_RIGHT);
		}
		else if (origin == ORG_CENTER)
		{
			renderer->setBillboardOrigin(Ogre::BBO_CENTER);
		}
		else if (origin == ORG_CENTER_LEFT)
		{
			renderer->setBillboardOrigin(Ogre::BBO_CENTER_LEFT);
		}
		else if (origin == ORG_CENTER_RIGHT)
		{
			renderer->setBillboardOrigin(Ogre::BBO_CENTER_RIGHT);
		}
		else if (origin == ORG_TOP_CENTER)
		{
			renderer->setBillboardOrigin(Ogre::BBO_TOP_CENTER);
		}
		else if (origin == ORG_TOP_LEFT)
		{
			renderer->setBillboardOrigin(Ogre::BBO_TOP_LEFT);
		}
		else if (origin == ORG_TOP_RIGHT)
		{
			renderer->setBillboardOrigin(Ogre::BBO_TOP_RIGHT);
		}
	}
	else if (propertyName == PRNL_ROTATION_TYPE)
	{
		// Rotation Type: List
		wxString rotationType = prop->GetValueAsString();
		if (rotationType == RT_VERTEX)
		{
			renderer->setBillboardRotationType(Ogre::BBR_VERTEX);
		}
		else if (rotationType == RT_TEXTURE_COORDINATES)
		{
			renderer->setBillboardRotationType(Ogre::BBR_TEXCOORD);
		}
	}
	else if (propertyName == PRNL_COMMON_DIRECTION + PRNL_X ||
		propertyName == PRNL_COMMON_DIRECTION + PRNL_Y ||
		propertyName == PRNL_COMMON_DIRECTION + PRNL_Z)
	{
		// Common Direction: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_COMMON_DIRECTION, v3);
		renderer->setCommonDirection(v3);
	}
	else if (propertyName == PRNL_UP_VECTOR + PRNL_X ||
		propertyName == PRNL_UP_VECTOR + PRNL_Y ||
		propertyName == PRNL_UP_VECTOR + PRNL_Z)
	{
		// Common Up Vector: Ogre::Vector3
		Ogre::Vector3 v3;
		v3 = doGetVector3(PRNL_UP_VECTOR, v3);
		renderer->setCommonUpVector(v3);
	}
	else if (propertyName == PRNL_POINT_RENDERING)
	{
		// Point Rendering: bool
		renderer->setPointRenderingEnabled(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_ACCURATE_FACING)
	{
		// Accurate Facing: List
		wxString accurateFacing = prop->GetValueAsString();
		if (accurateFacing == ACF_ON)
		{
			renderer->setUseAccurateFacing(true);
		}
		else if (accurateFacing == ACF_OFF)
		{
			renderer->setUseAccurateFacing(false);
		}
	}
	else if (propertyName == PRNL_RENDERER_USE_SOFT_PARTICLES)
	{
		// Use soft particles: Bool
		renderer->setUseSoftParticles(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER)
	{
		// Soft particles contrast power: ParticleUniverse::Real
		renderer->setSoftParticlesContrastPower(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_RENDERER_SOFT_PARTICLES_SCALE)
	{
		// Soft particles scale: ParticleUniverse::Real
		renderer->setSoftParticlesScale(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_RENDERER_SOFT_PARTICLES_DELTA)
	{
		// Soft particles delta: ParticleUniverse::Real
		renderer->setSoftParticlesDelta(prop->DoGetValue().GetDouble());
	}
	else
	{
		// Update renderer with another attribute
		RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void BillboardRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer)
{
	RendererPropertyWindow::copyAttributesFromRenderer(renderer);

	// Copy properties from renderer to property window
	ParticleUniverse::BillboardRenderer* billboardRenderer = static_cast<ParticleUniverse::BillboardRenderer*>(renderer);

	// Billboard Type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_BILLBOARD_TYPE);
	ParticleUniverse::BillboardRenderer::BillboardType billboardType = billboardRenderer->getBillboardType();
	wxString billboardTypeString = BBT_POINT;
	if (billboardType == ParticleUniverse::BillboardRenderer::BBT_ORIENTED_COMMON)
	{
		billboardTypeString = BBT_ORIENTED_COMMON;
	}
	else if (billboardType == ParticleUniverse::BillboardRenderer::BBT_ORIENTED_SELF)
	{
		billboardTypeString = BBT_ORIENTED_SELF;
	}
	else if (billboardType == ParticleUniverse::BillboardRenderer::BBT_PERPENDICULAR_COMMON)
	{
		billboardTypeString = BBT_PERPENDICULAR_COMMON;
	}
	else if (billboardType == ParticleUniverse::BillboardRenderer::BBT_PERPENDICULAR_SELF)
	{
		billboardTypeString = BBT_PERPENDICULAR_SELF;
	}
	else if (billboardType == ParticleUniverse::BillboardRenderer::BBT_ORIENTED_SHAPE)
	{
		billboardTypeString = BBT_ORIENTED_SHAPE;
	}
	propTo->SetValueFromString(billboardTypeString);

	// Origin: List
	propTo = GetPropertyPtr(PRNL_ORIGIN);
	Ogre::BillboardOrigin origin = billboardRenderer->getBillboardOrigin();
	wxString originString = ORG_CENTER;
	if (origin == Ogre::BBO_BOTTOM_CENTER)
	{
		originString = ORG_BOTTOM_CENTER;
	}
	else if (origin == Ogre::BBO_BOTTOM_LEFT)
	{
		originString = ORG_BOTTOM_LEFT;
	}
	else if (origin == Ogre::BBO_BOTTOM_RIGHT)
	{
		originString = ORG_BOTTOM_RIGHT;
	}
	else if (origin == Ogre::BBO_CENTER_LEFT)
	{
		originString = ORG_CENTER_LEFT;
	}
	else if (origin == Ogre::BBO_CENTER_RIGHT)
	{
		originString = ORG_CENTER_RIGHT;
	}
	else if (origin == Ogre::BBO_TOP_CENTER)
	{
		originString = ORG_TOP_CENTER;
	}
	else if (origin == Ogre::BBO_TOP_LEFT)
	{
		originString = ORG_TOP_LEFT;
	}
	else if (origin == Ogre::BBO_TOP_RIGHT)
	{
		originString = ORG_TOP_RIGHT;
	}
	propTo->SetValueFromString(originString);

	// Rotation Type: List
	propTo = GetPropertyPtr(PRNL_ROTATION_TYPE);
	Ogre::BillboardRotationType rotationType = billboardRenderer->getBillboardRotationType();
	wxString rotationTypeString = RT_VERTEX;
	if (rotationType == Ogre::BBR_TEXCOORD)
	{
		rotationTypeString = RT_TEXTURE_COORDINATES;
	}
	propTo->SetValueFromString(rotationTypeString);

	// Common Direction: Ogre::Vector3
	doSetVector3(PRNL_COMMON_DIRECTION, billboardRenderer->getCommonDirection());

	// Common Up Vector: Ogre::Vector3
	doSetVector3(PRNL_UP_VECTOR, billboardRenderer->getCommonUpVector());

	// Point Rendering: bool
	doSetBool(PRNL_POINT_RENDERING, billboardRenderer->isPointRenderingEnabled());

	// Accurate Facing: List
	propTo = GetPropertyPtr(PRNL_ACCURATE_FACING);
	wxString accurateFacingString = ACF_OFF;
	if (billboardRenderer->isUseAccurateFacing())
	{
		accurateFacingString = ACF_ON;
	}
	propTo->SetValueFromString(accurateFacingString);

	// Use soft particles: Bool
	doSetBool(PRNL_RENDERER_USE_SOFT_PARTICLES, renderer->getUseSoftParticles());

	// Soft particles contrast power: ParticleUniverse::Real
	doSetDouble(PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER, renderer->getSoftParticlesContrastPower());

	// Soft particles scale: ParticleUniverse::Real
	doSetDouble(PRNL_RENDERER_SOFT_PARTICLES_SCALE, renderer->getSoftParticlesScale());

	// Soft particles delta: ParticleUniverse::Real
	doSetDouble(PRNL_RENDERER_SOFT_PARTICLES_DELTA, renderer->getSoftParticlesDelta());
}
//-----------------------------------------------------------------------
void BillboardRendererPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_RENDERER_TYPE = _("Renderer type");
	PRNL_BILLBOARD_TYPE = _("Billboard type");
	BBT_POINT = _("Point");
	BBT_ORIENTED_COMMON = _("Oriented Common");
	BBT_ORIENTED_SELF = _("Oriented Self");
	BBT_PERPENDICULAR_COMMON = _("Perpendicular Common");
	BBT_PERPENDICULAR_SELF = _("Perpendicular Self");
	BBT_ORIENTED_SHAPE = _("Oriented Shape");
	PRNL_ORIGIN = _("Origin");
	ORG_TOP_LEFT = _("Top-left");
	ORG_TOP_CENTER = _("Top-center");
	ORG_TOP_RIGHT = _("Top-right");
	ORG_CENTER_LEFT = _("Center-left");
	ORG_CENTER_RIGHT = _("Center-right");
	ORG_CENTER = _("Center");
	ORG_BOTTOM_LEFT = _("Bottom-left");
	ORG_BOTTOM_RIGHT = _("Bottom-right");
	ORG_BOTTOM_CENTER = _("Bottom-center");
	PRNL_ROTATION_TYPE = _("Rotation type");
	RT_VERTEX = _("Vertex");
	RT_TEXTURE_COORDINATES = _("Texture coordinates");
	PRNL_COMMON_DIRECTION = _("Common direction");
	PRNL_UP_VECTOR = _("Up Vector");
	PRNL_POINT_RENDERING = _("Point Rendering");
	PRNL_ACCURATE_FACING = _("Accurate Facing");
	ACF_ON = _("On");
	ACF_OFF = _("Off");
	PRNL_RENDERER_USE_SOFT_PARTICLES = _("Use soft particles");
	PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER = _("Soft particles contrast power");
	PRNL_RENDERER_SOFT_PARTICLES_SCALE = _("Soft particles scale");
	PRNL_RENDERER_SOFT_PARTICLES_DELTA = _("Soft particles delta");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");

	mHelpHtml = wxT("RendererBillboard.html");

	// Billboard Type: List
	mBillboardTypes.Add(BBT_POINT);
	mBillboardTypes.Add(BBT_ORIENTED_COMMON);
	mBillboardTypes.Add(BBT_ORIENTED_SELF);
	mBillboardTypes.Add(BBT_PERPENDICULAR_COMMON);
	mBillboardTypes.Add(BBT_PERPENDICULAR_SELF);
	mBillboardTypes.Add(BBT_ORIENTED_SHAPE);
	wxPGId pid = Append(wxEnumProperty(PRNL_BILLBOARD_TYPE, PRNL_BILLBOARD_TYPE, mBillboardTypes));

	// Origin: List
	mOrigin.Add(ORG_TOP_LEFT);
	mOrigin.Add(ORG_TOP_CENTER);
	mOrigin.Add(ORG_TOP_RIGHT);
	mOrigin.Add(ORG_CENTER_LEFT);
	mOrigin.Add(ORG_CENTER_RIGHT);
	mOrigin.Add(ORG_CENTER);
	mOrigin.Add(ORG_BOTTOM_LEFT);
	mOrigin.Add(ORG_BOTTOM_RIGHT);
	mOrigin.Add(ORG_BOTTOM_CENTER);
	pid = Append(wxEnumProperty(PRNL_ORIGIN, PRNL_ORIGIN, mOrigin));

	// Rotation Type: List
	mRotationTypes.Add(RT_VERTEX);
	mRotationTypes.Add(RT_TEXTURE_COORDINATES);
	pid = Append(wxEnumProperty(PRNL_ROTATION_TYPE, PRNL_ROTATION_TYPE, mRotationTypes));

	// Common Direction: Ogre::Vector3
	appendVector3(PRNL_COMMON_DIRECTION, PRNL_COMMON_DIRECTION, ParticleUniverse::BillboardRenderer::DEFAULT_COMMON_DIRECTION);

	// Common Up Vector: Ogre::Vector3
	appendVector3(PRNL_UP_VECTOR, PRNL_UP_VECTOR, ParticleUniverse::BillboardRenderer::DEFAULT_COMMON_UP_VECTOR);

	// Point Rendering: bool
	Append(wxBoolProperty(PRNL_POINT_RENDERING, PRNL_POINT_RENDERING, ParticleUniverse::BillboardRenderer::DEFAULT_POINT_RENDERING));

	// Accurate Facing: List
	mAccurateFacing.Add(ACF_ON);
	mAccurateFacing.Add(ACF_OFF);
	wxPGProperty* prop = wxEnumProperty(PRNL_ACCURATE_FACING, PRNL_ACCURATE_FACING, mAccurateFacing);
	Append(prop);
	prop->SetValueFromString(ACF_OFF);

	// Note: Although soft particles are generic renderer properties, they currently only work in combination with a billboard renderer.

	// Use soft particles: Bool
	Append(wxBoolProperty(PRNL_RENDERER_USE_SOFT_PARTICLES, 
		PRNL_RENDERER_USE_SOFT_PARTICLES, 
		ParticleUniverse::ParticleRenderer::DEFAULT_USE_SOFT_PARTICLES));

	// Soft particles contrast power: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER, 
		PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER, 
		ParticleUniverse::ParticleRenderer::DEFAULT_SOFT_PARTICLES_CONTRAST_POWER));
	SetPropertyEditor(PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER, wxPG_EDITOR(SpinCtrl));

	// Soft particles scale: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_RENDERER_SOFT_PARTICLES_SCALE, 
		PRNL_RENDERER_SOFT_PARTICLES_SCALE, 
		ParticleUniverse::ParticleRenderer::DEFAULT_SOFT_PARTICLES_SCALE));
	SetPropertyEditor(PRNL_RENDERER_SOFT_PARTICLES_SCALE, wxPG_EDITOR(SpinCtrl));

	// Soft particles delta: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_RENDERER_SOFT_PARTICLES_DELTA, 
		PRNL_RENDERER_SOFT_PARTICLES_DELTA, 
		ParticleUniverse::ParticleRenderer::DEFAULT_SOFT_PARTICLES_DELTA));
	SetPropertyEditor(PRNL_RENDERER_SOFT_PARTICLES_DELTA, wxPG_EDITOR(SpinCtrl));
}