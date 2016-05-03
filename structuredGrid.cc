#include <structuredGrid.hh>
#include <vtkPoints.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkXMLStructuredGridWriter.h>
#include <string>
#include <sstream>


StructuredGrid::StructuredGrid(double *x, size_t N, double *y, size_t P,
    double *z, size_t Q)
{
    // initialize a new grid object
    grid = vtkStructuredGrid::New();

    // allocate memory for all points
    vtkPoints *points = vtkPoints::New();
    points->SetNumberOfPoints(N * P * Q);

    size_t idx = 0;

    // loop needs to be in fortran order (first index varies fastest, last
    // index varies slowest) for vtk to be happy
    for (size_t k = 0; k < Q; k++)
    {
        for (size_t j = 0; j < P; j++)
        {
            for (size_t i = 0; i < N; i++)
            {
                points->SetPoint(idx, x[i], y[j], z[k]);
                idx++;
            }
        }
    }

    // attach the points to the grid
    grid->SetDimensions(N, P, Q);
    grid->SetPoints(points);

    // clean up memory
    points->Delete();
}


StructuredGrid::~StructuredGrid()
{
    // clean up memory
    grid->Delete();
}


void StructuredGrid::addScalarData(const char *name,
    void (*f)(double[3], double*))
{
    vtkDoubleArray *xyz = vtkDoubleArray::New();
    xyz->SetName(name);
    xyz->SetNumberOfComponents(1);

    size_t numCells = grid->GetNumberOfCells();
    xyz->SetNumberOfTuples(numCells);

    for (size_t idx = 0; idx < numCells; idx++)
    {
        // compute the center of this cell
        double center[3];
        vtkCell *cell = grid->GetCell(idx);
        computeCellCenter(cell, center);

        // find the value
        double v;
        f(center, &v);
        xyz->SetTuple1(idx, v);
    }

    grid->GetCellData()->AddArray(xyz);
    xyz->Delete();
}


void StructuredGrid::writeToFile(const char *prefix)
{
    // create a new writer
    vtkXMLStructuredGridWriter *writer = vtkXMLStructuredGridWriter::New();

    // construct the filename from the given prefix and default extension
    std::ostringstream oss;
    oss << std::string(prefix) << "." << writer->GetDefaultFileExtension();
    writer->SetFileName(oss.str().c_str());

    // attach the grid to the writer
    writer->SetInputData(grid);

    // write the file and clean up
    writer->Write();
    writer->Delete();
}
