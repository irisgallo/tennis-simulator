#include "physicscontrols.h"
#include "ui_physicscontrols.h"

PhysicsControls::PhysicsControls(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhysicsControls)
{
    ui->setupUi(this);

    connect(ui->startstopButton, SIGNAL(clicked()),
            this, SLOT(slot_setStartStop()));
    connect(ui->resetButton, SIGNAL(clicked()),
            this, SLOT(slot_reset()));

    connect(ui->pxSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(slot_setPX(double)));
    connect(ui->pySpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(slot_setPY(double)));
    connect(ui->vxSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(slot_setVX(double)));
    connect(ui->vySpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(slot_setVY(double)));
}

void PhysicsControls::slot_setStartStop()
{
    emit sig_setStartStop();
}

void PhysicsControls::slot_reset()
{
    emit sig_reset();
}

void PhysicsControls::slot_setPX(double px)
{
    emit sig_setPX(px);
}

void PhysicsControls::slot_setPY(double py)
{
    emit sig_setPY(py);
}

void PhysicsControls::slot_setVX(double vx)
{
    emit sig_setVX(vx);
}

void PhysicsControls::slot_setVY(double vy)
{
    emit sig_setVY(vy);
}

PhysicsControls::~PhysicsControls()
{
    delete ui;
}
