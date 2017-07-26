#ifndef GVMI_H
#define GVMI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Gvmi;
}

class Gvmi : public QDialog
{
    Q_OBJECT

public:
    explicit Gvmi(QWidget *parent = 0);
    ~Gvmi();

    void gvmi(std::string nir_FilePath, std::string swir_FilePath, std::string out_FilePath);

private slots:
    void on_gvmiNIR_btn_clicked();

    void on_gvmiSWIR_btn_clicked();

    void on_gvmiiOut_btn_clicked();

    void on_gvmi_btn_clicked();

private:
    Ui::Gvmi *ui;

    QString nirFilepath;
    QString swirFilepath;
    QString outFilepath;
};

#endif // GVMI_H
