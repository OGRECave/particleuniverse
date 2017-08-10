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

#include "ParticleAffectors/ParticleUniverseInterParticleCollider.h"
#include "ParticleAffectors/ParticleUniverseInterParticleColliderTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool InterParticleColliderTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleAffector* af = any_cast<ParticleAffector*>(prop->parent->context);
		InterParticleCollider* affector = static_cast<InterParticleCollider*>(af);

		if (prop->name == token[TOKEN_ADJUSTMENT])
		{
			// Property: adjustment
			if (passValidateProperty(compiler, prop, token[TOKEN_ADJUSTMENT], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					affector->setAdjustment(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_IPC_ADJUSTMENT])
		{
			// Property: ip_adjustment (deprecated and replaced by 'adjustment')
			if (passValidateProperty(compiler, prop, token[TOKEN_IPC_ADJUSTMENT], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					affector->setAdjustment(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_COLLISION_RESPONSE])
		{
			// Property: collision_response
			if (passValidateProperty(compiler, prop, token[TOKEN_COLLISION_RESPONSE], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_IPC_AVERAGE_VELOCITY])
					{
						affector->setInterParticleCollisionResponse(InterParticleCollider::IPCR_AVERAGE_VELOCITY);
						return true;
					}
					else if (val == token[TOKEN_IPC_ANGLE_BASED_VELOCITY])
					{
						affector->setInterParticleCollisionResponse(InterParticleCollider::IPCR_ANGLE_BASED_VELOCITY);
						return true;
					}
				}
			}
		}
		else if (prop->name == token[TOKEN_IPC_COLLISION_RESPONSE])
		{
			// Property: ip_collision_response (deprecated and replaced by 'collision_response')
			if (passValidateProperty(compiler, prop, token[TOKEN_IPC_COLLISION_RESPONSE], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_IPC_AVERAGE_VELOCITY])
					{
						affector->setInterParticleCollisionResponse(InterParticleCollider::IPCR_AVERAGE_VELOCITY);
						return true;
					}
					else if (val == token[TOKEN_IPC_ANGLE_BASED_VELOCITY])
					{
						affector->setInterParticleCollisionResponse(InterParticleCollider::IPCR_ANGLE_BASED_VELOCITY);
						return true;
					}
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool InterParticleColliderTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void InterParticleColliderWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a InterParticleCollider
		const InterParticleCollider* affector = static_cast<const InterParticleCollider*>(element);

		// Write the header of the InterParticleCollider
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (affector->getAdjustment() != InterParticleCollider::DEFAULT_ADJUSTMENT) serializer->writeLine(
			token[TOKEN_ADJUSTMENT], StringConverter::toString(affector->getAdjustment()), 12);

		if (affector->getInterParticleCollisionResponse() != InterParticleCollider::DEFAULT_COLLISION_RESPONSE) 
		{
			String collisionResponse = token[TOKEN_IPC_ANGLE_BASED_VELOCITY];
			if (affector->getInterParticleCollisionResponse() == InterParticleCollider::IPCR_AVERAGE_VELOCITY) collisionResponse = token[TOKEN_IPC_AVERAGE_VELOCITY];
			serializer->writeLine(token[TOKEN_COLLISION_RESPONSE], collisionResponse, 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
