#pragma once

#include <acado/acado_optimal_control.hpp>
#include "context.hxx"

extern void define_differential_equation();

namespace ACADO {

    class PyDifferentialEquation : public DifferentialEquation {
        // Attributes
    private:
        PyContext::ptr  _ctx;
        // Types
    public:
        typedef std::shared_ptr<PyDifferentialEquation> ptr;

        // Constructors
    public:
        PyDifferentialEquation( PyContext::ptr, double, TermRef);
        virtual ~PyDifferentialEquation();

        // Interface
    public:
        void set_ode( TermRef lhs, TermRef rhs );

    };

}
