#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../back_c/back_c.h"

// PARCING

START_TEST(parc_true) {
  obj_info_struct res_true = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};

  res_true.vertex.coords = (float *)calloc(sizeof(float), 12);
  if (res_true.vertex.coords != NULL) {
    res_true.vertex.coords[0] = -0.55;
    res_true.vertex.coords[1] = 0.67;
    res_true.vertex.coords[2] = 0.15;
    res_true.vertex.coords[3] = 0.53;
    res_true.vertex.coords[4] = 0.5;
    res_true.vertex.coords[5] = 0.43;
    res_true.vertex.coords[6] = 0.93;
    res_true.vertex.coords[7] = -0.57;
    res_true.vertex.coords[8] = 0.78;
    res_true.vertex.coords[9] = -0.45;
    res_true.vertex.coords[10] = -0.35;
    res_true.vertex.coords[11] = 0.11;

    res_true.cnt_vertex = 4;

    res_true.vertex.x_max = 0.93;
    res_true.vertex.x_min = -0.55;
    res_true.vertex.y_max = 0.67;
    res_true.vertex.y_min = -0.57;
    res_true.vertex.z_max = 0.78;
    res_true.vertex.z_min = 0.11;

    float xc = -0.55 + (1.48 / 2.0);
    float yc = -0.57 + (1.24 / 2.0);
    float zc = 0.11 + (0.67 / 2.0);

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] -= xc;
      res_true.vertex.coords[i] *= START_SCALE_RATIO / 1.48;
      res_true.vertex.coords[i + 1] -= yc;
      res_true.vertex.coords[i + 1] *= START_SCALE_RATIO / 1.48;
      res_true.vertex.coords[i + 2] -= zc;
      res_true.vertex.coords[i + 2] *= START_SCALE_RATIO / 1.48;
    }

    res_true.facets = (facet_struct *)calloc(sizeof(facet_struct), 2);
    if (res_true.facets != NULL) {
      res_true.cnt_facets = 2;

      res_true.facets[0].f_indices =
          (unsigned int *)calloc(sizeof(unsigned int), 3);
      if (res_true.facets[0].f_indices != NULL) {
        res_true.facets[0].f_indices[0] = 0;
        res_true.facets[0].f_indices[1] = 1;
        res_true.facets[0].f_indices[2] = 2;
        res_true.facets[0].cnt_indices = 3;

        res_true.facets[1].f_indices =
            (unsigned int *)calloc(sizeof(unsigned int), 4);
        if (res_true.facets[1].f_indices != NULL) {
          res_true.facets[1].f_indices[0] = 1;
          res_true.facets[1].f_indices[1] = 2;
          res_true.facets[1].f_indices[2] = 0;
          res_true.facets[1].f_indices[3] = 3;
          res_true.facets[1].cnt_indices = 4;

          char filename_obj[] = "tests/test_parcer.txt";

          parcer(filename_obj, &res);

          ck_assert_double_eq(res_true.cnt_facets, res.cnt_facets);
          ck_assert_double_eq(res_true.cnt_vertex, res.cnt_vertex);

          for (int i = 0; i < 2; i++) {
            for (int j = 0; j < res_true.facets[i].cnt_indices; j++) {
              ck_assert_double_eq_tol(res_true.facets[i].f_indices[j],
                                      res.facets[i].f_indices[j], 1e-6);
            }
            ck_assert_double_eq(res_true.facets[i].cnt_indices,
                                res.facets[i].cnt_indices);
          }
          for (int i = 0; i < 12; i++) {
            ck_assert_double_eq_tol(res_true.vertex.coords[i],
                                    res.vertex.coords[i], 1e-6);
          }
        }
      }
    }
  }

  ck_assert_double_eq_tol(res_true.vertex.x_max, res.vertex.x_max, 1e-6);
  ck_assert_double_eq_tol(res_true.vertex.x_min, res.vertex.x_min, 1e-6);
  ck_assert_double_eq_tol(res_true.vertex.y_max, res.vertex.y_max, 1e-6);
  ck_assert_double_eq_tol(res_true.vertex.y_min, res.vertex.y_min, 1e-6);
  ck_assert_double_eq_tol(res_true.vertex.z_max, res.vertex.z_max, 1e-6);
  ck_assert_double_eq_tol(res_true.vertex.z_min, res.vertex.z_min, 1e-6);

  obj_info_free(&res_true);
  obj_info_free(&res);
}
END_TEST

//

START_TEST(parc_fail_open) {
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  int true_res = 1;
  int res_fail = 0;
  char filename_obj[100] = "tests/teest_parcer.txt";
  res_fail = parcer(filename_obj, &res);
  ck_assert_double_eq(true_res, res_fail);
  obj_info_free(&res);
}
END_TEST

//

START_TEST(parc_fail_file) {
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  int true_res = 3;
  int res_fail = 0;
  char filename_obj[100] = "tests/unable_data.txt";
  res_fail = parcer(filename_obj, &res);
  ck_assert_double_eq(true_res, res_fail);
  obj_info_free(&res);
}
END_TEST

//

START_TEST(parc_centering) {
  obj_info_struct res_true = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};

  res_true.vertex.coords = (float *)calloc(sizeof(float), 12);
  if (res_true.vertex.coords != NULL) {
    res_true.vertex.coords[0] = -1.55;
    res_true.vertex.coords[1] = 0.67;
    res_true.vertex.coords[2] = 1.15;
    res_true.vertex.coords[3] = 0.53;
    res_true.vertex.coords[4] = 0.5;
    res_true.vertex.coords[5] = 0.43;
    res_true.vertex.coords[6] = 1.93;
    res_true.vertex.coords[7] = -0.57;
    res_true.vertex.coords[8] = 0.78;
    res_true.vertex.coords[9] = -0.45;
    res_true.vertex.coords[10] = -0.35;
    res_true.vertex.coords[11] = 2.11;

    res_true.cnt_vertex = 4;

    float xc = -1.55 + (3.48 / 2.0);
    float yc = -0.57 + (1.24 / 2.0);
    float zc = 0.43 + (1.68 / 2.0);
    float scale = START_SCALE_RATIO / 3.48;

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] -= xc;
      res_true.vertex.coords[i] *= scale;
      res_true.vertex.coords[i + 1] -= yc;
      res_true.vertex.coords[i + 1] *= scale;
      res_true.vertex.coords[i + 2] -= zc;
      res_true.vertex.coords[i + 2] *= scale;
    }

    char filename_obj[] = "tests/test_parcer_centering.txt";
    parcer(filename_obj, &res);

    for (int i = 0; i < 12; i++) {
      ck_assert_double_eq_tol(res_true.vertex.coords[i], res.vertex.coords[i],
                              1e-3);
    }
  }

  obj_info_free(&res_true);
  obj_info_free(&res);
}
END_TEST

// MOVE

START_TEST(move) {
  obj_info_struct true_res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  res.facets = NULL;
  true_res.facets = NULL;

  true_res.vertex.coords = (float *)calloc(sizeof(float), 6);
  if (true_res.vertex.coords != NULL) {
    true_res.vertex.coords[0] = 0.63;
    true_res.vertex.coords[1] = 0.6;
    true_res.vertex.coords[2] = 0.12;
    true_res.vertex.coords[3] = 0.415;
    true_res.vertex.coords[4] = 0.23;
    true_res.vertex.coords[5] = -0.95;
    true_res.cnt_vertex = 2;

    res.vertex.coords = (float *)calloc(sizeof(float), 6);
    if (res.vertex.coords != NULL) {
      res.vertex.coords[0] = 0.53;
      res.vertex.coords[1] = 0.23;
      res.vertex.coords[2] = 0.64;
      res.vertex.coords[3] = 0.315;
      res.vertex.coords[4] = -0.14;
      res.vertex.coords[5] = -0.43;
      res.cnt_vertex = 2;

      move_C(&res, 0.1, 0.37, -0.52);
      for (int i = 0; res.vertex.coords[i] != 0; i++) {
        ck_assert_double_eq_tol(res.vertex.coords[i], true_res.vertex.coords[i],
                                1e-6);
      }
    }
  }

  obj_info_free(&true_res);
  obj_info_free(&res);
}
END_TEST

// ROTATE

START_TEST(test_rotate_x_plus) {
  obj_info_struct res_true = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};

  res_true.vertex.coords = (float *)calloc(sizeof(float), 12);
  if (res_true.vertex.coords != NULL) {
    res_true.vertex.coords[0] = 1.55;
    res_true.vertex.coords[1] = 0.67;
    res_true.vertex.coords[2] = 1.15;
    res_true.vertex.coords[3] = 0.53;
    res_true.vertex.coords[4] = 0.5;
    res_true.vertex.coords[5] = 0.43;
    res_true.vertex.coords[6] = 1.93;
    res_true.vertex.coords[7] = 0.57;
    res_true.vertex.coords[8] = 0.78;
    res_true.vertex.coords[9] = 0.45;
    res_true.vertex.coords[10] = 0.35;
    res_true.vertex.coords[11] = 2.11;
    res_true.cnt_vertex = 4;

    float xc = 0.45 + (1.48 / 2.0);
    float yc = 0.35 + (0.32 / 2.0);
    float zc = 0.43 + (1.68 / 2.0);
    float scale = START_SCALE_RATIO / 1.68;

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] -= xc;
      res_true.vertex.coords[i] *= scale;
      res_true.vertex.coords[i + 1] -= yc;
      res_true.vertex.coords[i + 1] *= scale;
      res_true.vertex.coords[i + 2] -= zc;
      res_true.vertex.coords[i + 2] *= scale;
    }

    char filename_obj[] = "tests/test_rorate.txt";
    parcer(filename_obj, &res);

    float angle = 0.15;
    angle = (angle * PI) / 180.0f;
    rotate_C(&res, angle, 1);

    for (int i = 0; i < 12; i += 3) {
      float tmp_y = res_true.vertex.coords[i + 1];
      float tmp_z = res_true.vertex.coords[i + 2];
      res_true.vertex.coords[i + 1] = cos(angle) * tmp_y - sin(angle) * tmp_z;
      res_true.vertex.coords[i + 2] = sin(angle) * tmp_y + cos(angle) * tmp_z;
    }
    for (int i = 0; i < 12; i++) {
      ck_assert_double_eq_tol(res_true.vertex.coords[i], res.vertex.coords[i],
                              1e-3);
    }
  }

  obj_info_free(&res_true);
  obj_info_free(&res);
}
END_TEST

//

START_TEST(test_rotate_x_minus) {
  obj_info_struct res_true = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};

  res_true.vertex.coords = (float *)calloc(sizeof(float), 12);
  if (res_true.vertex.coords != NULL) {
    res_true.vertex.coords[0] = 1.55;
    res_true.vertex.coords[1] = 0.67;
    res_true.vertex.coords[2] = 1.15;
    res_true.vertex.coords[3] = 0.53;
    res_true.vertex.coords[4] = 0.5;
    res_true.vertex.coords[5] = 0.43;
    res_true.vertex.coords[6] = 1.93;
    res_true.vertex.coords[7] = 0.57;
    res_true.vertex.coords[8] = 0.78;
    res_true.vertex.coords[9] = 0.45;
    res_true.vertex.coords[10] = 0.35;
    res_true.vertex.coords[11] = 2.11;
    res_true.cnt_vertex = 4;

    float xc = 0.45 + (1.48 / 2.0);
    float yc = 0.35 + (0.32 / 2.0);
    float zc = 0.43 + (1.68 / 2.0);
    float scale = START_SCALE_RATIO / 1.68;

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] -= xc;
      res_true.vertex.coords[i] *= scale;
      res_true.vertex.coords[i + 1] -= yc;
      res_true.vertex.coords[i + 1] *= scale;
      res_true.vertex.coords[i + 2] -= zc;
      res_true.vertex.coords[i + 2] *= scale;
    }

    char filename_obj[] = "tests/test_rorate.txt";
    parcer(filename_obj, &res);

    float angle = -0.15;
    angle = (angle * PI) / 180.0f;
    rotate_C(&res, angle, 1);

    for (int i = 0; i < 12; i += 3) {
      float tmp_y = res_true.vertex.coords[i + 1];
      float tmp_z = res_true.vertex.coords[i + 2];
      res_true.vertex.coords[i + 1] = cos(angle) * tmp_y - sin(angle) * tmp_z;
      res_true.vertex.coords[i + 2] = sin(angle) * tmp_y + cos(angle) * tmp_z;
    }
    for (int i = 0; i < 12; i++) {
      ck_assert_double_eq_tol(res_true.vertex.coords[i], res.vertex.coords[i],
                              1e-3);
    }
  }

  obj_info_free(&res_true);
  obj_info_free(&res);
}
END_TEST

//

START_TEST(test_rotate_y_plus) {
  obj_info_struct res_true = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};

  res_true.vertex.coords = (float *)calloc(sizeof(float), 12);
  if (res_true.vertex.coords != NULL) {
    res_true.vertex.coords[0] = 1.55;
    res_true.vertex.coords[1] = 0.67;
    res_true.vertex.coords[2] = 1.15;
    res_true.vertex.coords[3] = 0.53;
    res_true.vertex.coords[4] = 0.5;
    res_true.vertex.coords[5] = 0.43;
    res_true.vertex.coords[6] = 1.93;
    res_true.vertex.coords[7] = 0.57;
    res_true.vertex.coords[8] = 0.78;
    res_true.vertex.coords[9] = 0.45;
    res_true.vertex.coords[10] = 0.35;
    res_true.vertex.coords[11] = 2.11;
    res_true.cnt_vertex = 4;

    float xc = 0.45 + (1.48 / 2.0);
    float yc = 0.35 + (0.32 / 2.0);
    float zc = 0.43 + (1.68 / 2.0);
    float scale = START_SCALE_RATIO / 1.68;

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] -= xc;
      res_true.vertex.coords[i] *= scale;
      res_true.vertex.coords[i + 1] -= yc;
      res_true.vertex.coords[i + 1] *= scale;
      res_true.vertex.coords[i + 2] -= zc;
      res_true.vertex.coords[i + 2] *= scale;
    }

    char filename_obj[] = "tests/test_rorate.txt";
    parcer(filename_obj, &res);

    float angle = 0.15;
    angle = (angle * PI) / 180.0f;
    rotate_C(&res, angle, 2);

    for (int i = 0; i < 12; i += 3) {
      float tmp_x = res_true.vertex.coords[i];
      float tmp_z = res_true.vertex.coords[i + 2];
      res_true.vertex.coords[i] = cos(angle) * tmp_x + sin(angle) * tmp_z;
      res_true.vertex.coords[i + 2] = -sin(angle) * tmp_x + cos(angle) * tmp_z;
    }

    for (int i = 0; i < 12; i++) {
      ck_assert_double_eq_tol(res_true.vertex.coords[i], res.vertex.coords[i],
                              1e-3);
    }
  }

  obj_info_free(&res_true);
  obj_info_free(&res);
}
END_TEST

//

START_TEST(test_rotate_y_minus) {
  obj_info_struct res_true = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};

  res_true.vertex.coords = (float *)calloc(sizeof(float), 12);
  if (res_true.vertex.coords != NULL) {
    res_true.vertex.coords[0] = 1.55;
    res_true.vertex.coords[1] = 0.67;
    res_true.vertex.coords[2] = 1.15;
    res_true.vertex.coords[3] = 0.53;
    res_true.vertex.coords[4] = 0.5;
    res_true.vertex.coords[5] = 0.43;
    res_true.vertex.coords[6] = 1.93;
    res_true.vertex.coords[7] = 0.57;
    res_true.vertex.coords[8] = 0.78;
    res_true.vertex.coords[9] = 0.45;
    res_true.vertex.coords[10] = 0.35;
    res_true.vertex.coords[11] = 2.11;
    res_true.cnt_vertex = 4;

    float xc = 0.45 + (1.48 / 2.0);
    float yc = 0.35 + (0.32 / 2.0);
    float zc = 0.43 + (1.68 / 2.0);
    float scale = START_SCALE_RATIO / 1.68;

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] -= xc;
      res_true.vertex.coords[i] *= scale;
      res_true.vertex.coords[i + 1] -= yc;
      res_true.vertex.coords[i + 1] *= scale;
      res_true.vertex.coords[i + 2] -= zc;
      res_true.vertex.coords[i + 2] *= scale;
    }

    char filename_obj[] = "tests/test_rorate.txt";
    parcer(filename_obj, &res);

    float angle = -0.15;
    angle = (angle * PI) / 180.0f;
    rotate_C(&res, angle, 2);

    for (int i = 0; i < 12; i += 3) {
      float tmp_x = res_true.vertex.coords[i];
      float tmp_z = res_true.vertex.coords[i + 2];
      res_true.vertex.coords[i] = cos(angle) * tmp_x + sin(angle) * tmp_z;
      res_true.vertex.coords[i + 2] = -sin(angle) * tmp_x + cos(angle) * tmp_z;
    }
    for (int i = 0; i < 12; i++) {
      ck_assert_double_eq_tol(res_true.vertex.coords[i], res.vertex.coords[i],
                              1e-3);
    }
  }

  obj_info_free(&res_true);
  obj_info_free(&res);
}
END_TEST

//

START_TEST(test_rotate_z_plus) {
  obj_info_struct res_true = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};

  res_true.vertex.coords = (float *)calloc(sizeof(float), 12);
  if (res_true.vertex.coords != NULL) {
    res_true.vertex.coords[0] = 1.55;
    res_true.vertex.coords[1] = 0.67;
    res_true.vertex.coords[2] = 1.15;
    res_true.vertex.coords[3] = 0.53;
    res_true.vertex.coords[4] = 0.5;
    res_true.vertex.coords[5] = 0.43;
    res_true.vertex.coords[6] = 1.93;
    res_true.vertex.coords[7] = 0.57;
    res_true.vertex.coords[8] = 0.78;
    res_true.vertex.coords[9] = 0.45;
    res_true.vertex.coords[10] = 0.35;
    res_true.vertex.coords[11] = 2.11;
    res_true.cnt_vertex = 4;

    float xc = 0.45 + (1.48 / 2.0);
    float yc = 0.35 + (0.32 / 2.0);
    float zc = 0.43 + (1.68 / 2.0);
    float scale = START_SCALE_RATIO / 1.68;

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] -= xc;
      res_true.vertex.coords[i] *= scale;
      res_true.vertex.coords[i + 1] -= yc;
      res_true.vertex.coords[i + 1] *= scale;
      res_true.vertex.coords[i + 2] -= zc;
      res_true.vertex.coords[i + 2] *= scale;
    }

    char filename_obj[] = "tests/test_rorate.txt";
    parcer(filename_obj, &res);

    float angle = 0.15;
    angle = (angle * PI) / 180.0f;
    rotate_C(&res, angle, 3);

    for (int i = 0; i < 12; i += 3) {
      float tmp_x = res_true.vertex.coords[i];
      float tmp_y = res_true.vertex.coords[i + 1];
      res_true.vertex.coords[i] = cos(angle) * tmp_x + sin(angle) * tmp_y;
      res_true.vertex.coords[i + 1] = -sin(angle) * tmp_x + cos(angle) * tmp_y;
    }
    for (int i = 0; i < 12; i++) {
      ck_assert_double_eq_tol(res_true.vertex.coords[i], res.vertex.coords[i],
                              1e-3);
    }
  }

  obj_info_free(&res_true);
  obj_info_free(&res);
}
END_TEST

//

START_TEST(test_rotate_z_minus) {
  obj_info_struct res_true = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};

  res_true.vertex.coords = (float *)calloc(sizeof(float), 12);
  if (res_true.vertex.coords != NULL) {
    res_true.vertex.coords[0] = 1.55;
    res_true.vertex.coords[1] = 0.67;
    res_true.vertex.coords[2] = 1.15;
    res_true.vertex.coords[3] = 0.53;
    res_true.vertex.coords[4] = 0.5;
    res_true.vertex.coords[5] = 0.43;
    res_true.vertex.coords[6] = 1.93;
    res_true.vertex.coords[7] = 0.57;
    res_true.vertex.coords[8] = 0.78;
    res_true.vertex.coords[9] = 0.45;
    res_true.vertex.coords[10] = 0.35;
    res_true.vertex.coords[11] = 2.11;
    res_true.cnt_vertex = 4;

    float xc = 0.45 + (1.48 / 2.0);
    float yc = 0.35 + (0.32 / 2.0);
    float zc = 0.43 + (1.68 / 2.0);
    float scale = START_SCALE_RATIO / 1.68;

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] -= xc;
      res_true.vertex.coords[i] *= scale;
      res_true.vertex.coords[i + 1] -= yc;
      res_true.vertex.coords[i + 1] *= scale;
      res_true.vertex.coords[i + 2] -= zc;
      res_true.vertex.coords[i + 2] *= scale;
    }

    char filename_obj[] = "tests/test_rorate.txt";
    parcer(filename_obj, &res);

    float angle = -0.15;
    angle = (angle * PI) / 180.0f;
    rotate_C(&res, angle, 3);

    for (int i = 0; i < 12; i += 3) {
      float tmp_x = res_true.vertex.coords[i];
      float tmp_y = res_true.vertex.coords[i + 1];
      res_true.vertex.coords[i] = cos(angle) * tmp_x + sin(angle) * tmp_y;
      res_true.vertex.coords[i + 1] = -sin(angle) * tmp_x + cos(angle) * tmp_y;
    }
    for (int i = 0; i < 12; i++) {
      ck_assert_double_eq_tol(res_true.vertex.coords[i], res.vertex.coords[i],
                              1e-3);
    }
  }

  obj_info_free(&res_true);
  obj_info_free(&res);
}
END_TEST

// ZOOM

START_TEST(test_zoom) {
  obj_info_struct res_true = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};
  obj_info_struct res = {0, 0, NULL, {NULL, 0, 0, 0, 0, 0, 0}};

  res_true.vertex.coords = (float *)calloc(sizeof(float), 12);
  if (res_true.vertex.coords != NULL) {
    res_true.vertex.coords[0] = 1.55;
    res_true.vertex.coords[1] = 0.67;
    res_true.vertex.coords[2] = 1.15;
    res_true.vertex.coords[3] = 0.53;
    res_true.vertex.coords[4] = 0.5;
    res_true.vertex.coords[5] = 0.43;
    res_true.vertex.coords[6] = 1.93;
    res_true.vertex.coords[7] = 0.57;
    res_true.vertex.coords[8] = 0.78;
    res_true.vertex.coords[9] = 0.45;
    res_true.vertex.coords[10] = 0.35;
    res_true.vertex.coords[11] = 2.11;
    res_true.cnt_vertex = 4;

    float xc = 0.45 + (1.48 / 2.0);
    float yc = 0.35 + (0.32 / 2.0);
    float zc = 0.43 + (1.68 / 2.0);
    float scale = START_SCALE_RATIO / 1.68;

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] -= xc;
      res_true.vertex.coords[i] *= scale;
      res_true.vertex.coords[i + 1] -= yc;
      res_true.vertex.coords[i + 1] *= scale;
      res_true.vertex.coords[i + 2] -= zc;
      res_true.vertex.coords[i + 2] *= scale;
    }

    char filename_obj[] = "tests/test_rorate.txt";
    parcer(filename_obj, &res);

    float zoom = 1.15;
    scale_C(&res, zoom);

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] *= zoom;
      res_true.vertex.coords[i + 1] *= zoom;
      res_true.vertex.coords[i + 2] *= zoom;
    }

    for (int i = 0; i < 12; i++) {
      ck_assert_double_eq_tol(res_true.vertex.coords[i], res.vertex.coords[i],
                              1e-3);
    }
    zoom = 0.85;
    scale_C(&res, zoom);

    for (int i = 0; i < 12; i += 3) {
      res_true.vertex.coords[i] *= zoom;
      res_true.vertex.coords[i + 1] *= zoom;
      res_true.vertex.coords[i + 2] *= zoom;
    }
    for (int i = 0; i < 12; i++) {
      ck_assert_double_eq_tol(res_true.vertex.coords[i], res.vertex.coords[i],
                              1e-3);
    }
  }

  obj_info_free(&res_true);
  obj_info_free(&res);
}
END_TEST

/////////

Suite *parcer_suite(void) {
  Suite *s_parcer = suite_create("parcer_testcase");
  TCase *tc_parcer = tcase_create("parcer");

  suite_add_tcase(s_parcer, tc_parcer);
  tcase_add_test(tc_parcer, parc_true);
  tcase_add_test(tc_parcer, parc_fail_open);
  tcase_add_test(tc_parcer, parc_fail_file);
  tcase_add_test(tc_parcer, parc_centering);

  return s_parcer;
}

Suite *move_suite(void) {
  Suite *s_move = suite_create("move_testcase");
  TCase *tc_move = tcase_create("move");

  suite_add_tcase(s_move, tc_move);
  tcase_add_test(tc_move, move);

  return s_move;
}

Suite *rotate_suite(void) {
  Suite *s_rotate = suite_create("rotate_testcase");
  TCase *tc_rotate = tcase_create("rotate");

  suite_add_tcase(s_rotate, tc_rotate);
  tcase_add_test(tc_rotate, test_rotate_x_plus);
  tcase_add_test(tc_rotate, test_rotate_x_minus);
  tcase_add_test(tc_rotate, test_rotate_y_plus);
  tcase_add_test(tc_rotate, test_rotate_y_minus);
  tcase_add_test(tc_rotate, test_rotate_z_plus);
  tcase_add_test(tc_rotate, test_rotate_z_minus);

  return s_rotate;
}

Suite *zoom_suite(void) {
  Suite *s_zoom = suite_create("zoom_testcase");
  TCase *tc_zoom = tcase_create("zoom");

  suite_add_tcase(s_zoom, tc_zoom);
  tcase_add_test(tc_zoom, test_zoom);

  return s_zoom;
}

////////////

int main() {
  int number_failed;
  Suite *s_move, *s_parcer, *s_rotate, *s_zoom;
  SRunner *sr_move, *sr_parcer, *sr_rotate, *sr_zoom;

  s_parcer = parcer_suite();
  sr_parcer = srunner_create(s_parcer);
  srunner_set_fork_status(sr_parcer, CK_NOFORK);
  srunner_run_all(sr_parcer, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr_parcer);
  srunner_free(sr_parcer);

  s_move = move_suite();
  sr_move = srunner_create(s_move);
  srunner_set_fork_status(sr_move, CK_NOFORK);
  srunner_run_all(sr_move, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr_move);
  srunner_free(sr_move);

  s_rotate = rotate_suite();
  sr_rotate = srunner_create(s_rotate);
  srunner_set_fork_status(sr_rotate, CK_NOFORK);
  srunner_run_all(sr_rotate, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr_rotate);
  srunner_free(sr_rotate);

  s_zoom = zoom_suite();
  sr_zoom = srunner_create(s_zoom);
  srunner_set_fork_status(sr_zoom, CK_NOFORK);
  srunner_run_all(sr_zoom, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr_zoom);
  srunner_free(sr_zoom);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
