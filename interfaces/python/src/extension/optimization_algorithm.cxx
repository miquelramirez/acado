#include "optimization_algorithm.hxx"

namespace ACADO {

    PyOptimizationAlgorithm::PyOptimizationAlgorithm(PyOCP::ptr ocp)
        : OptimizationAlgorithm(*ocp), _ocp(ocp) {

    }

    PyOptimizationAlgorithm::~PyOptimizationAlgorithm() {

    }

    bp::object
    PyOptimizationAlgorithm::_init() {
        auto r = init();

        return bp::object(r == SUCCESSFUL_RETURN);
    }

    bp::object
    PyOptimizationAlgorithm::_solve() {
        auto r = solve();

        getDifferentialStates(_xd);
        getAlgebraicStates(_xa);
        getParameters(_params);
        getControls(_u);

        return bp::object(r == SUCCESSFUL_RETURN);
    }

}

void define_optimization_algorithm() {
    using namespace ACADO;

    bp::class_< PyOptimizationAlgorithm,
                PyOptimizationAlgorithm::ptr,
                boost::noncopyable>("OptimizationAlgorithm",
                                    bp::init<PyOCP::ptr>())
        .def("init", &PyOptimizationAlgorithm::_init)
        .def("solve", &PyOptimizationAlgorithm::_solve)
        .add_property("objective_value", &PyOptimizationAlgorithm::get_objective)
        ;
}
