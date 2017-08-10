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

#include "ParticleAffectors/ParticleUniverseTextureAnimator.h"
#include "ParticleAffectors/ParticleUniverseTextureAnimatorTokens.h"
#include "ParticleUniverseScriptDeserializerTokens.h"

namespace ParticleUniverse
{
	//-----------------------------------------------------------------------
	bool TextureAnimatorTranslator::translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		PropertyAbstractNode* prop = reinterpret_cast<PropertyAbstractNode*>(node.get());
		ParticleAffector* af = any_cast<ParticleAffector*>(prop->parent->context);
		TextureAnimator* affector = static_cast<TextureAnimator*>(af);

		if (prop->name == token[TOKEN_TIME_STEP])
		{
			// Property: time_step
			if (passValidateProperty(compiler, prop, token[TOKEN_TIME_STEP], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					affector->setAnimationTimeStep(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_TEXANIM_TIME_STEP])
		{
			// Property: time_step_animation (deprecated and replaced by 'time_step')
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_TIME_STEP], VAL_REAL))
			{
				Real val = 0.0f;
				if(getReal(prop->values.front(), &val))
				{
					affector->setAnimationTimeStep(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_START_TEXANIM_TEXCOORDS_RANGE])
		{
			// Property: start_texture_coords_range
			if (passValidateProperty(compiler, prop, token[TOKEN_START_TEXANIM_TEXCOORDS_RANGE], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					affector->setTextureCoordsStart(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_TEXANIM_TEXCOORDS_START])
		{
			// Property: texture_coords_start (deprecated and replaced by start_texture_coords_range)
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_TEXCOORDS_START], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					affector->setTextureCoordsStart(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_END_TEXANIM_TEXCOORDS_RANGE])
		{
			// Property: texture_coords_end
			if (passValidateProperty(compiler, prop, token[TOKEN_END_TEXANIM_TEXCOORDS_RANGE], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					affector->setTextureCoordsEnd(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_TEXANIM_TEXCOORDS_END])
		{
			// Property: texture_coords_end (deprecated and replaced by end_texture_coords_range)
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_TEXCOORDS_END], VAL_UINT))
			{
				uint val = 0;
				if(getUInt(prop->values.front(), &val))
				{
					affector->setTextureCoordsEnd(val);
					return true;
				}
			}
		}
		else if (prop->name == token[TOKEN_TEXANIM_ANIMATION_TYPE])
		{
			// Property: texture_animation_type
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_ANIMATION_TYPE], VAL_STRING))
			{
				String val;
				if(getString(prop->values.front(), &val))
				{
					if (val == token[TOKEN_TEXANIM_LOOP])
					{
						affector->setTextureAnimationType(TextureAnimator::TAT_LOOP);
						return true;
					}
					else if (val == token[TOKEN_TEXANIM_UP_DOWN])
					{
						affector->setTextureAnimationType(TextureAnimator::TAT_UP_DOWN);
						return true;
					}
					else if (val == token[TOKEN_TEXANIM_RANDOM])
					{
						affector->setTextureAnimationType(TextureAnimator::TAT_RANDOM);
						return true;
					}
				}
			}
		}
		else if (prop->name == token[TOKEN_TEXANIM_START_RANDOM])
		{
			// Property: texture_start_random
			if (passValidateProperty(compiler, prop, token[TOKEN_TEXANIM_START_RANDOM], VAL_BOOL))
			{
				bool val;
				if(getBoolean(prop->values.front(), &val))
				{
					affector->setStartRandom(val);
					return true;
				}
			}
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool TextureAnimatorTranslator::translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
	{
		// No objects
		return false;
	}
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	void TextureAnimatorWriter::write(ParticleScriptSerializer* serializer, const IElement* element)
	{
		// Cast the element to a TextureAnimator
		const TextureAnimator* affector = static_cast<const TextureAnimator*>(element);

		// Write the header of the TextureAnimator
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (affector->getAnimationTimeStep() != TextureAnimator::DEFAULT_TIME_STEP) serializer->writeLine(
			token[TOKEN_TIME_STEP], StringConverter::toString(affector->getAnimationTimeStep()), 12);
		if (affector->getTextureCoordsStart() != TextureAnimator::DEFAULT_TEXCOORDS_START) serializer->writeLine(
			token[TOKEN_START_TEXANIM_TEXCOORDS_RANGE], StringConverter::toString(affector->getTextureCoordsStart()), 12);
		if (affector->getTextureCoordsEnd() != TextureAnimator::DEFAULT_TEXCOORDS_END) serializer->writeLine(
			token[TOKEN_END_TEXANIM_TEXCOORDS_RANGE], StringConverter::toString(affector->getTextureCoordsEnd()), 12);
		if (affector->isStartRandom() != TextureAnimator::DEFAULT_START_RANDOM) serializer->writeLine(
			token[TOKEN_TEXANIM_START_RANDOM], StringConverter::toString(affector->isStartRandom()), 12);

		if (affector->getTextureAnimationType() != TextureAnimator::DEFAULT_ANIMATION_TYPE) 
		{
			String animationType = token[TOKEN_TEXANIM_LOOP];
			if (affector->getTextureAnimationType() == TextureAnimator::TAT_LOOP) animationType = token[TOKEN_TEXANIM_LOOP];
			else if (affector->getTextureAnimationType() == TextureAnimator::TAT_UP_DOWN) animationType = token[TOKEN_TEXANIM_UP_DOWN];
			else if (affector->getTextureAnimationType() == TextureAnimator::TAT_RANDOM) animationType = token[TOKEN_TEXANIM_RANDOM];
			serializer->writeLine(token[TOKEN_TEXANIM_ANIMATION_TYPE], animationType, 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

}
