#pragma once

#include <acado/acado_optimal_control.hpp>
#include "context.hxx"
#include "differential_equation.hxx"
#include "transition.hxx"

extern void define_ocp();

namespace ACADO {

    class PyOCP {
        // types
    public:
        typedef std::shared_ptr<PyOCP> ptr;
        // attributes
    private:
        OCP*                _cp;
        PyContext::ptr      _ctx;
        // constructors
    public:
        //PyOCP( PyContext::ptr ctx, double t0, TermRef tmax, int N );
        PyOCP( PyContext::ptr ctx, double t0, bp::object tmax, int N);
        virtual ~PyOCP();

        // interface
    public:
        void add_continuous_diff_constraints( PyDifferentialEquation::ptr );
        void add_discrete_diff_constraints( PyDiscretizedDifferentialEquation::ptr );

        void add_initial_constraint( AtomRef atom );
        void add_terminal_constraint( AtomRef atom );
        void add_constraint( AtomRef atom );
        void add_boundary_constraint(double lb, TermRef h1, TermRef h2, double ub);

        void set_terminal_cost( TermRef expr );
        void set_stage_cost( TermRef expr);

        OCP& problem() { return *_cp; }
    };
}
