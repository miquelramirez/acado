#include "context.hxx"

#include <iostream>

namespace ACADO {

    PyContext::PyContext(std::string name)
        : _name(name), _ref_count(0),
        _num_vars(0), _num_expressions(0) {}

    PyContext::~PyContext() {}

    TermRef
    PyContext::next_ref() {
        TermRef current = _ref_count;
        _ref_count++;
        return current;
    }

    TermRef
    PyContext::new_differential_state( std::string name ) {
        auto x = std::make_shared<DifferentialState>(name, 1, 1);
        TermRef x_ref = next_ref();
        _diff_vars[x_ref] = x;
        _num_vars++;
        return x_ref;
    }


}

void define_context() {
    using namespace ACADO;

    bp::class_<PyContext, PyContext::ptr, boost::noncopyable >("Context",
        bp::init<std::string>())
        .def("new_differential_state", &PyContext::new_differential_state)
        .add_property("num_vars", &PyContext::num_vars)
        .add_property("num_expressions", &PyContext::num_expressions)
        .add_property("name", &PyContext::name)
        ;
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

}
