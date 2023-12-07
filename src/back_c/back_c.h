#ifndef SRC_BACK_C_H_
#define SRC_BACK_C_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265358979323846
#define START_SCALE_RATIO 0.7

/**
 * @brief Структура данных вершин объекта
 * @param coords указатель на массив координат вершин ([i] - X, [i+1] - Y,
 * [i+2] - Z)
 * @param x_min величина минимального значения по оси Х
 * @param x_max величина максимального значения по оси X
 * @param y_min величина минимального значения по оси Y
 * @param y_max величина максимального значения по оси Y
 * @param z_min величина минимального значения по оси Z
 * @param z_max величина максимального значения по оси Z
 */
typedef struct vertex_t {
  float* coords;
  float x_min;
  float x_max;
  float y_min;
  float y_max;
  float z_min;
  float z_max;
} vertex_struct;

/**
 * @brief Структура данных граней объекта
 * @param f_indices указатель на массив индексов вершин грани
 * @param cnt_indices количество вершин грани
 */
typedef struct facet_t {
  unsigned int* f_indices;
  unsigned int cnt_indices;
} facet_struct;

/**
 * @brief Структура данных объекта
 * @param cnt_facets количество граней объекта
 * @param cnt_indices количество вершин объекта
 * @param facets указатель на массив структур данных граней объекта
 * @param vertex объект структуры данных вершин объекта
 */
typedef struct obj_info_t {
  int cnt_facets;
  int cnt_vertex;
  facet_struct* facets;
  vertex_struct vertex;
} obj_info_struct;

/**
 * @brief Структура данных для сохранения цвета
 * @param r показатель красного цвета
 * @param g показатель зеленого цвета
 * @param b показатель синего цвета
 */
typedef struct color_t {
  float r;
  float g;
  float b;
} color;

/**
 * @brief Структура данных для настроек
 * @param projection_mode вид проекции
 * @param vertex_mode вид отображения точек
 * @param line_mode вид отображения линий
 * @param line_width толщина линий
 * @param vertex_size размер точек
 * @param color_background цвет фона
 * @param color_vertex цвет точек
 * @param color_line цвет линий
 */
typedef struct settings_t {
  short int projection_mode;
  short int vertex_mode;
  short int line_mode;
  float line_width;
  float vertex_size;
  color color_background;
  color color_vertex;
  color color_line;
} settings_struct;

/**
 * @brief Структура данных для смещений от начального положения
 * @param moveX смещение по оси X
 * @param moveY смещение по оси Y
 * @param moveZ смещение по оси Z
 * @param rotateX угол поворота по оси X
 * @param rotateY угол поворота по оси Y
 * @param rotateZ угол поворота по оси Z
 */
typedef struct position_t {
  float moveX;
  float moveY;
  float moveZ;
  float rotateX;
  float rotateY;
  float rotateZ;
  float scale_ratio;
} position_struct;

/**
 * @brief Функция чтения и парсинга файла
 * @param filename_obj указатель на строку пути до считываемого файла
 * @param obj указатель на структуру данных
 * @param error 1 - ошибка чтения файла, 2 - ошибка памяти, 3 - ошибка в файле
 */
int parcer(char* filename_obj, obj_info_struct* obj);

/**
 * @brief Функция парсинга вершин
 * @param obj указатель на структуру данных
 * @param tmp_str указатель на считываемую строку
 * @param v_i указатель на количество прочитанных символов
 * @param error указатель на ошибку (1 - ошибка открытия файла, 2 - ошибка
 * памяти, 3 - ошибка в файле)
 */
void read_v(obj_info_struct* obj, char* tmp_str, int* v_i, int* error);

/**
 * @brief Функция парсинга полигонов
 * @param obj указатель на структуру данных
 * @param tmp_str указатель на считываемую строку
 * @param error указатель на ошибку (2 - ошибка памяти, 3 - ошибка в файле)
 */
void read_f(obj_info_struct* obj, char* tmp_str, int* error);

/**
 * @brief Функция сравнения минимальных и максимальных значений координат
 * объекта у заданной вершины
 * @param obj указатель на структуру данных
 * @param v_i номер вершины от 0
 */
void check_min_max(obj_info_struct* obj, int v_i);

/**
 * @brief Функция центровки объекта при первой отрисовке
 * @param obj указатель на структуру данных
 */
void centering(obj_info_struct* obj);

/**
 * @brief Функция зумирования объекта
 * @param obj указатель на структуру данных
 * @param scale_ratio величина зумирования (>1 - увеличение, <1 - уменьшение)
 */
void scale_C(obj_info_struct* obj, float scale_ratio);

/**
 * @brief Функция перемещения объекта
 * @param obj указатель на структуру данных
 * @param dx величина перемещения по оси Х
 * @param dy величина перемещения по оси Y
 * @param dz величина перемещения по оси Z
 */
void move_C(obj_info_struct* obj, float dx, float dy, float dz);

/**
 * @brief Функция вращения объекта
 * @param obj указатель на структуру данных
 * @param angle величина вращения(радианы)
 * @param axis ось поворота (1 - Х, 2 - Y, 3 - Z)
 */
void rotate_C(obj_info_struct* obj, float angle, int axis);

/**
 * @brief Функция удаления структуры данных
 * @param obj указатель на структуру данных
 */
void obj_info_free(obj_info_struct* obj);

#endif  //  SRC_BACK_C_H_
