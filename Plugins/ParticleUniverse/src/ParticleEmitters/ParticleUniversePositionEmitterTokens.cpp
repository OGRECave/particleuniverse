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

#include "ParticleEmitters/ParticleUniversePositionEmitter.h"
#include "ParticleEmitters/ParticleUniversePositionEmitterTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool PositionEmitterTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEmitter* em = any_cast<ParticleEmitter*>(prop->parent->context);
		PositionEmitter* emitter = static_cast<PositionEmitter*>(em);

		if (prop->name == token[TOKEN_POS_ADD_POSITION])
		{
			// Property: add_position
			if (passValidateProperty(compiler, prop, token[TOKEN_POS_ADD_POSITION], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					emitter->addPosition(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_POS_RANDOMIZE])
		{
			// Property: random_position
			if (passValidateProperty(compiler, prop, token[TOKEN_POS_RANDOMIZE], VAL_BOOL))
			{
				bool val;
				if(getBoolean(prop->values.front(), &val))
				{
					emitter->setRandomized(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool PositionEmitterTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void PositionEmitterWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a PositionEmitter
		const PositionEmitter* emitter = static_cast<const PositionEmitter*>(element);

		// Write the header of the PositionEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->isRandomized() != PositionEmitter::DEFAULT_RANDOMIZE) serializer->writeLine(
			token[TOKEN_POS_RANDOMIZE], StringConverter::toString(emitter->isRandomized()), 12);

		// List of positions
		vector<Vector3> positionList = emitter->getPositions();
		if(!positionList.empty())
		{
			vector<Vector3>::const_iterator it;
			vector<Vector3>::const_iterator itEnd = positionList.end();
			for (it = positionList.begin(); it != itEnd; ++it)
			{
				serializer->writeLine(token[TOKEN_POS_ADD_POSITION], StringConverter::toString(*it), 12);
			}
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
