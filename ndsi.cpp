#include "ndsi.h"
#include "ui_ndsi.h"

Ndsi::Ndsi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ndsi)
{
    ui->setupUi(this);
    this->setWindowTitle("NDSI - Normalized Difference Salinity Index");
}

Ndsi::~Ndsi()
{
    delete ui;
}


void Ndsi::ndsi(std::string swir1_FilePath, std::string swir2_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating NDSI...");
    GDALDataset *swir2, *swir, *ndsi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (SWIR and SWIR 2 bands)
    ui->textBrowser->append("--> Raster Information <--");
    swir = (GDALDataset*)GDALOpen(swir1_FilePath.c_str(), GA_ReadOnly);
    swir2 = (GDALDataset*)GDALOpen(swir2_FilePath.c_str(), GA_ReadOnly);

    // Get raster info to use as parameters
    nRows = swir->GetRasterBand(1)->GetYSize();
    nCols = swir->GetRasterBand(1)->GetXSize();
    noData = swir->GetRasterBand(1)->GetNoDataValue();
    swir->GetGeoTransform(transform);
    ui->textBrowser->append("Number of Rows: " + QString::number(  nRows  ));
    ui->textBrowser->append("Number of Columns: " + QString::number(  nCols  ));
    ui->textBrowser->append("Width pixel size: " + QString::number(  transform[1]  ));
    ui->textBrowser->append("Height pixel size: " + QString::number(  transform[5]  ));
    ui->textBrowser->append("--> ................... <--");

    // Cria a imagem NDSI - Create NDSI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    ndsi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    ndsi->SetGeoTransform(transform);
    ndsi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *swir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *swir2_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *ndsi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating NDSI...");
    for (int i = 0; i < nRows; i++)
    {
        swir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, swir_Row, nCols, 1, GDT_Float32, 0, 0);
        swir2->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, swir2_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (ndsi_Row[j] == noData)
            {
                upper_Row[j] = noData;
                lower_Row[j] = noData;
                ndsi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = swir_Row[j] - swir2_Row[j];
                lower_Row[j] = swir_Row[j] + swir2_Row[j];
                ndsi_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        ndsi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, ndsi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(swir_Row);
    CPLFree(swir2_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(ndsi_Row);

    // Delete datasets
    GDALClose(swir);
    GDALClose(swir2);
    GDALClose(ndsi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> NDSI created! <--");
}

void Ndsi::on_ndsiSWIR1_btn_clicked()
{
    swir1Filepath = QFileDialog::getOpenFileName(this, tr("Choose the input SWIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndsiSWIR1_txt->setText(swir1Filepath);
}

void Ndsi::on_ndsiSWIR2_btn_clicked()
{
    swir2Filepath = QFileDialog::getOpenFileName(this, tr("Choose the input SWIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndsiSWIR2_txt->setText(swir2Filepath);
}

void Ndsi::on_ndsiOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->ndsiOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Ndsi::on_ndsi_btn_clicked()
{
    ndsi(swir1Filepath.toUtf8().constData(), swir2Filepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
