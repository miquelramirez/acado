#pragma once

#include <string>
#include <map>
#include <boost/python.hpp>

#include "extension_types.hxx"

extern void define_context();

namespace  ACADO {
    namespace bp = boost::python;

    class PyContext {
        //types:
    public:
        typedef std::shared_ptr<PyContext>  ptr;

        //Attributes
    private:
        std::map< TermRef, DifferentialVarPtr >     _diff_vars;
        std::map< TermRef, ControlVarPtr >          _control_vars;
        std::map< TermRef, AlgebraicVarPtr >        _algebraic_vars;
        std::map< TermRef, ExpressionPtr >          _expressions;
        std::string                                 _name;
        TermRef                                     _ref_count;
        unsigned                                    _num_vars;
        unsigned                                    _num_expressions;


        //Constructors
    public:
        PyContext(std::string str);
        virtual ~PyContext();


        // Interface
    public:
        TermRef     new_differential_state(std::string name);
        TermRef     new_control_input(std::string name);
        TermRef     new_algebraic_state(std::string name);

    protected:

        TermRef     next_ref();


    public:
        // accessors
        std::string name() const { return _name; }
        unsigned    num_vars() const { return _num_vars; }
        unsigned    num_expressions() const { return _num_expressions; }

    };
}
