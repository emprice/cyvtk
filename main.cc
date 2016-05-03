#include <unOctreeGrid.hh>


bool refine(double p[3], int level)
{
    double x = p[0], y = p[1], z = p[2];
    double r = sqrt(x * x + y * y + z * z);

    if (level < 3)
        return true;
    if ((r < 0.5) && (level < 5))
        return true;

    return false;
}


void f(double p[3], double *v)
{
    double x = p[0], y = p[1], z = p[2];
    *v = pow(x, 3.) + pow(z, 5.) + pow(y, 2.);
}


void g(double p[3], double *v)
{
    double x = p[0], y = p[1], z = p[2];
    *v = pow(x, 3.) * pow(z, 5.) * pow(y, 8.);
}


void h(double p[3], double *v)
{
    double x = p[0], y = p[1], z = p[2];
    *v = x;
}


int main()
{
    double origin[3] = {0., 0., 0.};
    double size[3] = {1., 1., 1.};

    UnOctreeGrid grid(origin, size, refine);
    grid.addScalarData("foo", f);
    grid.addScalarData("bar", g);
    grid.addScalarData("baz", h);
    grid.writeToFile("hello");
}
