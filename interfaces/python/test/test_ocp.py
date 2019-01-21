
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import pytest

import acado as ac


from .common import initialize_rocket_context, min_time_rocket_init,\
    initialize_semi_implicit_dae_context, initialize_discrete_time_system_context,\
    setup_jojo_model

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

    plt.savefig('test_rocket_min_time.trajectory.pdf')

    assert de_sys.num_dynamic_equations == 3
    assert solver.objective_value == 7.4417405995042

def test_ocp_rocket_with_init():

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

    x0, u0, p0 = min_time_rocket_init()

    solver = ac.OptimizationAlgorithm(ocp)
    solver.max_num_iterations = 20
    #solver.init()
    solver.initialize_differential_states(x0)
    solver.initialize_controls(u0)
    solver.initialize_parameters(p0)

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

    plt.savefig('test_rocket_min_time.with_init.trajectory.pdf')

    assert de_sys.num_dynamic_equations == 3
    assert solver.objective_value == 7.4417405995046995

def test_ocp_semi_implicit_dae():
    ctx, X, U, odes, impl_odes = initialize_semi_implicit_dae_context()

    de_sys = ac.DifferentialEquation(ctx, 0.0, 10.0)
    for x, de in odes:
        de_sys.set_ode(x, de)
    for eq in impl_odes:
        de_sys.set_implicit_ode(eq)

    ocp = ac.OCP(ctx, 0.0, 10.0, 10)

    ocp.set_terminal_cost(X[1])

    ocp.add_continuous_diff_constraints(de_sys)

    ocp.add_initial_constraint(ctx.equal(X[0], 1.0))
    ocp.add_initial_constraint(ctx.equal(X[1], 0.0))

    solver = ac.OptimizationAlgorithm(ocp)
    solver.absolute_tolerance = 1e-7
    solver.integrator_tolerance = 1e-7
    solver.hessian_approximation = ac.HessianApproximation.EXACT_HESSIAN

    solver.solve()

    times = np.array(solver.t) * (solver.tf-solver.t0) + solver.t0
    tau_Xd = np.hstack([np.array(x_k).reshape(len(X[:2]),1) for x_k in solver.Xd])
    tau_Xa = np.hstack([np.array(x_k).reshape(len(X[2:]),1) for x_k in solver.Xa])
    tau_U = np.hstack([np.array(u_k).reshape(len(U),1) for u_k in solver.U])

    fig, axes = plt.subplots(3,1)
    axes[0].grid(True)
    axes[0].set_xlabel('time')
    for x_dim in range(tau_Xd.shape[0]):
        # Plot a single line
        ys = tau_Xd[x_dim,:]
        axes[0].plot(times, ys, label='x_{}'.format(x_dim))
    axes[0].legend()

    axes[1].grid(True)
    axes[1].set_xlabel('time')
    for x_dim in range(tau_Xa.shape[0]):
        # Plot a single line
        ys = tau_Xa[x_dim,:]
        axes[1].plot(times, ys, label='z_{}'.format(x_dim))
    axes[1].legend()

    axes[2].grid(True)
    axes[2].set_xlabel('time')
    for x_dim in range(tau_U.shape[0]):
        # Plot a single line
        ys = tau_U[x_dim,:]
        axes[2].plot(times, ys, label='u_{}'.format(x_dim))
    axes[2].legend()

    plt.savefig('semi-implicit-dae.trajectory.pdf')

def test_ocp_discrete_time():

    t_start = 0.0
    t_end = 10.0
    h = 0.01

    ctx, X, U, odes, stage_cost = initialize_discrete_time_system_context(h)

    de_sys = ac.DiscretizedDifferentialEquation(ctx, h)
    for x, de in odes:
        de_sys.set_ode(x, de)

    ocp = ac.OCP(ctx, t_start, t_end, 50)

    ocp.set_stage_cost(stage_cost)

    ocp.add_discrete_diff_constraints(de_sys)

    ocp.add_initial_constraint(ctx.equal(X[0], 0.0))
    ocp.add_initial_constraint(ctx.equal(X[1], 0.0))
    ocp.add_initial_constraint(ctx.equal(X[2], 1.0))

    ocp.add_terminal_constraint(ctx.equal(X[0], 10.0))
    ocp.add_terminal_constraint(ctx.equal(X[1], 0.0))

    ocp.add_constraint(ctx.bound(-0.01, X[1], 1.7))

    solver = ac.OptimizationAlgorithm(ocp)
    #solver.max_num_iterations = 20
    solver.hessian_approximation = ac.HessianApproximation.EXACT_HESSIAN
    solver.kkt_tolerance = 1e-10
    #solver.init()
    solver.solve()

    # plot results
    times = np.array(solver.t) * (solver.tf-solver.t0) + solver.t0
    tau_X = np.hstack([np.array(x_k[:len(X)]).reshape(len(X),1) for x_k in solver.Xd])
    tau_J = np.hstack([np.array(x_k[len(X):]).reshape(1,1) for x_k in solver.Xd])
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

    plt.savefig('test_discrete_time_rocket_min_energy.trajectory.pdf')

    assert de_sys.num_dynamic_equations == 3
    assert solver.objective_value == 1.2084178048256953

def test_hybrid_ocp():
    ctx, X, U, a1, timings, trans, stage_cost = setup_jojo_model()

    ocp = ac.OCP(ctx, 0.0, 1.0, 20)
    ocp.set_stage_cost(stage_cost)

    ocp.add_continuous_diff_constraints(trans)

    for xi in X[0]:
        ocp.add_initial_constraint(ctx.equal(xi, 0.0))

    ocp.add_terminal_constraint(ctx.equal(ctx.sub(X[0][0],X[0][2]), 1.0)) # L = 1.0
    ocp.add_terminal_constraint(ctx.equal(X[1][0], -0.1))
    ocp.add_terminal_constraint(ctx.equal(X[1][1], 0.0))
    ocp.add_terminal_constraint(ctx.equal(ctx.sub(X[1][0],X[1][2]), 0.0))
    ocp.add_terminal_constraint(ctx.equal(X[1][3], 0.0))


    # jump constraints
    ocp.add_boundary_constraint(0.0, X[1][0], ctx.mul(ctx.constant(-1.0), X[0][0]), 0.0)
    ocp.add_boundary_constraint(0.0, X[1][1], ctx.mul(ctx.constant(-1.0), X[0][1]), 0.0)
    ocp.add_boundary_constraint(0.0, X[1][2], ctx.mul(ctx.constant(-1.0), X[0][2]), 0.0)
    ocp.add_boundary_constraint(0.0, X[1][3], ctx.mul(ctx.constant(-1.0), a1), 0.0)

    ocp.add_constraint(ctx.bound(-10.0, U[0][0], 10.0))
    ocp.add_constraint(ctx.bound(-10.0, U[1][0], 10.0))

    ocp.add_constraint( ctx.bound(0.1, timings[0], 2.0))
    ocp.add_constraint( ctx.bound(0.1, timings[1], 4.0))

    solver = ac.OptimizationAlgorithm(ocp)
    solver.integrator_type = ac.IntegratorType.INT_RK45
    solver.discretization_type = ac.DiscretizationType.MULTIPLE_SHOOTING
    solver.kkt_tolerance = 1e-10
    solver.absolute_tolerance = 1e-4
    solver.integrator_tolerance = 1e-8
    solver.max_num_iterations = 1000
    solver.solve()

    # plot results
    times = np.array(solver.t) * (solver.tf-solver.t0) + solver.t0
    tau_X = np.hstack([np.array(x_k[:2*len(X[0])]).reshape(2*len(X[0]),1) for x_k in solver.Xd])
    tau_J = np.hstack([np.array(x_k[2*len(X[0]):]).reshape(1,1) for x_k in solver.Xd])
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

    plt.savefig('test_hybrid_ocp.trajectory.pdf')

    assert solver.objective_value == 28.790760787152017
