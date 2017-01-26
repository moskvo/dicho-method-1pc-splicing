#ifndef BURKOV_H
#define BURKOV_H

#ifdef BUR_SPLICING
 #define WITH_SPLICEVAR(lastvar) lastvar, splicegap
 #define WITH_SPLICETYPE(lasttype) lasttype, int splicegap
#else
 #define WITH_SPLICEVAR(lastvar) lastvar
 #define WITH_SPLICETYPE(lasttype) lasttype
#endif

#ifndef TASK_H
#include "task.h"
#endif

#ifndef _MATH_H_
#include <math.h>
#endif


/*
*  building optimum dichotomy tree on Burkov,Burkova's works
*/
node_t* optimal_dichotomic_tree ( const task_t* );
node_t* (*burkovtree) (const task_t*);
void dicho_tree_notrecursive (node_t*, const int, item_t* );
void (*dicho_tree) (node_t*, const int, item_t*);
void prep_items (const int, item_t*, const int, item_t**, item_t**);
int find_q (knint);

void (*treesolver) (node_t*, WITH_SPLICETYPE(knint) );
void notrecursive_treesolver (node_t*, WITH_SPLICETYPE(knint) );
void recursive_treesolver (node_t*, WITH_SPLICETYPE(knint) );

void dichosolve( node_t*, node_t*, node_t*, WITH_SPLICETYPE(knint) );

#endif