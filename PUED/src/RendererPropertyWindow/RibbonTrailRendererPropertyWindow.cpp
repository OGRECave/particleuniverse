/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "RendererPropertyWindow/RibbonTrailRendererPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
RibbonTrailRendererPropertyWindow::RibbonTrailRendererPropertyWindow(wxWindow* parent, EditComponent* owner) :
	RendererPropertyWindow(parent, owner)
{
	_initProperties();
}
//-----------------------------------------------------------------------
RibbonTrailRendererPropertyWindow::RibbonTrailRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow) :
	RendererPropertyWindow(rendererPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
RibbonTrailRendererPropertyWindow::RibbonTrailRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::RibbonTrailRenderer* renderer) :
	RendererPropertyWindow(parent, owner)
{
	copyAttributesFromRenderer(renderer);
}
//-----------------------------------------------------------------------
void RibbonTrailRendererPropertyWindow::copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::RibbonTrailRenderer* renderer = static_cast<ParticleUniverse::RibbonTrailRenderer*>(mOwner->getPUElement());
	if (!renderer)
		return;

	if (propertyName == PRNL_USE_VERTEX_COLOURS)
	{
		// Use Vertex Colours: bool
		renderer->setUseVertexColours(prop->DoGetValue().GetBool());
		ParticleUniverse::ParticleTechnique* technique = renderer->getParentTechnique();
		if (technique)
		{
			renderer->_unprepare(technique);
		}
	}
	else if (propertyName == PRNL_MAX_ELEMENTS)
	{
		// Max Chain Elements: unsigned int
		renderer->setMaxChainElements(prop->DoGetValue().GetLong());
		ParticleUniverse::ParticleTechnique* technique = renderer->getParentTechnique();
		if (technique)
		{
			renderer->_unprepare(technique);
		}
	}
	else if (propertyName == PRNL_TRAIL_LENGTH)
	{
		// Trail Length: ParticleUniverse::Real
		renderer->setTrailLength(prop->DoGetValue().GetDouble());
		ParticleUniverse::ParticleTechnique* technique = renderer->getParentTechnique();
		if (technique)
		{
			renderer->_unprepare(technique);
		}
	}
	else if (propertyName == PRNL_TRAIL_WIDTH)
	{
		// Trail Width: ParticleUniverse::Real
		renderer->setTrailWidth(prop->DoGetValue().GetDouble());
		ParticleUniverse::ParticleTechnique* technique = renderer->getParentTechnique();
		if (technique)
		{
			renderer->_unprepare(technique);
		}
	}
	else if (propertyName == PRNL_RANDOM_INITIAL_COLOUR)
	{
		// Random Initial Colour: bool
		renderer->setRandomInitialColour(prop->DoGetValue().GetBool());
		ParticleUniverse::ParticleTechnique* technique = renderer->getParentTechnique();
		if (technique)
		{
			renderer->_unprepare(technique);
		}
	}
	else if (propertyName == PRNL_INITIAL_COLOUR + PRNL_POST_RGB ||
		propertyName == PRNL_INITIAL_COLOUR + PRNL_POST_ALPHA)
	{
		// Initial Colour: Ogre::Colour
		wxString name = prop->GetParent()->GetName();
		wxColor c;
		c = doGetColourWithAlpha(name, c);
		Ogre::ColourValue colour(((ParticleUniverse::Real)c.Red())/255.0f, ((ParticleUniverse::Real)c.Green())/255.0f, ((ParticleUniverse::Real)c.Blue())/255.0f, ((ParticleUniverse::Real)c.Alpha())/255.0f);
		renderer->setInitialColour(colour);
		ParticleUniverse::ParticleTechnique* technique = renderer->getParentTechnique();
		if (technique)
		{
			renderer->_unprepare(technique);
		}
	}
	else if (propertyName == PRNL_COLOUR_CHANGE + PRNL_POST_RGB ||
		propertyName == PRNL_COLOUR_CHANGE + PRNL_POST_ALPHA)
	{
		// Colour Change: Ogre::Colour
		wxString name = prop->GetParent()->GetName();
		wxColor c;
		c = doGetColourWithAlpha(name, c);
		Ogre::ColourValue colour(((ParticleUniverse::Real)c.Red())/255.0f, ((ParticleUniverse::Real)c.Green())/255.0f, ((ParticleUniverse::Real)c.Blue())/255.0f, ((ParticleUniverse::Real)c.Alpha())/255.0f);
		renderer->setColourChange(colour);
		ParticleUniverse::ParticleTechnique* technique = renderer->getParentTechnique();
		if (technique)
		{
			renderer->_unprepare(technique);
		}
	}
	else
	{
		// Update renderer with another attribute
		RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void RibbonTrailRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer)
{
	RendererPropertyWindow::copyAttributesFromRenderer(renderer);

	// Copy properties from renderer to property window
	ParticleUniverse::RibbonTrailRenderer* ribbonTrailRenderer = static_cast<ParticleUniverse::RibbonTrailRenderer*>(renderer);

	// Use Vertex Colours: bool
	doSetBool(PRNL_USE_VERTEX_COLOURS, ribbonTrailRenderer->isUseVertexColours());

	// Max Chain Elements: unsigned int
	doSetUint16(PRNL_MAX_ELEMENTS, ribbonTrailRenderer->getMaxChainElements());

	// Trail Length: ParticleUniverse::Real
	doSetDouble(PRNL_TRAIL_LENGTH, ribbonTrailRenderer->getTrailLength());

	// Trail Width: ParticleUniverse::Real
	doSetDouble(PRNL_TRAIL_WIDTH, ribbonTrailRenderer->getTrailWidth());

	// Random Initial Colour: bool
	doSetBool(PRNL_RANDOM_INITIAL_COLOUR, ribbonTrailRenderer->isRandomInitialColour());

	// Initial Colour: Ogre::Colour
	wxColour initialColour(255 * ribbonTrailRenderer->getInitialColour().r,
		255 * ribbonTrailRenderer->getInitialColour().g,
		255 * ribbonTrailRenderer->getInitialColour().b,
		255 * ribbonTrailRenderer->getInitialColour().a);
	doSetColourWithAlpha(PRNL_INITIAL_COLOUR, initialColour);

	// Colour Change: Ogre::Colour
	wxColour colourChange(255 * ribbonTrailRenderer->getColourChange().r,
		255 * ribbonTrailRenderer->getColourChange().g,
		255 * ribbonTrailRenderer->getColourChange().b,
		255 * ribbonTrailRenderer->getColourChange().a);
	doSetColourWithAlpha(PRNL_COLOUR_CHANGE, colourChange);
}
//-----------------------------------------------------------------------
void RibbonTrailRendererPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_RENDERER_TYPE = _("Renderer type");
	PRNL_USE_VERTEX_COLOURS = _("Use vertex colours");
	PRNL_MAX_ELEMENTS = _("Maximum elements");
	PRNL_TRAIL_LENGTH = _("Trail length");
	PRNL_TRAIL_WIDTH = _("Trail width");
	PRNL_RANDOM_INITIAL_COLOUR = _("Random initial colour");
	PRNL_INITIAL_COLOUR = _("Initial colour");
	PRNL_COLOUR_CHANGE = _("Colour change");
	PRNL_POST_RGB = _(" RGB");
	PRNL_POST_ALPHA = _(" Alpha");

	mHelpHtml = wxT("RendererRibbonTrail.html");

	// Use Vertex Colours: bool
	Append(wxBoolProperty(PRNL_USE_VERTEX_COLOURS, 
		PRNL_USE_VERTEX_COLOURS, 
		ParticleUniverse::RibbonTrailRenderer::DEFAULT_USE_VERTEX_COLOURS));

	// Max Chain Elements: unsigned int
	Append(wxUIntProperty(PRNL_MAX_ELEMENTS, 
		PRNL_MAX_ELEMENTS, 
		ParticleUniverse::RibbonTrailRenderer::DEFAULT_MAX_ELEMENTS));
	SetPropertyEditor(PRNL_MAX_ELEMENTS, wxPG_EDITOR(SpinCtrl));

	// Trail Length: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TRAIL_LENGTH, PRNL_TRAIL_LENGTH, ParticleUniverse::RibbonTrailRenderer::DEFAULT_LENGTH));
	SetPropertyEditor(PRNL_TRAIL_LENGTH, wxPG_EDITOR(SpinCtrl));

	// Trail Width: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TRAIL_WIDTH, PRNL_TRAIL_WIDTH, ParticleUniverse::RibbonTrailRenderer::DEFAULT_WIDTH));
	SetPropertyEditor(PRNL_TRAIL_WIDTH, wxPG_EDITOR(SpinCtrl));

	// Random Initial Colour: bool
	Append(wxBoolProperty(PRNL_RANDOM_INITIAL_COLOUR, 
		PRNL_RANDOM_INITIAL_COLOUR, 
		ParticleUniverse::RibbonTrailRenderer::DEFAULT_RANDOM_INITIAL_COLOUR));

	// Initial Colour: Ogre::Colour
	wxColour initialColour(ParticleUniverse::RibbonTrailRenderer::DEFAULT_INITIAL_COLOUR.r,
		ParticleUniverse::RibbonTrailRenderer::DEFAULT_INITIAL_COLOUR.g,
		ParticleUniverse::RibbonTrailRenderer::DEFAULT_INITIAL_COLOUR.b,
		ParticleUniverse::RibbonTrailRenderer::DEFAULT_INITIAL_COLOUR.a);
	appendColourWithAlphaProperty(PRNL_INITIAL_COLOUR, PRNL_INITIAL_COLOUR, initialColour);

	// Colour Change: Ogre::Colour
	wxColour colourChange(ParticleUniverse::RibbonTrailRenderer::DEFAULT_COLOUR_CHANGE.r,
		ParticleUniverse::RibbonTrailRenderer::DEFAULT_COLOUR_CHANGE.g,
		ParticleUniverse::RibbonTrailRenderer::DEFAULT_COLOUR_CHANGE.b,
		ParticleUniverse::RibbonTrailRenderer::DEFAULT_COLOUR_CHANGE.a);
	appendColourWithAlphaProperty(PRNL_COLOUR_CHANGE, PRNL_COLOUR_CHANGE, colourChange);
}