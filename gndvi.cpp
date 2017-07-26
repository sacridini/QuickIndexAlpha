#include "gndvi.h"
#include "ui_gndvi.h"

Gndvi::Gndvi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gndvi)
{
    ui->setupUi(this);
    this->setWindowTitle("GNDVI - Green Normalized Difference Vegetation Index");
}

Gndvi::~Gndvi()
{
    delete ui;
}

void Gndvi::gndvi(std::string nir_FilePath, std::string green_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating GNDVI...");
    GDALDataset *green, *nir, *gndvi;
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

    // Cria a imagem Chlorophyll Index Green - Create Chlorophyll Index Green source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    gndvi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    gndvi->SetGeoTransform(transform);
    gndvi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *green_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *gndvi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating GNDVI...");
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
                gndvi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = nir_Row[j] - green_Row[j];
                lower_Row[j] = nir_Row[j] + green_Row[j];
                gndvi_Row[j] = upper_Row[j] / lower_Row[j];

            }
        }
        gndvi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, gndvi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(green_Row);
    CPLFree(gndvi_Row);

    // Delete datasets
    GDALClose(green);
    GDALClose(nir);
    GDALClose(gndvi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> GNDVI created! <--");
}

void Gndvi::on_gndviNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->gndviNIR_txt->setText(nirFilepath);
}

void Gndvi::on_gndviGREEN_btn_2_clicked()
{
    greenFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input SWIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->gndviGREEN_txt->setText(greenFilepath);
}

void Gndvi::on_gndviOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->gndviOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Gndvi::on_gndvi_btn_clicked()
{
    gndvi(nirFilepath.toUtf8().constData(), greenFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
