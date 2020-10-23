/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "wx/ogre/utils.h"
#include "ParticleUniverseEditTab.h"
#include "ParticleUniverseEditor.h"
#include "ParticleUniverseSystemPropertyWindow.h"
#include "ParticleUniverseTechniquePropertyWindow.h"
#include "ParticleUniverseEmitterPropertyWindow.h"
#include "ParticleUniverseObserverPropertyWindow.h"
#include "ParticleUniverseEventHandlerPropertyWindow.h"
#include "ParticleUniverseAffectorPropertyWindow.h"
#include "ParticleUniverseBehaviourPropertyWindow.h"
#include "ParticleUniverseExternPropertyWindow.h"
#include "ParticleUniverseOgreControlComponent.h"
#include "ParticleUniverseSystem.h"
#include "ParticleUniverseUtils.h"

//-----------------------------------------------------------------------
EditTab::EditTab(wxWindow* parentNotebook, wxWindow* rootParent) : wxMDIParentFrame(
		parentNotebook,
		ID_TAB_EDIT, 
		wxT(""), 
		wxPoint(rootParent->GetPosition().x + TAB_POS_X + 10, rootParent->GetPosition().y + TAB_POS_Y + 72),
		wxSize(MAX_WIDTH, MAX_HEIGHT), 
		wxVSCROLL | wxHSCROLL | wxFRAME_FLOAT_ON_PARENT,
		wxT("editframe")),
	mRootParent(rootParent),
	mNumberOfSystems(0),
	mConnectionMode(CM_CONNECT_NONE),
	mStartConnector(0),
	mEndConnector(0),
	mSystemCounter(0),
	mTechniqueCounter(0),
	mRendererCounter(0),
	mEmitterCounter(0),
	mAffectorCounter(0),
	mObserverCounter(0),
	mHandlerCounter(0),	
	mBehaviourCounter(0),
	mExternCounter(0),
	mOffsetX(48),
	mOffsetY(8),
	mOffsetFraction(0.05f),
	mScale(0.75f),
	mEditChanged(false)
{
	// Internationize the strings
	CT_SYSTEM = _("System");
	CT_TECHNIQUE = _("Technique");
	CT_RENDERER = _("Renderer");
	CT_EMITTER = _("Emitter");
	CT_AFFECTOR = _("Affector");
	CT_OBSERVER = _("Observer");
	CT_HANDLER = _("Handler");
	CT_BEHAVIOUR = _("Behaviour");
	CT_EXTERN = _("Extern");
	CT_VISUAL = _("Visual");
	CRD_INCLUDES = _("includes");
	CRD_INCLUDED_BY = _("included by");
	CRD_EXCLUDES = _("excludes");
	CRD_EXCLUDED_BY = _("excluded by");
	CRD_HANDLES = _("handles");
	CRD_HANDLED_BY = _("handled by");
	CRD_EMITS = _("emits");
	CRD_EMITTED_BY = _("emitted by");
	CRD_AFFECTS = _("affects");
	CRD_AFFECTED_BY = _("affected by");
	CRD_OBSERVES = _("observes");
	CRD_OBSERVED_BY = _("observed by");
	CRD_INTERFACES = _("interfaces");
	CRD_INTERFACED_BY = _("interfaced by");
	CRD_SLAVES = _("slaves");
	CRD_SLAVE_OF = _("slave of");
	CRD_MASTER = _("master of");
	CRD_ENABLES = _("enables");
	CRD_ENABLED_BY = _("enabled by");
	CRD_FORCES = _("forces");
	CRD_FORCED_BY = _("forced by");
	CRD_PLACES = _("places");
	CRD_PLACED_BY = _("placed by");
	CRD_UNKNOWN = _("connection with");
	CST_HANDLER_DO_PLACEMENT_PARTICLE = ogre2wxTranslate(HANDLER_DO_PLACEMENT_PARTICLE);
	CST_EMITTER_SLAVE = ogre2wxTranslate(EMITTER_SLAVE);
	CST_HANDLER_DO_ENABLE_COMPONENT = ogre2wxTranslate(HANDLER_DO_ENABLE_COMPONENT);
	CST_HANDLER_DO_AFFECTOR = ogre2wxTranslate(HANDLER_DO_AFFECTOR);
	CST_RENDERER_BILLBOARD = ogre2wxTranslate(RENDERER_BILLBOARD);
	CST_EMITTER_BOX = ogre2wxTranslate(EMITTER_BOX);
	CST_AFFECTOR_ALIGN = ogre2wxTranslate(AFFECTOR_ALIGN);
	CST_OBSERVER_ON_CLEAR = ogre2wxTranslate(OBSERVER_ON_CLEAR);
	CST_EXTERN_BOX_COLLIDER = ogre2wxTranslate(EXTERN_BOX_COLLIDER);

	mClientWindow = new ClientWindow(this);
	mEditTools = new EditTools(this);
	wxPoint pos(0.93 * mRootParent->GetSize().x, GetPosition().y + 8);
	mEditTools->SetPosition(pos);
	mCanvas = new EditCanvas(this);
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(EditTab::OnMouseWheel));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(EditTab::OnKeyPressed));
}
//-----------------------------------------------------------------------
EditTab::~EditTab(void)
{
	mEditTools = 0;
}
//-----------------------------------------------------------------------
void EditTab::adjustPosition(void)
{
	SetPosition(wxPoint(mRootParent->GetPosition().x + TAB_POS_X + 10, mRootParent->GetPosition().y + TAB_POS_Y + 72));
	if (mCanvas)
	{
		mCanvas->SetFocus();
	}
}
//-----------------------------------------------------------------------
unsigned int EditTab::getNumberOfSystems() const
{
	return mNumberOfSystems;
}
//-----------------------------------------------------------------------
unsigned int EditTab::getNumberOfComponents() const
{
	return mComponents.size();
}
//-----------------------------------------------------------------------
void EditTab::pushSystem(EditComponent* system)
{
	if (mNumberOfSystems == 0)
	{
		mComponents.push_back(system);
		mNumberOfSystems++;
	}
}
//-----------------------------------------------------------------------
void EditTab::pushComponent(EditComponent* component)
{
	mComponents.push_back(component);
}
//-----------------------------------------------------------------------
void EditTab::popComponent(EditComponent* component)
{
	std::vector<EditComponent*>::iterator it;
	for (it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		if (component == *it)
		{
			mComponents.erase(it);
			break;
		}
	}
}
//-----------------------------------------------------------------------
void EditTab::refreshCanvas(void)
{
	mCanvas->Refresh();
}
//-----------------------------------------------------------------------
void EditTab::notifyFocusLeft(void)
{
	if (mCanvas)
	{
		mCanvas->SetFocus();
	}
}
//-----------------------------------------------------------------------
void EditTab::notifyComponentActivated(EditComponent* component)
{
	// A component is activated, check the mode
	if (mConnectionMode == CM_CONNECT_STARTING)
	{
		// Start connecting
		mCanvas->createMouseConnector(component);
		mStartConnector = component;
		setConnectionMode(CM_CONNECT_ENDING);
	}
	else if (mConnectionMode == CM_CONNECT_ENDING && component != mStartConnector)
	{
		if (isConnectionPossible(component))
		{
			// A policy has already been selected
			setConnectionMode(CM_CONNECT_STARTING);
			mEndConnector = component;
			mCanvas->destroyMouseConnector();
			ConnectionPolicy* policyEnd = mEndConnector->getSelectedPolicy();
			if (!policyEnd)
			{
				return;
			}

			mCanvas->connect(mStartConnector, mEndConnector, policyEnd->getRelation(), policyEnd->getColourCode(), policyEnd->getLineStyle());
			mEndConnector->addConnection(mStartConnector, policyEnd->getRelation(), policyEnd->getRelationDirection());

			// Use the relation of the policyEnd, because this is the same for Start and End
			mStartConnector->addConnection(mEndConnector, policyEnd->getRelation(), getOppositeRelationDirection(policyEnd->getRelationDirection()));
			notifyConnectionAdded(mStartConnector, mEndConnector, policyEnd->getRelation(), policyEnd->getRelationDirection());
			notifyConnectionsChanged();
			mStartConnector = 0;
			mEndConnector = 0;
			mCanvas->Refresh();
		}
	}
}
//-----------------------------------------------------------------------
void EditTab::notifyConnectionsChanged(void)
{
	mEditTools->notifyConnectionsChanged();
	mEditChanged = true; // If a connection is made or removed, the flag must be set.
}
//-----------------------------------------------------------------------
void EditTab::notifyReferers(EditComponent* component, SimpleEvent simpleEvent)
{
	// Notify other components that don't have a connection, but do have some kind of reference (name, pointer) to this component
	// Todo: Add other types?
	EditComponent* referer;
	std::vector<EditComponent*>::iterator it;
	for (it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		referer = *it;
		if (referer == component)
		{
			break;
		}
		else if (simpleEvent == SE_CLOSE)
		{
			if (component->getComponentType() == CT_TECHNIQUE && 
				referer->getComponentType() == CT_HANDLER && 
				referer->getComponentSubType() == CST_HANDLER_DO_PLACEMENT_PARTICLE)
			{
				ParticleUniverse::ParticleTechnique* techniqueComponent = static_cast<ParticleUniverse::ParticleTechnique*>(component->getPUElement());
				ParticleUniverse::DoPlacementParticleEventHandler* handler = static_cast<ParticleUniverse::DoPlacementParticleEventHandler*>(referer->getPUElement());

				if (techniqueComponent && handler)
				{
					ParticleUniverse::ParticleEmitter* emitter = handler->getForceEmitter();
					if (emitter)
					{
						ParticleUniverse::ParticleTechnique* techniqueReferer = emitter->getParentTechnique();
						if (techniqueReferer && techniqueReferer->getName() == techniqueComponent->getName())
						{
							// The handler has a reference to the technique.
							handler->removeAsListener();
						}
					}
				}
			}
		}
	}
}
//-----------------------------------------------------------------------
void EditTab::notifyComponentRemoved(EditComponent* component)
{
	mCanvas->removeConnection(component);
	popComponent(component);
	if (mConnectionMode == CM_CONNECT_ENDING)
	{
		resetConnectionMode();
	}

	if (component->getComponentType() == CT_TECHNIQUE)
	{
		mEditTools->notifyDeleteTechnique();
		destroyTechniqueFromComponent(component);
	}
	else if (component->getComponentType() == CT_RENDERER)
	{
		mEditTools->notifyDeleteRenderer();
		destroyRendererFromComponent(component);
	}
	else if (component->getComponentType() == CT_EMITTER)
	{
		mEditTools->notifyDeleteEmitter();
		destroyEmitterFromComponent(component);
	}
	else if (component->getComponentType() == CT_AFFECTOR)
	{
		mEditTools->notifyDeleteAffector();
		destroyAffectorFromComponent(component);
	}
	else if (component->getComponentType() == CT_OBSERVER)
	{
		mEditTools->notifyDeleteObserver();
		destroyObserverFromComponent(component);
	}
	else if (component->getComponentType() == CT_HANDLER)
	{
		mEditTools->notifyDeleteHandler();
		destroyHandlerFromComponent(component);
	}
	else if (component->getComponentType() == CT_BEHAVIOUR)
	{
		mEditTools->notifyDeleteBehaviour();
		destroyBehaviourFromComponent(component);
	}
	else if (component->getComponentType() == CT_EXTERN)
	{
		mEditTools->notifyDeleteExtern();
		destroyExternFromComponent(component);
	}

	mEditChanged = true; // If a component is deleted manually, the flag must be set.
	refreshCanvas();
}
//-----------------------------------------------------------------------
void EditTab::resetConnectionMode(void)
{
	// End connecting
	mCanvas->destroyMouseConnector();
	mStartConnector = 0;
	mEndConnector = 0;
	setConnectionMode(CM_CONNECT_NONE);
}
//-----------------------------------------------------------------------
void EditTab::notifyConnectionRemoved(EditComponent* node1, 
	EditComponent* node2, 
	ComponentRelation relation, 
	ComponentRelationDirection relationDirection)
{
	// Stop the system if needed
	bool wasStarted = _mustStopParticleSystem();

	// relationDirection is not used, but past as an argument anyway
	mCanvas->removeConnection(node1, node2, relation);

	// Deleting connections also means removing (not deleting) them from the ParticleSystem or its related components.
	if (relation == CR_INCLUDE)
	{
		if (!_processIncludeRemoved(node1, node2))
		{
			// Switch
			_processIncludeRemoved(node2, node1);
		}
	}
	else if (relation == CR_EXCLUDE)
	{
		if (!_processExcludeRemoved(node1, node2))
		{
			// Switch
			_processExcludeRemoved(node2, node1);
		}
	}
	else if (relation == CR_EMIT)
	{
		if (!_processEmitRemoved(node1, node2))
		{
			// Switch
			_processEmitRemoved(node2, node1);
		}
	}
	else if (relation == CR_INTERFACE)
	{
		if (!_processInterfaceRemoved(node1, node2))
		{
			// Switch
			_processInterfaceRemoved(node2, node1);
		}
	}
	else if (relation == CR_SLAVE)
	{
		if (!_processSlaveRemoved(node1, node2))
		{
			// Switch
			_processSlaveRemoved(node2, node1);
		}
	}
	else if (relation == CR_ENABLE)
	{
		if (!_processEnableRemoved(node1, node2))
		{
			// Switch
			_processEnableRemoved(node2, node1);
		}
	}
	else if (relation == CR_FORCE)
	{
		if (!_processForceRemoved(node1, node2))
		{
			// Switch
			_processForceRemoved(node2, node1);
		}
	}
	else if (relation == CR_PLACE)
	{
		if (!_processPlaceRemoved(node1, node2))
		{
			// Switch
			_processPlaceRemoved(node2, node1);
		}
	}

	refreshCanvas(); // V1.6
	_mustRestartParticleSystem(wasStarted);
}
//-----------------------------------------------------------------------
bool EditTab::_processIncludeRemoved(EditComponent* node1, EditComponent* node2)
{
	// If element is not set, ignore removing it.
	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_SYSTEM)
	{
		// Remove the technique from the system
		ParticleUniverse::ParticleTechnique* technique = static_cast<ParticleUniverse::ParticleTechnique*>(element2);
		if (technique->getParentSystem())
		{
			technique->getParentSystem()->removeTechnique(technique);
		}
		return true;
	}
	else if (node1->getComponentType() == CT_TECHNIQUE)
	{
		if (node2->getComponentType() == CT_RENDERER)
		{
			// Remove renderer from the technique
			ParticleUniverse::ParticleRenderer* renderer = static_cast<ParticleUniverse::ParticleRenderer*>(element2);
			if (renderer->getParentTechnique())
			{
				renderer->getParentTechnique()->removeRenderer(renderer);
			}
			return true;
		}
		else if (node2->getComponentType() == CT_EMITTER)
		{
			// Remove the emitter from the technique
			ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(element2);
			if (emitter->getParentTechnique())
			{
				if (emitter->_isMarkedForEmission())
				{
					emitter->getParentTechnique()->_unprepareEmitters(); // Destroy pool of emitted emitters
				}
				emitter->getParentTechnique()->removeEmitter(emitter);
			}
			return true;
		}
		else if (node2->getComponentType() == CT_AFFECTOR)
		{
			// Remove the affector from the technique
			ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(element2);
			if (affector->getParentTechnique())
			{
				if (affector->_isMarkedForEmission())
				{
					affector->getParentTechnique()->_unprepareAffectors(); // Destroy pool of emitted affectors
				}
				affector->getParentTechnique()->removeAffector(affector);
			}
			return true;
		}
		else if (node2->getComponentType() == CT_OBSERVER)
		{
			// Remove the observer from the technique
			ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(element2);
			if (observer->getParentTechnique())
			{
				observer->getParentTechnique()->removeObserver(observer);
			}
			return true;
		}
		else if (node2->getComponentType() == CT_BEHAVIOUR)
		{
			// Remove the behaviour from the technique
			ParticleUniverse::ParticleBehaviour* behaviour = static_cast<ParticleUniverse::ParticleBehaviour*>(element2);
			if (behaviour->getParentTechnique())
			{
				behaviour->getParentTechnique()->_removeBehaviourTemplate(behaviour);
			}
			return true;
		}
		else if (node2->getComponentType() == CT_EXTERN)
		{
			// Remove the extern from the technique
			ParticleUniverse::Extern* externObject = static_cast<ParticleUniverse::Extern*>(element2);
			if (externObject->getParentTechnique())
			{
				externObject->getParentTechnique()->removeExtern(externObject);
			}
			return true;
		}
	}
	else if (node1->getComponentType() == CT_OBSERVER)
	{
		if (node2->getComponentType() == CT_HANDLER)
		{
			// Remove the handler from the observer
			ParticleUniverse::ParticleEventHandler* handler = static_cast<ParticleUniverse::ParticleEventHandler*>(element2);
			if (handler->getParentObserver())
			{
				handler->getParentObserver()->removeEventHandler(handler);
			}
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processExcludeRemoved(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_AFFECTOR)
	{
		if (node2->getComponentType() == CT_EMITTER)
		{
			// Remove the emittername from the excluded names
			ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(element1);
			ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(element2);
			affector->removeEmitterToExclude(emitter->getName());
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processEmitRemoved(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (node1->getComponentType() == CT_EMITTER)
	{
		// Check what type of particle is emitted.
		ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(element1);
		ParticleUniverse::ParticleTechnique* technique;
		if (element2)
		{
			// Use the technique from the emitted component; this is preferred, because the technique from the emitter might be 0
			// (if the emitter component was closed, the CR_INCLUDE connection is deleted first and removes the emitter from its
			// parent technique).
			ParticleUniverse::ParticleEmitter* emittedEmitter = static_cast<ParticleUniverse::ParticleEmitter*>(element2);
			technique = emittedEmitter->getParentTechnique();
		}
		if (!technique)
		{
			// Use this one instead.
			technique = emitter->getParentTechnique();
		}

		switch(emitter->getEmitsType())
		{
			case ParticleUniverse::Particle::PT_EMITTER:
			{
				if (technique)
				{
					technique->_unprepareEmitters();
					technique->_markForEmission(emitter, false); // Unmarks the emitted component
				}
				emitter->setEmitsType(ParticleUniverse::ParticleEmitter::DEFAULT_EMITS);
				emitter->setEmitsName(Ogre::StringUtil::BLANK);
				return true;
			}
			break;
	
			case ParticleUniverse::Particle::PT_TECHNIQUE:
			{
				technique = emitter->getParentTechnique();
				if (technique)
				{
					technique->_unprepareTechnique();
					technique->_markForEmission(emitter, false); // Unmarks the emitted component
				}
				emitter->setEmitsType(ParticleUniverse::ParticleEmitter::DEFAULT_EMITS);
				emitter->setEmitsName(Ogre::StringUtil::BLANK);
				return true;
			}
			break;

			case ParticleUniverse::Particle::PT_AFFECTOR:
			{
				if (technique)
				{
					technique->_unprepareAffectors();
					technique->_markForEmission(emitter, false); // Unmarks the emitted component
				}
				emitter->setEmitsType(ParticleUniverse::ParticleEmitter::DEFAULT_EMITS);
				emitter->setEmitsName(Ogre::StringUtil::BLANK);
				return true;
			}
			break;

			case ParticleUniverse::Particle::PT_SYSTEM:
			{
				if (technique)
				{
					technique->_unprepareSystem();
					technique->_markForEmission(emitter, false); // Unmarks the emitted component
				}
				emitter->setEmitsType(ParticleUniverse::ParticleEmitter::DEFAULT_EMITS);
				emitter->setEmitsName(Ogre::StringUtil::BLANK);
				return true;
			}
			break;
		}
	}
	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processInterfaceRemoved(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_OBSERVER)
	{
		if (node2->getComponentType() == CT_HANDLER)
		{
			// Remove the handler from the observer
			ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(element1);
			ParticleUniverse::ParticleEventHandler* handler = static_cast<ParticleUniverse::ParticleEventHandler*>(element2);
			observer->removeEventHandler(handler);
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processSlaveRemoved(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	if (node1->getComponentType() == CT_EMITTER && node1->getComponentSubType() == CST_EMITTER_SLAVE)
	{
		ParticleUniverse::SlaveEmitter* emitter = static_cast<ParticleUniverse::SlaveEmitter*>(element1);
		if (emitter->getParentTechnique())
		{
			emitter->_unprepare(emitter->getParentTechnique());
		}
		emitter->setMasterTechniqueName(Ogre::StringUtil::BLANK);
		emitter->setMasterEmitterName(Ogre::StringUtil::BLANK);
		if (emitter->getParentTechnique())
		{
			emitter->_prepare(emitter->getParentTechnique());
		}
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processEnableRemoved(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_ENABLE_COMPONENT)
	{
		ParticleUniverse::DoEnableComponentEventHandler* handler = static_cast<ParticleUniverse::DoEnableComponentEventHandler*>(element1);
		handler->setComponentEnabled(true);
		handler->setComponentName(Ogre::StringUtil::BLANK);
		handler->setComponentType(ParticleUniverse::CT_EMITTER);
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processForceRemoved(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_AFFECTOR)
	{
		ParticleUniverse::DoAffectorEventHandler* handler = static_cast<ParticleUniverse::DoAffectorEventHandler*>(element1);
		handler->setAffectorName(Ogre::StringUtil::BLANK);
		handler->setPrePost(ParticleUniverse::DoAffectorEventHandler::DEFAULT_PRE_POST);
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processPlaceRemoved(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_PLACEMENT_PARTICLE)
	{
		ParticleUniverse::DoPlacementParticleEventHandler* handler = static_cast<ParticleUniverse::DoPlacementParticleEventHandler*>(element1);
		handler->removeAsListener();
		handler->setForceEmitterName(Ogre::StringUtil::BLANK);
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------
void EditTab::notifyConnectionAdded(EditComponent* node1, 
	EditComponent* node2, 
	ComponentRelation relation, 
	ComponentRelationDirection relationDirection)
{
	// Stop the system if needed
	bool wasStarted = _mustStopParticleSystem();

	if (relation == CR_INCLUDE)
	{
		if (!_processIncludeAdded(node1, node2))
		{
			// Switch
			_processIncludeAdded(node2, node1);
		}
	}
	else if (relation == CR_EXCLUDE)
	{
		if (!_processExcludeAdded(node1, node2))
		{
			// Switch
			_processExcludeAdded(node2, node1);
		}
	}
	else if (relation == CR_EMIT)
	{
		if (!_processEmitAdded(node1, node2, relationDirection))
		{
			// Switch
			_processEmitAdded(node2, node1, getOppositeRelationDirection(relationDirection));
		}
	}
	else if (relation == CR_INTERFACE)
	{
		if (!_processInterfaceAdded(node1, node2))
		{
			// Switch
			_processInterfaceAdded(node2, node1);
		}
	}
	else if (relation == CR_SLAVE)
	{
		if (!_processSlaveAdded(node1, node2))
		{
			// Switch
			_processSlaveAdded(node2, node1);
		}
	}
	else if (relation == CR_ENABLE)
	{
		if (!_processEnableAdded(node1, node2))
		{
			// Switch
			_processEnableAdded(node2, node1);
		}
	}
	else if (relation == CR_FORCE)
	{
		if (!_processForceAdded(node1, node2))
		{
			// Switch
			_processForceAdded(node2, node1);
		}
	}
	else if (relation == CR_PLACE)
	{
		if (!_processPlaceAdded(node1, node2))
		{
			// Switch
			_processPlaceAdded(node2, node1);
		}
	}

	_mustRestartParticleSystem(wasStarted);
}

//-----------------------------------------------------------------------
bool EditTab::_processIncludeAdded(EditComponent* node1, EditComponent* node2)
{
	// If element is not set, ignore removing it.
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_SYSTEM)
	{
		// Add the technique to the system
		ParticleUniverse::ParticleSystem* system = static_cast<ParticleUniverse::ParticleSystem*>(element1);
		ParticleUniverse::ParticleTechnique* technique = static_cast<ParticleUniverse::ParticleTechnique*>(element2);
		if (system)
		{
			system->addTechnique(technique);
		}
		return true;
	}
	else if (node1->getComponentType() == CT_TECHNIQUE)
	{
		ParticleUniverse::ParticleTechnique* technique = static_cast<ParticleUniverse::ParticleTechnique*>(element1);
		if (node2->getComponentType() == CT_RENDERER)
		{
			// Set the renderer in the technique
			ParticleUniverse::ParticleRenderer* renderer = static_cast<ParticleUniverse::ParticleRenderer*>(element2);
			if (technique)
			{
				technique->setRenderer(renderer);
			}
			return true;
		}
		else if (node2->getComponentType() == CT_EMITTER)
		{
			// Add the emitter to the technique
			ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(element2);
			if (technique)
			{
				technique->addEmitter(emitter);
				if (emitter->_isMarkedForEmission())
				{
					technique->_unprepareEmitters(); // Destroy pool of emitted emitters
				}
			}

			return true;
		}
		else if (node2->getComponentType() == CT_AFFECTOR)
		{
			// Add the affector to the technique
			ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(element2);
			if (technique)
			{
				technique->addAffector(affector);
				if (affector->_isMarkedForEmission())
				{
					technique->_unprepareAffectors(); // Destroy pool of emitted affectors
				}
			}
			return true;
		}
		else if (node2->getComponentType() == CT_OBSERVER)
		{
			// Add the observer to the technique
			ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(element2);
			if (technique)
			{
				technique->addObserver(observer);
			}
			return true;
		}
		else if (node2->getComponentType() == CT_BEHAVIOUR)
		{
			// Add the behaviour to the technique
			ParticleUniverse::ParticleBehaviour* behaviour = static_cast<ParticleUniverse::ParticleBehaviour*>(element2);
			if (technique)
			{
				technique->_addBehaviourTemplate(behaviour);
			}
			return true;
		}
		else if (node2->getComponentType() == CT_EXTERN)
		{
			// Add the extern to the technique
			ParticleUniverse::Extern* externObject = static_cast<ParticleUniverse::Extern*>(element2);
			if (technique)
			{
				technique->addExtern(externObject);
			}
			return true;
		}
	}
	else if (node1->getComponentType() == CT_OBSERVER)
	{
		ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(element1);
		if (node2->getComponentType() == CT_HANDLER)
		{
			// Add the handler to the observer
			ParticleUniverse::ParticleEventHandler* handler = static_cast<ParticleUniverse::ParticleEventHandler*>(element2);
			if (observer)
			{
				observer->addEventHandler(handler);
			}
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processExcludeAdded(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_AFFECTOR)
	{
		if (node2->getComponentType() == CT_EMITTER)
		{
			// Add the emittername to the excluded names
			ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(element1);
			ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(element2);
			if (emitter->getName() == Ogre::StringUtil::BLANK)
			{
				// The emitter doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_EMITTER);
			}
			affector->addEmitterToExclude(emitter->getName());
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processEmitAdded(EditComponent* node1, EditComponent* node2, ComponentRelationDirection relationDirection)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_EMITTER)
	{
		// Check what type of particle is emitted.
		ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(element1);

		// CRDIR_SECUNDAIRY is the relation of node2 towards node1
		if (node2->getComponentType() == CT_EMITTER && relationDirection == CRDIR_SECUNDAIRY)
		{
			ParticleUniverse::ParticleEmitter* emittedEmitter = static_cast<ParticleUniverse::ParticleEmitter*>(element2);
			emitter->setEmitsType(ParticleUniverse::Particle::PT_EMITTER);
			if (emittedEmitter->getName() == Ogre::StringUtil::BLANK)
			{
				// The emitter doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_EMITTER);
			}
			emitter->setEmitsName(emittedEmitter->getName());
			if (emitter->getParentTechnique())
			{
				// Force creation of emitted emitters
				emitter->getParentTechnique()->_unprepareEmitters();
			}
			return true;
		}
		else if (node2->getComponentType() == CT_TECHNIQUE)
		{
			ParticleUniverse::ParticleTechnique* emittedTechnique = static_cast<ParticleUniverse::ParticleTechnique*>(element2);
			emitter->setEmitsType(ParticleUniverse::Particle::PT_TECHNIQUE);
			if (emittedTechnique->getName() == Ogre::StringUtil::BLANK)
			{
				// The emittedTechnique doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_TECHNIQUE);
			}
			emitter->setEmitsName(emittedTechnique->getName());
			if (emitter->getParentTechnique())
			{
				// Force creation of emitted techniques
				emitter->getParentTechnique()->_unprepareTechnique();
			}
			return true;
		}
		else if (node2->getComponentType() == CT_AFFECTOR)
		{
			ParticleUniverse::ParticleAffector* emittedAffector = static_cast<ParticleUniverse::ParticleAffector*>(element2);
			emitter->setEmitsType(ParticleUniverse::Particle::PT_AFFECTOR);
			if (emittedAffector->getName() == Ogre::StringUtil::BLANK)
			{
				// The emittedAffector doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_AFFECTOR);
			}
			emitter->setEmitsName(emittedAffector->getName());
			if (emitter->getParentTechnique())
			{
				// Force creation of emitted affectors
				emitter->getParentTechnique()->_unprepareAffectors();
			}
			return true;
		}
		else if (node2->getComponentType() == CT_SYSTEM)
		{
			ParticleUniverse::ParticleSystem* emittedSystem = static_cast<ParticleUniverse::ParticleSystem*>(element2);
			emitter->setEmitsType(ParticleUniverse::Particle::PT_SYSTEM);
			emitter->setEmitsName(emittedSystem->getName());
			if (emitter->getParentTechnique())
			{
				// Force creation of emitted systems
				emitter->getParentTechnique()->_unprepareSystem();
			}
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processInterfaceAdded(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_OBSERVER)
	{
		if (node2->getComponentType() == CT_HANDLER)
		{
			// Add the handler to the observer
			ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(element1);
			ParticleUniverse::ParticleEventHandler* handler = static_cast<ParticleUniverse::ParticleEventHandler*>(element2);
			observer->addEventHandler(handler);
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processSlaveAdded(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_EMITTER && node1->getComponentSubType() == CST_EMITTER_SLAVE)
	{
		ParticleUniverse::SlaveEmitter* emitter1 = static_cast<ParticleUniverse::SlaveEmitter*>(element1);
		if (node2->getComponentType() == CT_EMITTER)
		{
			ParticleUniverse::ParticleEmitter* emitter2 = static_cast<ParticleUniverse::ParticleEmitter*>(element2);
			if (emitter1->getParentTechnique())
			{
				emitter1->_unprepare(emitter1->getParentTechnique());
			}
			if (emitter2->getName() == Ogre::StringUtil::BLANK)
			{
				// The emitter2 doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_EMITTER);
			}
			emitter1->setMasterEmitterName(emitter2->getName());
			ParticleUniverse::ParticleTechnique* technique = emitter2->getParentTechnique();
			if (technique)
			{
				if (technique->getName() == Ogre::StringUtil::BLANK)
				{
					// The technique doesn't have a name, so assign a name to it
					Ogre::String name = "Technique" + Ogre::StringConverter::toString(mTechniqueCounter);
					mTechniqueCounter++;
					technique->setName(name);
					
					// Todo: Update the technique component
				}
				emitter1->setMasterTechniqueName(technique->getName());
			}
			if (emitter1->getParentTechnique())
			{
				emitter1->_prepare(emitter1->getParentTechnique());
			}
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processEnableAdded(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_ENABLE_COMPONENT)
	{
		ParticleUniverse::DoEnableComponentEventHandler* handler = static_cast<ParticleUniverse::DoEnableComponentEventHandler*>(element1);
		if (node2->getComponentType() == CT_TECHNIQUE)
		{
			ParticleUniverse::ParticleTechnique* technique = static_cast<ParticleUniverse::ParticleTechnique*>(element2);
			if (technique->getName() == Ogre::StringUtil::BLANK)
			{
				// The technique doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_TECHNIQUE);
			}
			handler->setComponentName(technique->getName());
			handler->setComponentType(ParticleUniverse::CT_TECHNIQUE);
			return true;
		}
		else if (node2->getComponentType() == CT_EMITTER)
		{
			ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(element2);
			if (emitter->getName() == Ogre::StringUtil::BLANK)
			{
				// The emitter doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_EMITTER);
			}
			handler->setComponentName(emitter->getName());
			handler->setComponentType(ParticleUniverse::CT_EMITTER);
			return true;
		}
		else if (node2->getComponentType() == CT_AFFECTOR)
		{
			ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(element2);
			if (affector->getName() == Ogre::StringUtil::BLANK)
			{
				// The affector doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_AFFECTOR);
			}
			handler->setComponentName(affector->getName());
			handler->setComponentType(ParticleUniverse::CT_AFFECTOR);
			return true;
		}
		else if (node2->getComponentType() == CT_OBSERVER)
		{
			ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(element2);
			if (observer->getName() == Ogre::StringUtil::BLANK)
			{
				// The observer doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_OBSERVER);
			}
			handler->setComponentName(observer->getName());
			handler->setComponentType(ParticleUniverse::CT_OBSERVER);
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processForceAdded(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_AFFECTOR)
	{
		if (node2->getComponentType() == CT_AFFECTOR)
		{
			ParticleUniverse::DoAffectorEventHandler* handler = static_cast<ParticleUniverse::DoAffectorEventHandler*>(element1);
			ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(element2);
			if (affector->getName() == Ogre::StringUtil::BLANK)
			{
				// The affector doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_AFFECTOR);
			}
			handler->setAffectorName(affector->getName());
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
bool EditTab::_processPlaceAdded(EditComponent* node1, EditComponent* node2)
{
	ParticleUniverse::IElement* element1 = node1->getPUElement();
	if (!element1)
		return false;

	ParticleUniverse::IElement* element2 = node2->getPUElement();
	if (!element2)
		return false;

	if (node1->getComponentType() == CT_HANDLER && node1->getComponentSubType() == CST_HANDLER_DO_PLACEMENT_PARTICLE)
	{
		if (node2->getComponentType() == CT_EMITTER)
		{
			ParticleUniverse::DoPlacementParticleEventHandler* handler = static_cast<ParticleUniverse::DoPlacementParticleEventHandler*>(element1);
			ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(element2);
			handler->removeAsListener();
			if (emitter->getName() == Ogre::StringUtil::BLANK)
			{
				// The emitter doesn't have a name, so assign a name to it
				_generateNameForComponentAndPUElement(node2, CT_EMITTER);
			}
			handler->setForceEmitterName(emitter->getName());
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------
EditCanvas* EditTab::getEditCanvas(void) const
{
	return mCanvas;
}
//-----------------------------------------------------------------------
EditTab::ConnectionMode EditTab::getConnectionMode(void) const
{
	return mConnectionMode;
}
//-----------------------------------------------------------------------
void EditTab::setConnectionMode(ConnectionMode connectionMode)
{
	mConnectionMode = connectionMode;
	wxCursor connectCursor = wxNullCursor;

	if (connectionMode == CM_CONNECT_STARTING || connectionMode == CM_CONNECT_ENDING)
	{
		// Change the cursor
		connectCursor = wxCursor(wxImage(ICONS_DIR + wxT("connect.png")));
	}
	else if (connectionMode == CM_DISCONNECT)
	{
		// Change the cursor
		connectCursor = wxCursor(wxImage(ICONS_DIR + wxT("disconnect.png")));
	}

	mCanvas->SetCursor(connectCursor);
	std::vector<EditComponent*>::iterator it;
	for (it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		(*it)->SetCursor(connectCursor);
	}
}
//-----------------------------------------------------------------------
std::vector<EditComponent*>& EditTab::getComponents(void)
{
	return mComponents;
}
//-----------------------------------------------------------------------
void EditTab::notifyMouseMovedInComponent(const EditComponent* component, const wxPoint& mousePosition)
{
	wxPoint position;
	int titlebarHeight = component->GetSize().GetHeight() - 2 * component->GetWindowBorderSize().GetHeight() - component->GetClientSize().GetHeight();
	position.x = component->GetPosition().x + component->GetWindowBorderSize().GetWidth();
	position.y = component->GetPosition().y + titlebarHeight;
	mCanvas->adjustMousePosition(position, mousePosition);
}
//-----------------------------------------------------------------------
bool EditTab::isConnectionPossible(EditComponent* component)
{
	if (mStartConnector)
	{
		// Check both sides
		return component->isConnectionPossible(mStartConnector) && mStartConnector->isConnectionPossible(component);
	}
	return false;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::getStartConnector(void)
{
	return mStartConnector;
}
//-----------------------------------------------------------------------
void EditTab::setPropertyWindow(PropertyWindow* propertyWindow)
{
	if (!propertyWindow)
		return;

	ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
	frame->setPropertyWindow(propertyWindow);
	adjustPosition();
}
//-----------------------------------------------------------------------
void EditTab::removePropertyWindow(wxPropertyGrid* propertyWindow)
{
	ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
	frame->removePropertyWindow(propertyWindow);
	adjustPosition();
}
//-----------------------------------------------------------------------
void EditTab::deleteParticleSystemComponents(void)
{
	/** (1) Set all references to the particle system in the small render window to 0, because the particle system is not deleted through the 
		individual Edit Components.
	*/
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*> newList = mComponents;
	for (it = newList.begin(); it != newList.end(); ++it)
	{
		(*it)->setPUElement(0);
	}

	/** (2) Delete all Edit Components, except the particle system edit component.
	*/
	it;
	newList = mComponents;
	for (it = newList.begin(); it != newList.end(); ++it)
	{
		if ((*it)->getComponentType() != CT_SYSTEM)
		{
			// Destroy edit components (affectors, emitters, ...)
			(*it)->Close(true);
		}
	}
}
//-----------------------------------------------------------------------
EditComponent* EditTab::forceCreateParticleSystemEditComponent(void)
{
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*> newList = mComponents;
	EditComponent* particleSystemEditComponent = 0;
	for (it = newList.begin(); it != newList.end(); ++it)
	{
		EditComponent* component = *it;
		if (component->getComponentType() == CT_SYSTEM)
		{
			particleSystemEditComponent = component;
			break;
		}
	}

	if (!particleSystemEditComponent)
	{
		// Create a particle system and particle system edit component
		particleSystemEditComponent = createParticleSystemEditComponent();
	}

	// Return the SYSTEM EditComponent
	return particleSystemEditComponent;
}
//-----------------------------------------------------------------------
bool EditTab::copyParticleSystemPropertiesToPropertyWindow(EditComponent* particleSystemEditComponent, ParticleUniverse::ParticleSystem* particleSystem)
{
	if (!particleSystem || !particleSystemEditComponent)
		return false;

	SystemPropertyWindow* systemPropertyWindow = static_cast<SystemPropertyWindow*>(particleSystemEditComponent->getPropertyWindow());
	particleSystemEditComponent->setComponentName(particleSystem->getTemplateName());
	particleSystemEditComponent->setCaption();
	systemPropertyWindow->copyAttributesFromSystem(particleSystem);
	particleSystemEditComponent->SetFocus(); // Causes the property window to refresh

	return true;
}
//-----------------------------------------------------------------------
bool EditTab::createParticleSystemComponents(EditComponent* particleSystemEditComponent, ParticleUniverse::ParticleSystem* particleSystem)
{
	if (!particleSystem || !particleSystemEditComponent)
		return false;

	particleSystemEditComponent->setPUElement(particleSystem);

	/** Run through the components of the particle system and create edit components and 
		property windows and also propagate the data to the property windows.
	*/
	wxPoint position = wxPoint(TOOLS_EDIT_WIDTH + 16, 16);
	particleSystemEditComponent->SetPosition(position);
	position += particleSystemEditComponent->GetSize();
	position += wxPoint(mOffsetX, mOffsetY);
	size_t numberTechniques = particleSystem->getNumTechniques();
	ParticleUniverse::ParticleTechnique* technique;
	for (size_t i = 0; i < numberTechniques; ++i)
	{
		technique = particleSystem->getTechnique(i);
		position = createComponentsFromTechnique(particleSystemEditComponent, technique, position);
	}
		
	// Create the other connections, which can only be done when all components are available
	for (size_t i = 0; i < numberTechniques; ++i)
	{
		technique = particleSystem->getTechnique(i);
		createOtherConnections(technique);
	}

	notifyConnectionsChanged();
	mCanvas->Refresh();

	// Reset the flag and overwrite if it was changed
	mEditChanged = false;
	return true;
}
//-----------------------------------------------------------------------
void EditTab::destroyDanglingPUComponents(void)
{
	/** Run through all EditComponents and check whether its corresponding ParticleUniverse component (IElement) has a parent.
		If not, destroy it and set the IElement in the EditComponent to 0. In case of a ParticleTechnique and ParticleObserver, 
		they may have included other ParticleUniverse components. Therefor, scan the EditComponents again and check whether a component 
		has the selected ParticleTechnique or ParticleObserver as a parent. If that is the case, set the IElement in the EditComponent 
		to 0, but don't destroy it; it will be destroyed when its parent is destroyed.
	*/
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*>::iterator itSub;
	std::vector<EditComponent*> newList = mComponents;
	EditComponent* particleSystemEditComponent = 0;
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	for (it = newList.begin(); it != newList.end(); ++it)
	{
		EditComponent* component = *it;

		//---------------------------------------- TECHNIQUE ----------------------------------------
		if (component->getComponentType() == CT_TECHNIQUE)
		{
			ParticleUniverse::ParticleTechnique* technique = static_cast<ParticleUniverse::ParticleTechnique*>(component->getPUElement());
			if (technique && !technique->getParentSystem())
			{
				// This is a dangling technique; it may have subcomponents
				for (itSub = newList.begin(); itSub != newList.end(); ++itSub)
				{
					EditComponent* componentSub = *itSub;
					if (componentSub->getComponentType() == CT_RENDERER)
					{
						ParticleUniverse::ParticleRenderer* renderer = static_cast<ParticleUniverse::ParticleRenderer*>(componentSub->getPUElement());
						if (renderer && renderer->getParentTechnique() == technique)
						{
							componentSub->setPUElement(0);
						}
					}
					else if (componentSub->getComponentType() == CT_EMITTER)
					{
						ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(componentSub->getPUElement());
						if (emitter && emitter->getParentTechnique() == technique)
						{
							componentSub->setPUElement(0);
						}
					}
					else if (componentSub->getComponentType() == CT_AFFECTOR)
					{
						ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(componentSub->getPUElement());
						if (affector && affector->getParentTechnique() == technique)
						{
							componentSub->setPUElement(0);
						}
					}
					else if (componentSub->getComponentType() == CT_OBSERVER)
					{
						ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(componentSub->getPUElement());
						if (observer && observer->getParentTechnique() == technique)
						{
							componentSub->setPUElement(0);
						}
					}
					else if (componentSub->getComponentType() == CT_BEHAVIOUR)
					{
						ParticleUniverse::ParticleBehaviour* behaviour = static_cast<ParticleUniverse::ParticleBehaviour*>(componentSub->getPUElement());
						if (behaviour && behaviour->getParentTechnique() == technique)
						{
							componentSub->setPUElement(0);
						}
					}
					else if (componentSub->getComponentType() == CT_EXTERN)
					{
						ParticleUniverse::Extern* externObject = static_cast<ParticleUniverse::Extern*>(componentSub->getPUElement());
						if (externObject && externObject->getParentTechnique() == technique)
						{
							componentSub->setPUElement(0);
						}
					}
				}
				particleSystemManager->destroyTechnique(technique);
				component->setPUElement(0);
			}
		}
		//---------------------------------------- RENDERER ----------------------------------------
		else if (component->getComponentType() == CT_RENDERER)
		{
			ParticleUniverse::ParticleRenderer* renderer = static_cast<ParticleUniverse::ParticleRenderer*>(component->getPUElement());
			if (renderer && !renderer->getParentTechnique())
			{
				particleSystemManager->destroyRenderer(renderer);
				component->setPUElement(0);
			}
		}
		//---------------------------------------- EMITTER ----------------------------------------
		else if (component->getComponentType() == CT_EMITTER)
		{
			ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(component->getPUElement());
			if (emitter && !emitter->getParentTechnique())
			{
				particleSystemManager->destroyEmitter(emitter);
				component->setPUElement(0);
			}
		}
		//---------------------------------------- AFFECTOR ----------------------------------------
		else if (component->getComponentType() == CT_AFFECTOR)
		{
			ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(component->getPUElement());
			if (affector && !affector->getParentTechnique())
			{
				particleSystemManager->destroyAffector(affector);
				component->setPUElement(0);
			}
		}
		//---------------------------------------- OBSERVER ----------------------------------------
		else if (component->getComponentType() == CT_OBSERVER)
		{
			ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(component->getPUElement());
			if (observer && !observer->getParentTechnique())
			{
				// This is a dangling observer; it may have subcomponents
				for (itSub = newList.begin(); itSub != newList.end(); ++itSub)
				{
					EditComponent* componentSub = *itSub;
					if (componentSub->getComponentType() == CT_HANDLER)
					{
						ParticleUniverse::ParticleEventHandler* handler = static_cast<ParticleUniverse::ParticleEventHandler*>(componentSub->getPUElement());
						if (handler && handler->getParentObserver() == observer)
						{
							componentSub->setPUElement(0);
						}
					}
				}
				particleSystemManager->destroyObserver(observer);
				component->setPUElement(0);
			}
		}
		//---------------------------------------- HANDLER ----------------------------------------
		else if (component->getComponentType() == CT_HANDLER)
		{
			ParticleUniverse::ParticleEventHandler* handler = static_cast<ParticleUniverse::ParticleEventHandler*>(component->getPUElement());
			if (handler && !handler->getParentObserver())
			{
				particleSystemManager->destroyEventHandler(handler);
				component->setPUElement(0);
			}
		}
		//---------------------------------------- BEHAVIOUR ----------------------------------------
		else if (component->getComponentType() == CT_BEHAVIOUR)
		{
			ParticleUniverse::ParticleBehaviour* behaviour = static_cast<ParticleUniverse::ParticleBehaviour*>(component->getPUElement());
			if (behaviour && !behaviour->getParentTechnique())
			{
				particleSystemManager->destroyBehaviour(behaviour);
				component->setPUElement(0);
			}
		}
		//---------------------------------------- EXTERN ----------------------------------------
		else if (component->getComponentType() == CT_EXTERN)
		{
			ParticleUniverse::Extern* externObject = static_cast<ParticleUniverse::Extern*>(component->getPUElement());
			if (externObject && !externObject->getParentTechnique())
			{
				particleSystemManager->destroyExtern(externObject);
				component->setPUElement(0);
			}
		}
	}
}
//-----------------------------------------------------------------------
wxPoint EditTab::createComponentsFromTechnique(EditComponent* systemEditComponent, 
	ParticleUniverse::ParticleTechnique* technique,
	wxPoint position)
{
	// First create the component of the technique itself
	EditComponent* techniqueEditComponent = createTechniqueEditComponent();
	techniqueEditComponent->setComponentName(technique->getName());
	techniqueEditComponent->setCaption();
	techniqueEditComponent->SetPosition(position);
	techniqueEditComponent->setPUElement(technique);
	int componentWidth = techniqueEditComponent->GetSize().x;
	int componentHeight = techniqueEditComponent->GetSize().y;
	static_cast<TechniquePropertyWindow*>(techniqueEditComponent->getPropertyWindow())->copyAttributesFromTechnique(technique);
	createConnection(systemEditComponent, techniqueEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
	int highestY = position.y + componentHeight + mOffsetY;

	// Iterate trough the technique and create emitters, affectors, etc...
	
	// 1. Renderer
	position.x = techniqueEditComponent->GetPosition().x - mOffsetX - componentWidth;
	position.y += 0.5 * componentHeight + mOffsetY;
	createComponentFromRenderer(techniqueEditComponent, technique->getRenderer(), position);
	highestY = std::max(highestY, position.y);

	// 2. Emitters
	position.x = techniqueEditComponent->GetPosition().x + componentWidth + mOffsetX;
	size_t numEmitters = technique->getNumEmitters();
	for (size_t i = 0; i < numEmitters; ++i)
	{
		ParticleUniverse::ParticleEmitter* emitter = technique->getEmitter(i);
		createComponentFromEmitter(techniqueEditComponent, emitter, position);
		position.y += componentHeight + mOffsetY;
	}
	highestY = std::max(highestY, position.y);

	// 3. Affectors
	if (numEmitters > 0)
	{
		// Align it next to the emitters
		position.x = techniqueEditComponent->GetPosition().x + 2 * (componentWidth + mOffsetX);
		position.y = techniqueEditComponent->GetPosition().y + + 1.5 * componentHeight + mOffsetY;
	}

	size_t numAffectors = technique->getNumAffectors();
	for (size_t i = 0; i < numAffectors; ++i)
	{
		ParticleUniverse::ParticleAffector* affector = technique->getAffector(i);
		createComponentFromAffector(techniqueEditComponent, affector, position);
		position.y += componentHeight + mOffsetY;
	}
	highestY = std::max(highestY, position.y);

	// 4 + 5. Observers and Event Handlers
	size_t numObservers = technique->getNumObservers();
	position.x = techniqueEditComponent->GetPosition().x;
	position.y = techniqueEditComponent->GetPosition().y + componentHeight + mOffsetY;
	int latestHandlerY = position.y + 0.5 * componentHeight;
	for (size_t i = 0; i < numObservers; ++i)
	{
		ParticleUniverse::ParticleObserver* observer = technique->getObserver(i);
		latestHandlerY = createComponentFromObserver(techniqueEditComponent, observer, position, latestHandlerY);
		position.y += componentHeight + mOffsetY;
	}
	highestY = std::max(highestY, latestHandlerY);

	// 6. Behaviours
	position.x = techniqueEditComponent->GetPosition().x;
	size_t numBehaviours = technique->_getNumBehaviourTemplates();
	for (size_t i = 0; i < numBehaviours; ++i)
	{
		ParticleUniverse::ParticleBehaviour* behaviour = technique->_getBehaviourTemplate(i);
		createComponentFromBehaviour(techniqueEditComponent, behaviour, position);
		position.y += componentHeight + mOffsetY;
	}
	highestY = std::max(highestY, position.y);

	// 7. Externs
	size_t numExterns = technique->getNumExterns();
	for (size_t i = 0; i < numExterns; ++i)
	{
		ParticleUniverse::Extern* externObject = technique->getExtern(i);
		createComponentFromExtern(techniqueEditComponent, externObject, position);
		position.y += componentHeight + mOffsetY;
	}
	highestY = std::max(highestY, position.y);

	// Set position for next technique edit component
	position.x = techniqueEditComponent->GetPosition().x;
	position.y = highestY;
	return position;
}
//-----------------------------------------------------------------------
void EditTab::createComponentFromRenderer(EditComponent* techniqueEditComponent, 
	ParticleUniverse::ParticleRenderer* renderer,
	wxPoint position)
{
	// First create the component of the renderer itself
	EditComponent* rendererEditComponent = createRendererEditComponent(ogre2wxTranslate(renderer->getRendererType()));
	rendererEditComponent->setComponentName(Ogre::StringUtil::BLANK);
	rendererEditComponent->setCaption();
	rendererEditComponent->SetPosition(position);
	rendererEditComponent->setPUElement(renderer);
	static_cast<RendererPropertyWindow*>(rendererEditComponent->getPropertyWindow())->copyAttributesFromRenderer(renderer);
	createConnection(techniqueEditComponent, rendererEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
}
//-----------------------------------------------------------------------
void EditTab::createComponentFromEmitter(EditComponent* techniqueEditComponent, 
	ParticleUniverse::ParticleEmitter* emitter,
	wxPoint position)
{
	// First create the component of the emitter itself
	EditComponent* emitterEditComponent = createEmitterEditComponent(ogre2wxTranslate(emitter->getEmitterType()));
	emitterEditComponent->setComponentName(emitter->getName());
	emitterEditComponent->setCaption();
	emitterEditComponent->SetPosition(position);
	emitterEditComponent->setPUElement(emitter);
	static_cast<EmitterPropertyWindow*>(emitterEditComponent->getPropertyWindow())->copyAttributesFromEmitter(emitter);
	createConnection(techniqueEditComponent, emitterEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
}
//-----------------------------------------------------------------------
void EditTab::createComponentFromAffector(EditComponent* techniqueEditComponent, 
	ParticleUniverse::ParticleAffector* affector,
	wxPoint position)
{
	// First create the component of the affector itself
	EditComponent* affectorEditComponent = createAffectorEditComponent(ogre2wxTranslate(affector->getAffectorType()));
	affectorEditComponent->setComponentName(affector->getName());
	affectorEditComponent->setCaption();
	affectorEditComponent->SetPosition(position);
	affectorEditComponent->setPUElement(affector);
	static_cast<AffectorPropertyWindow*>(affectorEditComponent->getPropertyWindow())->copyAttributesFromAffector(affector);
	createConnection(techniqueEditComponent, affectorEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
}
//-----------------------------------------------------------------------
int EditTab::createComponentFromObserver(EditComponent* techniqueEditComponent, 
	ParticleUniverse::ParticleObserver* observer,
	wxPoint position,
	int latestHandlerY)
{
	// First create the component of the observer itself
	EditComponent* observerEditComponent = createObserverEditComponent(ogre2wxTranslate(observer->getObserverType()));
	observerEditComponent->setComponentName(observer->getName());
	observerEditComponent->setCaption();
	observerEditComponent->SetPosition(position);
	observerEditComponent->setPUElement(observer);
	static_cast<ObserverPropertyWindow*>(observerEditComponent->getPropertyWindow())->copyAttributesFromObserver(observer);
	createConnection(techniqueEditComponent, observerEditComponent, CR_INCLUDE, CRDIR_PRIMARY);

	// Create all handlers
	int componentWidth = techniqueEditComponent->GetSize().x;
	int componentHeight = techniqueEditComponent->GetSize().y;
	position.x = techniqueEditComponent->GetPosition().x - componentWidth - mOffsetX;
	position.y = latestHandlerY + mOffsetY;
	size_t numEventHandlers = observer->getNumEventHandlers();
	for (size_t i = 0; i < numEventHandlers; ++i)
	{
		ParticleUniverse::ParticleEventHandler* handler = observer->getEventHandler(i);
		createComponentFromEventHandler(observerEditComponent, handler, position);
		position.y += componentHeight + mOffsetY;
	}
	
	return position.y;
}
//-----------------------------------------------------------------------
void EditTab::createComponentFromEventHandler(EditComponent* observerEditComponent, 
	ParticleUniverse::ParticleEventHandler* eventHandler,
	wxPoint position)
{
	EditComponent* handlerEditComponent = createHandlerEditComponent(ogre2wxTranslate(eventHandler->getEventHandlerType()));
	handlerEditComponent->setComponentName(eventHandler->getName());
	handlerEditComponent->setCaption();
	handlerEditComponent->SetPosition(position);
	handlerEditComponent->setPUElement(eventHandler);
	static_cast<EventHandlerPropertyWindow*>(handlerEditComponent->getPropertyWindow())->copyAttributesFromEventHandler(eventHandler);
	createConnection(observerEditComponent, handlerEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
}
//-----------------------------------------------------------------------
void EditTab::createComponentFromBehaviour(EditComponent* techniqueEditComponent, 
	ParticleUniverse::ParticleBehaviour* behaviour,
	wxPoint position)
{
	// First create the component of the behaviour itself
	EditComponent* behaviourEditComponent = createBehaviourEditComponent(ogre2wxTranslate(behaviour->getBehaviourType()));
	behaviourEditComponent->setComponentName(Ogre::StringUtil::BLANK);
	behaviourEditComponent->setCaption();
	behaviourEditComponent->SetPosition(position);
	behaviourEditComponent->setPUElement(behaviour);
	static_cast<BehaviourPropertyWindow*>(behaviourEditComponent->getPropertyWindow())->copyAttributesFromBehaviour(behaviour);
	createConnection(techniqueEditComponent, behaviourEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
}
//-----------------------------------------------------------------------
void EditTab::createComponentFromExtern(EditComponent* techniqueEditComponent, 
	ParticleUniverse::Extern* externObject,
	wxPoint position)
{
	// First create the component of the extern itself
	EditComponent* externEditComponent = createExternEditComponent(ogre2wxTranslate(externObject->getExternType()));
	externEditComponent->setComponentName(externObject->getName());
	externEditComponent->setCaption();
	externEditComponent->SetPosition(position);
	externEditComponent->setPUElement(externObject);
	static_cast<ExternPropertyWindow*>(externEditComponent->getPropertyWindow())->copyAttributesFromExtern(externObject);
	createConnection(techniqueEditComponent, externEditComponent, CR_INCLUDE, CRDIR_PRIMARY);
}
//-----------------------------------------------------------------------
void EditTab::createOtherConnections(const ParticleUniverse::ParticleTechnique* technique)
{
	/** Creating the other connections can only be done if all components are already created, so you need to run through
		the particle technique again.
	*/

	// 1. Emitters
	size_t numEmitters = technique->getNumEmitters();
	for (size_t i = 0; i < numEmitters; ++i)
	{
		ParticleUniverse::ParticleEmitter* emitter = technique->getEmitter(i);
		EditComponent* emitterEditComponent = findEditComponent(emitter);
		if (emitterEditComponent)
		{
			// Create connections: CR_SLAVE
			if (ogre2wxTranslate(emitter->getEmitterType()) == CST_EMITTER_SLAVE)
			{
				ParticleUniverse::SlaveEmitter* slaveEmiter = static_cast<ParticleUniverse::SlaveEmitter*>(emitter);
				wxString wxName = ogre2wx(slaveEmiter->getMasterEmitterName());
				wxString techniqueName = ogre2wx(slaveEmiter->getMasterTechniqueName());
				EditComponent* masterEmitterEditComponent = findEditComponentForTechnique(wxName, techniqueName);
				if (masterEmitterEditComponent)
				{
					createConnection(emitterEditComponent, masterEmitterEditComponent, CR_SLAVE, CRDIR_PRIMARY);
				}
			}
		
			// Create connections: CR_EMIT
			if (emitter->getEmitsType() != ParticleUniverse::Particle::PT_VISUAL)
			{
				// This version of the editor doesn't allow emitted particle systems, because emitting particle systems is not recommended.
				ComponentType componentType = CT_VISUAL;
				if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_EMITTER)
				{
					componentType = CT_EMITTER;
				}
				else if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_AFFECTOR)
				{
					componentType = CT_AFFECTOR;
				}
				else if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_TECHNIQUE)
				{
					componentType = CT_TECHNIQUE;
				}
				if (componentType != CT_VISUAL)
				{
					wxString wxName = ogre2wx(emitter->getEmitsName());
					EditComponent* editComponent = findEditComponent(wxName, componentType);
					if (editComponent)
					{
						createConnection(emitterEditComponent, editComponent, CR_EMIT, CRDIR_PRIMARY);
					}
				}
			}
		}
	}

	// 2. Affectors
	// Create connections: CR_EXCLUDE
	size_t numAffector = technique->getNumAffectors();
	for (size_t i = 0; i < numAffector; ++i)
	{
		ParticleUniverse::ParticleAffector* affector = technique->getAffector(i);
		EditComponent* affectorEditComponent = findEditComponent(affector);
		EditComponent* editComponent;
		if (affectorEditComponent)
		{
			ParticleUniverse::list<Ogre::String> emittersToExclude = affector->getEmittersToExclude();
			ParticleUniverse::list<Ogre::String>::iterator it;
			ParticleUniverse::list<Ogre::String>::iterator itEnd = emittersToExclude.end();
			for (it = emittersToExclude.begin(); it != itEnd; ++it)
			{
				// Remove the double quotes
				Ogre::String name = (*it);
				Ogre::String::size_type index = name.find_first_of("\"");
				while(index != Ogre::String::npos)
				{
					name = name.erase(index, 1);
					index = name.find_first_of("\"");
				}
				wxString wxName = ogre2wx(name);
				editComponent = findEditComponent(wxName, CT_EMITTER);
				if (editComponent)
				{
					createConnection(affectorEditComponent, editComponent, CR_EXCLUDE, CRDIR_PRIMARY);
				}
			}
		}
	}

	// 3. Event Handlers
	size_t numObservers = technique->getNumObservers();
	EditComponent* editComponent;
	for (size_t i = 0; i < numObservers; ++i)
	{
		ParticleUniverse::ParticleObserver* observer = technique->getObserver(i);
		size_t numEventHandlers = observer->getNumEventHandlers();
		for (size_t j = 0; j < numEventHandlers; ++j)
		{
			ParticleUniverse::ParticleEventHandler* handler = observer->getEventHandler(j);
			EditComponent* eventHandlerEditComponent = findEditComponent(handler);
			if (ogre2wxTranslate(handler->getEventHandlerType()) == CST_HANDLER_DO_ENABLE_COMPONENT)
			{
				// Create connections: CR_ENABLE
				ParticleUniverse::DoEnableComponentEventHandler* doEnableComponentEventHandler = 
					static_cast<ParticleUniverse::DoEnableComponentEventHandler*>(handler);
				ParticleUniverse::ComponentType componentType = doEnableComponentEventHandler->getComponentType();
				ComponentType ct = CT_EMITTER;
				if (componentType == ParticleUniverse::CT_AFFECTOR)
				{
					ct = CT_AFFECTOR;
				}
				else if (componentType == ParticleUniverse::CT_TECHNIQUE)
				{
					ct = CT_TECHNIQUE;
				}
				else if (componentType == ParticleUniverse::CT_OBSERVER)
				{
					ct = CT_OBSERVER;
				}
				wxString wxName = ogre2wx(doEnableComponentEventHandler->getComponentName());
				editComponent = findEditComponent(wxName, ct);
				if (editComponent)
				{
					createConnection(eventHandlerEditComponent, editComponent, CR_ENABLE, CRDIR_PRIMARY);
				}
			}
			else if (ogre2wxTranslate(handler->getEventHandlerType()) == CST_HANDLER_DO_AFFECTOR)
			{
				// Create connections: CR_FORCE
				ParticleUniverse::DoAffectorEventHandler* doAffectorEventHandler = 
					static_cast<ParticleUniverse::DoAffectorEventHandler*>(handler);
				wxString wxName = ogre2wx(doAffectorEventHandler->getAffectorName());
				editComponent = findEditComponent(wxName, CT_AFFECTOR);
				if (editComponent)
				{
					createConnection(eventHandlerEditComponent, editComponent, CR_FORCE, CRDIR_PRIMARY);
				}
			}
			else if (ogre2wxTranslate(handler->getEventHandlerType()) == CST_HANDLER_DO_PLACEMENT_PARTICLE)
			{
				// Create connections: CR_PLACE
				ParticleUniverse::DoPlacementParticleEventHandler* doPlacementParticleEventHandler = 
					static_cast<ParticleUniverse::DoPlacementParticleEventHandler*>(handler);
				wxString wxName = ogre2wx(doPlacementParticleEventHandler->getForceEmitterName());
				editComponent = findEditComponent(wxName, CT_EMITTER);
				if (editComponent)
				{
					createConnection(eventHandlerEditComponent, editComponent, CR_PLACE, CRDIR_PRIMARY);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------
void EditTab::createConnection(EditComponent* componentPrimary, 
	EditComponent* componentSecundairy, 
	ComponentRelation relation, 
	ComponentRelationDirection direction)
{
	// Establish the connection between the two
	ConnectionPolicy* policy = componentPrimary->getPolicy(relation, direction, componentSecundairy->getComponentType());
	mCanvas->connect(componentPrimary, componentSecundairy, relation, policy->getColourCode(), policy->getLineStyle());
	componentPrimary->addConnection(componentSecundairy, relation, direction);
	componentSecundairy->addConnection(componentPrimary, relation, getOppositeRelationDirection(direction));
}
//-----------------------------------------------------------------------
EditComponent* EditTab::getParticleSystemEditComponent(void)
{
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*>::iterator itEnd = mComponents.end();
	EditComponent* component = 0;
	for (it = mComponents.begin(); it != itEnd; ++it)
	{
		component = *it;
		if (component->getComponentType() == CT_SYSTEM)
		{
			// There is only one!
			return component;
		}
	}
	return 0;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::findEditComponent(const wxString& name, const ComponentType& type, EditComponent* skip)
{
	/*  This implementation returns the first component found, but it ignores the fact that the same type with the same name
		can occur multiple times (i.e. an emitter with the same name in another technique).
		Todo: Add another parameter to make the search unique.
	*/
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*>::iterator itEnd = mComponents.end();
	EditComponent* component;
	for (it = mComponents.begin(); it != itEnd; ++it)
	{
		component = *it;
		if (skip != component &&
			!component->GetName().empty() && 
			component->getComponentName() == wx2ogre(name) && 
			component->getComponentType() == type)
		{
			return component;
		}
	}

	return 0;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::findEditComponent(const ParticleUniverse::IElement* puElement)
{
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*>::iterator itEnd = mComponents.end();
	EditComponent* component;
	for (it = mComponents.begin(); it != itEnd; ++it)
	{
		component = *it;
		if (component->getPUElement() == puElement)
		{
			return component;
		}
	}
	return 0;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::findEditComponentForTechnique(const wxString& name, const wxString& techniqueName)
{
	// Search for a component that is included by a certain technique component
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*>::iterator itEnd = mComponents.end();
	EditComponent* editComponent;
	EditComponent* techniqueEditComponent = findEditComponent(techniqueName, CT_TECHNIQUE);
	for (it = mComponents.begin(); it != itEnd; ++it)
	{
		editComponent = *it;
		if (editComponent->getComponentName() == wx2ogre(name))
		{
			if (techniqueEditComponent->isConnected(editComponent, CR_INCLUDE, CRDIR_PRIMARY))
			{
				return editComponent;
			}
		}
	}
	return 0;
}
//-----------------------------------------------------------------------
void EditTab::createTechniqueForComponent(EditComponent* component)
{
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::ParticleTechnique* newTechnique = particleSystemManager->createTechnique();
	newTechnique->setName(component->getPropertyWindow()->getComponentName());
	component->setPUElement(newTechnique);
}
//-----------------------------------------------------------------------
void EditTab::createRendererForComponent(const Ogre::String& type, EditComponent* component)
{
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::ParticleRenderer* newRenderer = particleSystemManager->createRenderer(type);
	component->setPUElement(newRenderer);
}
//-----------------------------------------------------------------------
void EditTab::createEmitterForComponent(const Ogre::String& type, EditComponent* component)
{
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::ParticleEmitter* newEmitter = particleSystemManager->createEmitter(type);
	newEmitter->setName(component->getPropertyWindow()->getComponentName());
	component->setPUElement(newEmitter);
}
//-----------------------------------------------------------------------
void EditTab::createAffectorForComponent(const Ogre::String& type, EditComponent* component)
{
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::ParticleAffector* newAffector = particleSystemManager->createAffector(type);
	newAffector->setName(component->getPropertyWindow()->getComponentName());
	component->setPUElement(newAffector);
}
//-----------------------------------------------------------------------
void EditTab::createObserverForComponent(const Ogre::String& type, EditComponent* component)
{
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::ParticleObserver* newObserver = particleSystemManager->createObserver(type);
	newObserver->setName(component->getPropertyWindow()->getComponentName());
	component->setPUElement(newObserver);
}
//-----------------------------------------------------------------------
void EditTab::createHandlerForComponent(const Ogre::String& type, EditComponent* component)
{
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::ParticleEventHandler* newHandler = particleSystemManager->createEventHandler(type);
	newHandler->setName(component->getPropertyWindow()->getComponentName());
	component->setPUElement(newHandler);
}
//-----------------------------------------------------------------------
void EditTab::createBehaviourForComponent(const Ogre::String& type, EditComponent* component)
{
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::ParticleBehaviour* newBehaviour = particleSystemManager->createBehaviour(type);
	component->setPUElement(newBehaviour);
}
//-----------------------------------------------------------------------
void EditTab::createExternForComponent(const Ogre::String& type, EditComponent* component)
{
	ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	ParticleUniverse::Extern* newExtern = particleSystemManager->createExtern(type);
	newExtern->setName(component->getPropertyWindow()->getComponentName());
	component->setPUElement(newExtern);
}
//-----------------------------------------------------------------------
void EditTab::destroyTechniqueFromComponent(EditComponent* component)
{
	if (!component->getPUElement())
		return;

	ParticleUniverse::ParticleTechnique* technique = static_cast<ParticleUniverse::ParticleTechnique*>(component->getPUElement());
	ParticleUniverse::ParticleSystem* system = technique->getParentSystem();
	if (system)
	{
		bool wasStarted = _mustStopParticleSystem();
		_removeComponentsFromTechnique(technique);
		system->destroyTechnique(technique);
		_mustRestartParticleSystem(wasStarted);
	}
	else
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		_removeComponentsFromTechnique(technique);
		particleSystemManager->destroyTechnique(technique);
	}

	component->setPUElement(0);
}
//-----------------------------------------------------------------------
void EditTab::_removeComponentsFromTechnique(ParticleUniverse::ParticleTechnique* technique)
{
	// Cleanup emitted systems, techniques, emitters, affectors, etc.
	technique->_unprepareSystem();
	technique->_unprepareTechnique();
	technique->_unprepareRenderer();
	technique->_unprepareEmitters();
	technique->_unprepareBehaviours();
	technique->_unprepareExterns();
	technique->_unprepareAffectors();
	technique->_unprepareVisualParticles();

	// Remove renderer
	ParticleUniverse::ParticleRenderer* renderer = technique->getRenderer();
	if (renderer)
	{
		technique->removeRenderer(renderer);
	}

	// Remove emitters.
	size_t size = technique->getNumEmitters();
	for (size_t i = 0; i < size; ++i)
	{
		technique->removeEmitter(technique->getEmitter(i));
	}

	// Remove affectors.
	size = technique->getNumAffectors();
	for (size_t i = 0; i < size; ++i)
	{
		technique->removeAffector(technique->getAffector(i));
	}

	// Remove behaviour.
	size = technique->_getNumBehaviourTemplates();
	for (size_t i = 0; i < size; ++i)
	{
		technique->_removeBehaviourTemplate(technique->_getBehaviourTemplate(i));
	}

	// Remove observers.
	size = technique->getNumObservers();
	for (size_t i = 0; i < size; ++i)
	{
		technique->removeObserver(technique->getObserver(i));
	}

	// Remove externs.
	size = technique->getNumExterns();
	for (size_t i = 0; i < size; ++i)
	{
		technique->removeExtern(technique->getExtern(i));
	}
}
//-----------------------------------------------------------------------
void EditTab::destroyRendererFromComponent(EditComponent* component)
{
	if (!component->getPUElement())
		return;

	ParticleUniverse::ParticleRenderer* renderer = static_cast<ParticleUniverse::ParticleRenderer*>(component->getPUElement());
	ParticleUniverse::ParticleTechnique* technique = renderer->getParentTechnique();
	if (technique)
	{
		bool wasStarted = _mustStopParticleSystem();
		technique->destroyRenderer();
		_mustRestartParticleSystem(wasStarted);
	}
	else
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		particleSystemManager->destroyRenderer(renderer);
	}

	component->setPUElement(0);
}
//-----------------------------------------------------------------------
void EditTab::destroyEmitterFromComponent(EditComponent* component)
{
	if (!component->getPUElement())
		return;

	ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(component->getPUElement());
	ParticleUniverse::ParticleTechnique* technique = emitter->getParentTechnique();
	if (technique)
	{
		bool wasStarted = _mustStopParticleSystem();
		technique->destroyEmitter(emitter);
		_mustRestartParticleSystem(wasStarted);
	}
	else
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		particleSystemManager->destroyEmitter(emitter);
	}

	component->setPUElement(0);
}
//-----------------------------------------------------------------------
void EditTab::destroyAffectorFromComponent(EditComponent* component)
{
	if (!component->getPUElement())
		return;

	ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(component->getPUElement());
	ParticleUniverse::ParticleTechnique* technique = affector->getParentTechnique();
	if (technique)
	{
		bool wasStarted = _mustStopParticleSystem();
		technique->destroyAffector(affector);
		_mustRestartParticleSystem(wasStarted);
	}
	else
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		particleSystemManager->destroyAffector(affector);
	}

	component->setPUElement(0);
}
//-----------------------------------------------------------------------
void EditTab::destroyObserverFromComponent(EditComponent* component)
{
	if (!component->getPUElement())
		return;

	// Remove event handlers
	ParticleUniverse::ParticleObserver* observer = static_cast<ParticleUniverse::ParticleObserver*>(component->getPUElement());
	size_t size = observer->getNumEventHandlers();
	for (size_t i = 0; i < size; ++i)
	{
		observer->removeEventHandler(observer->getEventHandler(i));
	}

	// Destroy the observer
	ParticleUniverse::ParticleTechnique* technique = observer->getParentTechnique();
	if (technique)
	{
		bool wasStarted = _mustStopParticleSystem();
		technique->destroyObserver(observer);
		_mustRestartParticleSystem(wasStarted);
	}
	else
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		particleSystemManager->destroyObserver(observer);
	}

	component->setPUElement(0);
}
//-----------------------------------------------------------------------
void EditTab::destroyHandlerFromComponent(EditComponent* component)
{
	if (!component->getPUElement())
		return;

	ParticleUniverse::ParticleEventHandler* handler = static_cast<ParticleUniverse::ParticleEventHandler*>(component->getPUElement());
	ParticleUniverse::ParticleObserver* observer = handler->getParentObserver();
	if (observer)
	{
		bool wasStarted = _mustStopParticleSystem();
		observer->destroyEventHandler(handler);
		_mustRestartParticleSystem(wasStarted);
	}
	else
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		particleSystemManager->destroyEventHandler(handler);
	}

	component->setPUElement(0);
}
//-----------------------------------------------------------------------
void EditTab::destroyBehaviourFromComponent(EditComponent* component)
{
	if (!component->getPUElement())
		return;

	ParticleUniverse::ParticleBehaviour* behaviour = static_cast<ParticleUniverse::ParticleBehaviour*>(component->getPUElement());
	ParticleUniverse::ParticleTechnique* technique = behaviour->getParentTechnique();
	if (technique)
	{
		bool wasStarted = _mustStopParticleSystem();
		technique->_destroyBehaviourTemplate(behaviour);
		_mustRestartParticleSystem(wasStarted);
	}
	else
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		particleSystemManager->destroyBehaviour(behaviour);
	}

	component->setPUElement(0);
}
//-----------------------------------------------------------------------
void EditTab::destroyExternFromComponent(EditComponent* component)
{
	if (!component->getPUElement())
		return;

	ParticleUniverse::Extern* externObject = static_cast<ParticleUniverse::Extern*>(component->getPUElement());
	ParticleUniverse::ParticleTechnique* technique = externObject->getParentTechnique();
	if (technique)
	{
		bool wasStarted = _mustStopParticleSystem();
		technique->destroyExtern(externObject);
		_mustRestartParticleSystem(wasStarted);
	}
	else
	{
		ParticleUniverse::ParticleSystemManager* particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		particleSystemManager->destroyExtern(externObject);
	}

	component->setPUElement(0);
}
//-----------------------------------------------------------------------
EditComponent* EditTab::createParticleSystemEditComponent(void)
{
	if (mSystemCounter > 0)
		return 0;

	wxColour col;
	col.Set(wxT("#000000"));
	EditComponent* systemComponent = new EditComponent(this, "mySystem", CT_SYSTEM, CST_UNDEFINED, col, wxSize(200, 80), wxBORDER_RAISED | wxCAPTION);
	systemComponent->setRootFrame(static_cast<ParticleUniverseEditorFrame*>(mRootParent));
	//systemComponent->createPropertyWindow(CT_SYSTEM); // Recreate it, so it contains the root frame TESTTESTTESTTESTTESTTEST
	systemComponent->createPropertyWindow(CST_UNDEFINED); // Recreate it, so it contains the root frame
	systemComponent->SetSize(systemComponent->getOriginalSize().Scale(mScale, mScale));

	// Altough it is possible that a particle system itself is emitted, the connection to the emitter is not defined (there can only be one system on the canvas)
	systemComponent->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_TECHNIQUE, CST_UNDEFINED);
	systemComponent->SetSize(systemComponent->getOriginalSize().Scale(mScale, mScale));
	pushSystem(systemComponent);
	mSystemCounter++;
	return systemComponent;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::createTechniqueEditComponent(void)
{
	Ogre::String name = "Technique" + Ogre::StringConverter::toString(mTechniqueCounter);
	wxColour col;
	col.Set(wxT("#6698FF"));
	EditComponent* technique = new EditComponent(this, name, CT_TECHNIQUE, CST_UNDEFINED, col);
	technique->SetSize(technique->getOriginalSize().Scale(mScale, mScale));
	technique->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_SYSTEM, CST_UNDEFINED, false);
	technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_RENDERER, CST_UNDEFINED, false);
	technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_EMITTER, CST_UNDEFINED);
	technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_AFFECTOR, CST_UNDEFINED);
	technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_OBSERVER, CST_UNDEFINED);
	technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_BEHAVIOUR, CST_UNDEFINED);
	technique->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_EXTERN, CST_UNDEFINED);
	technique->addPolicy(CR_EMIT, CRDIR_SECUNDAIRY, CRD_EMITTED_BY, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_EMITTED_COLOURCODE);
	technique->addPolicy(CR_ENABLE, CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
	pushComponent(technique);
	mTechniqueCounter++;
	return technique;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::createRendererEditComponent(const wxString& type)
{
	//Ogre::String name = "Renderer" + Ogre::StringConverter::toString(mRendererCounter);
	Ogre::String name = Ogre::StringUtil::BLANK;
	wxColour col;
	col.Set(wxT("#E42217"));
	EditComponent* rendererComponent = new EditComponent(this, name, CT_RENDERER, type, col);
	rendererComponent->SetSize(rendererComponent->getOriginalSize().Scale(mScale, mScale));
	rendererComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
	pushComponent(rendererComponent);
	mRendererCounter++;
	return rendererComponent;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::createEmitterEditComponent(const wxString& type)
{
	Ogre::String name = "Emitter" + Ogre::StringConverter::toString(mEmitterCounter);
	wxColour col;
	col.Set(wxT("#4CC417"));
	EditComponent* emitterComponent = new EditComponent(this, name, CT_EMITTER, type, col);
	emitterComponent->SetSize(emitterComponent->getOriginalSize().Scale(mScale, mScale));

	// Altough it is possible to emit a particle system, the connection to the system is not defined (there can only be one system on the canvas)
	emitterComponent->addUniqueRelation(CR_EMIT, CRDIR_PRIMARY); // Only emission of one type is allowed
	emitterComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
	emitterComponent->addPolicy(CR_EMIT, CRDIR_PRIMARY, CRD_EMITS, CT_TECHNIQUE, CST_UNDEFINED, false, true, DRAW_EMITTED_COLOURCODE);
	emitterComponent->addPolicy(CR_EMIT, CRDIR_PRIMARY, CRD_EMITS, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_EMITTED_COLOURCODE);
	emitterComponent->addPolicy(CR_EMIT, CRDIR_PRIMARY, CRD_EMITS, CT_AFFECTOR, CST_UNDEFINED, false, true, DRAW_EMITTED_COLOURCODE);
	emitterComponent->addPolicy(CR_EMIT, CRDIR_SECUNDAIRY, CRD_EMITTED_BY, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_EMITTED_COLOURCODE);
	emitterComponent->addPolicy(CR_SLAVE, CRDIR_SECUNDAIRY, CRD_SLAVES, CT_EMITTER, CST_EMITTER_SLAVE, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
	emitterComponent->addPolicy(CR_SLAVE, CRDIR_PRIMARY, CRD_SLAVE_OF, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
	emitterComponent->addPolicy(CR_EXCLUDE, CRDIR_SECUNDAIRY, CRD_EXCLUDED_BY, CT_AFFECTOR, CST_UNDEFINED, true, true, DRAW_DEFAULT_COLOURCODE, wxSHORT_DASH);
	emitterComponent->addPolicy(CR_ENABLE, CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
	emitterComponent->addPolicy(CR_PLACE, CRDIR_SECUNDAIRY, CRD_PLACED_BY, CT_HANDLER, CST_HANDLER_DO_PLACEMENT_PARTICLE, true, false, DRAW_DEFAULT_COLOURCODE, wxDOT);
	pushComponent(emitterComponent);
	mEmitterCounter++;
	return emitterComponent;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::createAffectorEditComponent(const wxString& type)
{
	Ogre::String name = "Affector" + Ogre::StringConverter::toString(mAffectorCounter);
	wxColour col;
	col.Set(wxT("#FBB117"));
	EditComponent* affectorComponent = new EditComponent(this, name, CT_AFFECTOR, type, col);
	affectorComponent->SetSize(affectorComponent->getOriginalSize().Scale(mScale, mScale));
	affectorComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
	affectorComponent->addPolicy(CR_EMIT, CRDIR_SECUNDAIRY, CRD_EMITTED_BY, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_EMITTED_COLOURCODE);
	affectorComponent->addPolicy(CR_EXCLUDE, CRDIR_PRIMARY, CRD_EXCLUDES, CT_EMITTER, CST_UNDEFINED, true, true, DRAW_DEFAULT_COLOURCODE, wxSHORT_DASH);
	affectorComponent->addPolicy(CR_ENABLE, CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
	affectorComponent->addPolicy(CR_FORCE, CRDIR_SECUNDAIRY, CRD_FORCED_BY, CT_HANDLER, CST_HANDLER_DO_AFFECTOR, true, false, DRAW_DEFAULT_COLOURCODE, wxDOT);
	pushComponent(affectorComponent);
	mAffectorCounter++;
	return affectorComponent;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::createObserverEditComponent(const wxString& type)
{
	Ogre::String name = "Observer" + Ogre::StringConverter::toString(mObserverCounter);
	wxColour col;
	col.Set(wxT("#254117"));
	EditComponent* observerComponent = new EditComponent(this, name, CT_OBSERVER, type, col);
	observerComponent->SetSize(observerComponent->getOriginalSize().Scale(mScale, mScale));
	observerComponent->addPolicy(CR_INCLUDE, CRDIR_PRIMARY, CRD_INCLUDES, CT_HANDLER, CST_UNDEFINED, true);
	observerComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
	observerComponent->addPolicy(CR_ENABLE, CRDIR_SECUNDAIRY, CRD_ENABLED_BY, CT_HANDLER, CST_HANDLER_DO_ENABLE_COMPONENT, true, false, DRAW_SPECIAL_CASE_COLOURCODE);
	pushComponent(observerComponent);
	mObserverCounter++;
	return observerComponent;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::createHandlerEditComponent(const wxString& type)
{
	Ogre::String name = "Handler" + Ogre::StringConverter::toString(mHandlerCounter);
	wxColour col;
	col.Set(wxT("#8D38C9"));
	EditComponent* handler = new EditComponent(this, name, CT_HANDLER, type, col);
	handler->SetSize(handler->getOriginalSize().Scale(mScale, mScale));
	handler->addUniqueRelation(CR_ENABLE, CRDIR_PRIMARY); // Only enabling of one type is allowed
	handler->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_OBSERVER, CST_UNDEFINED, false);
	handler->addPolicy(CR_ENABLE, CRDIR_PRIMARY, CRD_ENABLES, CT_TECHNIQUE, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
	handler->addPolicy(CR_ENABLE, CRDIR_PRIMARY, CRD_ENABLES, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
	handler->addPolicy(CR_ENABLE, CRDIR_PRIMARY, CRD_ENABLES, CT_AFFECTOR, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
	handler->addPolicy(CR_ENABLE, CRDIR_PRIMARY, CRD_ENABLES, CT_OBSERVER, CST_UNDEFINED, false, true, DRAW_SPECIAL_CASE_COLOURCODE);
	handler->addPolicy(CR_PLACE, CRDIR_PRIMARY, CRD_PLACES, CT_EMITTER, CST_UNDEFINED, false, true, DRAW_DEFAULT_COLOURCODE, wxDOT);
	handler->addPolicy(CR_FORCE, CRDIR_PRIMARY, CRD_FORCES, CT_AFFECTOR, CST_UNDEFINED, false, true, DRAW_DEFAULT_COLOURCODE, wxDOT);
	pushComponent(handler);
	mHandlerCounter++;
	return handler;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::createBehaviourEditComponent(const wxString& type)
{
	Ogre::String name = "Behaviour" + Ogre::StringConverter::toString(mBehaviourCounter);
	wxColour col;
	col.Set(wxT("#307D7E"));
	EditComponent* behaviourComponent = new EditComponent(this, name, CT_BEHAVIOUR, type, col);
	behaviourComponent->SetSize(behaviourComponent->getOriginalSize().Scale(mScale, mScale));
	behaviourComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
	pushComponent(behaviourComponent);
	mBehaviourCounter++;
	return behaviourComponent;
}
//-----------------------------------------------------------------------
EditComponent* EditTab::createExternEditComponent(const wxString& type)
{
	Ogre::String name = "Extern" + Ogre::StringConverter::toString(mExternCounter);
	wxColour col;
	col.Set(wxT("#827B60"));
	EditComponent* externObjectComponent = new EditComponent(this, name, CT_EXTERN, type, col);
	externObjectComponent->SetSize(externObjectComponent->getOriginalSize().Scale(mScale, mScale));
	externObjectComponent->addPolicy(CR_INCLUDE, CRDIR_SECUNDAIRY, CRD_INCLUDED_BY, CT_TECHNIQUE, CST_UNDEFINED, false);
	pushComponent(externObjectComponent);
	mExternCounter++;
	return externObjectComponent;
}
//-----------------------------------------------------------------------
void EditTab::scaleEditComponents(ParticleUniverse::Real scale)
{
	std::vector<EditComponent*>::iterator it;
	std::vector<EditComponent*>::iterator itEnd = mComponents.end();
	ParticleUniverse::Real sc = scale/mScale;
	for (it = mComponents.begin(); it != itEnd; ++it)
	{
		EditComponent* editComponent = *it;
		wxPoint pos(sc * editComponent->GetPosition().x, sc * editComponent->GetPosition().y);
		editComponent->SetPosition(pos);
		editComponent->SetSize(editComponent->getOriginalSize().Scale(scale, scale));
	}
	mScale = scale;
}
//-----------------------------------------------------------------------
void EditTab::OnMouseWheel(wxMouseEvent& event)
{
	// Scroll the window up or down
	wxMDIClientWindow* clientWindow = GetClientWindow();
	if (!clientWindow)
		return;

	clientWindow->Freeze();
	if (event.GetWheelRotation() < 0)
	{
		clientWindow->ScrollWindow(0, -24);
	}
	else if (event.GetWheelRotation() > 0)
	{
		clientWindow->ScrollWindow(0, 24);
	}
	clientWindow->Refresh();
	clientWindow->Thaw();
	clientWindow->SetScrollPos(wxVERTICAL, 0, true);
}
//-----------------------------------------------------------------------
void EditTab::OnKeyPressed(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_PAGEUP)
	{
		if (mScale < 1.5)
		{
			scaleEditComponents(mScale + 0.1);
		}
	}
	else if (event.GetKeyCode() == WXK_PAGEDOWN)
	{
		if (mScale > 0.5)
		{
			scaleEditComponents(mScale - 0.1);
		}
	}

	// Unfortunately, keystrokes for menu events must be dispatched as below.
	else if ((event.GetKeyCode() == 78 || event.GetKeyCode() == 110) && event.GetModifiers() == wxMOD_CONTROL)
	{
		// CTRL-N => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doNewParticleSystem();
	}
	else if ((event.GetKeyCode() == 76 || event.GetKeyCode() == 108) && event.GetModifiers() == wxMOD_CONTROL)
	{
		// CTRL-L => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doLoadFile();
	}
	else if ((event.GetKeyCode() == 82 || event.GetKeyCode() == 114) && event.GetModifiers() == wxMOD_CONTROL)
	{
		// CTRL-R => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doLoadResourcePath();
	}
#ifdef PU_FULL_VERSION
	else if ((event.GetKeyCode() == 83 || event.GetKeyCode() == 115) && event.GetModifiers() == wxMOD_CONTROL)
	{
		// CTRL-S => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doSave();
	}
	else if ((event.GetKeyCode() == 83 || event.GetKeyCode() == 115) && event.GetModifiers() == (wxMOD_CONTROL | wxMOD_SHIFT))
	{
		// CTRL-SHIFT-S => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doSaveAs();
	}
#endif // PU_FULL_VERSION
	else if ((event.GetKeyCode() == 81 || event.GetKeyCode() == 113) && event.GetModifiers() == wxMOD_CONTROL)
	{
		// CTRL-Q => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doQuit();
	}
	else if (event.GetKeyCode() == WXK_F6)
	{
		// F6 => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doPlay();
	}
#ifdef PU_FULL_VERSION
	else if (event.GetKeyCode() == WXK_F7)
	{
		// F7 => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doRecord();
	}
#endif // PU_FULL_VERSION
	else if (event.GetKeyCode() == WXK_F8)
	{
		// F8 => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doPause();
	}
	else if (event.GetKeyCode() == WXK_F9)
	{
		// F9 => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doStop();
	}
	else if (event.GetKeyCode() == WXK_F5)
	{
		// F5 => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doCompile();
	}
	else if ((event.GetKeyCode() == 67 || event.GetKeyCode() == 99) && event.GetModifiers() == wxMOD_CONTROL)
	{
		// CTRL-C => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doBackgroundColour();
	}
	else if ((event.GetKeyCode() == 90 || event.GetKeyCode() == 122) && event.GetModifiers() == wxMOD_CONTROL)
	{
		// CTRL-Z => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doCameraReset();
	}
#ifdef PU_FULL_VERSION
	else if ((event.GetKeyCode() == 79 || event.GetKeyCode() == 111) && event.GetModifiers() == wxMOD_CONTROL)
	{
		// CTRL-O => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doOptions();
	}
#endif // PU_FULL_VERSION
	else if (event.GetKeyCode() == WXK_F1)
	{
		// F1 => Menu-item, so redirect
		ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(mRootParent);
		frame->doAbout();
	}
}
//-----------------------------------------------------------------------
bool EditTab::isSystemUpdatedByEditPage(void)
{
	return mEditChanged;
}
//-----------------------------------------------------------------------
ParticleUniverse::ParticleSystem* EditTab::getParticleSystemFromSystemComponent(void)
{
	EditComponent* component = getParticleSystemEditComponent();
	if (!component)
		return 0;

	return static_cast<ParticleUniverse::ParticleSystem*>(component->getPUElement());
}
//-----------------------------------------------------------------------
bool EditTab::_mustStopParticleSystem(void)
{
	// Get the Particle System Edit Component, because it is associated with the Particle System
	EditComponent* component = getParticleSystemEditComponent();
	if (!component)
		return false;

	// Set it to stop
	ParticleUniverse::ParticleSystem* particleSystem = static_cast<ParticleUniverse::ParticleSystem*>(component->getPUElement());
	bool wasStarted = false;
	if (particleSystem && particleSystem->getState() == ParticleUniverse::ParticleSystem::PSS_STARTED)
	{
		wasStarted = true;
		particleSystem->stop();
	}
	return wasStarted;
}
//-----------------------------------------------------------------------
void EditTab::_mustRestartParticleSystem(bool wasStarted)
{
	// Start the system if needed

	if (wasStarted)
	{
		// Get the Particle System Edit Component, because it is associated with the Particle System
		EditComponent* component = getParticleSystemEditComponent();
		if (component)
		{
			ParticleUniverse::ParticleSystem* particleSystem = static_cast<ParticleUniverse::ParticleSystem*>(component->getPUElement());
			if (particleSystem)
			{
				particleSystem->start();
			}
		}
	}
}
//-----------------------------------------------------------------------
void EditTab::_generateNameForComponentAndPUElement(EditComponent* component, ComponentType type)
{
	if (!component)
		return;

	if (!component->getPUElement())
		return;

	Ogre::String name;
	if (type == CT_TECHNIQUE)
	{
		name = "Technique" + Ogre::StringConverter::toString(mTechniqueCounter);
		mTechniqueCounter++;
		static_cast<ParticleUniverse::ParticleTechnique*>(component->getPUElement())->setName(name);
	}
	else if (type == CT_EMITTER)
	{
		name = "Emitter" + Ogre::StringConverter::toString(mEmitterCounter);
		mEmitterCounter++;
		static_cast<ParticleUniverse::ParticleEmitter*>(component->getPUElement())->setName(name);
	}
	else if (type == CT_AFFECTOR)
	{
		name = "Affector" + Ogre::StringConverter::toString(mAffectorCounter);
		mAffectorCounter++;
		static_cast<ParticleUniverse::ParticleAffector*>(component->getPUElement())->setName(name);
	}
	else if (type == CT_OBSERVER)
	{
		name = "Observer" + Ogre::StringConverter::toString(mObserverCounter);
		mObserverCounter++;
		static_cast<ParticleUniverse::ParticleObserver*>(component->getPUElement())->setName(name);
	}

	if (component->getPropertyWindow())
	{
		component->getPropertyWindow()->setComponentName(name);
	}
	component->setComponentName(name);
	component->setCaption();
}
//-----------------------------------------------------------------------
void EditTab::adjustNames(const Ogre::String& oldName, const Ogre::String& newName, ComponentType type)
{
	// Run through the all components and change references that are based on the name
	std::vector<EditComponent*>::iterator it;
	for (it = mComponents.begin(); it != mComponents.end(); ++it)
	{
		EditComponent* component = *it;
		if (component->getComponentType() == CT_AFFECTOR)
		{
			// 1. Affectors: Exclude name
			ParticleUniverse::ParticleAffector* affector = static_cast<ParticleUniverse::ParticleAffector*>(component->getPUElement());
			if (affector->hasEmitterToExclude(oldName))
			{
				affector->removeEmitterToExclude(oldName);
				affector->addEmitterToExclude(newName);
			}
		}
		else if (component->getComponentType() == CT_EMITTER)
		{
			// 2. Emitters: Emits name
			ParticleUniverse::ParticleEmitter* emitter = static_cast<ParticleUniverse::ParticleEmitter*>(component->getPUElement());
			if (emitter->getEmitsName() == oldName)
			{
				if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_AFFECTOR && type == CT_AFFECTOR)
				{
					emitter->setEmitsName(newName);
				}
				else if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_EMITTER && type == CT_EMITTER)
				{
					emitter->setEmitsName(newName);
				}
				else if (emitter->getEmitsType() == ParticleUniverse::Particle::PT_TECHNIQUE && type == CT_TECHNIQUE)
				{
					emitter->setEmitsName(newName);
				}
			}
		}
		else if (component->getComponentType() == CT_HANDLER)
		{
			if (component->getComponentSubType() == CST_HANDLER_DO_ENABLE_COMPONENT)
			{
				// 3. DoEnableComponentEventHandler: Enables component name
				ParticleUniverse::DoEnableComponentEventHandler* handler = static_cast<ParticleUniverse::DoEnableComponentEventHandler*>(component->getPUElement());
				if (handler->getComponentName() == oldName)
				{
					if (handler->getComponentType() == ParticleUniverse::CT_AFFECTOR && type == CT_AFFECTOR)
					{
						handler->setComponentName(newName);
					}
					if (handler->getComponentType() == ParticleUniverse::CT_EMITTER && type == CT_EMITTER)
					{
						handler->setComponentName(newName);
					}
					if (handler->getComponentType() == ParticleUniverse::CT_TECHNIQUE && type == CT_TECHNIQUE)
					{
						handler->setComponentName(newName);
					}
					if (handler->getComponentType() == ParticleUniverse::CT_OBSERVER && type == CT_OBSERVER)
					{
						handler->setComponentName(newName);
					}
				}
			}
			else if (component->getComponentSubType() == CST_HANDLER_DO_PLACEMENT_PARTICLE)
			{
				// 4. DoPlacementParticleEventHandler
				ParticleUniverse::DoPlacementParticleEventHandler* handler = static_cast<ParticleUniverse::DoPlacementParticleEventHandler*>(component->getPUElement());
				if (handler->getForceEmitterName() == oldName)
				{
					if (type == CT_EMITTER)
					{
						handler->setForceEmitterName(newName);
					}
				}
			}
			else if (component->getComponentSubType() == CST_HANDLER_DO_AFFECTOR)
			{
				// 5. DoAffectorEventHandler
				ParticleUniverse::DoAffectorEventHandler* handler = static_cast<ParticleUniverse::DoAffectorEventHandler*>(component->getPUElement());
				if (handler->getAffectorName() == oldName)
				{
					if (type == CT_AFFECTOR)
					{
						handler->setAffectorName(newName);
					}
				}
			}
		}
		
		// This is not an else-if, because if the emittercode is executed, this must also be executed (2)
		if (component->getComponentType() == CT_EMITTER && component->getComponentSubType() == CST_EMITTER_SLAVE)
		{
			// 6. SlaveEmitter: Master technique and emitter
			ParticleUniverse::SlaveEmitter* emitter = static_cast<ParticleUniverse::SlaveEmitter*>(component->getPUElement());
			if (emitter->getMasterEmitterName() == oldName && type == CT_EMITTER)
			{
				emitter->setMasterEmitterName(newName);
			}
			if (emitter->getMasterTechniqueName() == oldName && type == CT_TECHNIQUE)
			{
				emitter->setMasterTechniqueName(newName);
			}
		}
	}
}
//-----------------------------------------------------------------------
void EditTab::enableTools(bool enable)
{
	if (!mEditTools)
		return;

	if (enable)
	{
		mEditTools->Show();
	}
	else
	{
		mEditTools->Hide();
	}
}
//-----------------------------------------------------------------------
void EditTab::FreezeClientWindow()
{
	mClientWindow->Freeze();
}
//-----------------------------------------------------------------------
void EditTab::ThawClientWindow()
{
	mClientWindow->Thaw();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ClientWindow::ClientWindow(wxMDIParentFrame* parent) : wxMDIClientWindow(parent)
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
EditTools::EditTools(EditTab* parent) : 
		wxDialog(
		parent, 
		wxID_ANY, 
		_("Tools"), 
		wxPoint(16, 16),
		wxSize(TOOLS_EDIT_WIDTH, TOOLS_EDIT_HEIGHT), 
		wxCAPTION, 
		_("edittools")),
		mParent(parent)
{
	mEditToolbar = new UIEditToolbar(this);
	Connect(ID_ADD_TECHNIQUE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnNewTechnique));
	Connect(ID_ADD_RENDERER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnNewRenderer));
	Connect(ID_ADD_EMITTER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnNewEmitter));
	Connect(ID_ADD_AFFECTOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnNewAffector));
	Connect(ID_ADD_OBSERVER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnNewObserver));
	Connect(ID_ADD_HANDLER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnNewHandler));
	Connect(ID_ADD_BEHAVIOUR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnNewBehaviour));
	Connect(ID_ADD_EXTERN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnNewExtern));
	Connect(ID_CONNECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnConnect));
	Connect(ID_DISCONNECT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnDisconnect));
	Connect(ID_HELP_HTML, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnHelp));
	Connect(ID_CURSOR, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(EditTools::OnCursor));
}
//-----------------------------------------------------------------------
bool EditTools::Destroy(void)
{
	delete mEditToolbar;
	return wxDialog::Destroy();
}
//-----------------------------------------------------------------------
void EditTools::OnNewTechnique(wxCommandEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	EditComponent* component = parent->createTechniqueEditComponent();
	parent->createTechniqueForComponent(component);
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::OnNewRenderer(wxCommandEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	EditComponent* component = parent->createRendererEditComponent(CST_RENDERER_BILLBOARD);
	parent->createRendererForComponent(RENDERER_BILLBOARD, component);
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::OnNewEmitter(wxCommandEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	EditComponent* component = parent->createEmitterEditComponent(CST_EMITTER_BOX);
	parent->createEmitterForComponent(EMITTER_BOX, component);
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::OnNewAffector(wxCommandEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	EditComponent* component = parent->createAffectorEditComponent(CST_AFFECTOR_ALIGN);
	parent->createAffectorForComponent(AFFECTOR_ALIGN, component);
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::OnNewObserver(wxCommandEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	EditComponent* component = parent->createObserverEditComponent(CST_OBSERVER_ON_CLEAR);
	parent->createObserverForComponent(OBSERVER_ON_CLEAR, component);
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::OnNewHandler(wxCommandEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	EditComponent* component = parent->createHandlerEditComponent(CST_HANDLER_DO_AFFECTOR);
	parent->createHandlerForComponent(HANDLER_DO_AFFECTOR, component);
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::OnNewBehaviour(wxCommandEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	EditComponent* component = parent->createBehaviourEditComponent(CST_BEHAVIOUR_SLAVE);
	parent->createBehaviourForComponent(BEHAVIOUR_SLAVE, component);
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::OnNewExtern(wxCommandEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	EditComponent* component = parent->createExternEditComponent(CST_EXTERN_BOX_COLLIDER);
	parent->createExternForComponent(EXTERN_BOX_COLLIDER, component);
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::OnConnect(wxCommandEvent& event)
{
	// The connect icon has been pressed, so the mouse is in 'connect mode'.
	EditTab* parent = static_cast<EditTab*>(GetParent());
	parent->setConnectionMode(EditTab::CM_CONNECT_STARTING);
}
//-----------------------------------------------------------------------
void EditTools::OnDisconnect(wxCommandEvent& event)
{
	// The disconnect icon has been pressed, so the mouse is in 'disconnect mode'.
	EditTab* parent = static_cast<EditTab*>(GetParent());
	parent->setConnectionMode(EditTab::CM_DISCONNECT);
}
//-----------------------------------------------------------------------
void EditTools::OnHelp(wxCommandEvent& event)
{
	// Get property window
	EditTab* parent = static_cast<EditTab*>(GetParent());
	ParticleUniverseEditorFrame* frame = static_cast<ParticleUniverseEditorFrame*>(parent->GetGrandParent());
	frame->OnHelp();
//	PropertyWindow* propWin = frame->getPropertyWindow();
//	if (propWin)
//	{
//		// Start the browser
//		wxString url = CURRENT_DIR + SCRIPT_DIR + propWin->getHelpHtml();
//
//		// TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
//		// TODO: Starting the browser causes the application to crash if clicking on the edit components
//		ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
//		//ShellExecute((HWND)frame->GetHWND(), "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
//		// TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
//	}
}
//-----------------------------------------------------------------------
void EditTools::OnCursor(wxCommandEvent& event)
{
	// Reset the cursor
	EditTab* parent = static_cast<EditTab*>(GetParent());
	parent->getEditCanvas()->OnMouseRButtonPressed(wxMouseEvent(0));
}
//-----------------------------------------------------------------------
void EditTools::notifyConnectionsChanged(void)
{
	// A connection has been added or deleted
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::notifyDeleteSystem(void)
{
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::notifyDeleteTechnique(void)
{
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::notifyDeleteRenderer(void)
{
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::notifyDeleteEmitter(void)
{
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::notifyDeleteAffector(void)
{
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::notifyDeleteObserver(void)
{
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::notifyDeleteHandler(void)
{
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::notifyDeleteBehaviour(void)
{
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::notifyDeleteExtern(void)
{
	resetIcons();
}
//-----------------------------------------------------------------------
void EditTools::resetIcons(void)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	unsigned int connections = (parent->getEditCanvas())->getNumberOfConnections();
	UIEditIcons2* editIcons = mEditToolbar->getEditIcons2();
	editIcons->reset(parent->getNumberOfComponents(), connections);
}
