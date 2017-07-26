#ifndef LAI_H
#define LAI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Lai;
}

class Lai : public QDialog
{
    Q_OBJECT

public:
    explicit Lai(QWidget *parent = 0);
    ~Lai();

    void lai(std::string nir_FilePath, std::string red_FilePath, std::string blue_FilePath, std::string outEvi_FilePath, std::string outLai_FilePath);

private slots:
    void on_laiBLUE_btn_clicked();

    void on_laiRED_btn_clicked();

    void on_laiNIR_btn_clicked();

    void on_eviOut_btn_clicked();

    void on_laiOut_btn_clicked();

    void on_lai_btn_clicked();

private:
    Ui::Lai *ui;

    QString nirFilepath;
    QString redFilepath;
    QString blueFilepath;
    QString outEviFilepath;
    QString outLaiFilepath;
};

#endif // LAI_H
