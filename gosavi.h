#ifndef GOSAVI_H
#define GOSAVI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Gosavi;
}

class Gosavi : public QDialog
{
    Q_OBJECT

public:
    explicit Gosavi(QWidget *parent = 0);
    ~Gosavi();

     void gosavi(std::string green_FilePath, std::string nir_FilePath, std::string out_FilePath);

private slots:
     void on_gosaviGREEN_btn_clicked();

     void on_gosaviNIR_btn_clicked();

     void on_gosaviOut_btn_clicked();

     void on_gosavi_btn_clicked();

private:
    Ui::Gosavi *ui;

    QString greenFilepath;
    QString nirFilepath;
    QString outFilepath;
};

#endif // GOSAVI_H
