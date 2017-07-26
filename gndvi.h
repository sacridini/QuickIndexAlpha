#ifndef GNDVI_H
#define GNDVI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Gndvi;
}

class Gndvi : public QDialog
{
    Q_OBJECT

public:
    explicit Gndvi(QWidget *parent = 0);
    ~Gndvi();

    void gndvi(std::string nir_FilePath, std::string green_FilePath, std::string out_FilePath);

private slots:
    void on_gndviNIR_btn_clicked();

    void on_gndviGREEN_btn_2_clicked();

    void on_gndviOut_btn_clicked();

    void on_gndvi_btn_clicked();

private:
    Ui::Gndvi *ui;

    QString nirFilepath;
    QString greenFilepath;
    QString outFilepath;
};

#endif // GNDVI_H
