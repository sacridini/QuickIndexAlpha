#include "gdvi.h"
#include "ui_gdvi.h"

Gdvi::Gdvi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gdvi)
{
    ui->setupUi(this);
    this->setWindowTitle("GVDI");

}

Gdvi::~Gdvi()
{
    delete ui;
}

void Gdvi::gdvi(std::string nir_FilePath, std::string green_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating GDVI...");
    GDALDataset *nir, *green, *gdvi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (NIR and GREEN bands)
    ui->textBrowser->append("--> Raster Information <--");
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);
    green = (GDALDataset*)GDALOpen(green_FilePath.c_str(), GA_ReadOnly);

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

    // Cria a imagem GDVI - Create GDVI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    gdvi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    gdvi->SetGeoTransform(transform);
    gdvi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *green_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *gdvi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating GDVI...");
    for (int i = 0; i < nRows; i++)
    {
        nir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, nir_Row, nCols, 1, GDT_Float32, 0, 0);
        green->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, green_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (nir_Row[j] == noData)
            {
                gdvi_Row[j] = noData;
            }
            else
            {
                gdvi_Row[j] = nir_Row[j] - green_Row[j];
            }
        }
        gdvi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, gdvi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(green_Row);
    CPLFree(gdvi_Row);

    // Delete datasets
    GDALClose(green);
    GDALClose(nir);
    GDALClose(gdvi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> GDVI created! <--");
}

void Gdvi::on_gdviNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->gdviNIR_txt->setText(nirFilepath);
}

void Gdvi::on_gdviGREEN_btn_2_clicked()
{
    greenFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input SWIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->gdviGREEN_txt->setText(greenFilepath);
}

void Gdvi::on_gdviOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->gdviOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Gdvi::on_gdvi_btn_clicked()
{
    gdvi(nirFilepath.toUtf8().constData(), greenFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
