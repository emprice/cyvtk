#ifndef __GRID_HH__
#define __GRID_HH__

#include <vtkCell.h>

class Grid
{
public:
    virtual void addScalarCellData(const char *name, const double *data) = 0;
    virtual void addScalarPointData(const char *name, const double *data) = 0;
    virtual void writeToFile(const char *prefix) = 0;

protected:
    void computeCellCenter(vtkCell *cell, double center[3]);
};

#endif      // __GRID_HH__
