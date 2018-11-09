#ifndef MEEN_570_OSGWIDGET
#define MEEN_570_OSGWIDGET

#include <QOpenGLWidget>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osgText/Text>
#include <osg/Camera>
#include <osg/ShapeDrawable>

class OSGWidget : public QOpenGLWidget
{
  Q_OBJECT

public:
  OSGWidget( QWidget* parent = nullptr,
             Qt::WindowFlags f = nullptr);

  virtual ~OSGWidget();
  double radiusMin = 0.2;
  double radiusMax = 0.6;
  double massMin = 2;
  double massMax = 10;
  double coefRestMin = 0.2;
  double coefRestMax = 1.0;
  double velocityMin = -6;
  double velocityMax = 6;
  unsigned int numSpheres = 10;
  double get_random_number(double lowerLimit, double upperLimit);
  void get_new_values_for_spheres();
  bool running = false;

protected:
  virtual void paint_event( QPaintEvent* paintEvent );
  virtual void paintGL();
  virtual void resizeGL( int width, int height);
  virtual void keyPressEvent( QKeyEvent* event );
  virtual void keyReleaseEvent( QKeyEvent* event );
  virtual void mouseMoveEvent( QMouseEvent* event );
  virtual void mousePressEvent( QMouseEvent* event );
  virtual void mouseReleaseEvent( QMouseEvent* event );
  virtual void wheelEvent( QWheelEvent* event );
  virtual bool event( QEvent* event );

  void repaint_osg_graphics_after_interaction(QEvent* event);
  void initialize_view_and_manipulator(osg::Camera* camera);
  void create_sphere(unsigned int index, double radius, double mass, double coefRest, double xPos, double yPos, double zPos, double xVel, double yVel, double zVel, osg::Vec4 colorV);
  void color_sphere(osg::ShapeDrawable *sd, osg::Vec4 colorVect);
  void create_geode(osg::ShapeDrawable *sd, unsigned int index, double radius, double mass, double coefRest, double xPos, double Pos, double zPos, double xVel, double yVel, double zVel);
  void initialize_mviewer(osgViewer::View *newView, osg::ref_ptr<osgGA::TrackballManipulator> manipulator);
  void timerEvent(QTimerEvent *);
  osg::Camera* get_new_camera(const int width, const int height, int pixelRatio);
  osg::Node* insert_box_into_visualization(osg::Vec4 color, osg::Vec3d scaleFactor);
  osg::Geometry* create_wireframe_box_geometry_centered_at_origin(float sideLength);


private:
  virtual void on_resize( int width, int height );

  osgGA::EventQueue* get_event_queue() const;
  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGraphicsWindow;
  osg::ref_ptr<osgViewer::CompositeViewer> mViewer;
  osg::ref_ptr<osgViewer::View> mView;
  osg::ref_ptr<osg::Group> mRoot;
  int mTimerId{0};

};

#endif
