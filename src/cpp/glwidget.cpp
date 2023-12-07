#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void GLWidget::send_obj_slot(obj_info_struct *obj_ptr_from_main) {
  this->obj_ptr = obj_ptr_from_main;
  update();
}

void GLWidget::send_settings_slot(settings_struct *settings_ptr_from_main) {
  this->settings_ptr = settings_ptr_from_main;
  update();
}

void GLWidget::initializeGL() {
  this->initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

void GLWidget::paintGL() {
  update();  //  при сбросе настроек
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(settings_ptr->color_background.r,
               settings_ptr->color_background.g,
               settings_ptr->color_background.b, 0.0f);
  glMatrixMode(GL_MODELVIEW);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (obj_ptr != NULL && settings_ptr != NULL) {
    //  выбираем проекцию:
    if (settings_ptr->projection_mode == 1) {
      glFrustum(-0.1, 0.1, -0.1, 0.1, 1.0, 100);
      glTranslatef(0, 0, -10);
    } else if (settings_ptr->projection_mode == 2) {
      glOrtho(-1, 1, -1, 1, -2, 100);
    }
    glVertexPointer(3, GL_FLOAT, 0, obj_ptr->vertex.coords);
    glEnableClientState(GL_VERTEX_ARRAY);
    //  отрисовка точек
    if (settings_ptr->vertex_mode != 0 && settings_ptr->vertex_size != 0) {
      if (settings_ptr->vertex_mode == 1) {
        glEnable(GL_POINT_SMOOTH);  //  точка-круг
      } else {
        glDisable(GL_POINT_SMOOTH);  //  точка-квадрат
      }
      glPointSize(settings_ptr->vertex_size);
      glColor3f(settings_ptr->color_vertex.r, settings_ptr->color_vertex.g,
                settings_ptr->color_vertex.b);
      glDrawArrays(GL_POINTS, 0, obj_ptr->cnt_vertex);
    }
    //  отрисовка линий
    if (settings_ptr->line_width != 0) {  //  если толщина линий 0, не рисуем их
      if (settings_ptr->line_mode == 2) {
        glEnable(GL_LINE_STIPPLE);  //  задаем пунктир
        glLineStipple(1, 0xFF);
      } else {
        glDisable(GL_LINE_STIPPLE);  //  сплошная линия
      }
      glLineWidth(settings_ptr->line_width);
      glColor3f(settings_ptr->color_line.r, settings_ptr->color_line.g,
                settings_ptr->color_line.b);
      for (int i = 0; i < obj_ptr->cnt_facets; i++) {
        glDrawElements(GL_LINE_LOOP, obj_ptr->facets[i].cnt_indices,
                       GL_UNSIGNED_INT, obj_ptr->facets[i].f_indices);
      }
    }
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  glFlush();
}

void GLWidget::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

GLWidget::~GLWidget() {}
