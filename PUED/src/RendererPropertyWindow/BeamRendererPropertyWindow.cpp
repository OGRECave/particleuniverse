/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "RendererPropertyWindow/BeamRendererPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
BeamRendererPropertyWindow::BeamRendererPropertyWindow(wxWindow* parent, EditComponent* owner) :
	RendererPropertyWindow(parent, owner)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BeamRendererPropertyWindow::BeamRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow) :
	RendererPropertyWindow(rendererPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BeamRendererPropertyWindow::BeamRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::BeamRenderer* renderer) :
	RendererPropertyWindow(parent, owner)
{
	copyAttributesFromRenderer(renderer);
}
//-----------------------------------------------------------------------
void BeamRendererPropertyWindow::copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::BeamRenderer* renderer = static_cast<ParticleUniverse::BeamRenderer*>(mOwner->getPUElement());
	if (!renderer)
		return;

	if (propertyName == PRNL_USE_VERTEX_COLOURS)
	{
		// Use Vertex Colours: bool
		renderer->setUseVertexColours(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_MAX_ELEMENTS)
	{
		// Max Elements: size_t
		renderer->setMaxChainElements(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_UPDATE_INTERVAL)
	{
		// Update Interval: ParticleUniverse::Real
		renderer->setUpdateInterval(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_DEVIATION)
	{
		// Deviation: ParticleUniverse::Real
		renderer->setDeviation(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_NUMBER_OF_SEGMENTS)
	{
		// Number Of Segments: size_t
		renderer->setNumberOfSegments(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_JUMP)
	{
		// Jump: size_t
		renderer->setJump(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_TEXTURE_DIRECTION)
	{
		// Texture Direction: List
		wxString textureDirection = prop->GetValueAsString();
		if (textureDirection == TEXTURE_DIRECTION_U)
		{
			renderer->setTexCoordDirection(Ogre::BillboardChain::TCD_U);
		}
		else if (textureDirection == TEXTURE_DIRECTION_V)
		{
			renderer->setTexCoordDirection(Ogre::BillboardChain::TCD_V);
		}
	}
	else
	{
		// Update renderer with another attribute
		RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void BeamRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer)
{
	RendererPropertyWindow::copyAttributesFromRenderer(renderer);

	// Copy properties from renderer to property window
	ParticleUniverse::BeamRenderer* beamRenderer = static_cast<ParticleUniverse::BeamRenderer*>(renderer);

	// Use Vertex Colours: bool
	doSetBool(PRNL_USE_VERTEX_COLOURS, beamRenderer->isUseVertexColours());

	// Max Elements: size_t
	doSetLong(PRNL_MAX_ELEMENTS, beamRenderer->getMaxChainElements());

	// Update Interval: ParticleUniverse::Real
	doSetDouble(PRNL_UPDATE_INTERVAL, beamRenderer->getUpdateInterval());

	// Deviation: ParticleUniverse::Real
	doSetDouble(PRNL_DEVIATION, beamRenderer->getDeviation());

	// Number Of Segments: size_t
	doSetLong(PRNL_NUMBER_OF_SEGMENTS, beamRenderer->getNumberOfSegments());

	// Number Of Segments: size_t
	doSetBool(PRNL_JUMP, beamRenderer->isJump());

	// Accurate Facing: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_TEXTURE_DIRECTION);
	wxString textureDirection = TEXTURE_DIRECTION_U;
	if (beamRenderer->getTexCoordDirection() == Ogre::BillboardChain::TCD_V)
	{
		textureDirection = TEXTURE_DIRECTION_V;
	}
	propTo->SetValueFromString(textureDirection);
}
//-----------------------------------------------------------------------
void BeamRendererPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_RENDERER_TYPE = _("Renderer type");
	PRNL_USE_VERTEX_COLOURS = _("Use vertex colours");
	PRNL_UPDATE_INTERVAL = _("Update interval");
	PRNL_MAX_ELEMENTS = _("Maximum elements");
	PRNL_DEVIATION = _("Deviation");
	PRNL_NUMBER_OF_SEGMENTS = _("Number of segments");
	PRNL_JUMP = _("Jump");
	PRNL_TEXTURE_DIRECTION = _("Texture direction");
	TEXTURE_DIRECTION_U = _("TCD_U");
	TEXTURE_DIRECTION_V = _("TCD_V");

	mHelpHtml = wxT("RendererBeam.html");

	// Use Vertex Colours: bool
	Append(wxBoolProperty(PRNL_USE_VERTEX_COLOURS, 
		PRNL_USE_VERTEX_COLOURS, 
		ParticleUniverse::BeamRenderer::DEFAULT_USE_VERTEX_COLOURS));

	// Max Elements: size_t
	Append(wxUIntProperty(PRNL_MAX_ELEMENTS, 
		PRNL_MAX_ELEMENTS, 
		ParticleUniverse::BeamRenderer::DEFAULT_MAX_ELEMENTS));
	SetPropertyEditor(PRNL_MAX_ELEMENTS, wxPG_EDITOR(SpinCtrl));

	// Update Interval: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_UPDATE_INTERVAL, 
		PRNL_UPDATE_INTERVAL, 
		ParticleUniverse::BeamRenderer::DEFAULT_UPDATE_INTERVAL));
	SetPropertyEditor(PRNL_UPDATE_INTERVAL, wxPG_EDITOR(SpinCtrl));

	// Deviation: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_DEVIATION, 
		PRNL_DEVIATION, 
		ParticleUniverse::BeamRenderer::DEFAULT_DEVIATION));
	SetPropertyEditor(PRNL_DEVIATION, wxPG_EDITOR(SpinCtrl));

	// Number Of Segments: size_t
	Append(wxUIntProperty(PRNL_NUMBER_OF_SEGMENTS, 
		PRNL_NUMBER_OF_SEGMENTS, 
		ParticleUniverse::BeamRenderer::DEFAULT_NUMBER_OF_SEGMENTS));
	SetPropertyEditor(PRNL_NUMBER_OF_SEGMENTS, wxPG_EDITOR(SpinCtrl));

	// Jump: bool
	Append(wxBoolProperty(PRNL_JUMP, PRNL_JUMP, false));

	// Texture Direction: List
	mTextureDirection.Add(TEXTURE_DIRECTION_U);
	mTextureDirection.Add(TEXTURE_DIRECTION_V);
	wxPGProperty* prop = wxEnumProperty(PRNL_TEXTURE_DIRECTION, PRNL_TEXTURE_DIRECTION, mTextureDirection);
	Append(prop);
	prop->SetValueFromString(TEXTURE_DIRECTION_V);

}