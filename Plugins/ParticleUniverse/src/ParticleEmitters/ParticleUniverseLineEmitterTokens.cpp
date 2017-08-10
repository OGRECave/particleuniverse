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

#include "ParticleEmitters/ParticleUniverseLineEmitter.h"
#include "ParticleEmitters/ParticleUniverseLineEmitterTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool LineEmitterTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEmitter* em = any_cast<ParticleEmitter*>(prop->parent->context);
		LineEmitter* emitter = static_cast<LineEmitter*>(em);

		if (prop->name == token[TOKEN_END])
		{
			// Property: end
			if (passValidateProperty(compiler, prop, token[TOKEN_END], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					emitter->setEnd(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_LINE_EMIT_END])
		{
			// Property: line_em_end (deprecated and replaced by 'end')
			if (passValidateProperty(compiler, prop, token[TOKEN_LINE_EMIT_END], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					emitter->setEnd(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_MAX_INCREMENT])
		{
			// Property: max_increment
			if (passValidateProperty(compiler, prop, token[TOKEN_MAX_INCREMENT], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setMaxIncrement(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_LINE_EMIT_MAX_INCREMENT])
		{
			// Property: line_em_max_increment (de[recated and replaced by 'max_increment')
			if (passValidateProperty(compiler, prop, token[TOKEN_LINE_EMIT_MAX_INCREMENT], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setMaxIncrement(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_MIN_INCREMENT])
		{
			// Property: min_increment
			if (passValidateProperty(compiler, prop, token[TOKEN_MIN_INCREMENT], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setMinIncrement(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_LINE_EMIT_MIN_INCREMENT])
		{
			// Property: line_em_min_increment (deprecated and replaced by 'min_increment')
			if (passValidateProperty(compiler, prop, token[TOKEN_LINE_EMIT_MIN_INCREMENT], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setMinIncrement(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_MAX_DEVIATION])
		{
			// Property: max_deviation
			if (passValidateProperty(compiler, prop, token[TOKEN_MAX_DEVIATION], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setMaxDeviation(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_LINE_EMIT_MAX_DEVIATION])
		{
			// Property: line_em_max_deviation (deprecated and replaced by 'max_deviation')
			if (passValidateProperty(compiler, prop, token[TOKEN_LINE_EMIT_MAX_DEVIATION], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					emitter->setMaxDeviation(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool LineEmitterTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void LineEmitterWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a LineEmitter
		const LineEmitter* emitter = static_cast<const LineEmitter*>(element);

		// Write the header of the LineEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->getEnd() != LineEmitter::DEFAULT_END) serializer->writeLine(
			token[TOKEN_END], StringConverter::toString(emitter->getEnd()), 12);
		if (emitter->getMinIncrement() != LineEmitter::DEFAULT_MIN_INCREMENT) serializer->writeLine(
			token[TOKEN_MIN_INCREMENT], StringConverter::toString(emitter->getMinIncrement()), 12);
		if (emitter->getMaxIncrement() != LineEmitter::DEFAULT_MAX_INCREMENT) serializer->writeLine(
			token[TOKEN_MAX_INCREMENT], StringConverter::toString(emitter->getMaxIncrement()), 12);
		if (emitter->getMaxDeviation() != LineEmitter::DEFAULT_MAX_DEVIATION) serializer->writeLine(
			token[TOKEN_MAX_DEVIATION], StringConverter::toString(emitter->getMaxDeviation()), 12);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
