#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <stack>


struct TreeNode;

struct NodeList {
  struct TreeNode* head;
  struct TreeNode* tail;
};

struct TreeNode {
    int val;
    int is_last_sib; // allows todolist alg to undo mutation
    struct TreeNode* next;
    struct NodeList children;
};


/* Iterative function for inorder tree 
   traversal */
int inOrderSum(struct TreeNode *root) 
{ 
    std::stack<TreeNode*> s; 
    TreeNode *curr = root; 
    int sum = 0;
  
    while (curr != NULL || s.empty() == false) 
    { 
        /* Reach the left most Node of the 
           curr Node */
        while (curr !=  NULL) 
        { 
            /* place pointer to a tree node on 
               the stack before traversing 
              the node's left subtree */
            s.push(curr); 
            curr = curr->children.head; 
        } 
  
        /* Current must be NULL at this point */
        curr = s.top(); 
        s.pop(); 
  
        sum += curr->val;
  
        /* we have visited the node and its 
           left subtree.  Now, it's right 
           subtree's turn */
        curr = curr->next; 
  
    } /* end of while */
    return sum;
} 

int fn_for_children(struct NodeList children, int sum, std::list<TreeNode*>& todo);

int fn_for_node(struct TreeNode* node, int sum, std::list<TreeNode*>& todo) {
  return fn_for_children(node->children, sum + node->val, todo);
}

int fn_for_children(struct NodeList children, int sum, std::list<TreeNode*>& todo) {
  if (children.head == NULL) {
    if (todo.front() == NULL) {
      return sum;
    }
    struct TreeNode* first_todo = todo.front();
    todo.pop_front();
    return fn_for_node(first_todo, sum, todo);
  } else {
    struct TreeNode* curr = children.head->next;
    while (curr) {
      todo.push_front(curr); //reverse order compared to before, not technically backtracking?
      curr = curr->next;
    }
    return fn_for_node(children.head, sum, todo);
  }
}


int fn_for_children_c(struct NodeList children, int sum, struct NodeList todo);

int fn_for_node_c(struct TreeNode* node, int sum, struct NodeList todo) {
  return fn_for_children_c(node->children, sum + node->val, todo);
}

// todo.tail isn't needed i.e. it can be of type struct TreeNode*
int fn_for_children_c(struct NodeList children, int sum, struct NodeList todo) {
  if (children.head == NULL) {
    if (todo.head == NULL) {
      return sum;
    }
    struct TreeNode* first_todo = todo.head;
    todo.head = todo.head->next;
    // undo mutation done when appending to todo
    if (first_todo->is_last_sib) {
      first_todo->next = NULL;
    }
    return fn_for_node_c(first_todo, sum, todo);
  } else {
      struct TreeNode* temp = todo.head;
      todo.head = children.head->next;
      children.tail->next = temp;
      return fn_for_node_c(children.head, sum, todo);
  }
}


/*
return the sum of all node values in tree rooted at root
accumulators:
 - sum, the sum of all node values seen so far
 - todo, list of pointers to known nodes still to visit
*/
int sum_tree_tr_cpp(struct TreeNode* root) {
  // trampoline guard
  if (root == NULL) { 
    return 0;
  }

  //struct NodeList todo = {NULL, NULL};
  //return fn_for_node_c(root, 0, todo);

  std::list<TreeNode*> todo;
  return fn_for_node(root, 0, todo);
}

int sum_tree_tr_c(struct TreeNode* root) {
  // trampoline guard
  if (root == NULL) { 
    return 0;
  }
  struct NodeList todo = {NULL, NULL};
  return fn_for_node_c(root, 0, todo);
}


// construct perfect tree of arity n and levels levels with each node having val
// base cases from condition table of n, levels
struct TreeNode* build_perfect_arbtree(int n, int val, int levels) {
  if (levels == 0) {
    return NULL;
  }
  struct TreeNode* subRoot = (TreeNode*)malloc(sizeof(struct TreeNode));
  subRoot->val = val;
  subRoot->next = NULL;
  subRoot->children = (struct NodeList){NULL, NULL};
  subRoot->is_last_sib = 0;
  if (levels == 1) {
    return subRoot;
  }

  struct TreeNode* prev_sib = NULL;
  for (int i = 0; i < n; i++) {
    struct TreeNode* curr = build_perfect_arbtree(n, val, levels - 1);
    //printf("subtree built at %p\n", curr);
    if (i == 0) {
      subRoot->children.tail = curr;
      curr->is_last_sib = 1;
    }
    curr->next = prev_sib;
    prev_sib = curr;
  }
  subRoot->children.head = prev_sib;
  //printf("head: %p, tail: %p\n", subRoot->children.head, subRoot->children.tail);
  return subRoot;
}

// free all memory associated with tree rooted at subRoot
void delete_arbtree(struct TreeNode* subRoot) {
  if (subRoot == NULL) {
    return;
  }
  // leaf case
  if (subRoot->children.head == NULL && subRoot->children.tail == NULL) {
    free(subRoot);
    return;
  }
  struct TreeNode* curr = subRoot->children.head;
  while (curr) {
    struct TreeNode* next = curr->next;
    delete_arbtree(curr);
    curr = next;
  }
  free(subRoot);
}


void delete_children(struct NodeList children, struct NodeList todo);

void delete_node(struct TreeNode* node, struct NodeList todo) {
  struct NodeList children = node->children;
  free(node);
  delete_children(children, todo);
}

// todo.tail isn't needed i.e. it can be of type struct TreeNode*
void delete_children(struct NodeList children, struct NodeList todo) {
  if (children.head == NULL) {
    if (todo.head == NULL) {
      return;
    }
    struct TreeNode* first_todo = todo.head;
    todo.head = todo.head->next;
    delete_node(first_todo, todo);
  } else {
      struct TreeNode* temp = todo.head;
      todo.head = children.head->next;
      children.tail->next = temp;
      delete_node(children.head, todo);
  }
}


/*
free all nodes in tree rooted at root
accumulators:
 - todo, list of pointers to known nodes still to free
*/
void delete_arbtree_tr(struct TreeNode* root) {
  if (root == NULL) {
    return;
  }
  struct NodeList todo = {NULL, NULL};
  delete_node(root, todo);
}



/*
print tree to terminal

          val
        val  val
      val val val val
accumulators: 
depth - 

*/
void print_tree(struct TreeNode* root) {
  char diagram[1][1];
}

// from stackOverflow

void printTabs(int count)
{
    for (int i = 0; i < count; i++)
    {
        putchar('\t');
    }
}
void printTreeRecursive(TreeNode* node, int level)
{
    while (node != NULL)
    {
        printTabs(level);
        printf("Node: %d\n", node->val);

        if (node->children.head != NULL)
        {
            printTabs(level);
            printf("Children:\n");
            printTreeRecursive(node->children.head, level + 1);
        }

        node = node->next;
    }
}

// usage: ./arbtree <arity> <levels> <mode 0: C, 1: C++>
int main(int argc, char** argv) {
  int arity;
  int levels;
  int mode;
  char *endptr;
  if (argc == 4)
    arity = strtol (argv [1], &endptr, 10);
    levels = strtol (argv [2], &endptr, 10);
    mode = strtol (argv [3], &endptr, 10);
  if (argc != 4 || *endptr != 0) {
    printf ("argument error\n");
    return EXIT_FAILURE;
  }

  struct TreeNode* ptree = build_perfect_arbtree(arity, 10, levels);
  printf("build finished\n");
  printTreeRecursive(ptree, 0);
  int sum;
  if (mode == 0) {
    sum = sum_tree_tr_c(ptree);
  } else {
    sum = inOrderSum(ptree);
    printf("iterative\n");
  }
  printf("ptree sum: %d\n", sum);

  delete_arbtree_tr(ptree);
  ptree = NULL;
  printf("delete successful\n");
}
