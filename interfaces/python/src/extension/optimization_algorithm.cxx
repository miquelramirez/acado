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
        get(MAX_NUM_ITERATIONS, param_value);
        return param_value;
    }

    IntegratorType
    PyOptimizationAlgorithm::get_integrator_type() const {
        int param_value;
        get(INTEGRATOR_TYPE, param_value);
        return (IntegratorType)param_value;
    }

    void
    PyOptimizationAlgorithm::set_integrator_type(IntegratorType v) {
        set(INTEGRATOR_TYPE, v);
    }

    double
    PyOptimizationAlgorithm::get_integrator_tolerance() const {
        double param_value;
        get(INTEGRATOR_TOLERANCE, param_value);
        return param_value;
    }

    void
    PyOptimizationAlgorithm::set_integrator_tolerance(double v) {
        set(INTEGRATOR_TOLERANCE, v);
    }

    void
    PyOptimizationAlgorithm::set_discretization_type(StateDiscretizationType v) {
        set(DISCRETIZATION_TYPE, v);
    }

    StateDiscretizationType
    PyOptimizationAlgorithm::get_discretizaton_type() const {
        int param_value;
        get(DISCRETIZATION_TYPE, param_value);
        return (StateDiscretizationType)param_value;
    }

    void
    PyOptimizationAlgorithm::set_kkt_tolerance(double v) {
        set(KKT_TOLERANCE, v);
    }

    double
    PyOptimizationAlgorithm::get_kkt_tolerance() const {
        double param_value;
        get(KKT_TOLERANCE, param_value);
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

    bp::enum_<IntegratorType>("IntegratorType")
            .value("INT_RK12", IntegratorType::INT_RK12)
            .value("INT_RK23", IntegratorType::INT_RK23)
            .value("INT_RK45", IntegratorType::INT_RK45)
            .value("INT_RK78", IntegratorType::INT_RK78)
            .value("INT_BDF", IntegratorType::INT_BDF)
            .value("INT_DISCRETE", IntegratorType::INT_DISCRETE)
            .value("INT_LYAPUNOV45", IntegratorType::INT_LYAPUNOV45)
            .value("INT_UNKNOWN", IntegratorType::INT_UNKNOWN)
    ;

    bp::enum_<StateDiscretizationType>("DiscretizationType")
        .value("SINGLE_SHOOTING", StateDiscretizationType::SINGLE_SHOOTING)
        .value("MULTIPLE_SHOOTING", StateDiscretizationType::MULTIPLE_SHOOTING)
        .value("COLLOCATION", StateDiscretizationType::COLLOCATION)
        .value("UNKNOWN_DISCRETIZATION", StateDiscretizationType::UNKNOWN_DISCRETIZATION)
        ;

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
        .add_property("integrator_type",
                        &PyOptimizationAlgorithm::get_integrator_type,
                        &PyOptimizationAlgorithm::set_integrator_type)
        .add_property("integrator_tolerance",
                        &PyOptimizationAlgorithm::get_integrator_tolerance,
                        &PyOptimizationAlgorithm::set_integrator_tolerance)
        .add_property("discretization_type",
                        &PyOptimizationAlgorithm::get_discretizaton_type,
                        &PyOptimizationAlgorithm::set_discretization_type)
        .add_property("kkt_tolerance",
                        &PyOptimizationAlgorithm::get_kkt_tolerance,
                        &PyOptimizationAlgorithm::set_kkt_tolerance)
        ;
}
