#include "evi.h"
#include "ui_evi.h"

Evi::Evi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Evi)
{
    ui->setupUi(this);
    this->setWindowTitle("EVI - Enhanced Vegetation Index");

}

Evi::~Evi()
{
    delete ui;
}

void Evi::evi(std::string nir_FilePath, std::string red_FilePath, std::string blue_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating EVI...");
    GDALDataset *red, *nir, *blue, *evi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (NIR, BLUE and RED bands)
    ui->textBrowser->append("Reading bands...");
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);
    red = (GDALDataset*)GDALOpen(red_FilePath.c_str(), GA_ReadOnly);
    blue = (GDALDataset*)GDALOpen(blue_FilePath.c_str(), GA_ReadOnly);

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

    // Cria a imagem EVI - Create EVI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    evi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    evi->SetGeoTransform(transform);
    evi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *blue_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *evi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating EVI...");
    for (int i = 0; i < nRows; i++)
    {
        nir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, nir_Row, nCols, 1, GDT_Float32, 0, 0);
        red->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, red_Row, nCols, 1, GDT_Float32, 0, 0);
        blue->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, blue_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (nir_Row[j] == noData)
            {
                upper_Row[j] = noData;
                lower_Row[j] = noData;
                evi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = nir_Row[j] - red_Row[j];
                lower_Row[j] = nir_Row[j] + (6.0 * red_Row[j]) - (7.5 * blue_Row[j]) + 1.0;
                evi_Row[j] = 2.5 * (upper_Row[j] / lower_Row[j]);
            }
        }
        evi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, evi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(red_Row);
    CPLFree(blue_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(evi_Row);

    // Delete datasets
    GDALClose(nir);
    GDALClose(red);
    GDALClose(blue);
    GDALClose(evi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> EVI created! <--");
}

void Evi::on_eviBLUE_btn_clicked()
{
    blueFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->eviBLUE_txt->setText(blueFilepath);
}

void Evi::on_eviRED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->eviRED_txt->setText(redFilepath);
}

void Evi::on_eviNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->eviNIR_txt->setText(nirFilepath);
}

void Evi::on_eviOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->eviOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Evi::on_evi_btn_clicked()
{
    evi(nirFilepath.toUtf8().constData(), redFilepath.toUtf8().constData(), blueFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
