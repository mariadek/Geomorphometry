#include "morph.h"

float slope(float *der){

  float slope;

  slope = sqrt(der[3] * der[3] + der[4] * der[4]);

  return atan(slope) * RAD2DEG;  // degrees
}

float profc(float *der, double nodata){

  float profc;

  if(der[3] == 0 && der[4] == 0)
    profc = nodata;
  else
    profc = -((der[3] * der[3] * der[0]) + 2 * (der[3] * der[4] * der[2]) +
              (der[4] * der[4] * der[1])) /
              (((der[3] * der[3]) + (der[4] * der[4])) *
              pow((1 + (der[3] * der[3]) + (der[4] * der[4])), 1.5));

  return profc;
}

float tangc(float *der, double nodata){

  float tangc;

  if(der[3] == 0 && der[4] == 0)
    tangc = nodata;
  else
    tangc = -((der[1] * der[3] * der[3]) + (der[0] * der[4] * der[4]) -
            2 * (der[2] * der[3] * der[4])) /
            (((der[4] * der[4]) + (der[3] * der[3])) *
            sqrt(1 + (der[3] * der[3]) + (der[4] * der[4])));

  return tangc;
}

float minc(float *der){

  float minc;

  minc = -der[0] - der[1] - sqrt(((der[0] - der[1]) * (der[0] - der[1])) + (der[2] * der[2]));

  return minc;
}

float maxc(float *der){

  float maxc;

  maxc = -der[0] - der[1] + sqrt(((der[0] - der[1]) * (der[0] - der[1])) + (der[2] * der[2]));

  return maxc;
}
