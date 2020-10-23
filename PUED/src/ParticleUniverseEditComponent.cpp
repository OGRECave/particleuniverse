/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2012 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#include "ParticleUniverseEditorPCH.h"
#include "ParticleUniverseEditor.h"
#include "wx/ogre/utils.h"
#include "ParticleUniverseEditComponent.h"
#include "ParticleUniverseEditTab.h"
#include "ParticleUniverseSystemPropertyWindow.h"
#include "ParticleUniverseTechniquePropertyWindow.h"
#include "ParticleUniverseRendererPropertyWindow.h"
#include "ParticleUniverseEmitterPropertyWindow.h"
#include "ParticleUniverseAffectorPropertyWindow.h"
#include "ParticleUniverseObserverPropertyWindow.h"

//-----------------------------------------------------------------------
EditComponent::EditComponent(
		wxMDIParentFrame* parent, 
		const Ogre::String& name, 
		ComponentType type, 
		ComponentSubType subType, 
		const wxColour& backgroundColor,
		wxSize size,
		long style) : 

		wxMDIChildFrame(
		parent,
		wxID_ANY, 
		wxT(""),
		wxDefaultPosition, 
		size,
		style,
		ogre2wx(name)),
	mName(name),
	mType(type),
	mSubType(subType),
	mParentName(Ogre::StringUtil::BLANK),
	mSelectedPolicy(0),
	mOldPropertyWindow(0),
	mPUElement(0),
	mRootFrame(0)
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

	mRootParent = parent->GetParent()->GetParent();
	setCaption();
	SetBackgroundColour(backgroundColor);
//	wxFont font(10, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_NORMAL, true);
//	wxStaticText* text = new wxStaticText(this, wxID_ANY, "\n" + ogre2wx(name), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
//	text->SetFont(font);

	mPropertyWindow = 0,
	mPropertyWindow = createPropertyWindow(mSubType);
	mOriginalSize = GetSize();

	// Add connections if the component is double clicked, closed, moved, ...etc.
	Connect(wxID_ANY, wxEVT_MOVE, wxMoveEventHandler(EditComponent::OnMove));
	Connect(wxID_ANY, wxEVT_MOVING, wxMoveEventHandler(EditComponent::OnMove));
	Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(EditComponent::OnClose));
	Connect(wxEVT_MOTION, wxMouseEventHandler(EditComponent::OnMouseMove));
	Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(EditComponent::OnWindowEnter));
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(EditComponent::OnWindowLeave));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(EditComponent::OnMouseLButtonPressed));
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(EditComponent::OnMouseRButtonPressed));
	Connect(wxEVT_ACTIVATE, wxActivateEventHandler(EditComponent::OnActivate));
}
//-----------------------------------------------------------------------
EditComponent::~EditComponent(void)
{
	// Delete all policies
	std::vector<ConnectionPolicy*>::iterator itCp;
	std::vector<ConnectionPolicy*>::iterator itCpEnd = mPolicies.end();
	for (itCp = mPolicies.begin(); itCp != itCpEnd; ++itCp)
	{
		delete (*itCp);
	}
	mPolicies.clear();

	// Delete all connections, but on close they are already deleted.
	std::vector<Connection*>::iterator itC;
	std::vector<Connection*>::iterator itCEnd = mConnections.end();
	for (itC = mConnections.begin(); itC != itCEnd; ++itC)
	{
		delete (*itC);
	}
	mConnections.clear();

	// Delete all unique relations
	std::vector<UniqueRelation*>::iterator it;
	std::vector<UniqueRelation*>::iterator itEnd = mUniqueRelations.end();
	for (it = mUniqueRelations.begin(); it != itEnd; ++it)
	{
		delete *it;
	}
	mUniqueRelations.clear();
}
//-----------------------------------------------------------------------
ComponentType EditComponent::getComponentType(void) const
{
	return mType;
}
//-----------------------------------------------------------------------
ComponentSubType EditComponent::getComponentSubType(void) const
{
	return mSubType;
}
//-----------------------------------------------------------------------
void EditComponent::addPolicy(
	ComponentRelation relation, 
	ComponentRelationDirection relationDirection,
	const wxString& relationDescription,
	ComponentType typeToBeConnectedWith, 
	ComponentSubType subTypeToBeConnectedWith,
	bool multipleConnectionsPossible,
	bool ignoreSubType,
	const Ogre::String& colourCode,
	int lineStyle)
{
	mPolicies.push_back(new ConnectionPolicy(relation, 
		relationDirection,
		relationDescription,
		typeToBeConnectedWith, 
		subTypeToBeConnectedWith,
		multipleConnectionsPossible,
		ignoreSubType,
		colourCode,
		lineStyle));
}
//-----------------------------------------------------------------------
void EditComponent::addUniqueRelation(ComponentRelation relation, ComponentRelationDirection relationDirection)
{
	mUniqueRelations.push_back(new UniqueRelation(relation, relationDirection));
}
//-----------------------------------------------------------------------
void EditComponent::addConnection(EditComponent* componentToBeConnectedWith, 
	ComponentRelation relation, 
	ComponentRelationDirection relationDirection)
{
	mConnections.push_back(new Connection(componentToBeConnectedWith, relation, relationDirection));

	// Loop through the policies and possibly lock it
	std::vector<ConnectionPolicy*>::iterator it;
	std::vector<ConnectionPolicy*>::iterator itEnd = mPolicies.end();
	for (it = mPolicies.begin(); it != itEnd; ++it)
	{
		if (isRelationUnique(relation, relationDirection))
		{
			// Ignore a possible policy setting of mMultipleConnectionsPossible = true and just override
			(*it)->validateAndLock(relation, relationDirection);
		}
		else
		{
			(*it)->validateAndLock(relation,
				relationDirection,
				componentToBeConnectedWith->getComponentType(),
				componentToBeConnectedWith->getComponentSubType());
		}
	}
}
//-----------------------------------------------------------------------
ConnectionPolicy* EditComponent::getPolicy(ComponentRelation relation, 
	ComponentRelationDirection relationDirection,
	ComponentType typeToBeConnectedWith, 
	ComponentSubType subTypeToBeConnectedWith)
{
	std::vector<ConnectionPolicy*>::iterator it;
	std::vector<ConnectionPolicy*>::iterator itEnd = mPolicies.end();
	ConnectionPolicy* policy;
	for (it = mPolicies.begin(); it != itEnd; ++it)
	{
		policy = *it;
		if (policy->getRelation() == relation && 
			policy->getRelationDirection() == relationDirection && 
			policy->getTypeToBeConnectedWith() == typeToBeConnectedWith)
		{
			if (policy->ignoreSubType())
			{
				return policy;
			}
			else if (policy->getSubTypeToBeConnectedWith() == subTypeToBeConnectedWith)
			{
				return policy;
			}
		}
	}
	return 0;
}
//-----------------------------------------------------------------------
bool EditComponent::isRelationUnique(ComponentRelation relation, ComponentRelationDirection relationDirection)
{
	std::vector<UniqueRelation*>::iterator it;
	std::vector<UniqueRelation*>::iterator itEnd = mUniqueRelations.end();
	for (it = mUniqueRelations.begin(); it != itEnd; ++it)
	{
		if ((*it)->isRelationUnique(relation, relationDirection))
		{
			// Found
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------
bool EditComponent::isConnectionPossible(void)
{
	std::vector<ConnectionPolicy*>::iterator it;
	std::vector<ConnectionPolicy*>::iterator itEnd = mPolicies.end();
	for (it = mPolicies.begin(); it != itEnd; ++it)
	{
		if (!(*it)->isPolicyLocked())
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------
bool EditComponent::isConnectionPossible(EditComponent* component)
{
	if (component)
	{
		std::vector<ConnectionPolicy*>::iterator it;
		std::vector<ConnectionPolicy*>::iterator itEnd = mPolicies.end();
		for (it = mPolicies.begin(); it != itEnd; ++it)
		{
			// The policy of 'this' component cannot be checked against a relation and relation direction (because there isn't one)
			ConnectionPolicy* connectionPolicy = *it;
			if ((*it)->isConnectionPossible(component->getComponentType(), component->getComponentSubType()))
			{
				// The second one (the component) can be checked against all four attributes
				if (component->isConnectionPossible(connectionPolicy->getRelation(), 
					getOppositeRelationDirection(connectionPolicy->getRelationDirection()),
					mType, 
					mSubType))
				{
					return true;
				}
			}
		}
	}
	return false;
}
//-----------------------------------------------------------------------
bool EditComponent::isConnectionPossible(ComponentRelation relation, 
	ComponentRelationDirection relationDirection,
	ComponentType typeToBeConnectedWith, 
	ComponentSubType subTypeToBeConnectedWith)
{
	std::vector<ConnectionPolicy*>::iterator it;
	std::vector<ConnectionPolicy*>::iterator itEnd = mPolicies.end();
	for (it = mPolicies.begin(); it != itEnd; ++it)
	{
		if ((*it)->isConnectionPossible(relation, relationDirection, typeToBeConnectedWith, subTypeToBeConnectedWith))
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------
bool EditComponent::isDisconnectionPossible(void)
{
	return !mConnections.empty();
}
//-----------------------------------------------------------------------
bool EditComponent::isConnected(EditComponent* componentToBeConnectedWith, ComponentRelation relation, ComponentRelationDirection relationDirection)
{
	std::vector<Connection*>::iterator it;
	std::vector<Connection*>::iterator itEnd = mConnections.end();
	Connection* connection;
	for (it = mConnections.begin(); it != itEnd; ++it)
	{
		connection = *it;
		if (connection->getComponentToBeConnectedWith() == componentToBeConnectedWith && 
			connection->getRelation() == relation && 
			connection->getRelationDirection() == relationDirection)
		{
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------
void EditComponent::OnMove(wxMoveEvent& event)
{
	// Notify the parent
	(static_cast<EditTab*>(GetParent()))->refreshCanvas();
}
//-----------------------------------------------------------------------
void EditComponent::OnClose(wxCloseEvent& event)
{
	// Notify its child
	// ??????????????????

	// Before this component is deleted, notify other components (not components that are connected) that this one is going to be deleted.
	(static_cast<EditTab*>(GetParent()))->notifyReferers(this, EditTab::SE_CLOSE);

	// Delete all connections, including that of other components
	_sortConnections(); // The order of deleting connections matter, so rearrange them.
	std::vector<Connection*>::iterator it;
	std::vector<Connection*>::iterator itEnd = mConnections.end();
	for (it = mConnections.begin(); it != itEnd; ++it)
	{
		Connection* connection = *it;
		EditComponent* componentConnectedWith = (*it)->getComponentToBeConnectedWith();
		componentConnectedWith->deleteConnection(this, (*it)->getRelation(), getOppositeRelationDirection((*it)->getRelationDirection()));
		unlockPolicy(connection->getRelation(),
			connection->getRelationDirection(),
			connection->getComponentToBeConnectedWith()->getComponentType(),
			connection->getComponentToBeConnectedWith()->getComponentSubType());
		delete *it;
	}
	mConnections.clear();

	// Notify the parent that the component is removed.
	(static_cast<EditTab*>(GetParent()))->notifyComponentRemoved(this);

	// Remove corresponding propertyWindow.
	if (mPropertyWindow)
	{
		(static_cast<EditTab*>(GetParent()))->removePropertyWindow(mPropertyWindow);
		mPropertyWindow->Destroy();
		mPropertyWindow = 0;
	}
	if (mOldPropertyWindow)
	{
		(static_cast<EditTab*>(GetParent()))->removePropertyWindow(mOldPropertyWindow);
		mOldPropertyWindow->Destroy();
		mOldPropertyWindow = 0;
	}

	Destroy();
}
//------------------------------------------------------------------------------
void EditComponent::_sortConnections(void)
{
	std::vector<Connection*> temp;
	std::vector<Connection*>::iterator it;
	std::vector<Connection*>::iterator itEnd = mConnections.end();

	for (it = mConnections.begin(); it != itEnd; ++it) 
	{
		ComponentRelation relation = (*it)->getRelation();
		if (relation == CR_PLACE ||
			relation == CR_FORCE ||
			relation == CR_ENABLE ||
			relation == CR_EMIT ||
			relation == CR_SLAVE)
		{
			temp.push_back(*it);
		}
	}
	for (it = mConnections.begin(); it != itEnd; ++it)
	{
		ComponentRelation relation = (*it)->getRelation();
		if (relation != CR_PLACE &&
			relation != CR_FORCE &&
			relation != CR_ENABLE &&
			relation != CR_EMIT &&
			relation != CR_SLAVE &&
			relation != CR_INCLUDE)
		{
			temp.push_back(*it);
		}
	}
	for (it = mConnections.begin(); it != itEnd; ++it)
	{
		if ((*it)->getRelation() == CR_INCLUDE)
		{
			temp.push_back(*it);
		}
	}

	mConnections.clear();
	mConnections = temp;
}
//------------------------------------------------------------------------------
void EditComponent::deleteConnection(EditComponent* componentConnectedWith, 
	ComponentRelation relation, 
	ComponentRelationDirection relationDirection)
{
	std::vector<Connection*>::iterator it;
	std::vector<Connection*>::iterator itEnd = mConnections.end();
	for (it = mConnections.begin(); it != itEnd; ++it)
	{
		if (componentConnectedWith == (*it)->getComponentToBeConnectedWith() &&
			relation == (*it)->getRelation() &&
			relationDirection == (*it)->getRelationDirection())
		{
			(static_cast<EditTab*>(GetParent()))->notifyConnectionRemoved(this, componentConnectedWith, relation, relationDirection);
			unlockPolicy(relation, relationDirection, componentConnectedWith->getComponentType(), componentConnectedWith->getComponentSubType());
			delete (*it);
			mConnections.erase(it);
			return;
		}
	}
}
//------------------------------------------------------------------------------
void EditComponent::deleteConnection(Connection* connection)
{
	if (!connection)
		return;

	std::vector<Connection*>::iterator it;
	std::vector<Connection*>::iterator itEnd = mConnections.end();
	for (it = mConnections.begin(); it != itEnd; ++it)
	{
		Connection* connectionFromList = *it;
		if (connection == connectionFromList)
		{
			connection->getComponentToBeConnectedWith()->deleteConnection(this, 
				connection->getRelation(), 
				getOppositeRelationDirection(connection->getRelationDirection()));
			unlockPolicy(connection->getRelation(), 
				connection->getRelationDirection(), 
				connection->getComponentToBeConnectedWith()->getComponentType(),
				connection->getComponentToBeConnectedWith()->getComponentSubType());
			delete (*it);
			mConnections.erase(it);
			return;
		}
	}
}
//------------------------------------------------------------------------------
void EditComponent::deleteConnection(ComponentRelation relation, ComponentRelationDirection relationDirection)
{
	std::vector<Connection*>::iterator it = mConnections.begin();
	std::vector<Connection*>::iterator itEnd = mConnections.end();
	for (it = mConnections.begin(); it != itEnd; ++it)
	{
		Connection* connectionFromList = *it;
		if (connectionFromList->getRelation() == relation && connectionFromList->getRelationDirection() == relationDirection)
		{
			connectionFromList->getComponentToBeConnectedWith()->deleteConnection(this, 
				relation, 
				getOppositeRelationDirection(relationDirection));
			unlockPolicy(relation, 
				relationDirection, 
				connectionFromList->getComponentToBeConnectedWith()->getComponentType(),
				connectionFromList->getComponentToBeConnectedWith()->getComponentSubType());
			delete (*it);
			mConnections.erase(it);
			return;
		}
	}
}
//------------------------------------------------------------------------------
void EditComponent::unlockPolicy(ComponentRelation relation, 
	ComponentRelationDirection relationDirection,
	ComponentType typeToBeConnectedWith, 
	ComponentSubType subTypeToBeConnectedWith)
{
	std::vector<ConnectionPolicy*>::iterator it;
	std::vector<ConnectionPolicy*>::iterator itEnd = mPolicies.end();
	for (it = mPolicies.begin(); it != itEnd; ++it)
	{
		if (isRelationUnique(relation, relationDirection))
		{
			(*it)->validateAndLock(relation, relationDirection, false);
		}
		else
		{
			(*it)->validateAndLock(relation,
				relationDirection,
				typeToBeConnectedWith,
				subTypeToBeConnectedWith,
				false);
		}
	}
}
//------------------------------------------------------------------------------
void EditComponent::OnMouseMove(wxMouseEvent& event)
{
	wxPoint position = event.GetPosition();
	(static_cast<EditTab*>(GetParent()))->notifyMouseMovedInComponent(this, position);
}
//------------------------------------------------------------------------------
void EditComponent::OnWindowEnter(wxMouseEvent& event)
{
	// Change mouse cursor if the policies don't allow connection
	EditTab* parent = static_cast<EditTab*>(GetParent());
	if (parent->getConnectionMode() == EditTab::CM_CONNECT_ENDING)
	{
		// Check both sides
		if (!isConnectionPossible(parent->getStartConnector()))
		{
			wxCursor connectCursor = wxCursor(wxImage(ICONS_DIR + wxT("no_connect.png")));
			SetCursor(connectCursor);
		}
	}
	else if (parent->getConnectionMode() == EditTab::CM_CONNECT_STARTING)
	{
		// Only check if connection is at all possible
		if (!isConnectionPossible())
		{
			wxCursor connectCursor = wxCursor(wxImage(ICONS_DIR + wxT("no_connect.png")));
			SetCursor(connectCursor);
		}
	}
	else if (parent->getConnectionMode() == EditTab::CM_DISCONNECT)
	{
		// Only check if disconnection is possible
		if (!isDisconnectionPossible())
		{
			wxCursor connectCursor = wxCursor(wxImage(ICONS_DIR + wxT("no_disconnect.png")));
			SetCursor(connectCursor);
		}
	}
}
//------------------------------------------------------------------------------
void EditComponent::OnWindowLeave(wxMouseEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	if (parent->getConnectionMode() == EditTab::CM_CONNECT_STARTING)
	{
		// Reset mouse cursor to connecting cursor
		wxCursor connectCursor = wxCursor(wxImage(ICONS_DIR + wxT("connect.png")));
		SetCursor(connectCursor);
	}
	else if (parent->getConnectionMode() == EditTab::CM_DISCONNECT)
	{
		// Reset mouse cursor to disconnecting cursor
		wxCursor connectCursor = wxCursor(wxImage(ICONS_DIR + wxT("disconnect.png")));
		SetCursor(connectCursor);
	}
}
//------------------------------------------------------------------------------
void EditComponent::OnMouseLButtonPressed(wxMouseEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());

	// TODO: The 'if' must be implemented as one single call to the parent
	if (parent->getConnectionMode() == EditTab::CM_CONNECT_ENDING && this != parent->getStartConnector())
	{
		// First make a selection of available policies
		selectPolicy(parent->getStartConnector());
	} 
	else if (parent->getConnectionMode() == EditTab::CM_DISCONNECT)
	{
		// Make a selection of available connections (to be deleted)
		selectConnection(false);
		parent->notifyConnectionsChanged();
		return;
	}

	// Notify the parent
	parent->notifyComponentActivated(this);
}
//------------------------------------------------------------------------------
void EditComponent::OnMouseRButtonPressed(wxMouseEvent& event)
{
	EditTab* parent = static_cast<EditTab*>(GetParent());
	selectConnection(true);
}
//------------------------------------------------------------------------------
void EditComponent::OnActivate(wxActivateEvent& event)
{
	if (IsBeingDeleted())
		return;

	EditTab* parent = static_cast<EditTab*>(GetParent());
	parent->getEditCanvas()->SetPosition(wxPoint(0, 0)); // In case scrolling has ruined the layout.
	parent->setPropertyWindow(mPropertyWindow);
	if (mOldPropertyWindow && !mOldPropertyWindow->IsBeingDeleted())
	{
		/** Remove the old property window here and not in the createPropertyWindow(), because that would give a runtime exception. The
			createPropertyWindow() function sets the old property window (mOldPropertyWindow), which must be destroyed on a later moment.
		*/
		parent->removePropertyWindow(mOldPropertyWindow);
		mOldPropertyWindow->Destroy();
		mOldPropertyWindow = 0;
	}
}
//------------------------------------------------------------------------------
void EditComponent::selectConnection(bool viewOnly)
{
	//	Display connections. These are actual connections this component has with other components
	wxString choices[MAX_NUMBER_OF_CONNECTIONS];
	Connection* connectionsAsArray[MAX_NUMBER_OF_CONNECTIONS];
	std::vector<Connection*>::iterator it;
	std::vector<Connection*>::iterator itEnd = mConnections.end();
	int count = 0;
	Connection* connection = 0;
	for (it = mConnections.begin(); it != itEnd; ++it)
	{
		connection = *it;
		connectionsAsArray[count] = connection;
		if (!getComponentName().empty())
		{
			choices[count] = ogre2wx(getComponentName());
		}
		else
		{
			choices[count] = getComponentType();
			if (!getComponentSubType().empty())
			{
				if (getComponentSubType() != CST_UNDEFINED)
				{
					choices[count] += wxT(" ") + getComponentSubType();
				}
			}
		}
		choices[count] += wxT(" ") + 
			getRelationDescription(connection->getRelation(), 
			connection->getRelationDirection(),
			connection->getComponentToBeConnectedWith()->getComponentType(), 
			connection->getComponentToBeConnectedWith()->getComponentSubType()) + 
			wxT(" ");
		if (!connection->getComponentToBeConnectedWith()->getComponentName().empty())
		{
			choices[count] += ogre2wx(connection->getComponentToBeConnectedWith()->getComponentName());
		}
		else
		{
			choices[count] += connection->getComponentToBeConnectedWith()->getComponentType();
			if (!connection->getComponentToBeConnectedWith()->getComponentSubType().empty())
			{
				if (connection->getComponentToBeConnectedWith()->getComponentSubType() != CST_UNDEFINED)
				{
					choices[count] += wxT(" ") + connection->getComponentToBeConnectedWith()->getComponentSubType();
				}
			}
		}
		count++;
	}

	if (viewOnly)
	{
		GetParent()->Enable(false); // Disables input from the parent
		wxSingleChoiceDialog choiceWindow(this, 
			_("overview of existing connections"), 
			_("Existing connections"), 
			count, 
			choices,
			0,
			wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxOK | wxCENTRE);
			choiceWindow.SetSize(320, 200);
			choiceWindow.ShowModal();
		GetParent()->Enable(true); // Enable input from the parent
		GetParent()->SetFocus();
	}
	else
	{
		if (count > 0)
		{
			GetParent()->Enable(false); // Disables input from the parent
			wxSingleChoiceDialog choiceWindow(this, _("Select a connection to be deleted"), _("Delete a connection"), count, choices);
			choiceWindow.SetSize(320, 200);
			if (choiceWindow.ShowModal() == wxID_OK)
			{
				// Get selected connection
				Connection* connection = connectionsAsArray[choiceWindow.GetSelection()];
				deleteConnection(connection);
			}
			GetParent()->Enable(true); // Enable input from the parent
			GetParent()->SetFocus();
		}
	}
}
//------------------------------------------------------------------------------
ConnectionPolicy* EditComponent::selectPolicy(EditComponent* componentToBeConnectedWith)
{
	wxString choices[MAX_NUMBER_OF_CONNECTIONS];
	ConnectionPolicy* policiesAsArray[MAX_NUMBER_OF_CONNECTIONS];
	std::vector<ConnectionPolicy*>::iterator it;
	std::vector<ConnectionPolicy*>::iterator itEnd = mPolicies.end();
	int count = 0;
	ConnectionPolicy* policy = 0;
	for (it = mPolicies.begin(); it != itEnd; ++it)
	{
		policy = *it;

		/*	Display only available connections. These are connections where the policy of this component allows it to connect
			and if the other component (componentToBeConnectedWith) can also make the same connection.
		**/
		if (componentToBeConnectedWith->isConnectionPossible(policy->getRelation(), 
				getOppositeRelationDirection(policy->getRelationDirection()),
				mType, 
				mSubType) &&
			policy->isConnectionPossible(componentToBeConnectedWith->getComponentType(), componentToBeConnectedWith->getComponentSubType()))
		{
			policiesAsArray[count] = policy;
			choices[count] = getComponentType() + wxT(" ");
			if (!getComponentName().empty())
			{
				choices[count] = ogre2wx(getComponentName());
			}
			else
			{
				choices[count] = getComponentType();
				if (!getComponentSubType().empty())
				{
					if (getComponentSubType() != CST_UNDEFINED)
					{
						choices[count] += wxT(" ") + getComponentSubType();
					}
				}
			}
			choices[count] += wxT(" ") + policy->getRelationDescription() + wxT(" ");
			if (!componentToBeConnectedWith->getComponentName().empty())
			{
				choices[count] += ogre2wx(componentToBeConnectedWith->getComponentName());
			}
			else
			{
				choices[count] += componentToBeConnectedWith->getComponentType();
				if (!componentToBeConnectedWith->getComponentSubType().empty())
				{
					if (componentToBeConnectedWith->getComponentSubType() != CST_UNDEFINED)
					{
						choices[count] += wxT(" ") + componentToBeConnectedWith->getComponentSubType();
					}
				}
			}
			count++;
		}
	}

	if (count > 1)
	{
		GetParent()->Enable(false); // Disables input from the parent
		wxSingleChoiceDialog choiceWindow(this, _("Select a connection type"), _("Add a connection"), count, choices);
		choiceWindow.SetSize(320, 200);
		if (choiceWindow.ShowModal() == wxID_OK)
		{
			mSelectedPolicy = policiesAsArray[choiceWindow.GetSelection()];
		}
		else
		{
			mSelectedPolicy = 0;
		}
		GetParent()->Enable(true); // Enable input from the parent
		GetParent()->SetFocus();
	}
	else if (count == 1)
	{
		mSelectedPolicy = policiesAsArray[0];
	}
	else
	{
		mSelectedPolicy = 0;
	}

	return mSelectedPolicy;
}
//------------------------------------------------------------------------------
ConnectionPolicy* EditComponent::getSelectedPolicy(void)
{
	return mSelectedPolicy;
}
//------------------------------------------------------------------------------
const wxString& EditComponent::getRelationDescription(ComponentRelation relation, 
	ComponentRelationDirection relationDirection, 
	ComponentType typeToBeConnectedWith, 
	ComponentSubType subTypeToBeConnectedWith)
{
	std::vector<ConnectionPolicy*>::iterator it;
	std::vector<ConnectionPolicy*>::iterator itEnd = mPolicies.end();
	for (it = mPolicies.begin(); it != itEnd; ++it)
	{
		wxString description = (*it)->getRelationDescription(relation, relationDirection, typeToBeConnectedWith, subTypeToBeConnectedWith);
		if (description != CRD_UNKNOWN)
		{
			return (*it)->getRelationDescription(relation, relationDirection, typeToBeConnectedWith, subTypeToBeConnectedWith);
		}
	}

	return CRD_UNKNOWN;
}
//------------------------------------------------------------------------------
void EditComponent::setCaption(void)
{
	wxString caption = mType;
	wxString name = ogre2wx(mName);
	if (mSubType != CST_UNDEFINED)
	{
		caption = caption + wxT("(") + mSubType + wxT(")");
	}
	if (!name.empty())
	{
		caption = caption + wxT(" - ") + name;
	}
	SetLabel(caption);
	SetToolTip(caption);
}
//------------------------------------------------------------------------------
const ComponentSubType& EditComponent::getSubType(void) const
{
	return mSubType;
}
//------------------------------------------------------------------------------
void EditComponent::setSubType(ComponentSubType subType)
{
	mSubType = subType;
}
//------------------------------------------------------------------------------
const Ogre::String& EditComponent::getComponentName(void) const
{
	return mName;
}
//------------------------------------------------------------------------------
void EditComponent::setComponentName(const Ogre::String& componentName)
{
	mName = componentName;
}
//------------------------------------------------------------------------------
void EditComponent::refreshCanvas(void)
{
	(static_cast<EditTab*>(GetParent()))->refreshCanvas();
}
//------------------------------------------------------------------------------
PropertyWindow* EditComponent::createPropertyWindow(ComponentSubType subType, const PropertyWindow* propertyWindow)
{
	// Create new propertyWindow and propagate the attributes of the old one.
	// Don't delete the existing one, because it is deleting itself.

	mSubType = subType;
	mOldPropertyWindow = mPropertyWindow;

	if (mType == CT_SYSTEM)
	{
		SystemPropertyWindow* pWin = new SystemPropertyWindow(mRootParent, this, mName);
		pWin->setRootFrame(mRootFrame); // To keep a pointer to the main (root) frame
		mPropertyWindow = pWin;
	}
	else if (mType == CT_TECHNIQUE)
	{
		mPropertyWindow = new TechniquePropertyWindow(mRootParent, this, mName);
	}
	else if (mType == CT_RENDERER)
	{
		if (propertyWindow)
		{
			mPropertyWindow = mRendererPropertyWindowFactory.createRendererPropertyWindow(mSubType, static_cast<RendererPropertyWindow*>(mPropertyWindow));
		}
		else
		{
			mPropertyWindow = mRendererPropertyWindowFactory.createRendererPropertyWindow(mRootParent, this, mName, mSubType);
		}
	}
	else if (mType == CT_EMITTER)
	{
		if (propertyWindow)
		{
			mPropertyWindow = mEmitterPropertyWindowFactory.createEmitterPropertyWindow(mSubType, static_cast<EmitterPropertyWindow*>(mPropertyWindow));
		}
		else
		{
			mPropertyWindow = mEmitterPropertyWindowFactory.createEmitterPropertyWindow(mRootParent, this, mName, mSubType);
		}
	}
	else if (mType == CT_AFFECTOR)
	{
		if (propertyWindow)
		{
			mPropertyWindow = mAffectorPropertyWindowFactory.createAffectorPropertyWindow(mSubType, static_cast<AffectorPropertyWindow*>(mPropertyWindow));
		}
		else
		{
			mPropertyWindow = mAffectorPropertyWindowFactory.createAffectorPropertyWindow(mRootParent, this, mName, mSubType);
		}
	}
	else if (mType == CT_OBSERVER)
	{
		if (propertyWindow)
		{
			mPropertyWindow = mObserverPropertyWindowFactory.createObserverPropertyWindow(mSubType, static_cast<ObserverPropertyWindow*>(mPropertyWindow));
		}
		else
		{
			mPropertyWindow = mObserverPropertyWindowFactory.createObserverPropertyWindow(mRootParent, this, mName, mSubType);
		}
	}
	else if (mType == CT_HANDLER)
	{
		if (propertyWindow)
		{
			mPropertyWindow = mEventHandlerPropertyWindowFactory.createEventHandlerPropertyWindow(mSubType, static_cast<EventHandlerPropertyWindow*>(mPropertyWindow));
		}
		else
		{
			mPropertyWindow = mEventHandlerPropertyWindowFactory.createEventHandlerPropertyWindow(mRootParent, this, mName, mSubType);
		}
	}
	else if (mType == CT_BEHAVIOUR)
	{
		if (propertyWindow)
		{
			mPropertyWindow = mBehaviourPropertyWindowFactory.createBehaviourPropertyWindow(mSubType, static_cast<BehaviourPropertyWindow*>(mPropertyWindow));
		}
		else
		{
			mPropertyWindow = mBehaviourPropertyWindowFactory.createBehaviourPropertyWindow(mRootParent, this, mName, mSubType);
		}
	}
	else if (mType == CT_EXTERN)
	{
		if (propertyWindow)
		{
			mPropertyWindow = mExternPropertyWindowFactory.createExternPropertyWindow(mSubType, static_cast<ExternPropertyWindow*>(mPropertyWindow));
		}
		else
		{
			mPropertyWindow = mExternPropertyWindowFactory.createExternPropertyWindow(mRootParent, this, mName, mSubType);
		}
	}

	mPropertyWindow->Hide();
	(static_cast<EditTab*>(GetParent()))->setPropertyWindow(mPropertyWindow);
	return mPropertyWindow;
}
//------------------------------------------------------------------------------
PropertyWindow* EditComponent::getPropertyWindow(void)
{
	return mPropertyWindow;
}
//-----------------------------------------------------------------------
void EditComponent::notifyPropertyChanged(void)
{
	// Inform the parent
	EditTab* parent = static_cast<EditTab*>(GetParent());
	parent->setSystemUpdatedByEditPage(true);
}
//-----------------------------------------------------------------------
void EditComponent::notifyAdjustNames(const Ogre::String& newName)
{
	// Inform the parent
	EditTab* parent = static_cast<EditTab*>(GetParent());
	parent->adjustNames(mName, newName, mType);
}
//-----------------------------------------------------------------------
ParticleUniverseEditorFrame* EditComponent::getRootFrame(void)
{
	return mRootFrame;
}
//-----------------------------------------------------------------------
void EditComponent::setRootFrame(ParticleUniverseEditorFrame* rootFrame)
{
	mRootFrame = rootFrame;
}
