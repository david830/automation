/***********************************************************************************************
 * Copyright (C) 2010 Anvesh Komuravelli, Paolo Zuliani and Edmund M. Clarke.  All rights reserved.
 * By using this software the USER indicates that he or she has read, understood and will comply
 * with the following:
 *
 * 1. The USER is hereby granted non-exclusive permission to use, copy and/or
 * modify this software for internal, non-commercial, research purposes only. Any
 * distribution, including commercial sale or license, of this software, copies of
 * the software, its associated documentation and/or modifications of either is
 * strictly prohibited without the prior consent of the authors. Title to copyright
 * to this software and its associated documentation shall at all times remain with
 * the authors. Appropriated copyright notice shall be placed on all software
 * copies, and a complete copy of this notice shall be included in all copies of
 * the associated documentation. No right is granted to use in advertising,
 * publicity or otherwise any trademark, service mark, or the name of the authors.
 *
 * 2. This software and any associated documentation is provided "as is".
 *
 * THE AUTHORS MAKE NO REPRESENTATIONS OR WARRANTIES, EXPRESSED OR IMPLIED,
 * INCLUDING THOSE OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT
 * USE OF THE SOFTWARE, MODIFICATIONS, OR ASSOCIATED DOCUMENTATION WILL NOT
 * INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER INTELLECTUAL PROPERTY
 * RIGHTS OF A THIRD PARTY.
 *
 * The authors shall not be liable under any circumstances for any direct,
 * indirect, special, incidental, or consequential damages with respect to any
 * claim by USER or any third party on account of or arising from the use, or
 * inability to use, this software or its associated documentation, even if the
 * authors have been advised of the possibility of those damages.
 * ***********************************************************************************************/


/**
 * inline methods vs making every field public and accessing directly :
 *   somehow the latter is faster (significantly, differing in runtimes of about
 *   a second)
 */

#ifndef _DEFS_HPP_
#define _DEFS_HPP_

#include "headers.hpp"

enum AROperator {ADD, SUB, MUL, DIV, EQ, NEQ, LT, LTE, GT, GTE, NOOP};
                                   // NOOP has two meanings
                                   // 1. in a Block, a logical (non-base) node
                                   // 2. in a Arithmetic, it's a real variable
                                   //    or a real value

enum Type {F, T, B,
           D_LR, C_LR,
           G_RF, G_R, G_F,
           F_RF, F_R, F_F,
           U_LRF, U_LR, U_LF, U_RF, U_L, U_R, U_F};

enum TriVal {FALSE, TRUE, UNDEF};

enum Direction {LEFT, RIGHT, FUTURE};

class Block;
class Arithmetic;

class Arithmetic {
  public:
    AROperator op; // can only be arithmetic or NOOP, the latter denoting a real variable
    string var; // name of the variable, in case `op' is NOOP
    double val; // real value of the arithmetic operator
    Arithmetic *L, *R, *G; // left, right, golden ; golden stands for the
                           // corresponding node in the parse_tree
    Block *PB; // block parent
    Arithmetic *PA; // arithmetic parent
    Direction dir; // left/right/future direction
    bool parent; // false for PB, true for PA ; Arithmetic has only one parent
    int gid; // id in the array of ground arithmetics, for initiating the propagation
    int id; // used only for variables ; as an index into the array for trace values
    bool recvdOnce; // denotes that a value is received from one of the children

    Arithmetic() {
      L = R = G = NULL;
      PA = NULL; PB = NULL;
      gid = -1; id = -1;
      recvdOnce = false;
    }

    /* used, only in parse tree construction */
    Arithmetic(AROperator _op, const string &_var, const double &_val, Arithmetic *Lnode, Arithmetic *Rnode) {
      op = _op; var = _var; val = _val;
      L = Lnode; R = Rnode; G = this;
      if (Lnode != NULL) {Lnode -> PA = this; dir = LEFT; parent = true;}
      if (Rnode != NULL) {Rnode -> PA = this; dir = RIGHT; parent = true;}
      PA = NULL; PB = NULL;
      gid = -1; id = -1;
      recvdOnce = false;
    }

    /* used, only in parse tree construction */
    Arithmetic(Arithmetic *_G, Block *_p, Direction _d) {
      op = _G -> op; var = _G -> var; val = _G -> val;
      L = R = NULL; G = this;
      PB = _p; dir = _d; parent = false; PA = NULL;
      gid = -1; id = -1;
      recvdOnce = false;
    }

    /* used, only in parse tree construction */
    Arithmetic(Arithmetic *_G, Arithmetic *_p, Direction _d) {
      op = _G -> op; var = _G -> var; val = _G -> val;
      L = R = NULL; G = this;
      PA = _p; dir = _d; parent = true; PB = NULL;
      gid = -1; id = -1;
      recvdOnce = false;
    }

    /* used in copySubtree */
    Arithmetic(Arithmetic *_G, Block *_p, Direction _d, int _gid) {
      op = _G -> op; var = _G -> var; val = _G -> val;
      L = R = NULL; G = _G;
      PB = _p; dir = _d; parent = false; PA = NULL;
      gid = _gid; id = _G -> id;
      recvdOnce = false;
    }

    /* used in copySubtree */
    Arithmetic(Arithmetic *_G, Arithmetic *_p, Direction _d, int _gid) {
      op = _G -> op; var = _G -> var; val = _G -> val;
      L = R = NULL; G = _G;
      PA = _p; dir = _d; parent = true; PB = NULL;
      gid = _gid; id = _G -> id;
      recvdOnce = false;
    }

    virtual ~Arithmetic() {}
};

class Block {
  public:
    AROperator op; // it can be a relational operator denoting a boolean
                   // proposition (`t' = B) or
                   // NOOP denoting a logical operator (or T/F)
                   //
                   // it can't be an arithmetic operator!
    Type t; // type of the block. children can be arithmetic only if t is B
    double bound; // bound of a temporal operator; negative value implies non-temporal
    double val; // to (temporarily) store values of children if
                // `t' is B and `op' is relational

    Block *LB, *RB, *F, *G; // left, right, future and golden
    Arithmetic *LA, *RA; // children can be blocks or arithmetic
    map<Block *, Direction> propagate; // a block can only propagate to blocks
    int tid; // id in the array of temporal blocks
    int id;  // block id of the Node in the parse tree of the input formula

    Block() {
      LB = RB = F = G = NULL; LA = RA = NULL;
      tid = id = -1;
    }

    /* used in expand */
    Block(AROperator _op, Type _t, const double &_bound, Block *_G,
          Block *_p, Direction _d, int _tid) {
      op = _op; t = _t; bound = _bound;
      G = _G; LB = RB = F = NULL; LA = RA = NULL;
      propagate[_p] = _d;
      tid = _tid; id = -1;
    }

    /* used in expanding from parse_tree */
    Block(Block *_G, int _tid) {
      op = _G -> op; t = _G -> t; bound = _G -> bound;
      G = _G; LB = RB = F = NULL; LA = RA = NULL;
      tid = _tid; id = G -> id;
    }

    /* used only in parse tree construction */
    Block(AROperator _op, Type _t, const double &_bound, Block *Lnode, Block *Rnode) {
      op = _op; t = _t; bound = _bound;
      LB = Lnode; RB = Rnode; F = NULL; G = this; LA = RA = NULL;
      if (Lnode != NULL) (Lnode -> propagate)[this] = LEFT;
      if (Rnode != NULL) (Rnode -> propagate)[this] = RIGHT;
      tid = -1; id = -1;
    }

    /* used only in parse tree construction */
    Block(AROperator _op, Type _t, const double &_bound, Arithmetic *Lnode, Arithmetic *Rnode) {
      op = _op; t = _t; bound = _bound;
      LA = Lnode; RA = Rnode; LB = RB = F = NULL; G = this;
      if (Lnode != NULL) {Lnode -> PB = this; Lnode -> dir = LEFT; Lnode -> parent = true;}
//      if (Rnode != NULL) {Lnode -> PB = this; Lnode -> dir = RIGHT; Lnode -> parent = true;}
      if (Rnode != NULL) {Rnode -> PB = this; Rnode -> dir = RIGHT; Rnode -> parent = true;}
      tid = -1; id = -1;
    }

    /* used only in parse tree construction */
    Block(AROperator _op, Type _t, const double &_bound, Block *_G) {
      op = _op; t = _t; bound = _bound;
      LA = RA = NULL; LB = RB = F = NULL; G = _G;
      tid = -1; id = -1;
    }

    /* used, only in parse tree construction */
    Block(Block *_G, Block *_p, Direction _d) {
      op = _G -> op; t = _G -> t; bound = _G -> bound;
      LB = RB = F = NULL; G = this; LA = RA = NULL;
      propagate[_p] = _d;
      tid = id = -1;
    }

    /* used in copySubtree */
    Block(Block *_G, Block *_p, Direction _d, int _tid) {
      op = _G -> op; t = _G -> t; bound = _G -> bound;
      LB = RB = F = NULL; G = _G; LA = RA = NULL;
      propagate[_p] = _d;
      tid = _tid; id = G -> id;
    }

    virtual ~Block() {}
};

extern vector<Arithmetic *> var; // vector of all Arithmetics denoting the real variables

extern vector<Block *> *current; // vector of blocks to be (possibly) expanded in future
                                 //   a block in this vector will not be
                                 //   expanded if it is determined that it
                                 //   doesn't have a future
extern vector<Block *> *current_old; // has the previous values of current
extern vector<Block *> ground_blocks; // list of blocks to start resolving from
extern vector<Arithmetic *> ground_arithmetics; // list of arithmetics to start resolving from
extern Block **future;       // array of size equal to that of the number of blocks in the parse tree
                              // ptr != NULL implies the corresponding subtree has
                              // already been created and our job is to just point to ptr,
                              // thereby making it a DAG
extern Block *parse_tree; // root of the parse-tree/DAG
                          //   it is now a DAG as we are allowing sharing of the
                          //   block nodes

extern TriVal result; // whether the property holds or not or can't be decided
extern double *obs; // observation from the trace

extern int blockCount; // number of arithmetic nodes
extern int current_size; // size of current (see above)
extern double ts_new, ts_old; // current and previous timestamps from the trace

extern void advance(); // function which consumes the new input from the trace

#endif
