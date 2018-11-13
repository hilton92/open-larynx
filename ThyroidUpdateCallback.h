// Author: Benjamin Hilton
// Date: November 2018

#include <osg/PositionAttitudeTransform>

class ThyroidUpdateCallback: public osg::NodeCallback
{
public:
    ThyroidUpdateCallback(bool &running):
        progRunning{running}
    {

    }
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        if (progRunning)
        {
            double newDispX = 1.0;
            double newDispY = 2.0;
            double newDispZ = 3.0;
            osg::Vec3d trans_position(newDispX, newDispY, newDispZ);
            osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *> (node);
            pat->setPosition(trans_position);
            traverse(node, nv);
        }
    }

protected:
    double mTimeStep{1.0/30.0};
    unsigned int objIndex;
    bool& progRunning;
};
