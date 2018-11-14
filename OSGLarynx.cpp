// Author: Benjamin Hilton
// Date: November 2018


#include "OSGLarynx.h"
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>


osg::ref_ptr<osg::Node> create_scaled_model(osg::ref_ptr<osg::Node> model)
{
     osg::BoundingSphere bb = model->getBound();
     osg::ref_ptr<osg::PositionAttitudeTransform> scaleTrans = new osg::PositionAttitudeTransform;
     double boundingBoxRadius = bb.radius();
     double radiusRatio{1.0/boundingBoxRadius};
     scaleTrans->setScale(osg::Vec3d(radiusRatio,radiusRatio,radiusRatio));
     scaleTrans->addChild(model);
     return scaleTrans.release();
}

osg::ref_ptr<osg::Node> create_translated_model(osg::ref_ptr<osg::Node> model)
{
    osg::BoundingSphere bb = model->getBound();
    osg ::ref_ptr<osg::PositionAttitudeTransform> positionTrans = new osg::PositionAttitudeTransform;
    osg::Vec3d pos=bb.center();
    pos=osg::Vec3d(pos.x()*-1,pos.y()*-1,pos.z()*-1);
    positionTrans->setPosition(pos);
    positionTrans->addChild(model);
    return positionTrans.release();
}

osg::ref_ptr<osg::Node> load_cartilage_model_file(unsigned int modelNum)
{
    osg::ref_ptr<osg::Node> model = nullptr;
    switch (modelNum)
    {
        case 1: model = osgDB::readNodeFile("/home/benjamin/ME570/final-project-hilton92/thyroid.stl");
            break;
        case 2: model = osgDB::readNodeFile("/home/benjamin/ME570/final-project-hilton92/cricoid.stl");
            break;
        case 3: model = osgDB::readNodeFile("/home/benjamin/ME570/final-project-hilton92/arytenoid.stl");
            break;
    }
    if(model.valid())
     {
         osg::StateSet* stateSet = model->getOrCreateStateSet();
         stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
         stateSet->setMode(GL_RESCALE_NORMAL ,osg::StateAttribute::ON);
     }
    return model.release();
}

osg::ref_ptr<osg::Node> create_cartilage(unsigned int modelNum)
{
    osg::ref_ptr<osg::Node> model = load_cartilage_model_file(modelNum);
    osg::ref_ptr<osg::Node> scaledModel = create_scaled_model(model);
    osg::ref_ptr<osg::Node> translatedModel = create_translated_model(scaledModel);
    return model;
}

