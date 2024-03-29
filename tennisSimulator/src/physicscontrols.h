#ifndef PHYSICSCONTROLS_H
#define PHYSICSCONTROLS_H

#include <QWidget>

namespace Ui {
class PhysicsControls;
}

class PhysicsControls : public QWidget
{
    Q_OBJECT

public:
    explicit PhysicsControls(QWidget *parent = 0);
    ~PhysicsControls();

    Ui::PhysicsControls *ui;

public slots:
    void slot_setStartStop();
    void slot_reset();
    void slot_setPX(double px);
    void slot_setPY(double py);
    void slot_setVX(double vx);
    void slot_setVY(double vy);
    void slot_setAV(double av);
    void slot_setDebugState(int state);

    void slot_setPos(double px, double py);
    void slot_setVel(double vx, double vy);
    void slot_setAngVel(double av);
    void slot_setHitVelocity(double hx, double hy);


signals:
    void sig_setStartStop();
    void sig_reset();
    void sig_setPX(double px);
    void sig_setPY(double py);
    void sig_setVX(double vx);
    void sig_setVY(double vy);
    void sig_setAV(double av);
    void sig_setDebugState(int state);
};

#endif // PHYSICSCONTROLS_H
