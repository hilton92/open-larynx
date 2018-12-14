#include "osgwidget.h"
#include "ArytenoidUpdateCallback.h"
#include "ThyroidUpdateCallback.h"
#include "CricoidUpdateCallback.h"
#include "AxisUpdateCallback.h"
#include "OSGLarynx.h"

#include <osg/Material>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/EventQueue>
#include <osgViewer/ViewerEventHandlers>
#include <osg/PositionAttitudeTransform>
#include <osg/BlendFunc>
#include <QPainter>
#include <random>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QPainter>


OSGWidget::OSGWidget(QWidget *parent, Qt::WindowFlags flags):
    QOpenGLWidget{parent,flags},
    mGraphicsWindow{new osgViewer::GraphicsWindowEmbedded{this->x(),
                                                          this->y(),
                                                          this->width(),
                                                          this->height()}},
    mViewer{new osgViewer::CompositeViewer}

{
    mRoot = new osg::Group;
    osg::Camera* camera = get_new_camera(this->width(), this->height(), this->devicePixelRatio());
    initialize_view_and_manipulator(camera);
    unsigned int numberRepresentingThyroid = 1;
    unsigned int numberRepresentingCricoid = 2;
    unsigned int numberRepresentingArytenoid = 3;
    Thyroid = create_cartilage(numberRepresentingThyroid);
    Cricoid = create_cartilage(numberRepresentingCricoid);
    Arytenoid = create_cartilage(numberRepresentingArytenoid);
    set_cartilage_colors();
    osg::PositionAttitudeTransform *cricoidTransform = new osg::PositionAttitudeTransform;
    osg::PositionAttitudeTransform *thyroidTransform = new osg::PositionAttitudeTransform;
    osg::PositionAttitudeTransform *arytenoidTransform = new osg::PositionAttitudeTransform;
    Axis = insert_geom_into_visualization(create_axis(osg::Vec3(0.f, 4.f, 0.f), osg::Vec3(0.f, -4.f, 0.f)), osg::Vec4(0.f, 0.7f, 0.7f, 1.f));
    //Axis = insert_geom_into_visualization(create_axis(osg::Vec3(0.f, 4.f, 0.f), osg::Vec3(0.f, -4.f, 0.f)), osg::Vec4(0.f, 0.7f, 0.7f, 1.f));
    osg::PositionAttitudeTransform *axisTransform = new osg::PositionAttitudeTransform;
    axisTransform = new osg::PositionAttitudeTransform;
    axisTransform->addChild(Axis);
    axisTransform->setUpdateCallback(new AxisUpdateCallback(running, zLocation, xLocation));
    //arytenoidTransform->setUpdateCallback(new ArytenoidUpdateCallback(running));
    arytenoidTransform->addChild(Arytenoid);
    thyroidTransform->setUpdateCallback(new ThyroidUpdateCallback(running, zLocation, xLocation));
    thyroidTransform->addChild(Thyroid);
    cricoidTransform->addChild(Cricoid);
    //cricoidTransform->addChild(thyroidTransform);
    mRoot->addChild(thyroidTransform);
    cricoidTransform->addChild(arytenoidTransform);
    //cricoidTransform->addChild(axisTransform);
    mRoot->addChild(cricoidTransform);
    mRoot->addChild(insert_geom_into_visualization(create_wireframe_box(10.f), osg::Vec4(0.f, 0.7f, 0.7f, 1.f)));
    mRoot->addChild(axisTransform);
    osg::Quat xRot, zRot;
    xRot.makeRotate(osg::PI_2, osg::X_AXIS);
    zRot.makeRotate(osg::DegreesToRadians(-20.0), osg::Z_AXIS);
    fullRot = xRot * zRot;
    cricoidTransform->setAttitude(fullRot);
    cricoidTransform->setPosition(osg::Vec3(2.f, 2.f, -1.f));
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMinimumSize(100, 100);
    this->setMouseTracking(true);
    this->update();
    double framesPerSecond{30};
    double timeStep{1.0/framesPerSecond};
    double timerDurationInMilliSeconds{timeStep * 1000};
    mTimerId = startTimer(timerDurationInMilliSeconds);
    running = true;
}

OSGWidget::~OSGWidget()
{
    killTimer(mTimerId);
}

void OSGWidget::timerEvent(QTimerEvent *)
{
    if (running)
    {
        update();
    }
}

void OSGWidget::set_cartilage_colors()
{
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 0.875f, 0.74f, 1.0f));
    Thyroid->getStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    Thyroid->getStateSet()->setAttributeAndModes(mat, osg::StateAttribute::OVERRIDE);
    Cricoid->getStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    Cricoid->getStateSet()->setAttributeAndModes(mat, osg::StateAttribute::OVERRIDE);
    Arytenoid->getStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    Arytenoid->getStateSet()->setAttributeAndModes(mat, osg::StateAttribute::OVERRIDE);
}

osg::Camera* OSGWidget::get_new_camera(const int width, const int height, int pixelRatio)
{
    double aspectRatio = static_cast<double>(width) / static_cast<double>(height);
    osg::Camera *camera = new osg::Camera;
    camera->setGraphicsContext(mGraphicsWindow);
    camera->setViewport(0, 0, width * pixelRatio, height * pixelRatio);
    float redColor{0.f};
    float greenColor{0.f};
    float blueColor{0.5f};
    float opaqueValue{1.f};
    camera->setClearColor(osg::Vec4(redColor, greenColor, blueColor, opaqueValue));
    double fovy{45};
    double zNear{1};
    double zFar{1000};
    camera->setProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    return camera;
}

osg::Geometry*  OSGWidget::create_axis(osg::Vec3 point, osg::Vec3 point2)
{
    osg::Geometry* axis = new osg::Geometry;
    osg::ref_ptr<osg::Vec3Array> points = new osg::Vec3Array;
    points->push_back(point);
    points->push_back(point2);
    osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
    color->push_back(osg::Vec4(1.f, 0.f, 0.f, 1.f));
    axis->setColorArray(color);
    axis->setVertexArray(points);
    axis->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);
    axis->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, 2));
    return axis;
}

osg::Geometry* OSGWidget::create_wireframe_box(float sideLength)
{
    osg::Vec3Array* v = new osg::Vec3Array;
    v->resize(8);
    float lim = sideLength/2.f;
    (*v)[0].set(lim, lim, lim);
    (*v)[1].set(lim, -lim, lim);
    (*v)[2].set(-lim, lim, lim);
    (*v)[3].set(-lim, -lim, lim);
    (*v)[4].set(lim, lim, -lim);
    (*v)[5].set(lim, -lim, -lim);
    (*v)[6].set(-lim, lim, -lim);
    (*v)[7].set(-lim, -lim, -lim);
    osg::Geometry* geom = new osg::Geometry;
    geom->setUseDisplayList(false);
    geom->setVertexArray(v);
    GLushort idxLines[8] = {0, 4, 1, 5, 2, 6, 3, 7};
    GLushort idxLoops[4] = {0, 1, 3, 2};
    GLushort idxLoops2[4] = {4, 5, 7, 6};
    geom->addPrimitiveSet(new osg::DrawElementsUShort( osg::PrimitiveSet::LINES, 8, idxLines));
    geom->addPrimitiveSet(new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops));
    geom->addPrimitiveSet(new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoops2));
    return geom;
}

osg::Node* OSGWidget::insert_geom_into_visualization(osg::Geometry* geom, osg::Vec4 color)
{
    osg::Vec4Array* c = new osg::Vec4Array;
    c->push_back(color);
    geom->setColorArray(c, osg::Array::BIND_OVERALL);
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( geom );
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
    geode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
    transform->addChild(geode);
    return transform;
}

void OSGWidget::initialize_view_and_manipulator(osg::Camera *camera)
{
    osgViewer::View *newView = new osgViewer::View;
    newView->setCamera(camera);
    newView->setSceneData(mRoot.get());
    newView->addEventHandler(new osgViewer::StatsHandler);
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
    manipulator->setAllowThrow(false);
    manipulator->setHomePosition(osg::Vec3d(0.0,-20.0,3.0),osg::Vec3d(0,0,0),osg::Vec3d(0,0,1));
    initialize_mviewer(newView, manipulator);
}

void OSGWidget::initialize_mviewer(osgViewer::View *newView, osg::ref_ptr<osgGA::TrackballManipulator> manipulator)
{
    mView = newView;
    mView->setCameraManipulator(manipulator);
    mViewer->addView(mView);
    mViewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
    mViewer->realize();
    mView->home();
}


void OSGWidget::make_cartilage_transparent(osg::ref_ptr<osg::Node> myNode)
{
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setTransparency(osg::Material::FRONT, 0.6f);
    myNode->getStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    myNode->getStateSet()->setAttributeAndModes(mat, osg::StateAttribute::OVERRIDE);
    osg::StateSet* set = myNode->getOrCreateStateSet();
    set->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);
}

void OSGWidget::make_cartilage_opaque(osg::ref_ptr<osg::Node> myNode)
{
    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0f, 0.875f, 0.74f, 1.0f));
    mat->setTransparency(osg::Material::FRONT, 0.f);
    myNode->getStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    myNode->getStateSet()->setAttributeAndModes(mat, osg::StateAttribute::OVERRIDE);
    osg::StateSet* set = myNode->getOrCreateStateSet();
    set->setRenderingHint(osg::StateSet::OPAQUE_BIN);
    set->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);
}

osg::ref_ptr<osg::Node> OSGWidget::get_thyroid_pointer()
{
    return Thyroid;
}
osg::ref_ptr<osg::Node> OSGWidget::get_cricoid_pointer()
{
    return Cricoid;
}
osg::ref_ptr<osg::Node> OSGWidget::get_arytenoid_pointer()
{
    return Arytenoid;
}

void OSGWidget::paint_event(QPaintEvent*)
{
    this->makeCurrent();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    this->paintGL();
    painter.end();
    this->doneCurrent();
}

void OSGWidget::paintGL()
{
    mViewer->frame();
}

void OSGWidget::resizeGL(int width, int height)
{
    this->get_event_queue()->windowResize(this->x(), this->y(), width, height);
    mGraphicsWindow->resized(this->x(), this->y(), width, height);
    this->on_resize(width, height);
}

void OSGWidget::keyPressEvent(QKeyEvent* event)
{
    QString keyString = event->text();
    const char* keyData = keyString.toLocal8Bit().data();
    if( event->key() == Qt::Key_H )
    {
        mView->home();
        return;
    }
    this->get_event_queue()->keyPress( osgGA::GUIEventAdapter::KeySymbol(*keyData));
}

void OSGWidget::keyReleaseEvent(QKeyEvent* event)
{
    QString keyString   = event->text();
    const char* keyData = keyString.toLocal8Bit().data();
    this->get_event_queue()->keyRelease( osgGA::GUIEventAdapter::KeySymbol(*keyData));
}

void OSGWidget::mouseMoveEvent(QMouseEvent* event)
{
    auto pixelRatio = this->devicePixelRatio();
    this->get_event_queue()->mouseMotion(static_cast<float>(event->x() * pixelRatio),
                                        static_cast<float>(event->y() * pixelRatio));
}

void OSGWidget::mousePressEvent( QMouseEvent* event)
 {
    unsigned int button{0};
     switch( event->button() )
     {
     case Qt::LeftButton:
         button = 1;
         break;
     case Qt::MiddleButton:
         button = 2;
         break;
     case Qt::RightButton:
         button = 3;
         break;
     default:
         break;
     }

     auto pixelRatio = this->devicePixelRatio();
     this->get_event_queue()->mouseButtonPress( static_cast<float>( event->x() * pixelRatio ),
                                              static_cast<float>( event->y() * pixelRatio ),
                                              button );
 }

void OSGWidget::mouseReleaseEvent(QMouseEvent* event)
 {

    unsigned int button{0};
     switch( event->button() )
     {
     case Qt::LeftButton:
         button = 1;
         break;
     case Qt::MiddleButton:
         button = 2;
         break;
     case Qt::RightButton:
         button = 3;
         break;
     default:
         break;
     }
     auto pixelRatio = this->devicePixelRatio();
     this->get_event_queue()->mouseButtonRelease( static_cast<float>( pixelRatio * event->x() ),
                                                static_cast<float>( pixelRatio * event->y() ),
                                                button );
 }


void OSGWidget::wheelEvent( QWheelEvent* event )
{
    event->accept();
    int delta = event->delta();
    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?   osgGA::GUIEventAdapter::SCROLL_UP
                                                                 : osgGA::GUIEventAdapter::SCROLL_DOWN;
    this->get_event_queue()->mouseScroll( motion );
}

void OSGWidget::on_resize(int width, int height)
{
    std::vector<osg::Camera*> cameras;
    mViewer->getCameras(cameras);
    auto pixelRatio = this->devicePixelRatio();
    cameras[0]->setViewport(0, 0, width * pixelRatio, height * pixelRatio);
}

osgGA::EventQueue* OSGWidget::get_event_queue() const
{
    osgGA::EventQueue* eventQueue = mGraphicsWindow->getEventQueue();
    if(eventQueue)
        return eventQueue;
    else
        throw std::runtime_error("Unable to obtain valid event queue");
}

bool OSGWidget::event( QEvent* event )
{
    bool handled = QOpenGLWidget::event( event );
    repaint_osg_graphics_after_interaction(event);
    return handled;
}

void OSGWidget::repaint_osg_graphics_after_interaction(QEvent* event)
{
    if(!running)
    {
        switch(event->type())
        {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        case QEvent::Wheel:
            this->update();
            break;
        default:
            break;
        }
    }
}
