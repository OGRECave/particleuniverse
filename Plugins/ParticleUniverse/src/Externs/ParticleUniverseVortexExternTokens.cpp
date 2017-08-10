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

#include "Externs/ParticleUniverseVortexExtern.h"
#include "Externs/ParticleUniverseVortexExternTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"
#include "ParticleUniverseAny.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool VortexExternTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		Extern* ex = any_cast<Extern*>(prop->parent->context);
		VortexExtern* externObject = static_cast<VortexExtern*>(ex);

		if (prop->name == token[TOKEN_ROTATION_AXIS])
		{
			// Property: rotation_axis
			if (passValidateProperty(compiler, prop, token[TOKEN_ROTATION_AXIS], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					externObject->setRotationVector(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_VORTEX_ROTATION_VECTOR])
		{
			// Property: vortex_aff_vector
			if (passValidateProperty(compiler, prop, token[TOKEN_SCENE_SCALE], VAL_VECTOR3))
			{
				Vector3 val;
				if(getVector3(prop->values.begin(), prop->values.end(), &val))
				{
					externObject->setRotationVector(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_ROTATION_SPEED])
		{
			// Property: rotation_speed
			if (passValidateProperty(compiler, prop, token[TOKEN_VORTEX_ROTATION_SPEED], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					DynamicAttributeFixed* dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					externObject->setRotationSpeed(dynamicAttributeFixed);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_VORTEX_ROTATION_SPEED])
		{
			// Property: vortex_aff_speed
			if (passValidateProperty(compiler, prop, token[TOKEN_VORTEX_ROTATION_SPEED], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					DynamicAttributeFixed* dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					externObject->setRotationSpeed(dynamicAttributeFixed);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool VortexExternTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		ObjectAbstractNode* child = reinterpret_cast<ObjectAbstractNode*>(node.get());
		Extern* ex = any_cast<Extern*>(child->parent->context);
		VortexExtern* externObject = static_cast<VortexExtern*>(ex);

		DynamicAttributeTranslator dynamicAttributeTranslator;
		if (child->cls == token[TOKEN_ROTATION_SPEED])
		{
			// Property: rotation_speed
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute* dynamicAttribute = any_cast<DynamicAttribute*>(child->context);
			externObject->setRotationSpeed(dynamicAttribute);
			return true;
		}
		else if (child->cls == token[TOKEN_VORTEX_ROTATION_SPEED])
		{
			// Property: vortex_aff_speed
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute* dynamicAttribute = any_cast<DynamicAttribute*>(child->context);
			externObject->setRotationSpeed(dynamicAttribute);
			return true;
		}

		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void VortexExternWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a VortexExtern
		const Extern* externObject = static_cast<const Extern*>(element);
		const VortexExtern* vortexExtern = static_cast<const VortexExtern*>(externObject);
		serializer->writeLine(token[TOKEN_EXTERN], externObject->getExternType(), externObject->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ExternWriter::write(serializer, element);
		AttachableWriter::write(serializer, element);

		// Write own attributes
		if (vortexExtern->getRotationVector() != VortexAffector::DEFAULT_ROTATION_VECTOR) serializer->writeLine(
			token[TOKEN_ROTATION_AXIS], StringConverter::toString(vortexExtern->getRotationVector()), 12);
		DynamicAttributeFactory dynamicAttributeFactory;
		if (dynamicAttributeFactory._getDefaultValue(vortexExtern->getRotationSpeed()) != VortexAffector::DEFAULT_ROTATION_SPEED)
		{
			serializer->setKeyword(token[TOKEN_ROTATION_SPEED]);
			serializer->setIndentation(12);
			dynamicAttributeFactory.write(serializer, vortexExtern->getRotationSpeed());
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
