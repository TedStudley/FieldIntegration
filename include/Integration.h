#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <Dense>

RowVectorXd integrateEuler(RowVectorXd, double, double);
RowVectorXd integrateStepEuler(RowVectorXd, double);

RowVectorXd integrateRK2(RowVectorXd, double, double);
RowVectorXd integrateStepRK2(RowVectorXd, double);

RowVectorXd integrateRK4(RowVectorXd, double, double);
RowVectorXd integrateStepRK4(RowVectorXd, double);

RowVectorXd integrateRKF45(RowVectorXd, double, double);
RowVectorXd integrateStepRKF45(RowVectorXd, double);

#endif // INTEGRATION_H
