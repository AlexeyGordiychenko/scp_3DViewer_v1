#include <math.h>

#include "s21_viewer.h"

void s21_xyz_movement(s21_matrix_t *dot_matrix, double ax, double ay,
                      double az) {
  if (ax) {
    for (uint32_t i = 0; i < dot_matrix->rows; i++)
      dot_matrix->matrix[i][0] += ax;
  }
  if (ay) {
    for (uint32_t i = 0; i < dot_matrix->rows; i++)
      dot_matrix->matrix[i][1] += ay;
  }
  if (az) {
    for (uint32_t i = 0; i < dot_matrix->rows; i++)
      dot_matrix->matrix[i][2] += az;
  }
}

void s21_rotation_by_ox(s21_matrix_t *dot_matrix, double angle) {
  if (angle) {
    for (uint32_t i = 0; i < dot_matrix->rows; i++) {
      double temp_y = dot_matrix->matrix[i][1];
      double temp_z = dot_matrix->matrix[i][2];
      dot_matrix->matrix[i][1] = temp_y * cos(angle) - temp_z * sin(angle);
      dot_matrix->matrix[i][2] = temp_y * sin(angle) + temp_z * cos(angle);
    }
  }
}

void s21_rotation_by_oy(s21_matrix_t *dot_matrix, double angle) {
  if (angle) {
    for (uint32_t i = 0; i < dot_matrix->rows; i++) {
      double temp_x = dot_matrix->matrix[i][0];
      double temp_z = dot_matrix->matrix[i][2];
      dot_matrix->matrix[i][0] = temp_x * cos(angle) + temp_z * sin(angle);
      dot_matrix->matrix[i][2] = -temp_x * sin(angle) + temp_z * cos(angle);
    }
  }
}

void s21_rotation_by_oz(s21_matrix_t *dot_matrix, double angle) {
  if (angle) {
    for (uint32_t i = 0; i < dot_matrix->rows; i++) {
      double temp_x = dot_matrix->matrix[i][0];
      double temp_y = dot_matrix->matrix[i][1];
      dot_matrix->matrix[i][0] = temp_x * cos(angle) - temp_y * sin(angle);
      dot_matrix->matrix[i][1] = temp_x * sin(angle) + temp_y * cos(angle);
    }
  }
}

void s21_scale(s21_matrix_t *dot_matrix, double k) {
  if (k) {
    for (uint32_t i = 0; i < dot_matrix->rows; i++) {
      for (uint32_t j = 0; j < dot_matrix->cols; j++)
        dot_matrix->matrix[i][j] *= k;
    }
  }
}