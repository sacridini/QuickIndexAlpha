#ifndef NDGI_H
#define NDGI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Ndgi;
}

class Ndgi : public QDialog
{
    Q_OBJECT

public:
    explicit Ndgi(QWidget *parent = 0);
    ~Ndgi();

    void ndgi(std::string green_FilePath, std::string red_FilePath, std::string out_FilePath);

private slots:
    void on_ndgiGREEN_btn_clicked();

    void on_ndgiRED_btn_clicked();

    void on_ndgiOut_btn_clicked();

    void on_ndgi_btn_clicked();

private:
    Ui::Ndgi *ui;

    QString greenFilepath;
    QString redFilepath;
    QString outFilepath;
};

#endif // NDGI_H
