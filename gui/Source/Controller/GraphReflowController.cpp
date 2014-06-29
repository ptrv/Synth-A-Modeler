/*
  ==============================================================================

    GraphReflowController.cpp
    Created: 24 Jun 2014 12:01:16am
    Author:  peter

  ==============================================================================
*/

#include "../Application/CommonHeaders.h"
#include "../Graph/Node.h"
#include "../View/SelectableObject.h"
#include "../View/BaseObjectComponent.h"
#include "../View/ContentComp.h"
#include "../View/ObjectComponent.h"
#include "../View/LinkComponent.h"
#include "../View/ObjectsHolder.h"
#include "../Utilities/StoredSettings.h"
#include "../Utilities/ObjectsHelper.h"
#include "../Graph/DirectedGraph.h"
#include "../Graph/GraphUtils.h"
#include "../Graph/FlowAlgorithm.h"
#include "../Graph/CircleFlowAlgorithm.h"
#include "../Graph/ForceDirectedFlowAlgorithm.h"
#include "../Controller/ObjController.h"


#include "GraphReflowController.h"

using namespace synthamodeler;

GraphReflowController::GraphReflowController(ObjController& objCtrl)
    : objController(objCtrl), objHolder(nullptr)
{
}

void GraphReflowController::startTimer(int intervalInMilliseconds)
{
    objController.setIsReflowing(true);
    Timer::startTimer(intervalInMilliseconds);
}

void GraphReflowController::stopTimer()
{
    objController.setIsReflowing(false);
    Timer::stopTimer();
}

void GraphReflowController::timerCallback()
{
    if (reflow())
    {
        graph = nullptr;
        objHolder = nullptr;
        stopTimer();
        DBG("stop timer");
    }
}

double timeStep = 0.6;

bool GraphReflowController::reflow()
{
    if (graph != nullptr && objHolder != nullptr)
    {
        bool done = graph->reflow(bounds, objController, timeStep);
        objHolder->repaint();
        return done;
    }
    return true;
}

void GraphReflowController::startReflow(ObjectsHolder& objectsHolder, const int cmdId)
{
    objHolder = &objectsHolder;

    if (isTimerRunning())
    {
        stopTimer();
    }

    graph = nullptr;

    timeStep = StoredSettings::getInstance()->getProps()
        .getDoubleValue("redrawparam_timestep", 0.6);

    graph = new DirectedGraph();
    ObjectsHelper::makeGraph(objController, *graph.get());

    if(cmdId == CommandIDs::redrawForceDirected)
    {
        graph->setFlowAlgorithm(new ForceDirectedFlowAlgorithm());

        const bool randomizeNodes =
            StoredSettings::getInstance()->getProps()
                .getBoolValue("redrawparam_randomize", true);

        const ContentComp* const cc = objHolder->getContentComp();
        bounds = Rectangle<int>(cc->getViewPosition().x, cc->getViewPosition().y,
                                cc->getViewWidth(), cc->getViewHeight());
        if (randomizeNodes)
        {
            GraphUtils::randomizeNodes(*graph, bounds);
        }
    }

    objController.getUndoManager().beginNewTransaction();

    startTimer(100 * timeStep);
}

void GraphReflowController::stopReflow()
{
    stopTimer();
    graph->setPositions();
    graph = nullptr;
    objHolder = nullptr;
}