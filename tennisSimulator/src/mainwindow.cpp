#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QResizeEvent>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    physicsControls(new PhysicsControls())
{
    ui->setupUi(this);

    physicsControls->show();
    physicsControls->move(QGuiApplication::primaryScreen()->availableGeometry().center() - this->rect().center() + QPoint(this->width(), 0));

    connect(physicsControls, SIGNAL(sig_setStartStop()),
            ui->mygl, SLOT(slot_setStartStop()));
    connect(physicsControls, SIGNAL(sig_reset()),
            ui->mygl, SLOT(slot_reset()));
    connect(physicsControls, SIGNAL(sig_setPX(double)),
            ui->mygl, SLOT(slot_setPX(double)));
    connect(physicsControls, SIGNAL(sig_setPY(double)),
            ui->mygl, SLOT(slot_setPY(double)));
    connect(physicsControls, SIGNAL(sig_setVX(double)),
            ui->mygl, SLOT(slot_setVX(double)));
    connect(physicsControls, SIGNAL(sig_setVY(double)),
            ui->mygl, SLOT(slot_setVY(double)));
    connect(physicsControls, SIGNAL(sig_setAV(double)),
            ui->mygl, SLOT(slot_setAV(double)));
    connect(physicsControls, SIGNAL(sig_setDebugState(int)),
            ui->mygl, SLOT(slot_setDebugState(int)));

    connect(ui->mygl, SIGNAL(sig_sendPos(double, double)),
            physicsControls, SLOT(slot_setPos(double, double)));
    connect(ui->mygl, SIGNAL(sig_sendVel(double, double)),
            physicsControls, SLOT(slot_setVel(double, double)));
    connect(ui->mygl, SIGNAL(sig_sendAngVel(double)),
            physicsControls, SLOT(slot_setAngVel(double)));
    connect(ui->mygl, SIGNAL(sig_sendHitVelocity(double, double)),
            physicsControls, SLOT(slot_setHitVelocity(double, double)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete physicsControls;
}

void MainWindow::on_actionQuit_triggered()
{
    physicsControls->close();
    QApplication::exit();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    physicsControls->close();
    QMainWindow::closeEvent(e);
}
