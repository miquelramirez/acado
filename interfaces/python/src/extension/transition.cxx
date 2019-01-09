#include "transition.hxx"

namespace ACADO {

    PyTransition::PyTransition(PyContext::ptr ctx)
        : _ctx(ctx) {

    }

    PyTransition::~PyTransition() {

    }

    void
    PyTransition::set_constraint(TermRef lhs, TermRef rhs) {
        auto lhs_var = _ctx->get_d_var(lhs);
        auto rhs_expr = _ctx->get(rhs);
        (*this) << *lhs_var == *rhs_expr;
    }

}

void define_transition() {
    using namespace ACADO;

    bp::class_<PyTransition, PyTransition::ptr, boost::noncopyable>(
        "Transition", bp::init<PyContext::ptr>())
        .def("set_constraint", &PyTransition::set_constraint)
        ;
}
