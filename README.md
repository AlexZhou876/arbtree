# arbtree
a mini-library mostly in C with some C++ stl that implements n-ary trees and some algorithms in a functional style, presenting alternatives. Experimental.

## Instructions
compile: g++ -O2 -o arbtree arbtree.cpp
-O2 enables tail call optimization. Add flags as needed.

terminal execute: ./arbtree arity levels
example: ./arbtree 2 3


## Notes
functional programs use mutation sparingly if at all. In the C implementation of the todolist-based tree traversal, I leveraged the existing data structure to create the todolist. This required a pointer assignment similar to appending linked lists.

To undo the mutation, I could have either created another function to traverse and reset pointers or reset the pointers in the original traversal. The second option is implemented and has the disadvantage of requiring each node to know whether it is a last sibling.

This approach ensures the integrity of the data structure before and after its execution, but not during. A concurrent process/thread, even a read-only one, would not be allowed to use the tree while already being traversed.

To ensure the integrity of the tree during traversal, the todolist or other collection-based accumulators must be stored in a separate data structure. In the code, the todolist is a C++ std::list.

Possible C alternatives are:
- creating a linkedlist struct and using it in the same way as the
std::list
- allocating a statically-sized array with enough space to store
all of the nodes in the tree, though this requires additional information

### Code-Related
neither C++ or C have local functions (functions inside functions), though C++ has lambdas. Recursion helpers are defined at file scope at the moment.

## Improvements
Make type-independent functionality