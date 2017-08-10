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

#include "ParticleEmitters/ParticleUniverseBoxEmitter.h"
#include "ParticleEmitters/ParticleUniverseBoxEmitterTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool BoxEmitterTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEmitter* em = any_cast<ParticleEmitter*>(prop->parent->context);
		BoxEmitter* emitter = static_cast<BoxEmitter*>(em);

		if (prop->name == token[TOKEN_BOX_WIDTH])
		{
			// Property: box_width
			if (passValidateProperty(compiler, prop, token[TOKEN_BOX_WIDTH], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setWidth(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_EMITTER_BOX_WIDTH])
		{
			// Property: box_em_width (deprecated and replaced by 'box_width')
			if (passValidateProperty(compiler, prop, token[TOKEN_EMITTER_BOX_WIDTH], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setWidth(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_BOX_HEIGHT])
		{
			// Property: box_height
			if (passValidateProperty(compiler, prop, token[TOKEN_BOX_HEIGHT], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setHeight(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_EMITTER_BOX_HEIGHT])
		{
			// Property: box_em_height (deprecated and replaced by 'box_height')
			if (passValidateProperty(compiler, prop, token[TOKEN_EMITTER_BOX_HEIGHT], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setHeight(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_BOX_DEPTH])
		{
			// Property: box_depth
			if (passValidateProperty(compiler, prop, token[TOKEN_BOX_DEPTH], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setDepth(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_EMITTER_BOX_DEPTH])
		{
			// Property: box_em_depth (deprecated and replaced by 'box_depth')
			if (passValidateProperty(compiler, prop, token[TOKEN_EMITTER_BOX_DEPTH], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setDepth(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool BoxEmitterTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void BoxEmitterWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a BoxEmitter
		const BoxEmitter* emitter = static_cast<const BoxEmitter*>(element);

		// Write the header of the BoxEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->getWidth() != BoxEmitter::DEFAULT_WIDTH) serializer->writeLine(
			token[TOKEN_BOX_WIDTH], StringConverter::toString(emitter->getWidth()), 12);
		if (emitter->getHeight() != BoxEmitter::DEFAULT_HEIGHT) serializer->writeLine(
			token[TOKEN_BOX_HEIGHT], StringConverter::toString(emitter->getHeight()), 12);
		if (emitter->getDepth() != BoxEmitter::DEFAULT_DEPTH) serializer->writeLine(
			token[TOKEN_BOX_DEPTH], StringConverter::toString(emitter->getDepth()), 12);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
