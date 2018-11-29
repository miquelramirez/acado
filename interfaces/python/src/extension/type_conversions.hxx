#pragma once

#include <boost/python.hpp>
#include <acado/variables_grid/variables_grid.hpp>

namespace ACADO {
    namespace bp = boost::python;

    inline bp::list
    variable_grid_to_nested_list( const VariablesGrid& grid, int num_cols ) {
        bp::list py_outer_list;
        for (unsigned k = 0; k < grid.getNumPoints(); k++ ) {
            bp::list x_k = bp::list();
            for (int j = 0; j < num_cols; j++)
                x_k.append(grid(k,j));
            py_outer_list.append(x_k);
        }
        return py_outer_list;
    }
}
