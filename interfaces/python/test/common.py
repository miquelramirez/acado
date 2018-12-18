import acado as ac

def initialize_rocket_context():
    ctx = ac.Context("Rocket")
    s = ctx.new_differential_state("s")
    v = ctx.new_differential_state("v")
    m = ctx.new_differential_state("m")
    u = ctx.new_control_input("u")
    T = ctx.new_parameter("T")

    dot_s = v
    dot_v = ctx.div(ctx.sub(u, ctx.mul(ctx.constant(0.2), ctx.mul(v, v))), m)
    dot_m = ctx.mul( ctx.constant(-0.01), ctx.mul(u, u))

    X = [s, v, m]
    U = [u]
    odes = [ (v, dot_v), (s, dot_s), (m, dot_m)]
    return ctx, X, U, odes, T

def min_time_rocket_init():
    time_grid = ac.Grid(0.0, 1.0, 11)
    x_init = ac.VariablesGrid(3, time_grid)

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

    return x_init, u_init, p_init

def initialize_semi_implicit_dae_context():
    ctx = ac.Context("semi-implicit-DAE")

    x = ctx.new_differential_state("x")
    l = ctx.new_differential_state("l")
    z = ctx.new_algebraic_state("z")
    u = ctx.new_control_input("u")

    X = [x, l, z]
    U = [u]

    dot_x = ctx.sub(ctx.add(ctx.mul(ctx.constant(0.5), ctx.mul(x, x)),
                    ctx.add(u, ctx.mul(ctx.constant(0.5), z))), x)
    dot_l = ctx.add(ctx.mul(x, x), ctx.mul(ctx.constant(3.0), ctx.mul(u, u)))
    odes = [(x, dot_x), (l, dot_l)]

    zx_coupling = ctx.add(ctx.sub(ctx.add(z, ctx.exp(z)), ctx.constant(1.0)), x)
    impl_odes = [ ctx.equal(zx_coupling, ctx.constant(0.0))]

    return ctx, X, U, odes, impl_odes
