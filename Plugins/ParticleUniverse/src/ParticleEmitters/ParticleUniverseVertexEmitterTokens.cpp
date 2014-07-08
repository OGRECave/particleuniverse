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

#include "ParticleEmitters/ParticleUniverseVertexEmitter.h"
#include "ParticleEmitters/ParticleUniverseVertexEmitterTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool VertexEmitterTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEmitter* em = any_cast<ParticleEmitter*>(prop->parent->context);
		VertexEmitter* emitter = static_cast<VertexEmitter*>(em);

		if (prop->name == token[TOKEN_STEP])
		{
			// Property: step
			if (passValidateProperty(compiler, prop, token[TOKEN_STEP], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					emitter->setStep(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_VERTEX_STEP])
		{
			// Property: vertex_em_step (deprecated and replaced by 'step')
			if (passValidateProperty(compiler, prop, token[TOKEN_VERTEX_STEP], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					emitter->setStep(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_NUMBER_OF_SEGMENTS])
		{
			// Property: number_of_segments
			if (passValidateProperty(compiler, prop, token[TOKEN_NUMBER_OF_SEGMENTS], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					emitter->setSegments(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_VERTEX_SEGMENTS])
		{
			// Property: vertex_em_segments (deprecated and replaced by 'number_of_segments')
			if (passValidateProperty(compiler, prop, token[TOKEN_VERTEX_SEGMENTS], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					emitter->setSegments(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_ITERATIONS])
		{
			// Property: iterations
			if (passValidateProperty(compiler, prop, token[TOKEN_ITERATIONS], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					emitter->setIterations(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_VERTEX_ITERATIONS])
		{
			// Property: vertex_em_iterations (deprecated and replaced by 'iterations')
			if (passValidateProperty(compiler, prop, token[TOKEN_VERTEX_ITERATIONS], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					emitter->setIterations(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_MESH_NAME])
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
		else if (prop->name == token[TOKEN_VERTEX_MESH_NAME])
		{
			// Property: vertex_em_mesh_name (deprecated and replaced by mesh_name)
			if (passValidateProperty(compiler, prop, token[TOKEN_VERTEX_MESH_NAME], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					emitter->setMeshName(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool VertexEmitterTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void VertexEmitterWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a VertexEmitter
		const VertexEmitter* emitter = static_cast<const VertexEmitter*>(element);

		// Write the header of the VertexEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->getStep() != VertexEmitter::DEFAULT_STEP) serializer->writeLine(
			token[TOKEN_STEP], StringConverter::toString(emitter->getStep()), 12);
		if (emitter->getSegments() != VertexEmitter::DEFAULT_SEGMENTS) serializer->writeLine(
			token[TOKEN_NUMBER_OF_SEGMENTS], StringConverter::toString(emitter->getSegments()), 12);
		if (emitter->getIterations() != VertexEmitter::DEFAULT_ITERATIONS) serializer->writeLine(
			token[TOKEN_ITERATIONS], StringConverter::toString(emitter->getIterations()), 12);
		if (emitter->getMeshName() != BLANK_STRING) serializer->writeLine(
			token[TOKEN_MESH_NAME], emitter->getMeshName(), 12);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
