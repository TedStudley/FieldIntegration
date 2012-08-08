#include <Dense>
#include <iostream>

#include "Interpolation.h"
#include "Integration.h"
#include "Field.h"

RowVectorXd integrateEuler(RowVectorXd initial, double delta_t, double final_t)
{
    RowVectorXd x = initial;
    double t = 0;
    while(t < final_t && withinBounds(x))
    {
        x = integrateStepEuler(x, delta_t);
        t += delta_t;
    }
    return x;
}

RowVectorXd integrateStepEuler(RowVectorXd initial, double delta_t)
{
    RowVectorXd x = initial;
    x += interpolate(x) * delta_t;
    return x;
}

void integrateInPlaceStepEuler(RowVectorXd * point, double delta_t)
{
    (*point) += interpolate(*point) * delta_t;
}

RowVectorXd integrateRK2(RowVectorXd initial, double delta_t, double final_t)
{
    RowVectorXd x = initial;
    double t = 0;
    RowVectorXd k1, k2;
    while(t < final_t && withinBounds(x))
    {
        x = integrateStepRK2(x, delta_t);
        t += delta_t;
    }
    return x;
}

RowVectorXd integrateStepRK2(RowVectorXd initial, double delta_t)
{
    RowVectorXd x = initial;
    RowVectorXd k1, k2;
    k1 = delta_t * interpolate(x);
    k2 = delta_t * interpolate(x + k1 / 2);
    x += k2;
    return x;
}

RowVectorXd integrateRK4(RowVectorXd initial, double delta_t, double final_t)
{
    RowVectorXd x = initial;
    double t = 0;
    RowVectorXd k1, k2, k3, k4;
    while(t < final_t && withinBounds(x))
    {
        // cout << t << ": " << x << endl;
        x = integrateStepRK4(x, delta_t);
        t += delta_t;
    }
    return x;
}

RowVectorXd integrateStepRK4(RowVectorXd initial, double delta_t)
{
    RowVectorXd x = initial;
    RowVectorXd k1, k2, k3, k4;
    k1 = delta_t * interpolate(x);
    k2 = delta_t * interpolate(x + k1 / 2);
    k3 = delta_t * interpolate(x + k2 / 2);
    k4 = delta_t * interpolate(x + k3);
    x += k1 / 6 + k2 / 3 + k3 / 3 + k4 / 6;
    return x;
}

RowVectorXd integrateRKF45(RowVectorXd initial, double delta_t, double final_t)
{
    RowVectorXd x = initial;
    double t = 0;
    while(t < final_t && withinBounds(x))
    {
        x = integrateStepRKF45(x, delta_t);
        t += delta_t;
    }
    return x;
}

RowVectorXd integrateStepRKF45(RowVectorXd initial, double delta_t)
{
    RowVectorXd x = initial;
    RowVectorXd k1, k2, k3, k4, k5, k6;
    k1 = delta_t * interpolate(x);
    k2 = delta_t * interpolate(x + k1 / 4);
    k3 = delta_t * interpolate(x + 3 * k1 / 32 + 9 * k2 / 32);
    k4 = delta_t * interpolate(x + 1932 * k1 / 2197 - 7200 * k2 / 2197 + 7296 * k3 / 2197);
    k5 = delta_t * interpolate(x + 439 * k1 / 216 - 8 * k2 + 3680 * k3 / 513 - 845 * k4 / 4103);
    k6 = delta_t * interpolate(x - 8 * k1 / 27 + 2 * k2 - 3544 * k3 / 2565 + 1859 * k4 / 4104 - 11 * k5 / 40);
    x += 16 * k1 / 135 + 6656 * k3 / 12825 + 28561 * k4 / 56430 - 9 * k5 / 50 + 2 * k6 / 55;
    return x;
}

double findRKF45Error(RowVectorXd initial, double delta_t)
{
    RowVectorXd x4 = initial;
    RowVectorXd x5 = initial;
    RowVectorXd final;
    RowVectorXd k1, k2, k3, k4, k5, k6;
    k1 = delta_t * interpolate(initial);
    k2 = delta_t * interpolate(initial + k1 / 4);
    k3 = delta_t * interpolate(initial + 3 * k1 / 32 + 9 * k2 / 32);
    k4 = delta_t * interpolate(initial + 1932 * k1 / 2197 - 7200 * k2 / 2197 + 7296 * k3 / 2197);
    k5 = delta_t * interpolate(initial + 439 * k1 / 216 - 8 * k2 + 3680 * k3 / 513 - 845 * k4 / 4103);
    k6 = delta_t * interpolate(initial - 8 * k1 / 27 + 2 * k2 - 3544 * k3 / 2565 + 1859 * k4 / 4104 - 11 * k5 / 40);
    x4 += 25 * k1 / 216 + 1408 * k3 / 2565 + 2197 * k4 / 4104 - k5 / 5;
    x5 += 16 * k1 / 135 + 6656 * k3 / 12825 + 28561 * k4 / 56430 - 9 * k5 / 50 + 2 * k6 / 55;

    final = x5-x4;

    return final.norm();
}
