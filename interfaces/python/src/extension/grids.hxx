#pragma once

#include <boost/python.hpp>
#include <acado/variables_grid/grid.hpp>
#include <acado/variables_grid/variables_grid.hpp>


extern void define_grids();

namespace ACADO {

    namespace bp = boost::python;

    class PyGrid : public Grid {
        //types
    public:
        typedef std::shared_ptr<PyGrid>      ptr;

        // constructors
    public:
        PyGrid( double t0, double tf, int N);
        virtual ~PyGrid();
    };

    class PyVariablesGrid : public VariablesGrid {
        // types
    public:
        typedef std::shared_ptr<PyVariablesGrid>    ptr;

        // constructors
    public:
        PyVariablesGrid( unsigned nvars, PyGrid::ptr grid );
        virtual ~PyVariablesGrid();

        // interface
    public:
        void set_vector(unsigned i, bp::list r);
        bp::list get_vector(unsigned i);
        bp::tuple shape();
    };

}
