#ifndef NDSI_H
#define NDSI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Ndsi;
}

class Ndsi : public QDialog
{
    Q_OBJECT

public:
    explicit Ndsi(QWidget *parent = 0);
    ~Ndsi();

     void ndsi(std::string swir1_FilePath, std::string swir2_FilePath, std::string out_FilePath);

private slots:
     void on_ndsiSWIR1_btn_clicked();

     void on_ndsiSWIR2_btn_clicked();

     void on_ndsiOut_btn_clicked();

     void on_ndsi_btn_clicked();

private:
    Ui::Ndsi *ui;

    QString swir1Filepath;
    QString swir2Filepath;
    QString outFilepath;
};

#endif // NDSI_H
