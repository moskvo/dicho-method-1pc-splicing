#include "task.h"

/*-- item section --*/

size_t KNINT_SIZE = sizeof (knint);
size_t ITEM_SIZE = sizeof (item_t);
int buffer_knint_size = 100;
knint* buffer
knintmatrix* knm;
// задачи:
// 1. выдача новых данных
// 2. хранение для удаления
knint* get_knint() {

}

knint*


item_t* createitems(int count){
  return (item_t*)malloc (count*ITEM_SIZE);
}

item_t* createitems0(int count){
  return (item_t*)calloc (count,ITEM_SIZE);
}

item_t* copyitem (item_t *other){
  return (item_t*) memcpy(malloc(ITEM_SIZE),other,ITEM_SIZE);
}

/*item_t* copyitems (int size, item_t *others) {
  item_t* r = createitems0(size);
  memcpy (r->p,others->p,size*KNINT_SIZE);
  memcpy (r->w,others->w,size*KNINT_SIZE);
  return r;
}*/

item_t* copyitemslist (int size, item_t *others) {
  item_t *r = copyitem(others), *cur = r, *tmp;
	int i;
	for ( i = 1, tmp = others->next ; i < size && tmp != NULL ; i++, tmp=tmp->next )
	{
		cur->next = copyitem(tmp);
		cur = cur->next;
	}
  return r;
}

/*item_t* joinitems(int size1, item_t *it1, int size2, item_t *it2) {
  item_t *items = createitems(size1+size2);
  memcpy (items->p, it1->p, size1*KNINT_SIZE);
  memcpy (items->w, it1->w, size1*KNINT_SIZE);
  memcpy (items->p+size1, it2->p, size2*KNINT_SIZE);
  memcpy (items->w+size1, it2->w, size2*KNINT_SIZE);
  return items;
}*/

void print_items ( int size , item_t *item ){
  if ( size == 0 ) return;

  item_t *vitem = item;
  for( ; vitem < item+size ; vitem++ )
    printf("{%4lld, %4lld}, ",vitem->p,vitem->w);
  puts("");
}
// same as print_items() but not add newline symbol
void print_items_line ( int size , item_t *item ){
  if ( size == 0 ) return;

  item_t *vitem;
  for( ; vitem < item+size ; vitem++ )
    printf("{%4lld, %4lld}, ",vitem->p,vitem->w);
}

void print_items_list (item_t *list){
  item_t *p;
  for ( p = list ; p != NULL ; p = p->next ) {
    printf ("(%4lld %4lld) ",p->p,p->w);
  }
  puts("");
}

void free_items (item_t **headp){
  if ( headp ) {
    free (*headp);
    *headp = NULL;
  }
}

void free_items_list (item_t **list){
  if ( list ) {
    item_t *p, *tmp;
    for ( p = *list ; p != NULL ; ) {
      tmp = p->next;
      free_items (&p);
      p = tmp;
    }
    *list = NULL;
  }
}

// *(preplace->w) < *(item->w)
int put_item (item_t *preplace, item_t **item, int *listlen) {
	item_t *vitem = *item;
	if ( preplace->p >= vitem->p ) {
		free_items (item);
		return 1;
	} else {
		/*if ( vitem->flag != TRUE_ELEM ) */vitem->flag = NEW_ELEM;
		item_t *pnext = preplace->next, *tmp;
		if ( pnext != NULL && /*pnext->flag != TRUE_ELEM && */pnext->w == vitem->w ) {
			if ( pnext->p < vitem->p  ) {
				preplace->next = vitem;
				if ( pnext->flag == OLD_ELEM || pnext->flag == ONESHOT_ELEM ) {
					//vitem->next = pnext->next;
					//pnext->next = vitem;
					//preplace->next = pnext;
					vitem->next = pnext;
					pnext->flag = ONESHOT_ELEM;
					(*listlen)++;
				} else {
					vitem->next = pnext->next;
					free_items (&pnext);
				}
				return 0;
			} else {
				free_items (item);
				return 1;
			}
		} else {
			preplace->next = vitem;
			vitem->next = pnext;
			(*listlen)++;
		}
	}
	return 0;
}
// same as put_item but doesn't free item
int safe_put_item (item_t *preplace, item_t **item, int *listlen) {
	item_t *vitem = *item;
	if ( preplace->p >= vitem->p ) {
		return 1;
	} else {
		/*if ( vitem->flag != TRUE_ELEM ) */vitem->flag = NEW_ELEM;
		item_t *pnext = preplace->next, *tmp;
		if ( pnext != NULL && /*pnext->flag != TRUE_ELEM && */pnext->w == vitem->w ) {
			if ( pnext->p < vitem->p  ) {
				preplace->next = vitem;
				if ( pnext->flag == OLD_ELEM ) {
					vitem->next = pnext;
					pnext->flag = ONESHOT_ELEM;
					(*listlen)++;
				} else {
					vitem->next = pnext->next;
					free_items (&pnext);
				}
				return 0;
			} else {
				return 1;
			}
		} else {
			preplace->next = vitem;
			vitem->next = pnext;
			(*listlen)++;
		}
	}
	return 0;
}

// using when simple move elements from one node to another
int put_item_simple (item_t *preplace, item_t **item, int *listlen) {
	item_t *vitem = *item;
	if ( preplace->p >= vitem->p ) {
		return 1;
	} else {
		/*if ( vitem->flag != TRUE_ELEM ) */vitem->flag = NEW_ELEM;
		item_t *pnext = preplace->next, *tmp;
		if ( pnext != NULL && /*pnext->flag != TRUE_ELEM && */pnext->w == vitem->w ) {
			if ( pnext->p < vitem->p ) {
				preplace->next = vitem;
				vitem->next = pnext->next;
				free_items (&pnext);
				return 0;
			} else {
				return 1;
			}
		} else {
			preplace->next = vitem;
			vitem->next = pnext;
			(*listlen)++;
		}
	}
	return 0;
}


item_t* find_preplace (item_t *list, knint *itemw) {
	if ( list->w >= *itemw ) return NULL;
	for ( ; list->next != NULL && list->next->w < *itemw ; list = list->next );
	return list;
}

// find preplace and cut bad items with inefficient payoffs
item_t* find_preplace_badcutter (item_t *list, knint itemw, int *listlen) {
	if ( list->w >= itemw ) return NULL;
	knint edge = list->p;
	item_t *tmp;
	do {
		// remove inefficient elems after item "list"
		while ( list->next != NULL && edge >= list->next->p ) {
			if ( list->next->flag == NEW_ELEM ) {
				tmp = list->next;
				list->next = list->next->next;
				free_items (&tmp);
				(*listlen)--;
			} else {
				if ( list->next->flag == OLD_ELEM ) {
					list->next->flag = ONESHOT_ELEM;
				}
				break;//list = list->next;
			}
		}// while

		if ( list->next != NULL && list->next->w < itemw ) {
			list = list->next;
			edge = list->p;
		// while we can step further
		} else break;
	} while ( 1 );
	return list;
}

// using when simple move elements from one node to another
item_t* find_preplace_badcutter_simple (item_t *list, knint itemw, int *listlen) {
	if ( list->w >= itemw ) return NULL;
	knint edge = list->p;
	item_t *tmp;
	do {
		// remove inefficient elems after item "list"
		while ( list->next != NULL && edge >= list->next->p ) {
				tmp = list->next;
				list->next = list->next->next;
				free_items (&tmp);
				(*listlen)--;
		}// while

		if ( list->next != NULL && list->next->w < itemw ) {
			list = list->next;
			edge = list->p;
		// while we can step further
		} else break;
	} while ( 1 );
	return list;
}


/*-- task section --*/

task_t* createtask(int size, knint b){
  task_t *t = (task_t*)malloc(sizeof(task_t));
  t->length = size;
  t->b = b;
  t->items = createitems0 (size);
  return t;
}

/*
  File's format:
  b
  n
  p1 p2 ... pn
  w1 w2 ... wn
*/
task_t* readtask(char* filename){
  task_t* task;

  FILE *file;
  if( (file = fopen(filename,"r")) == 0 ) return NULL;

    knint b;
    if( fscanf(file,"%lld",&b) != 1 ) return NULL;

    int size;
    if( fscanf(file,"%d",&size) != 1 ) return NULL;

    task = createtask(size,b);

    item_t *head = task->items;
    item_t *tmp;
    //int morethanb = 0;
    for( tmp = head ; tmp < head+size ; tmp++ )
    { if( fscanf (file,"%lld", &(tmp->p)) != 1 ) return 0; }
    for( tmp = head ; tmp < head+size ; tmp++ )
    {
      if( fscanf (file,"%lld", &(tmp->w)) != 1 ) return 0;
      //if( *tmp > b ) morethanb++;
    }

	/*if( morethanb > 0 ) {
		task_t* task2 = createtask(size-morethanb,b);
		item_t* head2;
		for( head = task->items, head2 = task2->items ; head < task->items+size ; head++ ) {
			if( *(head->w) <= b ) {
				*(head2->p) = *(head->p);
				*(head2->w) = *(head->w);
				head2++;
			}
		}
		free_task(&task);
		task = task2;
	}// if */

 #ifdef BUR_SPLICING
  int splicegap;
  if( fscanf(file,"%d",&splicegap) != 1 ) return NULL;
  task->splicegap = splicegap;
 #endif /*splicing mode */

  fclose(file);

  return task;
}

void print_task (task_t* task) {
  puts("task");
  printf ("  b = %lld\n  length = %d\n", task->b, task->length);
  print_items (task->length, task->items);
}

void free_task(task_t **p){
  if ( p ) {
    if( (*p)->items != NULL ) free_items ( &((*p)->items) );
    free (*p);
    *p = NULL;
  }
}

/*-- tree section --*/

const size_t NODE_SIZE = sizeof (node_t);

node_t* createnodes (int size) {
  if( size < 1 ) return NULL;
  node_t *rez = (node_t*)calloc (size,NODE_SIZE), *t;
  for ( t = rez ; t < rez + size ; t++ ) {
    t->source = -1;
  }
  return rez;
}

void print_tree (node_t *root){
  print_node ("", root);
}

void print_node (char * pre, node_t *node){
  fputs(pre,stdout);
  if( node->length < 1 ) printf("node: %d items\n", node->length);
  else {
    item_t *item;
    for ( item = node->items ; item != NULL ; item = item->next ) {
      printf ("(%lld %lld, %d) ",item->p,item->w,item->flag);
    }
    printf("src:%d",node->source);
    puts("");
  }
  fflush(stdout);
  char *lpre = (char*)malloc(strlen(pre)+4), *rpre = (char*)malloc(strlen(pre)+4);
  strcpy(lpre,pre);
  strcpy(rpre,pre);
  if( node->lnode != NULL ) print_node(strcat(lpre,"  L"), node->lnode);
  if( node->rnode != NULL ) print_node(strcat(rpre,"  R"), node->rnode);
  free(lpre); free(rpre);
}

void free_node (node_t* node){
  free_items_list (&(node->items)); //free_hash (&(node->items));
  free (node);
}

void free_tree (node_t *root){
  node_t *runner = root, *tmp;
  int lastjump; // 0 - to left, 1 - to right
  while ( runner != NULL ) {
  	while ( (runner->rnode != NULL) || (runner->lnode != NULL) ) {
  		while ( runner->rnode != NULL ) { runner = runner->rnode; lastjump = 1; }
  		while ( runner->lnode != NULL ) { runner = runner->lnode; lastjump = 0; }
  	}
  	tmp = runner;
  	runner = runner->hnode;
  	free_node (tmp);
  	if ( runner != NULL ) {
  		if ( lastjump == 0 ) runner->lnode = NULL;
  		else runner->rnode = NULL;
  	}
  }// while
}// free_tree

void clean_tree (node_t *root) {
  node_t *runner = root;
  int lastjump; // 0 - to left, 1 - to right
  while ( runner != NULL ) {
  	while ( (runner->rnode != NULL) || (runner->lnode != NULL) ) {
  		while ( runner->rnode != NULL ) { runner = runner->rnode; lastjump = 1; }
  		while ( runner->lnode != NULL ) { runner = runner->lnode; lastjump = 0; }
  	}
  	clean_node (runner);
  	runner = runner->hnode;
  	if ( runner != NULL ) {
  		if ( lastjump == 0 ) runner->lnode = NULL;
  		else runner->rnode = NULL;
  	}
  }// while
}

void clean_node (node_t* node){
  free_items_list (&(node->items));
}

#if FULL_SOLUTION == 1

void print_tree_inclojurefile (node_t* root, FILE* f){
  print_node_inclojurefile (/*"{", */root, f);
}

void print_node_inclojurefile (/*char * pre, */node_t* node, FILE* f){
  fputs ("{",f);
  fputs (":data ",f);
  if( node->length < 1 ) fprintf(f, "nil");
  else {
	fputs("[",f);
    item_t *item = node->items;
	if ( item != NULL ) fprintf (f, "[%lld,%lld]", item->p, item->w);
    for ( item = item->next ; item != NULL ; item = item->next ) {
      fprintf (f, " [%lld,%lld]", item->p, item->w);
    }
	fputs("]",f);
  }
  fputs (",", f);
//  fflush(stdout);
//  char *lpre = (char*)malloc(strlen(pre)+4), *rpre = (char*)malloc(strlen(pre)+4);
//  strcpy(lpre,pre);
//  strcpy(rpre,pre);
  fputs (":left ", f);
  if( node->lnode != NULL ) {
	print_node_inclojurefile (/*strcat(lpre,"left:{"), */node->lnode, f);
	fputs (",", f);
  } else {
	fputs("nil,", f);
  }
  fputs (":right ", f);
  if( node->rnode != NULL ) {
	print_node_inclojurefile (/*strcat(rpre,"right:{"), */node->rnode, f);
	fputs ("", f);
  } else {
	fputs("nil", f);
  }
//  free(lpre); free(rpre);
  fputs ("}",f);
}

#endif
