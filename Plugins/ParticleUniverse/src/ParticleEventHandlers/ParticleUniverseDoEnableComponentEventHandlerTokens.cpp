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

#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.h"
#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandlerTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool DoEnableComponentEventHandlerTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleEventHandler* evt = any_cast<ParticleEventHandler*>(prop->parent->context);
		DoEnableComponentEventHandler* handler = static_cast<DoEnableComponentEventHandler*>(evt);

		if (prop->name == token[TOKEN_DOENABLE_COMPONENT])
		{
			// Property: enable_component
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_DOENABLE_COMPONENT], 3))
			{
				String componentType;
				String name;
				bool enabled = true;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if(getString(*i, &componentType))
				{
					if (componentType == token[TOKEN_DOENABLE_EMITTER_COMPONENT])
					{
						handler->setComponentType(CT_EMITTER);
					}
					else if (componentType == token[TOKEN_DOENABLE_AFFECTOR_COMPONENT])
					{
						handler->setComponentType(CT_AFFECTOR);
					}
					else if (componentType == token[TOKEN_DOENABLE_OBSERVER_COMPONENT])
					{
						handler->setComponentType(CT_OBSERVER);
					}
					else if (componentType == token[TOKEN_DOENABLE_TECHNIQUE_COMPONENT])
					{
						handler->setComponentType(CT_TECHNIQUE);
					}
					++i;
					if(getString(*i, &name))
					{
						handler->setComponentName(name);
						++i;
						if (getBoolean(*i, &enabled))
						{
							handler->setComponentEnabled(enabled);
							return true;
						}
					}
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool DoEnableComponentEventHandlerTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void DoEnableComponentEventHandlerWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a DoEnableComponentEventHandler
		const DoEnableComponentEventHandler* eventHandler = static_cast<const DoEnableComponentEventHandler*>(element);

		// Write the header of the DoEnableComponentEventHandler
		serializer->writeLine(token[TOKEN_HANDLER], eventHandler->getEventHandlerType(), eventHandler->getName(), 12);
		serializer->writeLine("{", 12);

		// Write base attributes
		ParticleEventHandlerWriter::write(serializer, element);

		// Write own attributes
		String componentType = token[TOKEN_DOENABLE_EMITTER_COMPONENT];
		if (eventHandler->getComponentType() == CT_AFFECTOR) componentType = token[TOKEN_DOENABLE_AFFECTOR_COMPONENT];
		else if (eventHandler->getComponentType() == CT_TECHNIQUE) componentType = token[TOKEN_DOENABLE_TECHNIQUE_COMPONENT];
		else if (eventHandler->getComponentType() == CT_OBSERVER) componentType = token[TOKEN_DOENABLE_OBSERVER_COMPONENT];
		serializer->writeLine(
			token[TOKEN_DOENABLE_COMPONENT], 
			componentType, 
			eventHandler->getComponentName(), 
			StringConverter::toString(eventHandler->isComponentEnabled()),
			16);

		// Write the close bracket
		serializer->writeLine("}", 12);
	}

}
