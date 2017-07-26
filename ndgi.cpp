#include "ndgi.h"
#include "ui_ndgi.h"

Ndgi::Ndgi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ndgi)
{
    ui->setupUi(this);
    this->setWindowTitle("NDGI - Normalized Difference Greenness Index");
}

Ndgi::~Ndgi()
{
    delete ui;
}

void Ndgi::ndgi(std::string green_FilePath, std::string red_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating NDGI...");
    GDALDataset *green, *red, *ndgi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (GREEN and RED bands)
    green = (GDALDataset*)GDALOpen(green_FilePath.c_str(), GA_ReadOnly);
    red = (GDALDataset*)GDALOpen(red_FilePath.c_str(), GA_ReadOnly);

    // Get raster info to use as parameters
    ui->textBrowser->append("--> Raster Information <--");
    nRows = green->GetRasterBand(1)->GetYSize();
    nCols = green->GetRasterBand(1)->GetXSize();
    noData = green->GetRasterBand(1)->GetNoDataValue();
    green->GetGeoTransform(transform);
    ui->textBrowser->append("Number of Rows: " + QString::number(  nRows  ));
    ui->textBrowser->append("Number of Columns: " + QString::number(  nCols  ));
    ui->textBrowser->append("Width pixel size: " + QString::number(  transform[1]  ));
    ui->textBrowser->append("Height pixel size: " + QString::number(  transform[5]  ));
    ui->textBrowser->append("--> ................... <--");

    // Cria a imagem NDGI - Create NDGI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    ndgi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    ndgi->SetGeoTransform(transform);
    ndgi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *green_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *ndgi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating NDGI...");
    for (int i = 0; i < nRows; i++)
    {
        green->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, green_Row, nCols, 1, GDT_Float32, 0, 0);
        red->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, red_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (green_Row[j] == noData)
            {
                upper_Row[j] = noData;
                lower_Row[j] = noData;
                ndgi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = green_Row[j] - red_Row[j];
                lower_Row[j] = green_Row[j] + red_Row[j];
                ndgi_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        ndgi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, ndgi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(green_Row);
    CPLFree(red_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(ndgi_Row);

    // Delete datasets
    GDALClose(red);
    GDALClose(green);
    GDALClose(ndgi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> NDGI created! <--");
}

void Ndgi::on_ndgiGREEN_btn_clicked()
{
    greenFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input GREEN band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndgiGREEN_txt->setText(greenFilepath);
}

void Ndgi::on_ndgiRED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndgiRED_txt->setText(redFilepath);
}

void Ndgi::on_ndgiOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->ndgiOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Ndgi::on_ndgi_btn_clicked()
{
    ndgi(greenFilepath.toUtf8().constData(), redFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
