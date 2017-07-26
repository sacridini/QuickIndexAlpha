#include "panndvi.h"
#include "ui_panndvi.h"

panNDVI::panNDVI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::panNDVI)
{
    ui->setupUi(this);
    this->setWindowTitle("panNDVI - Pan NDVI");
}

panNDVI::~panNDVI()
{
    delete ui;
}

void panNDVI::panndvi(std::string blue_FilePath, std::string green_FilePath, std::string red_FilePath, std::string nir_FilePath, std::string out_FilePath)
{
    GDALAllRegister();

    GDALDataset *blue, *green, *red, *nir, *panNDVI;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (BLUE, GREEN, RED and NIR bands)
    LOG("Reading bands...");
    blue = (GDALDataset*)GDALOpen(blue_FilePath.c_str(), GA_ReadOnly);
    green = (GDALDataset*)GDALOpen(green_FilePath.c_str(), GA_ReadOnly);
    red = (GDALDataset*)GDALOpen(red_FilePath.c_str(), GA_ReadOnly);
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);

    // Get raster info to use as parameters
    LOG("--> Raster Information <--");
    nRows = green->GetRasterBand(1)->GetYSize();
    nCols = green->GetRasterBand(1)->GetXSize();
    noData = green->GetRasterBand(1)->GetNoDataValue();
    green->GetGeoTransform(transform);
    LOG("Number of Rows: " + QString::number(  nRows  ));
    LOG("Number of Columns: " + QString::number(  nCols  ));
    LOG("Width pixel size: " + QString::number(  transform[1]  ));
    LOG("Height pixel size: " + QString::number(  transform[5]  ));
    LOG("--> ................... <--");

    // Create Log Ratio source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    panNDVI = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    panNDVI->SetGeoTransform(transform);
    panNDVI->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    LOG("Allocation memory...");
    float *blue_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *green_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *panNDVI_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    LOG("Creating Pan NDVI...");
    for (int i = 0; i < nRows; i++)
    {
        blue->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, blue_Row, nCols, 1, GDT_Float32, 0, 0);
        green->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, green_Row, nCols, 1, GDT_Float32, 0, 0);
        red->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, red_Row, nCols, 1, GDT_Float32, 0, 0);
        nir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, nir_Row, nCols, 1, GDT_Float32, 0, 0);

        for (int j = 0; j < nCols; j++)
        {
            if (green_Row[j] == noData)
            {
                upper_Row[j] = noData;
                lower_Row[j] = noData;
                panNDVI_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = nir_Row[j] - (green_Row[j] + red_Row[j] + blue_Row[j]);
                lower_Row[j] = nir_Row[j] + (green_Row[j] + red_Row[j] + blue_Row[j]);
                panNDVI_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        panNDVI->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, panNDVI_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(blue_Row);
    CPLFree(green_Row);
    CPLFree(red_Row);
    CPLFree(nir_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(panNDVI_Row);

    // Delete datasets
    GDALClose(blue);
    GDALClose(green);
    GDALClose(red);
    GDALClose(nir);
    GDALClose(panNDVI);
    GDALDestroyDriverManager();

    LOG("--> Pan NDVI created! <--");
}

void panNDVI::on_panNdviBLUE_btn_clicked()
{
    blueFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->panNdviBLUE_txt->setText(blueFilepath);
}

void panNDVI::on_panNdviGREEN_btn_2_clicked()
{
    greenFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->panNdviGREEN_txt->setText(greenFilepath);
}

void panNDVI::on_panNdviRED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->panNdviRED_txt->setText(redFilepath);
}

void panNDVI::on_panNdviNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->panNdviNIR_txt->setText(nirFilepath);
}

void panNDVI::on_panNdviOut_btn_2_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->panNdviOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void panNDVI::on_panNdvi_btn_clicked()
{
    panndvi(blueFilepath.toUtf8().constData(), greenFilepath.toUtf8().constData(), redFilepath.toUtf8().constData(), nirFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
