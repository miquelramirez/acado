#include "optimization_algorithm.hxx"
#include "type_conversions.hxx"

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
        _py_xd = bp::list();
        _py_xa = bp::list();
        _py_u = bp::list();
        _py_params = bp::list();
        _py_w = bp::list();
        _py_t = bp::list();

        auto r = solve();
        bool solved = r == SUCCESSFUL_RETURN;
        if (solved) {
            if (getNX() > 0) {
                getDifferentialStates(_xd);
                _xd.getGrid(_time_grid);
                _py_xd = variable_grid_to_nested_list(_xd, getNX());
                for (unsigned k = 0; k < _time_grid.getNumPoints(); k++ )
                    _py_t.append(_time_grid.getTime(k));
            }
            if (getNXA() > 0) {
                getAlgebraicStates(_xa);
                _py_xa = variable_grid_to_nested_list(_xa, getNXA());
            }
            if (getNP() > 0) {
                getParameters(_params);
                _py_params = variable_grid_to_nested_list(_params, getNP());
            }
            if (getNU() > 0) {
                getControls(_u);
                _py_u = variable_grid_to_nested_list(_u, getNU());
            }
            if (getNW() > 0) {
                getDisturbances(_w);
                _py_w = variable_grid_to_nested_list(_w, getNW());
            }
        }

        return bp::object();
    }

    int
    PyOptimizationAlgorithm::max_num_iterations() const {
        int param_value;
        return get(MAX_NUM_ITERATIONS, param_value);
        return param_value;
    }

    void
    PyOptimizationAlgorithm::initialize_differential_states(PyVariablesGrid::ptr grid) {
        initializeDifferentialStates(*grid);
    }

    void
    PyOptimizationAlgorithm::initialize_controls(PyVariablesGrid::ptr grid) {
        initializeControls(*grid);
    }

    void
    PyOptimizationAlgorithm::initialize_parameters(PyVariablesGrid::ptr grid) {
        initializeParameters(*grid);
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
        .def("initialize_differential_states", &PyOptimizationAlgorithm::initialize_differential_states)
        .def("initialize_controls", &PyOptimizationAlgorithm::initialize_controls)
        .def("initialize_parameters", &PyOptimizationAlgorithm::initialize_parameters)
        .add_property("objective_value", &PyOptimizationAlgorithm::get_objective)
        .add_property("Xd", &PyOptimizationAlgorithm::Xd)
        .add_property("Xa", &PyOptimizationAlgorithm::Xa)
        .add_property("U", &PyOptimizationAlgorithm::U)
        .add_property("P", &PyOptimizationAlgorithm::P)
        .add_property("W", &PyOptimizationAlgorithm::W)
        .add_property("t", &PyOptimizationAlgorithm::times)
        .add_property("t0", &PyOptimizationAlgorithm::getStartTime)
        .add_property("tf", &PyOptimizationAlgorithm::getEndTime)
        .add_property("max_num_iterations",
                        &PyOptimizationAlgorithm::max_num_iterations,
                        &PyOptimizationAlgorithm::set_max_num_iterations)
        ;
}
