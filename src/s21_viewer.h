#ifndef S21_VIEWER_H
#define S21_VIEWER_H

#include <stdint.h>

enum { S21_OK, S21_ERR, S21_MEM };

typedef struct {
  uint32_t *vertexes;
  uint32_t count;
} s21_polygon_t;

typedef struct {
  double **matrix;
  uint32_t rows;
  uint32_t cols;
} s21_matrix_t;

typedef struct {
  uint32_t polygons_count;
  s21_polygon_t *polygons;
  s21_matrix_t *matrix_3d;
} s21_obj;

int s21_parse_obj_file(char *filename, s21_obj *data);
void s21_free_obj_struct(s21_obj *data);

#endif  // S21_VIEWER_H