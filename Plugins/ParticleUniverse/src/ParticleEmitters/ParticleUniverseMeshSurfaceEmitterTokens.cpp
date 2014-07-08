/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

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

#include "ParticleUniversePCH.h"

#ifndef PARTICLE_UNIVERSE_EXPORTS
#define PARTICLE_UNIVERSE_EXPORTS
#endif

#include "ParticleEmitters/ParticleUniverseMeshSurfaceEmitter.h"
#include "ParticleEmitters/ParticleUniverseMeshSurfaceEmitterTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool MeshSurfaceEmitterTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEmitter* em = any_cast<ParticleEmitter*>(prop->parent->context);
		MeshSurfaceEmitter* emitter = static_cast<MeshSurfaceEmitter*>(em);

		if (prop->name == token[TOKEN_MESH_NAME])
		{
			// Property: mesh_name
			if (passValidateProperty(compiler, prop, token[TOKEN_MESH_NAME], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					emitter->setMeshName(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_MESH_SURFACE_NAME])
		{
			// Property: mesh_surface_mesh_name (deprecated and replaced by mesh_name)
			if (passValidateProperty(compiler, prop, token[TOKEN_MESH_SURFACE_NAME], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					emitter->setMeshName(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_MESH_SURFACE_DISTRIBUTION])
		{
			// Property: mesh_surface_distribution
			if (passValidateProperty(compiler, prop, token[TOKEN_MESH_SURFACE_DISTRIBUTION], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_MESH_SURFACE_EDGE])
					{
						emitter->setDistribution(MeshInfo::MSD_EDGE);
						return true;
					}
					else if (val == token[TOKEN_MESH_SURFACE_HETEROGENEOUS_1])
					{
						emitter->setDistribution(MeshInfo::MSD_HETEROGENEOUS_1);
						return true;
					}
					else if (val == token[TOKEN_MESH_SURFACE_HETEROGENEOUS_2])
					{
						emitter->setDistribution(MeshInfo::MSD_HETEROGENEOUS_2);
						return true;
					}
					else if (val == token[TOKEN_MESH_SURFACE_HOMOGENEOUS])
					{
						emitter->setDistribution(MeshInfo::MSD_HOMOGENEOUS);
						return true;
					}
					else if (val == token[TOKEN_MESH_SURFACE_HOMOGENEOUS])
					{
						emitter->setDistribution(MeshInfo::MSD_HOMOGENEOUS);
						return true;
					}
				}
			}
		}
		else if (prop->name == token[TOKEN_MESH_SURFACE_MESH_SCALE])
		{
			// Property: mesh_surface_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_MESH_SURFACE_MESH_SCALE], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					emitter->setScale(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool MeshSurfaceEmitterTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void MeshSurfaceEmitterWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a MeshSurfaceEmitter
		const MeshSurfaceEmitter* emitter = static_cast<const MeshSurfaceEmitter*>(element);

		// Write the header of the MeshSurfaceEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->getMeshName() != BLANK_STRING) serializer->writeLine(
			token[TOKEN_MESH_NAME], emitter->getMeshName(), 12);
		if (emitter->getScale() != MeshSurfaceEmitter::DEFAULT_SCALE) serializer->writeLine(
			token[TOKEN_MESH_SURFACE_MESH_SCALE], StringConverter::toString(emitter->getScale()), 12);
		if (emitter->getDistribution() != MeshSurfaceEmitter::DEFAULT_DISTRIBUTION) 
		{
			String distribution = token[TOKEN_MESH_SURFACE_HOMOGENEOUS];
			if (emitter->getDistribution() == MeshInfo::MSD_HOMOGENEOUS) distribution = token[TOKEN_MESH_SURFACE_HOMOGENEOUS];
			else if (emitter->getDistribution() == MeshInfo::MSD_EDGE) distribution = token[TOKEN_MESH_SURFACE_EDGE];
			else if (emitter->getDistribution() == MeshInfo::MSD_HETEROGENEOUS_1) distribution = token[TOKEN_MESH_SURFACE_HETEROGENEOUS_1];
			else if (emitter->getDistribution() == MeshInfo::MSD_HETEROGENEOUS_2) distribution = token[TOKEN_MESH_SURFACE_HETEROGENEOUS_2];
			else if (emitter->getDistribution() == MeshInfo::MSD_VERTEX) distribution = token[TOKEN_VERTEX];
			serializer->writeLine(token[TOKEN_MESH_SURFACE_DISTRIBUTION], distribution, 12);
		}


		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
