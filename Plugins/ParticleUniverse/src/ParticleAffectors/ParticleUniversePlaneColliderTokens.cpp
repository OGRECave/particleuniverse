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

#include "ParticleAffectors/ParticleUniversePlaneCollider.h"
#include "ParticleAffectors/ParticleUniversePlaneColliderTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool PlaneColliderTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleAffector* af = any_cast<ParticleAffector*>(prop->parent->context);
		PlaneCollider* affector = static_cast<PlaneCollider*>(af);

		if (prop->name == token[TOKEN_NORMAL])
		{
			// Property: normal
			if (passValidateProperty(compiler, prop, token[TOKEN_NORMAL], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					affector->setNormal(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_PLANECOLL_NORMAL])
		{
			// Property: plane_collider_normal (deprecated and replaced by 'normal')
			if (passValidateProperty(compiler, prop, token[TOKEN_PLANECOLL_NORMAL], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					affector->setNormal(val);
					return true;
				}
			}
		}
		else
		{
			// Parse the BaseCollider
			BaseColliderTranslator baseColliderTranslator;
			return baseColliderTranslator.translateChildProperty(compiler, node);
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool PlaneColliderTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void PlaneColliderWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a PlaneCollider
		const PlaneCollider* affector = static_cast<const PlaneCollider*>(element);

		// Write the header of the PlaneCollider
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		BaseColliderWriter::write(serializer, element);

		// Write own attributes
		if (affector->getNormal() != PlaneCollider::DEFAULT_NORMAL) serializer->writeLine(
			token[TOKEN_NORMAL], StringConverter::toString(affector->getNormal()), 12);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
