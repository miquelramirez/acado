import pytest
import acado as ac

def test_differential_state_creation():
    print(dir(ac))
    ctx = ac.Context("test_differential_state")

    x = ctx.new_differential_state("x")
    y = ctx.new_differential_state("y")

    assert ctx.name == "test_differential_state"
    assert ctx.num_vars == 2
    assert ctx.num_expressions == 0
