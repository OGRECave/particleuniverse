/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "wx/ogre/utils.h"
#include "EmitterPropertyWindow/MeshSurfaceEmitterPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseMeshSelector.h"

//-----------------------------------------------------------------------
MeshSurfaceEmitterPropertyWindow::MeshSurfaceEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EmitterPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
MeshSurfaceEmitterPropertyWindow::MeshSurfaceEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) :
	EmitterPropertyWindow(emitterPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
MeshSurfaceEmitterPropertyWindow::MeshSurfaceEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::MeshSurfaceEmitter* emitter) :
	EmitterPropertyWindow(parent, owner, emitter->getName())
{
	copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void MeshSurfaceEmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::MeshSurfaceEmitter* meshSurfaceEmitter = static_cast<ParticleUniverse::MeshSurfaceEmitter*>(mOwner->getPUElement());
	if (!meshSurfaceEmitter)
		return;

	if (propertyName == PRNL_MESH_SURFACE_EMITTER_NAME)
	{
		// Update emitter with Mesh name
		MeshProperty* meshProperty = static_cast<MeshProperty*>(prop);
		Ogre::String meshName = meshProperty->getMeshName();
		if (meshName != Ogre::StringUtil::BLANK)
		{
			meshSurfaceEmitter->setMeshName(meshName); // No explicit build needed, because the setMeshName() already rebuilds it.
		}
	}
	else if (propertyName == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION)
	{
		// Update emitter with Distribution
		copyDistributionToMeshSurfaceEmitter(prop, meshSurfaceEmitter);
	}
	else if (propertyName == PRNL_MESH_SURFACE_EMITTER_SCALE + PRNL_X ||
		propertyName == PRNL_MESH_SURFACE_EMITTER_SCALE + PRNL_Y ||
		propertyName == PRNL_MESH_SURFACE_EMITTER_SCALE + PRNL_Z)
	{
		// Update emitter with Scale
		copyScaleToMeshSurfaceEmitter(prop, meshSurfaceEmitter);
	}
	else
	{
		// Update emitter with another attribute
		EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void MeshSurfaceEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

	// Copy properties from emitter to property window
	ParticleUniverse::MeshSurfaceEmitter* meshSurfaceEmitter = static_cast<ParticleUniverse::MeshSurfaceEmitter*>(emitter);

	// Mesh name: Ogre::String
	wxString meshName = ogre2wx(meshSurfaceEmitter->getMeshName());
	doSetString(PRNL_MESH_SURFACE_EMITTER_NAME, meshName);

	// Distribution: List
	wxPGProperty* propTo = GetPropertyPtr(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION);
	ParticleUniverse::MeshInfo::MeshSurfaceDistribution distribution = meshSurfaceEmitter->getDistribution();
	wxString distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE;
	if (distribution == ParticleUniverse::MeshInfo::MSD_HETEROGENEOUS_1)
	{
		distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1;
	}
	else if (distribution == ParticleUniverse::MeshInfo::MSD_HETEROGENEOUS_2)
	{
		distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2;
	}
	else if (distribution == ParticleUniverse::MeshInfo::MSD_HOMOGENEOUS)
	{
		distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS;
	}
	else if (distribution == ParticleUniverse::MeshInfo::MSD_VERTEX)
	{
		distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX;
	}
	propTo->SetValueFromString(distributionString);

	// Scale: Ogre::Vector3
	doSetVector3(PRNL_MESH_SURFACE_EMITTER_SCALE, meshSurfaceEmitter->getScale());
}
//-----------------------------------------------------------------------
void MeshSurfaceEmitterPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");
	PRNL_MESH_SURFACE_EMITTER_NAME = _("Mesh name");
	PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION = _("Mesh surface distribution");
	PRNL_MESH_SURFACE_EMITTER_SCALE = _("Mesh surface scale");
	PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE = _("Edge");
	PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1 = _("Heterogeneous_1");
	PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2 = _("Heterogeneous_2");
	PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS = _("Homogeneus");
	PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX = _("Vertex");

	mHelpHtml = wxT("EmitterMeshSurface.html");

	// Mesh name: Ogre::String
	wxPGId pid = Append(new MeshProperty(PRNL_MESH_SURFACE_EMITTER_NAME, PRNL_MESH_SURFACE_EMITTER_NAME));
	SetPropertyEditor(pid, wxPG_EDITOR(TextCtrlAndButton)); // Add a button

	// Distribution: List
	wxArrayString distribution;
	distribution.Add(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE);
	distribution.Add(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1);
	distribution.Add(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2);
	distribution.Add(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS);
	distribution.Add(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX);
	Append(wxEnumProperty(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION, PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION, distribution));

	// Scale: Ogre::Vector3
	appendVector3(PRNL_MESH_SURFACE_EMITTER_SCALE, PRNL_MESH_SURFACE_EMITTER_SCALE, ParticleUniverse::MeshSurfaceEmitter::DEFAULT_SCALE);
}
//-----------------------------------------------------------------------
void MeshSurfaceEmitterPropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	// Perform additional validations.
	if (!_validatePropertyStringNoSpaces(event.GetPropertyPtr(), PRNL_MESH_SURFACE_EMITTER_NAME))
		return;

	EmitterPropertyWindow::onPropertyChanged(event);
}
//-----------------------------------------------------------------------
void MeshSurfaceEmitterPropertyWindow::copyDistributionToMeshSurfaceEmitter(wxPGProperty* prop, ParticleUniverse::MeshSurfaceEmitter* meshSurfaceEmitter)
{
	wxString distribution = prop->GetValueAsString();
	if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE)
	{
		meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_EDGE);
		meshSurfaceEmitter->build();
	}
	else if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1)
	{
		meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_HETEROGENEOUS_1);
		meshSurfaceEmitter->build();
	}
	else if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2)
	{
		meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_HETEROGENEOUS_2);
		meshSurfaceEmitter->build();
	}
	else if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS)
	{
		meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_HOMOGENEOUS);
		meshSurfaceEmitter->build();
	}
	else if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX)
	{
		meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_VERTEX);
		meshSurfaceEmitter->build();
	}
}
//-----------------------------------------------------------------------
void MeshSurfaceEmitterPropertyWindow::copyScaleToMeshSurfaceEmitter(wxPGProperty* prop, ParticleUniverse::MeshSurfaceEmitter* meshSurfaceEmitter)
{
	Ogre::Vector3 v3;
	v3 = doGetVector3(PRNL_MESH_SURFACE_EMITTER_SCALE, v3);
	meshSurfaceEmitter->setScale(v3);
	meshSurfaceEmitter->build();
}
