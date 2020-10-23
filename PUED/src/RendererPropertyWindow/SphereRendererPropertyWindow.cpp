/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "RendererPropertyWindow/SphereRendererPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"

//-----------------------------------------------------------------------
SphereRendererPropertyWindow::SphereRendererPropertyWindow(wxWindow* parent, EditComponent* owner) :
	RendererPropertyWindow(parent, owner)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SphereRendererPropertyWindow::SphereRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow) :
	RendererPropertyWindow(rendererPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
SphereRendererPropertyWindow::SphereRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::SphereRenderer* renderer) :
	RendererPropertyWindow(parent, owner)
{
	copyAttributesFromRenderer(renderer);
}
//-----------------------------------------------------------------------
void SphereRendererPropertyWindow::copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	// Update renderer with another attribute
	RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
}
//-----------------------------------------------------------------------
void SphereRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer)
{
	RendererPropertyWindow::copyAttributesFromRenderer(renderer);

	// This renderer doesn't have properties of itself.
}
//-----------------------------------------------------------------------
void SphereRendererPropertyWindow::_initProperties(void)
{
	PRNL_RENDERER_TYPE = _("Renderer type");

	mHelpHtml = wxT("RendererSphere.html");

	// This renderer doesn't have properties of itself.
}