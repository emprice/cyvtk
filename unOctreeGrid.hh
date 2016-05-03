#ifndef __UNOCTREEGRID_HH__
#define __UNOCTREEGRID_HH__

#include <grid.hh>
#include <vtkHyperOctree.h>
#include <vtkDoubleArray.h>
#include <vtkHyperOctreeCursor.h>

class UnOctreeGrid : protected Grid
{
public:
    UnOctreeGrid(double origin[3], double size[3], bool (*f)(double[3], int));
    ~UnOctreeGrid();

    void addScalarData(const char *name, void (*f)(double[3], double*));
    void writeToFile(const char *prefix);

private:
    vtkHyperOctree *grid;
    vtkHyperOctreeCursor *cursor;

    void gridRefine(bool (*f)(double[3], int));
    void gridRecurseScalar(vtkDoubleArray *xyz, void (*f)(double[3], double*));
};

#endif      // __UNOCTREEGRID_HH__
