#ifndef EVI2_H
#define EVI2_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Evi2;
}

class Evi2 : public QDialog
{
    Q_OBJECT

public:
    explicit Evi2(QWidget *parent = 0);
    ~Evi2();

    void evi2(std::string nir_FilePath, std::string red_FilePath, std::string out_FilePath);

private slots:
    void on_evi2RED_btn_clicked();

    void on_evi2NIR_btn_clicked();

    void on_evi2Out_btn_clicked();

    void on_evi2_btn_clicked();

private:
    Ui::Evi2 *ui;

    QString nirFilepath;
    QString redFilepath;
    QString outFilepath;
};

#endif // EVI2_H
