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

#include "Externs/ParticleUniverseSphereColliderExtern.h"
#include "Externs/ParticleUniverseSphereColliderExternTokens.h"
#include "ParticleAffectors/ParticleUniverseBaseColliderTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool SphereColliderExternTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		Extern* ex = any_cast<Extern*>(prop->parent->context);
		SphereColliderExtern* externObject = static_cast<SphereColliderExtern*>(ex);
//		BaseColliderTranslator baseColliderTranslator;

		if (prop->name == token[TOKEN_RADIUS])
		{
			// Property: radius
			if (passValidateProperty(compiler, prop, token[TOKEN_RADIUS], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					externObject->setRadius(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_SPHERE_COLLIDER_RADIUS])
		{
			// Property: sphere_collider_radius (Deprecated; replaced by radius)
			if (passValidateProperty(compiler, prop, token[TOKEN_SPHERE_COLLIDER_RADIUS], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					externObject->setRadius(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_DISTANCE_THRESHOLD])
		{
			// Property: distance_threshold
			if (passValidateProperty(compiler, prop, token[TOKEN_DISTANCE_THRESHOLD], VAL_REAL))
			{
				Real val;
				if(getReal(prop->values.front(), &val))
				{
					externObject->setDistanceThreshold(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_EXTERN_DISTANCE_THRESHOLD])
		{
			// Property: attachable_distance_threshold (deprecated and replaced by 'distance_threshold')
			if (passValidateProperty(compiler, prop, token[TOKEN_EXTERN_DISTANCE_THRESHOLD], VAL_REAL))
			{
				Real val;
				if(getReal(prop->values.front(), &val))
				{
					externObject->setDistanceThreshold(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_FRICTION])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_FRICTION], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					externObject->setFriction(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_COLLIDER_FRICTION])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_COLLIDER_FRICTION], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					externObject->setFriction(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_BOUNCYNESS])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_BOUNCYNESS], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					externObject->setBouncyness(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_COLLIDER_BOUNCYNESS])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_COLLIDER_BOUNCYNESS], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					externObject->setBouncyness(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_INTERSECTION])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_INTERSECTION], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_COLLIDER_INTERSECTION_POINT])
					{
						externObject->setIntersectionType(BaseCollider::IT_POINT);
						return true;
					}
					else if (val == token[TOKEN_COLLIDER_INTERSECTION_BOX])
					{
						externObject->setIntersectionType(BaseCollider::IT_BOX);
						return true;
					}
				}
			}
		}
		else if (prop->name == token[TOKEN_COLLIDER_INTERSECTION])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_COLLIDER_INTERSECTION], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_COLLIDER_INTERSECTION_POINT])
					{
						externObject->setIntersectionType(BaseCollider::IT_POINT);
						return true;
					}
					else if (val == token[TOKEN_COLLIDER_INTERSECTION_BOX])
					{
						externObject->setIntersectionType(BaseCollider::IT_BOX);
						return true;
					}
				}
			}
		}
		else if (prop->name == token[TOKEN_COLLIDER_COLLISION_TYPE])
		{
			if (passValidateProperty(compiler, prop, token[TOKEN_COLLIDER_COLLISION_TYPE], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_COLLIDER_BOUNCE])
					{
						externObject->setCollisionType(BaseCollider::CT_BOUNCE);
						return true;
					}
					else if (val == token[TOKEN_COLLIDER_FLOW])
					{
						externObject->setCollisionType(BaseCollider::CT_FLOW);
						return true;
					}
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool SphereColliderExternTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void SphereColliderExternWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a SphereColliderExtern
		const Extern* externObject = static_cast<const Extern*>(element);
		const SphereColliderExtern* sphereColliderExtern = static_cast<const SphereColliderExtern*>(externObject);
		serializer->writeLine(token[TOKEN_EXTERN], externObject->getExternType(), externObject->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ExternWriter::write(serializer, element);
		AttachableWriter::write(serializer, element);

		// Write own attributes
		if (sphereColliderExtern->getRadius() != SphereCollider::DEFAULT_RADIUS) serializer->writeLine(
			token[TOKEN_RADIUS], StringConverter::toString(sphereColliderExtern->getRadius()), 12);
		if (sphereColliderExtern->getFriction() != BaseCollider::DEFAULT_FRICTION) serializer->writeLine(
			token[TOKEN_FRICTION], StringConverter::toString(sphereColliderExtern->getFriction()), 12);
		if (sphereColliderExtern->getBouncyness() != BaseCollider::DEFAULT_BOUNCYNESS) serializer->writeLine(
			token[TOKEN_BOUNCYNESS], StringConverter::toString(sphereColliderExtern->getBouncyness()), 12);
		if (sphereColliderExtern->getIntersectionType() != BaseCollider::DEFAULT_INTERSECTION_TYPE) 
		{
			String intersectionType = token[TOKEN_COLLIDER_INTERSECTION_POINT];
			if (sphereColliderExtern->getIntersectionType() == BaseCollider::IT_BOX) intersectionType = token[TOKEN_COLLIDER_INTERSECTION_BOX];
			serializer->writeLine(token[TOKEN_INTERSECTION], intersectionType, 12);
		}
		if (sphereColliderExtern->getCollisionType() != BaseCollider::DEFAULT_COLLISION_TYPE) 
		{
			String collisionType = token[TOKEN_COLLIDER_BOUNCE];
			if (sphereColliderExtern->getCollisionType() == BaseCollider::CT_FLOW) collisionType = token[TOKEN_COLLIDER_FLOW];
			else if (sphereColliderExtern->getCollisionType() == BaseCollider::CT_NONE) collisionType = token[TOKEN_COLLIDER_NONE];
			serializer->writeLine(token[TOKEN_COLLIDER_COLLISION_TYPE], collisionType, 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
