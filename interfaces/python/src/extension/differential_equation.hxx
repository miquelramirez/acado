#pragma once

#include <acado/acado_optimal_control.hpp>
#include "context.hxx"

extern void define_differential_equation();

namespace ACADO {

    class PyDifferentialEquation {
        // Attributes
    private:
        DifferentialEquation* _de;
        PyContext::ptr      _ctx;
        // Types
    public:
        typedef std::shared_ptr<PyDifferentialEquation> ptr;

        // Constructors
    public:
        PyDifferentialEquation( PyContext::ptr, bp::object t0, bp::object tf);
        virtual ~PyDifferentialEquation();

        // Interface
    public:
        DifferentialEquation* get_odes() { return _de; }
        void set_ode( TermRef lhs, TermRef rhs );
        void set_implicit_ode( AtomRef equation );
        int getNumDynamicEquations() const { return _de->getNumDynamicEquations(); }
        int getNumAlgebraicEquations() const { return _de->getNumAlgebraicEquations(); }

    };

}
