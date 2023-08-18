/**
 * @file scp_parser.c
 * @brief Loading a 3D model into an object from a file
 * @author elidacon
 * @version 1
 * @date 2023-08-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scp_viewer.h"

/**
 * @brief File size calculation
 *
 * @param fp Obj File
 * @return long File size in bytes
 */

static long scp_get_file_size(FILE *fp) {
  fseek(fp, 0, SEEK_END);
  long file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return file_size;
}

/**
 * @brief
 *
 * @param fp Obj File
 */

static void scp_skip_line(FILE *fp) {
  if (!feof(fp)) {
    long pos_before = ftell(fp);
    fscanf(fp, "%*[^\n\r] ");
    long pos_after = ftell(fp);
    if (pos_before == pos_after && !feof(fp)) {
      fseek(fp, 1, SEEK_CUR);
    }
  }
}

static bool scp_is_line_to_process(FILE *fp, char c) {
  char tmp;
  char format[] = {c, '%', 'c'};
  return fscanf(fp, format, &tmp) == 1 && tmp == ' ';
}

/**
 * @brief Сalculation of the minimum and maximum coordinates
 *
 * @param data Model structure
 * @param count Number of vertices
 */

static void scp_calculate_min_max_coords(scp_obj *data, uint32_t v_count) {
  double *curr_row = data->matrix_3d->matrix[v_count - 1];
  if (data->viewbox[0] > curr_row[0]) data->viewbox[0] = curr_row[0];
  if (data->viewbox[1] < curr_row[0]) data->viewbox[1] = curr_row[0];
  if (data->viewbox[2] > curr_row[1]) data->viewbox[2] = curr_row[1];
  if (data->viewbox[3] < curr_row[1]) data->viewbox[3] = curr_row[1];
  if (data->viewbox[4] > curr_row[2]) data->viewbox[4] = curr_row[2];
  if (data->viewbox[5] < curr_row[2]) data->viewbox[5] = curr_row[2];
}

/**
 * @brief Memory allocation by structure
 *
 * @param data Model structure
 * @param v_size
 * @param s_size
 * @return int Error code
 */

static int scp_allocate_obj_struct(scp_obj **data, uint32_t v_size,
                                   uint32_t f_size) {
  int res = v_size + f_size;
  res = SCP_OK;
  scp_matrix_t *matrix_3d = malloc(sizeof(scp_matrix_t));
  double **matrix = malloc(v_size * sizeof(double *));
  scp_polygon_t *polygons = malloc(sizeof(scp_polygon_t) * f_size);

  if (matrix_3d == NULL || matrix == NULL || polygons == NULL) res = SCP_MEM;

  (*data)->matrix_3d = matrix_3d;
  if (matrix_3d) {
    (*data)->matrix_3d->rows = 0;
    (*data)->matrix_3d->cols = SCP_MATRIX_COLS;
    (*data)->matrix_3d->matrix = matrix;
  } else {
    free(matrix);
  }
  (*data)->polygons = polygons;
  (*data)->polygons_count = 0;
  if (polygons) (*data)->polygons->count = 0;

  memset((*data)->viewbox, 0, sizeof(double) * SCP_MATRIX_COLS * 2);
  return res;
}

/**
 * @brief Memory reallocation
 *
 * @param data Model structure
 * @param v_count Number of vertices
 * @param f_count Number of faces
 */

static void scp_realloc_to_count(scp_obj **data, uint32_t v_count,
                                 uint32_t f_count) {
  if (v_count != 0) {
    double **tmp_m =
        realloc((*data)->matrix_3d->matrix, v_count * sizeof(double *));
    if (tmp_m) (*data)->matrix_3d->matrix = tmp_m;
  }
  if (f_count != 0) {
    scp_polygon_t *tmp_p =
        realloc((*data)->polygons, f_count * sizeof(scp_polygon_t));
    if (tmp_p) (*data)->polygons = tmp_p;
  }
}

/**
 * @brief Freeing memory for the vertex matrix
 *
 * @param matrix_3d Vertex matrix

 */

void scp_free_matrix(scp_matrix_t *matrix_3d) {
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

/**
 * @brief Freeing memory for an array of vertices
 *
 * @param polygons Vertex matrix
 * @param count Count vertices

 */

static void scp_free_polygons(scp_polygon_t *polygons, uint32_t count) {
  if (!polygons) return;
  for (uint32_t i = 0; i < count; i++) {
    if (polygons[i].vertexes) free(polygons[i].vertexes);
  }
  free(polygons);
  polygons = NULL;
}

static int scp_adjust_f_value(int64_t *value, uint32_t v_count) {
  int res = SCP_OK;
  if (*value < 0) (*value) += v_count + 1;
  if (*value <= 0 || *value > v_count) res = SCP_ERR;
  return res;
}

/**
 * @brief Parsing polygons in file
 *
 * @param fp Obj File
 * @param v_count Count vertex
 * @param f_count Count polygons
 * @param f_size Count File size
 * @param polygons Poligons Stucture
 * @return int Error code
 */

static int scp_parse_f(FILE *fp, uint32_t v_count, uint32_t *f_count,
                       uint32_t *f_size, scp_polygon_t **polygons) {
  // realloc if we're out of bounds
  if (*f_count >= *f_size) {
    (*f_size) *= 2;
    scp_polygon_t *tmp = realloc(*polygons, *f_size * sizeof(scp_polygon_t));
    if (tmp == NULL) return SCP_MEM;
    *polygons = tmp;
  }

  uint32_t *vertexes = malloc(sizeof(uint32_t) * SCP_MATRIX_COLS);
  if (vertexes == NULL) return SCP_MEM;

  scp_polygon_t *polygon = (*polygons) + *f_count;
  uint32_t count = 0, size = SCP_MATRIX_COLS;
  int64_t value;
  char term = 0;
  int res = SCP_OK;
  while (res == SCP_OK && !feof(fp) && term != '\n' && term != '\r' &&
         fscanf(fp, "%" PRId64 "%c", &value, &term) > 0) {
    if (count >= size) {
      size *= 2;
      uint32_t *tmp = realloc(vertexes, size * sizeof(uint32_t));
      if (tmp == NULL) {
        res = SCP_MEM;
      } else {
        vertexes = tmp;
      }
    }
    if (res == SCP_OK) {
      res = scp_adjust_f_value(&value, v_count);
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

/**
 * @brief Parsing vertices in file
 *
 * @param fp Obj File
 * @param v_count Count vertex
 * @param f_size Count File size
 * @param matrix Vertex matrix
 * @return int Error code
 */

static int scp_parse_v(FILE *fp, uint32_t *v_count, uint32_t *v_size,
                       double ***matrix) {
  // realloc if we're out of bounds
  if (*v_count >= *v_size) {
    (*v_size) *= 2;
    double **tmp = realloc(*matrix, *v_size * sizeof(double *));
    if (tmp == NULL) return SCP_MEM;
    *matrix = tmp;
  }
  // parse a line starting with "v "
  char term = 0;
  double x, y, z;
  int res = SCP_ERR;
  if (fscanf(fp, "%lf %lf %lf%c", &x, &y, &z, &term) >= 3) {
    double *row = malloc(SCP_MATRIX_COLS * sizeof(double));
    if (row) {
      row[0] = x, row[1] = y, row[2] = z;
      (*matrix)[*v_count] = row;
      (*v_count)++;
      if (term == '\r') {  // fscanf can't skip \r
        fseek(fp, 1, SEEK_CUR);
      } else if (term != '\n') {  // skip the rest of the line if needed
        fscanf(fp, "%*[^\n\r] ");
      }
      res = SCP_OK;
    } else {
      res = SCP_MEM;
    }
  }

  return res;
}

/**
 * @brief Parsing Obj file
 *
 * @param filename The path to the file
 * @param data Model structure
 * @return int Error code
 */

int scp_parse_obj_file(char *filename, scp_obj *data) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) return SCP_ERR;

  // file size / file lines ~ 30 - approximate coefficient
  // "v "/"f " takes approximately 1/4 of the lines
  long file_size = scp_get_file_size(fp);
  uint32_t coefficient = file_size / 30 * 0.25;
  uint32_t v_size = coefficient, f_size = coefficient, v_count = 0, f_count = 0;

  int res = scp_allocate_obj_struct(&data, v_size, f_size);
  if (res != SCP_OK) {
    fclose(fp);
    return res;
  }

  while (res == SCP_OK && !feof(fp)) {
    if (scp_is_line_to_process(fp, 'v')) {
      res = scp_parse_v(fp, &v_count, &v_size, &data->matrix_3d->matrix);
      if (res == SCP_OK) scp_calculate_min_max_coords(data, v_count);
    } else if (scp_is_line_to_process(fp, 'f')) {
      res = scp_parse_f(fp, v_count, &f_count, &f_size, &data->polygons);
    } else {
      scp_skip_line(fp);
    }
  }

  data->matrix_3d->rows = v_count;
  data->polygons_count = f_count;

  if (res == SCP_OK) {
    scp_realloc_to_count(&data, v_count, f_count);
  } else {
    scp_free_obj_struct(data);
  }

  fclose(fp);

  return res;
}

/**
 * @brief Free obj struct
 *
 * @param data Model structure
 */

void scp_free_obj_struct(scp_obj *data) {
  if (!data) return;
  scp_free_matrix(data->matrix_3d);
  scp_free_polygons(data->polygons, data->polygons_count);
}

/**
 * @brief Matrix copy
 *
 * @param from Matrix input
 * @param to Matrix output
 * @return int Error code
 */

int scp_copy_matrix(scp_matrix_t *from, scp_matrix_t **to) {
  int res = SCP_OK;
  if (!from || !from->matrix) return SCP_MEM;
  if (*to) scp_free_matrix(*to);
  *to = malloc(sizeof(scp_matrix_t));
  if (!(*to)) return SCP_MEM;
  (*to)->matrix = malloc(sizeof(double *) * from->rows);
  if (!(*to)->matrix) return SCP_MEM;

  (*to)->rows = from->rows;
  (*to)->cols = from->cols;
  for (uint32_t i = 0; i < from->rows && res == SCP_OK; i++) {
    (*to)->matrix[i] = malloc(sizeof(double) * from->cols);
    if ((*to)->matrix[i]) {
      for (uint32_t j = 0; j < from->cols; j++) {
        (*to)->matrix[i][j] = from->matrix[i][j];
      }
    } else {
      res = SCP_MEM;
    }
  }
  return res;
}