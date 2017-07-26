#include "formula.h"

Formula::Formula()
{

}


void Formula::ndvi(std::string _inputA, std::string _inputB, std::string _output)
{
    GDALAllRegister();
    std::string inputA = _inputA;
    std::string inputB = _inputB;
    std::string output = _output;

    GDALDataset *red, *nir, *ndvi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (NIR and RED bands)
    nir = (GDALDataset*)GDALOpen(inputA.c_str(), GA_ReadOnly);
    red = (GDALDataset*)GDALOpen(inputB.c_str(), GA_ReadOnly);

    // Get raster info to use as parameters
    nRows = nir->GetRasterBand(1)->GetYSize();
    nCols = nir->GetRasterBand(1)->GetXSize();
    noData = nir->GetRasterBand(1)->GetNoDataValue();
    nir->GetGeoTransform(transform);
    // for (int i = 0; i < 6; i++)
    // {
    //     cout << transform[i] << endl;
    // }

    // Cria a imagem NDVI - Create NDVI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    ndvi = geoTiff->Create(output.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    ndvi->SetGeoTransform(transform);
    ndvi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *ndvi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    for (int i = 0; i < nRows; i++)
    {
        nir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, nir_Row, nCols, 1, GDT_Float32, 0, 0);
        red->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, red_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (nir_Row[j] == noData)
            {
                upper_Row[j] = noData;
                lower_Row[j] = noData;
                ndvi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = nir_Row[j] - red_Row[j];
                lower_Row[j] = nir_Row[j] + red_Row[j];
                ndvi_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        ndvi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, ndvi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // delete buffers
    CPLFree(nir_Row);
    CPLFree(red_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(ndvi_Row);

    // delete datasets
    GDALClose(red);
    GDALClose(nir);
    GDALClose(ndvi);
    GDALDestroyDriverManager();
}
