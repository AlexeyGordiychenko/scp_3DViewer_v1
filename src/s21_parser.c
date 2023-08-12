#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_viewer.h"

static long s21_get_file_size(FILE *fp) {
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return file_size;
}

static void s21_skip_line(FILE *fp) {
  if (!feof(fp)) {
    char *tmp = fp->_IO_read_ptr;
    fscanf(fp, "%*[^\n\r] ");
    if (tmp == fp->_IO_read_ptr && !feof(fp)) {
      fseek(fp, 1, SEEK_CUR);
    }
  }
}

static bool s21_is_line_to_process(FILE *fp, char c) {
  char tmp;
  char format[] = {c, '%', 'c'};
  return fscanf(fp, format, &tmp) == 1 && tmp == ' ';
}

static void s21_calculate_min_max_coords(s21_obj *data, uint32_t v_count) {
  double *curr_row = data->matrix_3d->matrix[v_count - 1];
  if (data->viewbox[0] > curr_row[0]) data->viewbox[0] = curr_row[0];
  if (data->viewbox[1] < curr_row[0]) data->viewbox[1] = curr_row[0];
  if (data->viewbox[2] > curr_row[1]) data->viewbox[2] = curr_row[1];
  if (data->viewbox[3] < curr_row[1]) data->viewbox[3] = curr_row[1];
  if (data->viewbox[4] > curr_row[2]) data->viewbox[4] = curr_row[2];
  if (data->viewbox[5] < curr_row[2]) data->viewbox[5] = curr_row[2];
}

static int s21_allocate_obj_struct(s21_obj **data, uint32_t v_size,
                                   uint32_t f_size) {
  int res = v_size + f_size;
  res = S21_OK;
  s21_matrix_t *matrix_3d = malloc(sizeof(s21_matrix_t));
  double **matrix = malloc(v_size * sizeof(double *));
  s21_polygon_t *polygons = malloc(sizeof(s21_polygon_t) * f_size);

  if (matrix_3d == NULL || matrix == NULL || polygons == NULL) res = S21_MEM;

  (*data)->matrix_3d = matrix_3d;
  if (matrix_3d) {
    (*data)->matrix_3d->rows = 0;
    (*data)->matrix_3d->cols = S21_MATRIX_COLS;
    (*data)->matrix_3d->matrix = matrix;
  } else {
    free(matrix);
  }
  (*data)->polygons = polygons;
  (*data)->polygons_count = 0;
  if (polygons) (*data)->polygons->count = 0;

  memset((*data)->viewbox, 0, sizeof(double) * S21_MATRIX_COLS * 2);
  return res;
}

static void s21_realloc_to_count(s21_obj **data, uint32_t v_count,
                                 uint32_t f_count) {
  if (v_count != 0) {
    double **tmp_m =
        realloc((*data)->matrix_3d->matrix, v_count * sizeof(double *));
    if (tmp_m) (*data)->matrix_3d->matrix = tmp_m;
  }
  if (f_count != 0) {
    s21_polygon_t *tmp_p =
        realloc((*data)->polygons, f_count * sizeof(s21_polygon_t));
    if (tmp_p) (*data)->polygons = tmp_p;
  }
}

static void s21_free_matrix(s21_matrix_t *matrix_3d) {
  if (!matrix_3d) return;
  for (uint32_t i = 0; i < matrix_3d->rows; i++) {
    if (matrix_3d->matrix[i]) free(matrix_3d->matrix[i]);
  }
  free(matrix_3d->matrix);
  matrix_3d->matrix = NULL;
  matrix_3d->rows = 0;
  matrix_3d->cols = 0;
  free(matrix_3d);
}

static void s21_free_polygons(s21_polygon_t *polygons, uint32_t count) {
  if (!polygons) return;
  for (uint32_t i = 0; i < count; i++) {
    if (polygons[i].vertexes) free(polygons[i].vertexes);
  }
  free(polygons);
  polygons = NULL;
}

static int s21_adjust_f_value(int64_t *value, uint32_t v_count) {
  int res = S21_OK;
  if (*value < 0) (*value) += v_count + 1;
  if (*value <= 0 || *value > v_count) res = S21_ERR;
  return res;
}

static int s21_parse_f(FILE *fp, uint32_t v_count, uint32_t *f_count,
                       uint32_t *f_size, s21_polygon_t **polygons) {
  // realloc if we're out of bounds
  if (*f_count >= *f_size) {
    (*f_size) *= 2;
    s21_polygon_t *tmp = realloc(*polygons, *f_size * sizeof(s21_polygon_t));
    if (tmp == NULL) return S21_MEM;
    *polygons = tmp;
  }

  uint32_t *vertexes = malloc(sizeof(uint32_t) * S21_MATRIX_COLS);
  if (vertexes == NULL) return S21_MEM;

  s21_polygon_t *polygon = (*polygons) + *f_count;
  uint32_t count = 0, size = S21_MATRIX_COLS;
  int64_t value;
  char term = 0;
  int res = S21_OK;
  while (res == S21_OK && !feof(fp) && term != '\n' && term != '\r' &&
         fscanf(fp, "%ld%c", &value, &term) > 0) {
    if (count >= size) {
      size *= 2;
      uint32_t *tmp = realloc(vertexes, size * sizeof(uint32_t));
      if (tmp == NULL) {
        res = S21_MEM;
      } else {
        vertexes = tmp;
      }
    }
    if (res == S21_OK) {
      res = s21_adjust_f_value(&value, v_count);
      vertexes[count++] = value;
      if (term == '/') {
        fscanf(fp, "%*s%c", &term);
      }
    }
  }

  polygon->count = count;
  polygon->vertexes = vertexes;

  (*f_count)++;
  if (term == '\r') fseek(fp, 1, SEEK_CUR);

  return res;
}

static int s21_parse_v(FILE *fp, uint32_t *v_count, uint32_t *v_size,
                       double ***matrix) {
  // realloc if we're out of bounds
  if (*v_count >= *v_size) {
    (*v_size) *= 2;
    double **tmp = realloc(*matrix, *v_size * sizeof(double *));
    if (tmp == NULL) return S21_MEM;
    *matrix = tmp;
  }
  // parse a line starting with "v "
  char term = 0;
  double x, y, z;
  int res = S21_ERR;
  if (fscanf(fp, "%lf %lf %lf%c", &x, &y, &z, &term) >= 3) {
    double *row = malloc(S21_MATRIX_COLS * sizeof(double));
    if (row) {
      row[0] = x, row[1] = y, row[2] = z;
      (*matrix)[*v_count] = row;
      (*v_count)++;
      if (term == '\r') {  // fscanf can't skip \r
        fseek(fp, 1, SEEK_CUR);
      } else if (term != '\n') {  // skip the rest of the line if needed
        fscanf(fp, "%*[^\n\r] ");
      }
      res = S21_OK;
    } else {
      res = S21_MEM;
    }
  }

  return res;
}

int s21_parse_obj_file(char *filename, s21_obj *data) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) return S21_ERR;

  // file size / file lines ~ 30 - approximate coefficient
  // "v "/"f " takes approximately 1/4 of the lines
  long file_size = s21_get_file_size(fp);
  uint32_t coefficient = file_size / 30 * 0.25;
  uint32_t v_size = coefficient, f_size = coefficient, v_count = 0, f_count = 0;

  int res = s21_allocate_obj_struct(&data, v_size, f_size);
  if (res != S21_OK) {
    fclose(fp);
    return res;
  }

  while (res == S21_OK && !feof(fp)) {
    if (s21_is_line_to_process(fp, 'v')) {
      res = s21_parse_v(fp, &v_count, &v_size, &data->matrix_3d->matrix);
      if (res == S21_OK) s21_calculate_min_max_coords(data, v_count);
    } else if (s21_is_line_to_process(fp, 'f')) {
      res = s21_parse_f(fp, v_count, &f_count, &f_size, &data->polygons);
    } else {
      s21_skip_line(fp);
    }
  }

  data->matrix_3d->rows = v_count;
  data->polygons_count = f_count;

  if (res == S21_OK) {
    s21_realloc_to_count(&data, v_count, f_count);
  } else {
    s21_free_obj_struct(data);
  }

  fclose(fp);

  return res;
}

void s21_free_obj_struct(s21_obj *data) {
  if (!data) return;
  s21_free_matrix(data->matrix_3d);
  s21_free_polygons(data->polygons, data->polygons_count);
}
