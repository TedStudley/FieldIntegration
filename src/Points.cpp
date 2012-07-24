#include <iostream>
#include <Dense>
#include <fstream>
#include <list>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "Points.h"

using namespace std;
using namespace Eigen;

void genPoints(int numPoints, int pointDimension, list<RowVectorXd> * pointsList)
{
    srand(time(NULL));

    double tempDouble;
    RowVectorXd tempVector;
    tempVector.resize(pointDimension);
    for(int i = 0; i < numPoints; i++)
    {
        for(int j = 0; j < pointDimension; j++)
        {
            tempDouble = ((double) rand() / RAND_MAX);
            tempVector[j] = tempDouble;
        }
        pointsList->push_back(tempVector);
    }
    return;
}
