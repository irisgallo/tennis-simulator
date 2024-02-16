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
};

#endif // PHYSICSCONTROLS_H
