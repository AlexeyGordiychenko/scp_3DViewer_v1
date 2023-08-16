#ifndef S21_VIEWER_H
#define S21_VIEWER_H

#include <stdint.h>

#define S21_MATRIX_COLS 3

enum { S21_OK, S21_ERR, S21_MEM };

/**
 * @param vertexes Array of vertexes
 * @param count Count polygons
 *
 */

typedef struct {
  uint32_t *vertexes;
  uint32_t count;
} s21_polygon_t;

/**
 * @param matrix Matrix of vertexes
 * @param rows Count rows
 * @param cols Count cols
 *
 */

typedef struct {
  double **matrix;
  uint32_t rows;
  uint32_t cols;
} s21_matrix_t;

typedef struct {
  uint32_t polygons_count;
  s21_polygon_t *polygons;
  s21_matrix_t *matrix_3d;
  double viewbox[S21_MATRIX_COLS * 2];  // min_x, max_x...min_z, max_z
} s21_obj;

int s21_parse_obj_file(char *filename, s21_obj *data);
void s21_free_obj_struct(s21_obj *data);

void s21_xyz_movement(s21_matrix_t *dot_matrix, double ax, double ay,
                      double az);
void s21_rotation_by_ox(s21_matrix_t *dot_matrix, double angle);
void s21_rotation_by_oy(s21_matrix_t *dot_matrix, double angle);
void s21_rotation_by_oz(s21_matrix_t *dot_matrix, double angle);
void s21_scale(s21_matrix_t *dot_matrix, double k);

int s21_copy_matrix(s21_matrix_t *from, s21_matrix_t **to);
void s21_free_matrix(s21_matrix_t *matrix_3d);

#endif  // S21_VIEWER_H