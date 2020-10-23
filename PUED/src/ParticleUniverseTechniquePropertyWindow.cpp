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
#include "ParticleUniverseTechniquePropertyWindow.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseSystemManager.h"
#include "wx/ogre/utils.h"
#include "ParticleUniverseMaterialSelector.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
TechniquePropertyWindow::TechniquePropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) : PropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyAttributesFromTechnique(const ParticleUniverse::ParticleTechnique* technique)
{
	// Name: Ogre::String
	doSetString(PRNL_NAME, ogre2wx(technique->getName()));

	// Enabled: bool
	doSetBool(PRNL_TECHNIQUE_ENABLED, technique->isEnabled());
	
	// Position: Vector3
	doSetVector3(PRNL_TECHNIQUE_POSITION, technique->position);

	// Keep local: bool
	doSetBool(PRNL_TECHNIQUE_KEEP_LOCAL, technique->isKeepLocal());

	// Visual particle quota: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA, technique->getVisualParticleQuota());

	// Emitted emitter quota: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA, technique->getEmittedEmitterQuota());

	// Emitted affector quota: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA, technique->getEmittedAffectorQuota());

	// Emitted technique quota: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA, technique->getEmittedTechniqueQuota());

	// Emitted system quota: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA, technique->getEmittedSystemQuota());

	// Material: Ogre::String
	wxString materialName = ogre2wx(technique->getMaterialName());
	doSetString(PRNL_TECHNIQUE_MATERIAL, materialName);

	// Lod index: unsigned short
	doSetUint16(PRNL_TECHNIQUE_LOD_INDEX, technique->getLodIndex());

	// Default particle width: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH, technique->getDefaultWidth());

	// Default particle height: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT, technique->getDefaultHeight());

	// Default particle depth: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH, technique->getDefaultDepth());

	// Spatial hashing cell dimension: ParticleUniverse::ushort
	doSetUint16(PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION, technique->getSpatialHashingCellDimension());

	// Spatial hashing cell overlap: ParticleUniverse::ushort
	doSetUint16(PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP, technique->getSpatialHashingCellOverlap());

	// Spatial hashtable size: unsigned int
	doSetUint16(PRNL_TECHNIQUE_SPHASHING_SIZE, technique->getSpatialHashTableSize());

	// Spatial hashing update interval: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL, technique->getSpatialHashingInterval());

	// Maximum velocity: ParticleUniverse::Real
	doSetDouble(PRNL_TECHNIQUE_MAX_VELOCITY, technique->getMaxVelocity());
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyAttributeToTechnique(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::ParticleTechnique* technique = static_cast<ParticleUniverse::ParticleTechnique*>(mOwner->getPUElement());
	if (!technique)
		return;

	if (propertyName == PRNL_NAME)
	{
		// Name: String
		Ogre::String name = wx2ogre(prop->GetValueAsString());
		technique->setName(name);
	}
	else if (propertyName == PRNL_TECHNIQUE_ENABLED)
	{
		// Enabled: Bool
		copyEnabledToTechnique(prop, technique);
	}
	else if (propertyName == PRNL_TECHNIQUE_POSITION + PRNL_X ||
		propertyName == PRNL_TECHNIQUE_POSITION + PRNL_Y ||
		propertyName == PRNL_TECHNIQUE_POSITION + PRNL_Z)
	{
		// Position: Vector3
		copyPositionToTechnique(prop, technique);
	}
	else if (propertyName == PRNL_TECHNIQUE_KEEP_LOCAL)
	{
		// Keep local: Bool
		copyKeepLocalToTechnique(prop, technique);
	}
	else if (propertyName == PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA)
	{
		// Visual particle quota: ParticleUniverse::Real
		_unprepare(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_VISUAL);
		copyVisualParticleQuotaToTechnique(prop, technique);
	}
	else if (propertyName == PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA)
	{
		// Emitted emitter quota: ParticleUniverse::Real
		_unprepare(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_EMITTER);
		copyEmittedEmitterQuotaToTechnique(prop, technique);
	}
	else if (propertyName == PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA)
	{
		// Emitted affector quota: ParticleUniverse::Real
		_unprepare(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_AFFECTOR);
		copyEmittedAffectorQuotaToTechnique(prop, technique);
	}
	else if (propertyName == PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA)
	{
		// Emitted technique quota: ParticleUniverse::Real
		_unprepare(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_TECHNIQUE);
		copyEmittedTechniqueQuotaToTechnique(prop, technique);
	}
	else if (propertyName == PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA)
	{
		// Emitted system quota: ParticleUniverse::Real
		_unprepare(technique, ParticleUniverse::Particle::PT_TECHNIQUE, ParticleUniverse::Particle::PT_SYSTEM);
		copyEmittedSystemQuotaToTechnique(prop, technique);
	}
	else if (propertyName == PRNL_TECHNIQUE_MATERIAL)
	{
		MaterialProperty* materialProperty = static_cast<MaterialProperty*>(prop);
		Ogre::String materialName = materialProperty->getMaterialName();
		technique->setMaterialName(materialName);
	}
	else if (propertyName == PRNL_TECHNIQUE_LOD_INDEX)
	{
		// Lod index: unsigned short
		technique->setLodIndex(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH)
	{
		// Default particle width: ParticleUniverse::Real
		technique->setDefaultWidth(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT)
	{
		// Default particle height: ParticleUniverse::Real
		technique->setDefaultHeight(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH)
	{
		// Default particle depth: ParticleUniverse::Real
		technique->setDefaultDepth(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION)
	{
		// Spatial hashing cell dimension: ParticleUniverse::ushort
		technique->setSpatialHashingCellDimension(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP)
	{
		// Spatial hashing cell overlap: ParticleUniverse::ushort
		technique->setSpatialHashingCellOverlap(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_TECHNIQUE_SPHASHING_SIZE)
	{
		// Spatial hashtable size: unsigned int
		technique->setSpatialHashTableSize(prop->DoGetValue().GetLong());
	}
	else if (propertyName == PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL)
	{
		// Spatial hashing update interval: ParticleUniverse::Real
		technique->setSpatialHashingInterval(prop->DoGetValue().GetDouble());
	}
	else if (propertyName == PRNL_TECHNIQUE_MAX_VELOCITY)
	{
		// Maximum velocity: ParticleUniverse::Real
		technique->setMaxVelocity(prop->DoGetValue().GetDouble());
	}
	else
	{
		PropertyWindow::copyAttributeToComponent(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_TECHNIQUE_ENABLED = _("Enabled");
	PRNL_TECHNIQUE_POSITION = _("Position");
	PRNL_TECHNIQUE_KEEP_LOCAL = _("Keep local");
	PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA = _("Visual particle quota");
	PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA = _("Emitted emitter quota");
	PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA = _("Emitted affector quota");
	PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA = _("Emitted technique quota");
	PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA = _("Emitted system quota");
	PRNL_TECHNIQUE_MATERIAL = _("Material");
	PRNL_TECHNIQUE_LOD_INDEX = _("Lod index");
	PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH = _("Default particle width");
	PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT = _("Default particle height");
	PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH = _("Default particle depth");
	PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION = _("Spatial hashing cell dimension");
	PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP = _("Spatial hashing cell overlap");
	PRNL_TECHNIQUE_SPHASHING_SIZE = _("Spatial hashtable size");
	PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL = _("Spatial hashing update interval");
	PRNL_TECHNIQUE_MAX_VELOCITY = _("Maximum velocity");
	PRNL_NAME = _("Name");
	PRNL_X = _(".x");
	PRNL_Y = _(".y");
	PRNL_Z = _(".z");

	mHelpHtml = wxT("ParticleTechnique.html");

	// Enabled: bool
	SetBoolChoices (_("True"), _("False")); // Forces Internationalization
	Append(wxBoolProperty(PRNL_TECHNIQUE_ENABLED, PRNL_TECHNIQUE_ENABLED, ParticleUniverse::ParticleTechnique::DEFAULT_ENABLED));

	// Position: Vector3
	appendVector3(PRNL_TECHNIQUE_POSITION, PRNL_TECHNIQUE_POSITION, ParticleUniverse::ParticleTechnique::DEFAULT_POSITION);

	// Keep local: bool
	Append(wxBoolProperty(PRNL_TECHNIQUE_KEEP_LOCAL, PRNL_TECHNIQUE_KEEP_LOCAL, ParticleUniverse::ParticleTechnique::DEFAULT_KEEP_LOCAL));

	// Visual particle quota: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA, 
		PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA, ParticleUniverse::ParticleTechnique::DEFAULT_VISUAL_PARTICLE_QUOTA));
	SetPropertyEditor(PRNL_TECHNIQUE_VISUAL_PARTICLE_QUOTA, wxPG_EDITOR(SpinCtrl));

	// Emitted emitter quota: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA, 
		PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA, 
		ParticleUniverse::ParticleTechnique::DEFAULT_EMITTED_EMITTER_QUOTA));
	SetPropertyEditor(PRNL_TECHNIQUE_EMITTED_EMITTER_QUOTA, wxPG_EDITOR(SpinCtrl));

	// Emitted affector quota: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA, 
		PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA, 
		ParticleUniverse::ParticleTechnique::DEFAULT_EMITTED_AFFECTOR_QUOTA));
	SetPropertyEditor(PRNL_TECHNIQUE_EMITTED_AFFECTOR_QUOTA, wxPG_EDITOR(SpinCtrl));

	// Emitted technique quota: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA, 
		PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA, 
		ParticleUniverse::ParticleTechnique::DEFAULT_EMITTED_TECHNIQUE_QUOTA));
	SetPropertyEditor(PRNL_TECHNIQUE_EMITTED_TECHNIQUE_QUOTA, wxPG_EDITOR(SpinCtrl));

	// Emitted system quota: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA, 
		PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA, 
		ParticleUniverse::ParticleTechnique::DEFAULT_EMITTED_SYSTEM_QUOTA));
	SetPropertyEditor(PRNL_TECHNIQUE_EMITTED_SYSTEM_QUOTA, wxPG_EDITOR(SpinCtrl));

	// Material: Ogre::String
	wxPGId pid = Append(new MaterialProperty(PRNL_TECHNIQUE_MATERIAL, PRNL_TECHNIQUE_MATERIAL));
	SetPropertyEditor(pid, wxPG_EDITOR(TextCtrlAndButton)); // Add a button

	// Lod index: unsigned short
	Append(wxUIntProperty(PRNL_TECHNIQUE_LOD_INDEX, 
		PRNL_TECHNIQUE_LOD_INDEX, 
		ParticleUniverse::ParticleTechnique::DEFAULT_LOD_INDEX));
	SetPropertyEditor(PRNL_TECHNIQUE_LOD_INDEX, wxPG_EDITOR(SpinCtrl));

	// Default particle width: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH, 
		PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH, 
		ParticleUniverse::ParticleTechnique::DEFAULT_WIDTH));
	SetPropertyEditor(PRNL_TECHNIQUE_DEFAULT_PARTICLE_WIDTH, wxPG_EDITOR(SpinCtrl));

	// Default particle height: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT,
		PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT, 
		ParticleUniverse::ParticleTechnique::DEFAULT_HEIGHT));
	SetPropertyEditor(PRNL_TECHNIQUE_DEFAULT_PARTICLE_HEIGHT, wxPG_EDITOR(SpinCtrl));

	// Default particle depth: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH, 
		PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH, 
		ParticleUniverse::ParticleTechnique::DEFAULT_DEPTH));
	SetPropertyEditor(PRNL_TECHNIQUE_DEFAULT_PARTICLE_DEPTH, wxPG_EDITOR(SpinCtrl));

	// Spatial hashing cell dimension: ParticleUniverse::ushort
	Append(wxUIntProperty(PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION, 
		PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION, 
		ParticleUniverse::ParticleTechnique::DEFAULT_SPATIAL_HASHING_CELL_DIM));
	SetPropertyEditor(PRNL_TECHNIQUE_SPHASHING_CELL_DIMENSION, wxPG_EDITOR(SpinCtrl));

	// Spatial hashing cell overlap: ParticleUniverse::ushort
	Append(wxUIntProperty(PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP, 
		PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP, 
		ParticleUniverse::ParticleTechnique::DEFAULT_SPATIAL_HASHING_CELL_OVERLAP));
	SetPropertyEditor(PRNL_TECHNIQUE_SPHASHING_CELL_OVERLAP, wxPG_EDITOR(SpinCtrl));

	// Spatial hashtable size: unsigned int
	Append(wxUIntProperty(PRNL_TECHNIQUE_SPHASHING_SIZE, 
		PRNL_TECHNIQUE_SPHASHING_SIZE, 
		ParticleUniverse::ParticleTechnique::DEFAULT_SPATIAL_HASHING_TABLE_SIZE));
	SetPropertyEditor(PRNL_TECHNIQUE_SPHASHING_SIZE, wxPG_EDITOR(SpinCtrl));

	// Spatial hashing update interval: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL, 
		PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL, 
		ParticleUniverse::ParticleTechnique::DEFAULT_SPATIAL_HASHING_INTERVAL));
	SetPropertyEditor(PRNL_TECHNIQUE_SPHASHING_UPDATE_INTERVAL, wxPG_EDITOR(SpinCtrl));

	// Maximum velocity: ParticleUniverse::Real
	Append(wxFloatProperty(PRNL_TECHNIQUE_MAX_VELOCITY, 
		PRNL_TECHNIQUE_MAX_VELOCITY, 
		ParticleUniverse::ParticleTechnique::DEFAULT_MAX_VELOCITY));
	SetPropertyEditor(PRNL_TECHNIQUE_MAX_VELOCITY, wxPG_EDITOR(SpinCtrl));
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	wxPGProperty* prop = event.GetPropertyPtr();
	onParentPropertyChanged(event);
	copyAttributeToTechnique(prop, propertyName);
	ParticleUniverse::ParticleTechnique* technique = static_cast<ParticleUniverse::ParticleTechnique*>(mOwner->getPUElement());
	if (technique && technique->_isMarkedForEmission())
	{
		ParticleUniverse::ParticleSystem* system = technique->getParentSystem();
		if (system)
		{
			// Force recreation of new emitted techniques
			// 1. Search emitter that is emitting this technique.
			// 2. Find its parent technique.
			// 3. call the other technique's _unprepareTechnique().
			size_t numTechniques = system->getNumTechniques();
			for (size_t i = 0; i < numTechniques; ++i)
			{
				ParticleUniverse::ParticleTechnique* t = system->getTechnique(i);
				size_t numEmitters = t->getNumEmittedEmitters();
				for (size_t j = 0; j < numEmitters; ++j)
				{
					ParticleUniverse::ParticleEmitter* emitter = t->getEmitter(j);
					if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_TECHNIQUE &&
						emitter->getEmitsName() == technique->getName())
					{
						// Found it.
						t->_unprepareTechnique();
					}
				}
			}
		}
	}
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::onParentPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();
	PropertyWindow::onPropertyChanged(event);
	notifyPropertyChanged();
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyEnabledToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique)
{
	technique->_setOriginalEnabled(prop->DoGetValue().GetBool());
	technique->setEnabled(prop->DoGetValue().GetBool());
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyPositionToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique)
{
	Ogre::Vector3 v3;
	v3 = doGetVector3(PRNL_TECHNIQUE_POSITION, v3);
	technique->position = v3;
	technique->originalPosition = v3;
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyKeepLocalToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique)
{
	technique->setKeepLocal(prop->DoGetValue().GetBool());
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyVisualParticleQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique)
{
	technique->setVisualParticleQuota(prop->DoGetValue().GetDouble());
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyEmittedEmitterQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique)
{
	technique->setEmittedEmitterQuota(prop->DoGetValue().GetDouble());
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyEmittedAffectorQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique)
{
	technique->setEmittedAffectorQuota(prop->DoGetValue().GetDouble());
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyEmittedTechniqueQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique)
{
	technique->setEmittedTechniqueQuota(prop->DoGetValue().GetDouble());
}
//-----------------------------------------------------------------------
void TechniquePropertyWindow::copyEmittedSystemQuotaToTechnique(wxPGProperty* prop, ParticleUniverse::ParticleTechnique* technique)
{
	technique->setEmittedSystemQuota(prop->DoGetValue().GetDouble());
}
