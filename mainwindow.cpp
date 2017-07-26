#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Quick Index Alpha");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNDVI_triggered()
{
    ndvi = new Ndvi(this);
    ndvi->show();
}

void MainWindow::on_actionNDWI_triggered()
{
    ndwi = new Ndwi(this);
    ndwi->show();
}

void MainWindow::on_actionSAVI_triggered()
{
    savi = new Savi(this);
    savi->show();
}

void MainWindow::on_actionGOSAVI_triggered()
{
    gosavi = new Gosavi(this);
    gosavi->show();
}

void MainWindow::on_actionNDGI_triggered()
{
    ndgi = new Ndgi(this);
    ndgi->show();
}

void MainWindow::on_actionNDBI_triggered()
{
    ndbi = new Ndbi(this);
    ndbi->show();
}

void MainWindow::on_actionNBR_triggered()
{
    nbr = new Nbr(this);
    nbr->show();
}

void MainWindow::on_actionGDVI_triggered()
{
    gdvi = new Gdvi(this);
    gdvi->show();
}

void MainWindow::on_actionEVI_2_triggered()
{
    evi2 = new Evi2(this);
    evi2->show();
}

void MainWindow::on_actionGNDVI_triggered()
{
    gndvi = new Gndvi(this);
    gndvi->show();
}

void MainWindow::on_actionCIGreen_triggered()
{
    cigreen = new CIGreen(this);
    cigreen->show();
}

void MainWindow::on_actionNDSI_triggered()
{
    ndsi = new Ndsi(this);
    ndsi->show();
}

void MainWindow::on_actionTVI_triggered()
{
    tvi = new Tvi(this);
    tvi->show();
}

void MainWindow::on_actionEVI_triggered()
{
    evi = new Evi(this);
    evi->show();
}

void MainWindow::on_actionGRVI_triggered()
{
    grvi = new Grvi(this);
    grvi->show();
}

void MainWindow::on_actionLAI_triggered()
{
    lai = new Lai(this);
    lai->show();
}

void MainWindow::on_actionGVMI_triggered()
{
    gvmi = new Gvmi(this);
    gvmi->show();
}

void MainWindow::on_actionGLI_triggered()
{
    gli = new Gli(this);
    gli->show();
}

void MainWindow::on_actionLog_Ratio_triggered()
{
    logratio = new logRatio(this);
    logratio->show();
}

void MainWindow::on_actionPan_NDVI_triggered()
{
    panndvi = new panNDVI(this);
    panndvi->show();
}
