#include <boost/python.hpp>

#include "context.hxx"
#include "lti_system.hxx"
#include "qr_cost_function.hxx"
#include "ddp_lqr.hxx"
#include "generic_system.hxx"
#include "generic_cost_function.hxx"
#include "ddp_generic.hxx"

// IMPORTANT: The name in BOOST_PYTHON_MODULE(.) must match that of the .so library file.
BOOST_PYTHON_MODULE( _acado )
{
    define_context();
}
