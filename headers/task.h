#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXINT(x,y) ((x>y)?(x):(y))

// item->flag values
#define OLD_ELEM 0
#define NEW_ELEM 1
#define ONESHOT_ELEM 2
#define TRUE_ELEM 3

/*-- item section --*/

#define KNINT_LONG
//#define KNINT_INT
typedef long long knint;
size_t KNINT_SIZE;

// abstract for p and w, one item <--> one pair (p,w)
typedef struct item_t {
  knint p, w; // payoff and weight
  struct item_t *next;
  char flag; // OLD_ELEM, NEW_ELEM, ...
  //UT_hash_handle hh;
} item_t;
size_t ITEM_SIZE;

item_t* createitems (int);
item_t* createitems0 (int);
item_t* copyitem (item_t*);
//item_t* copyitems (int, item_t*);
item_t* copyitemslist (int, item_t*);
//item_t* joinitems (int, item_t*, int, item_t*);
void print_items (int,item_t*);
void print_items_line (int,item_t*);
void print_items_list (item_t*);
void free_items (item_t**);
void free_items_list (item_t**);

int put_item (item_t*, item_t**, int*);
int safe_put_item (item_t*, item_t**, int*);
int put_item_simple (item_t*, item_t**, int*);
item_t* find_preplace (item_t*, knint*);
item_t* find_preplace_badcutter (item_t*, knint, int*); // find preplace and cut bad items with inefficient payoffs within find process
item_t* find_preplace_badcutter_simple (item_t*, knint, int*);

//-- task section ---

typedef struct task{
  knint b;
  item_t *items;
  int length;
#ifdef BUR_SPLICING
  int splicegap;
#endif
} task_t;

task_t* createtask (int,knint);
task_t* readtask(char*);
void print_task (task_t*);

void free_task (task_t**);

// -- tree section ---

typedef struct node {
  item_t  *items; // hash
  int length; // if length = 1, then free through DL_FOREACH, otherwise "HASH_CLEAR(hh,&items); free(items)"
  struct node  *lnode, *rnode, *hnode; // left, right and head nodes in tree
  int source;
} node_t;

node_t* createnodes(int size);

void print_tree ( node_t* );
void print_node ( char*, node_t* );

void free_tree ( node_t* ) ;
void free_node ( node_t* ) ;

void clean_tree (node_t*);
void clean_node (node_t*);

#if FULL_SOLUTION == 1
  void print_tree_inclojurefile (node_t*, FILE*);
  void print_node_inclojurefile (/*char * pre, */node_t*, FILE*);
#endif

#endif
