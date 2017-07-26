#ifndef GLI_H
#define GLI_H

#include <QString>
#include <QFileDialog>
#include <gdal.h>
#include <gdal_priv.h>
#include <string>

namespace Ui {
class Gli;
}

class Gli : public QDialog
{
    Q_OBJECT

public:
    explicit Gli(QWidget *parent = 0);
    ~Gli();

    void gli(std::string blue_FilePath, std::string green_FilePath, std::string red_FilePath, std::string out_FilePath);

private slots:
    void on_gliBLUE_btn_clicked();

    void on_gliGreen_btn_clicked();

    void on_gliRED_btn_clicked();

    void on_gli_btn_clicked();

private:
    Ui::Gli *ui;

    QString blueFilepath;
    QString greenFilepath;
    QString redFilepath;
    QString outFilepath;
};

#endif // GLI_H
