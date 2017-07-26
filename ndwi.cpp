#include "ndwi.h"
#include "ui_ndwi.h"

Ndwi::Ndwi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ndwi)
{
    ui->setupUi(this);
    this->setWindowTitle("NDWI - Normalized Difference Water Index");
}

Ndwi::~Ndwi()
{
    delete ui;
}

void Ndwi::ndwi(std::string green_FilePath, std::string nir_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating NDWI...");
    GDALDataset *green, *nir, *ndwi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (NIR and GREEN bands)
    ui->textBrowser->append("Reading bands...");
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);
    green = (GDALDataset*)GDALOpen(green_FilePath.c_str(), GA_ReadOnly);

    // Get raster info to use as parameters
    ui->textBrowser->append("--> Raster Information <--");
    nRows = nir->GetRasterBand(1)->GetYSize();
    nCols = nir->GetRasterBand(1)->GetXSize();
    noData = nir->GetRasterBand(1)->GetNoDataValue();
    nir->GetGeoTransform(transform);
    ui->textBrowser->append("Number of Rows: " + QString::number(  nRows  ));
    ui->textBrowser->append("Number of Columns: " + QString::number(  nCols  ));
    ui->textBrowser->append("Width pixel size: " + QString::number(  transform[1]  ));
    ui->textBrowser->append("Height pixel size: " + QString::number(  transform[5]  ));
    ui->textBrowser->append("--> ................... <--");

    // Create NDVI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    ndwi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    ndwi->SetGeoTransform(transform);
    ndwi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *green_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *ndwi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating NDVI...");
    for (int i = 0; i < nRows; i++)
    {
        nir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, nir_Row, nCols, 1, GDT_Float32, 0, 0);
        green->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, green_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (nir_Row[j] == noData)
            {
                upper_Row[j] = noData;
                lower_Row[j] = noData;
                ndwi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = green_Row[j] - nir_Row[j];
                lower_Row[j] = green_Row[j] + nir_Row[j];
                ndwi_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        ndwi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, ndwi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(green_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(ndwi_Row);

    // Delete datasets
    GDALClose(green);
    GDALClose(nir);
    GDALClose(ndwi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> NDWI created! <--");
}

void Ndwi::on_ndwi_btn_clicked()
{
    // Call the function
    ndwi(greenFilepath.toUtf8().constData(), nirFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}

void Ndwi::on_ndwiGREEN_btn_clicked()
{
    greenFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input GREEN band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndwiGREEN_txt->setText(greenFilepath);
}

void Ndwi::on_ndwiNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndwiNIR_txt->setText(nirFilepath);
}

void Ndwi::on_ndwiOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->ndwiOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}
