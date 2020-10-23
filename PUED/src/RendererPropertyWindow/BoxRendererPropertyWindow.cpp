/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "RendererPropertyWindow/BoxRendererPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
BoxRendererPropertyWindow::BoxRendererPropertyWindow(wxWindow* parent, EditComponent* owner) :
	RendererPropertyWindow(parent, owner)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BoxRendererPropertyWindow::BoxRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow) :
	RendererPropertyWindow(rendererPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
BoxRendererPropertyWindow::BoxRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::BoxRenderer* renderer) :
	RendererPropertyWindow(parent, owner)
{
	copyAttributesFromRenderer(renderer);
}
//-----------------------------------------------------------------------
void BoxRendererPropertyWindow::copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update renderer with another attribute
	RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
}
//-----------------------------------------------------------------------
void BoxRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer)
{
	RendererPropertyWindow::copyAttributesFromRenderer(renderer);

	// This renderer doesn't have properties of itself.
}
//-----------------------------------------------------------------------
void BoxRendererPropertyWindow::_initProperties(void)
{
	PRNL_RENDERER_TYPE = _("Renderer type");

	mHelpHtml = wxT("RendererBox.html");

	// This renderer doesn't have properties of itself.
}