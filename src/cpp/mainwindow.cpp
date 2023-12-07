#include "mainwindow.h"

#include "ui_mainwindow.h"

extern "C" {
int parcer(char* filename_obj, obj_info_struct* obj);
void move_C(obj_info_struct* obj, float dx, float dy, float dz);
void rotate_C(obj_info_struct* obj, float angle, int axis);
void scale_C(obj_info_struct* obj, float scale_ratio);
void obj_info_free(obj_info_struct* obj);
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setWindowTitle("3DViewer v.1.0");
  file_was_open = false;
  timer = new QTimer(this);
  timer_seconds = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(do_snapshot()));
  connect(timer_seconds, SIGNAL(timeout()), this, SLOT(update_timer_seconds()));
  connect(this, &MainWindow::send_obj_signal, ui->openGLWidget,
          &GLWidget::send_obj_slot);
  connect(this, &MainWindow::send_settings_signal, ui->openGLWidget,
          &GLWidget::send_settings_slot);
  position = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  QString settings_path = QDir::homePath() + "/3d_viewer_settengs.ini";
  settings_file = new QSettings(settings_path, QSettings::IniFormat);
  load_settings();
  emit send_settings_signal(&settings);
  update_window_settings();  //    обновляем переключатели на ui
}

MainWindow::~MainWindow() {
  if (file_was_open == true) {
    obj_info_free(&obj);
  }
  save_settings();
  delete ui;
}

//  кнопка "Обзор"
void MainWindow::on_pushButton_open_file_clicked() {
  QString fileFilter = "OBJ Files (*.obj);;All Files (*)";
  QString fileName =
      QFileDialog::getOpenFileName(this, tr("Load Model File"), "", fileFilter);
  if (!fileName.isEmpty()) {
    QFileInfo fileInfo(fileName);
    QString absoluteFilePath = fileInfo.absoluteFilePath();
    start_parcer(absoluteFilePath);
  }
}

//  Открытие файла по кнопке
void MainWindow::on_pushButton_parcer_clicked() {
  QString filename = ui->filename_input->text();
  start_parcer(filename);
}

//  старт парсера
void MainWindow::start_parcer(QString file_adress) {
  check_window_settings();
  QByteArray ba = file_adress.toLocal8Bit();  // перевод из Qstring в *char
  char* adress = ba.data();
  if (file_was_open == true) {
    obj_info_free(&obj);
  }
  short int error = parcer(adress, &obj);
  if (!error) {
    file_was_open = true;
    emit send_obj_signal(
        &obj);  //  вызываем сигнал для отправки данных в окно графика
    ui->filename_input->setText(file_adress);
    QStringList w = file_adress.split("/");
    ui->label_filename->setText(w.last());
    ui->label_cnt_vertex->setText("Кол-во вершин: " +
                                  QString::number(obj.cnt_vertex));
    ui->label_cnt_facets->setText("Поверхностей: " +
                                  QString::number(obj.cnt_facets));
  } else {
    file_was_open = false;
    ui->label_filename->setText("Имя файла");
    if (error == 1) {
      ui->filename_input->setText("FILE READING ERROR");
    } else if (error == 2) {
      ui->filename_input->setText("MEMORY ERROR");
    } else if (error == 3) {
      ui->filename_input->setText("INVALID FILE DATA");
    }
  }
  //  зануляем смещения:
  position = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
}

//  Начальная инициализация настроек
void MainWindow::reset_settings() {
  settings.color_vertex = {0.0f, 0.0f, 0.0f};
  settings.color_line = {0.0f, 0.0f, 0.0f};
  settings.color_background = {0.5f, 0.5f, 0.5f};
  settings.projection_mode = 1;
  settings.line_mode = 1;
  settings.vertex_mode = 1;
  settings.line_width = 1;
  settings.vertex_size = 3;
  update_window_settings();

  ui->openGLWidget->update();
}

//  Считывание настроек с главного окна
void MainWindow::check_window_settings() {
  if (ui->radioButton_projection_mode_1->isChecked()) {
    settings.projection_mode = 1;
  } else if (ui->radioButton_projection_mode_2->isChecked()) {
    settings.projection_mode = 2;
  }

  if (ui->radioButton_line_mode_1->isChecked()) {
    settings.line_mode = 1;
  } else if (ui->radioButton_line_mode_2->isChecked()) {
    settings.line_mode = 2;
  }

  if (ui->radioButton_vertex_mode_0->isChecked()) {
    settings.vertex_mode = 0;
  } else if (ui->radioButton_vertex_mode_1->isChecked()) {
    settings.vertex_mode = 1;
  } else if (ui->radioButton_vertex_mode_2->isChecked()) {
    settings.vertex_mode = 2;
  }

  if (ui->radioButton_color_background->isChecked()) {
    settings.color_background.r = ui->slider_color_r->value() / 100.0f;
    settings.color_background.g = ui->slider_color_g->value() / 100.0f;
    settings.color_background.b = ui->slider_color_b->value() / 100.0f;
  } else if (ui->radioButton_color_background->isChecked()) {
    settings.color_vertex.r = ui->slider_color_r->value() / 100.0f;
    settings.color_vertex.g = ui->slider_color_g->value() / 100.0f;
    settings.color_vertex.b = ui->slider_color_b->value() / 100.0f;
  }

  settings.line_width = (float)ui->slider_line_width->value();
  settings.vertex_size = (float)ui->slider_vertex_size->value();
}

//  оюновление переключателей на ui
void MainWindow::update_window_settings() {
  if (settings.projection_mode == 1) {
    ui->radioButton_projection_mode_1->setChecked(true);
  } else if (settings.projection_mode == 2) {
    ui->radioButton_projection_mode_2->setChecked(true);
  }

  if (settings.vertex_mode == 0) {
    ui->radioButton_vertex_mode_0->setChecked(true);
  } else if (settings.vertex_mode == 1) {
    ui->radioButton_vertex_mode_1->setChecked(true);
  } else if (settings.vertex_mode == 2) {
    ui->radioButton_vertex_mode_2->setChecked(true);
  }
  ui->slider_vertex_size->setValue((int)settings.vertex_size);

  if (settings.line_mode == 1) {
    ui->radioButton_line_mode_1->setChecked(true);
  } else if (settings.line_mode == 2) {
    ui->radioButton_line_mode_2->setChecked(true);
  }
  ui->slider_line_width->setValue((int)settings.line_width);

  if (ui->radioButton_color_background->isChecked()) {
    ui->slider_color_r->setValue((int)(settings.color_background.r * 100.0f));
    ui->slider_color_g->setValue((int)(settings.color_background.g * 100.0f));
    ui->slider_color_b->setValue((int)(settings.color_background.b * 100.0f));
  } else if (ui->radioButton_color_vertex->isChecked()) {
    ui->slider_color_r->setValue((int)(settings.color_vertex.r * 100.0f));
    ui->slider_color_g->setValue((int)(settings.color_vertex.g * 100.0f));
    ui->slider_color_b->setValue((int)(settings.color_vertex.b * 100.0f));
  } else if (ui->radioButton_color_line->isChecked()) {
    ui->slider_color_r->setValue((int)(settings.color_line.r * 100.0f));
    ui->slider_color_g->setValue((int)(settings.color_line.g * 100.0f));
    ui->slider_color_b->setValue((int)(settings.color_line.b * 100.0f));
  }
  ui->openGLWidget->update();
}

//  сохранине настроек в файл
void MainWindow::save_settings() {
  if (ui->checkBox_save_settings->isChecked()) {  //  если есть галка
    settings_file->setValue("view/projection_mode", settings.projection_mode);
    settings_file->setValue("view/vertex_mode", settings.vertex_mode);
    settings_file->setValue("view/vertex_size", settings.vertex_size);
    settings_file->setValue("view/line_mode", settings.line_mode);
    settings_file->setValue("view/line_width", settings.line_width);

    settings_file->setValue("colors/color_background_r",
                            settings.color_background.r);
    settings_file->setValue("colors/color_background_g",
                            settings.color_background.g);
    settings_file->setValue("colors/color_background_b",
                            settings.color_background.b);

    settings_file->setValue("colors/color_vertex_r", settings.color_vertex.r);
    settings_file->setValue("colors/color_vertex_g", settings.color_vertex.g);
    settings_file->setValue("colors/color_vertex_b", settings.color_vertex.b);

    settings_file->setValue("colors/color_line_r", settings.color_line.r);
    settings_file->setValue("colors/color_line_g", settings.color_line.g);
    settings_file->setValue("colors/color_line_b", settings.color_line.b);
    //      записываем настройки:
    settings_file->sync();
  }
}

//  загрузка настроек из файла или по умолчанию
void MainWindow::load_settings() {
  settings.projection_mode =
      settings_file->value("view/projection_mode", 1).toInt();
  settings.vertex_mode = settings_file->value("view/vertex_mode", 1).toInt();
  settings.vertex_size =
      settings_file->value("view/vertex_size", 1.0f).toFloat();
  settings.line_mode = settings_file->value("view/line_mode", 1).toInt();
  settings.line_width = settings_file->value("view/line_width", 1.0f).toFloat();
  settings.color_background.r =
      settings_file->value("colors/color_background_r", 0.5f).toFloat();
  settings.color_background.g =
      settings_file->value("colors/color_background_g", 0.5f).toFloat();
  settings.color_background.b =
      settings_file->value("colors/color_background_b", 0.5f).toFloat();
  settings.color_vertex.r =
      settings_file->value("colors/color_vertex_r", 0.0f).toFloat();
  settings.color_vertex.g =
      settings_file->value("colors/color_vertex_g", 0.0f).toFloat();
  settings.color_vertex.b =
      settings_file->value("colors/color_vertex_b", 0.0f).toFloat();
  settings.color_line.r =
      settings_file->value("colors/color_line_r", 0.0f).toFloat();
  settings.color_line.g =
      settings_file->value("colors/color_line_g", 0.0f).toFloat();
  settings.color_line.b =
      settings_file->value("colors/color_line_b", 0.0f).toFloat();
}

void MainWindow::on_pushButton_reset_clicked() {
  reset_settings();  //  сброс настроек
  ui->openGLWidget->update();
}

//  Перемещения:
void MainWindow::on_pushButton_move_left_clicked() {
  float dx = (-1.0f) * (float)ui->SpinBox_move->value();
  position.moveX += dx;
  move_C(&obj, dx, 0, 0);
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_move_right_clicked() {
  float dx = (float)ui->SpinBox_move->value();
  position.moveX += dx;
  move_C(&obj, dx, 0, 0);
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_move_up_clicked() {
  float dy = (float)ui->SpinBox_move->value();
  position.moveY += dy;
  move_C(&obj, 0, (float)ui->SpinBox_move->value(), 0);
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_move_down_clicked() {
  float dy = (-1.0f) * (float)ui->SpinBox_move->value();
  position.moveY += dy;
  move_C(&obj, 0, dy, 0);
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_move_near_clicked() {
  float dz = (float)ui->SpinBox_move->value();
  position.moveZ += dz;
  move_C(&obj, 0, 0, dz);
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_move_far_clicked() {
  float dz = (-1.0f) * (float)ui->SpinBox_move->value();
  position.moveZ += dz;
  move_C(&obj, 0, 0, dz);
  ui->openGLWidget->update();
}

//  Увеличение и уменьшение:
void MainWindow::on_pushButton_zoom_up_clicked() {
  if (file_was_open == true) {
    do_zoom(1.0f + ui->SpinBox_zoom->value());
  }
}

void MainWindow::on_pushButton_zoom_down_clicked() {
  if (file_was_open == true) {
    do_zoom(1.0f - ui->SpinBox_zoom->value());
  }
}

void MainWindow::do_zoom(float scale_now) {
  position.scale_ratio *= scale_now;
  move_C(&obj, (-1.0f) * position.moveX, (-1.0f) * position.moveY,
         (-1.0f) * position.moveZ);
  scale_C(&obj, scale_now);
  move_C(&obj, position.moveX, position.moveY, position.moveZ);
  ui->openGLWidget->update();
}

//  Повороты:
void MainWindow::on_pushButton_rotate_right_x_clicked() {
  if (file_was_open == true) {
    do_rotate((-1.0f) * (float)ui->SpinBox_rotate->value(), 'x');
  }
}

void MainWindow::on_pushButton_rotate_left_x_clicked() {
  if (file_was_open == true) {
    do_rotate((float)ui->SpinBox_rotate->value(), 'x');
  }
}

void MainWindow::on_pushButton_rotate_right_y_clicked() {
  if (file_was_open == true) {
    do_rotate((-1.0f) * (float)ui->SpinBox_rotate->value(), 'y');
  }
}

void MainWindow::on_pushButton_rotate_left_y_clicked() {
  if (file_was_open == true) {
    do_rotate((float)ui->SpinBox_rotate->value(), 'y');
  }
}

void MainWindow::on_pushButton_rotate_right_z_clicked() {
  if (file_was_open == true) {
    do_rotate((-1.0f) * (float)ui->SpinBox_rotate->value(), 'z');
  }
}

void MainWindow::on_pushButton_rotate_left_z_clicked() {
  if (file_was_open == true) {
    do_rotate((float)ui->SpinBox_rotate->value(), 'z');
  }
}

void MainWindow::do_rotate(float angle, char axis) {
  move_C(&obj, (-1.0f) * position.moveX, (-1.0f) * position.moveY,
         (-1.0f) * position.moveZ);
  if (axis == 'x') {
    rotate_C(&obj, (-1.0f) * position.rotateY, 2);
    rotate_C(&obj, (-1.0f) * position.rotateZ, 3);
    rotate_C(&obj, angle, 1);
    rotate_C(&obj, position.rotateY, 2);
    rotate_C(&obj, position.rotateZ, 3);
    position.rotateX += angle;
  } else if (axis == 'y') {
    rotate_C(&obj, (-1.0f) * position.rotateX, 1);
    rotate_C(&obj, (-1.0f) * position.rotateZ, 3);
    rotate_C(&obj, angle, 2);
    rotate_C(&obj, position.rotateX, 1);
    rotate_C(&obj, position.rotateZ, 3);
    position.rotateY += angle;
  } else if (axis == 'z') {
    rotate_C(&obj, (-1.0f) * position.rotateX, 1);
    rotate_C(&obj, (-1.0f) * position.rotateY, 2);
    rotate_C(&obj, angle, 3);
    rotate_C(&obj, position.rotateX, 1);
    rotate_C(&obj, position.rotateY, 2);
    position.rotateZ += angle;
  }
  move_C(&obj, position.moveX, position.moveY, position.moveZ);
  ui->openGLWidget->update();
}

//  Настройки отображения элементов:
void MainWindow::on_radioButton_projection_mode_1_clicked() {
  settings.projection_mode = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_projection_mode_2_clicked() {
  settings.projection_mode = 2;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_vertex_mode_0_clicked() {
  settings.vertex_mode = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_vertex_mode_1_clicked() {
  settings.vertex_mode = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_vertex_mode_2_clicked() {
  settings.vertex_mode = 2;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_line_mode_1_clicked() {
  settings.line_mode = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_line_mode_2_clicked() {
  settings.line_mode = 2;
  ui->openGLWidget->update();
}

//  Ползунки:
void MainWindow::on_slider_vertex_size_valueChanged(int value) {
  settings.vertex_size = (float)value;
  if (settings.vertex_mode != 0) {
    ui->openGLWidget->update();
  }
}

void MainWindow::on_slider_line_width_valueChanged(int value) {
  settings.line_width = (float)value;
  ui->openGLWidget->update();
}

void MainWindow::on_slider_color_r_valueChanged(int value) {
  if (ui->radioButton_color_background->isChecked()) {
    settings.color_background.r = value / 100.0f;
  } else if (ui->radioButton_color_vertex->isChecked()) {
    settings.color_vertex.r = value / 100.0f;
  } else if (ui->radioButton_color_line->isChecked()) {
    settings.color_line.r = value / 100.0f;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_slider_color_g_valueChanged(int value) {
  if (ui->radioButton_color_background->isChecked()) {
    settings.color_background.g = value / 100.0f;
  } else if (ui->radioButton_color_vertex->isChecked()) {
    settings.color_vertex.g = value / 100.0f;
  } else if (ui->radioButton_color_line->isChecked()) {
    settings.color_line.g = value / 100.0f;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_slider_color_b_valueChanged(int value) {
  if (ui->radioButton_color_background->isChecked()) {
    settings.color_background.b = value / 100.0f;
  } else if (ui->radioButton_color_vertex->isChecked()) {
    settings.color_vertex.b = value / 100.0f;
  } else if (ui->radioButton_color_line->isChecked()) {
    settings.color_line.b = value / 100.0f;
  }
  ui->openGLWidget->update();
}

//  Переключатель настройка цветов
void MainWindow::on_radioButton_color_background_clicked() {
  ui->slider_color_r->setValue(settings.color_background.r * 100);
  ui->slider_color_g->setValue(settings.color_background.g * 100);
  ui->slider_color_b->setValue(settings.color_background.b * 100);
}

void MainWindow::on_radioButton_color_vertex_clicked() {
  ui->slider_color_r->setValue(settings.color_vertex.r * 100);
  ui->slider_color_g->setValue(settings.color_vertex.g * 100);
  ui->slider_color_b->setValue(settings.color_vertex.b * 100);
}

void MainWindow::on_radioButton_color_line_clicked() {
  ui->slider_color_r->setValue(settings.color_line.r * 100);
  ui->slider_color_g->setValue(settings.color_line.g * 100);
  ui->slider_color_b->setValue(settings.color_line.b * 100);
}

//  Скриншот
void MainWindow::on_pushButton_screenshot_clicked() {
  QString screenshot_path = NULL;

  if (ui->radioButton_jpeg->isChecked()) {
    screenshot_path = QFileDialog::getSaveFileName(this, "Save screenshot jpeg",
                                                   "../", "JPEG (*.jpeg)");
  } else {
    screenshot_path = QFileDialog::getSaveFileName(this, "Save screenshot bmp",
                                                   "../", "BMP (*.bmp)");
  }
  if (!screenshot_path.isNull()) {
    ui->openGLWidget->grabFramebuffer().save(screenshot_path, NULL, -1);
  }
}

void MainWindow::on_pushButton_gif_clicked() {
  cnt_snapshots = 0;
  cnt_seconds = 0;
  timer->start(1000 / GIF_FPS);  //  задаем частоту кадров
  timer_seconds->start(1000);
  ui->pushButton_gif->setDisabled(true);
  ui->label_gif_progress->setText("Идет запись...");
}

void MainWindow::update_timer_seconds() {
  int seconds = GIF_TIME - cnt_seconds;
  if (seconds != 0) {
    ui->label_gif_time->setText(QString::number(seconds));
    cnt_seconds++;
  }
}

void MainWindow::do_snapshot() {
  if (cnt_snapshots < GIF_TIME * GIF_FPS) {
    //  сохраняем снимок в вектор:
    gif_vector.push_back(ui->openGLWidget->grabFramebuffer());
    cnt_snapshots++;
  } else {
    timer_seconds->stop();
    timer->stop();
    save_gif();
    ui->label_gif_progress->setText("");
    cnt_snapshots = 0;
  }
}

void MainWindow::save_gif() {
  ui->label_gif_time->setText("");
  QString gif_path = QFileDialog::getSaveFileName(this, tr("Save GIF"), NULL,
                                                  tr("GIF files (*.gif)"));
  if (!gif_path.isNull()) {
    QGifImage gifka(QSize(GIF_SIZE));
    gifka.setDefaultDelay(1000 / GIF_FPS);  // выставляем задержку

    QVector<QImage>::Iterator snapshots = gif_vector.begin();
    while (snapshots != gif_vector.end()) {
      *snapshots = snapshots->scaled(GIF_SIZE, Qt::IgnoreAspectRatio);
      gifka.addFrame(*snapshots);  //  добавляем кадр
      snapshots++;
    }
    gifka.save(gif_path);
  }
  gif_vector.clear();  //  очищает вектор
  //  релоцирует память на 0 элементов
  gif_vector.squeeze();
  ui->pushButton_gif->setEnabled(true);
}
