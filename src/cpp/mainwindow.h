#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QMovie>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimer>

#include "./QtGifImage/src/gifimage/qgifimage.h"
#include "glwidget.h"

#define GIF_FPS 10
#define GIF_TIME 5
#define GIF_SIZE 640, 480

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  QSettings *settings_file;
  settings_struct settings;
  obj_info_struct obj;
  position_struct position;
  QVector<QImage> gif_vector;
  QTimer *timer;
  QTimer *timer_seconds;
  int cnt_seconds;
  int cnt_snapshots;
  bool file_was_open;

 private slots:
  void on_pushButton_move_left_clicked();
  void on_pushButton_move_right_clicked();
  void on_pushButton_move_up_clicked();
  void on_pushButton_move_down_clicked();
  void on_pushButton_move_near_clicked();
  void on_pushButton_move_far_clicked();

  void on_pushButton_zoom_up_clicked();
  void on_pushButton_zoom_down_clicked();
  void do_zoom(float scale_now);

  void on_pushButton_rotate_left_x_clicked();
  void on_pushButton_rotate_right_x_clicked();
  void on_pushButton_rotate_left_y_clicked();
  void on_pushButton_rotate_right_y_clicked();
  void on_pushButton_rotate_left_z_clicked();
  void on_pushButton_rotate_right_z_clicked();
  void do_rotate(float angle, char axis);

  void on_pushButton_reset_clicked();
  void on_pushButton_open_file_clicked();
  void on_pushButton_parcer_clicked();
  void start_parcer(QString file_adress);

  void reset_settings();
  void check_window_settings();
  void update_window_settings();
  void save_settings();
  void load_settings();

  void on_radioButton_projection_mode_1_clicked();
  void on_radioButton_projection_mode_2_clicked();
  void on_radioButton_line_mode_1_clicked();
  void on_radioButton_line_mode_2_clicked();
  void on_radioButton_color_background_clicked();
  void on_radioButton_color_vertex_clicked();
  void on_radioButton_color_line_clicked();
  void on_radioButton_vertex_mode_0_clicked();
  void on_radioButton_vertex_mode_1_clicked();
  void on_radioButton_vertex_mode_2_clicked();

  void on_slider_vertex_size_valueChanged(int value);
  void on_slider_line_width_valueChanged(int value);
  void on_slider_color_r_valueChanged(int value);
  void on_slider_color_g_valueChanged(int value);
  void on_slider_color_b_valueChanged(int value);

  void on_pushButton_screenshot_clicked();

  void on_pushButton_gif_clicked();
  void do_snapshot();
  void save_gif();
  void update_timer_seconds();

 signals:
  void send_obj_signal(obj_info_struct *);
  void send_settings_signal(settings_struct *);
};
#endif  // MAINWINDOW_H
