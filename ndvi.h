#ifndef NDVI_H
#define NDVI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

// #include <QFileInfo>

namespace Ui {
class Ndvi;
}

class Ndvi : public QDialog
{
    Q_OBJECT

public:
    explicit Ndvi(QWidget *parent = 0);
    ~Ndvi();

    void ndvi(std::string nir_FilePath, std::string red_FilePath, std::string out_FilePath);

private slots:
    void on_ndvi_btn_clicked();

    void on_ndviRED_btn_clicked();

    void on_ndviNIR_btn_clicked();

    void on_ndviOut_btn_clicked();

private:
    Ui::Ndvi *ui;

    QString nirFilepath;
    QString redFilepath;
    QString outFilepath;
};

#endif // NDVI_H
