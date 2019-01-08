#include <boost/python.hpp>

#include "context.hxx"
#include "differential_equation.hxx"
#include "ocp.hxx"
#include "optimization_algorithm.hxx"
#include "grids.hxx"

// IMPORTANT: The name in BOOST_PYTHON_MODULE(.) must match that of the .so library file.
BOOST_PYTHON_MODULE( _acado )
{
    define_grids();
    define_context();
    define_differential_equation();
    define_discretized_differential_equation();
    define_ocp();
    define_optimization_algorithm();
}
