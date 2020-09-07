#include "utils.h"

void interface(int argc, char **argv) {

  if (argc != 4) {
    printf("Wrong number of arguments.\n");
    printf(".\\main input_file output_file parameter_name\n");
  }

  input = argv[1];  // Input filename
  output = argv[2]; // Output filename
  param = argv[3];  // Morphometric parameter

  if (!strcmp(param, "elevation"))
    mparam = ELEV;
  else if (!strcmp(param, "slope"))
    mparam = SLOPE;
  else if (!strcmp(param, "aspect"))
    mparam = ASPECT;
  else if (!strcmp(param, "profc"))
    mparam = PROFC;
  else if (!strcmp(param, "planc"))
    mparam = PLANC;
  else if (!strcmp(param, "longc"))
    mparam = LONGC;
  else if (!strcmp(param, "crosc"))
    mparam = CROSC;
  else if (!strcmp(param, "minic"))
    mparam = MINIC;
  else if (!strcmp(param, "maxic"))
    mparam = MAXIC;
  else {
    printf("Morphometric parameter not recognized. Assuming 'Elevation'\n");
    mparam = ELEV;
  }
}

float **malloc2Dmatrix(int rows, int cols, double nodata) {

  float **m = (float **)malloc(sizeof(float *) * rows);
  for (int r = 0; r < rows; r++)
    m[r] = (float *)malloc(sizeof(float) * cols);

  for (int r = 0; r < rows; r++)
    for (int c = 0; c < cols; c++)
      m[r][c] = nodata;

  return m;
}

void free2Dmatrix(float **buffer, int nrows, int ncols) {

  for (int r = 0; r < nrows; r++)
    free(buffer[r]);

  free(buffer);
}

DATA readRaster(const char *input) {

  DATA in;

  /* Opening the file */
  GDALAllRegister(); // Register all known drivers

  in.hDataset = GDALOpen(input, GA_ReadOnly);

  if (in.hDataset == NULL) {
    CPLGetLastErrorMsg();
    exit(-1);
  }

  /* Getting Dataset Information */
  in.hDriver = GDALGetDatasetDriver(in.hDataset);

  if (GDALGetProjectionRef(in.hDataset) != NULL)

    if (GDALGetGeoTransform(in.hDataset, in.adfGeoTransform) == CE_None) {
      if (in.adfGeoTransform[1] != -in.adfGeoTransform[5]) {
        printf("w-e and n-s pixel size are not equal.\n");
        exit(-1);
      }
    }

  /* Fetching a Raster Band */
  in.nbands = GDALGetRasterCount(in.hDataset); // number of bands
  if (in.nbands != 1) {
    printf("DEM files have only one band.\n");
    exit(-1);
  }

  in.hBand = GDALGetRasterBand(in.hDataset, 1);
  in.ncols = GDALGetRasterBandXSize(in.hBand);
  in.nrows = GDALGetRasterBandYSize(in.hBand);

  int bGotNoValue;
  in.noData = GDALGetRasterNoDataValue(in.hBand, &bGotNoValue);
  if (!bGotNoValue)
    in.noData = -32767;

  int bGotMin, bGotMax;
  in.adfMinMax[0] = GDALGetRasterMinimum(in.hBand, &bGotMin);
  in.adfMinMax[1] = GDALGetRasterMaximum(in.hBand, &bGotMax);

  if (!(bGotMin && bGotMax))
    GDALComputeRasterMinMax(in.hBand, TRUE, in.adfMinMax);
  // printf("Min=%.3f, Max=%.3f\n", in.adfMinMax[b][0], in.adfMinMax[b][1]);

  in.buffer = malloc2Dmatrix(in.nrows, in.ncols, in.noData);

  for (int r = 0; r < in.nrows; r++) {
    CPLErr e = GDALRasterIO(in.hBand, GF_Read, 0, r, in.ncols, 1, in.buffer[r],
                            in.ncols, 1, GDT_Float32, 0, 0);
  }

  return in;
}

void writeOutput(GDALDriverH driver, const char *out, float **buffer, int nrows,
                 int ncols, float noData, int type, double *adfGeo,
                 const char *proj) {
  GDALDatasetH hDstDS;

  char **papszOptions = NULL;

  hDstDS = GDALCreate(driver, out, ncols, nrows, 1, type, papszOptions);

  GDALRasterBandH hBandOut = GDALGetRasterBand(hDstDS, 1);

  for (int r = 0; r < nrows; r++) {
    CPLErr e = GDALRasterIO(hBandOut, GF_Write, 0, r, ncols, 1, buffer[r],
                            ncols, 1, type, 0, 0);
  }

  GDALSetGeoTransform(hDstDS, adfGeo);
  GDALSetProjection(hDstDS, proj);

  GDALSetRasterNoDataValue(hBandOut, noData);

  if (hDstDS != NULL)
    GDALClose(hDstDS);
}
