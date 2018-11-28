import acado as ac

def initialize_rocket_context():
    ctx = ac.Context("Rocket")
    v = ctx.new_differential_state("v")
    s = ctx.new_differential_state("s")
    m = ctx.new_differential_state("m")
    u = ctx.new_control_input("u")
    T = ctx.new_parameter("T")

    dot_s = v
    dot_v = ctx.div(ctx.sub(u, ctx.mul(ctx.constant(0.2), ctx.mul(v, v))), m)
    dot_m = ctx.mul( ctx.constant(-0.01), ctx.mul(u, u))

    X = [v, s, m]
    U = [u]
    odes = [ (v, dot_v), (s, dot_s), (m, dot_m)]
    return ctx, X, U, odes, T
