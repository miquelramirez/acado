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
        int     max_num_iterations() const;
        void    set_max_num_iterations(int v) { set(MAX_NUM_ITERATIONS, v); }

        bp::list    Xd() { return _py_xd; }
        bp::list    Xa() { return _py_xa; }
        bp::list    U() { return _py_u; }
        bp::list    P() { return _py_params; }
        bp::list    W() { return _py_w; }
        bp::list    times() { return _py_t; }
    };
}
