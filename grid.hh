#ifndef __GRID_HH__
#define __GRID_HH__

#include <vtkCell.h>

class Grid
{
public:
    virtual void addScalarData(const char *name,
        void (*f)(double[3], double*)) = 0;
    virtual void writeToFile(const char *prefix) = 0;

protected:
    void computeCellCenter(vtkCell *cell, double center[3]);
};

#endif      // __GRID_HH__
