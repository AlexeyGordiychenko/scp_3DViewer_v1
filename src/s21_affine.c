/**
 * @file s21_affine.c
 * @brief Affine transformations of matrices
 * @author grapefru
 * @version 1
 * @date 2023-08-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <math.h>

#include "s21_viewer.h"

/**
 * @brief Model shift along the X, Y, Z axes
 *
 * @param dot_matrix Vertex matrix
 * @param ax X offset
 * @param ay Y offset
 * @param az Z offset
 *
 */

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

/**
 * @brief Rotation of the model around the X axis
 *
 * @param dot_matrix Vertex matrix
 * @param angle Angle of rotation in radians
 */

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

/**
 * @brief Rotation of the model around the Y axis
 *
 * @param dot_matrix Vertex matrix
 * @param angle Angle of rotation in radians
 */

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

/**
 * @brief Rotation of the model around the Z axis
 *
 * @param dot_matrix Vertex matrix
 * @param angle Angle of rotation in radians
 */

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

/**
 * @brief Model scaling by a value
 *
 * @param dot_matrix Vertex matrix
 * @param k Scale value
 */

void s21_scale(s21_matrix_t *dot_matrix, double k) {
  if (k) {
    for (uint32_t i = 0; i < dot_matrix->rows; i++) {
      for (uint32_t j = 0; j < dot_matrix->cols; j++)
        dot_matrix->matrix[i][j] *= k;
    }
  }
}