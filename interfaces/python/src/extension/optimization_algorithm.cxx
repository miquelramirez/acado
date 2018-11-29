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

    int
    PyOptimizationAlgorithm::max_num_iterations() const {
        int param_value;
        return get(MAX_NUM_ITERATIONS, param_value);
        return param_value;
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
        .add_property("max_num_iterations",
                        &PyOptimizationAlgorithm::max_num_iterations,
                        &PyOptimizationAlgorithm::set_max_num_iterations)
        ;
}
