#include "optimization_algorithm.hxx"
#include "type_conversions.hxx"

namespace ACADO {

    PyOptimizationAlgorithm::PyOptimizationAlgorithm(PyOCP::ptr ocp)
        : OptimizationAlgorithm(ocp->problem()), _ocp(ocp) {

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
    PyOptimizationAlgorithm::get_absolute_tolerance() const {
        double param_value;
        get(ABSOLUTE_TOLERANCE, param_value);
        return param_value;
    }

    void
    PyOptimizationAlgorithm::set_absolute_tolerance(double v) {
        set(ABSOLUTE_TOLERANCE, v);
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
    PyOptimizationAlgorithm::set_levenberg_marquardt(double v) {
        set(LEVENBERG_MARQUARDT, v);
    }

    double
    PyOptimizationAlgorithm::get_levenberg_marquardt() const {
        double param_value;
        get(LEVENBERG_MARQUARDT, param_value);
        return param_value;
    }

    void
    PyOptimizationAlgorithm::set_print_level(PrintLevel v) {
        set(PRINTLEVEL, v);
    }

    PrintLevel
    PyOptimizationAlgorithm::get_print_level() const {
        int param_value;
        get(PRINTLEVEL, param_value);
        return (PrintLevel)param_value;
    }

    void
    PyOptimizationAlgorithm::set_integrator_print_level(PrintLevel v) {
        set(INTEGRATOR_PRINTLEVEL, v);
    }

    PrintLevel
    PyOptimizationAlgorithm::get_integrator_print_level() const {
        int param_value;
        get(INTEGRATOR_PRINTLEVEL, param_value);
        return (PrintLevel)param_value;
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

    void
    PyOptimizationAlgorithm::set_hessian_approximation(HessianApproximationMode v) {
        set(HESSIAN_APPROXIMATION, v);
    }

    HessianApproximationMode
    PyOptimizationAlgorithm::get_hessian_approximation() const {
        int param_value;
        get(HESSIAN_APPROXIMATION, param_value);
        return (HessianApproximationMode)param_value;
    }

    void
    PyOptimizationAlgorithm::set_dynamic_sensitivity(SensitivityType v) {
        set(DYNAMIC_SENSITIVITY, v);
    }

    SensitivityType
    PyOptimizationAlgorithm::get_dynamic_sensitivity() const {
        int param_value;
        get(DYNAMIC_SENSITIVITY, param_value);
        return (SensitivityType)param_value;
    }

    void
    PyOptimizationAlgorithm::set_objective_sensitivity(SensitivityType v) {
        set(OBJECTIVE_SENSITIVITY, v);
    }

    SensitivityType
    PyOptimizationAlgorithm::get_objective_sensitivity() const {
        int param_value;
        get(OBJECTIVE_SENSITIVITY, param_value);
        return (SensitivityType)param_value;
    }

    void
    PyOptimizationAlgorithm::set_constraint_sensitivity(SensitivityType v) {
        set(CONSTRAINT_SENSITIVITY, v);
    }

    SensitivityType
    PyOptimizationAlgorithm::get_constraint_sensitivity() const {
        int param_value;
        get(CONSTRAINT_SENSITIVITY, param_value);
        return (SensitivityType)param_value;
    }

    void
    PyOptimizationAlgorithm::set_line_search_tolerance(double v) {
        set(LINESEARCH_TOLERANCE, v);
    }

    double
    PyOptimizationAlgorithm::get_line_search_tolerance() const {
        double v;
        get(LINESEARCH_TOLERANCE, v);
        return v;
    }

    void
    PyOptimizationAlgorithm::set_minimum_line_search_parameter(double v) {
        set(MIN_LINESEARCH_PARAMETER, v);
    }

    double
    PyOptimizationAlgorithm::get_minimum_line_search_parameter() const {
        double v;
        get(MIN_LINESEARCH_PARAMETER, v);
        return v;
    }

    void
    PyOptimizationAlgorithm::set_maximum_number_of_QP_iterations(int v) {
        set(MAX_NUM_QP_ITERATIONS, v);
    }

    int
    PyOptimizationAlgorithm::get_maximum_number_of_QP_iterations() const {
        int v;
        get(MAX_NUM_QP_ITERATIONS, v);
        return v;
    }

    void
    PyOptimizationAlgorithm::set_initial_step_size(double v) {
        set(INITIAL_INTEGRATOR_STEPSIZE, v);
    }

    double
    PyOptimizationAlgorithm::get_initial_step_size() const {
        double v;
        get(INITIAL_INTEGRATOR_STEPSIZE, v);
        return v;
    }

    void
    PyOptimizationAlgorithm::set_minimum_step_size(double v) {
        set(MIN_INTEGRATOR_STEPSIZE, v);
    }

    double
    PyOptimizationAlgorithm::get_minimum_step_size() const {
        double v;
        get(MIN_INTEGRATOR_STEPSIZE, v);
        return v;
    }

    void
    PyOptimizationAlgorithm::set_maximum_step_size(double v) {
        set(MAX_INTEGRATOR_STEPSIZE, v);
    }

    double
    PyOptimizationAlgorithm::get_maximum_step_size() const {
        double v;
        get(MAX_INTEGRATOR_STEPSIZE, v);
        return v;
    }

    void
    PyOptimizationAlgorithm::set_step_size_tuning(double v) {
        set(STEPSIZE_TUNING, v);
    }

    double
    PyOptimizationAlgorithm::get_step_size_tuning() const {
        double v;
        get(STEPSIZE_TUNING, v);
        return v;
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

    bp::enum_<PrintLevel>("PrintLevel")
        .value("NONE", PrintLevel::NONE)
        .value("LOW", PrintLevel::LOW)
        .value("MEDIUM", PrintLevel::MEDIUM)
        .value("HIGH", PrintLevel::HIGH)
        ;

    bp::enum_<HessianApproximationMode>("HessianApproximation")
        .value("CONSTANT_HESSIAN", HessianApproximationMode::CONSTANT_HESSIAN)
        .value("GAUSS_NEWTON", HessianApproximationMode::GAUSS_NEWTON)
        .value("FULL_BFGS_UPDATE", HessianApproximationMode::FULL_BFGS_UPDATE)
        .value("BLOCK_BFGS_UPDATE", HessianApproximationMode::BLOCK_BFGS_UPDATE)
        .value("GAUSS_NEWTON_WITH_BLOCK_BFGS", HessianApproximationMode::GAUSS_NEWTON_WITH_BLOCK_BFGS)
        .value("EXACT_HESSIAN", HessianApproximationMode::EXACT_HESSIAN)
        .value("DEFAULT_HESSIAN_APPROXIMATION", HessianApproximationMode::DEFAULT_HESSIAN_APPROXIMATION)
        ;

    bp::enum_<SensitivityType>("SensitivityType")
        .value("FORWARD_SENSITIVITY", SensitivityType::FORWARD_SENSITIVITY)
        .value("BACKWARD_SENSITIVITY", SensitivityType::BACKWARD_SENSITIVITY)
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
        .add_property("absolute_tolerance",
                        &PyOptimizationAlgorithm::get_absolute_tolerance,
                        &PyOptimizationAlgorithm::set_absolute_tolerance)
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
        .add_property("levenberg_marquardt",
                        &PyOptimizationAlgorithm::get_levenberg_marquardt,
                        &PyOptimizationAlgorithm::set_levenberg_marquardt)
        .add_property("print_level",
                        &PyOptimizationAlgorithm::get_print_level,
                        &PyOptimizationAlgorithm::set_print_level)
        .add_property("integrator_print_level",
                        &PyOptimizationAlgorithm::get_integrator_print_level,
                        &PyOptimizationAlgorithm::set_integrator_print_level)
        .add_property("hessian_approximation",
                        &PyOptimizationAlgorithm::get_hessian_approximation,
                        &PyOptimizationAlgorithm::set_hessian_approximation)
        .add_property("dynamic_sensitivity",
                        &PyOptimizationAlgorithm::get_dynamic_sensitivity,
                        &PyOptimizationAlgorithm::set_dynamic_sensitivity)
        .add_property("objective_sensitivity",
                        &PyOptimizationAlgorithm::get_objective_sensitivity,
                        &PyOptimizationAlgorithm::set_objective_sensitivity)
        .add_property("constraint_sensitivity",
                        &PyOptimizationAlgorithm::get_constraint_sensitivity,
                        &PyOptimizationAlgorithm::set_constraint_sensitivity)
        .add_property("line_search_tolerance",
                        &PyOptimizationAlgorithm::get_line_search_tolerance,
                        &PyOptimizationAlgorithm::set_line_search_tolerance)
        .add_property("minium_line_search_parameter",
                        &PyOptimizationAlgorithm::get_minimum_line_search_parameter,
                        &PyOptimizationAlgorithm::set_minimum_line_search_parameter)
        .add_property("maximum_number_of_QP_iterations",
                        &PyOptimizationAlgorithm::get_maximum_number_of_QP_iterations,
                        &PyOptimizationAlgorithm::set_maximum_number_of_QP_iterations)
        .add_property("initial_step_size",
                        &PyOptimizationAlgorithm::get_initial_step_size,
                        &PyOptimizationAlgorithm::set_initial_step_size)
        .add_property("minimum_step_size",
                        &PyOptimizationAlgorithm::get_minimum_step_size,
                        &PyOptimizationAlgorithm::set_minimum_step_size)
        .add_property("maximum_step_size",
                        &PyOptimizationAlgorithm::get_maximum_step_size,
                        &PyOptimizationAlgorithm::set_maximum_step_size)
        .add_property("step_size_tuning",
                        &PyOptimizationAlgorithm::get_step_size_tuning,
                        &PyOptimizationAlgorithm::set_step_size_tuning)
        ;
}
