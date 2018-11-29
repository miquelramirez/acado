import pytest
import acado as ac

def test_time_grid_creation():

    time_grid = ac.Grid(0.0, 1.0, 11)

def test_non_trivial_grid():

    time_grid = ac.Grid(0.0, 1.0, 11)
    x_init = ac.VariablesGrid(3, time_grid)

    print(x_init.shape)
    assert x_init.shape == (3,11)

    x_init.set_vector(0, [0.0, 0.0, 1.0])
    x_init.set_vector(1, [0.299, 0.79, 0.99])
    x_init.set_vector(2, [1.13, 1.42, 0.981])
    x_init.set_vector(3, [2.33, 1.69, 0.975])
    x_init.set_vector(4, [3.60, 1.7, 0.973])
    x_init.set_vector(5, [4.86, 1.7, 0.970])
    x_init.set_vector(6, [6.13, 1.7, 0.968])
    x_init.set_vector(7, [7.39, 1.7, 0.965])
    x_init.set_vector(8, [8.66, 1.7, 0.963])
    x_init.set_vector(9, [9.67, 0.898, 0.958])
    x_init.set_vector(10, [1.0, 0.0, 0.949])

    u_init = ac.VariablesGrid(1, time_grid)
    u_init.set_vector(0, [1.1])
    u_init.set_vector(1, [1.1])
    u_init.set_vector(2, [1.1])
    u_init.set_vector(3, [0.578])
    u_init.set_vector(4, [0.578])
    u_init.set_vector(5, [0.578])
    u_init.set_vector(6, [0.578])
    u_init.set_vector(7, [0.578])
    u_init.set_vector(8, [-0.212])
    u_init.set_vector(9, [-1.1])
    u_init.set_vector(10, [-1.1])

    p_init = ac.VariablesGrid(1, time_grid)
    p_init.set_vector(0, [7.44])
