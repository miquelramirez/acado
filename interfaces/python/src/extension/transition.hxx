#pragma once

#include <acado/acado_optimal_control.hpp>
#include "context.hxx"

extern void define_transition();

namespace ACADO {

    class PyTransition : public Transition {
        // attributes
    private:
        PyContext::ptr  _ctx;
        //types
    public:
        typedef std::shared_ptr<PyTransition> ptr;

        // Constructors
    public:

        PyTransition(PyContext::ptr);
        virtual ~PyTransition();

        // Interface
    public:

        void set_constraint( TermRef lhs, TermRef rhs );
    };
}
