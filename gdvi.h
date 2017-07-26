#ifndef GDVI_H
#define GDVI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Gdvi;
}

class Gdvi : public QDialog
{
    Q_OBJECT

public:
    explicit Gdvi(QWidget *parent = 0);
    ~Gdvi();

    void gdvi(std::string nir_FilePath, std::string green_FilePath, std::string out_FilePath);

private slots:
    void on_gdviNIR_btn_clicked();

    void on_gdviGREEN_btn_2_clicked();

    void on_gdviOut_btn_clicked();

    void on_gdvi_btn_clicked();

private:
    Ui::Gdvi *ui;

    QString nirFilepath;
    QString greenFilepath;
    QString outFilepath;
};

#endif // GDVI_H
