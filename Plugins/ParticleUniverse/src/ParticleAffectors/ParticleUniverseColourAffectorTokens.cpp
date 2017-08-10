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

#include "ParticleAffectors/ParticleUniverseColourAffector.h"
#include "ParticleAffectors/ParticleUniverseColourAffectorTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool ColourAffectorTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleAffector* af = any_cast<ParticleAffector*>(prop->parent->context);
		ColourAffector* affector = static_cast<ColourAffector*>(af);

		if (prop->name == token[TOKEN_TIME_COLOUR] || prop->name == token[TOKEN_COLOUR_TIME_COLOUR])
		{
			if (passValidatePropertyNumberOfValuesRange(compiler, prop, token[TOKEN_TIME_COLOUR], 4, 5))
			{
				int n = 0;
				Real time;
				ColourValue colour;
				AbstractNodeList::const_iterator i = prop->values.begin();
				AbstractNodeList::const_iterator end = prop->values.end();
				while(i != end)
				{
					float v = 0;
					if(getFloat(*i, &v))
					{
						switch(n)
						{
							case 0:
								time = v;
							break;
							case 1:
								colour.r = v;
							break;
							case 2:
								colour.g = v;
							break;
							case 3:
								colour.b = v;
							break;
							case 4:
								colour.a = v;
							break;
						}
					}
					++n;
					++i;
				}
				affector->addColour(time, colour);
				return true;
			}
		}
		else if (prop->name == token[TOKEN_COLOUR_OPERATION])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_COLOUR_OPERATION], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_COLOUR_SET])
					{
						affector->setColourOperation(ColourAffector::CAO_SET);
						return true;
					}
					else if (val == token[TOKEN_COLOUR_MULTIPLY])
					{
						affector->setColourOperation(ColourAffector::CAO_MULTIPLY);
						return true;
					}
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool ColourAffectorTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void ColourAffectorWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a ColourAffector
		const ColourAffector* affector = static_cast<const ColourAffector*>(element);

		// Write the header of the ColourAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		ColourAffector::ColourMap colourMap = affector->getTimeAndColour();
		ColourAffector::ColourMap::const_iterator it;
		ColourAffector::ColourMap::const_iterator itEnd = colourMap.end();
		Real time;
		ColourValue colour;
		for (it = colourMap.begin(); it != itEnd; ++it)
		{
			time = it->first;
			colour = it->second;
			serializer->writeLine(token[TOKEN_TIME_COLOUR], StringConverter::toString(time), String(" ") + StringConverter::toString(colour), 12);
		}

		if (affector->getColourOperation() != ColourAffector::DEFAULT_COLOUR_OPERATION) 
		{
			String colourOperation = token[TOKEN_COLOUR_MULTIPLY];
			if (affector->getColourOperation() == ColourAffector::CAO_SET) colourOperation = token[TOKEN_COLOUR_SET];
			serializer->writeLine(token[TOKEN_COLOUR_OPERATION], colourOperation, 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
