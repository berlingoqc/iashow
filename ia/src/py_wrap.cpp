//
// Created by wq on 18-12-19.
//

#include "video_parser.h"
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/scoped_array.hpp>

namespace py = boost::python;
namespace np = boost::python::numpy;


void (ShowContext::*load1)(std::string)         = &ShowContext::load;
BOOST_PYTHON_MODULE(pyvideo)
{
    using namespace boost::python;

    Py_Initialize();
    np::initialize();

    class_<CharacterInfo>("CharacterInfo",init<int,std::string,char>())
            .def_readonly("id", &CharacterInfo::id)
            .def_readonly("name", &CharacterInfo::name);

    class_<ShowContext>("ShowContext",no_init)
            .def("load",load1);

}