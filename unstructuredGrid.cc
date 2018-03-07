#include <unstructuredGrid.hh>
#include <vtkPoints.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <string>
#include <sstream>


UnstructuredGrid::UnstructuredGrid(double *x, double *y, double *z, size_t N)
{
    // initialize a new grid object
    grid = vtkUnstructuredGrid::New();

    // allocate memory for all points
    vtkPoints *points = vtkPoints::New();
    points->SetNumberOfPoints(N);

    // loop needs to be in fortran order (first index varies fastest, last
    // index varies slowest) for vtk to be happy
    for (size_t idx = 0; idx < N; ++idx)
    {
        points->SetPoint(idx, x[idx], y[idx], z[idx]);
    }

    // attach the points to the grid
    grid->SetPoints(points);

    // clean up memory
    points->Delete();
}


UnstructuredGrid::~UnstructuredGrid()
{
    // clean up memory
    grid->Delete();
}


void UnstructuredGrid::addScalarCellData(const char *name, const double *data)
{
    vtkDoubleArray *xyz = vtkDoubleArray::New();
    xyz->SetName(name);
    xyz->SetNumberOfComponents(1);

    size_t numCells = grid->GetNumberOfCells();
    xyz->SetNumberOfTuples(numCells);

    for (size_t idx = 0; idx < numCells; ++idx)
        xyz->SetTuple1(idx, data[idx]);

    grid->GetCellData()->AddArray(xyz);
    xyz->Delete();
}


void UnstructuredGrid::addVectorCellData(const char *name, const double *dataX,
    const double *dataY, const double *dataZ)
{
    vtkDoubleArray *xyz = vtkDoubleArray::New();
    xyz->SetName(name);
    xyz->SetNumberOfComponents(3);

    size_t numCells = grid->GetNumberOfCells();
    xyz->SetNumberOfTuples(numCells);

    for (size_t idx = 0; idx < numCells; ++idx)
        xyz->SetTuple3(idx, dataX[idx], dataY[idx], dataZ[idx]);

    grid->GetCellData()->AddArray(xyz);
    xyz->Delete();
}


void UnstructuredGrid::addScalarPointData(const char *name, const double *data)
{
    vtkDoubleArray *xyz = vtkDoubleArray::New();
    xyz->SetName(name);
    xyz->SetNumberOfComponents(1);

    size_t numPoints = grid->GetNumberOfPoints();
    xyz->SetNumberOfTuples(numPoints);

    for (size_t idx = 0; idx < numPoints; ++idx)
        xyz->SetTuple1(idx, data[idx]);

    grid->GetPointData()->AddArray(xyz);
    xyz->Delete();
}


void UnstructuredGrid::addVectorPointData(const char *name, const double *dataX,
    const double *dataY, const double *dataZ)
{
    vtkDoubleArray *xyz = vtkDoubleArray::New();
    xyz->SetName(name);
    xyz->SetNumberOfComponents(3);

    size_t numPoints = grid->GetNumberOfPoints();
    xyz->SetNumberOfTuples(numPoints);

    for (size_t idx = 0; idx < numPoints; ++idx)
        xyz->SetTuple3(idx, dataX[idx], dataY[idx], dataZ[idx]);

    grid->GetPointData()->AddArray(xyz);
    xyz->Delete();
}


void UnstructuredGrid::addScalarFieldData(const char *name, const double data)
{
    vtkDoubleArray *f = vtkDoubleArray::New();
    f->SetName(name);
    f->SetNumberOfTuples(1);
    f->SetTuple1(0, data);

    grid->GetFieldData()->AddArray(f);
    f->Delete();
}


void UnstructuredGrid::writeToFile(const char *prefix)
{
    // create a new writer
    vtkXMLUnstructuredGridWriter *writer = vtkXMLUnstructuredGridWriter::New();

    // construct the filename from the given prefix and default extension
    std::ostringstream oss;
    oss << std::string(prefix) << "." << writer->GetDefaultFileExtension();
    writer->SetFileName(oss.str().c_str());

    // attach the grid to the writer
#if VTK_MAJOR_VERSION <= 5
    writer->SetInput(grid);
#else
    writer->SetInputData(grid);
#endif

    // write the file and clean up
    writer->Write();
    writer->Delete();
}

// vim: set syntax=cpp.doxygen:
