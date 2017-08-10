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

#include "ParticleAffectors/ParticleUniverseScaleAffector.h"
#include "ParticleAffectors/ParticleUniverseScaleAffectorTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool ScaleAffectorTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleAffector* af = any_cast<ParticleAffector*>(prop->parent->context);
		ScaleAffector* affector = static_cast<ScaleAffector*>(af);

		if (prop->name == token[TOKEN_SCALE_XYZ_SCALE])
		{
			// Property: xyz_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_SCALE_XYZ_SCALE], VAL_REAL))
			{
				ParticleUniverse::Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					DynamicAttributeFixed* dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setDynScaleXYZ(dynamicAttributeFixed);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_SCALE_X_SCALE])
		{
			// Property: x_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_SCALE_X_SCALE], VAL_REAL))
			{
				ParticleUniverse::Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					DynamicAttributeFixed* dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setDynScaleX(dynamicAttributeFixed);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_SCALE_Y_SCALE])
		{
			// Property: y_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_SCALE_Y_SCALE], VAL_REAL))
			{
				ParticleUniverse::Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					DynamicAttributeFixed* dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setDynScaleY(dynamicAttributeFixed);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_SCALE_Z_SCALE])
		{
			// Property: z_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_SCALE_Z_SCALE], VAL_REAL))
			{
				ParticleUniverse::Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					DynamicAttributeFixed* dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setDynScaleZ(dynamicAttributeFixed);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_SINCE_START_SYSTEM])
		{
			// Property: since_start_system
			if (passValidateProperty(compiler, prop, token[TOKEN_SINCE_START_SYSTEM], VAL_BOOL))
			{
				bool val;
				if(getBoolean(prop->values.front(), &val))
				{
					affector->setSinceStartSystem(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool ScaleAffectorTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		ObjectAbstractNode* child = reinterpret_cast<ObjectAbstractNode*>(node.get());
		ParticleAffector* af = any_cast<ParticleAffector*>(child->parent->context);
		ScaleAffector* affector = static_cast<ScaleAffector*>(af);

		DynamicAttributeTranslator dynamicAttributeTranslator;
		if (child->cls == token[TOKEN_SCALE_XYZ_SCALE])
		{
			// Property: xyz_scale
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute* dynamicAttribute = any_cast<DynamicAttribute*>(child->context);
			affector->setDynScaleXYZ(dynamicAttribute);
			return true;
		}
		else if (child->cls == token[TOKEN_SCALE_X_SCALE])
		{
			// Property: x_scale
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute* dynamicAttribute = any_cast<DynamicAttribute*>(child->context);
			affector->setDynScaleX(dynamicAttribute);
			return true;
		}
		else if (child->cls == token[TOKEN_SCALE_Y_SCALE])
		{
			// Property: y_scale
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute* dynamicAttribute = any_cast<DynamicAttribute*>(child->context);
			affector->setDynScaleY(dynamicAttribute);
			return true;
		}
		else if (child->cls == token[TOKEN_SCALE_Z_SCALE])
		{
			// Property: z_scale
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute* dynamicAttribute = any_cast<DynamicAttribute*>(child->context);
			affector->setDynScaleZ(dynamicAttribute);
			return true;
		}

		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void ScaleAffectorWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a ScaleAffector
		const ScaleAffector* affector = static_cast<const ScaleAffector*>(element);

		// Write the header of the ScaleAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		DynamicAttributeFactory dynamicAttributeFactory;
		if (dynamicAttributeFactory._getDefaultValue(affector->getDynScaleXYZ()) != ScaleAffector::DEFAULT_XYZ_SCALE)
		{
			serializer->setKeyword(token[TOKEN_SCALE_XYZ_SCALE]);
			serializer->setIndentation(12);
			dynamicAttributeFactory.write(serializer, affector->getDynScaleXYZ());
		}
		if (dynamicAttributeFactory._getDefaultValue(affector->getDynScaleX()) != ScaleAffector::DEFAULT_X_SCALE)
		{
			serializer->setKeyword(token[TOKEN_SCALE_X_SCALE]);
			dynamicAttributeFactory.write(serializer, affector->getDynScaleX());
		}
		if (dynamicAttributeFactory._getDefaultValue(affector->getDynScaleY()) != ScaleAffector::DEFAULT_Y_SCALE)
		{
			serializer->setKeyword(token[TOKEN_SCALE_Y_SCALE]);
			dynamicAttributeFactory.write(serializer, affector->getDynScaleY());
		}
		if (dynamicAttributeFactory._getDefaultValue(affector->getDynScaleZ()) != ScaleAffector::DEFAULT_Z_SCALE)
		{
			serializer->setKeyword(token[TOKEN_SCALE_Z_SCALE]);
			dynamicAttributeFactory.write(serializer, affector->getDynScaleZ());
		}
		if (affector->isSinceStartSystem()) serializer->writeLine(
			token[TOKEN_SINCE_START_SYSTEM], StringConverter::toString(affector->isSinceStartSystem()), 12);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
