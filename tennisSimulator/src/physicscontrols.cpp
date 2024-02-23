#include "physicscontrols.h"
#include "ui_physicscontrols.h"

PhysicsControls::PhysicsControls(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhysicsControls)
{
    ui->setupUi(this);

    connect(ui->startstop, SIGNAL(clicked()), this, SLOT(slot_setStartStop()));
}

void PhysicsControls::slot_setStartStop()
{
    emit sig_setStartStop();
}

PhysicsControls::~PhysicsControls()
{
    delete ui;
}
