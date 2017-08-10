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

#include "ParticleObservers/ParticleUniverseOnCountObserver.h"
#include "ParticleObservers/ParticleUniverseOnCountObserverTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool OnCountObserverTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleObserver* ob = any_cast<ParticleObserver*>(prop->parent->context);
		OnCountObserver* observer = static_cast<OnCountObserver*>(ob);

		if (prop->name == token[TOKEN_ONCOUNT_THRESHOLD])
		{
			// Property: count_threshold
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_ONCOUNT_THRESHOLD], 2))
			{
				String compareType;
				uint val = 0;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if(getString(*i, &compareType))
				{
					if (compareType == token[TOKEN_LESS_THAN])
					{
						observer->setCompare(CO_LESS_THAN);
					}
					else if (compareType == token[TOKEN_GREATER_THAN])
					{
						observer->setCompare(CO_GREATER_THAN);
					}
					else if (compareType == token[TOKEN_EQUALS])
					{
						observer->setCompare(CO_EQUALS);
					}
					++i;
					if(getUInt(*i, &val))
					{
						observer->setThreshold(val);
						return true;
					}
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool OnCountObserverTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void OnCountObserverWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a OnCountObserver
		const OnCountObserver* observer = static_cast<const OnCountObserver*>(element);

		// Write the header of the OnCountObserver
		serializer->writeLine(token[TOKEN_OBSERVER], observer->getObserverType(), observer->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleObserverWriter::write(serializer, element);

		// Write own attributes
		String compare = token[TOKEN_GREATER_THAN];
		if (observer->getCompare() == CO_LESS_THAN)
		{
			compare = token[TOKEN_LESS_THAN];
		}
		else if (observer->getCompare() == CO_EQUALS)
		{
			compare = token[TOKEN_EQUALS];
		}
		serializer->writeLine(token[TOKEN_ONCOUNT_THRESHOLD], compare, StringConverter::toString(observer->getThreshold()), 12);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
