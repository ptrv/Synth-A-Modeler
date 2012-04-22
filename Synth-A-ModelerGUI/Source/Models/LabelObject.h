/*
  ==============================================================================

    LabelObject.h
    Created: 18 Apr 2012 12:09:08am
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

#ifndef __LABELOBJECT_H_CBE5136__
#define __LABELOBJECT_H_CBE5136__

#include "BaseObject.h"

struct LabelParameterIdx
{
	String labelName;
	int parameterId;
};
class LabelObject : public BaseObject
{
public:
	LabelObject(ObjectType objType_= LabelType);
	virtual ~LabelObject();

	void setTitle(const String& value);
	const String& getTitle() const { return title; }
	void addParameter(const String& value);
	void setParameterIndex(const String& key, int value);
	const String& getParamName() const { return labelParamIdx.labelName; }
	const int getParamId() const { return labelParamIdx.parameterId; }

private:
	String title;
	Array<String> parameters;
	LabelParameterIdx labelParamIdx;
};


#endif  // __LABELOBJECT_H_CBE5136__