#include "grids.hxx"
#include <stdexcept>
#include <sstream>

namespace ACADO {

    PyGrid::PyGrid( double t0, double tf, int N)
        : Grid(t0, tf, N) {}

    PyGrid::~PyGrid() {}

    PyVariablesGrid::PyVariablesGrid(unsigned nv, PyGrid::ptr grid)
        : VariablesGrid(nv, *grid) {}

    PyVariablesGrid::~PyVariablesGrid() {
    }

    bp::tuple
    PyVariablesGrid::shape() {
        return bp::make_tuple(getNumValues(), getNumPoints());
    }

    void
    PyVariablesGrid::set_vector(unsigned i, bp::list r) {
        if ( i >= getNumPoints() ) {
            std::stringstream buffer;
            buffer << "VariablesGrid.set_row: row index out of bounds" << std::endl;
            buffer << "# rows: is " << getNumPoints() << " index is " << i << std::endl;
            throw std::logic_error(buffer.str());
        }
        if (len(r) != getNumValues()) {
            std::stringstream buffer;
            buffer << "VariablesGrid.set_row: mismatched number of variables" << std::endl;
            buffer << "# vars: is " << getNumValues() << " size is: " << len(r) << std::endl;
            throw std::logic_error(buffer.str());
        }
        for ( unsigned k = 0; k < len(r); k++ )
            this->operator()(i,k) = bp::extract<double>(r[k]);
    }

    bp::list
    PyVariablesGrid::get_vector(unsigned i) {
        if ( i >= getNumPoints() ) {
            std::stringstream buffer;
            buffer << "VariablesGrid.set_row: row index out of bounds" << std::endl;
            buffer << "# rows: is " << getNumPoints() << " index is " << i << std::endl;
            throw std::logic_error(buffer.str());
        }
        DVector v_i = getVector(i);
        bp::list r_i;
        for ( unsigned k = 0; k < getNumRows(i); k++) {
            r_i.append(v_i[k]);
        }
        return r_i;
    }

}

void define_grids() {
    using namespace ACADO;

    bp::class_< PyGrid,
                PyGrid::ptr,
                boost::noncopyable>("Grid",
                                    bp::init<double, double, int>())
        ;

    bp::class_< PyVariablesGrid,
                PyVariablesGrid::ptr,
                boost::noncopyable>("VariablesGrid",
                                    bp::init<int, PyGrid::ptr>())
        .add_property("shape", &PyVariablesGrid::shape)
        .def("set_vector", &PyVariablesGrid::set_vector)
        .def("get_vector", &PyVariablesGrid::get_vector)
        ;


}
