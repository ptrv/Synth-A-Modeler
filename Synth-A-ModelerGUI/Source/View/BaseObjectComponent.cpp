/*
  ==============================================================================

    BaseObjectComponent.cpp
    Created: 23 Aug 2012 2:52:50pm
    Author:  Peter vasil

  ==============================================================================

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

 */

#include "../Application/CommonHeaders.h"
#include "ObjectsHolder.h"

#include "BaseObjectComponent.h"

BaseObjectComponent::BaseObjectComponent(ObjController& owner_, ValueTree data_)
: owner(owner_), data(data_)
{
    
}

BaseObjectComponent::~BaseObjectComponent()
{
    
}

ValueTree BaseObjectComponent::getData()
{
    return data;
}

ObjectsHolder* BaseObjectComponent::getObjectsHolder() const noexcept
{
    return findParentComponentOfClass<ObjectsHolder>();
}

void BaseObjectComponent::showContextMenu()
{
	PopupMenu m;
	m.addItem (1, "Edit");
	m.addSeparator();
	m.addItem (2, "Help");

	const int r = m.show();

	if (r == 1)
	{
		getObjectsHolder()->editObjectProperties(this);
	}
	else if (r == 2)
	{
        Utils::openHelpUrl(data.getType());
		DBG("open help for " + data.getType().toString());
	}
}