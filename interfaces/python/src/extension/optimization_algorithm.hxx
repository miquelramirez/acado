#pragma once

#include <memory>
#include "ocp.hxx"
#include "grids.hxx"

extern void define_optimization_algorithm();

namespace ACADO {

    class PyOptimizationAlgorithm : public OptimizationAlgorithm {
        // attributes
    private:
        PyOCP::ptr      _ocp;

        VariablesGrid   _xd; // differential states
        VariablesGrid   _xa; // algebraic states
        VariablesGrid   _params; // _parameters
        VariablesGrid   _u; // _controls
        VariablesGrid   _w; // perturbation values
        Grid            _time_grid;

        bp::list        _py_xd;
        bp::list        _py_xa;
        bp::list        _py_params;
        bp::list        _py_u;
        bp::list        _py_w;
        bp::list        _py_t;

        // types
    public:
        typedef std::shared_ptr<PyOptimizationAlgorithm> ptr;
        // constructors
    public:
        PyOptimizationAlgorithm(PyOCP::ptr);

        virtual ~PyOptimizationAlgorithm();
        // interface
    public:
        bp::object  _init();
        bp::object  _solve();
        double      get_objective() const { return getObjectiveValue(); }

        void    initialize_differential_states( PyVariablesGrid::ptr );
        void    initialize_controls( PyVariablesGrid::ptr );
        void    initialize_parameters( PyVariablesGrid::ptr );
        // accessors
    public:
        int             max_num_iterations() const;
        void            set_max_num_iterations(int v) { set(MAX_NUM_ITERATIONS, v); }
        IntegratorType  get_integrator_type() const;
        void            set_integrator_type(IntegratorType v);
        double          get_absolute_tolerance() const;
        void            set_absolute_tolerance(double v);
        double          get_integrator_tolerance() const;
        void            set_integrator_tolerance(double v);
        StateDiscretizationType
                        get_discretizaton_type() const;
        void            set_discretization_type(StateDiscretizationType v);
        double          get_kkt_tolerance() const;
        void            set_kkt_tolerance(double v);
        double          get_levenberg_marquardt() const;
        void            set_levenberg_marquardt(double v);
        PrintLevel      get_print_level() const;
        void            set_print_level(PrintLevel v);
        PrintLevel      get_integrator_print_level() const;
        void            set_integrator_print_level(PrintLevel v);
        HessianApproximationMode
                        get_hessian_approximation() const;
        void            set_hessian_approximation(HessianApproximationMode m);
        SensitivityType get_dynamic_sensitivity() const;
        void            set_dynamic_sensitivity(SensitivityType v);
        SensitivityType get_objective_sensitivity() const;
        void            set_objective_sensitivity(SensitivityType v);
        SensitivityType get_constraint_sensitivity() const;
        void            set_constraint_sensitivity(SensitivityType v);
        double          get_line_search_tolerance() const;
        void            set_line_search_tolerance(double v);
        double          get_minimum_line_search_parameter() const;
        void            set_minimum_line_search_parameter(double v);
        int             get_maximum_number_of_QP_iterations() const;
        void            set_maximum_number_of_QP_iterations(int v);
        double          get_initial_step_size() const;
        void            set_initial_step_size(double v);
        double          get_minimum_step_size() const;
        void            set_minimum_step_size(double v);
        double          get_maximum_step_size() const;
        void            set_maximum_step_size(double v);
        double          get_step_size_tuning() const;
        void            set_step_size_tuning(double v);

        bp::list    Xd() { return _py_xd; }
        bp::list    Xa() { return _py_xa; }
        bp::list    U() { return _py_u; }
        bp::list    P() { return _py_params; }
        bp::list    W() { return _py_w; }
        bp::list    times() { return _py_t; }
    };
}
