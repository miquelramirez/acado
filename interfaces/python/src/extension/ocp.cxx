#include "ocp.hxx"

namespace ACADO {

    PyOCP::PyOCP( PyContext::ptr ctx, double t0, TermRef tmax, int N )
        : OCP(t0, *(ctx->get_parameter(tmax)), N), _ctx(ctx) {

    }

    PyOCP::~PyOCP() {

    }

    void
    PyOCP::add_continuous_diff_constraints( PyDifferentialEquation::ptr de ) {
        subjectTo(*de->get_odes());
    }

    void
    PyOCP::add_initial_constraint(AtomRef atom) {
        subjectTo(AT_START, *_ctx->get_atom(atom));
    }

    void
    PyOCP::add_terminal_constraint(AtomRef atom) {
        subjectTo(AT_END, *_ctx->get_atom(atom));
    }

    void
    PyOCP::add_constraint(AtomRef atom) {
        AtomPtr c_comp = _ctx->get_atom(atom);
        //std::cout << "num grid points: " << grid->getNumPoints() << std::endl;
        //std::cout << c_comp->getLB()[0] << " <= " <<  c_comp->getExpression() << "<=" << c_comp->getUB()[0] << std::endl;
        subjectTo( c_comp->getLB()[0] <= c_comp->getExpression() <= c_comp->getUB()[0] );
    }

    void
    PyOCP::set_terminal_cost(TermRef expr) {
        minimizeMayerTerm(*_ctx->get(expr));
    }

}

void define_ocp() {
    using namespace ACADO;

    bp::class_<PyOCP, PyOCP::ptr,
        boost::noncopyable>("OCP",
            bp::init<PyContext::ptr, double, TermRef, int>())
            .def("add_continuous_diff_constraints", &PyOCP::add_continuous_diff_constraints)
            .def("add_initial_constraint", &PyOCP::add_initial_constraint)
            .def("add_terminal_constraint", &PyOCP::add_terminal_constraint)
            .def("add_constraint", &PyOCP::add_constraint)
            .def("set_terminal_cost", &PyOCP::set_terminal_cost)
        ;
}
