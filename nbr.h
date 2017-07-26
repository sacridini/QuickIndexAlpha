#ifndef NBR_H
#define NBR_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Nbr;
}

class Nbr : public QDialog
{
    Q_OBJECT

public:
    explicit Nbr(QWidget *parent = 0);
    ~Nbr();

    void nbr(std::string nir_FilePath, std::string swir_FilePath, std::string out_FilePath);

private slots:
    void on_nbrNIR_btn_clicked();

    void on_nbrSWIR_btn_2_clicked();

    void on_nbrOut_btn_clicked();

    void on_nbr_btn_clicked();

private:
    Ui::Nbr *ui;

    QString nirFilepath;
    QString swirFilepath;
    QString outFilepath;
};

#endif // NBR_H
