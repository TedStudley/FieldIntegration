#ifndef FIELD_H
#define FIELD_H

#include <Vector>
#include <Dense>

using namespace std;
using namespace Eigen;



/*
    A field is a C++ vector, containing a point to vector of floats (from the Eigen library) for
    each vertex of the field which it represents. Vertices are encoded into the vector as a sort
    of bitfield. For example:
        (0,1,1)     ==> 0x011
        (1,1,0,1)   ==> 0x1101
        (1,1)       ==> 0x11
    While a field can be of arbitrary dimension, the functions which act on it might not be. use
    caution!
*/
class FieldV
{
    public:
        vector<RowVectorXd> coords;
        int dim();
        void initialize(int);
        void inputValues();
        void genField(int);
        double calcCurl();
        double calcLinearity(RowVectorXd, double);
};

bool withinBounds(RowVectorXd);

double EulerLinearityThreshold(double, double);
double RK2LinearityThreshold(double, double);

#endif // FIELD_H
