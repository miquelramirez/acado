import pytest
import acado as ac

def test_differential_state_creation():
    ctx = ac.Context("test_differential_state")

    x = ctx.new_differential_state("x")
    y = ctx.new_differential_state("y")

    assert ctx.name == "test_differential_state"
    assert ctx.num_vars == 2
    assert ctx.num_expressions == 0

def test_control_variable_creation():
    ctx = ac.Context("test_control")

    u = ctx.new_control_input("u")

    assert ctx.name == "test_control"
    assert ctx.num_vars == 1
    assert ctx.num_expressions == 0

def test_algebraic_variable_creation():
    ctx = ac.Context("test_algebraic")

    z = ctx.new_algebraic_state("z")

    assert ctx.name == "test_algebraic"
    assert ctx.num_vars == 1
    assert ctx.num_expressions == 0

def test_constant_creation():

    ctx = ac.Context("dummy")

    t = ctx.constant(2.0)

    assert ctx.num_vars == 0

def test_multiple_vars():

    ctx = ac.Context("rocket")

    v = ctx.new_differential_state("x")
    s = ctx.new_differential_state("s")
    m = ctx.new_differential_state("m")
    u = ctx.new_control_input("u")

    assert ctx.num_vars == 4
    assert ctx.num_terms == 4

def test_add():

    ctx = ac.Context("Rocket")

    v = ctx.new_differential_state("x")
    s = ctx.new_differential_state("s")
    m = ctx.new_differential_state("m")
    u = ctx.new_control_input("u")

    expr1 = ctx.add(s, v)

    assert ctx.num_terms == 5
    assert ctx.str(expr1) == "[ (xd[6]+xd[5])]"

def test_mul():

    ctx = ac.Context("Rocket")

    v = ctx.new_differential_state("v")
    s = ctx.new_differential_state("s")
    m = ctx.new_differential_state("m")
    u = ctx.new_control_input("u")

    ode = ctx.mul( ctx.constant(0.2), ctx.mul(v, v))

    assert ctx.num_terms == 7
    assert ctx.str(ode) == '[ ((real_t)(0.2)*(xd[8]*xd[8]))]'

def test_rocket_equations():

    ctx = ac.Context("Rocket")

    v = ctx.new_differential_state("v")
    s = ctx.new_differential_state("s")
    m = ctx.new_differential_state("m")
    u = ctx.new_control_input("u")

    dot_s = v
    dot_v = ctx.div(ctx.sub(u, ctx.mul(ctx.constant(0.2), ctx.mul(v, v))), m)
    dot_m = ctx.mul( ctx.constant(-0.01), ctx.mul(u, u))

    assert ctx.num_terms == 12
