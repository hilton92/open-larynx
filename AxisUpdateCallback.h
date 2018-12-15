// Author: Benjamin Hilton
// Date: November 2018

#include <osg/PositionAttitudeTransform>
#include "osgwidget.h"

class AxisUpdateCallback: public osg::NodeCallback
{
public:
    AxisUpdateCallback(bool &running, float &zLocation, float &xLocation):
        progRunning{running},
        axisZLocation{zLocation},
        axisXLocation{xLocation}
    {

    }
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        if (progRunning)
        {
            osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *> (node);
            osg::Vec3 transPosition(axisXLocation, 0.f, axisZLocation);
            pat->setPosition(transPosition);
        }
    }

protected:
    double mTimeStep{1.0/30.0};
    bool& progRunning;
    float& axisZLocation;
    float& axisXLocation;
};
