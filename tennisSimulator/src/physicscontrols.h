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


signals:
    void sig_setStartStop();
    void sig_reset();
    void sig_setPX(double px);
    void sig_setPY(double py);
    void sig_setVX(double vx);
    void sig_setVY(double vy);
};

#endif // PHYSICSCONTROLS_H
