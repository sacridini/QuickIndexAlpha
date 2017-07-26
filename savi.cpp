#include "savi.h"
#include "ui_savi.h"

Savi::Savi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Savi)
{
    ui->setupUi(this);
    this->setWindowTitle("SAVI - Soil Adjusted Vegetation Index");
}

Savi::~Savi()
{
    delete ui;
}


void Savi::savi(std::string red_FilePath, std::string nir_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating ...");
    GDALDataset *red, *nir, *savi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    const float soilFactor = 0.5; // SAVI "L" soil adjustment factor constant
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

    // Cria a imagem SAVI - Create SAVI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    savi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    savi->SetGeoTransform(transform);
    savi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *savi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating SAVI...");
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
                savi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = nir_Row[j] - red_Row[j];
                lower_Row[j] = nir_Row[j] + red_Row[j] + soilFactor;
                savi_Row[j] = (upper_Row[j] / lower_Row[j]) * 1 + soilFactor;
            }
        }
        savi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, savi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(red_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(savi_Row);

    // Delete datasets
    GDALClose(red);
    GDALClose(nir);
    GDALClose(savi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> SAVI created! <--");
}

void Savi::on_saviRED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->saviRED_txt->setText(redFilepath);
}

void Savi::on_saviNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->saviNIR_txt->setText(nirFilepath);
}

void Savi::on_saviOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->saviOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Savi::on_savi_btn_clicked()
{
    savi(redFilepath.toUtf8().constData(), nirFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
