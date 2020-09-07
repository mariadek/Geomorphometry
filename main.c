/*
  MODULE:

  AUTHOR:   Maria Dekavalla

  PURPOSE:

            This program is free software: you can redistribute it and/or modify
            it under the terms of the GNU General Public License as published
            by the Free Software Foundation, either version 3 of the License, or
            (at your option) any later version.

            This program is distributed in the hope that it will be useful,
            but WITHOUT ANY WARRANTY; without even the implied warranty of
            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
            GNU General Public License for more details.

            You should have received a copy of the GNU General Public License
            along with this program. If not, see <http://www.gnu.org/licenses/>.

            Dependencies: gdal, openmp(optional)

            Compilation:

            Execution:

            input: a raster Digital Elevation Model (DEM) file

            output: morphometric parameter

            example:
*/

#include "derivatives.h"
#include "morph.h"
#include "utils.h"
#include <stdio.h>

int main(int argc, char **argv) {

  interface(argc, argv);

  DATA in;
  in = readRaster(input);

  float **outbuffer = malloc2Dmatrix(in.nrows, in.ncols, NODATA);

  for (int r = 1; r < in.nrows - 1; r++) {
    for (int c = 1; c < in.ncols - 1; c++) {
      if (in.buffer[r - 1][c - 1] != in.noData &&
          in.buffer[r - 1][c] != in.noData &&
          in.buffer[r - 1][c + 1] != in.noData &&
          in.buffer[r][c - 1] != in.noData && in.buffer[r][c] != in.noData &&
          in.buffer[r][c + 1] != in.noData &&
          in.buffer[r + 1][c - 1] != in.noData &&
          in.buffer[r + 1][c] != in.noData &&
          in.buffer[r + 1][c + 1] != in.noData) {

        float *derivatives =
            derivatives_evans(in.buffer, r, c, in.adfGeoTransform[1]);

        if (mparam == 1)
          outbuffer[r][c] = slope(derivatives);
        else if (mparam == 3)
          outbuffer[r][c] = profc(derivatives, NODATA);
        else if (mparam == 4)
          outbuffer[r][c] = tangc(derivatives, NODATA);
        else if (mparam == 7)
          outbuffer[r][c] = minc(derivatives);
        else if (mparam == 8)
          outbuffer[r][c] = maxc(derivatives);
      }
    }
  }

  // create output files
  writeOutput(in.hDriver, output, outbuffer, in.nrows, in.ncols, NODATA,
              GDT_Float32, in.adfGeoTransform,
              GDALGetProjectionRef(in.hDataset));

  free2Dmatrix(in.buffer, in.nrows, in.ncols);
  free2Dmatrix(outbuffer, in.nrows, in.ncols);

  if (in.hDataset != NULL)
    GDALClose(in.hDataset);

  return 0;
}
