
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import pytest

import acado as ac


from .common import initialize_rocket_context, min_time_rocket_init

def test_rocket_precise_integrator():
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
    solver.integrator_type = ac.IntegratorType.INT_RK78
    solver.integrator_tolerance = 1e-8
    solver.discretization_type = ac.DiscretizationType.SINGLE_SHOOTING
    solver.kkt_tolerance = 1e-4
    #solver.init()
    solver.solve()

    # plot results
    times = np.array(solver.t) * (solver.tf-solver.t0) + solver.t0
    tau_X = np.hstack([np.array(x_k).reshape(len(X),1) for x_k in solver.Xd])
    tau_U = np.hstack([np.array(u_k).reshape(len(U),1) for u_k in solver.U])

    fig, axes = plt.subplots(2,1)
    axes[0].grid(True)
    axes[0].set_xlabel('time')
    for x_dim in range(tau_X.shape[0]):
        # Plot a single line
        ys = tau_X[x_dim,:]
        axes[0].plot(times, ys, label='x_{}'.format(x_dim))
    axes[0].legend()

    axes[1].grid(True)
    axes[1].set_xlabel('time')
    for x_dim in range(tau_U.shape[0]):
        # Plot a single line
        ys = tau_U[x_dim,:]
        axes[1].plot(times, ys, label='u_{}'.format(x_dim))
    axes[1].legend()

    plt.savefig('test_rocket_min_time.trajectory.rk78.pdf')

    assert de_sys.num_dynamic_equations == 3
    assert solver.objective_value == 7.44174065832654
