#include <iostream>
#include <iomanip>
#include <Dense>
#include <list>
#include <ctime>
#include <fstream>

#include "Field.h"
#include "Interpolation.h"
#include "Integration.h"
#include "Points.h"
#include "QuadTree.h"

#define EXACT_SUBDIVISION           100
#define DIMENSION                   2
#define POINTS_TO_GEN               1000
#define POINTS_FILE                 "points.csv"
#define DELTA_T                     0.1
#define DESIRED_ACCURACY            0.001
#define VEC_RANGE                   1
#define NUM_TRIALS                  1000
#define LINEARITYFILE               "linearity.csv"
#define DATAFILE                    "data.csv"
#define EULERFILE                   "euler.csv"
#define RK2FILE                     "RK2.csv"
#define RK4FILE                     "RK4.csv"
#define GEN_RANDOM                  true

using namespace std;
using namespace Eigen;

int main()
{
    srand(time(NULL));

    RowVectorXd tempPoint;

    int numTrials = NUM_TRIALS, methodUsed, interpolations, integrationCost;
    double runningTotalError, runningTotalEulerError, runningTotalRK2Error, runningTotalRK4Error;
    double maxError, maxEulerError, maxRK2Error, maxRK4Error;
    double runningTotalLinearity, tempLinearity, tempError;
    double eulerThreshold, RK2threshold;
    FieldV field;
    ofstream outLinearity (LINEARITYFILE);
    ofstream outData (DATAFILE);
    ofstream outEulerError (EULERFILE);
    ofstream outRK2Error (RK2FILE);
    ofstream outRK4Error (RK4FILE);
    RowVectorXd (*integrateMethod)(RowVectorXd, double);

    field.initialize(DIMENSION);
    field.genField(sqrt(VEC_RANGE));
    initInterpolation(field);

    list<RowVectorXd> pointsList;
    list<RowVectorXd>::iterator pointsIterator;

    eulerThreshold = EulerLinearityThreshold(DESIRED_ACCURACY, DELTA_T);
    RK2threshold = RK2LinearityThreshold(DESIRED_ACCURACY, DELTA_T);
    do{
        if(GEN_RANDOM)
            { field.genField(VEC_RANGE); }
        else
            { field.inputValues(); }
        initInterpolation(field);

        genPoints(POINTS_TO_GEN, DIMENSION, &pointsList);

        runningTotalError = 0;
        runningTotalEulerError = 0;
        runningTotalRK2Error = 0;
        runningTotalRK4Error = 0;
        runningTotalLinearity = 0;
        maxError = 0;
        maxEulerError = 0;
        maxRK2Error = 0;
        maxRK4Error = 0;
        interpolations = 0;

        // clock_t first = clock();
        for(pointsIterator = pointsList.begin(); pointsIterator != pointsList.end(); pointsIterator++)
        {
            tempError = findRKF45Error(*pointsIterator, DELTA_T);
            tempLinearity = field.calcLinearity(*pointsIterator, DELTA_T);
            // outData << (*pointsIterator)[0] << ", " << (*pointsIterator)[1] << ", " << tempLinearity << endl;
            runningTotalError += tempError;
            runningTotalLinearity += tempLinearity;
        }
        runningTotalError /= POINTS_TO_GEN;
        runningTotalLinearity /= POINTS_TO_GEN;
        // cout << clock() - first << endl;


        // system("pause");

        cout << "Linearity = " << runningTotalLinearity << ": ";
        if(runningTotalLinearity < eulerThreshold)
        {
            cout << "Using Euler." << endl;
            integrateMethod = &integrateStepEuler;
            methodUsed = 0;
            integrationCost = EULERCOST * POINTS_TO_GEN;
        } else if(runningTotalLinearity < RK2threshold) {
            cout << "Using RK2." << endl;
            integrateMethod = &integrateStepRK2;
            methodUsed = 1;
            integrationCost = RK2COST * POINTS_TO_GEN;
        } else {
            cout << "Using RK4." << endl;
            integrateMethod = &integrateStepRK4;
            methodUsed = 2;
            integrationCost = RK4COST * POINTS_TO_GEN;
        }

        outLinearity << runningTotalLinearity << ", " << runningTotalError << endl;
        cout << "Desired threshold: " << eulerThreshold << "\t" << RK2threshold << endl;

        // system("pause");

        runningTotalError = 0;
        maxError = 0;

        for(pointsIterator = pointsList.begin(); pointsIterator != pointsList.end(); pointsIterator++)
        {

            tempPoint = *pointsIterator;
            for(int i = 0; i < EXACT_SUBDIVISION; i++)
                tempPoint = integrateStepRK4(tempPoint, DELTA_T/EXACT_SUBDIVISION);
            // cout << "Exact: " << tempPoint << endl;

            tempError = (tempPoint - integrateMethod(*pointsIterator, DELTA_T)).norm();
            if(tempError > maxError)
                maxError = tempError;
            runningTotalError += tempError;


            tempError = (tempPoint - integrateStepEuler(*pointsIterator, DELTA_T)).norm();
            if(tempError > maxEulerError)
                maxEulerError = tempError;
            runningTotalEulerError += tempError;
            // cout << "\t Euler: " << tempError << endl;

            tempError = (tempPoint - integrateStepRK2(*pointsIterator, DELTA_T)).norm();
            if(tempError > maxRK2Error)
                maxRK2Error = tempError;
            runningTotalRK2Error += tempError;
            // cout << "\t RK2: " << tempError << endl;

            tempError = (tempPoint - integrateStepRK4(*pointsIterator, DELTA_T)).norm();
            if(tempError > maxRK4Error)
                maxRK4Error = tempError;
            runningTotalRK4Error += tempError;
            // cout << "\t RK4: " << tempError << endl;

        }

        runningTotalError /= POINTS_TO_GEN;
        runningTotalEulerError /= POINTS_TO_GEN;
        runningTotalRK2Error /= POINTS_TO_GEN;
        runningTotalRK4Error /= POINTS_TO_GEN;
        cout << "Average Error: \t"<< runningTotalError << "\t Maximum Error: \t" << maxError << endl;
        cout << "Saved " << RK4COST * POINTS_TO_GEN - integrationCost << " interpolation operations." << endl;

        outEulerError << runningTotalLinearity << ", " << runningTotalEulerError /* << ", " << maxEulerError */ << endl;
        outRK2Error << runningTotalLinearity << ", " << runningTotalRK2Error /* << ", " << maxRK2Error */ << endl;
        outRK4Error << runningTotalLinearity << ", " << runningTotalRK4Error /* << ", " << maxRK4Error */ << endl;

        cout << numTrials << endl;
    }while(--numTrials);

    /*
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
    */

    outLinearity.close();
    outData.close();

    return 0;
}
