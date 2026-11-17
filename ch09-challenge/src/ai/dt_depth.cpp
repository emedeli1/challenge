// The depth of a tree: the nodes of its longest way, leaf included.
// Recursion over both branches: the depth of n is 1 plus the depth of its deeper child
#include "dt.hpp"
#include <algorithm>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
std::size_t depth(Node const& n) {
   if (!n.yes()) { return 1; }                        // a leaf
   return 1 + std::max(depth(*n.yes()), depth(*n.no()));
}
