#include <unOctreeGrid.hh>
#include <error.hh>
#include <vtkPoints.h>
#include <vtkCellData.h>
#include <vtkCellArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetAttributes.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <string>
#include <sstream>


UnOctreeGrid::UnOctreeGrid(double origin[3], double size[3],
    bool (*f)(double[3], int))
{
    // initialize a new grid object
    grid = vtkHyperOctree::New();
    grid->SetDimension(3);
    grid->SetSize(size[0], size[1], size[2]);
    grid->SetOrigin(origin[0], origin[1], origin[2]);
    grid->SetDualGridFlag(false);

    cursor = grid->NewCellCursor();
    cursor->ToRoot();
    gridRefine(f);
}


UnOctreeGrid::~UnOctreeGrid()
{
    grid->Delete();
}


void UnOctreeGrid::gridRefine(bool (*f)(double[3], int))
{
    int level = cursor->GetCurrentLevel();

    // compute the center of this cell
    double center[3];
    int idx = cursor->GetLeafId();
    vtkCell *cell = grid->GetCell(idx);
    computeCellCenter(cell, center);

    if (f(center, level))
    {
        grid->SubdivideLeaf(cursor);

        for (int i = 0; i < cursor->GetNumberOfChildren(); i++)
        {
            // move to the ith child and recurse (dfs)
            cursor->ToChild(i);
            gridRefine(f);
            cursor->ToParent();
        }
    }
}


void UnOctreeGrid::addScalarData(const char *name,
    void (*f)(double[3], double*))
{
    vtkDoubleArray *xyz = vtkDoubleArray::New();
    xyz->SetName(name);
    xyz->SetNumberOfComponents(1);

    size_t numCells = grid->GetNumberOfCells();
    xyz->SetNumberOfTuples(numCells);

    // iterate through all cells
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

    grid->GetLeafData()->AddArray(xyz);
    xyz->Delete();
}


void UnOctreeGrid::writeToFile(const char *prefix)
{
    vtkUnstructuredGrid *tempGrid = vtkUnstructuredGrid::New();

    // allocate memory for all points
    size_t numPoints = grid->GetNumberOfPoints();
    vtkPoints *points = vtkPoints::New();
    points->SetNumberOfPoints(numPoints);

    // iterate through all points
    for (size_t i = 0; i < numPoints; i++)
    {
        double *xyz = grid->GetPoint(i);
        points->SetPoint(i, xyz);
    }

    // attach these points to the temporary grid
    tempGrid->SetPoints(points);

    // allocate memory for all cells
    size_t numCells = grid->GetNumberOfCells();
    vtkCellArray *cells = vtkCellArray::New();
    int *types = new int[numCells];

    // iterate through all cells
    for (size_t i = 0; i < numCells; i++)
    {
        types[i] = grid->GetCellType(i);

        vtkCell *cell = grid->GetCell(i);
        vtkIdList *pts = cell->GetPointIds();
        cells->InsertNextCell(pts);
    }

    // attach these cells to the temporary grid
    tempGrid->SetCells(types, cells);

    // iterate through all arrays
    for (size_t i = 0; i < grid->GetLeafData()->GetNumberOfArrays(); i++)
    {
        vtkDataArray *arr = grid->GetLeafData()->GetArray(i);
        tempGrid->GetCellData()->AddArray(arr);
    }

    // initialize a new writer
    vtkXMLUnstructuredGridWriter *writer = vtkXMLUnstructuredGridWriter::New();

    // construct the filename from the given prefix and default extension
    std::ostringstream oss;
    oss << std::string(prefix) << "." << writer->GetDefaultFileExtension();
    writer->SetFileName(oss.str().c_str());

    // attach the grid to the writer
    writer->SetInputData(tempGrid);

    // write the file and clean up
    writer->Write();

    // clean up
    writer->Delete();
    points->Delete();
    cells->Delete();
    delete[] types;
}
