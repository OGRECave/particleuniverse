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

#include "ParticleUniverseCameraDependencyTokens.h"
#include "ParticleUniverseCameraDependency.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	/**************************************************************************
	 * CameraDependencyTranslator
	 *************************************************************************/
	CameraDependencyTranslator::CameraDependencyTranslator()
	{
	}
	//-------------------------------------------------------------------------
	void CameraDependencyTranslator::translate(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		ObjectAbstractNode* obj = reinterpret_cast<ObjectAbstractNode*>(node.get());
		CameraDependency* cameraDependency = any_cast<CameraDependency*>(obj->context);

		// Run through properties
		for(AbstractNodeList::iterator i = obj->children.begin(); i != obj->children.end(); ++i)
		{
			if((*i)->type == ANT_PROPERTY)
			{
				PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>((*i).get());
				if (prop->name == token[TOKEN_DISTANCE_THRESHOLD])
				{
					// Property: distance_threshold
					if (passValidateProperty(compiler, prop, token[TOKEN_DISTANCE_THRESHOLD], VAL_REAL))
					{
						Real val;
						if(getReal(prop->values.front(), &val))
						{
							cameraDependency->setThreshold(val);
						}
					}
				}
				else if (prop->name == token[TOKEN_INCREASE])
				{
					// Property: increase
					if (passValidateProperty(compiler, prop, token[TOKEN_INCREASE], VAL_BOOL))
					{
						bool val;
						if(getBoolean(prop->values.front(), &val))
						{
							cameraDependency->setIncrease(val);
						}
					}
				}
				else
				{
					errorUnexpectedProperty(compiler, prop);
				}
			}
			else if((*i)->type == ANT_OBJECT)
			{
				processNode(compiler, *i);
			}
            else
			{
				errorUnexpectedToken(compiler, *i);
			}
		}
	}
	//-----------------------------------------------------------------------
	void CameraDependencyWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a CameraDependency
		const CameraDependency* dependency = static_cast<const CameraDependency*>(element);

		// Write attributes
		if (dependency->getThreshold() != CameraDependency::DEFAULT_DISTANCE_THRESHOLD) serializer->writeLine(
			token[TOKEN_DISTANCE_THRESHOLD], StringConverter::toString(dependency->getThreshold()), 12);
		if (dependency->isIncrease() != CameraDependency::DEFAULT_INCREASE) serializer->writeLine(
			token[TOKEN_INCREASE], StringConverter::toString(dependency->isIncrease()), 12);
	}

}
