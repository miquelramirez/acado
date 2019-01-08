#include "differential_equation.hxx"
namespace ACADO {

    PyDifferentialEquation::PyDifferentialEquation( PyContext::ptr ctx, bp::object t0, bp::object tf)
        : _de(nullptr), _ctx(ctx)
    {
        // MRJ: we cannot distinguish between int and float using the type
        // checking functionality in the extractor class, because int and double
        // have implicit conversions in C++.
        std::string tStart_class = boost::python::extract<std::string>(t0.attr("__class__").attr("__name__"));
        std::string tEnd_class = boost::python::extract<std::string>(tf.attr("__class__").attr("__name__"));

        if (tStart_class == "float") {
            if (tEnd_class == "float") {
                std::cout << "tStart (double), tEnd (double)" << std::endl;
                double tStart = bp::extract<double>(t0);
                double tEnd = bp::extract<double>(tf);
                _de = new DifferentialEquation(tStart, tEnd);
            } else {
                std::cout << "tStart (double), tEnd (TermRef)" << std::endl;
                double tStart = bp::extract<double>(t0);
                TermRef tEnd = bp::extract<TermRef>(tf);
                _de = new DifferentialEquation(tStart, *(ctx->get_parameter(tEnd)));
            }
        } else {
            if (tEnd_class == "float") {
                std::cout << "tStart (double), tEnd (double)" << std::endl;
                TermRef tStart = bp::extract<TermRef>(t0);
                double tEnd = bp::extract<double>(tf);
                _de = new DifferentialEquation(*(ctx->get_parameter(tStart)), tEnd);
            } else {
                std::cout << "tStart (TermRef), tEnd (TermRef)" << std::endl;
                TermRef tStart = bp::extract<TermRef>(t0);
                TermRef tEnd = bp::extract<TermRef>(tf);
                _de = new DifferentialEquation(*(ctx->get_parameter(tStart)), *(ctx->get_parameter(tEnd)));
            }
        }

        if (_de == nullptr) {
            throw std::domain_error("Invalid arguments for constructor of DifferentialEquation");
        }
    }

    PyDifferentialEquation::~PyDifferentialEquation() {
        if (_de != nullptr)
            delete _de;
    }

    void
    PyDifferentialEquation::set_ode(TermRef lhs, TermRef rhs) {
        auto lhs_var = _ctx->get_d_var(lhs);
        auto eq_expr = _ctx->get(rhs);

        (*_de) << dot(*lhs_var) == *eq_expr;
    }

    void
    PyDifferentialEquation::set_implicit_ode(AtomRef equation) {
        auto constraint = _ctx->get_atom(equation);
        (*_de) << constraint->getExpression();
    }

    PyDiscretizedDifferentialEquation::PyDiscretizedDifferentialEquation(PyContext::ptr ctx, double h)
        : DiscretizedDifferentialEquation(h), _ctx(ctx) {

    }

    PyDiscretizedDifferentialEquation::~PyDiscretizedDifferentialEquation() {

    }

    void
    PyDiscretizedDifferentialEquation::set_ode(TermRef lhs, TermRef rhs) {
        auto lhs_var = _ctx->get_d_var(lhs);
        auto eq_expr = _ctx->get(rhs);

        (*this) << dot(*lhs_var) == *eq_expr;
    }

    void
    PyDiscretizedDifferentialEquation::set_implicit_ode(AtomRef equation) {
        auto constraint = _ctx->get_atom(equation);
        (*this) << constraint->getExpression();
    }
}



void define_differential_equation() {
    using namespace ACADO;

    bp::class_<PyDifferentialEquation, PyDifferentialEquation::ptr,
        boost::noncopyable>("DifferentialEquation",
            bp::init<PyContext::ptr, bp::object, bp::object>())
        .def("set_ode", &PyDifferentialEquation::set_ode)
        .def("set_implicit_ode", &PyDifferentialEquation::set_implicit_ode)
        .add_property("num_dynamic_equations", &PyDifferentialEquation::getNumDynamicEquations)
        .add_property("num_algebraic_equations", &PyDifferentialEquation::getNumAlgebraicEquations)
        ;
}

void define_discretized_differential_equation() {
    using namespace ACADO;

    bp::class_<PyDiscretizedDifferentialEquation, PyDiscretizedDifferentialEquation::ptr,
        boost::noncopyable>("DiscretizedDifferentialEquation",
            bp::init<PyContext::ptr, double>())
        .def("set_ode", &PyDiscretizedDifferentialEquation::set_ode)
        .def("set_implicit_ode", &PyDiscretizedDifferentialEquation::set_implicit_ode)
        .add_property("num_dynamic_equations", &PyDiscretizedDifferentialEquation::getNumDynamicEquations)
        .add_property("num_algebraic_equations", &PyDiscretizedDifferentialEquation::getNumAlgebraicEquations)
        ;
}
