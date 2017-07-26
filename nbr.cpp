#include "nbr.h"
#include "ui_nbr.h"

Nbr::Nbr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Nbr)
{
    ui->setupUi(this);
    this->setWindowTitle("NBR - Normalized Burn Ratio");
}

Nbr::~Nbr()
{
    delete ui;
}

void Nbr::nbr(std::string nir_FilePath, std::string swir_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating NBR...");
    GDALDataset *swir, *nir, *nbr;
    GDALDriver *geoTiff;
    int nRows, nCols;
    double noData;
    double transform[6];

    // Load input images (NIR and SWIR bands)
    ui->textBrowser->append("--> Raster Information <--");
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);
    swir = (GDALDataset*)GDALOpen(swir_FilePath.c_str(), GA_ReadOnly);

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

    // Cria a imagem NBR - Create NBR source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    nbr = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    nbr->SetGeoTransform(transform);
    nbr->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *swir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *nbr_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating NBR...");
    for (int i = 0; i < nRows; i++)
    {
        nir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, nir_Row, nCols, 1, GDT_Float32, 0, 0);
        swir->GetRasterBand(1)->RasterIO(GF_Read, 0, i, nCols, 1, swir_Row, nCols, 1, GDT_Float32, 0, 0);
        for (int j = 0; j < nCols; j++)
        {
            if (nir_Row[j] == noData)
            {
                upper_Row[j] = noData;
                lower_Row[j] = noData;
                nbr_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = nir_Row[j] - swir_Row[j];
                lower_Row[j] = nir_Row[j] + swir_Row[j];
                nbr_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        nbr->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, nbr_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(swir_Row);
    CPLFree(upper_Row);
    CPLFree(lower_Row);
    CPLFree(nbr_Row);

    // Delete datasets
    GDALClose(swir);
    GDALClose(nir);
    GDALClose(nbr);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> NBR created! <--");
}

void Nbr::on_nbrNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->nbrNIR_txt->setText(nirFilepath);
}

void Nbr::on_nbrSWIR_btn_2_clicked()
{
    swirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input SWIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->nbrSWIR_txt->setText(swirFilepath);
}

void Nbr::on_nbrOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->nbrOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Nbr::on_nbr_btn_clicked()
{
    nbr(nirFilepath.toUtf8().constData(), swirFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
