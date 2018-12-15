// Author: Benjamin Hilton
// Date: November 2018

#include <osg/PositionAttitudeTransform>
#include "osgwidget.h"

class ThyroidUpdateCallback: public osg::NodeCallback
{
public:
    ThyroidUpdateCallback(bool &running, float &zLocation, float &xLocation):
        progRunning{running},
        axisXLocation{xLocation},
        axisZLocation{zLocation}
    {

    }
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        if (progRunning)
        {
            if(mUp)
                mCount++;
            else
                mCount--;

            osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *> (node);
            osg::Quat xRot, zRot, fullRot;
            xRot.makeRotate(osg::PI_2, osg::X_AXIS);
            zRot.makeRotate(osg::DegreesToRadians(-20.0), osg::Z_AXIS);
            fullRot = xRot * zRot;
            osg::Matrixd mat1 = osg::Matrixd::rotate(fullRot);
            osg::Matrixd mat2 = osg::Matrixd::translate(osg::Vec3(2.f, 2.f, -1.f));
            osg::Matrixd mat3 = osg::Matrixd::translate(osg::Vec3(-axisXLocation, 0.f, -axisZLocation));
            osg::Matrixd mat4 = osg::Matrixd::rotate(0.01 * mCount, osg::Vec3(0.f, 1.f, 0.f));
            osg::Matrixd mat5 = osg::Matrixd::translate(osg::Vec3(axisXLocation, 0.f, axisZLocation));
            osg::Matrix mat7 = mat1 * mat2 * mat3 * mat4 * mat5;
            pat->setAttitude(mat7.getRotate());
            pat->setPosition(mat7.getTrans());
            traverse(node, nv);

            if(mCount == 50 || mCount == 0)
                mUp = !mUp;
        }
    }

protected:
    double mTimeStep{1.0/30.0};
    bool mUp{true};
    unsigned int mCount{0};
    bool& progRunning;
    float& axisXLocation;
    float& axisZLocation;
};
