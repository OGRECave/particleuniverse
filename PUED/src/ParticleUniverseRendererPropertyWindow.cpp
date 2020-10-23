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
#include "ParticleUniverseRendererPropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseSystemManager.h"
#include "wx/ogre/utils.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
RendererPropertyWindow::RendererPropertyWindow(wxWindow* parent, EditComponent* owner) : PropertyWindow(parent, owner, Ogre::StringUtil::BLANK)
{
	_initProperties();
}
//-----------------------------------------------------------------------
RendererPropertyWindow::RendererPropertyWindow(RendererPropertyWindow* rendererPropertyWindow) : PropertyWindow(
	rendererPropertyWindow->GetParent(), 
	rendererPropertyWindow->getOwner(),
	Ogre::StringUtil::BLANK)
{
	_initProperties();
	copyAttributesFromPropertyWindow(rendererPropertyWindow);
}
//-----------------------------------------------------------------------
void RendererPropertyWindow::copyAttributesFromPropertyWindow(RendererPropertyWindow* rendererPropertyWindow)
{
	Ogre::Vector3 v;

	// Name: String
	doSetString(PRNL_NAME, rendererPropertyWindow->doGetString(PRNL_NAME));

	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_RENDERER_TYPE);
	wxPGProperty* propFrom = rendererPropertyWindow->GetPropertyPtr(PRNL_RENDERER_TYPE);
	propTo->DoSetValue(propFrom->DoGetValue());

	// Render queue group: ParticleUniverse::uint8
	doSetLong(PRNL_RENDERER_RENDER_Q_GROUP, rendererPropertyWindow->doGetLong(PRNL_RENDERER_RENDER_Q_GROUP));

	// Sorting: Bool
	SetBoolChoices (_("True"), _("False")); // Forces Internationalization
	doSetBool(PRNL_RENDERER_SORTING, rendererPropertyWindow->doGetBool(PRNL_RENDERER_SORTING));

	// Texture coords set: List of 4 x ParticleUniverse::Real
	// Todo

	// Texture coords rows: uchar
	doSetLong(PRNL_RENDERER_TEXCOORDS_ROWS, rendererPropertyWindow->doGetLong(PRNL_RENDERER_TEXCOORDS_ROWS));

	// Texture coords columns: uchar
	doSetLong(PRNL_RENDERER_TEXCOORDS_COLUMNS, rendererPropertyWindow->doGetLong(PRNL_RENDERER_TEXCOORDS_COLUMNS));

	// Note: Enable the part below if soft particles can also be used for other renderer types than billboards (and don't forget to add 
	// soft particle attributes to copyAttributesFromRenderer, _initProperties, ...

	// Use soft particles: Bool
//	doSetBool(PRNL_RENDERER_USE_SOFT_PARTICLES, rendererPropertyWindow->doGetBool(PRNL_RENDERER_USE_SOFT_PARTICLES));

	// Soft particles contrast power: ParticleUniverse::Real
//	doSetDouble(PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER, rendererPropertyWindow->doGetDouble(PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER));

	// Soft particles scale: ParticleUniverse::Real
//	doSetDouble(PRNL_RENDERER_SOFT_PARTICLES_SCALE, rendererPropertyWindow->doGetDouble(PRNL_RENDERER_SOFT_PARTICLES_SCALE));

	// Soft particles delta: ParticleUniverse::Real
//	doSetDouble(PRNL_RENDERER_SOFT_PARTICLES_DELTA, rendererPropertyWindow->doGetDouble(PRNL_RENDERER_SOFT_PARTICLES_DELTA));
}
//-----------------------------------------------------------------------
void RendererPropertyWindow::copyAttributeToRenderer(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ParticleRenderer* renderer = static_cast<ParticleUniverse::ParticleRenderer*>(mOwner->getPUElement());
	if (!renderer)
		return;

	if (propertyName == PRNL_RENDERER_TYPE)
	{
		// Type: List of types
		// This requires the renderer to be replaced.
		replaceRendererType(prop);
	}
	else if (propertyName == PRNL_RENDERER_RENDER_Q_GROUP)
	{
		// Render queue group: ParticleUniverse::uint8
		renderer->setRenderQueueGroup(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_RENDERER_SORTING)
	{
		// Sorting: Bool
		renderer->setSorted(prop->DoGetValue().GetBool());
	}
	else if (propertyName == PRNL_AFFECTOR_SPECIALISATION)
	{
		// Texture coords set: List of 4 x ParticleUniverse::Real
		// Todo
	}
	else if (propertyName == PRNL_RENDERER_TEXCOORDS_ROWS)
	{
		// Texture coords rows: uchar
		renderer->setTextureCoordsRows(prop->DoGetValue().GetLong());
		renderer->_unprepare(renderer->getParentTechnique());
		renderer->setRendererInitialised(false);
	}
	else if (propertyName == PRNL_RENDERER_TEXCOORDS_COLUMNS)
	{
		// Texture coords columns: uchar
		renderer->setTextureCoordsColumns(prop->DoGetValue().GetLong());
		renderer->_unprepare(renderer->getParentTechnique());
		renderer->setRendererInitialised(false);
	}
	else
	{
		PropertyWindow::copyAttributeToComponent(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void RendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer* renderer)
{
	// Type: List of types
	wxPGProperty* propTo = GetPropertyPtr(PRNL_RENDERER_TYPE);
	wxString type = ogre2wxTranslate(renderer->getRendererType());
	propTo->SetValueFromString(type);

	// Render queue group: ParticleUniverse::uint8
	doSetLong(PRNL_RENDERER_RENDER_Q_GROUP, renderer->getRenderQueueGroup());

	// Sorting: Bool
	doSetBool(PRNL_RENDERER_SORTING, renderer->isSorted());

	// Texture coords set: List of 4 x ParticleUniverse::Real
	// Todo

	// Texture coords rows: uchar
	doSetLong(PRNL_RENDERER_TEXCOORDS_ROWS, renderer->getTextureCoordsRows());

	// Texture coords columns: uchar
	doSetLong(PRNL_RENDERER_TEXCOORDS_COLUMNS, renderer->getTextureCoordsColumns());
}
//-----------------------------------------------------------------------
void RendererPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	// Renderers
	CST_RENDERER_BEAM = ogre2wxTranslate(RENDERER_BEAM);
	CST_RENDERER_BILLBOARD = ogre2wxTranslate(RENDERER_BILLBOARD);
	CST_RENDERER_SPHERE = ogre2wxTranslate(RENDERER_SPHERE);
	CST_RENDERER_BOX = ogre2wxTranslate(RENDERER_BOX);
	CST_RENDERER_ENTITY = ogre2wxTranslate(RENDERER_ENTITY);
	CST_RENDERER_LIGHT = ogre2wxTranslate(RENDERER_LIGHT);
	CST_RENDERER_RIBBONTRAIL = ogre2wxTranslate(RENDERER_RIBBONTRAIL);
	PRNL_RENDERER_TYPE = _("Renderer type");
	PRNL_USE_VERTEX_COLOURS = _("Use vertex colours");
	PRNL_MAX_ELEMENTS = _("Maximum elements");
	PRNL_RENDERER_RENDER_Q_GROUP = _("Render queue group");
	PRNL_RENDERER_SORTING = _("Sorting");
	PRNL_RENDERER_TEXCOORDS_DEFINE = _("Texture coords define");
	PRNL_RENDERER_TEXCOORDS_SET = _("Texture coords set");
	PRNL_RENDERER_TEXCOORDS_ROWS = _("Texture coords rows");
	PRNL_RENDERER_TEXCOORDS_COLUMNS = _("Texture coords columns");
	PRNL_RENDERER_USE_SOFT_PARTICLES = _("Use soft particles");
	PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER = _("Soft particles contrast power");
	PRNL_RENDERER_SOFT_PARTICLES_SCALE = _("Soft particles scale");
	PRNL_RENDERER_SOFT_PARTICLES_DELTA = _("Soft particles delta");
	PRNL_NAME = _("Name");
	PRNL_MESH_NAME = _("Mesh name");

	// Type: List of types (DO NOT CHANGE THE ORDER!
	// Internationalization means that the string values of this property are unusable to create a renderer)
	mTypes.Add(CST_RENDERER_BILLBOARD);
	mTypes.Add(CST_RENDERER_BEAM);
	mTypes.Add(CST_RENDERER_BOX);
	mTypes.Add(CST_RENDERER_ENTITY);
	mTypes.Add(CST_RENDERER_LIGHT);
	mTypes.Add(CST_RENDERER_RIBBONTRAIL);
	mTypes.Add(CST_RENDERER_SPHERE);
	wxPGId pid = Append(wxEnumProperty(PRNL_RENDERER_TYPE, PRNL_RENDERER_TYPE, mTypes));

	// Render queue group: ParticleUniverse::uint8
	Append(wxUIntProperty(PRNL_RENDERER_RENDER_Q_GROUP, 
		PRNL_RENDERER_RENDER_Q_GROUP, 
		ParticleUniverse::ParticleRenderer::DEFAULT_RENDER_QUEUE_GROUP));
	SetPropertyEditor(PRNL_RENDERER_RENDER_Q_GROUP, wxPG_EDITOR(SpinCtrl));

	// Sorting: Bool
	SetBoolChoices (_("True"), _("False")); // Forces Internationalization
	Append(wxBoolProperty(PRNL_RENDERER_SORTING, 
		PRNL_RENDERER_SORTING, 
		ParticleUniverse::ParticleRenderer::DEFAULT_SORTED));

	// Texture coords define: Is no attribute, but only a 'container' in the script

	// Texture coords set: List of 4 x ParticleUniverse::Real
	// Todo

	// Texture coords rows: uchar
	Append(wxUIntProperty(PRNL_RENDERER_TEXCOORDS_ROWS, 
		PRNL_RENDERER_TEXCOORDS_ROWS, 
		ParticleUniverse::ParticleRenderer::DEFAULT_TEXTURECOORDS_ROWS));
	SetPropertyEditor(PRNL_RENDERER_TEXCOORDS_ROWS, wxPG_EDITOR(SpinCtrl));

	// Texture coords columns: uchar
	Append(wxUIntProperty(PRNL_RENDERER_TEXCOORDS_COLUMNS, 
		PRNL_RENDERER_TEXCOORDS_COLUMNS, 
		ParticleUniverse::ParticleRenderer::DEFAULT_TEXTURECOORDS_COLUMNS));
	SetPropertyEditor(PRNL_RENDERER_TEXCOORDS_COLUMNS, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void RendererPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	wxPGProperty* prop = event.GetPropertyPtr();
	onParentPropertyChanged(event);
	copyAttributeToRenderer(prop, propertyName);
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void RendererPropertyWindow::onParentPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	PropertyWindow::onPropertyChanged(event);

	if (propertyName == PRNL_RENDERER_TYPE)
	{
		// Replace this window by another one
		wxString subType = event.GetPropertyValueAsString();
		mOwner->createPropertyWindow(subType, this);
		mOwner->setCaption();
		getOwner()->refreshCanvas();
	}
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void RendererPropertyWindow::replaceRendererType(wxPGProperty* prop)
{
	// Type: List of types
	Ogre::String type = getRendererTypeByProperty(prop);
	if (type == Ogre::StringUtil::BLANK)
		return;

	ParticleUniverse::ParticleRenderer* oldRenderer = static_cast<ParticleUniverse::ParticleRenderer*>(mOwner->getPUElement());
	if (oldRenderer)
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleRenderer* newRenderer = particleSystemManager->createRenderer(type);
		oldRenderer->copyParentAttributesTo(newRenderer);
		ParticleUniverse::ParticleTechnique* technique = oldRenderer->getParentTechnique();
		if (technique)
		{
			// V1.5: Bug: Crash when meshname of EntityRenderer is blank
			if (newRenderer->getRendererType() == RENDERER_ENTITY)
			{
				ParticleUniverse::EntityRenderer* entityRenderer = static_cast<ParticleUniverse::EntityRenderer*>(newRenderer);
				ParticleUniverse::String s = entityRenderer->getMeshName();
				if (entityRenderer->getMeshName() == Ogre::StringUtil::BLANK)
				{
					ParticleUniverse::String s = "pu_bold_marker.mesh";
					entityRenderer->setMeshName(s);
					wxPGProperty* prop = GetPropertyPtr(PRNL_MESH_NAME);
					if (prop)
					{
						prop->SetValueFromString(ogre2wx(s));
					}
				}
			}
			// V1.5: Bug end

			bool wasStarted = false;
			ParticleUniverse::ParticleSystem* system = technique->getParentSystem();
			if (system && system->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED)
			{
				wasStarted = true;
				system->stop();
			}
			technique->setRenderer(newRenderer);
			mOwner->setPUElement(newRenderer);
			technique->_unprepareRenderer();
			if (wasStarted)
			{
				system->start();
			}
		}
		else
		{
			/** The old renderer didn't have a technique.
			*/
			particleSystemManager->destroyRenderer(oldRenderer);
			mOwner->setPUElement(newRenderer);
		}
	}
	else
	{
		// There is no old renderer. Create a new renderer by means of the ParticleSystemManager
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		ParticleUniverse::ParticleRenderer* newRenderer = particleSystemManager->createRenderer(type);
		mOwner->setPUElement(newRenderer);
	}
}
//-----------------------------------------------------------------------
const Ogre::String& RendererPropertyWindow::getRendererTypeByProperty(wxPGProperty* prop)
{
	int type = prop->DoGetValue().GetLong(); // The propert must be a list (PRNL_RENDERER_TYPE)
	switch (type)
	{
		case 0:
			return RENDERER_BILLBOARD;
		break;
		case 1:
			return RENDERER_BEAM;
		break;
		case 2:
			return RENDERER_BOX;
		break;
		case 3:
			return RENDERER_ENTITY;
		break;
		case 4:
			return RENDERER_LIGHT;
		break;
		case 5:
			return RENDERER_RIBBONTRAIL;
		break;
		case 6:
			return RENDERER_SPHERE;
		break;
	}

	return Ogre::StringUtil::BLANK;
}