#ifndef PANNDVI_H
#define PANNDVI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

#if 1
    #define LOG(x) ui->textBrowser->append(x);
#endif

namespace Ui {
class panNDVI;
}

class panNDVI : public QDialog
{
    Q_OBJECT

public:
    explicit panNDVI(QWidget *parent = 0);
    ~panNDVI();

    void panndvi(std::string blue_FilePath, std::string green_FilePath, std::string red_FilePath, std::string nir_FilePath, std::string out_FilePath);

private slots:
    void on_panNdviBLUE_btn_clicked();

    void on_panNdviGREEN_btn_2_clicked();

    void on_panNdviRED_btn_clicked();

    void on_panNdviNIR_btn_clicked();

    void on_panNdviOut_btn_2_clicked();

    void on_panNdvi_btn_clicked();

private:
    Ui::panNDVI *ui;

    QString blueFilepath;
    QString greenFilepath;
    QString redFilepath;
    QString nirFilepath;
    QString outFilepath;
};

#endif // PANNDVI_H
