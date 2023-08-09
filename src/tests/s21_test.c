#include <math.h>

#include "s21_viewer_tests.h"

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

  double standart[8][3] = {
      {0.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, -1.0, 1.0},
      {1.0, 0.0, 0.0}, {1.0, -1.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, -1.0, 1.0},
  };

  s21_parse_obj_file(file, &data);
  s21_rotation_by_ox(data.matrix_3d, PI / 2);

  for (uint32_t i = 0; i < data.polygons->count; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq_tol(standart[i][j], data.matrix_3d->matrix[i][j],
                              0.0000001);
    }
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

  return s1;
}