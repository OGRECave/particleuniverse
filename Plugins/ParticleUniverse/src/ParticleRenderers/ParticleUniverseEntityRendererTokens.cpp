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

#include "ParticleRenderers/ParticleUniverseEntityRendererTokens.h"
#include "ParticleRenderers/ParticleUniverseEntityRenderer.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool EntityRendererTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleRenderer* ren = any_cast<ParticleRenderer*>(prop->parent->context);
		EntityRenderer* renderer = static_cast<EntityRenderer*>(ren);

		if (prop->name == token[TOKEN_MESH_NAME])
		{
			// Property: mesh_name
			if (passValidateProperty(compiler, prop, token[TOKEN_MESH_NAME], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					renderer->setMeshName(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_ENT_MESH_NAME])
		{
			// Property: entity_renderer_mesh_name (deprecated and replaced by mesh_name)
			if (passValidateProperty(compiler, prop, token[TOKEN_ENT_MESH_NAME], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					renderer->setMeshName(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_ENT_ORIENTATION_TYPE])
		{
			// Property: entity_orientation_type
			if (passValidateProperty(compiler, prop, token[TOKEN_ENT_ORIENTATION_TYPE], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_ENT_ORIENTED_SELF])
					{
						renderer->setEntityOrientationType(EntityRenderer::ENT_ORIENTED_SELF);
						return true;
					}
					else if (val == token[TOKEN_ENT_ORIENTED_SELF_MIRRORED])
					{
						renderer->setEntityOrientationType(EntityRenderer::ENT_ORIENTED_SELF_MIRRORED);
						return true;
					}
					else if (val == token[TOKEN_ENT_ORIENTED_SHAPE])
					{
						renderer->setEntityOrientationType(EntityRenderer::ENT_ORIENTED_SHAPE);
						return true;
					}
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool EntityRendererTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void EntityRendererWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a EntityRenderer
		const EntityRenderer* renderer = static_cast<const EntityRenderer*>(element);

		// Write the header of the EntityRenderer
		serializer->writeLine(token[TOKEN_RENDERER], renderer->getRendererType(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleRendererWriter::write(serializer, element);

		// Write own attributes
		if (renderer->getMeshName() != BLANK_STRING) serializer->writeLine(
			token[TOKEN_MESH_NAME], renderer->getMeshName(), 12);
		if (renderer->getEntityOrientationType() != EntityRenderer::DEFAULT_ORIENTATION_TYPE)
		{
			String orientationType = token[TOKEN_ENT_ORIENTED_SELF];
			if (renderer->getEntityOrientationType() == EntityRenderer::ENT_ORIENTED_SHAPE) orientationType = token[TOKEN_ENT_ORIENTED_SELF];
			else if (renderer->getEntityOrientationType() == EntityRenderer::ENT_ORIENTED_SELF_MIRRORED) orientationType = token[TOKEN_ENT_ORIENTED_SELF_MIRRORED];
			serializer->writeLine(token[TOKEN_ENT_ORIENTATION_TYPE], orientationType, 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
