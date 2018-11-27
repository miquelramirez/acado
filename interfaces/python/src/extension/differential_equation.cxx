#include "differential_equation.hxx"

namespace ACADO {

    PyDifferentialEquation::PyDifferentialEquation( PyContext::ptr ctx,
        double t0, TermRef t_max)
        : DifferentialEquation(t0, *(ctx->get_parameter(t_max))),
        _ctx(ctx) {

    }

    PyDifferentialEquation::~PyDifferentialEquation() {

    }

    void
    PyDifferentialEquation::set_ode(TermRef lhs, TermRef rhs) {
        auto lhs_var = _ctx->get_d_var(lhs);
        auto eq_expr = _ctx->get(rhs);

        (*this) << dot(*lhs_var) == *eq_expr;
    }
}

void define_differential_equation() {
    using namespace ACADO;

    bp::class_<PyDifferentialEquation, PyDifferentialEquation::ptr,
        boost::noncopyable>("DifferentialEquation",
            bp::init<PyContext::ptr, double, TermRef>())
        .def("set_ode", &PyDifferentialEquation::set_ode)
        .add_property("num_dynamic_equations", &PyDifferentialEquation::getNumDynamicEquations)
        .add_property("num_algebraic_equations", &PyDifferentialEquation::getNumAlgebraicEquations)
        ;
}
