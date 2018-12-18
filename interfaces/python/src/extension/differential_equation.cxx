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
        /*
        bp::extract<double> is_tStart_double(t0);
        if (!is_tStart_double.check()) {
            TermRef tStart = bp::extract<TermRef>(t0);
            bp::extract<double> is_tEnd_double(tf);
            if (!is_tEnd_double.check()) {
                std::cout << "tStart (TermRef), tEnd (TermRef)" << std::endl;
                TermRef tEnd = bp::extract<TermRef>(tf);
                _de = new DifferentialEquation(*(ctx->get_parameter(tStart)), *(ctx->get_parameter(tEnd)));
            } else {
                std::cout << "tStart (TermRef), tEnd (double)" << std::endl;
                double tEnd = (double)is_tEnd_double;
                _de = new DifferentialEquation(*(ctx->get_parameter(tStart)), tEnd);
            }
        } else {
            double tStart = (double)is_tStart_double;
            bp::extract<double> is_tEnd_double(tf);
            if (!is_tEnd_double.check()) {
                std::cout << "tStart (double), tEnd (TermRef)" << std::endl;
                TermRef tEnd = bp::extract<TermRef>(tf);
                _de = new DifferentialEquation(tStart, *(ctx->get_parameter(tEnd)));
            } else {
                std::cout << "tStart (double), tEnd (double)" << std::endl;
                double tEnd = (double)is_tEnd_double;
                _de = new DifferentialEquation(tStart, tEnd);
            }
        }
        */
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
