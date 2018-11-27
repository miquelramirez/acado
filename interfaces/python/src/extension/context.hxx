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
        std::vector< ExpressionPtr >                _terms;
        std::string                                 _name;
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

        TermRef     constant(double v);

        TermRef     add(TermRef lhs, TermRef rhs);
        TermRef     mul(TermRef lhs, TermRef rhs);
        TermRef     sub(TermRef lhs, TermRef rhs);
        TermRef     div(TermRef lhs, TermRef rhs);
        TermRef     pow(TermRef base, TermRef exp);

        TermRef     sin(TermRef arg);
        TermRef     cos(TermRef arg);
        TermRef     tan(TermRef arg);
        TermRef     asin(TermRef arg);
        TermRef     acos(TermRef arg);
        TermRef     atan(TermRef arg);
        TermRef     exp(TermRef arg);
        TermRef     sqrt(TermRef arg);
        TermRef     ln(TermRef arg);
        TermRef     log(TermRef arg);


        std::string str(TermRef i);

    protected:

        TermRef         register_expr(ExpressionPtr p_expr);
        ExpressionPtr   get(TermRef ref);


    public:
        // accessors
        std::string name() const { return _name; }
        unsigned    num_vars() const { return _num_vars; }
        unsigned    num_expressions() const { return _num_expressions; }
        unsigned    num_terms() const { return _terms.size(); }

        DifferentialVarPtr  get_d_var(TermRef i);
        ControlVarPtr       get_c_var(TermRef i);
        AlgebraicVarPtr     get_a_var(TermRef i);

    };
}
