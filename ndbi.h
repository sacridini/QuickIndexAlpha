#ifndef NDBI_H
#define NDBI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Ndbi;
}

class Ndbi : public QDialog
{
    Q_OBJECT

public:
    explicit Ndbi(QWidget *parent = 0);
    ~Ndbi();

    void ndbi(std::string nir_FilePath, std::string swir_FilePath, std::string out_FilePath);

private slots:
    void on_ndbiNIR_btn_clicked();

    void on_ndbiSWIR_btn_2_clicked();

    void on_ndbiOut_btn_clicked();

    void on_ndbi_btn_clicked();

private:
    Ui::Ndbi *ui;

    QString nirFilepath;
    QString swirFilepath;
    QString outFilepath;
};

#endif // NDBI_H
