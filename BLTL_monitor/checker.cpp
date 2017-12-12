
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


#include "defs.hpp"

vector<Block *> *current, // blocks which "may" be expanded in future
                *current_old, // blocks of current in the previous step
                *tb; // used for swapping current and current_old
int current_size, current_old_size; // sizes of current and current_old

vector<Arithmetic *> ground_arithmetics; // variable and real-number nodes which start propagation
Block **future; // array of blocks corresponding to parse tree (with no change
                // in bounds, or non-temporal blocks) to enable some sharing
                // for e.g., if we are expanding F[t] p, t becomes t' but "p"
                // will remain the same. Now if F[t]p is a subformula of a
                // bigger formula, say G[t1](F[t]p), an expansion of the G
                // operator into future may change t1 to t1' but down the line,
                // both share the same 'p'. In this case, 'p' is added to
                // "future". Basically, anything which doesn't result directly
                // from "expansion" (like G[t1'] or F[t']) is added to "future".

double ts_new, duration, ts_old = 0; // previous and current timestamps and the difference
int tid, gida; // indices into current and ground_arithmetics
Block *gtrue, *gfalse; // ground true and false blocks in the current step
TriVal result; // initialize to UNDEF

void changeNresolve(Block *);
void remove(Block *,Block *);
void remove(Arithmetic *);
void pull(Block *, Block *);
void propagate(Block *, Direction, TriVal);
void propagate(Block *, Direction, const double &);
void propagate(Arithmetic *, Direction, const double &);
void resolve();
void copySubtree(Block *);
void copySubtree(Arithmetic *);
void expand();
void advance();

/*void printCurrentOld() {
  int index = 0;
  if (current_old != NULL) {
  for (vector<Node *>::iterator it = current_old.begin(); it != current_old.end(); it++, index++) {
    cout << index << ":";
    if (*it == NULL) {
      cout << "NULL";
    } else {
      cout << (*it) -> b -> t;
    }
    cout << "  ";
  }
  cout << endl;
  }
}

void printCurrent() {
  int index = 0;
  if (current != NULL) {
  for (vector<Node *>::iterator it = current.begin(); it != current.end(); it++, index++) {
    cout << index << ":";
    if (*it == NULL) {
      cout << "NULL";
    } else {
      cout << (*it) -> b -> t;
    }
    cout << "  ";
  }
  cout << endl;
  }
}*/

// if b is realized to have no future, after seeing the new timestamp, change
// its type and propagate the value up
void changeNresolve(Block *b) {
  map<Block *, Direction> &m = b -> propagate;
  map<Block *, Direction>::iterator m_it;
  switch (b -> t) {
  case F_F:
  case U_F:
    if (m.empty()) {
      result = FALSE;
    } else {
      for (m_it = m.begin(); m_it != m.end(); m_it++) {
        propagate(m_it -> first, m_it -> second, FALSE);
      }
    }
    delete b;
    break;
  case G_F:
    if (m.empty()) {
      result = TRUE;
    } else {
      for (m_it = m.begin(); m_it != m.end(); m_it++) {
        propagate(m_it -> first, m_it -> second, TRUE);
      }
    }
    delete b;
    break;
  case F_RF:
  case G_RF:
  case U_RF:
    pull(b -> RB, b);
    break;
  case U_LRF:
    remove(b -> LB, b);
    b -> LB = NULL;
    pull(b -> RB, b);
    break;
  case U_LF:
    remove(b -> LB, b);
    if (m.empty()) {
      result = FALSE;
    } else {
      for (m_it = m.begin(); m_it != m.end(); m_it++) {
        propagate(m_it -> first, m_it -> second, FALSE);
      }
    }
    delete b;
    break;
  default:
    assert(0);
  }
}

// r need not propagate to b anymore. remove b from r's propagate list and if
// the list becomes empty, delete r and whatever useless in its subtree
void remove(Block *r, Block *b) {
  int _tid, _id;
  Block *lb, *rb, *temp, *next;
  Type t;
  Arithmetic *la, *ra;
  if (r == NULL) return; // nothing to remove!

  if (r == gfalse) {
    _id = r -> id;
    if (future[_id] == r) {
      future[_id] = NULL;
    }
    delete gfalse;
    gfalse = NULL;
    delete r;
  } else if (r == gtrue) {
    _id = r -> id;
    if (future[_id] == r) {
      future[_id] = NULL;
    }
    delete gtrue;
    gtrue = NULL;
    delete r;
  }

  map<Block *, Direction> &m = r -> propagate;
  m.erase(m.find(b)); // remmove b from r's propagate list
  if (m.empty()) {
    _tid = r -> tid;
    _id = r -> id;
    if (_tid >= 0) {
      /* r can be in at most one of current, current_old. delete it */
      if (current_size > _tid) {
        if (current->at(_tid) == r) {
          current->at(_tid) = NULL;
        } else if (current_old_size > _tid) {
          if (current_old->at(_tid) == r) {
            current_old->at(_tid) = NULL;
          }
        }
      } else if (current_old_size > _tid) {
        if (current_old->at(_tid) == r) {
          current_old->at(_tid) = NULL;
        }
      }
    }
    if (_id >= 0) {
      if (future[_id] == r) {
        future[_id] = NULL;
      }
    }
    t = r -> t;
    lb = r -> LB; la = r -> LA;
    rb = r -> RB; ra = r -> RA;
    temp = r;
    next = r -> F;
    delete r;

    // be careful! temp is already deleted, so no dereferencing or member accessing!
    if (t != B) {
      remove(lb, temp); remove(rb, temp);
    } else {
      remove(la); remove(ra);
    }
    remove(next, temp);
  }
}

// as we are not sharing arithmetics (which implies arithmetics have only one
// node to propagate to), removing a parent from r's propagate list
// is the same as deleting the subtree of r
void remove(Arithmetic *r) {
  int _gid;
  Arithmetic *left, *right;
  if (r == NULL) return; // nothing to remove!

  _gid = r -> gid;
  if (_gid >= 0) {
    ground_arithmetics.at(_gid) = NULL;
  }
  left = r -> L;
  right = r -> R;
  delete r;
  remove(left);
  remove(right);
}

// b's value is the same as p's value. so, no need to have two different blocks
// and have p propagate the value to b - we can just club them!
//
// the name "pull" may not be obvious. basically, b's propagate list is merged
// with that of p
void pull(Block *p, Block *b) {
  // we take the simple strategy of adding all propagates of b to that of p
  // (appropriately) and deleting b (from current/current_old and itself)!
  //
  // Note that b depends only on p and
  // not on anyone else. So, if b is present in the propagate list of any other
  // block/arithmetic, either it already got propagated to b, or b would have
  // sent a "remove" to that node. In both cases, we need not worry!

  map<Block *, Direction> &mp = p -> propagate;
  map<Block *, Direction> &mb = b -> propagate;
  map<Block *, Direction>::iterator p_it, b_it;
  list<pair<Block *, Block *> > l;
  Block *b1;
  Direction d1, d2;

  mp.erase(mp.find(b)); // p need not propagate to b anymore!

  for (b_it = mb.begin(); b_it != mb.end(); b_it++) {
    // if p also propagates to the node where b propagates to, take appropriate
    // action. o.w., simply add it to the propagate list of p

    b1 = b_it -> first;
    d1 = b_it -> second;
#ifdef ASSERTS
    assert(b1 != NULL);
#endif
    p_it = mp.find(b1);
    if (p_it == mp.end()) {
      // p wouldn't have propagated to b1
      mp[b1] = d1;
      // as p doesn't propagate to b1, b1 doesn't have p as L, R or F
      // change the link from b to p
      if (d1 == LEFT) {
        b1 -> LB = p;
      } else if (d1 == RIGHT) {
        b1 -> RB = p;
      } else { // FUTURE
        b1 -> F = p;
      }
    } else {
      d2 = p_it -> second;
#ifdef ASSERTS
      assert(d1 != d2);
#endif
      switch (b1 -> t) {
      case D_LR:
      case C_LR:
      case F_RF:
      case G_RF:
      case U_LF:
      case U_LR:
      case U_RF:
        l.push_back(make_pair(p, b1)); // pull up the block 'p' to b1
        break;
      case U_LRF:
        if (d1 != LEFT && d2 != LEFT) { // RIGHT and FUTURE
          remove(b1 -> LB, b1);
          l.push_back(make_pair(p, b1));
        } else if (d1 != RIGHT && d2 != RIGHT) { // LEFT and FUTURE
          // keep the direction of p as we are not deleting p
          if (d2 == LEFT) {
            b1 -> t = U_LR;
            b1 -> F = NULL;
          } else { // d2 == FUTURE
            b1 -> t = U_RF;
            b1 -> LB = NULL;
          }
        }
        break;
      default:
        assert(0);
      }
    }
  }

  // remove b from current/current_old
  int _tid;
  _tid = b -> tid;
  if (_tid >= 0) {
    if (current_size > _tid) {
      if (current->at(_tid) == b) {
        current->at(_tid) = NULL;
      } else if (current_old_size > _tid) {
        if (current_old->at(_tid) == b) {
          current_old->at(_tid) = NULL;
        }
      }
    } else if (current_old_size > _tid) {
      if (current_old->at(_tid) == b) {
        current_old->at(_tid) = NULL;
      }
    }
  } // b can't be ground, so need not check

  delete b;

  for (list<pair<Block *, Block *> >::iterator l_it = l.begin(); l_it != l.end(); l_it++) {
    pull(l_it -> first, l_it -> second);
  }
}

//void pull(Block *p, Block *b) {
//  map<Block *, Direction> &mp = p -> propagate;
//  map<Block *, Direction> &mb = b -> propagate;
//  map<Block *, Direction> &mt = b -> propagate; // temporary map reference
//                                                // pointing to b -> propagate
//                                                // just to initialize!
//  map<Block *, Direction>::iterator p_it, b_it, t_it;
//  list<pair<Block *, Block *> > l;
//  Block *b1, *b2;
//  Arithmetic *a1;
//  int _tid, _gid, _id;
//
//  _tid = b -> tid;
//  if (_tid >= 0) {
//    if (current_size > _tid) {
//      if (current->at(_tid) == b) {
//        current->at(_tid) = NULL;
//      } else if (current_old_size > _tid) {
//        if (current_old->at(_tid) == b) {
//          current_old->at(_tid) = NULL;
//        }
//      }
//    } else if (current_old_size > _tid) {
//      if (current_old->at(_tid) == b) {
//        current_old->at(_tid) = NULL;
//      }
//    }
//  } // b can't be in ground, so need not check
//
//  b -> op = p -> op;
//  b -> t = p -> t;
//  b -> bound = p -> bound;
//  b -> val = p -> val; // if you are pulling a B block
//
//#ifdef ASSERTS
//  assert(b -> type != B); // if it is B, it can't have a Block as a child or future
//#endif
//
//  if (p -> t != B) { // p has Blocks as children and future
//    /* left child */
//    b1 = b -> LB = p -> LB;
//    if (b1) {
//      mt = b1 -> propagate;
//      mt.erase(mt.find(p));
//      t_it = mt.find(b);
//      if (t_it == mt.end()) {
//        mt[b] = LEFT;
//      } else {
//        b2 = b -> F;
//        if (b2 != NULL) {
//          remove(b2, b);
//        }
//#ifdef ASSERTS
//        assert(t_it -> second == RIGHT);
//          // it can't be future as b1 can't be b's future as well as p's child
//#endif
//        l.push_back(make_pair(b1, b));
//      }
//    }
//    /* right child */
//    b1 = b -> RB = p -> RB;
//    if (b1) {
//      mt = b1 -> propagate;
//      mt.erase(mt.find(p));
//      t_it = mt.find(b);
//      if (t_it == mt.end()) {
//        mt[b] = RIGHT;
//      } else {
//        b2 = b -> F;
//        if (b2 != NULL) {
//          remove(b2, b);
//        }
//#ifdef ASSERTS
//        assert(t_it -> second == LEFT);
//          // it can't be future as b1 can't be b's future as well as p's child
//#endif
//        l.push_back(make_pair(b1, b));
//      }
//    }
//  } else { // p has Arithmetic children
//    b -> LB = b -> RB = NULL; // b might have had some children
//    /* left child */
//    a1 = b -> LA = p -> LA;
//    if (a1) {
//      mt = a1 -> propagate_blocks;
//      mt.erase(mt.find(p));
//      mt[b] = LEFT;
//      // it's impossible for a1 to be a child of b
//    }
//    /* right child */
//    a1 = b -> RA = p -> RA;
//    if (a1) {
//      mt = a1 -> propagate_blocks;
//      mt.erase(mt.find(p));
//      mt[b] = RIGHT;
//      // it's impossible for a1 to be a child of b
//    }
//  }
//
//  b1 = b -> F = p -> F;
//  if (b1) {
//    mt = b1 -> propagate;
//    mt.erase(mt.find(p));
//    mt[b] = FUTURE;
//  }
//
//  b -> G = p -> G;
//
//  _tid = p -> tid; _gid = p -> gid; _id = p -> id;
//  b -> tid = _tid;
//  b -> gid = _gid;
//  b -> id = _id;
//
//  for (p_it = mp.begin(); p_it != mp.end(); p_it++) {
//    b1 = p_it -> first;
//    if (b1 == b) { // this is the whole point of pulling the node!!
//      continue;
//    }
//    b_it = mb.find(b1);
//    if (b_it == mb.end()) {
//      mb[b1] = p_it -> second;
//    } else {
//#ifdef ASSERTS
//      assert((p_it -> second) != (b_it -> second)); // directions have to be different!
//                                                    // o.w., a block has two
//                                                    // other blocks connected in
//                                                    // the same direction!
//#endif
//      switch (b1 -> t) {
//      case F_RF:
//      case G_RF:
//      case U_RF:
//        /*if (n_it -> second != RIGHT) {
//          (n -> propagate)[node] = RIGHT;
//          node -> R = n;
//        }
//        node -> F = NULL;*/
//        l.push_back(make_pair(b, b1));
//        break;
//      case U_LRF:
//        if ((p_it -> second != LEFT) && (b_it -> second != LEFT)) { // RIGHT && FUTURE
//          /*if (n_it -> second != RIGHT) {
//            (n -> propagate)[node] = RIGHT;
//            node -> R = n;
//          }
//          node -> F = NULL;*/
//          remove(b1 -> LB, b1);
//          //node -> L = NULL;
//          l.push_back(make_pair(b, b1));
//        } else if ((p_it -> second != RIGHT) && (b_it -> second != RIGHT)) { // LEFT && FUTURE
//          if (b_it -> second != FUTURE) { // it has to be LEFT !
//            (b -> propagate)[b1] = FUTURE;
//            b1 -> F = b;
//          }
//          b1 -> LB = NULL;
//          b -> t = U_RF; // it can be U_LR either
//        } else {
//          // LEFT && RIGHT - impossible
//          assert(0);
//        }
//        break;
//      case U_LF:
//        /*if (n_it -> second != LEFT) {
//          (n -> propagate)[node] = LEFT;
//          node -> L = n;
//        }
//        node -> F = NULL;*/
//        l.push_back(make_pair(b, b1));
//        break;
//      default:
//        assert(0);
//      }
//    }
//  }
//
//  if (_tid >= 0) {
//    if (current_size > _tid) {
//      if (current->at(_tid) == p) {
//        current->at(_tid) = b;
//      } else if (current_old_size > _tid) {
//        if (current_old->at(_tid) == p) {
//          current_old->at(_tid) = b;
//        }
//      }
//    } else if (current_old_size > _tid) {
//      if (current_old->at(_tid) == p) {
//        current_old->at(_tid) = b;
//      }
//    }
//  } else if (_gid >= 0) {
//#ifdef ASSERTS
//    assert(ground_blocks.at(_gid) == p);
//#endif
//    ground_blocks.at(_gid) = b;
//  }
//  if (_id >= 0) {
//    if (future[_id] == p) {
//      future[_id] = b;
//    }
//  }
//
//  delete p;
//    
//  for (list<pair<Block *, Block *> >::iterator l_it = l.begin(); l_it != l.end(); l_it++) {
//    pull(l_it -> first, l_it -> second);
//  }
//}

// tv is propagted to b from its subtree in the direction d
//   given that, consume the information tv, and propagate your value if you get
//   reduced
void propagate(Block *b, Direction d, TriVal tv) {
#ifdef ASSERTS
  assert(tv != UNDEF);
  assert(b != NULL);
#endif

  Block *b1;
  map<Block *, Direction> &m = b -> propagate;
  map<Block *, Direction>::iterator m_it;
  int _tid, _id;

  switch (b -> t) {
  case D_LR:
    if (tv == FALSE) {
      if (d == LEFT) {
        pull(b -> RB, b);
      } else if (d == RIGHT) {
        pull(b -> LB, b);
      }
      else {
        assert(0);
      }
    } else { // tv == TRUE
      if (d == LEFT) {
        remove(b -> RB, b);
      } else if (d == RIGHT) {
        remove(b -> LB, b);
      }
      else {
        assert(0);
      }
#ifdef ASSERTS
      assert(b -> tid == -1);
#endif
      if (m.empty()) {
        result = TRUE;
      } else {
        for (m_it = m.begin(); m_it != m.end(); m_it++) {
          propagate(m_it -> first, m_it -> second, TRUE);
        }
      }
      delete b;
    }
    break;

  case C_LR:
    if (tv == FALSE) {
      if (d == LEFT) {
        remove(b -> RB, b);
      } else if (d == RIGHT) {
        remove(b -> LB, b);
      }
      else {
        assert(0);
      }
#ifdef ASSERTS
      assert(b -> tid == -1);
#endif
      if (m.empty()) {
        result = FALSE;
      } else {
        for (m_it = m.begin(); m_it != m.end(); m_it++) {
          propagate(m_it -> first, m_it -> second, FALSE);
        }
      }
      delete b;
    } else {
      if (d == LEFT) {
        pull(b -> RB, b);
      } else if (d == RIGHT) {
        pull(b -> LB, b);
      } else {
        assert(0);
      }
    }
    break;

  case F_RF:
    if (tv == FALSE) {
      if (d == RIGHT) {
        b1 = b -> F;
        if (b1 == NULL) {
          b -> RB = NULL;
          b -> t = F_F;
        } else {
          pull(b1, b);
        }
      } else if (d == FUTURE) {
        pull(b -> RB, b);
      } else {
        assert(0);
      }
    } else {
      if (d == RIGHT) {
        remove(b -> F, b);
      } else if (d == FUTURE) {
        remove(b -> RB, b);
      } else {
        assert(0);
      }
      _tid = b -> tid; _id = b -> id;
      if (current_size > _tid) {
        if (current->at(_tid) == b) {
          current->at(_tid) = NULL;
        } else if (current_old_size > _tid) {
          if (current_old->at(_tid) == b) {
            current_old->at(_tid) = NULL;
          }
        }
      } else if (current_old_size > _tid) {
        if (current_old->at(_tid) == b) {
          current_old->at(_tid) = NULL;
        }
      }
      if (_id >= 0) {
        if (future[_id] == b) {
          future[_id] = NULL;
        }
      }
      if (m.empty()) {
        result = TRUE;
      } else {
        for (m_it = m.begin(); m_it != m.end(); m_it++) {
          propagate(m_it -> first, m_it -> second, TRUE);
        }
      }
      delete b;
    }
    break;

  case G_RF:
    if (tv == FALSE) {
      if (d == RIGHT) {
        remove(b -> F, b);
      } else if (d == FUTURE) {
        remove(b -> RB, b);
      } else {
        assert(0);
      }
      _tid = b -> tid; _id = b -> id;
      if (current_size > _tid) {
        if (current->at(_tid) == b) {
          current->at(_tid) = NULL;
        } else if (current_old_size > _tid) {
          if (current_old->at(_tid) == b) {
            current_old->at(_tid) = NULL;
          }
        }
      } else if (current_old_size > _tid) {
        if (current_old->at(_tid) == b) {
          current_old->at(_tid) = NULL;
        }
      }
      if (_id >= 0) {
        if (future[_id] == b) {
          future[_id] = NULL;
        }
      }
      if (m.empty()) {
        result = FALSE;
      } else {
        for (m_it = m.begin(); m_it != m.end(); m_it++) {
          propagate(m_it -> first, m_it -> second, FALSE);
        }
      }
      delete b;
    } else {
      if (d == RIGHT) {
        b1 = b -> F;
        if (b1 == NULL) {
          b -> RB = NULL;
          b -> t = G_F;
        } else {
          pull(b1, b);
        }
      } else if (d == FUTURE) {
        pull(b -> RB, b);
      } else {
        assert(0);
      }
    }
    break;

  case U_LRF:
    if (d == LEFT) {
      if (tv == FALSE) {
        remove(b -> F, b);
        pull(b -> RB, b);
      } else {
        b -> LB = NULL;
        b -> t = U_RF;
      }
    } else if (d == RIGHT) {
      if (tv == FALSE) {
        b -> RB = NULL;
        b -> t = U_LF;
      } else {
        remove(b -> LB, b);
        remove(b -> F, b);
        _tid = b -> tid; _id = b -> id;
        if (current_size > _tid) {
          if (current->at(_tid) == b) {
            current->at(_tid) = NULL;
          } else if (current_old_size > _tid) {
            if (current_old->at(_tid) == b) {
              current_old->at(_tid) = NULL;
            }
          }
        } else if (current_old_size > _tid) {
          if (current_old->at(_tid) == b) {
            current_old->at(_tid) = NULL;
          }
        }
        if (_id >= 0) {
          if (future[_id] == b) {
            future[_id] = NULL;
          }
        }
        if (m.empty()) {
          result = TRUE;
        } else {
          for (m_it = m.begin(); m_it != m.end(); m_it++) {
            propagate(m_it -> first, m_it -> second, TRUE);
          }
        }
        delete b;
      }
    } else { // FUTURE
      if (tv == FALSE) {
        remove(b -> LB, b);
        pull(b -> RB, b);
      } else {
        b -> F = NULL;
        b -> t = U_LR;
        // future already existed and we removed it now. So, no risk of it being
        // misused by its possible presence in current (impossible) or current_old
      }
    }
    break;

  case U_LR:
    if (d == LEFT) {
      if (tv == FALSE) {
        pull(b -> RB, b);
      } else {
        remove(b -> RB, b);
        // for sure, it's not in current or current_old or future
        if (m.empty()) {
          result = TRUE;
        } else {
          for (m_it = m.begin(); m_it != m.end(); m_it++) {
            propagate(m_it -> first, m_it -> second, TRUE);
          }
        }
        delete b;
      }
    } else { // RIGHT
      if (tv == FALSE) {
        pull(b -> LB, b);
      } else {
        remove(b -> LB, b);
        // for sure, it's not in current or current_old or future
        if (m.empty()) {
          result = TRUE;
        } else {
          for (m_it = m.begin(); m_it != m.end(); m_it++) {
            propagate(m_it -> first, m_it -> second, TRUE);
          }
        }
        delete b;
      }
    }
    break;

  case U_LF:
    if (d == LEFT) {
      if (tv == FALSE) {
        remove(b -> F, b);
        _tid = b -> tid; _id = b -> id;
        if (current_size > _tid) {
          if (current->at(_tid) == b) {
            current->at(_tid) = NULL;
          } else if (current_old_size > _tid) {
            if (current_old->at(_tid) == b) {
              current_old->at(_tid) = NULL;
            }
          }
        } else if (current_old_size > _tid) {
          if (current_old->at(_tid) == b) {
            current_old->at(_tid) = NULL;
          }
        }
        if (_id >= 0) {
          if (future[_id] == b) {
            future[_id] = NULL;
          }
        }
        if (m.empty()) {
          result = FALSE;
        } else {
          for (m_it = m.begin(); m_it != m.end(); m_it++) {
            propagate(m_it -> first, m_it -> second, FALSE);
          }
        }
        delete b;
      } else {
        b1 = b -> F;
        if (b1 == NULL) {
          b -> t = U_F;
        } else {
          pull(b1, b);
        }
      }
    } else {
      if (tv == FALSE) {
        remove(b -> LB, b);
        _tid = b -> tid; _id = b -> id;
        if (current_size > _tid) {
          if (current->at(_tid) == b) {
            current->at(_tid) = NULL;
          } else if (current_old_size > _tid) {
            if (current_old->at(_tid) == b) {
              current_old->at(_tid) = NULL;
            }
          }
        } else if (current_old_size > _tid) {
          if (current_old->at(_tid) == b) {
            current_old->at(_tid) = NULL;
          }
        }
        if (_id >= 0) {
          if (future[_id] == b) {
            future[_id] = NULL;
          }
        }
        if (m.empty()) {
          result = FALSE;
        } else {
          for (m_it = m.begin(); m_it != m.end(); m_it++) {
            propagate(m_it -> first, m_it -> second, FALSE);
          }
        }
        delete b;
      } else {
        pull(b -> LB, b);
      }
    }
    break;

  case U_RF:
    if (d == RIGHT) {
      if (tv == FALSE) {
        b1 = b -> F;
        if (b1 == NULL) {
          b -> t = U_F;
        } else {
          pull(b1, b);
        }
      } else {
        remove(b -> F, b);
        _tid = b -> tid; _id = b -> id;
        if (current_size > _tid) {
          if (current->at(_tid) == b) {
            current->at(_tid) = NULL;
          } else if (current_old_size > _tid) {
            if (current_old->at(_tid) == b) {
              current_old->at(_tid) = NULL;
            }
          }
        } else if (current_old_size > _tid) {
          if (current_old->at(_tid) == b) {
            current_old->at(_tid) = NULL;
          }
        }
        if (_id >= 0) {
          if (future[_id] == b) {
            future[_id] = NULL;
          }
        }
        if (m.empty()) {
          result = TRUE;
        } else {
          for (m_it = m.begin(); m_it != m.end(); m_it++) {
            propagate(m_it -> first, m_it -> second, TRUE);
          }
        }
        delete b;
      }
    } else {
      if (tv == FALSE) {
        pull(b -> RB, b);
      } else {
        remove(b -> RB, b);
        _tid = b -> tid; _id = b -> id;
        if (current_size > _tid) {
          if (current->at(_tid) == b) {
            current->at(_tid) = NULL;
          } else if (current_old_size > _tid) {
            if (current_old->at(_tid) == b) {
              current_old->at(_tid) = NULL;
            }
          }
        } else if (current_old_size > _tid) {
          if (current_old->at(_tid) == b) {
            current_old->at(_tid) = NULL;
          }
        }
        if (_id >= 0) {
          if (future[_id] == b) {
            future[_id] = NULL;
          }
        }
        if (m.empty()) {
          result = TRUE;
        } else {
          for (m_it = m.begin(); m_it != m.end(); m_it++) {
            propagate(m_it -> first, m_it -> second, TRUE);
          }
        }
        delete b;
      }
    }
    break;
  default:
    assert(0);
  }
}

// val is propagated to a in the direction d from below
void propagate(Arithmetic *a, Direction d, const double &val) {
#ifdef ASSERTS
  assert(a != NULL);
  assert(d != FUTURE);
#endif
  double aVal;
  
  if (a -> recvdOnce) {
    /* binary operator */
    switch (a -> op) {
    case ADD:
      aVal = (a -> val) + val;
      break;
    case SUB:
      if (d == LEFT) {
        aVal = val - (a -> val);
      } else if (d == RIGHT) {
        aVal = (a -> val) - val;
      }
      break;
    case MUL:
      aVal = (a -> val) * val;
      break;
    case DIV:
      if (d == LEFT) {
        aVal = val / (a -> val);
      } else if (d == RIGHT) {
        aVal = (a -> val) / val;
      }
      break;
    default:
      assert(0);
    }
    if (! a -> parent) { // parent is block
      propagate(a -> PB, a -> dir, aVal);
    } else {
      propagate(a -> PA, a -> dir, aVal);
    }
    delete a;
  } else {
    if ((d == LEFT && a -> R == NULL) ||
        (d == RIGHT && a -> L == NULL)) { // unary operator
      switch (a -> op) {
      case ADD:
        aVal = val;
        break;
      case SUB:
        aVal = -val;
        break;
      default:
        assert(0);
      }
      if (! a -> parent) { // parent is block
        propagate(a -> PB, a -> dir, aVal);
      } else {
        propagate(a -> PA, a -> dir, aVal);
      }
      delete a;
    } else {
      a -> recvdOnce = true;
      a -> val = val;
      if (d == LEFT) {
        a -> L = NULL;
      } else if (d == RIGHT) {
        a -> R = NULL;
      }
    }
  }
}

// val is propagate to b in the direction d
//   b has to have type B to get real values propagated to it
void propagate(Block *b, Direction d, const double &val) {
#ifdef ASSERTS
  assert(d != FUTURE);
#endif

  map<Block *, Direction> &m = b -> propagate;
  map<Block *, Direction>::iterator m_it;

#ifdef ASSERTS
  assert(b != NULL);
  assert(b -> t == B);
#endif
  TriVal bVal;
  int _id;
  if (b -> LA && b -> RA) { // both are non-null => first propagation up!
    b -> val = val;
    if (d == LEFT) {
      b -> LA = NULL;
    } else if (d == RIGHT) {
      b -> RA = NULL;
    }
  } else {
    switch (b -> op) {
    case EQ:
      bVal = (val == (b -> val)) ? TRUE : FALSE;
      break;
    case NEQ:
      bVal = (val != (b -> val)) ? TRUE : FALSE;
      break;
    case LT:
      if (d == LEFT) {
        bVal = (val < (b -> val)) ? TRUE : FALSE;
      } else if (d == RIGHT) {
        bVal = ((b -> val) < val) ? TRUE : FALSE;
      }
      break;
    case LTE:
      if (d == LEFT) {
        bVal = (val <= (b -> val)) ? TRUE : FALSE;
      } else if (d == RIGHT) {
        bVal = ((b -> val) <= val) ? TRUE : FALSE;
      }
      break;
    case GT:
      if (d == LEFT) {
        bVal = (val > (b -> val)) ? TRUE : FALSE;
      } else if (d == RIGHT) {
        bVal = ((b -> val) > val) ? TRUE : FALSE;
      }
      break;
    case GTE:
      if (d == LEFT) {
        bVal = (val >= (b -> val)) ? TRUE : FALSE;
      } else if (d == RIGHT) {
        bVal = ((b -> val) >= val) ? TRUE : FALSE;
      }
      break;
    default:
      assert(0);
    }
    _id = b -> id;
    if (_id >= 0) {
      if (future[_id] == b) {
        future[_id] = NULL;
      }
    }
    if (m.empty()) {
      result = bVal;
    } else {
      for (m_it = m.begin(); m_it != m.end(); m_it++) {
        propagate(m_it -> first, m_it -> second, bVal);
      }
    }
    delete b;
  }
}

// start resolving from the ground blocks and arithmetics
void resolve() {
  Arithmetic *a;
  Block *b;
  Direction dir;
  map<Block *, Direction>::iterator mb_it;
  map<Arithmetic *, Direction>::iterator ma_it;
  double aVal;
  TriVal bVal;
  int _id;

  for (vector<Arithmetic *>::iterator it = ground_arithmetics.begin();
       it != ground_arithmetics.end(); it++) {
    a = (*it);
    if (a == NULL) {
      continue;
    }
#ifdef ASSERTS
    assert(a != NULL);
    assert(a -> op == NOOP);
#endif
    _id = a -> id;
    if (_id == -1) {
      aVal = a -> val;
    } else {
      aVal = obs[_id];
    }
    if (! a -> parent) {
      propagate(a -> PB, a -> dir, aVal);
    } else {
      propagate(a -> PA, a -> dir, aVal);
    }
    delete a;
    if (result != UNDEF) {
      return;
    }
  }

  if (gfalse != NULL) {
    map<Block *, Direction> &mb = gfalse -> propagate;
    _id = gfalse -> id;
    if (future[_id] == gfalse) {
      future[_id] = NULL;
    }
    if (mb.empty()) {
      result = FALSE;
    } else {
      for (mb_it = mb.begin(); mb_it != mb.end(); mb_it++) {
        propagate(mb_it -> first, mb_it -> second, FALSE);
      }
    }
    delete gfalse;
    gfalse = NULL;
    if (result != UNDEF) {
      return;
    }
  } else if (gtrue != NULL) {
    map<Block *, Direction> &mb = gtrue -> propagate;
    _id = gtrue -> id;
    if (future[_id] == gtrue) {
      future[_id] = NULL;
    }
    if (mb.empty()) {
      result = FALSE;
    } else {
      for (mb_it = mb.begin(); mb_it != mb.end(); mb_it++) {
        propagate(mb_it -> first, mb_it -> second, TRUE);
      }
    }
    delete gtrue;
    gtrue = NULL;
    if (result != UNDEF) {
      return;
    }
  }
}

// b has been created. now create the subtree rooted at b by copying the
// information from the golden parse dag
void copySubtree(Block *b) {
#ifdef ASSERTS
  assert(b != NULL);
#endif
  int id;
  Block *g = b -> G;

  if (g -> t != B) { // children are blocks
    Block *gb;

    /* left subtree */
    gb = g -> LB;

    if (gb == NULL) {
      b -> LB = NULL;
    } else {
      Block *lb;
      id = gb -> id;
      lb = future[id];
      if (lb != NULL) { // if it's already been created, just use it
        b -> LB = lb;
        (lb -> propagate)[b] = LEFT;
      } else {
        switch (gb -> t) {
        case F:
          lb = new Block(gb, b, LEFT, -1);
#ifdef ASSERTS
          gfalse == NULL;
#endif
          gfalse = lb;
          break;
        case T:
          lb = new Block(gb, b, LEFT, -1);
#ifdef ASSERTS
          gtrue == NULL;
#endif
          gtrue = lb;
          break;
        case B:
        case D_LR:
        case C_LR:
          lb = new Block(gb, b, LEFT, -1);
          break;
        default:
          tid++;
          lb = new Block(gb, b, LEFT, tid);
          current->push_back(lb); current_size++;
          break;
        }
        b -> LB = lb;
        future[id] = lb;
        copySubtree(lb);
      }
    }

    /* right subtree */
    gb = g -> RB;

    if (gb == NULL) {
      b -> RB = NULL;
    } else {
      Block *rb;
      id = gb -> id;
      rb = future[id];
      if (rb != NULL) {
        b -> RB = rb;
        (rb -> propagate)[b] = RIGHT;
      } else {
        switch (gb -> t) {
        case F:
          rb = new Block(gb, b, RIGHT, -1);
#ifdef ASSERTS
          gfalse == NULL;
#endif
          gfalse = rb;
          break;
        case T:
          rb = new Block(gb, b, RIGHT, -1);
#ifdef ASSERTS
          gtrue == NULL;
#endif
          gtrue = rb;
          break;
        case B:
        case D_LR:
        case C_LR:
          rb = new Block(gb, b, RIGHT, -1);
          break;
        default:
          tid++;
          rb = new Block(gb, b, RIGHT, tid);
          current->push_back(rb); current_size++;
          break;
        }
        b -> RB = rb;
        future[id] = rb;
        copySubtree(rb);
      }
    }
  } else { // children are arithmetics
    Arithmetic *ga;

    /* left subtree */
    ga = g -> LA;

    if (ga == NULL) {
      b -> LA = NULL;
    } else {
      Arithmetic *la;
      if (ga -> op == NOOP) {
        gida++;
        la = new Arithmetic(ga, b, LEFT, gida);
        ground_arithmetics.push_back(la);
      } else {
        la = new Arithmetic(ga, b, LEFT, -1);
      }
      b -> LA = la;
      copySubtree(la);
    }

    /* right subtree */
    ga = g -> RA;

    if (ga == NULL) {
      b -> RA = NULL;
    } else {
      Arithmetic *ra;
      if (ga -> op == NOOP) {
        gida++;
        ra = new Arithmetic(ga, b, RIGHT, gida);
        ground_arithmetics.push_back(ra);
      } else {
        ra = new Arithmetic(ga, b, RIGHT, -1);
      }
      b -> RA = ra;
      copySubtree(ra);
    }
  }
}

void copySubtree(Arithmetic *a) {
#ifdef ASSERTS
  assert(a != NULL);
#endif
  Arithmetic *g;

  // copy the left subtree from the golden parse tree
  g = a -> G -> L;
  if (g == NULL) {
    a -> L = NULL;
  } else {
    Arithmetic *l;
    if (g -> op == NOOP) {
      gida++;
      l = new Arithmetic(g, a, LEFT, gida);
      ground_arithmetics.push_back(l);
    } else {
      l = new Arithmetic(g, a, LEFT, -1);
    }
    a -> L = l;
    copySubtree(l);
  }
      
  // copy the right subtree from the golden parse tree
  g = a -> G -> R;
  if (g == NULL) {
    a -> R = NULL;
  } else {
    Arithmetic *r;
    if (g -> op == NOOP) {
      gida++;
      r = new Arithmetic(g, a, RIGHT, gida);
      ground_arithmetics.push_back(r);
    } else {
      r = new Arithmetic(g, a, RIGHT, -1);
    }
    a -> R = r;
    copySubtree(r);
  }
}

Type getNewType(Type t) {
  switch (t) {
    case G_RF:
    case G_F:
      return G_RF;
    case F_RF:
    case F_F:
      return F_RF;
    case U_LRF:
    case U_LF:
    case U_RF:
    case U_F:
      return U_LRF;
    default:
      assert(0);
  }
  //return F; // well, this is unreachable!
}

// we have seen the new timestamp and calculated the duration spent on the trace
// since the last timestamp. now, expand the blocks in current to future
void expand() {
  Block *b, *b1;
  Type t, t_old;
  double bound;
  if (current_old_size == 0) {
    switch (parse_tree -> t) {
    case F:
      b = new Block(parse_tree, -1);
#ifdef ASSERTS
      gfalse == NULL;
#endif
      gfalse = b;
      break;
    case T:
      b = new Block(parse_tree, -1);
#ifdef ASSERTS
      gtrue == NULL;
#endif
      gtrue = b;
      break;
    case B:
    case D_LR:
    case C_LR:
      b = new Block(parse_tree, -1);
      break;
    default:
      tid++;
      b = new Block(parse_tree, tid);
      current->push_back(b); current_size++;
      break;
    }
    //future[b -> id] = b; // unnecessary, no one shares the root!
    copySubtree(b);
  } else {
    //cout << "current_old in expand" << endl;
    //printCurrentOld();
    for (vector<Block *>::iterator it = current_old->begin();
           it != current_old->end(); it++) {
      b = (*it);
      if (b == NULL) {
        continue;
      }
#ifdef ASSERTS
      assert(b -> F == NULL);
      assert(b -> bound >= 0);
#endif

      t_old = b -> t;
        /*if (t_old != 5 && t_old != 7 && t_old != 8) {
          cout << "index : " << (*it) -> tid << " : " << t_old << endl;
        }*/
      t = getNewType(t_old);
      bound = (b -> bound) - duration;
      if (bound < 0) {
        changeNresolve(b);
        if (result != UNDEF) { // it is either TRUE or FALSE
          break;
        }
        continue;
      }
      tid++;

      if ((t_old == F_F) || (t_old == G_F) || (t_old == U_F)) {
        // depends only on the F block, so just modify the block
        b -> t = t;
        b -> bound = bound;
        b -> tid = tid;
        b -> id = -1;
        current->push_back(b); current_size++;
        copySubtree(b);
      } else {
        b1 = new Block(b -> op, t, bound, b -> G,
                       b, FUTURE, tid);
        b -> F = b1;
        current->push_back(b1); current_size++;
        copySubtree(b1); // adds to `future', and `ground_arithmetics' and
                         // may change gtrue/gfalse
      }
    }
    current_old->clear(); current_old_size = 0;
  }
}

void advance() {
  duration = ts_new - ts_old;
  tid = gida = -1;
  gtrue = gfalse = NULL;
  /* clear future */
  memset(future, NULL, sizeof(Block *) * blockCount);

    //cout << "before expand" << endl; printCurrent();

  // swap current and current_old
  tb = current;
  current = current_old;
  current_old = tb;

  current_old_size = current_size;
  current_size = 0;

  expand();
    //cout << "expanded" << endl; printCurrent();

  if (result == UNDEF) {
    resolve();
      //cout << "resolved" << endl; printCurrent();
  }

  ground_arithmetics.clear();
  ts_old = ts_new;
}
