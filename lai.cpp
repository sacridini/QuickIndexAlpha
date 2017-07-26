#include "lai.h"
#include "ui_lai.h"

Lai::Lai(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lai)
{
    ui->setupUi(this);
    this->setWindowTitle("LAI - Leaf Area Index");
}

Lai::~Lai()
{
    delete ui;
}

void Lai::lai(std::string nir_FilePath, std::string red_FilePath, std::string blue_FilePath, std::string outEvi_FilePath, std::string outLai_FilePath)
{
    GDALAllRegister();

    GDALDataset *red, *nir, *blue, *evi, *lai;
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

    // Cria a imagem LAI - Create LAI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    evi = geoTiff->Create(outEvi_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    lai = geoTiff->Create(outLai_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    evi->SetGeoTransform(transform);
    lai->SetGeoTransform(transform);
    evi->GetRasterBand(1)->SetNoDataValue(noData);
    lai->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *blue_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *evi_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lai_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating EVI and LAI...");
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
                lai_Row[j] = 3.618 * evi_Row[j] - 0.118;
            }
        }
        evi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, evi_Row, nCols, 1, GDT_Float32, 0, 0);
        lai->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, lai_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(red_Row);
    CPLFree(blue_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(evi_Row);
    CPLFree(lai_Row);

    // Delete datasets
    GDALClose(nir);
    GDALClose(red);
    GDALClose(blue);
    GDALClose(evi);
    GDALClose(lai);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> EVI and LAI created! <--");
}

void Lai::on_laiBLUE_btn_clicked()
{
    blueFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->laiBLUE_txt->setText(blueFilepath);
}

void Lai::on_laiRED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->laiRED_txt->setText(redFilepath);
}

void Lai::on_laiNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->laiNIR_txt->setText(nirFilepath);
}

void Lai::on_eviOut_btn_clicked()
{
    outEviFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->eviOut_txt->setText(outEviFilepath);
    outEviFilepath.append(".tif");
}

void Lai::on_laiOut_btn_clicked()
{
    outLaiFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->laiOut_txt->setText(outLaiFilepath);
    outLaiFilepath.append(".tif");
}

void Lai::on_lai_btn_clicked()
{
    lai(nirFilepath.toUtf8().constData(), redFilepath.toUtf8().constData(), blueFilepath.toUtf8().constData(), outEviFilepath.toUtf8().constData(), outLaiFilepath.toUtf8().constData());
}
