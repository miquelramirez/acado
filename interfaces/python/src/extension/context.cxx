#include "context.hxx"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace ACADO {

    PyContext::PyContext(std::string name)
        : _name(name),
        _num_vars(0), _num_expressions(0) {}

    PyContext::~PyContext() {}

    TermRef
    PyContext::register_expr(ExpressionPtr p_expr) {
        TermRef current = _terms.size();
        _terms.push_back(p_expr);
        return current;
    }

    ExpressionPtr
    PyContext::get(TermRef i) {
        return _terms.at(i);
    }

    TermRef
    PyContext::constant(double v) {
        auto c = std::make_shared<Expression>(v);
        TermRef c_ref = register_expr(c);
        return c_ref;
    }

    TermRef
    PyContext::new_differential_state( std::string name ) {
        auto x = std::make_shared<DifferentialState>(name, 1, 1);
        TermRef x_ref = register_expr(x);
        _diff_vars[x_ref] = x;
        _num_vars++;
        return x_ref;
    }

    TermRef
    PyContext::new_control_input(std::string name) {
        auto u = std::make_shared<Control>(name, 1, 1);
        TermRef u_ref = register_expr(u);
        _control_vars[u_ref] = u;
        _num_vars++;
        return u_ref;
    }

    TermRef
    PyContext::new_algebraic_state(std::string name) {
        auto z = std::make_shared<AlgebraicState>(name, 1, 1);
        TermRef z_ref = register_expr(z);
        _algebraic_vars[z_ref] = z;
        _num_vars++;
        return z_ref;
    }

    DifferentialVarPtr
    PyContext::get_d_var(TermRef i) {
        auto it = _diff_vars.find(i);
        if (it == _diff_vars.end())
            throw std::logic_error("Term is not a differential state variable!");
        return it->second;
    }

    ControlVarPtr
    PyContext::get_c_var(TermRef i) {
        auto it = _control_vars.find(i);
        if (it == _control_vars.end())
            throw std::logic_error("Term is not a control state variable!");
        return it->second;
    }

    AlgebraicVarPtr
    PyContext::get_a_var(TermRef i) {
        auto it = _algebraic_vars.find(i);
        if (it == _algebraic_vars.end())
            throw std::logic_error("Term is not an algebraic state variable!");
        return it->second;
    }

    std::string
    PyContext::str(TermRef i) {
        try {
            auto p_expr = get(i);
            std::stringstream buffer;
            buffer << *p_expr;
            return buffer.str();
        } catch (std::out_of_range) {
            std::stringstream buffer;
            buffer << "Term " << i << " not registered as an expression (registered terms: " << num_terms() << ")" << std::endl;
            throw std::out_of_range(buffer.str());
        }

    }

    TermRef
    PyContext::add(TermRef lhs, TermRef rhs) {
        auto lhs_expr = get(lhs);
        auto rhs_expr = get(rhs);
        auto p_expr = std::make_shared<Expression>((*lhs_expr)+(*rhs_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::mul(TermRef lhs, TermRef rhs) {
        auto lhs_expr = get(lhs);
        auto rhs_expr = get(rhs);
        auto p_expr = std::make_shared<Expression>((*lhs_expr)*(*rhs_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::sub(TermRef lhs, TermRef rhs) {
        auto lhs_expr = get(lhs);
        auto rhs_expr = get(rhs);
        auto p_expr = std::make_shared<Expression>((*lhs_expr)-(*rhs_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::div(TermRef lhs, TermRef rhs) {
        auto lhs_expr = get(lhs);
        auto rhs_expr = get(rhs);
        auto p_expr = std::make_shared<Expression>((*lhs_expr)/(*rhs_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::pow(TermRef base, TermRef power) {
        auto base_expr = get(base);
        auto exp_expr = get(power);
        auto p_expr = std::make_shared<Expression>(::pow((*base_expr),(*exp_expr)));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::sin(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::sin(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::cos(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::cos(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::tan(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::tan(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::asin(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::asin(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::acos(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::acos(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::atan(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::atan(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::exp(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::exp(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::sqrt(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::sqrt(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::ln(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::ln(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }

    TermRef
    PyContext::log(TermRef arg) {
        auto arg_expr = get(arg);
        auto p_expr = std::make_shared<Expression>(::log(*arg_expr));
        TermRef e = register_expr(p_expr);
        _expressions[e] = p_expr;
        return e;
    }
}

void define_context() {
    using namespace ACADO;

    bp::class_<PyContext, PyContext::ptr, boost::noncopyable >("Context",
        bp::init<std::string>())
        .def("new_differential_state", &PyContext::new_differential_state)
        .def("new_control_input", &PyContext::new_control_input)
        .def("new_algebraic_state", &PyContext::new_algebraic_state)
        .def("constant", &PyContext::constant)
        .def("str", &PyContext::str)
        .add_property("num_vars", &PyContext::num_vars)
        .add_property("num_expressions", &PyContext::num_expressions)
        .add_property("num_terms", &PyContext::num_terms)
        .add_property("name", &PyContext::name)
        .def("add", &PyContext::add)
        .def("sub", &PyContext::sub)
        .def("mul", &PyContext::mul)
        .def("div", &PyContext::div)
        .def("pow", &PyContext::pow)
        .def("sin", &PyContext::sin)
        .def("cos", &PyContext::cos)
        .def("tan", &PyContext::tan)
        .def("asin", &PyContext::asin)
        .def("acos", &PyContext::acos)
        .def("atan", &PyContext::atan)
        .def("exp", &PyContext::exp)
        .def("sqrt", &PyContext::sqrt)
        .def("ln", &PyContext::ln)
        .def("log", &PyContext::log)
        ;
}
