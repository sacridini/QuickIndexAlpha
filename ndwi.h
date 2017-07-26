#ifndef NDWI_H
#define NDWI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

// #include <QFileInfo>

namespace Ui {
class Ndwi;
}

class Ndwi : public QDialog
{
    Q_OBJECT

public:
    explicit Ndwi(QWidget *parent = 0);
    ~Ndwi();

    void ndwi(std::string green_FilePath, std::string nir_FilePath, std::string out_FilePath);

private slots:
    void on_ndwi_btn_clicked();

    void on_ndwiGREEN_btn_clicked();

    void on_ndwiNIR_btn_clicked();

    void on_ndwiOut_btn_clicked();

private:
    Ui::Ndwi *ui;

    QString greenFilepath;
    QString nirFilepath;
    QString outFilepath;
};

#endif // NDWI_H
