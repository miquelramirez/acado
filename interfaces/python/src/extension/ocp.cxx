#include "ocp.hxx"

namespace ACADO {

    PyOCP::PyOCP( PyContext::ptr ctx, double t0, bp::object tmax, int N )
        : _cp(nullptr), _ctx(ctx) {

        std::string tEnd_class = boost::python::extract<std::string>(tmax.attr("__class__").attr("__name__"));

        if (tEnd_class == "float") {
            std::cout << "tStart (double), tEnd (double)" << std::endl;
            double tEnd = bp::extract<double>(tmax);
            _cp = new OCP(t0, tEnd, N);
        } else {
            std::cout << "tStart (double), tEnd (TermRef)" << std::endl;
            TermRef tEnd = bp::extract<TermRef>(tmax);
            _cp = new OCP(t0, *(ctx->get_parameter(tEnd)), N);
        }

    }

    PyOCP::~PyOCP() {
        if (_cp != nullptr)
            delete _cp;
    }

    void
    PyOCP::add_continuous_diff_constraints( PyDifferentialEquation::ptr de ) {
        _cp->subjectTo(*de->get_odes());
    }

    void
    PyOCP::add_initial_constraint(AtomRef atom) {
        _cp->subjectTo(AT_START, *_ctx->get_atom(atom));
    }

    void
    PyOCP::add_terminal_constraint(AtomRef atom) {
        _cp->subjectTo(AT_END, *_ctx->get_atom(atom));
    }

    void
    PyOCP::add_constraint(AtomRef atom) {
        AtomPtr c_comp = _ctx->get_atom(atom);
        //std::cout << "num grid points: " << grid->getNumPoints() << std::endl;
        //std::cout << c_comp->getLB()[0] << " <= " <<  c_comp->getExpression() << "<=" << c_comp->getUB()[0] << std::endl;
        _cp->subjectTo( c_comp->getLB()[0] <= c_comp->getExpression() <= c_comp->getUB()[0] );
    }

    void
    PyOCP::set_terminal_cost(TermRef expr) {
        _cp->minimizeMayerTerm(*_ctx->get(expr));
    }

}

void define_ocp() {
    using namespace ACADO;

    bp::class_<PyOCP, PyOCP::ptr,
        boost::noncopyable>("OCP",
            bp::init<PyContext::ptr, double, bp::object, int>())
            .def("add_continuous_diff_constraints", &PyOCP::add_continuous_diff_constraints)
            .def("add_initial_constraint", &PyOCP::add_initial_constraint)
            .def("add_terminal_constraint", &PyOCP::add_terminal_constraint)
            .def("add_constraint", &PyOCP::add_constraint)
            .def("set_terminal_cost", &PyOCP::set_terminal_cost)
        ;
}
