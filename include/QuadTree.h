#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <Dense>

using namespace std;
using namespace Eigen;

class QuadTree {
    public:
        QuadTree * parent;
        QuadTree * children;
        vector <RowVectorXd> * contents;

        void makeRoot()
            {parent = NULL;};
        QuadTree * moveUp()
            {return parent;};
        QuadTree * moveDown(int n)
            {if(n<0 || n > 3 || children == NULL)
                return NULL;
             return (children + sizeof(QuadTree *) * n);
            }
        void subDivide()
            { children = new QuadTree[4];}
};

vector <RowVectorXd> findCorners(vector <int>);

#endif // QUADTREE_H
