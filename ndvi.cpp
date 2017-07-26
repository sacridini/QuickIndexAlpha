#include "ndvi.h"
#include "ui_ndvi.h"

Ndvi::Ndvi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ndvi)
{
    ui->setupUi(this);
    this->setWindowTitle("NDVI - Normalized Difference Vegetation Index");
}

Ndvi::~Ndvi()
{
    delete ui;
}

void Ndvi::on_ndvi_btn_clicked()
{
    ndvi(nirFilepath.toUtf8().constData(), redFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}


void Ndvi::ndvi(std::string nir_FilePath, std::string red_FilePath, std::string out_FilePath)
{
    GDALAllRegister();

    GDALDataset *red, *nir, *ndvi;
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

    // Create NDVI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    ndvi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    ndvi->SetGeoTransform(transform);
    ndvi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *red_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *ndvi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating NDVI...");
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
            }
            else
            {
                upper_Row[j] = nir_Row[j] - red_Row[j];
                lower_Row[j] = nir_Row[j] + red_Row[j];
                ndvi_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        ndvi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, ndvi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(red_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(ndvi_Row);

    // Delete datasets
    GDALClose(red);
    GDALClose(nir);
    GDALClose(ndvi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> NDVI created! <--");
}

void Ndvi::on_ndviRED_btn_clicked()
{
    redFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input RED band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndviRED_txt->setText(redFilepath);

    // // Extra - Add NIR band
    // nirFilepath = redFilepath.left(redFilepath.length() - 5);
    // nirFilepath.append("4.TIF");
    //
    // // Check if the NIR file exists
    // QFileInfo check_nir_file(nirFilepath);
    // if (check_nir_file.exists() && check_nir_file.isFile()) {
    //     ui->ndviNIR_txt->setText(nirFilepath);
    // } else {
    //     nirFilepath = "";
    // }
    //
    // // Extra - Add Output filepath
    // outFilepath = redFilepath.left(redFilepath.length() - 6);
    // outFilepath.append("NDVI.TIF");
    //
    // // Check if the NDVI file exists
    // QFileInfo check_ndvi_file(outFilepath);
    // if (!(check_ndvi_file.exists() && check_ndvi_file.isFile())) {
    //     ui->ndviOut_txt->setText(outFilepath);
    // } else {
    //     outFilepath = "";
    // }
}

void Ndvi::on_ndviNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->ndviNIR_txt->setText(nirFilepath);
}

void Ndvi::on_ndviOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->ndviOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}
