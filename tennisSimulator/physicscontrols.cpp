#include "physicscontrols.h"
#include "ui_physicscontrols.h"

PhysicsControls::PhysicsControls(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhysicsControls)
{
    ui->setupUi(this);
}

PhysicsControls::~PhysicsControls()
{
    delete ui;
}
