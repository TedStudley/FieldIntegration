#define E_M     2.718281828459045

#include <iostream>
#include <Dense>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "Field.h"
#include "Error.h"
#include "Interpolation.h"

using namespace std;
using namespace Eigen;

int FieldV::dim()
{
    return (coords.size());
}

void FieldV::initialize(int d)
{
    coords.resize(pow(2,d));
    for(unsigned int i = 0; i < coords.size(); i++)
    {
        coords[i].resize(d);
        coords[i].setZero(d);
    }
    return;
}

void FieldV::inputValues()
{
    switch((coords[0]).size())
    {
        case 2:
        case 3:
            for(int i = 0; i < coords.size(); i++)
                for(int j = 0; j < (coords[i]).size(); j++)
                    cin >> (coords[i])[j];
            break;
        default:
            cerr << FIELD_DIM_UNSUP_ERR;
            exit(FIELD_DIM_UNSUP);
    };
}

void FieldV::genField(int range)
{
    switch((coords[0]).size())
    {
        double theta, alpha;
        case 2:
        case 3:
            for(int i = 0; i < coords.size(); i++)
            {
                theta = ((double) rand() / RAND_MAX) * M_PI;
                alpha = ((double) rand() / RAND_MAX) * range;
                (coords[i])[0] = alpha * cos(theta);
                (coords[i])[1] = alpha * sin(theta);
            }
            break;
        default:
            cerr << FIELD_DIM_UNSUP_ERR;
            exit(FIELD_DIM_UNSUP);
    };
}

double FieldV::calcLinearity(RowVectorXd x, double deltaT)
{
    RowVectorXd directionalDerivative;
    directionalDerivative.resize(2);
    RowVectorXd currVec = interpolate(x);
    RowVectorXd nextVec = interpolate(x + currVec);

    return sqrt((nextVec.norm() * currVec.norm() * (acos(currVec.normalized().dot(nextVec.normalized())))));
}

bool withinBounds(RowVectorXd x)
{
    bool isBounded = true;
    for(int i = 0; i < x.size(); i++)
        if((x[i] < 0) || (x[i] > 1))
            isBounded = false;
    return isBounded;
}

double EulerLinearityThreshold(double desiredError, double deltaT)
{
    return pow((exp(desiredError) - 1) / (exp(0.7372941446637714 * pow(deltaT, 2)) - 1), (double) 2/3);
}

double RK2LinearityThreshold(double desiredError, double deltaT)
{
    return pow((exp(desiredError) - 1) / (exp(0.2786146996428321 * pow(deltaT,3)) - 1), (double) 2/3);
}
