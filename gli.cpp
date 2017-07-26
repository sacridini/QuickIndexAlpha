#include "gli.h"
#include "ui_gli.h"

Gli::Gli(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gli)
{
    ui->setupUi(this);
    this->setWindowTitle("GLI - Green leaf index");
}

Gli::~Gli()
{
    delete ui;
}

void Gli::gli(std::string blue_FilePath, std::string green_FilePath, std::string red_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating GLI...");
    GDALDataset *red, *blue, *green, *gli;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (BLUE, GREEN and RED bands)
    ui->textBrowser->append("Reading bands...");
    blue = (GDALDataset*)GDALOpen(blue_FilePath.c_str(), GA_ReadOnly);
    green = (GDALDataset*)GDALOpen(green_FilePath.c_str(), GA_ReadOnly);
    red = (GDALDataset*)GDALOpen(red_FilePath.c_str(), GA_ReadOnly);

    // Get raster info to use as parameters
    ui->textBrowser->append("--> Raster Information <--");
    nRows = blue->GetRasterBand(1)->GetYSize();
    nCols = blue->GetRasterBand(1)->GetXSize();
    noData = blue->GetRasterBand(1)->GetNoDataValue();
    blue->GetGeoTransform(transform);
    ui->textBrowser->append("Number of Rows: " + QString::number(  nRows  ));
    ui->textBrowser->append("Number of Columns: " + QString::number(  nCols  ));
    ui->textBrowser->append("Width pixel size: " + QString::number(  transform[1]  ));
    ui->textBrowser->append("Height pixel size: " + QString::number(  transform[5]  ));
    ui->textBrowser->append("--> ................... <--");

    // Cria a imagem GLI - Create GLI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    gli = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    gli->SetGeoTransform(transform);
    gli->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *blue_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *green_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *gli_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating GLI...");
    for (int i = 0; i < nRows; i++)
    {
        blue->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, blue_Row, nCols, 1, GDT_Float32, 0, 0);
        green->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, green_Row, nCols, 1, GDT_Float32, 0, 0);
        red->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, red_Row, nCols, 1, GDT_Float32, 0, 0);

        for (int j = 0; j < nCols; j++)
        {
            if (blue_Row[j] == noData)
            {
                upper_Row[j] = noData;
                lower_Row[j] = noData;
                gli_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = 2 * green_Row[j] - red_Row[j] - blue_Row[j];
                lower_Row[j] = 2 * green_Row[j] + red_Row[j] + blue_Row[j];
                gli_Row[j] = upper_Row[j] + lower_Row[j];
            }
        }
        gli->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, gli_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(blue_Row);
    CPLFree(red_Row);
    CPLFree(green_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(gli_Row);

    // Delete datasets
    GDALClose(red);
    GDALClose(blue);
    GDALClose(green);
    GDALClose(gli);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> GLI created! <--");
}

void Gli::on_gliBLUE_btn_clicked()
{
    blueFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->gliBLUE_txt->setText(blueFilepath);
}

void Gli::on_gliGreen_btn_clicked()
{
    greenFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->gliGreen_txt->setText(greenFilepath);
}

void Gli::on_gliRED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->gliRED_txt->setText(redFilepath);
}

void Gli::on_gli_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->gliOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}
