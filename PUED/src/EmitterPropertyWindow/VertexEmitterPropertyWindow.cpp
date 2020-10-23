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
#include "EmitterPropertyWindow/VertexEmitterPropertyWindow.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseMeshSelector.h"
#include "wx/propgrid/advprops.h"

//-----------------------------------------------------------------------
VertexEmitterPropertyWindow::VertexEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, const Ogre::String& name) :
	EmitterPropertyWindow(parent, owner, name)
{
	_initProperties();
}
//-----------------------------------------------------------------------
VertexEmitterPropertyWindow::VertexEmitterPropertyWindow(EmitterPropertyWindow* emitterPropertyWindow) :
	EmitterPropertyWindow(emitterPropertyWindow)
{
	_initProperties();
}
//-----------------------------------------------------------------------
VertexEmitterPropertyWindow::VertexEmitterPropertyWindow(wxWindow* parent, EditComponent* owner, ParticleUniverse::VertexEmitter* emitter) :
	EmitterPropertyWindow(parent, owner, emitter->getName())
{
	copyAttributesFromEmitter(emitter);
}
//-----------------------------------------------------------------------
void VertexEmitterPropertyWindow::copyAttributeToEmitter(wxPGProperty* prop, wxString propertyName)
{
	if (!prop)
		return;

	ParticleUniverse::VertexEmitter* vertexEmitter = static_cast<ParticleUniverse::VertexEmitter*>(mOwner->getPUElement());
	if (!vertexEmitter)
		return;

	if (propertyName == PRNL_VERTEX_EMITTER_STEP)
	{
		// Update emitter with Step
		copyStepToVertexEmitter(prop, vertexEmitter);
	}
	else if (propertyName == PRNL_VERTEX_EMITTER_SEGMENTS)
	{
		// Update emitter with Segments
		copySegmentsToVertexEmitter(prop, vertexEmitter);
	}
	else if (propertyName == PRNL_VERTEX_EMITTER_ITERATION)
	{
		// Update emitter with Iteration
		copyIterationToVertexEmitter(prop, vertexEmitter);
	}
	else if (propertyName == PRNL_VERTEX_EMITTER_MESH_NAME)
	{
		// Update emitter with Mesh name
		MeshProperty* meshProperty = static_cast<MeshProperty*>(prop);
		Ogre::String meshName = meshProperty->getMeshName();
		if (meshName != Ogre::StringUtil::BLANK)
		{
			vertexEmitter->setMeshName(meshName);
		}
	}
	else
	{
		// Update emitter with another attribute
		EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
	}
}
//-----------------------------------------------------------------------
void VertexEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter* emitter)
{
	EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

	// Copy properties from emitter to property window
	ParticleUniverse::VertexEmitter* vertexEmitter = static_cast<ParticleUniverse::VertexEmitter*>(emitter);

	// Step: ParticleUniverse::uint
	doSetUint16(PRNL_VERTEX_EMITTER_STEP, vertexEmitter->getStep());

	// Segments: ParticleUniverse::uint
	doSetUint16(PRNL_VERTEX_EMITTER_SEGMENTS, vertexEmitter->getSegments());

	// Iterations: ParticleUniverse::uint
	doSetUint16(PRNL_VERTEX_EMITTER_ITERATION, vertexEmitter->getIterations());

	// Step: Ogre::String
	wxString meshName = ogre2wx(vertexEmitter->getMeshName());
	doSetString(PRNL_VERTEX_EMITTER_MESH_NAME, meshName);
}
//-----------------------------------------------------------------------
void VertexEmitterPropertyWindow::_initProperties(void)
{
	// Set the (internationalized) property names
	PRNL_VERTEX_EMITTER_STEP = _("Vertex step");
	PRNL_VERTEX_EMITTER_SEGMENTS = _("Vertex segments");
	PRNL_VERTEX_EMITTER_ITERATION = _("Vertex iterations");
	PRNL_VERTEX_EMITTER_MESH_NAME = _("Vertex meshname");

	mHelpHtml = wxT("EmitterVertex.html");

	// Step: ParticleUniverse::uint
	Append(wxUIntProperty(PRNL_VERTEX_EMITTER_STEP, PRNL_VERTEX_EMITTER_STEP, ParticleUniverse::VertexEmitter::DEFAULT_STEP));
	SetPropertyEditor(PRNL_VERTEX_EMITTER_STEP, wxPG_EDITOR(SpinCtrl));

	// Segments: ParticleUniverse::uint
	Append(wxUIntProperty(PRNL_VERTEX_EMITTER_SEGMENTS, PRNL_VERTEX_EMITTER_SEGMENTS, ParticleUniverse::VertexEmitter::DEFAULT_SEGMENTS));
	SetPropertyEditor(PRNL_VERTEX_EMITTER_SEGMENTS, wxPG_EDITOR(SpinCtrl));

	// Iterations: ParticleUniverse::uint
	Append(wxUIntProperty(PRNL_VERTEX_EMITTER_ITERATION, PRNL_VERTEX_EMITTER_ITERATION, ParticleUniverse::VertexEmitter::DEFAULT_ITERATIONS));
	SetPropertyEditor(PRNL_VERTEX_EMITTER_ITERATION, wxPG_EDITOR(SpinCtrl));

	// Step: Ogre::String
	wxPGId pid = Append(new MeshProperty(PRNL_VERTEX_EMITTER_MESH_NAME, PRNL_VERTEX_EMITTER_MESH_NAME));
	SetPropertyEditor(pid, wxPG_EDITOR(TextCtrlAndButton)); // Add a button
}
//-----------------------------------------------------------------------
void VertexEmitterPropertyWindow::copyStepToVertexEmitter(wxPGProperty* prop, ParticleUniverse::VertexEmitter* vertexEmitter)
{
	vertexEmitter->setStep(prop->DoGetValue().GetLong());
}
//-----------------------------------------------------------------------
void VertexEmitterPropertyWindow::copySegmentsToVertexEmitter(wxPGProperty* prop, ParticleUniverse::VertexEmitter* vertexEmitter)
{
	vertexEmitter->setSegments(prop->DoGetValue().GetLong());
}
//-----------------------------------------------------------------------
void VertexEmitterPropertyWindow::copyIterationToVertexEmitter(wxPGProperty* prop, ParticleUniverse::VertexEmitter* vertexEmitter)
{
	vertexEmitter->setIterations(prop->DoGetValue().GetLong());
}
