#include "gvmi.h"
#include "ui_gvmi.h"

Gvmi::Gvmi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gvmi)
{
    ui->setupUi(this);
    this->setWindowTitle("GVMI - Global Vegetation Moisture Index");
}

Gvmi::~Gvmi()
{
    delete ui;
}

void Gvmi::gvmi(std::string nir_FilePath, std::string swir_FilePath, std::string out_FilePath)
{
    GDALAllRegister();

    GDALDataset *swir, *nir, *gvmi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (NIR and SWIR bands)
    ui->textBrowser->append("Reading bands...");
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);
    swir = (GDALDataset*)GDALOpen(swir_FilePath.c_str(), GA_ReadOnly);

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

    // Create GVMI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    gvmi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    gvmi->SetGeoTransform(transform);
    gvmi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *swir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *gvmi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating GVMI...");
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
                gvmi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = (nir_Row[j] + 0.1f) - (swir_Row[j] + 0.02f);
                lower_Row[j] = (nir_Row[j] + 0.1f) + (swir_Row[j] + 0.02f);
                gvmi_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        gvmi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, gvmi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(swir_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(gvmi_Row);

    // Delete datasets
    GDALClose(swir);
    GDALClose(nir);
    GDALClose(gvmi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> GVMI created! <--");
}

void Gvmi::on_gvmiNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->gvmiNIR_txt->setText(nirFilepath);
}

void Gvmi::on_gvmiSWIR_btn_clicked()
{
    swirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input SWIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->gvmiSWIR_txt->setText(swirFilepath);
}

void Gvmi::on_gvmiiOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->gvmiOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Gvmi::on_gvmi_btn_clicked()
{
    gvmi(nirFilepath.toUtf8().constData(), swirFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
