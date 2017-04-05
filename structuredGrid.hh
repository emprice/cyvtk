#ifndef __STRUCTUREDGRID_HH__
#define __STRUCTUREDGRID_HH__

#include <grid.hh>
#include <vtkStructuredGrid.h>

class StructuredGrid : protected Grid
{
public:
    StructuredGrid(double *x, size_t N, double *y, size_t P,
        double *z, size_t Q);
    ~StructuredGrid();

    void addScalarCellData(const char *name, const double *data);
    void addVectorCellData(const char *name, const double *dataX,
        const double *dataY, const double *dataZ);
    void addScalarPointData(const char *name, const double *data);
    void addVectorPointData(const char *name, const double *dataX,
        const double *dataY, const double *dataZ);
    void writeToFile(const char *prefix);

private:
    vtkStructuredGrid *grid;
};

#endif      // __STRUCTUREDGRID_HH__
