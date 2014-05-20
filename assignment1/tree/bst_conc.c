#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct tnode {
    int value;
    struct tnode *left, *right;
    pthread_mutex_t m;
} treeNode;

typedef struct {
    treeNode *root;
} treeRoot;

void insert(treeRoot *tree, int val)
{
    treeNode **tptr = &(tree->root);
    while (*tptr != NULL){

	/* ignore duplicates */
	if ((*tptr)->value == val)
	    return;

	/* follow the right or left branch */
	if ((*tptr)->value < val){
	    if((*tptr)->right != NULL){
		    pthread_mutex_lock((*tptr)->right->m);
		    pthread_mutex_unlock((*tptr)->m);
	    }
	    tptr = &((*tptr)->right);
	}
	else{
	    if((*tptr)->left != NULL){
		    pthread_mutex_lock((*tptr)->left->m);
		    pthread_mutex_unlock((*tptr)->m);
	    }
	    tptr = &((*tptr)->left);
	}
    }
    *tptr = (treeNode *)malloc(sizeof(treeNode));
    malloc(sizeof(treeNode));
    (*tptr)->value = val;
    (*tptr)->left = NULL;
    (*tptr)->right = NULL;    
    (*tptr)->m = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_unlock((*tptr)->m);
}

void print(treeNode *t)
{
    if (t == NULL)
	return;
    print(t->left);
    printf("Value: %d\n", t->value);
    print(t->right);
}

main()
{
    int i;
    treeRoot t;
    
    t.root = NULL;
    for (i=0; i<10; i++) {
	int v;
	scanf("%d", &v);
	insert(&t, v);
	print(t.root);
    }	
}
