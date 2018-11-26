#pragma once

#include <string>
#include <boost/python.hpp>

extern void define_context();

namespace  dp {
    namespace bp = boost::python;

    class PyContext {
        //types:
    public:
        typedef std::shared_ptr<PyContext>  ptr;
        //Constructors
    public:
        PyContext(std::string str);
        virtual ~PyContext();

        // Interface
    public:

    };
}
