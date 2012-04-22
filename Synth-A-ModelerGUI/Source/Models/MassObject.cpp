/*
  ==============================================================================

    MassObject.cpp
    Created: 16 Apr 2012 1:23:24am
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

#include "MassObject.h"

MassObject::MassObject(ObjectType objType_)
: BaseObject(objType_)
{

}

MassObject::~MassObject()
{

}

void MassObject::addParameter(float value)
{
	parameters.add(value);
}

void MassObject::addLabel(const String& label)
{
	labels.add(label);
}