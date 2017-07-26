#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ndvi.h"
#include "ndwi.h"
#include "savi.h"
#include "gosavi.h"
#include "ndgi.h"
#include "ndbi.h"
#include "nbr.h"
#include "gdvi.h"
#include "evi.h"
#include "evi2.h"
#include "gndvi.h"
#include "cigreen.h"
#include "ndsi.h"
#include "tvi.h"
#include "grvi.h"
#include "lai.h"
#include "gvmi.h"
#include "gli.h"
#include "logratio.h"
#include "panndvi.h"


using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNDVI_triggered();

    void on_actionNDWI_triggered();

    void on_actionSAVI_triggered();

    void on_actionGOSAVI_triggered();

    void on_actionNDGI_triggered();

    void on_actionNDBI_triggered();

    void on_actionNBR_triggered();

    void on_actionGDVI_triggered();

    void on_actionEVI_2_triggered();

    void on_actionGNDVI_triggered();

    void on_actionCIGreen_triggered();

    void on_actionNDSI_triggered();

    void on_actionTVI_triggered();

    void on_actionEVI_triggered();

    void on_actionGRVI_triggered();

    void on_actionLAI_triggered();

    void on_actionGVMI_triggered();

    void on_actionGLI_triggered();

    void on_actionLog_Ratio_triggered();

    void on_actionPan_NDVI_triggered();

private:
    Ui::MainWindow *ui;
    Ndvi *ndvi;
    Ndwi *ndwi;
    Savi *savi;
    Gosavi *gosavi;
    Ndgi *ndgi;
    Ndbi *ndbi;
    Nbr *nbr;
    Gdvi *gdvi;
    Evi2 *evi2;
    Gndvi *gndvi;
    CIGreen *cigreen;
    Ndsi *ndsi;
    Tvi *tvi;
    Evi *evi;
    Grvi *grvi;
    Lai *lai;
    Gvmi *gvmi;
    Gli *gli;
    logRatio *logratio;
    panNDVI *panndvi;
};

#endif // MAINWINDOW_H
