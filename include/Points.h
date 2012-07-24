#ifndef POINTS_H
#define POINTS_H

#include <iostream>
#include <vector>
#include <Dense>
#include <queue>
#include <list>

using namespace std;
using namespace Eigen;

/*  TODO: Figure out file I/O.
void readPoints(char *, list<RowVectorXd>);
void writePoints(char *, list<RowVectorXd>);
*/

void genPoints(int, int, list<RowVectorXd> *);

#endif // POINTS_H
