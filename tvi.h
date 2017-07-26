#ifndef TVI_H
#define TVI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>
#include "math.h"


namespace Ui {
class Tvi;
}

class Tvi : public QDialog
{
    Q_OBJECT

public:
    explicit Tvi(QWidget *parent = 0);
    ~Tvi();

    void tvi(std::string nir_FilePath, std::string red_FilePath, std::string out_FilePath);

private slots:
    void on_tviRED_btn_clicked();

    void on_tviNIR_btn_clicked();

    void on_tviOut_btn_clicked();

    void on_tvi_btn_clicked();

private:
    Ui::Tvi *ui;

    QString nirFilepath;
    QString redFilepath;
    QString outFilepath;
};

#endif // TVI_H
