#include <grid.hh>


void Grid::computeCellCenter(vtkCell *cell, double center[3])
{
    double pCenter[3];
    double *weights = new double[cell->GetNumberOfPoints()];
    int id = cell->GetParametricCenter(pCenter);
    cell->EvaluateLocation(id, pCenter, center, weights);
    delete[] weights;
}
