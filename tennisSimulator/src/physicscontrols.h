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

signals:
    void sig_setStartStop();
};

#endif // PHYSICSCONTROLS_H
