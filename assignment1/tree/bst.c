#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define NUMTHRS 3
#define NUMINSERTS 10
#define MAXTIME 10

typedef struct tnode{
    int value;
    struct tnode *left, *right;
    pthread_mutex_t m;
} treeNode;

typedef struct {
    treeNode *root;
    pthread_mutex_t m;
} treeRoot;

void insert(treeRoot *tree, int val)
{
    pthread_mutex_lock(&(tree->m));
    treeNode **tptr = &(tree->root);
    if((*tptr) == NULL){
    	*tptr = (treeNode *)malloc(sizeof(treeNode));
    	(*tptr)->value = val;
    	(*tptr)->left = NULL;
    	(*tptr)->right = NULL;    
	pthread_mutex_init(&(*tptr)->m,NULL);
	pthread_mutex_unlock(&(tree->m));
	return;
    }
    pthread_mutex_lock(&(*tptr)->m);
    pthread_mutex_unlock(&tree->m);
    treeNode **parent;
    do{

	/* ignore duplicates */
	if ((*tptr)->value == val)
	    return;

	/* follow the right or left branch */
	if ((*tptr)->value < val){
	    if((*tptr)->right != NULL){
		    pthread_mutex_lock(&(*tptr)->right->m);
		    pthread_mutex_unlock(&(*tptr)->m);
	    }
	    else
		    parent = tptr;
	    tptr = &((*tptr)->right);
	}
	else{
	    if((*tptr)->left != NULL){
		    pthread_mutex_lock(&(*tptr)->left->m);
		    pthread_mutex_unlock(&(*tptr)->m);
	    }
	    else
		    parent = tptr;
	    tptr = &((*tptr)->left);
	}
    } while((*tptr) != NULL);
    *tptr = (treeNode *)malloc(sizeof(treeNode));
    (*tptr)->value = val;
    (*tptr)->left = NULL;
    (*tptr)->right = NULL;    
    pthread_mutex_init(&(*tptr)->m,NULL);
    pthread_mutex_unlock(&(*parent)->m);
}

void print(treeNode *t)
{
    if (t == NULL)
	return;
    print(t->left);
    printf("Value: %d\n", t->value);
    print(t->right);
}

typedef struct{
	treeRoot *t;
	int id;
} proc_info_t;


void *process_init(void *proc){
	proc_info_t *p = (proc_info_t*)proc;
	printf("Process %d Started\n",p->id);
	int val=p->id;
	struct timespec time,rem;
	time.tv_nsec=0;
	for(int i=NUMINSERTS*val; i<NUMINSERTS*(val+1); i++){
		insert(p->t,i);
		printf("--\n");
		print(p->t->root);
		time.tv_sec=random()%MAXTIME;
		nanosleep(&time,&rem);
	}
	printf("Process %d Exited\n",p->id);
	delete p;
}

main()
{
    int i;
    treeRoot t;
    
    t.root = NULL;
    pthread_t thr[NUMTHRS];
    for(int i=0;i<NUMTHRS;i++){
	    proc_info_t *p = new proc_info_t;
	    p->id=i;
	    p->t=&t;
	    pthread_create(&thr[i],NULL,process_init,(void*)p);
    }
    void *status;
    for(int i=0;i<NUMTHRS;i++)
	    pthread_join(thr[i],&status);
    print(t.root);
}
