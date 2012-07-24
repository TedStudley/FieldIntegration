#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <Vector>
#include <Dense>

#include "Field.h"

using namespace std;
using namespace Eigen;

/*
    Interpolation data is stored as a vector of coefficients. The coefficients are stored as
    vectors, which is useful for parallelizing the interpolation operation.
*/
typedef RowVectorXd coefficient_t;
typedef vector<coefficient_t> interpolation_t;

void initInterpolation(FieldV);
RowVectorXd interpolate(RowVectorXd);

#endif // INTERPOLATION_H
