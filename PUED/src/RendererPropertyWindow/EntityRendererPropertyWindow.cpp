/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "RendererPropertyWindow/EntityRendererPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseMeshSelector.h"
#include "wx/ogre/utils.h"

//-----------------------------------------------------------------------
EntityRendererPropertyWindow::EntityRendererPropertyWindow(wxWindow* parent, EditComponent* owner) :
	RendererPropertyWindow(parent, owner)
{
	_initProperties();
}
//-----------------------------------------------------------------------
EntityRendererPropertyWindow::EntityRendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow) :
	RendererPropertyWindow(rendererPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
EntityRendererPropertyWindow::EntityRendererPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::EntityRenderer* renderer) :
	RendererPropertyWindow(parent, owner)
{
	copyAttributesFromRenderer(renderer);
}
//-----------------------------------------------------------------------
void EntityRendererPropertyWindow::copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::EntityRenderer* renderer = static_cast<ParticleUniverse::EntityRenderer*>(mOwner->getPUElement());
	if (!renderer)
		return;

	if (propertyName == PRNL_ORIENTATION_TYPE)
	{
		// Orientation Type: List
		wxString orientationType = prop->GetValueAsString();
		if (orientationType == OTT_ORIENTED_SELF)
		{
			renderer->setEntityOrientationType(ParticleUniverse::EntityRenderer::ENT_ORIENTED_SELF);
		}
		else if (orientationType == OTT_ORIENTED_SELF_MIRRORED)
		{
			renderer->setEntityOrientationType(ParticleUniverse::EntityRenderer::ENT_ORIENTED_SELF_MIRRORED);
		}
		else if (orientationType == OTT_ORIENTED_SHAPE)
		{
			renderer->setEntityOrientationType(ParticleUniverse::EntityRenderer::ENT_ORIENTED_SHAPE);
		}
	}
	else if (propertyName == PRNL_MESH_NAME)
	{
		// Mesh Name: Ogre::String
		MeshProperty* meshProperty = static_cast<MeshProperty*>(prop);
		wxString name = ogre2wx(meshProperty->getMeshName());
		Ogre::String meshName = wx2ogre(name);
		replaceRendererType(GetPropertyPtr(PRNL_RENDERER_TYPE));

		// A bit heavy solution to re-create the renderer, but only changing the meshname is not sufficient
		renderer = static_cast<ParticleUniverse::EntityRenderer*>(mOwner->getPUElement());
		if (!renderer)
			return;

		renderer->setMeshName(meshName);
		meshProperty->SetValueFromString(name);
	}
	else
	{
		// Update renderer with another attribute
		RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void EntityRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer)
{
	RendererPropertyWindow::copyAttributesFromRenderer(renderer);

	// Copy properties from renderer to property window
	ParticleUniverse::EntityRenderer* entityRenderer = static_cast<ParticleUniverse::EntityRenderer*>(renderer);

	// Mesh Name: Ogre::String
	wxString meshName = ogre2wx(entityRenderer->getMeshName());
	doSetString(PRNL_MESH_NAME, meshName);

	// Orientation Type: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_ORIENTATION_TYPE);
	ParticleUniverse::EntityRenderer::EntityOrientationType orientationType = entityRenderer->getEntityOrientationType();
	wxString orientationTypeString = OTT_ORIENTED_SELF;
	if (orientationType == ParticleUniverse::EntityRenderer::ENT_ORIENTED_SELF_MIRRORED)
	{
		orientationTypeString = OTT_ORIENTED_SELF_MIRRORED;
	}
	else if (orientationType == ParticleUniverse::EntityRenderer::ENT_ORIENTED_SHAPE)
	{
		orientationTypeString = OTT_ORIENTED_SHAPE;
	}
	propTo->SetValueFromString(orientationTypeString);
}
//-----------------------------------------------------------------------
void EntityRendererPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_RENDERER_TYPE = _("Renderer type");
	PRNL_ORIENTATION_TYPE = _("Orientation type");
	OTT_ORIENTED_SELF = _("Oriented self");
	OTT_ORIENTED_SELF_MIRRORED = _("Oriented self mirrored");
	OTT_ORIENTED_SHAPE = _("Oriented shape");
	PRNL_MESH_NAME = _("Mesh name");

	mHelpHtml = wxT("RendererEntity.html");

	// Mesh Name: Ogre::String
	wxPGId pid = Append(new MeshProperty(PRNL_MESH_NAME, PRNL_MESH_NAME));
	SetPropertyEditor(pid, wxPG_EDITOR(TextCtrlAndButton)); // Add a button

	// Orientation Type: List
	mOrientationTypes.Add(OTT_ORIENTED_SELF);
	mOrientationTypes.Add(OTT_ORIENTED_SELF_MIRRORED);
	mOrientationTypes.Add(OTT_ORIENTED_SHAPE);
	pid = Append(wxEnumProperty(PRNL_ORIENTATION_TYPE, PRNL_ORIENTATION_TYPE, mOrientationTypes));
}
//-----------------------------------------------------------------------
void EntityRendererPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	// Perform additional validations.

	// V1.5: Bug when selecting an entity
//	wxPGProperty* prop = event.GetPropertyPtr();
//	if (prop->GetName() == PRNL_MESH_NAME)
//	{
//		replaceRendererType(GetPropertyPtr(PRNL_RENDERER_TYPE));
//	} // V1.5

	if (!_validatePropertyStringNoSpaces(event.GetPropertyPtr(), PRNL_MESH_NAME))
		return;

	RendererPropertyWindow::onPropertyChanged(event);
}
