/*
  ==============================================================================

    LinkObject.cpp
    Created: 16 Apr 2012 1:23:34am
    Author:  Peter Vasil

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

#include "LinkObject.h"

LinkObject::LinkObject(ObjectType objType_)
: BaseObject(objType_)
{

}

LinkObject::~LinkObject()
{

}

void LinkObject::addParameter(float value)
{
	parameters.add(value);
}

void LinkObject::addLabel(const String& label)
{
	labels.add(label);
}

void LinkObject::setStartVertex(const String& startVert)
{
	startingVertex = startVert;
}
void LinkObject::setEndVertex(const String& endVert)
{
	endVertex = endVert;
}
