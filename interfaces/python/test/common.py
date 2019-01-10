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

def initialize_discrete_time_system_context(h):
    ctx = ac.Context("discrete-time-systemp")

    s = ctx.new_differential_state("s")
    v = ctx.new_differential_state("v")
    m = ctx.new_differential_state("m")
    u = ctx.new_control_input("u")

    X = [s, v, m]
    U = [u]

    h_sym = ctx.constant(h)
    dot_s = ctx.add(s, ctx.mul(h_sym, v))
    dot_v = ctx.add(v, ctx.mul(h_sym, ctx.sub(u, ctx.mul(ctx.constant(0.02), ctx.mul(v, v)))))
    dot_m = ctx.sub(m, ctx.mul(h_sym, ctx.mul(ctx.constant(0.01), ctx.mul(u, u))))

    odes = [(s, dot_s), (v, dot_v), (m, dot_m)]

    stage_cost = ctx.mul(u, u)

    return ctx, X, U, odes, stage_cost

def setup_jojo_model():

    ctx = ac.Context("jojo")
    # the position of the hand
    x1 = ctx.new_differential_state("x1")
    x2 = ctx.new_differential_state("x2")
    # the velocity of the hand
    v1 = ctx.new_differential_state("v1")
    v2 = ctx.new_differential_state("v2")
    # the position of the jojo
    y1 = ctx.new_differential_state("y1")
    y2 = ctx.new_differential_state("y2")
    # the velocity of the jojo
    w1 = ctx.new_differential_state("w1")
    w2 = ctx.new_differential_state("w2")

    # the control action of the "hand"
    u1 = ctx.new_control_input("u1")
    u2 = ctx.new_control_input("u2")

    # timings of mode switches
    T1 = ctx.new_parameter("T1")
    T2 = ctx.new_parameter("T2")

    # model parameters
    m = 0.200 # mass of the jojo
    J = 1e-4 # inertia of the jojo
    r = 0.010 # coiling radius of the jojo
    g = 9.81 # gravitational constant
    a = 1e-2 # coiling friction
    L = 1.0 # length of the rope

    k = J/(m*r*r+J) # damping ratio of the jojo
    mu = 1.0 - k # effective mass ratio

    f1 = ac.DifferentialEquation(ctx, 0.0, 1.0)
    f1.set_ode(x1, ctx.mul(v1, T1))
    f1.set_ode(v1, ctx.mul(u1, T1))
    f1.set_ode(y1, ctx.mul(w1, T1))
    w11 = ctx.mul(ctx.mul(ctx.constant(-1.0), ctx.constant(mu)), ctx.constant(g))
    w21 = ctx.mul(ctx.constant(k), u1)
    w31 = ctx.mul(ctx.constant(a), ctx.sub(v1, w1))
    f1.set_ode(w1, ctx.mul(ctx.add(w11, ctx.add(w21, w31)),T1))
    f1.set_ode(x2, ctx.mul(v2, T2))
    f1.set_ode(v2, ctx.mul(u2, T2))
    f1.set_ode(y2, ctx.mul(w2, T2))
    w12 = ctx.mul(ctx.mul(ctx.constant(-1.0), ctx.constant(mu)), ctx.constant(g))
    w22 = ctx.mul(ctx.constant(k), u2)
    w32 = ctx.mul(ctx.constant(a), ctx.sub(v2, w2))
    f1.set_ode(w2, ctx.mul(ctx.add(w12, ctx.add(w22, w32)),T2))


    z1 = ctx.mul(ctx.constant(k), v1)
    psi = ctx.add(ctx.mul(z1,z1), ctx.mul(ctx.constant(k), ctx.sub(ctx.mul(w1,w1), ctx.mul(ctx.constant(2.0), ctx.mul(w1, v1)))))
    a1 = ctx.add(z1, ctx.sqrt(psi))
    a2 = ctx.sub(x2, y2)

    return ctx, [[x1, v1, y1, w1],[x2, v2, y2, w2]], [[u1],[u2]], a1, [T1, T2], f1, ctx.add(ctx.mul(ctx.mul(u1,u1),T1),ctx.mul(ctx.mul(u2,u2),T2))
