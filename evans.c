#include "derivatives.h"

float *derivatives_evans(float **in, int r, int c, double cellsize) {
  float flt[9];
  float *der = (float *) malloc(sizeof(float) * 6);

  flt[0] = in[r-1][c-1];
  flt[1] = in[r-1][c];
  flt[2] = in[r-1][c+1];
  flt[3] = in[r][c-1];
  flt[4] = in[r][c];
  flt[5] = in[r][c+1];
  flt[6] = in[r+1][c-1];
  flt[7] = in[r+1][c];
  flt[8] = in[r+1][c+1];

  // a or R
  der[0] = ((flt[0] + flt[2] + flt[3] + flt[5] + flt[6] + flt[8]) / (6 * cellsize * cellsize))
            - ((flt[1] + flt[4] + flt[7]) / (3 * cellsize * cellsize));

  // b or T
  der[1] = ((flt[0] + flt[1] + flt[2] + flt[6] + flt[7] + flt[8]) / (6 * cellsize * cellsize))
            - ((flt[3] + flt[4] + flt[5])/ (3 * cellsize * cellsize));

  // c or S
  der[2] = (flt[2] + flt[6] - flt[0] - flt[8]) / (4 * cellsize * cellsize);

  // d or P
  der[3] = (flt[2] + flt[5] + flt[8] - flt[0] - flt[3] - flt[6]) / (6 * cellsize);

  // e or Q
  der[4] = (flt[0] + flt[1] + flt[2] - flt[6] - flt[7] - flt[8]) / (6 * cellsize);

  // f
  der[5] = (2 * (flt[1] + flt[3] + flt[5] + flt[7]) - (flt[0] + flt[2] + flt[6] + flt[8]) + 5 * flt[4])/9;

  return der;
}
