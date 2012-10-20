/*
  ==============================================================================

    CommentComponent.cpp
    Created: 17 Oct 2012 4:56:15pm
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

#include "../Application/CommonHeaders.h"
#include "BaseObjectComponent.h"
#include "SelectableObject.h"
#include "LinkComponent.h"
#include "../Controller/ObjController.h"

#include "CommentComponent.h"

using namespace synthamodeler;

CommentComponent::CommentComponent(ObjController& owner_, ValueTree data_)
: BaseObjectComponent(owner_, data_)
{

}

CommentComponent::~CommentComponent()
{
    
}