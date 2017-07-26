#include "ndbi.h"
#include "ui_ndbi.h"

Ndbi::Ndbi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ndbi)
{
    ui->setupUi(this);
    this->setWindowTitle("NDBI - Normalized Difference Built-up Index");
}

Ndbi::~Ndbi()
{
    delete ui;
}

void Ndbi::ndbi(std::string nir_FilePath, std::string swir_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating NDBI...");
    GDALDataset *swir, *nir, *ndbi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (NIR and SWIR bands)
    ui->textBrowser->append("--> Raster Information <--");
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);
    swir = (GDALDataset*)GDALOpen(swir_FilePath.c_str(), GA_ReadOnly);

    // Get raster info to use as parameters
    nRows = nir->GetRasterBand(1)->GetYSize();
    nCols = nir->GetRasterBand(1)->GetXSize();
    noData = nir->GetRasterBand(1)->GetNoDataValue();
    nir->GetGeoTransform(transform);
    ui->textBrowser->append("Number of Rows: " + QString::number(  nRows  ));
    ui->textBrowser->append("Number of Columns: " + QString::number(  nCols  ));
    ui->textBrowser->append("Width pixel size: " + QString::number(  transform[1]  ));
    ui->textBrowser->append("Height pixel size: " + QString::number(  transform[5]  ));
    ui->textBrowser->append("--> ................... <--");

    // Cria a imagem NDVI - Create NDVI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    ndbi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    ndbi->SetGeoTransform(transform);
    ndbi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *swir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *ndbi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating NDBI...");
    for (int i = 0; i < nRows; i++)
    {
        nir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, nir_Row, nCols, 1, GDT_Float32, 0, 0);
        swir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, swir_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (nir_Row[j] == noData)
            {
                upper_Row[j] = noData;
                lower_Row[j] = noData;
                ndbi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = swir_Row[j] - nir_Row[j];
                lower_Row[j] = swir_Row[j] + nir_Row[j];
                ndbi_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        ndbi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, ndbi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // delete buffers
    CPLFree(nir_Row);
    CPLFree(swir_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(ndbi_Row);

    // delete datasets
    GDALClose(swir);
    GDALClose(nir);
    GDALClose(ndbi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> NDBI created! <--");
}

void Ndbi::on_ndbiNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndbiNIR_txt->setText(nirFilepath);
}

void Ndbi::on_ndbiSWIR_btn_2_clicked()
{
    swirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input SWIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndbiSWIR_txt->setText(swirFilepath);
}

void Ndbi::on_ndbiOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->ndbiOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Ndbi::on_ndbi_btn_clicked()
{
    ndbi(nirFilepath.toUtf8().constData(), swirFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
