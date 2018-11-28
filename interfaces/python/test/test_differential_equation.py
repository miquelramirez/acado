import pytest
import acado as ac

from .common import initialize_rocket_context

def test_differential_equation():
    ctx, X, U, odes, T = initialize_rocket_context()

    de_sys = ac.DifferentialEquation(ctx, 0.0, T)
    for x, de in odes:
        de_sys.set_ode(x, de)

    assert de_sys.num_dynamic_equations == 3
