#pragma once

#include <acado/acado_optimal_control.hpp>
#include "context.hxx"
#include "differential_equation.hxx"

extern void define_ocp();

namespace ACADO {

    class PyOCP : public OCP {
        // types
    public:
        typedef std::shared_ptr<PyOCP> ptr;
        // attributes
    private:
        PyContext::ptr      _ctx;
        // constructors
    public:
        PyOCP( PyContext::ptr ctx, double t0, TermRef tmax, int N );
        virtual ~PyOCP();

        // interface
    public:
        void add_continuous_diff_constraints( PyDifferentialEquation::ptr );

        void add_initial_constraint( AtomRef atom );
        void add_terminal_constraint( AtomRef atom );
        void add_constraint( AtomRef atom );

        void set_terminal_cost( TermRef expr );
    };
}
