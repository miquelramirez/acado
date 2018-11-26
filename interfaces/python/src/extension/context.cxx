#include "context.hxx"

#include <iostream>

namespace dp {

    PyContext::PyContext(std::string name) {}

    PyContext::~PyContext() {}

}

void define_context() {
    using namespace dp;

    bp::class_<PyContext, PyContext::ptr, boost::noncopyable >("Context", bp::init<std::string>())
        /*
        .def("new_uninterpreted_term", &PyContext::new_uninterpreted_term)
        .def("real", &PyContext::real)
        .def("integer", &PyContext::integer)
        .def("term_str", &PyContext::term_str)
        .def("add", &PyContext::add)
        .def("sub", &PyContext::sub)
        .def("mul", &PyContext::mul)
        .def("div", &PyContext::div)
        .def("pow", &PyContext::pow)
        .def("sin", &PyContext::sin)
        .def("cos", &PyContext::cos)
        .def("tan", &PyContext::tan)
        .def("cot", &PyContext::cot)
        .def("asin", &PyContext::asin)
        .def("acos", &PyContext::acos)
        .def("atan", &PyContext::atan)
        .def("log", &PyContext::log)
        .def("exp", &PyContext::exp)
        .def("sqrt", &PyContext::sqrt)
        .def("abs", &PyContext::abs)
        .def("min", &PyContext::min)
        .def("max", &PyContext::max)
        .def("sign", &PyContext::sign)
        */
        ;
}
