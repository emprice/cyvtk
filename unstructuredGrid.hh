#ifndef __UNSTRUCTUREDGRID_HH__
#define __UNSTRUCTUREDGRID_HH__

#include <grid.hh>
#include <vtkUnstructuredGrid.h>

class UnstructuredGrid : protected Grid
{
public:
    UnstructuredGrid(double *x, double *y, double *z, size_t N);
    ~UnstructuredGrid();

    void addScalarCellData(const char *name, const double *data);
    void addVectorCellData(const char *name, const double *dataX,
        const double *dataY, const double *dataZ);
    void addScalarPointData(const char *name, const double *data);
    void addVectorPointData(const char *name, const double *dataX,
        const double *dataY, const double *dataZ);
    void addScalarFieldData(const char *name, const double data);
    void writeToFile(const char *prefix);

private:
    vtkUnstructuredGrid *grid;
};

#endif      // __UNSTRUCTUREDGRID_HH__
