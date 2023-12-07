#include "back_c.h"

void centering(obj_info_struct* obj) {
  if (obj != NULL) {
    float dx = obj->vertex.x_max - obj->vertex.x_min;
    float dy = obj->vertex.y_max - obj->vertex.y_min;
    float dz = obj->vertex.z_max - obj->vertex.z_min;
    float center_x = obj->vertex.x_min + dx / 2.0;
    float center_y = obj->vertex.y_min + dy / 2.0;
    float center_z = obj->vertex.z_min + dz / 2.0;
    float dmax = dx;

    if (dy >= dx && dy >= dz) {
      dmax = dy;
    } else if (dz >= dx && dz >= dy) {
      dmax = dz;
    }

    if (dmax == 0) {
      dmax = 1;  //  деление на 0
    }
    float scale_ratio = START_SCALE_RATIO / dmax;

    for (int i = 0; i < 3 * obj->cnt_vertex; i += 3) {
      obj->vertex.coords[i] -= center_x;
      obj->vertex.coords[i] *= scale_ratio;
      obj->vertex.coords[i + 1] -= center_y;
      obj->vertex.coords[i + 1] *= scale_ratio;
      obj->vertex.coords[i + 2] -= center_z;
      obj->vertex.coords[i + 2] *= scale_ratio;
    }
  }
}

void scale_C(obj_info_struct* obj, float scale_ratio) {
  if (obj != NULL && scale_ratio && scale_ratio != 1) {
    for (int i = 0; i < 3 * obj->cnt_vertex; i += 3) {
      obj->vertex.coords[i] *= scale_ratio;
      obj->vertex.coords[i + 1] *= scale_ratio;
      obj->vertex.coords[i + 2] *= scale_ratio;
    }
  }
}

void move_C(obj_info_struct* obj, float dx, float dy, float dz) {
  if (obj != NULL && (dx || dy || dz)) {
    for (int i = 0; i < 3 * obj->cnt_vertex; i += 3) {
      if (dx) {
        obj->vertex.coords[i] += dx;
      }
      if (dy) {
        obj->vertex.coords[i + 1] += dy;
      }
      if (dz) {
        obj->vertex.coords[i + 2] += dz;
      }
    }
  }
}

void rotate_C(obj_info_struct* obj, float angle, int axis) {
  if (obj != NULL && angle) {
    angle = (angle * PI) / 180.0f;  //  в радианы
    for (int i = 0; i < 3 * obj->cnt_vertex; i += 3) {
      float tmp_x = obj->vertex.coords[i];
      float tmp_y = obj->vertex.coords[i + 1];
      float tmp_z = obj->vertex.coords[i + 2];
      if (axis == 1) {
        obj->vertex.coords[i + 1] = cosf(angle) * tmp_y - sinf(angle) * tmp_z;
        obj->vertex.coords[i + 2] = sinf(angle) * tmp_y + cosf(angle) * tmp_z;
      } else if (axis == 2) {
        obj->vertex.coords[i] = cosf(angle) * tmp_x + sinf(angle) * tmp_z;
        obj->vertex.coords[i + 2] = -sinf(angle) * tmp_x + cosf(angle) * tmp_z;
      } else if (axis == 3) {
        obj->vertex.coords[i] = cosf(angle) * tmp_x + sinf(angle) * tmp_y;
        obj->vertex.coords[i + 1] = -sinf(angle) * tmp_x + cosf(angle) * tmp_y;
      }
    }
  }
}

void obj_info_free(obj_info_struct* obj) {
  obj->cnt_vertex = 0;
  obj->vertex.x_min = 0;
  obj->vertex.x_max = 0;
  obj->vertex.y_min = 0;
  obj->vertex.y_max = 0;
  obj->vertex.z_min = 0;
  obj->vertex.z_max = 0;
  free(obj->vertex.coords);
  obj->vertex.coords = NULL;
  for (int i = 0; i < obj->cnt_facets; i++) {
    free(obj->facets[i].f_indices);
    obj->facets[i].f_indices = NULL;
    obj->facets[i].cnt_indices = 0;
  }
  free(obj->facets);
  obj->facets = NULL;
  obj->cnt_facets = 0;
}
