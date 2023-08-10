#include <math.h>

#include "s21_viewer_tests.h"

#define PI 3.14159265358979323846

START_TEST(viewer_tests_0) {
  s21_obj data_1 = {0, NULL, NULL};
  s21_obj data_2 = {0, NULL, NULL};
  char file[] = "../../models/cube.obj";
  s21_parse_obj_file(file, &data_1);
  s21_parse_obj_file(file, &data_2);

  s21_xyz_movement(data_2.matrix_3d, 1, -0.25, 3.667);
  for (uint32_t i = 0; i < data_1.polygons->count; i++) {
    ck_assert_double_eq(data_1.matrix_3d->matrix[i][0] + 1,
                        data_2.matrix_3d->matrix[i][0]);
    ck_assert_double_eq(data_1.matrix_3d->matrix[i][1] - 0.25,
                        data_2.matrix_3d->matrix[i][1]);
    ck_assert_double_eq(data_1.matrix_3d->matrix[i][2] + 3.667,
                        data_2.matrix_3d->matrix[i][2]);
  }
  s21_free_obj_struct(&data_1);
  s21_free_obj_struct(&data_2);
}
END_TEST

START_TEST(viewer_tests_1) {
  s21_obj data = {0, NULL, NULL};
  char file[] = "../../models/cube.obj";

  double compared[8][3] = {
      {0.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, -1.0, 1.0},
      {1.0, 0.0, 0.0}, {1.0, -1.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, -1.0, 1.0},
  };

  s21_parse_obj_file(file, &data);
  s21_rotation_by_ox(data.matrix_3d, PI / 2);

  for (uint32_t i = 0; i < data.polygons->count; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(compared[i][j], data.matrix_3d->matrix[i][j],
                              0.000001);
    }
  }
  s21_free_obj_struct(&data);
}
END_TEST

START_TEST(viewer_tests_2) {
  s21_obj data = {0, NULL, NULL};
  char file[] = "../../models/cube.obj";

  double compared[8][3] = {
      {0.0, 0.0, 0.0},  {0.0, 0.0, -1.0},  {0.0, 1.0, 0.0},  {0.0, 1.0, -1.0},
      {-1.0, 0.0, 0.0}, {-1.0, 0.0, -1.0}, {-1.0, 1.0, 0.0}, {-1.0, 1.0, -1.0},
  };

  s21_parse_obj_file(file, &data);
  s21_rotation_by_oy(data.matrix_3d, PI);

  for (uint32_t i = 0; i < data.polygons->count; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(compared[i][j], data.matrix_3d->matrix[i][j],
                              0.000001);
    }
  }
  s21_free_obj_struct(&data);
}
END_TEST

START_TEST(viewer_tests_3) {
  s21_obj data = {0, NULL, NULL};
  char file[] = "../../models/cube.obj";

  double compared[8][3] = {
      {0.0, 0.0, 0.0},
      {0.0, 0.0, 1.0},
      {-0.707107, -0.707107, 0.0},
      {-0.707107, -0.707107, 1.0},
      {-0.707107, 0.707107, 0.0},
      {-0.707107, 0.707107, 1.0},
      {-1.414214, 0.0, 0.0},
      {-1.414214, 0.0, 1.0},
  };

  s21_parse_obj_file(file, &data);
  s21_rotation_by_oz(data.matrix_3d, 3 * PI / 4);

  for (uint32_t i = 0; i < data.polygons->count; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(compared[i][j], data.matrix_3d->matrix[i][j],
                              0.000001);
    }
  }
  s21_free_obj_struct(&data);
}
END_TEST

START_TEST(viewer_tests_4) {
  s21_obj data = {0, NULL, NULL};
  char file[] = "../../models/cube.obj";

  double compared[8][3] = {
      {0.0, 0.0, 0.0}, {0.0, 0.0, 2.5}, {0.0, 2.5, 0.0}, {0.0, 2.5, 2.5},
      {2.5, 0.0, 0.0}, {2.5, 0.0, 2.5}, {2.5, 2.5, 0.0}, {2.5, 2.5, 2.5},
  };

  s21_parse_obj_file(file, &data);
  s21_scale(data.matrix_3d, 2.5);

  for (uint32_t i = 0; i < data.polygons->count; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(compared[i][j], data.matrix_3d->matrix[i][j],
                              0.000001);
    }
  }
  s21_free_obj_struct(&data);
}
END_TEST

START_TEST(viewer_tests_5) {
  s21_obj data = {0, NULL, NULL};
  char file[] = "../../model/cube.obj";

  int error = s21_parse_obj_file(file, &data);
  ck_assert_int_eq(error, S21_ERR);
  s21_free_obj_struct(&data);
}
END_TEST

START_TEST(viewer_tests_6) {
  s21_obj data = {0, NULL, NULL};
  char file[] = "../../models/cube.obj";

  double compared_matrix[8][3] = {
      {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
      {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0},
  };

  uint32_t compared_vertex[72] = {0, 6, 6, 4, 4, 0, 0, 2, 2, 6, 6, 0, 0, 3, 3, 2, 2, 0, 0, 1, 1, 3, 3, 0,
                        2, 7, 7, 6, 6, 2, 2, 3, 3, 7, 7, 2, 4, 6, 6, 7, 7, 4, 4, 7, 7, 5, 5, 4,
                        0, 4, 4, 5, 5, 0, 0, 5, 5, 1, 1, 0, 1, 5, 5, 7, 7, 1, 1, 7, 7, 3, 3, 1};

  s21_parse_obj_file(file, &data);

  for (uint32_t i = 0; i < data.polygons->count; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(compared_matrix[i][j], data.matrix_3d->matrix[i][j],
                              0.000001);
    }
  }
  ck_assert_int_eq(data.polygons_count, 12);
  for (int i = 0; i < 2; i++) {
    ck_assert_int_eq(compared_vertex[i], data.polygons->vertexes[i]);
  }  

  s21_free_obj_struct(&data);
}
END_TEST

Suite *s21_viewer_tests(void) {
  Suite *s1 = suite_create(PRE_TEST_HEADER "S21_VIEWER" POST_TEST_HEADER);
  TCase *tc1_1 = tcase_create("S21_VIEWER");

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, viewer_tests_0);
  tcase_add_test(tc1_1, viewer_tests_1);
  tcase_add_test(tc1_1, viewer_tests_2);
  tcase_add_test(tc1_1, viewer_tests_3);
  tcase_add_test(tc1_1, viewer_tests_4);
  tcase_add_test(tc1_1, viewer_tests_5);
  tcase_add_test(tc1_1, viewer_tests_6);

  return s1;
}