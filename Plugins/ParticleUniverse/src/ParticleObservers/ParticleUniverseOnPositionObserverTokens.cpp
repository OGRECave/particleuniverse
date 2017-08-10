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

#include "ParticleObservers/ParticleUniverseOnPositionObserver.h"
#include "ParticleObservers/ParticleUniverseOnPositionObserverTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool OnPositionObserverTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleObserver* ob = any_cast<ParticleObserver*>(prop->parent->context);
		OnPositionObserver* observer = static_cast<OnPositionObserver*>(ob);

		if (prop->name == token[TOKEN_ONPOSITION_X])
		{
			// Property: position_x
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_ONPOSITION_X], 2))
			{
				String compareType;
				Real val = 0;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if(getString(*i, &compareType))
				{
					if (compareType == token[TOKEN_LESS_THAN])
					{
						observer->setComparePositionX(CO_LESS_THAN);
					}
					else if (compareType == token[TOKEN_GREATER_THAN])
					{
						observer->setComparePositionX(CO_GREATER_THAN);
					}
					else if (compareType == token[TOKEN_EQUALS])
					{
						observer->setComparePositionX(CO_EQUALS);
					}
					++i;
					if(getReal(*i, &val))
					{
						observer->setPositionXThreshold(val);
						return true;
					}
				}
			}
		}
		else if (prop->name == token[TOKEN_ONPOSITION_Y])
		{
			// Property: position_y
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_ONPOSITION_Y], 2))
			{
				String compareType;
				Real val = 0;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if(getString(*i, &compareType))
				{
					if (compareType == token[TOKEN_LESS_THAN])
					{
						observer->setComparePositionY(CO_LESS_THAN);
					}
					else if (compareType == token[TOKEN_GREATER_THAN])
					{
						observer->setComparePositionY(CO_GREATER_THAN);
					}
					else if (compareType == token[TOKEN_EQUALS])
					{
						observer->setComparePositionY(CO_EQUALS);
					}
					++i;
					if(getReal(*i, &val))
					{
						observer->setPositionYThreshold(val);
						return true;
					}
				}
			}
		}
		else if (prop->name == token[TOKEN_ONPOSITION_Z])
		{
			// Property: position_z
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_ONPOSITION_Z], 2))
			{
				String compareType;
				Real val = 0;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if(getString(*i, &compareType))
				{
					if (compareType == token[TOKEN_LESS_THAN])
					{
						observer->setComparePositionZ(CO_LESS_THAN);
					}
					else if (compareType == token[TOKEN_GREATER_THAN])
					{
						observer->setComparePositionZ(CO_GREATER_THAN);
					}
					else if (compareType == token[TOKEN_EQUALS])
					{
						observer->setComparePositionZ(CO_EQUALS);
					}
					++i;
					if(getReal(*i, &val))
					{
						observer->setPositionZThreshold(val);
						return true;
					}
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool OnPositionObserverTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void OnPositionObserverWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a OnPositionObserver
		const OnPositionObserver* observer = static_cast<const OnPositionObserver*>(element);

		// Write the header of the OnPositionObserver
		serializer->writeLine(token[TOKEN_OBSERVER], observer->getObserverType(), observer->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleObserverWriter::write(serializer, element);

		// Write own attributes
		if (observer->isPositionXThresholdSet())
		{
			String compare = token[TOKEN_GREATER_THAN];
			if (observer->getComparePositionX() == CO_LESS_THAN)
			{
				compare = token[TOKEN_LESS_THAN];
			}
			else if (observer->getComparePositionX() == CO_EQUALS)
			{
				compare = token[TOKEN_EQUALS];
			}
			serializer->writeLine(token[TOKEN_ONPOSITION_X], compare, StringConverter::toString(observer->getPositionXThreshold()), 12);
		}
		if (observer->isPositionYThresholdSet())
		{
			String compare = token[TOKEN_GREATER_THAN];
			if (observer->getComparePositionY() == CO_LESS_THAN) 
			{
				compare = token[TOKEN_LESS_THAN];
			}
			else if (observer->getComparePositionY() == CO_EQUALS) 
			{
				compare = token[TOKEN_EQUALS];
			}
			serializer->writeLine(token[TOKEN_ONPOSITION_Y], compare, StringConverter::toString(observer->getPositionYThreshold()), 12);
		}
		if (observer->isPositionZThresholdSet())
		{
			String compare = token[TOKEN_GREATER_THAN];
			if (observer->getComparePositionZ() == CO_LESS_THAN)
			{
				compare = token[TOKEN_LESS_THAN];
			}
			else if (observer->getComparePositionZ() == CO_EQUALS)
			{
				compare = token[TOKEN_EQUALS];
			}
			serializer->writeLine(token[TOKEN_ONPOSITION_Z], compare, StringConverter::toString(observer->getPositionZThreshold()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
