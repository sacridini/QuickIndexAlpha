#ifndef CIGREEN_H
#define CIGREEN_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class CIGreen;
}

class CIGreen : public QDialog
{
    Q_OBJECT

public:
    explicit CIGreen(QWidget *parent = 0);
    ~CIGreen();

    void cigreen(std::string nir_FilePath, std::string green_FilePath, std::string out_FilePath);

private slots:
    void on_cigreenNIR_btn_clicked();

    void on_cigreenGREEN_btn_2_clicked();

    void on_cigreenOut_btn_clicked();

    void on_cigreen_btn_clicked();

private:
    Ui::CIGreen *ui;

    QString nirFilepath;
    QString greenFilepath;
    QString outFilepath;
};

#endif // CIGREEN_H
