#include "tvi.h"
#include "ui_tvi.h"

Tvi::Tvi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tvi)
{
    ui->setupUi(this);
    this->setWindowTitle("TVI - Transformed Vegetation Index");
}

Tvi::~Tvi()
{
    delete ui;
}

void Tvi::tvi(std::string nir_FilePath, std::string red_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating TVI...");
    GDALDataset *red, *nir, *tvi;
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

    // Cria a imagem TVI - Create TVI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    tvi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    tvi->SetGeoTransform(transform);
    tvi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *tvi_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *ndvi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating TVI...");
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
                tvi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = nir_Row[j] - red_Row[j];
                lower_Row[j] = nir_Row[j] + red_Row[j];
                ndvi_Row[j] = upper_Row[j] / lower_Row[j];
                tvi_Row[j] = sqrt((ndvi_Row[j]) + 0.5);
            }
        }
        tvi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, tvi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(red_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(ndvi_Row);
    CPLFree(tvi_Row);

    // Delete datasets
    GDALClose(red);
    GDALClose(nir);
    GDALClose(tvi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> TVI created! <--");
}

void Tvi::on_tviRED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->tviRED_txt->setText(redFilepath);
}

void Tvi::on_tviNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->tviNIR_txt->setText(nirFilepath);
}

void Tvi::on_tviOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->tviOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Tvi::on_tvi_btn_clicked()
{
    tvi(nirFilepath.toUtf8().constData(), redFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
