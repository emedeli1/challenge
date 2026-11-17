#pragma once
// A decision tree, as objects in memory: data, not code. Built from a table of rows
// (NodeDesc, at the end of this file).
//
//   QuestionNode  asks a Question about the Percept and goes on with ONE of its two
//                 children: yes or no
//   ActionNode    a leaf: the Action chosen
//
// Management and use, apart (like the decision trees of the class video):
//   - USE: a node only USES its children. It keeps a reference to each one, never
//     owns it. A reference is another name for an object that already exists: it is
//     bound when the node is created and never changes. Several nodes can share a
//     child (the leaf Patrol is the child of three questions).
//   - MANAGEMENT: the DecisionTree owns ALL its nodes, each one through a
//     std::unique_ptr. They live in dynamic storage; their lifetime ends when the tree
//     is destroyed (each unique_ptr deletes its node), once and only once.
//
// Why std::vector<std::unique_ptr<Node>> and not std::vector<Node>:
//   - the nodes are of different types (QuestionNode, ActionNode): a vector<Node>
//     could only keep the Node part of each one;
//   - when the vector grows, its buffer moves (Challenge 7). Here it only moves the
//     unique_ptrs: the nodes stay where they are, so the references to them are
//     still valid. With vector<Node> they would be left dangling.
#include <array>
#include <cstdint>
#include <expected>
#include <memory>
#include <optional>
#include <span>
#include <string_view>
#include <vector>
#include <ai/decision.hpp>

// A question of the tree: its text (for the HUD) and how it is answered.
// Inside, it keeps a pointer to the function that answers (the lambdas of QUESTIONS,
// in ai/guards.cpp): the address of its code. From outside, it is just holds()
struct Question {
   using Answer = bool (*)(Percept const& p, Profile const& prof);
   constexpr Question(std::string_view t, Answer a) : text{ t }, answer_{ a } {}

   // Does it hold for this guard? It looks at what it perceives and at its profile
   [[nodiscard]] bool holds(Percept const& p, Profile const& prof) const {
      return answer_(p, prof);
   }

   std::string_view text;

private:
   Answer answer_;
};

// The way a decision went through the tree: the node of each step and its answer.
// The HUD draws it
inline constexpr std::size_t MAX_DEPTH{ 8 };
struct TreePath {
   struct Step {
      std::uint8_t node;     // id of the node (its position in the tree)
      bool         answer;   // what its question answered (leaves: true)
   };
   std::array<Step, MAX_DEPTH> steps{};
   std::uint8_t                size{};

   void add(std::uint8_t node, bool answer) {
      if (size < MAX_DEPTH) {
         steps[size] = { node, answer };
         size += 1;
      }
   }
};

// Exercise 5 (make ej05): with this on, every node says when it is destroyed
inline bool announce_destruction{ false };

//-----------------------------------------------------------------------------
// The base of every node. Polymorphic: decide() and the destructor are virtual
//-----------------------------------------------------------------------------
struct Node {
   Node() = default;
   // Virtual: the tree destroys its nodes through a pointer to Node, and the
   // destructor that runs must be the one of the real type (QuestionNode...)
   virtual ~Node() = default;
   // A node is never copied: a copy would only copy the Node part (slicing)
   Node(Node const&)            = delete("a node is used by reference, never copied");
   Node& operator=(Node const&) = delete("a node is used by reference, never copied");

   // The decision from this node down, with recursion. It writes the way in path
   [[nodiscard]] virtual Action decide(Percept const& p, Profile const& prof
                                      , TreePath& path) const = 0;
   // One step of the decision: the child it goes on with (nullptr for a leaf).
   // DecisionTree::decide goes down the tree with it, in a loop
   [[nodiscard]] virtual Node const* next(Percept const& p
                                         , Profile const& prof) const = 0;

   // Its text (for the HUD) and its children (nullptr for a leaf)
   [[nodiscard]] virtual std::string_view text() const = 0;
   [[nodiscard]] virtual Node const* yes() const { return nullptr; }
   [[nodiscard]] virtual Node const* no()  const { return nullptr; }
   // A leaf: the address of the Action it chooses. A question: nullptr
   [[nodiscard]] virtual Action const* chosen() const { return nullptr; }

   [[nodiscard]] std::uint8_t id() const { return id_; }

private:
   friend struct DecisionTree;    // only the tree gives each node its id
   std::uint8_t id_{};
};

//-----------------------------------------------------------------------------
// A leaf: it always chooses the same Action
//-----------------------------------------------------------------------------
struct ActionNode final : Node {
   explicit ActionNode(Action a) : action_{ a } {}
   ~ActionNode() override;

   [[nodiscard]] Action decide(Percept const& p, Profile const& prof
                              , TreePath& path) const override;
   [[nodiscard]] Node const* next(Percept const& p, Profile const& prof) const override;
   [[nodiscard]] std::string_view text() const override;
   [[nodiscard]] Action const* chosen() const override { return &action_; }

private:
   Action action_;
};

//-----------------------------------------------------------------------------
// A question: it goes on with yes or with no. It USES its question and its children
// through references; it does not own them
//-----------------------------------------------------------------------------
struct QuestionNode final : Node {
   QuestionNode(Question const& q, Node const& yes, Node const& no)
      : question_{ q }, yes_{ yes }, no_{ no } {}
   ~QuestionNode() override;

   [[nodiscard]] Action decide(Percept const& p, Profile const& prof
                              , TreePath& path) const override;
   [[nodiscard]] Node const* next(Percept const& p, Profile const& prof) const override;
   [[nodiscard]] std::string_view text() const override { return question_.text; }
   [[nodiscard]] Node const* yes() const override { return &yes_; }
   [[nodiscard]] Node const* no()  const override { return &no_; }

private:
   Question const& question_;     // an object of static storage (QUESTIONS)
   Node const&     yes_;          // nodes owned by the tree
   Node const&     no_;
};

//-----------------------------------------------------------------------------
// The tree: the owner of its nodes. It is built from the leaves up, because a
// question needs its children to exist already (its references bind to them)
//-----------------------------------------------------------------------------
struct DecisionTree {
   // A new node, owned by the tree. The reference it returns is to USE the node
   // (as the child of a question, or as the root): the tree keeps it alive
   Node const& action(Action a);
   Node const& question(Question const& q, Node const& yes, Node const& no);
   void        set_root(Node const& root) { root_ = &root; }

   // The decision: from the root down, with a loop (Node::decide does the same with
   // recursion: same Action, same path)
   [[nodiscard]] Action decide(Percept const& p, Profile const& prof
                              , TreePath& path) const;

   [[nodiscard]] Node const* root() const { return root_; }
   [[nodiscard]] std::size_t size() const { return nodes_.size(); }
   [[nodiscard]] Node const& node(std::size_t i) const { return *nodes_[i]; }

private:
   Node const& adopt(std::unique_ptr<Node> n);

   std::vector<std::unique_ptr<Node>> nodes_{};   // the owners of the nodes
   Node const*                        root_{};    // it only points: it does not own
};

//-----------------------------------------------------------------------------
// Walking the whole tree, with recursion over both branches.
// Two questions may share a child: what counts is every WAY from n down to a leaf
//-----------------------------------------------------------------------------
// Nodes on the longest way from n down to a leaf, both included (a leaf: 1). The
// way of a decision must fit in a TreePath: at most MAX_DEPTH (ai/dt_depth.cpp)
[[nodiscard]] std::size_t depth(Node const& n);
// The game does not use it: every way from n to a leaf that chooses a,
// added to found. Each one is path (the steps before n) and then its own steps, like
// the TreePath of a decision
void ways_to(Node const& n, Action a, TreePath path, std::vector<TreePath>& found);

//-----------------------------------------------------------------------------
// The tree of the HUD, as rows (exercise 7): one row per node, from the root down, the
// yes branch before the no one. A node shared by two questions has a row under each
// of them. The render system draws the rows (sys/render.cpp)
//-----------------------------------------------------------------------------
struct TreeRow {
   Node const*      node{};     // the node of the row: it only points to it
   std::string_view branch{};   // the answer that leads to it: "sí" or "no" ("": root)
   std::size_t      level{};    // how deep it is: the root 0, its children 1...
   bool             on_way{};   // the last decision went through it
   bool             folded{};   // a question whose children have no rows
};
// The rows of n and of every node under it, added at the end of rows. level and
// branch: those of n. parent_on_way: the last decision went through the parent of n
// and its answer leads to n (for the root: true). If the last decision went through
// n, its step in path is path.steps[level] (ai/dt_rows.cpp)
void tree_rows(Node const& n, TreePath const& path, std::size_t level
             , std::string_view branch, bool parent_on_way, std::vector<TreeRow>& rows);
// The same, folded: a question off the way of the last decision has its row, folded,
// and no rows under it (ai/dt_folded_rows.cpp)
void folded_rows(Node const& n, TreePath const& path, std::size_t level
               , std::string_view branch, bool parent_on_way, std::vector<TreeRow>& rows);
// The same, down to max_level: a question at level max_level has its row, folded,
// and no rows under it (ai/dt_level_rows.cpp, exercise 7; the game does not use it)
void level_rows(Node const& n, TreePath const& path, std::size_t level
              , std::string_view branch, bool parent_on_way, std::size_t max_level
              , std::vector<TreeRow>& rows);

//-----------------------------------------------------------------------------
// A tree described as data: a table, one row per node (exercise 8).
// The children of a question are rows ABOVE it, so they are built before it (a
// question needs its children to exist). The root is the last row. The ids of the
// nodes are their rows
//-----------------------------------------------------------------------------
struct NodeDesc {
   bool         is_leaf;
   Action       action;        // a leaf: what it chooses
   std::uint8_t question;      // a question: its index in the table of questions
   std::uint8_t yes;           // a question: the rows of its children
   std::uint8_t no;
};
// The two kinds of row: leaf(Action::Patrol), ask(4, 1, 0)
[[nodiscard]] constexpr NodeDesc leaf(Action a) {
   return { .is_leaf = true, .action = a, .question = 0, .yes = 0, .no = 0 };
}
[[nodiscard]] constexpr NodeDesc ask(std::uint8_t question
                                   , std::uint8_t yes, std::uint8_t no) {
   return { .is_leaf = false, .action = Action::Patrol
          , .question = question, .yes = yes, .no = no };
}

// What can be wrong in a table, and in which row
enum class BuildProblem : std::uint8_t {
   Empty,              // no rows: there is no root                   (row 0)
   NoQuestion,         // its question is not in the table of questions
   ChildNotBuilt,      // a child is not a row above it
   TooDeep,            // it does not fit in a TreePath (MAX_DEPTH)  (the last row)
   SameChildren,       // a question whose two children are the same row
};
inline constexpr std::array<std::string_view, 5> PROBLEM_NAME {
   "tabla vacía", "pregunta que no existe", "hijo que no es una fila anterior"
 , "árbol demasiado profundo", "pregunta con los dos hijos iguales"
};
struct BuildError {
   BuildProblem problem;
   std::size_t  row;
};
// What build_tree returns when the table is wrong: return fail(problem, row);
[[nodiscard]] inline std::unexpected<BuildError> fail(BuildProblem p, std::size_t row) {
   return std::unexpected{ BuildError{ .problem = p, .row = row } };
}

// The tree of a table, with these questions; or what is wrong in it
// (ai/dt_build.cpp)
[[nodiscard]] std::expected<DecisionTree, BuildError>
build_tree(std::span<NodeDesc const> table, std::span<Question const> questions);
// The first row that nobody uses: neither the root nor a child of another row.
// Nothing (nullopt) if every row is used (the game does not use it)
[[nodiscard]] std::optional<std::size_t> first_unused(std::span<NodeDesc const> table);
