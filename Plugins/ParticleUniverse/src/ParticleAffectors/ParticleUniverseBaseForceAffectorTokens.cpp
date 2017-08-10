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

#include "ParticleAffectors/ParticleUniverseBaseForceAffector.h"
#include "ParticleAffectors/ParticleUniverseBaseForceAffectorTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool BaseForceAffectorTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleAffector* af = any_cast<ParticleAffector*>(prop->parent->context);
		BaseForceAffector* affector = static_cast<BaseForceAffector*>(af); // Do not check type; assume the type is correct.

		if (prop->name == token[TOKEN_FORCE_VECTOR])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_VECTOR], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					affector->setForceVector(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_FORCE_AFF_VECTOR])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_AFF_VECTOR], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					affector->setForceVector(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_FORCE_APPLICATION])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_APPLICATION], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_FORCE_ADD])
					{
						affector->setForceApplication(BaseForceAffector::FA_ADD);
						return true;
					}
					else if (val == token[TOKEN_FORCE_AVERAGE])
					{
						affector->setForceApplication(BaseForceAffector::FA_AVERAGE);
						return true;
					}
				}
			}
		}
		else if (prop->name == token[TOKEN_FORCE_AFF_APPLICATION])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_AFF_APPLICATION], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_FORCE_ADD])
					{
						affector->setForceApplication(BaseForceAffector::FA_ADD);
						return true;
					}
					else if (val == token[TOKEN_FORCE_AVERAGE])
					{
						affector->setForceApplication(BaseForceAffector::FA_AVERAGE);
						return true;
					}
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool BaseForceAffectorTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void BaseForceAffectorWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a BaseForceAffector
		const BaseForceAffector* affector = static_cast<const BaseForceAffector*>(element);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (affector->getForceVector() != BaseForceAffector::DEFAULT_FORCE_VECTOR) serializer->writeLine(
			token[TOKEN_FORCE_VECTOR], StringConverter::toString(affector->getForceVector()), 12);

		if (affector->getForceApplication() != BaseForceAffector::DEFAULT_FORCE_APPL) 
		{
			String application = token[TOKEN_FORCE_ADD];
			if (affector->getForceApplication() == BaseForceAffector::FA_AVERAGE) application = token[TOKEN_FORCE_AVERAGE];
			serializer->writeLine(token[TOKEN_FORCE_APPLICATION], application, 12);
		}
	}

}
