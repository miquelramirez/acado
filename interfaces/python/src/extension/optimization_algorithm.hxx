#pragma once

#include <memory>
#include "ocp.hxx"

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
        // accessors
    public:
        int     max_num_iterations() const;
        void    set_max_num_iterations(int v) { set(MAX_NUM_ITERATIONS, v); }
    };
}
