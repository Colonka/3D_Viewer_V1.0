#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

extern "C" {
#include "back_c/back_c.h"
}

class GLWidget : public QOpenGLWidget, public QOpenGLFunctions {
  Q_OBJECT

 public:
  GLWidget(QWidget* parent = nullptr);
  ~GLWidget();

 public slots:
  void send_obj_slot(obj_info_struct* obj_ptr_from_main);
  void send_settings_slot(settings_struct* settings_ptr_from_main);

 private:
  obj_info_struct* obj_ptr = NULL;
  settings_struct* settings_ptr = NULL;
  void initializeGL() override;
  void resizeGL(int width, int height) override;
  void paintGL() override;
};

#endif  // GLWIDGET_H
