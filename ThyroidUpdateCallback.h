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
            if(mUp)
                mCount++;
            else
                mCount--;

            osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *> (node);
            osg::Quat* rot;
            rot->makeRotate(osg::DegreesToRadians(2.0), osg::Z_AXIS);
            osg::Matrix mat1 = osg::Matrix::translate(osg::Vec3(1.f, 1.f, 0.f));
            //osg::Matrix mat2 = osg::Matrix::se



            //pat->setAttitude(osg::Quat(osg::DegreesToRadians(-25.0), osg::Vec3(0,1,0)));
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
};
