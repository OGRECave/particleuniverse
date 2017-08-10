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

#include "ParticleEventHandlers/ParticleUniverseDoScaleEventHandler.h"
#include "ParticleEventHandlers/ParticleUniverseDoScaleEventHandlerTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool DoScaleEventHandlerTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEventHandler* evt = any_cast<ParticleEventHandler*>(prop->parent->context);
		DoScaleEventHandler* handler = static_cast<DoScaleEventHandler*>(evt);

		if (prop->name == token[TOKEN_DOSCALE_FRACTION])
		{
			// Property: scale_fraction
			if (passValidateProperty(compiler, prop, token[TOKEN_DOSCALE_FRACTION], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					handler->setScaleFraction(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_DOSCALE_TYPE])
		{
			// Property: scale_type
			if (passValidateProperty(compiler, prop, token[TOKEN_DOSCALE_TYPE], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_TIME_TO_LIVE] || val == token[TOKEN_DOSCALE_TIME_TO_LIVE])
					{
						handler->setScaleType(DoScaleEventHandler::ST_TIME_TO_LIVE);
						return true;
					}
					else if (val == token[TOKEN_VELOCITY] || val == token[TOKEN_DOSCALE_VELOCITY])
					{
						handler->setScaleType(DoScaleEventHandler::ST_VELOCITY);
						return true;
					}
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool DoScaleEventHandlerTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void DoScaleEventHandlerWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a DoScaleEventHandler
		const DoScaleEventHandler* eventHandler = static_cast<const DoScaleEventHandler*>(element);

		// Write the header of the DoScaleEventHandler
		serializer->writeLine(token[TOKEN_HANDLER], eventHandler->getEventHandlerType(), eventHandler->getName(), 12);
		serializer->writeLine("{", 12);

		// Write base attributes
		ParticleEventHandlerWriter::write(serializer, element);

		// Write own attributes
		if (eventHandler->getScaleFraction() != DoScaleEventHandler::DEFAULT_SCALE_FRACTION) serializer->writeLine(
			token[TOKEN_DOSCALE_FRACTION], StringConverter::toString(eventHandler->getScaleFraction()), 16);
		String scaleType = token[TOKEN_DOSCALE_TIME_TO_LIVE];
		if (eventHandler->getScaleType() == DoScaleEventHandler::ST_VELOCITY) scaleType = token[TOKEN_DOSCALE_VELOCITY];
		serializer->writeLine(token[TOKEN_DOSCALE_TYPE], scaleType, 16);

		// Write the close bracket
		serializer->writeLine("}", 12);
	}

}
