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

#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.h"
#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandlerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool DoAffectorEventHandlerTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEventHandler* evt = any_cast<ParticleEventHandler*>(prop->parent->context);
		DoAffectorEventHandler* handler = static_cast<DoAffectorEventHandler*>(evt);

		if (prop->name == token[TOKEN_FORCE_AFFECTOR])
		{
			// Property: force_affector
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_AFFECTOR], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					handler->setAffectorName(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_FORCE_AFFECTOR_PRE_POST])
		{
			// Property: pre_post
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_AFFECTOR_PRE_POST], VAL_BOOL))
			{
				bool val;
				if(getBoolean(prop->values.front(), &val))
				{
					handler->setPrePost(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool DoAffectorEventHandlerTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void DoAffectorEventHandlerWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a DoAffectorEventHandler
		const DoAffectorEventHandler* eventHandler = static_cast<const DoAffectorEventHandler*>(element);

		// Write the header of the DoAffectorEventHandler
		serializer->writeLine(token[TOKEN_HANDLER], eventHandler->getEventHandlerType(), eventHandler->getName(), 12);
		serializer->writeLine("{", 12);

		// Write base attributes
		ParticleEventHandlerWriter::write(serializer, element);

		// Write own attributes
		if (eventHandler->getAffectorName() != BLANK_STRING) serializer->writeLine(
			token[TOKEN_FORCE_AFFECTOR], eventHandler->getAffectorName(), 16);
		if (eventHandler->getPrePost() != DoAffectorEventHandler::DEFAULT_PRE_POST) serializer->writeLine(
			token[TOKEN_FORCE_AFFECTOR_PRE_POST], StringConverter::toString(eventHandler->getPrePost()), 16);

		// Write the close bracket
		serializer->writeLine("}", 12);
	}

}
