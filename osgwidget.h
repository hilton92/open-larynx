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
  bool running = false;
  void make_cartilage_transparent(osg::ref_ptr<osg::Node>);
  void make_cartilage_opaque(osg::ref_ptr<osg::Node>);
  osg::ref_ptr<osg::Node> get_thyroid_pointer();
  osg::ref_ptr<osg::Node> get_cricoid_pointer();
  osg::ref_ptr<osg::Node> get_arytenoid_pointer();

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
  void initialize_mviewer(osgViewer::View *newView, osg::ref_ptr<osgGA::TrackballManipulator> manipulator);
  void timerEvent(QTimerEvent *);

  osg::Camera* get_new_camera(const int width, const int height, int pixelRatio);


private:
  virtual void on_resize( int width, int height );
  void set_cartilage_colors();
  osgGA::EventQueue* get_event_queue() const;
  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGraphicsWindow;
  osg::ref_ptr<osgViewer::CompositeViewer> mViewer;
  osg::ref_ptr<osgViewer::View> mView;
  osg::ref_ptr<osg::Group> mRoot;
  int mTimerId{0};
  osg::ref_ptr<osg::Node> Thyroid;
  osg::ref_ptr<osg::Node> Cricoid;
  osg::ref_ptr<osg::Node> Arytenoid;

};

#endif
