#include "osgwidget.h"

#include <osg/Material>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/EventQueue>
#include <osgViewer/ViewerEventHandlers>
#include <osg/PositionAttitudeTransform>
#include <QPainter>
#include <random>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QPainter>


class SphereUpdateCallback: public osg::NodeCallback
{
public:
    SphereUpdateCallback(unsigned int index, bool &running):
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


OSGWidget::OSGWidget(QWidget *parent, Qt::WindowFlags flags):
    QOpenGLWidget{parent,flags},
    mGraphicsWindow{new osgViewer::GraphicsWindowEmbedded{this->x(),
                                                          this->y(),
                                                          this->width(),
                                                          this->height() }}
  , mViewer{new osgViewer::CompositeViewer}
{
    mRoot = new osg::Group;
    osg::Camera* camera = get_new_camera(this->width(), this->height(), this->devicePixelRatio());
    initialize_view_and_manipulator(camera);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMinimumSize(100, 100);
    this->setMouseTracking(true);
    this->update();
    double framesPerSecond{30};
    double timeStep{1.0/framesPerSecond};
    double timerDurationInMilliSeconds{timeStep * 1000};
    mTimerId = startTimer(timerDurationInMilliSeconds);
    running = false;
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


void OSGWidget::create_geode(osg::ShapeDrawable *sd, unsigned int index, double radius, double mass, double coefRest, double xPos, double yPos, double zPos, double xVel, double yVel, double zVel)
{
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(sd);
    osg::StateSet *stateSet = geode->getOrCreateStateSet();
    osg::Material *material = new osg::Material;
    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSet->setAttributeAndModes(material, osg::StateAttribute::ON);
    stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;
    transform->setPosition(osg::Vec3(xPos, yPos, zPos));
    transform->setUpdateCallback(new SphereUpdateCallback(index, running));
    transform->addChild(geode);
    mRoot->addChild(transform);
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
