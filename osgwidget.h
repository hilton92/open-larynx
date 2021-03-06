#ifndef OSGWIDGET_H_
#define OSGWIDGET_H_

#include <QOpenGLWidget>
#include "mainwindow.h"

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osgText/Text>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <string>

class OSGWidget : public QOpenGLWidget
{
  Q_OBJECT

public:
  OSGWidget(QWidget* parent = nullptr,
             Qt::WindowFlags f = nullptr);

  virtual ~OSGWidget();
  bool running = false;
  float xLocation = 0.f;
  float zLocation = 0.f;
  float range = 0.f;
  void make_cartilage_transparent(osg::ref_ptr<osg::Node>);
  void make_cartilage_opaque(osg::ref_ptr<osg::Node>);
  osg::ref_ptr<osg::Node> get_thyroid_pointer();
  osg::ref_ptr<osg::Node> get_cricoid_pointer();
  osg::ref_ptr<osg::Node> get_arytenoid_pointer();
  void write_to_file(std::string filename);
  bool thyroidTransparent = false;
  bool cricoidTransparent = false;
  bool arytenoidTransparent = false;
  osg::Vec3 transVector = osg::Vec3(2.f, 2.f, -1.f);
  osg::Quat fullRot;
  std::vector<osg::Vec3> ThyDataVec;
  std::vector<osg::Vec3> AryDataVec;
  void record_data_for_export();
  bool record = false;
  int counter = 0;
  void update_counter_progress_bar();
  void link_window(MainWindow*);

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
  osg::Geometry* create_axis(osg::Vec3, osg::Vec3);
  void repaint_osg_graphics_after_interaction(QEvent* event);
  void initialize_view_and_manipulator(osg::Camera* camera);
  void initialize_mviewer(osgViewer::View *newView, osg::ref_ptr<osgGA::TrackballManipulator> manipulator);
  void timerEvent(QTimerEvent *);
  osg::Node* insert_geom_into_visualization(osg::Geometry* geom, osg::Vec4 color);
  osg::Geometry* create_wireframe_box(float sideLength);
  osg::PositionAttitudeTransform*  create_sphere(float radius, float xVal, float yVal, float zVal);
  osg::Camera* get_new_camera(const int width, const int height, int pixelRatio);
  MainWindow* window;



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
  osg::ref_ptr<osg::Node> Axis;
  osg::ref_ptr<osg::Node> thyroidSphere;
  osg::ref_ptr<osg::Node> ArySphere;
  void create_and_add_cartilage_models();
  void orient_cricoid_and_arytenoid_cartilage();
  void orient_thyroid_cartilage();
  void create_and_add_axis();

};

#endif
