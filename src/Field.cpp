#include <iostream>
#include <Dense>

#include "Field.h"
#include "Error.h"

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

bool withinBounds(RowVectorXd x)
{
    bool isBounded = true;
    for(int i = 0; i < x.size(); i++)
        if((x[i] < 0) || (x[i] > 1))
            isBounded = false;
    return isBounded;
}
