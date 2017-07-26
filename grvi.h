#ifndef GRVI_H
#define GRVI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Grvi;
}

class Grvi : public QDialog
{
    Q_OBJECT

public:
    explicit Grvi(QWidget *parent = 0);
    ~Grvi();

    void grvi(std::string nir_FilePath, std::string green_FilePath, std::string out_FilePath);

private slots:
    void on_grviNIR_btn_clicked();

    void on_grviGREEN_btn_2_clicked();

    void on_grviOut_btn_clicked();

    void on_grvi_btn_clicked();

private:
    Ui::Grvi *ui;

    QString nirFilepath;
    QString greenFilepath;
    QString outFilepath;
};

#endif // GRVI_H
