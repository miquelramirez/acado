# -*- coding: utf-8 -*-
import acado as ac
from tarski.syntax import *
from tarski.syntax.arithmetic import *
from tarski.syntax.arithmetic.special import *
from tarski.syntax.arithmetic.random import *

class TranslationError(Exception):
    pass

class NoOutputGenerated(Exception):
    pass

tarski_syms = {
    BuiltinFunctionSymbol.ADD : ac.Context.add,
    BuiltinFunctionSymbol.SUB : ac.Context.sub,
    BuiltinFunctionSymbol.MUL : ac.Context.mul,
    BuiltinFunctionSymbol.DIV : ac.Context.div,
    BuiltinFunctionSymbol.POW : ac.Context.pow,
    BuiltinFunctionSymbol.SIN : ac.Context.sin,
    BuiltinFunctionSymbol.COS : ac.Context.cos,
    BuiltinFunctionSymbol.TAN : ac.Context.tan,
    BuiltinFunctionSymbol.ASIN: ac.Context.asin,
    BuiltinFunctionSymbol.ATAN : ac.Context.atan,
    BuiltinFunctionSymbol.EXP : ac.Context.exp,
    BuiltinFunctionSymbol.LOG : ac.Context.log,
    BuiltinFunctionSymbol.SQRT : ac.Context.sqrt
}

def translate(context: ac.Context, syms: dict, expr):

    if isinstance(expr, CompoundTerm):
        if expr.symbol.builtin:
            trans_st = [translate(context, syms, st) for st in expr.subterms]
            if len(trans_st) == 2:
                try:
                    return tarski_syms[expr.symbol.symbol](context, trans_st[0], trans_st[1])
                except KeyError:
                    raise TranslationError("translate(): can't translate '{}'".format(expr.symbol.symbol))
            assert len(trans_st) == 1
            try:
                return tarski_syms[expr.symbol.symbol](context, trans_st[0])
            except KeyError:
                raise TranslationError("translate(): can't translate '{}'".format(expr.symbol.symbol))
        try:
            return syms[symref(expr)]
        except KeyError:
            raise TranslationError("translate(): symbol '{}' has not been registered!".format(expr))
    elif isinstance(expr, Constant):
        try:
            return syms[symref(expr)]
        except KeyError:
            tr = context.constant(expr.symbol)
            syms[symref(expr)] = tr
            return tr
    elif isinstance(expr, float):
        try:
            return syms[expr]
        except KeyError:
            tr = context.constant(expr)
            syms[expr] = tr
    elif isinstance(expr, Atom):

        if expr.predicate.symbol == BuiltinPredicateSymbol.EQ:
            if isinstance(expr.subterms[0], CompoundTerm):
                lhs = translate(context, syms, expr.subterms[0])
                if isinstance(expr.subterms[1], Constant):
                    rhs = expr.subterms[1].symbol
                elif isinstance(expr.subterms[1], float):
                    rhs = expr.subterms[1]
                else:
                    raise TranslationError('translate(): cannot translate x == a where a is not constant')
                return context.equal(lhs, rhs)
            raise TranslationError('translate(): cannot translate a == b where a is not a term')
        elif expr.predicate.symbol == BuiltinPredicateSymbol.LE:
            if isinstance(expr.subterms[0], CompoundTerm):
                lhs = translate(context, syms, expr.subterms[0])
                if isinstance(expr.subterms[1], Constant):
                    rhs = expr.subterms[1].symbol
                elif isinstance(expr.subterms[1], float):
                    rhs = expr.subterms[1]
                else:
                    raise TranslationError('translate(): cannot translate x <= a, where a is not constant')
                return context.upper_bound(lhs, rhs)
            elif isinstance(expr.subterms[1], CompoundTerm):
                rhs = translate(context, syms, expr.subterms[1])
                if isinstance(expr.subterms[0], Constant):
                    lhs = expr.subterms[0].symbol
                elif isinstance(expr.subterms[0], float):
                    lhs = expr.subterms[0]
                else:
                    raise TranslationError('translate(): cannot translate a <= x, where a is not constant')
                return context.lower_bound(lhs, rhs)

        elif expr.predicate.symbol == BuiltinPredicateSymbol.GE:
            st = [ expr.subterms[1], expr.subterms[0]]
            if isinstance(st[0], CompoundTerm):
                lhs = translate(context, syms, st[0])
                if isinstance(st[1], Constant):
                    rhs = st[1].symbol
                elif isinstance(st[1], float):
                    rhs = st[1]
                else:
                    raise TranslationError('translate(): cannot translate a >= x, where a is not constant')
                return context.upper_bound(lhs, rhs)
            elif isinstance(st[1], CompoundTerm):
                rhs = translate(context, syms, st[1])
                if isinstance(st[0], Constant):
                    lhs = st[0].symbol
                elif isinstance(st[0], float):
                    lhs = st[0]
                else:
                    raise TranslationError('translate(): cannot translate a >= x, where a is not constant')
                return context.lower_bound(lhs, rhs)
            print(expr.predicate.symbol, BuiltinPredicateSymbol.GE, expr.predicate.symbol == BuiltinPredicateSymbol.GE)
            raise TranslationError('translate(): cannot translate expression "{}" at least one of subterms "{}", "{}" needs to be compound'.format(expr, str(st[0], st[1])))
        elif isinstance(expr, Tautology):
            raise NoOutputGenerated()
        elif isinstance(expr, Contradiction):
            raise NoOutputGenerated()
    raise TranslationError("translate(): could not translate {}".format(expr))
