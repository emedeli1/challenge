#pragma once
// Trees for the tests of exercises 7 and 8, built with the engine (DecisionTree).
#include <ai/guards.hpp>

// The tree of the guards (its leaves: Patrol 0, Investigate 1, Chase 2, RaiseAlarm 3;
// the root: node 10)
inline DecisionTree guard_tree() {
   DecisionTree t{};
   Node const& patrol     { t.action(Action::Patrol) };
   Node const& investigate{ t.action(Action::Investigate) };
   Node const& chase      { t.action(Action::Chase) };
   Node const& alarm      { t.action(Action::RaiseAlarm) };
   Node const& q6      { t.question(QUESTIONS[5], investigate, patrol) };
   Node const& q2_clue { t.question(QUESTIONS[1], investigate, q6) };
   Node const& q5      { t.question(QUESTIONS[4], q2_clue, patrol) };
   Node const& q4      { t.question(QUESTIONS[3], chase, q5) };
   Node const& q3      { t.question(QUESTIONS[2], alarm, chase) };
   Node const& q2_seen { t.question(QUESTIONS[1], chase, q3) };
   Node const& q1      { t.question(QUESTIONS[0], q2_seen, q4) };
   t.set_root(q1);
   return t;
}

// A tree with only a leaf
inline DecisionTree leaf_tree(Action a) {
   DecisionTree t{};
   t.set_root(t.action(a));
   return t;
}

// A question whose two answers lead to the same leaf
inline DecisionTree same_leaf_tree(Action a) {
   DecisionTree t{};
   Node const& l{ t.action(a) };
   t.set_root(t.question(QUESTIONS[0], l, l));
   return t;
}

// yes: Chase; no: a question with two Patrol leaves (no RaiseAlarm, no Investigate)
inline DecisionTree small_tree() {
   DecisionTree t{};
   Node const& chase { t.action(Action::Chase) };
   Node const& patrol{ t.action(Action::Patrol) };
   Node const& q     { t.question(QUESTIONS[4], patrol, patrol) };
   t.set_root(t.question(QUESTIONS[0], chase, q));
   return t;
}
