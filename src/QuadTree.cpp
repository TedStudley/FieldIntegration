#include "QuadTree.h"

#include <vector>
#include <Eigen>

using namespace Eigen;
using namespace std;

vector <RowVectorXd> findCorners(vector <int> quadMap)
{
    RowVectorXd corners;
    corners << 0, 0, 1, 1;
    RowVectorXd tempCorners;
    vector <RowVectorXd> finalCorners;

    vector<int>::size_type sz = quadMap.size();

    for(int i = 0; i < sz; i++)
        switch(quadMap[i])
        {
            case 0:
                tempCorners.resize(4);
                tempCorners << corners[0], corners[1], (corners[3] + corners[0]) / 2, (corners[4] + corners[1]) / 2;
                corners = tempCorners;
                break;
            case 1:
                tempCorners.resize(4);
                tempCorners << corners[0], (corners[4] + corners[1]) / 2, (corners[3] + corners[0]) / 2, corners[4];
                corners = tempCorners;
                break;
            case 2:
                tempCorners.resize(4);
                tempCorners << (corners[3] + corners[0]) / 2, corners[1], corners[3], (corners[4] + corners[1]) / 2;
                corners = tempCorners;
                break;
            case 3:
                tempCorners.resize(4);
                tempCorners << (corners[3] + corners[0]) / 2, (corners[4] + corners[1]) / 2, corners[0], corners[1];
                corners = tempCorners;
                break;
         }

    finalCorners.push_back({corners[0], corners[1]});
    finalCorners.push_back({corners[2], corners[3]});

    return finalCorners;
}
