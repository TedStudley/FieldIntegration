#ifndef QUADTREE_H
#define QUADTREE_H

#include <list>

using namespace std;

class QuadTree {
    public:
        QuadTree * parent;
        QuadTree * children[4];
        list <RowVectorXd> * contents;

        void makeRoot()
            {parent = NULL;};
        QuadTree * moveUp()
            {return parent;};
        QuadTree * moveDown(int n)
            {if(n<0 || n > 3 || children == NULL)
                return NULL;
             return children[n];
            }
        void subDivide()
            {
                children = new QuadTree*[4];
            }

};

#endif // QUADTREE_H
