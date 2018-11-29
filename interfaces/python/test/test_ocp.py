import pytest
import acado as ac

from .common import initialize_rocket_context

def test_ocp_rocket_setup():

    ctx, X, U, odes, T = initialize_rocket_context()

    de_sys = ac.DifferentialEquation(ctx, 0.0, T)
    for x, de in odes:
        de_sys.set_ode(x, de)

    ocp = ac.OCP(ctx, 0.0, T, 20)

    ocp.set_terminal_cost(T)

    ocp.add_continuous_diff_constraints(de_sys)

    ocp.add_initial_constraint(ctx.equal(X[0], 0.0))
    ocp.add_initial_constraint(ctx.equal(X[1], 0.0))
    ocp.add_initial_constraint(ctx.equal(X[2], 1.0))

    ocp.add_terminal_constraint(ctx.equal(X[0], 10.0))
    ocp.add_terminal_constraint(ctx.equal(X[1], 0.0))

    ocp.add_constraint(ctx.bound(-0.1, X[1], 1.7))
    ocp.add_constraint(ctx.bound(-1.1, U[0], 1.1))
    ocp.add_constraint(ctx.bound(5.0, T, 15.0))

    #assert False
    assert de_sys.num_dynamic_equations == 3

def test_ocp_rocket_solve():

    ctx, X, U, odes, T = initialize_rocket_context()

    de_sys = ac.DifferentialEquation(ctx, 0.0, T)
    for x, de in odes:
        de_sys.set_ode(x, de)

    ocp = ac.OCP(ctx, 0.0, T, 20)

    ocp.set_terminal_cost(T)

    ocp.add_continuous_diff_constraints(de_sys)

    ocp.add_initial_constraint(ctx.equal(X[0], 0.0))
    ocp.add_initial_constraint(ctx.equal(X[1], 0.0))
    ocp.add_initial_constraint(ctx.equal(X[2], 1.0))

    ocp.add_terminal_constraint(ctx.equal(X[0], 10.0))
    ocp.add_terminal_constraint(ctx.equal(X[1], 0.0))

    ocp.add_constraint(ctx.bound(-0.1, X[1], 1.7))
    ocp.add_constraint(ctx.bound(-1.1, U[0], 1.1))
    ocp.add_constraint(ctx.bound(5.0, T, 15.0))

    solver = ac.OptimizationAlgorithm(ocp)
    solver.max_num_iterations = 20


    #solver.init()
    solver.solve()

    assert de_sys.num_dynamic_equations == 3
    assert solver.objective_value == 7.4417405995042
