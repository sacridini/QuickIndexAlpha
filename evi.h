#ifndef EVI_H
#define EVI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Evi;
}

class Evi : public QDialog
{
    Q_OBJECT

public:
    explicit Evi(QWidget *parent = 0);
    ~Evi();

    void evi(std::string nir_FilePath, std::string red_FilePath, std::string blue_FilePath, std::string out_FilePath);

private slots:
    void on_eviBLUE_btn_clicked();

    void on_eviRED_btn_clicked();

    void on_eviNIR_btn_clicked();

    void on_eviOut_btn_clicked();

    void on_evi_btn_clicked();

private:
    Ui::Evi *ui;

    QString nirFilepath;
    QString redFilepath;
    QString blueFilepath;
    QString outFilepath;
};

#endif // EVI_H
