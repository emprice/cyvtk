import cython
import numpy as np
cimport numpy as np


cdef extern from "structuredGrid.hh":
    cdef cppclass StructuredGrid:
        StructuredGrid(double *, size_t, double *, size_t, double *, size_t)
        void addScalarCellData(const char *, const double *)
        void addVectorCellData(const char *, const double *,
            const double *, const double *)
        void addScalarPointData(const char *, const double *)
        void addVectorPointData(const char *, const double *,
            const double *, const double *)
        void addScalarFieldData(const char *, const double)
        void writeToFile(const char *)

cdef extern from "unstructuredGrid.hh":
    cdef cppclass UnstructuredGrid:
        UnstructuredGrid(double *, double *, double *, size_t)
        void addScalarCellData(const char *, const double *)
        void addVectorCellData(const char *, const double *,
            const double *, const double *)
        void addScalarPointData(const char *, const double *)
        void addVectorPointData(const char *, const double *,
            const double *, const double *)
        void addScalarFieldData(const char *, const double)
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


    def add_vector_cell_data(self, name,
    np.ndarray[double, ndim=3, mode='c'] dataX,
    np.ndarray[double, ndim=3, mode='c'] dataY,
    np.ndarray[double, ndim=3, mode='c'] dataZ):

        cdef np.ndarray[double, ndim=1, mode='c'] rX = dataX.ravel(order='F')
        cdef np.ndarray[double, ndim=1, mode='c'] rY = dataY.ravel(order='F')
        cdef np.ndarray[double, ndim=1, mode='c'] rZ = dataZ.ravel(order='F')

        self.thisptr.addVectorCellData(name, &(rX[0]), &(rY[0]), &(rZ[0]))


    def add_scalar_point_data(self, name,
    np.ndarray[double, ndim=3, mode='c'] data):

        cdef np.ndarray[double, ndim=1, mode='c'] r = data.ravel(order='F')
        self.thisptr.addScalarPointData(name, &(r[0]))


    def add_vector_point_data(self, name,
    np.ndarray[double, ndim=3, mode='c'] dataX,
    np.ndarray[double, ndim=3, mode='c'] dataY,
    np.ndarray[double, ndim=3, mode='c'] dataZ):

        cdef np.ndarray[double, ndim=1, mode='c'] rX = dataX.ravel(order='F')
        cdef np.ndarray[double, ndim=1, mode='c'] rY = dataY.ravel(order='F')
        cdef np.ndarray[double, ndim=1, mode='c'] rZ = dataZ.ravel(order='F')

        self.thisptr.addVectorPointData(name, &(rX[0]), &(rY[0]), &(rZ[0]))


    def add_scalar_field_data(self, name, double data):

        self.thisptr.addScalarFieldData(name, data)


    def write_to_file(self, prefix):

        self.thisptr.writeToFile(prefix)


    def __dealloc__(self):

        del self.thisptr


cdef class PyUnstructuredGrid:

    cdef UnstructuredGrid *thisptr

    cdef object u, v, w
    cdef double [:] _u, _v, _w


    def __init__(self, np.ndarray[double, ndim=1, mode='c'] uu,
    np.ndarray[double, ndim=1, mode='c'] vv,
    np.ndarray[double, ndim=1, mode='c'] ww):

        self.u = uu.copy(); self._u = self.u
        self.v = vv.copy(); self._v = self.v
        self.w = ww.copy(); self._w = self.w

        assert len(self.u) == len(self.v) and len(self.u) == len(self.w)
        self.thisptr = new UnstructuredGrid(&(self._u[0]), &(self._v[0]),
            &(self._w[0]), len(self.u))


    def add_scalar_cell_data(self, name,
    np.ndarray[double, ndim=1, mode='c'] data):

        self.thisptr.addScalarCellData(name, &(data[0]))


    def add_vector_cell_data(self, name,
    np.ndarray[double, ndim=1, mode='c'] dataX,
    np.ndarray[double, ndim=1, mode='c'] dataY,
    np.ndarray[double, ndim=1, mode='c'] dataZ):

        self.thisptr.addVectorCellData(name, &(dataX[0]), &(dataY[0]), &(dataZ[0]))


    def add_scalar_point_data(self, name,
    np.ndarray[double, ndim=1, mode='c'] data):

        self.thisptr.addScalarPointData(name, &(data[0]))


    def add_vector_point_data(self, name,
    np.ndarray[double, ndim=1, mode='c'] dataX,
    np.ndarray[double, ndim=1, mode='c'] dataY,
    np.ndarray[double, ndim=1, mode='c'] dataZ):

        self.thisptr.addVectorPointData(name, &(dataX[0]), &(dataY[0]), &(dataZ[0]))


    def add_scalar_field_data(self, name, double data):

        self.thisptr.addScalarFieldData(name, data)


    def write_to_file(self, prefix):

        self.thisptr.writeToFile(prefix)


    def __dealloc__(self):

        del self.thisptr
