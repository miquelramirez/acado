#include <boost/python.hpp>

#include "context.hxx"
#include "differential_equation.hxx"

// IMPORTANT: The name in BOOST_PYTHON_MODULE(.) must match that of the .so library file.
BOOST_PYTHON_MODULE( _acado )
{
    define_context();
    define_differential_equation();
}
