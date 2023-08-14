#include <math.h>

#include "s21_viewer_tests.h"

#define PI 3.14159265358979323846

START_TEST(viewer_tests_0) {
  s21_obj data_1 = {0, NULL, NULL, {0}};
  s21_obj data_2 = {0, NULL, NULL, {0}};
  char file[] = "tests/models/cube.txt";
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
  s21_obj data = {0, NULL, NULL, {0}};
  char file[] = "tests/models/cube.txt";
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
  s21_obj data = {0, NULL, NULL, {0}};
  char file[] = "tests/models/cube.txt";
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
  s21_obj data = {0, NULL, NULL, {0}};
  char file[] = "tests/models/cube.txt";
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
  s21_obj data = {0, NULL, NULL, {0}};
  char file[] = "tests/models/cube.txt";
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
  s21_obj data = {0, NULL, NULL, {0}};
  char file[] = "tests/model/cube.txt";
  int error = s21_parse_obj_file(file, &data);
  ck_assert_int_eq(error, S21_ERR);
  s21_free_obj_struct(&data);
}
END_TEST

START_TEST(viewer_tests_6) {
  s21_obj data = {0, NULL, NULL, {0}};
  char file[] = "tests/models/cube.txt";
  double compared_matrix[8][3] = {
      {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
      {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0},
  };

  s21_parse_obj_file(file, &data);
  for (uint32_t i = 0; i < data.polygons->count; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(compared_matrix[i][j],
                              data.matrix_3d->matrix[i][j], 0.000001);
    }
  }
  s21_free_obj_struct(&data);
}
END_TEST

START_TEST(viewer_tests_7) {
  s21_obj data = {0, NULL, NULL, {0}};
  char file[] = "tests/models/cube.txt";
  uint32_t compared_vertex[12][3] = {
      {1, 7, 5}, {1, 3, 7}, {1, 4, 3}, {1, 2, 4}, {3, 8, 7}, {3, 4, 8},
      {5, 7, 8}, {5, 8, 6}, {1, 5, 6}, {1, 6, 2}, {2, 6, 8}, {2, 8, 4},
  };

  s21_parse_obj_file(file, &data);
  ck_assert_int_eq(data.polygons_count, 12);
  for (uint32_t i = 0; i < data.polygons_count; i++) {
    for (uint32_t j = 0; j < data.polygons->count; j++) {
      ck_assert_int_eq(compared_vertex[i][j], data.polygons[i].vertexes[j]);
    }
  }
  s21_free_obj_struct(&data);
}
END_TEST

START_TEST(viewer_tests_8) {
  s21_obj data = {0, NULL, NULL, {0}};
  char file[] = "tests/models/flowers.txt";

  s21_parse_obj_file(file, &data);
  ck_assert_int_eq(data.polygons_count, 4061);
  s21_free_obj_struct(&data);
}
END_TEST

START_TEST(viewer_tests_9) {
  s21_obj data = {0, NULL, NULL, {0}};
  char file[] = "tests/models/cube1.txt";
  int res = s21_parse_obj_file(file, &data);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(viewer_tests_10) {
  s21_matrix_t *m1 = malloc(sizeof(s21_matrix_t));
  m1->rows = 2;
  m1->cols = 3;
  m1->matrix = malloc(m1->rows * sizeof(double *));
  for (uint32_t i = 0; i < m1->rows; i++) {
    m1->matrix[i] = malloc(m1->cols * sizeof(double));
  }

  // Assign values
  m1->matrix[0][0] = 1;
  m1->matrix[0][1] = 2;
  m1->matrix[0][2] = 3;
  m1->matrix[1][0] = 4;
  m1->matrix[1][1] = 5;
  m1->matrix[1][2] = 6;
  s21_matrix_t *m2 = NULL;
  s21_copy_matrix(m1, &m2);
  ck_assert_int_eq(m2->rows, m1->rows);
  ck_assert_int_eq(m2->cols, m1->cols);
  for (uint32_t i = 0; i < m1->rows; i++) {
    for (uint32_t j = 0; j < m1->cols; j++) {
      ck_assert_int_eq(m1->matrix[i][j], m2->matrix[i][j]);
    }
  }
  s21_free_matrix(m1);
  s21_free_matrix(m2);
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
  tcase_add_test(tc1_1, viewer_tests_7);
  tcase_add_test(tc1_1, viewer_tests_8);
  tcase_add_test(tc1_1, viewer_tests_9);
  tcase_add_test(tc1_1, viewer_tests_10);

  return s1;
}