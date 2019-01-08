
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import pytest

import acado as ac
from acado.tarski import translate
import tarski
from tarski.theories import Theory
from tarski.syntax import *
from tarski.io import rddl
from tarski.model import Model
from tarski.evaluators.simple import evaluate
from tarski.syntax.arithmetic import *
from tarski.syntax.arithmetic.special import *
from tarski.syntax.arithmetic.random import *

def test_ocp_rocket_setup():

    lang = tarski.language('rocket', [Theory.EQUALITY, Theory.ARITHMETIC, Theory.SPECIAL])

    # differential states
    s = lang.function('s', lang.Real)
    v = lang.function('v', lang.Real)
    m = lang.function('m', lang.Real)
    T = lang.function('T', lang.Real)

    u = lang.function('u', lang.Real)

    # differential constraints
    F = [ ( s(), v() ), \
        (v(), (u() - 0.2 * v() * v()) / m()), \
        (m(), -0.01 * u() * u())
    ]

    X0 = [ s() == 0.0, v() == 0.0, m() == 1.0 ]
    G = [ s() == 10.0, v() == 0.0 ]
    C = [ -0.1 <= v(), v() <= 1.7, -1.1 <= u(), u() <= 1.1, 5.0 <= T(), T() <= 15.0]

    # Sorting of symbols
    ctx = ac.Context("Rocket")
    syms = {}
    syms[symref(s())] = ctx.new_differential_state(str(s()))
    syms[symref(v())] = ctx.new_differential_state(str(v()))
    syms[symref(m())] = ctx.new_differential_state(str(m()))
    syms[symref(u())] = ctx.new_control_input(str(u()))
    syms[symref(T())] = ctx.new_parameter(str(T()))

    de_sys = ac.DifferentialEquation(ctx, 0.0, translate(ctx, syms, T()))
    for x, de in F:
        lhs = translate(ctx, syms, x)
        rhs = translate(ctx, syms, de)
        de_sys.set_ode(lhs, rhs)

    ocp = ac.OCP(ctx, 0.0, translate(ctx, syms, T()), 20)

    ocp.set_terminal_cost(syms[symref(T())])

    ocp.add_continuous_diff_constraints(de_sys)

    for phi in X0:
        ocp.add_initial_constraint(translate(ctx, syms, phi))

    for phi in G:
        ocp.add_terminal_constraint(translate(ctx, syms, phi))

    for phi in C:
        ocp.add_constraint(translate(ctx, syms, phi))

    solver = ac.OptimizationAlgorithm(ocp)
    #solver.max_num_iterations = 20
    solver.solve()

    # plot results
    times = np.array(solver.t) * (solver.tf-solver.t0) + solver.t0
    tau_X = np.hstack([np.array(x_k).reshape(3,1) for x_k in solver.Xd])
    tau_U = np.hstack([np.array(u_k).reshape(1,1) for u_k in solver.U])

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

    plt.savefig('test_tarski_rocket_min_time.trajectory.pdf')

    assert de_sys.num_dynamic_equations == 3
    assert solver.objective_value == 6.915820156613334#7.4417405995042
