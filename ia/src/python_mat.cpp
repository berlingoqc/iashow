//
// Created by wq on 18-12-19.
//

// test.cpp
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/scoped_array.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

namespace py = boost::python;
namespace np = boost::python::numpy;

void square(np::ndarray& array)
{
    if (array.get_dtype() != np::dtype::get_builtin<int>())
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array data type.");
        py::throw_error_already_set();
    }
    size_t rows = array.shape(0), cols = array.shape(1);
    size_t stride_row = array.strides(0) / sizeof(int),
            stride_col = array.strides(1) / sizeof(int);
    cv::Mat mat(rows, cols, CV_32S);
    int *row_iter = reinterpret_cast<int*>(array.get_data());
    for (int i = 0; i < rows; i++, row_iter += stride_row)
    {
        int *col_iter = row_iter;
        int *mat_row = (int*)mat.ptr(i);
        for (int j = 0; j < cols; j++, col_iter += stride_col)
        {
            *(mat_row + j) = (*col_iter) * (*col_iter);
        }
    }

    for (int i = 0; i < rows; i++, row_iter += stride_row)
    {
        int *col_iter = row_iter;
        int *mat_row = (int*)mat.ptr(i);
        for (int j = 0; j < cols; j++, col_iter += stride_col)
        {
            *col_iter = *(mat_row + j);
        }
    }
    printf("Hello ndarray\n");
}


BOOST_PYTHON_MODULE(hello)
{
    using namespace boost::python;

    Py_Initialize();
    np::initialize();

    def("square", square);
}