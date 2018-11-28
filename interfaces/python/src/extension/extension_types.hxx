#pragma once

#include <memory>
#include <vector>
#include <acado/symbolic_expression/symbolic_expression.hpp>

namespace  ACADO {

    typedef std::shared_ptr<Expression>             ExpressionPtr;
    typedef std::shared_ptr<ConstraintComponent>    AtomPtr;

    typedef std::shared_ptr<AlgebraicState>     AlgebraicVarPtr;
    typedef std::shared_ptr<DifferentialState>  DifferentialVarPtr;
    typedef std::shared_ptr<Control>            ControlVarPtr;
    typedef std::shared_ptr<Disturbance>        DisturbanceVarPtr;
    typedef std::shared_ptr<IntegerControl>     IntControlVarPtr;
    typedef std::shared_ptr<IntegerParameter>   IntParameterVarPtr;
    typedef std::shared_ptr<Parameter>          ParameterPtr;
    typedef std::shared_ptr<TIME>               TimeVarPtr;
    typedef std::shared_ptr<IntermediateState>  IntermVarPtr;


    typedef int                     TermRef;
    typedef std::vector<TermRef>    TermRefVec;

    inline TermRef invalid_term() { return -1; }

    typedef int                     AtomRef;
    typedef std::vector<AtomRef>    AtomRefVec;

    inline AtomRef invalid_atom() { return -1; }
}
