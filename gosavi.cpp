#include "gosavi.h"
#include "ui_gosavi.h"

Gosavi::Gosavi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gosavi)
{
    ui->setupUi(this);
    this->setWindowTitle("GOSAVI - Green Optimized Soil Adjusted Vegetation Index");
}

Gosavi::~Gosavi()
{
    delete ui;
}

void Gosavi::gosavi(std::string green_FilePath, std::string nir_FilePath, std::string out_FilePath)
{
    GDALAllRegister();
    ui->textBrowser->append("Creating GOSAVI...");
    GDALDataset *nir, *green, *gosavi;
    GDALDriver *geoTiff;
    int nRows, nCols;
    const double constanteY = 0.16;
    double noData;
    double transform[6];

    // Load input images (NIR and GREEN bands)
    ui->textBrowser->append("Reading bands...");
    nir = (GDALDataset*)GDALOpen(nir_FilePath.c_str(), GA_ReadOnly);
    green = (GDALDataset*)GDALOpen(green_FilePath.c_str(), GA_ReadOnly);

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

    // Cria a imagem GOSAVI - Create GOSAVI source imagem
    geoTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    gosavi = geoTiff->Create(out_FilePath.c_str(), nCols, nRows, 1, GDT_Float32, NULL);
    gosavi->SetGeoTransform(transform);
    gosavi->GetRasterBand(1)->SetNoDataValue(noData);

    // Allocate memory for the buffers
    ui->textBrowser->append("Allocation memory...");
    float *nir_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *green_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *upper_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *lower_Row = (float*)CPLMalloc(sizeof(float)*nCols);
    float *gosavi_Row = (float*)CPLMalloc(sizeof(float)*nCols);

    ui->textBrowser->append("Creating GOSAVI...");
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
                gosavi_Row[j] = noData;
            }
            else
            {
                upper_Row[j] = nir_Row[j] - green_Row[j];
                lower_Row[j] = nir_Row[j] + green_Row[j] + constanteY;
                gosavi_Row[j] = upper_Row[j] / lower_Row[j];
            }
        }
        gosavi->GetRasterBand(1)->RasterIO(GF_Write, 0, i, nCols, 1, gosavi_Row, nCols, 1, GDT_Float32, 0, 0);
    }

    // Delete buffers
    CPLFree(nir_Row);
    CPLFree(green_Row);
    CPLFree(gosavi_Row);

    // Delete datasets
    GDALClose(green);
    GDALClose(nir);
    GDALClose(gosavi);
    GDALDestroyDriverManager();

    ui->textBrowser->append("--> GOSAVI created! <--");
}

void Gosavi::on_gosaviGREEN_btn_clicked()
{
    greenFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input GREEN band"),
                                               "C://", "Tiff (*.tif)");
    ui->gosaviGREEN_txt->setText(greenFilepath);
}

void Gosavi::on_gosaviNIR_btn_clicked()
{
    nirFilepath = QFileDialog::getOpenFileName(this, tr("Choose the input NIR band"),
                                               "C://", "Tiff (*.tif)");
    ui->gosaviNIR_txt->setText(nirFilepath);
}

void Gosavi::on_gosaviOut_btn_clicked()
{
    outFilepath = QFileDialog::getSaveFileName(this, tr("Save file"),"C://", "Tiff (*tif)");
    ui->gosaviOut_txt->setText(outFilepath);
    outFilepath.append(".tif");
}

void Gosavi::on_gosavi_btn_clicked()
{
    gosavi(greenFilepath.toUtf8().constData(), nirFilepath.toUtf8().constData(), outFilepath.toUtf8().constData());
}
