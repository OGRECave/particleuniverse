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

#include "ParticleUniverseSystemManagerTokens.h"

namespace ParticleUniverse
{
	//-------------------------------------------------------------------------
	BuiltinScriptTranslatorManager::BuiltinScriptTranslatorManager()
	{
	}
	//-------------------------------------------------------------------------
	size_t BuiltinScriptTranslatorManager::getNumTranslators() const
	{
		return 9;
	}
	//-------------------------------------------------------------------------
	ScriptTranslator* BuiltinScriptTranslatorManager::getTranslator(const AbstractNodePtr &node)
	{
		ScriptTranslator* translator = 0;

		if(node->type == ANT_OBJECT)
		{
			ObjectAbstractNode* obj = reinterpret_cast<ObjectAbstractNode*>(node.get());
			ObjectAbstractNode* parent = obj->parent ? reinterpret_cast<ObjectAbstractNode*>(obj->parent) : 0;
			if(obj->cls == token[TOKEN_SYSTEM])
			{
				// Parse Particle System
				translator = &mSystemTranslator;
			}
			else if(obj->cls == token[TOKEN_ALIAS])
			{
				// Parse the Alias
				translator = &mAliasTranslator;
			}
			else if(obj->cls == token[TOKEN_TECHNIQUE] && parent && (parent->cls == token[TOKEN_SYSTEM] || parent->cls == token[TOKEN_ALIAS]))
			{
				// Parse Particle Technique
				translator = &mTechniqueTranslator;
			}
			else if(obj->cls == token[TOKEN_RENDERER] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS]))
			{
				// Parse Particle Renderer
				translator = &mRendererTranslator;
			}
			else if(obj->cls == token[TOKEN_EMITTER] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS]))
			{
				// Parse Particle Emitter
				translator = &mEmitterTranslator;
			}
			else if(obj->cls == token[TOKEN_AFFECTOR] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS]))
			{
				// Parse Particle Affector
				translator = &mAffectorTranslator;
			}
			else if(obj->cls == token[TOKEN_BEHAVIOUR] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS]))
			{
				// Parse Particle Behaviour
				translator = &mBehaviourTranslator;
			}
			else if(obj->cls == token[TOKEN_OBSERVER] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS]))
			{
				// Parse Particle Observer
				translator = &mObserverTranslator;
			}
			else if(obj->cls == token[TOKEN_HANDLER] && parent && (parent->cls == token[TOKEN_OBSERVER] || parent->cls == token[TOKEN_ALIAS]))
			{
				// Parse Particle Event Handler
				translator = &mParticleEventHandlerTranslator;
			}
			else if(obj->cls == token[TOKEN_EXTERN] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS]))
			{
				// Parse Extern
				translator = &mExternTranslator;
			}
		}

		return translator;
	}
}
