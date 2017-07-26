#include "evi2.h"
#include "ui_evi2.h"

Evi2::Evi2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Evi2)
{
    ui->setupUi(this);
    this->setWindowTitle("EVI-2 - Enhanced Vegetation Index");

}

Evi2::~Evi2()
{
    delete ui;
}

void Evi2::evi2(std::string nir_FilePath, std::string red_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating EVI-2...");
    GDALDataset *red, *nir, *evi2;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (NIR and RED bands)
    ui->textBrowser->append("Reading bands...");
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);
    red = (GDALDataset*)GDALOpen(red_FilePath.c_str(), GA_ReadOnly);

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

    // Cria a imagem EVI 2 - Create EVI 2 source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    evi2 = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    evi2->SetGeoTransform(transform);
    evi2->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *evi2_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating EVI-2...");
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
                evi2_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = nir_Row[j] - red_Row[j];
                lower_Row[j] = nir_Row[j] + 2.4 * red_Row[j] + 1.0;
                evi2_Row[j] = 2.5 * upper_Row[j] / lower_Row[j];
            }
        }
        evi2->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, evi2_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(red_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(evi2_Row);

    // Delete datasets
    GDALClose(nir);
    GDALClose(red);
    GDALClose(evi2);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> EVI-2 created! <--");
}

void Evi2::on_evi2RED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->evi2RED_txt->setText(redFilepath);
}

void Evi2::on_evi2NIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->evi2NIR_txt->setText(nirFilepath);
}

void Evi2::on_evi2Out_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->evi2Out_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Evi2::on_evi2_btn_clicked()
{
    evi2(nirFilepath.toUtf8().constData(), redFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
