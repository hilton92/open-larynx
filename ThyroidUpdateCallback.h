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
            //osg::Quat rot;
            //osg::Quat fullRot;
            //rot.makeRotate(0.02 * mCount, 1.368, 0, 3.75);

            osg::Matrixd mat1 = osg::Matrixd::translate(osg::Vec3(1.f, 1.f, -1.f));
            osg::Matrixd mat2 = osg::Matrixd::rotate(0.01 * mCount, osg::Vec3(1.368f, 0.f, 3.75f));
            osg::Matrixd mat3 = osg::Matrixd::translate(osg::Vec3(-1.f, -1.f, 1.f ));

            osg::Matrix mat4 = mat1 * mat2 * mat3;
            pat->setAttitude(mat4.getRotate());
            pat->setPosition(mat4.getTrans());
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
