#include "logratio.h"
#include "ui_logratio.h"

logRatio::logRatio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logRatio)
{
    ui->setupUi(this);
    this->setWindowTitle("Log Ratio - Log Ratio");
}

logRatio::~logRatio()
{
    delete ui;
}

void logRatio::logratio(std::string nir_FilePath, std::string red_FilePath, std::string out_FilePath)
{
    GDALAllRegister();

    GDALDataset *red, *nir, *logr;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (NIR and RED bands)
    LOG("Reading bands...");
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);
    red = (GDALDataset*)GDALOpen(red_FilePath.c_str(), GA_ReadOnly);

    // Get raster info to use as parameters
    LOG("--> Raster Information <--");
    nRows = nir->GetRasterBand(1)->GetYSize();
    nCols = nir->GetRasterBand(1)->GetXSize();
    noData = nir->GetRasterBand(1)->GetNoDataValue();
    nir->GetGeoTransform(transform);
    LOG("Number of Rows: " + QString::number(  nRows  ));
    LOG("Number of Columns: " + QString::number(  nCols  ));
    LOG("Width pixel size: " + QString::number(  transform[1]  ));
    LOG("Height pixel size: " + QString::number(  transform[5]  ));
    LOG("--> ................... <--");

    // Create Log Ratio source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    logr = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    logr->SetGeoTransform(transform);
    logr->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    LOG("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *logr_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    LOG("Creating Log Ratio...");
    for (int i = 0; i < nRows; i++)
    {
        nir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, nir_Row, nCols, 1, GDT_Float32, 0, 0);
        red->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, red_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (nir_Row[j] == noData)
            {
                logr_Row[j] = noData;
            }
            else
            {
                logr_Row[j] = log(nir_Row[j] / red_Row[j]);
            }
        }
        logr->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, logr_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(red_Row);
    CPLFree(logr_Row);

    // Delete datasets
    GDALClose(red);
    GDALClose(nir);
    GDALClose(logr);
    GDALDestroyDriverManager();

    LOG("--> Log Ratio created! <--");
}

void logRatio::on_logRatioRED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->logRatioRED_txt->setText(redFilepath);
}

void logRatio::on_logRatioNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->logRatioNIR_txt->setText(nirFilepath);
}

void logRatio::on_logRatioOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->logRatioOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void logRatio::on_logRatio_btn_clicked()
{
    logratio(nirFilepath.toUtf8().constData(), redFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
