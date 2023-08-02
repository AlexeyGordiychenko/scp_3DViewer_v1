#ifndef S21_VIEWER_H
#define S21_VIEWER_H

typedef struct {
  int *vertexes;
  int count;
} s21_polygon_t;

typedef struct {
  double **matrix;
  int rows;
  int cols;
} s21_matrix_t;

typedef struct {
  int vertexes_count;
  int facets_count;
  s21_matrix_t matrix_3d;
  s21_polygon_t *polygons;
} s21_obj;

#endif  // S21_VIEWER_H