/*
  ==============================================================================

    Connector.h
    Created: 27 Aug 2012 2:11:55pm
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

#ifndef __CONNECTOR_H_D0DAD273__
#define __CONNECTOR_H_D0DAD273__


#include "JuceHeader.h"
#include "SelectableObject.h"

namespace synthamodeler
{
class ObjController;
class BaseObjectComponent;
class ObjectComponent;
class LinkComponent;

class Connector : public Component,
                  public ChangeListener,
                  public SelectableObject
{
public:
    Connector(ObjController& owner_,
              BaseObjectComponent* objComp_,
              ObjectComponent* targetComp_);
    virtual ~Connector();

    void resized();
    void paint(Graphics& g);

    void update();
    
    void resizeToFit();

    bool hitTest(int x, int y);
    
    void getPoints(float& x1, float& y1, float& x2, float& y2) const;

    void changeListenerCallback (ChangeBroadcaster* source);
    
    void mouseDown(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);

    BaseObjectComponent* getSourceObject()             { return sourceComp; }
    const BaseObjectComponent* getSourceObject() const { return sourceComp; }
    ObjectComponent* getTargetObject()                 { return targetComp; }
    const ObjectComponent* getTargetObject() const     { return targetComp; }
    
    Rectangle<int> getIntersectioBounds();

    void setSegmented(bool isSegmented) { segmented = isSegmented; resized(); }
private:
    ObjController& owner;
    float lastInputX, lastInputY, lastOutputX, lastOutputY;
    Path linePath, hitPath;

    bool segmented;
    bool mouseDownSelectStatus;
    WeakReference<BaseObjectComponent> sourceComp;
    WeakReference<ObjectComponent> targetComp;

    WeakReference<ObjectComponent> objectComp;
    WeakReference<LinkComponent> linkComp;
    
    void getDistancesFromEnds (int x, int y, double& distanceFromStart, double& distanceFromEnd) const
    {
        float x1, y1, x2, y2;
        getPoints (x1, y1, x2, y2);

        distanceFromStart = juce_hypot (x - (x1 - getX()), y - (y1 - getY()));
        distanceFromEnd = juce_hypot (x - (x2 - getX()), y - (y2 - getY()));
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Connector);
};
}


#endif  // __CONNECTOR_H_D0DAD273__
