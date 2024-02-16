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
