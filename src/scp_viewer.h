#ifndef SCP_VIEWER_H
#define SCP_VIEWER_H

#include <stdint.h>

#define SCP_MATRIX_COLS 3

enum { SCP_OK, SCP_ERR, SCP_MEM };

/**
 * @param vertexes Array of vertexes
 * @param count Count polygons
 *
 */

typedef struct {
  uint32_t *vertexes;
  uint32_t count;
} scp_polygon_t;

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
} scp_matrix_t;

typedef struct {
  uint32_t polygons_count;
  scp_polygon_t *polygons;
  scp_matrix_t *matrix_3d;
  double viewbox[SCP_MATRIX_COLS * 2];  // min_x, max_x...min_z, max_z
} scp_obj;

int scp_parse_obj_file(char *filename, scp_obj *data);
void scp_free_obj_struct(scp_obj *data);

void scp_xyz_movement(scp_matrix_t *dot_matrix, double ax, double ay,
                      double az);
void scp_rotation_by_ox(scp_matrix_t *dot_matrix, double angle);
void scp_rotation_by_oy(scp_matrix_t *dot_matrix, double angle);
void scp_rotation_by_oz(scp_matrix_t *dot_matrix, double angle);
void scp_scale(scp_matrix_t *dot_matrix, double k);

int scp_copy_matrix(scp_matrix_t *from, scp_matrix_t **to);
void scp_free_matrix(scp_matrix_t *matrix_3d);

#endif  // SCP_VIEWER_H