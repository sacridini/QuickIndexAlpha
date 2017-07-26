#ifndef LOGRATIO_H
#define LOGRATIO_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

#if 1
    #define LOG(x) ui->textBrowser->append(x);
#endif

namespace Ui {
class logRatio;
}

class logRatio : public QDialog
{
    Q_OBJECT

public:
    explicit logRatio(QWidget *parent = 0);
    ~logRatio();

    void logratio(std::string nir_FilePath, std::string red_FilePath, std::string out_FilePath);

private slots:
    void on_logRatioRED_btn_clicked();

    void on_logRatioNIR_btn_clicked();

    void on_logRatioOut_btn_clicked();

    void on_logRatio_btn_clicked();

private:
    Ui::logRatio *ui;

    QString nirFilepath;
    QString redFilepath;
    QString outFilepath;
};

#endif // LOGRATIO_H
