#ifndef SAVI_H
#define SAVI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Savi;
}

class Savi : public QDialog
{
    Q_OBJECT

public:
    explicit Savi(QWidget *parent = 0);
    ~Savi();

    void savi(std::string red_FilePath, std::string nir_FilePath, std::string out_FilePath);

private slots:

    void on_saviRED_btn_clicked();

    void on_saviNIR_btn_clicked();

    void on_saviOut_btn_clicked();

    void on_savi_btn_clicked();

private:
    Ui::Savi *ui;

    QString redFilepath;
    QString nirFilepath;
    QString outFilepath;
};

#endif // SAVI_H
