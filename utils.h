#include "cpl_error.h"
#include "gdal.h"

#ifndef UTILS_H
#define UTILS_H

#define NODATA -9999

enum param { ELEV, SLOPE, ASPECT, PROFC, PLANC, LONGC, CROSC, MINIC, MAXIC };

const char *input;  // input filename
const char *output; // output filename
const char *param;  // morphometric parameter name
int mparam;         // morphometric parameter index

typedef struct {
  GDALDatasetH hDataset;
  GDALDriverH hDriver;
  int nrows;
  int ncols;
  int nbands;
  GDALRasterBandH hBand;
  double adfGeoTransform[6];
  /* adfGeoTransform[0] - top left x
     adfGeoTransform[1] - w-e pixel resolution
     adfGeoTransform[2] - 0
     adfGeoTransform[3] - top left y
     adfGeoTransform[4] - 0
     adfGeoTransform[5] - n-s pixel resolution (negative value)
  */
  double noData;
  double adfMinMax[2];
  float **buffer;
} DATA;

void interface(int, char **);

float **malloc2Dmatrix(int, int, double);

void free2Dmatrix(float **, int, int);

DATA readRaster(const char *); // read input file, char * input filename

void writeOutput(GDALDriverH, const char *, float **, int, int, float, int,
                 double *, const char *);

#endif
