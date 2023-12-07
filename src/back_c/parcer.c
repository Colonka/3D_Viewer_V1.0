#include "back_c.h"

int parcer(char* filename_obj, obj_info_struct* obj) {
  int error = 0;
  obj->cnt_vertex = 0;
  obj->cnt_facets = 0;
  obj->facets = NULL;
  obj->vertex.coords = NULL;
  FILE* file_ptr;
  file_ptr = fopen(filename_obj, "r");

  if (file_ptr == NULL) {
    error = 1;  //  Ошибка открытия файла
  } else {
    char tmp_str[1024] = {'\0'};
    for (int v_i = 0; !feof(file_ptr) && !ferror(file_ptr) && error == 0;) {
      char first_char = fgetc(file_ptr);
      switch (first_char) {
        case 'v': {
          char second_char = fgetc(file_ptr);
          if (second_char == ' ') {
            fgets(tmp_str, 1024, file_ptr);
            read_v(obj, tmp_str, &v_i, &error);
          }
          break;
        }
        case 'f': {
          char second_char = fgetc(file_ptr);
          if (second_char == ' ') {
            fgets(tmp_str, 1024, file_ptr);
            read_f(obj, tmp_str, &error);
          }
          break;
        }
        case '\n': {
          break;
        }
        default: {
          fgets(tmp_str, 1024, file_ptr);
          break;
        }
      }
    }
    fclose(file_ptr);
    centering(obj);
  }
  if (error) {
    obj_info_free(obj);
  }

  return error;
}

void read_v(obj_info_struct* obj, char* tmp_str, int* v_i, int* error) {
  (obj->cnt_vertex)++;
  obj->vertex.coords = (float*)realloc(obj->vertex.coords,
                                       (obj->cnt_vertex) * 3 * sizeof(float));
  if (obj->vertex.coords == NULL) {
    *error = 2;  //  MEMORY ERROR
  } else {
    int scan_result =
        sscanf(tmp_str, " %f %f %f", &obj->vertex.coords[*v_i],
               &obj->vertex.coords[*v_i + 1], &obj->vertex.coords[*v_i + 2]);
    if (scan_result >= 2) {
      if (scan_result == 2) {
        obj->vertex.coords[*v_i + 2] = 0.0f;  //  для 2d
      }
      check_min_max(obj, *v_i);
      *v_i += 3;
    } else {
      *error = 3;  //  ошибка, нет x или y
    }
  }
}

void read_f(obj_info_struct* obj, char* tmp_str, int* error) {
  char* tmp_strtok = tmp_str;
  char* token = NULL;
  obj->facets = (facet_struct*)realloc(
      obj->facets, (obj->cnt_facets + 1) * sizeof(facet_struct));

  if (obj->facets != NULL) {
    obj->facets[obj->cnt_facets].f_indices = NULL;

    for (int cnt_indices = 0; tmp_strtok != NULL && *error == 0;
         cnt_indices++) {
      token = strtok_r(tmp_strtok, " ", &tmp_strtok);
      if (token != NULL) {
        obj->facets[obj->cnt_facets].f_indices =
            (unsigned int*)realloc(obj->facets[obj->cnt_facets].f_indices,
                                   (cnt_indices + 1) * sizeof(unsigned int));
        if (obj->facets[obj->cnt_facets].f_indices != NULL) {
          int tmp_int = 0;
          if (!sscanf(token, "%d", &tmp_int)) {
            *error = 3;  //  INVALID FILE DATA
          } else {
            if (tmp_int < 0) {  //  для отрицательного отсчета вершин
              if (tmp_int >= -(obj->cnt_vertex)) {
                //  в массиве остчет с 0
                obj->facets[obj->cnt_facets].f_indices[cnt_indices] =
                    (unsigned int)(obj->cnt_vertex + tmp_int);
              } else {
                *error = 3;  //  когда точек меньше
              }
            } else if (tmp_int == 0) {
              cnt_indices--;  //  не считываем
            } else {          //  больше 0:
              if (tmp_int > obj->cnt_vertex) {
                *error = 3;  //  индекс больше, чем всего точек
              } else {
                obj->facets[obj->cnt_facets].f_indices[cnt_indices] =
                    (unsigned int)(tmp_int - 1);  //  в массиве остчет с 0
              }
            }
            obj->facets[obj->cnt_facets].cnt_indices = cnt_indices + 1;
          }
        } else {
          *error = 2;  //  MEMORY ERROR
        }
      }
    }
    obj->cnt_facets += 1;
  } else {
    *error = 2;  //  MEMORY ERROR
  }
}

void check_min_max(obj_info_struct* obj, int v_i) {
  if (v_i > 0) {
    if (obj->vertex.coords[v_i] < obj->vertex.x_min) {
      obj->vertex.x_min = obj->vertex.coords[v_i];
    } else if (obj->vertex.coords[v_i] > obj->vertex.x_max) {
      obj->vertex.x_max = obj->vertex.coords[v_i];
    }

    if (obj->vertex.coords[v_i + 1] < obj->vertex.y_min) {
      obj->vertex.y_min = obj->vertex.coords[v_i + 1];
    } else if (obj->vertex.coords[v_i + 1] > obj->vertex.y_max) {
      obj->vertex.y_max = obj->vertex.coords[v_i + 1];
    }

    if (obj->vertex.coords[v_i + 2] < obj->vertex.z_min) {
      obj->vertex.z_min = obj->vertex.coords[v_i + 2];
    } else if (obj->vertex.coords[v_i + 2] > obj->vertex.z_max) {
      obj->vertex.z_max = obj->vertex.coords[v_i + 2];
    }
  } else if (v_i == 0) {
    obj->vertex.x_min = obj->vertex.coords[v_i];
    obj->vertex.x_max = obj->vertex.x_min;

    obj->vertex.y_min = obj->vertex.coords[v_i + 1];
    obj->vertex.y_max = obj->vertex.y_min;

    obj->vertex.z_min = obj->vertex.coords[v_i + 2];
    obj->vertex.z_max = obj->vertex.z_min;
  }
}
