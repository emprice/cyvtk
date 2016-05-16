import cython
import numpy as np
cimport numpy as np


cdef extern from "structuredGrid.hh":
    cdef cppclass StructuredGrid:
        StructuredGrid(double *, size_t, double *, size_t, double *, size_t)
        void addScalarCellData(const char *, const double *)
        void addScalarPointData(const char *, const double *)
        void writeToFile(const char *)


cdef class PyStructuredGrid:

    cdef StructuredGrid *thisptr

    cdef object u, v, w
    cdef double [:] _u, _v, _w


    def __init__(self, np.ndarray[double, ndim=1, mode='c'] uu,
    np.ndarray[double, ndim=1, mode='c'] vv,
    np.ndarray[double, ndim=1, mode='c'] ww):

        self.u = uu.copy(); self._u = self.u
        self.v = vv.copy(); self._v = self.v
        self.w = ww.copy(); self._w = self.w

        self.thisptr = new StructuredGrid(&(self._u[0]), len(self.u),
            &(self._v[0]), len(self.v), &(self._w[0]), len(self.w))


    def add_scalar_cell_data(self, name,
    np.ndarray[double, ndim=3, mode='c'] data):

        cdef np.ndarray[double, ndim=1, mode='c'] r = data.ravel(order='F')
        self.thisptr.addScalarCellData(name, &(r[0]))


    def add_scalar_point_data(self, name,
    np.ndarray[double, ndim=3, mode='c'] data):

        cdef np.ndarray[double, ndim=1, mode='c'] r = data.ravel(order='F')
        self.thisptr.addScalarPointData(name, &(r[0]))


    def write_to_file(self, prefix):

        self.thisptr.writeToFile(prefix)


    def __dealloc__(self):

        del self.thisptr
