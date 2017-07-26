#include "grvi.h"
#include "ui_grvi.h"

Grvi::Grvi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Grvi)
{
    ui->setupUi(this);
    this->setWindowTitle("GRVI - Green Ratio Vegetation Index");
}

Grvi::~Grvi()
{
    delete ui;
}

void Grvi::grvi(std::string nir_FilePath, std::string green_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating GRVI...");
    GDALDataset *green, *nir, *grvi;
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
    grvi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    grvi->SetGeoTransform(transform);
    grvi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *green_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *grvi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating GRVI...");
    for (int i = 0; i < nRows; i++)
    {
        nir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, nir_Row, nCols, 1, GDT_Float32, 0, 0);
        green->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, green_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (nir_Row[j] == noData)
            {
                grvi_Row[j] = noData;
            }
            else
            {
                grvi_Row[j] = nir_Row[j] - green_Row[j];

            }
        }
        grvi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, grvi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(green_Row);
    CPLFree(grvi_Row);

    // Delete datasets
    GDALClose(green);
    GDALClose(nir);
    GDALClose(grvi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> GRVI created! <--");
}

void Grvi::on_grviNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->grviNIR_txt->setText(nirFilepath);
}

void Grvi::on_grviGREEN_btn_2_clicked()
{
    greenFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input SWIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->grviGREEN_txt->setText(greenFilepath);
}

void Grvi::on_grviOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->grviOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Grvi::on_grvi_btn_clicked()
{
    grvi(nirFilepath.toUtf8().constData(), greenFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
