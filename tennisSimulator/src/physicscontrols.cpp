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
    connect(ui->avSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(slot_setAV(double)));
}


void PhysicsControls::slot_setStartStop() {

    emit sig_setStartStop();
}

void PhysicsControls::slot_reset() {

    emit sig_reset();
}

void PhysicsControls::slot_setPX(double px) {

    emit sig_setPX(px);
}

void PhysicsControls::slot_setPY(double py) {

    emit sig_setPY(py);
}

void PhysicsControls::slot_setVX(double vx) {

    emit sig_setVX(vx);
}

void PhysicsControls::slot_setVY(double vy) {

    emit sig_setVY(vy);
}

void PhysicsControls::slot_setAV(double av) {

    emit sig_setAV(av);
}

void PhysicsControls::slot_setPos(double px, double py) {

    ui->pxSpinBox->setValue(px);
    ui->pySpinBox->setValue(py);
}

void PhysicsControls::slot_setVel(double vx, double vy) {

    ui->vxSpinBox->setValue(vx);
    ui->vySpinBox->setValue(vy);
}

void PhysicsControls::slot_setAngVel(double av) {

    ui->avSpinBox->setValue(av);
}

void PhysicsControls::slot_setHitVelocity(double hx, double hy) {

    ui->hxSpinBox->setValue(hx);
    ui->hySpinBox->setValue(hy);
}

PhysicsControls::~PhysicsControls() {

    delete ui;
}
