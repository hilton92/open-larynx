// Author: Benjamin Hilton
// Date: November 2018

#include <osg/PositionAttitudeTransform>

class CricoidUpdateCallback: public osg::NodeCallback
{
public:
    CricoidUpdateCallback(bool &running):
        progRunning{running}
    {

    }
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        if (progRunning)
        {
            //perform movement
        }
    }

protected:
    double mTimeStep{1.0/30.0};
    unsigned int objIndex;
    bool& progRunning;
};
