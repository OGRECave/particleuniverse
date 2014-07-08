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

#include "ParticleEmitters/ParticleUniverseSlaveEmitter.h"
#include "ParticleEmitters/ParticleUniverseSlaveEmitterTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool SlaveEmitterTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEmitter* em = any_cast<ParticleEmitter*>(prop->parent->context);
		SlaveEmitter* emitter = static_cast<SlaveEmitter*>(em);

		if (prop->name == token[TOKEN_MASTER_TECHNIQUE])
		{
			// Property: master_technique_name
			if (passValidateProperty(compiler, prop, token[TOKEN_MASTER_TECHNIQUE], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					emitter->setMasterTechniqueName(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_MASTER_EMITTER])
		{
			// Property: master_emitter_name
			if (passValidateProperty(compiler, prop, token[TOKEN_MASTER_EMITTER], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					emitter->setMasterEmitterName(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool SlaveEmitterTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void SlaveEmitterWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a SlaveEmitter
		const SlaveEmitter* emitter = static_cast<const SlaveEmitter*>(element);

		// Write the header of the SlaveEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->getMasterTechniqueName() != BLANK_STRING) serializer->writeLine(
			token[TOKEN_MASTER_TECHNIQUE], emitter->getMasterTechniqueName(), 12);
		if (emitter->getMasterEmitterName() != BLANK_STRING) serializer->writeLine(
			token[TOKEN_MASTER_EMITTER], emitter->getMasterEmitterName(), 12);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
