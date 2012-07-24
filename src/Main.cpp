#include <iostream>
#include <iomanip>
#include <Dense>
#include <list>

#include "Field.h"
#include "Interpolation.h"
#include "Integration.h"
#include "Points.h"
#include "QuadTree.h"

#define DIMENSION       2
#define POINTS_TO_GEN   10000
#define POINTS_FILE     "standard.points"
#define DELTA_T         0.1
#define DELTA_X         0.0001

using namespace std;
using namespace Eigen;

int main()
{
    RowVectorXd tempPoint;

    FieldV field;
    field.initialize(DIMENSION);
    field.inputValues();
    initInterpolation(field);
    list<RowVectorXd> pointsList;
    list<RowVectorXd>::iterator pointsIterator;
    genPoints(POINTS_TO_GEN, DIMENSION, &pointsList);
    cout << pointsList.size();
    do{
        for(pointsIterator = pointsList.begin(); pointsIterator != pointsList.end();)
        {
            (*pointsIterator) = integrateStepEuler(*pointsIterator, DELTA_T);
            if(!withinBounds(*pointsIterator))
            {
                pointsIterator = pointsList.erase(pointsIterator);
                cout << "point removed" << endl;
            } else
                pointsIterator++;
        }
        pointsIterator = pointsList.begin();
    }while(!pointsList.empty());
    return 0;
}
