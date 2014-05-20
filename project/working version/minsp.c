#include "transloc.h"

cycle *p2cycle_head,*p2cycle_tail;

pthread_mutex_t pcycle_2head_lock,pcycle_2tail_lock;

/* Is this gene at its final position ? */
int is_location_final(gene *g)
{
	if(g->prev == NULL)	// First Gene on the chromosome
		return 1;
	return (g->prev->name == ((g->left).gray)->parent->name);
}

/* Check if the cycle starting at gene arg is a minSP */
void *detect_minsp(void *arg)
{
	// Note : Assume one chromosome per thread
	// Before Call: pcycle_head == NULL && pthread_tail == NULL
	// Before call: p2cycle_head == NULL && p2cucle_tail == NULL
	// Mutex Init: For all 4 mutexes
	chromosome *chromo_head;
	gene *gene_head;
	cycle *temp_cycle;
	int tid = (int)arg;	// thread ID
	int i;

	printf("entered -- tid:%d\n",tid);
	chromo_head = genome1_head->c;
	for(i=0;i<tid;i++)
		chromo_head = chromo_head->next;

	gene_head=chromo_head->g;

	while(gene_head != NULL){
		if(classify_cycle(gene_head) == 2){
			temp_cycle=(cycle*)malloc(sizeof(cycle));
			temp_cycle->g=gene_head;
			temp_cycle->next=NULL;
			pthread_mutex_lock(&pcycle_2head_lock);
			pthread_mutex_lock(&pcycle_2tail_lock);
			if(p2cycle_head == NULL && p2cycle_tail == NULL){
				p2cycle_head=temp_cycle;
				p2cycle_tail=temp_cycle;
			}
			else{
				p2cycle_tail->next=temp_cycle;
				p2cycle_tail=p2cycle_tail->next;
			}
			pthread_mutex_unlock(&pcycle_2head_lock);
			pthread_mutex_unlock(&pcycle_2tail_lock);
		}
		else if(is_location_final(gene_head) && (classify_cycle(gene_head) == 3)){
			temp_cycle = (cycle*)malloc(sizeof(cycle));
			temp_cycle->g=gene_head;
			temp_cycle->next=NULL;
			pthread_mutex_lock(&pcycle_head_lock);
			pthread_mutex_lock(&pcycle_tail_lock);
			if(pcycle_head == NULL && pcycle_tail == NULL){
				pcycle_head=temp_cycle;
				pcycle_tail=temp_cycle;
			}
			else{
				pcycle_tail->next = temp_cycle;
				pcycle_tail = pcycle_tail->next;
			}
			pthread_mutex_unlock(&pcycle_head_lock);
			pthread_mutex_unlock(&pcycle_tail_lock);
			printf("gene_head:%d\n",gene_head->name);
		}
		gene_head=gene_head->next;
			
	}
	printf("byebye detect_minsp\n");
}

// Break a minSP -- bad translocation
// Break takes place at minSPs beginning at genes g1 and g2
void minSPtranslocation(gene *g1, gene *g2)
{
	gene *temp;
	if(g1->prev != NULL && g1->prev->name == ((g1->left).gray)->parent->name){
		g1->left.done = 0;
		g1->right.done=0;
	}
	if (g2->prev != NULL && g2->prev->name == ((g2->left).gray)->parent->name){
		g2->left.done = 0;
		g2->right.done = 0;
	}

	temp=g1->next;
	g1->next=g2->next;
	g2->next=temp;
	g1->next->prev=g1;
	g2->next->prev=g2;
}

// This function implements what each process does
// Break a pair of minSPs
void *beginminSP(void *arg){
	gene *g1,*g2=NULL;
	cycle *temp_cycle,*t=NULL;

	
	printf("entered begin minsSP:%d\n",(int)arg);
	pthread_mutex_lock(&pcycle_head_lock);
	while(pcycle_head != NULL){
		printf("loop:%d\n",(int)arg);
	// Take two minSPs on different chromosomes
		g1=pcycle_head->g;
		pcycle_head=pcycle_head->next;
		temp_cycle=pcycle_head;
		while(temp_cycle != NULL){
			if((temp_cycle->g)->left.chromoid != g1->left.chromoid){
				g2=temp_cycle->g;
				if(t!=NULL)
					t->next=temp_cycle->next;
				else
					pcycle_head=pcycle_head->next;
				break;
			}
			t=temp_cycle;
			temp_cycle=temp_cycle->next;
		}
		pthread_mutex_unlock(&pcycle_head_lock);
		// If not possible choose a length 2 cycle
		if(g2==NULL){
			pthread_mutex_lock(&pcycle_2head_lock);
			temp_cycle=p2cycle_head;
			while(temp_cycle != NULL){
				if(temp_cycle->g->left.chromoid != g1->left.chromoid){
					g2=temp_cycle->g;
					if(t!=NULL)
						t->next=temp_cycle->next;
					else
						p2cycle_head=p2cycle_head->next;
					break;
				}
				t=temp_cycle;
				temp_cycle=temp_cycle->next;
			}
			pthread_mutex_unlock(&pcycle_2head_lock);
		}
		// Here each bad translocation achieved
		printf("g1->next:%d\n",g1->next->name);
		printf("g2->next:%d\n",g2->next->name);
		minSPtranslocation(g1,g2);
		printf("g1->next:%d\n",g1->next->name);
		printf("g2->next:%d\n",g2->next->name);
		printf("after minsp:\n");
		pthread_mutex_lock(&pcycle_head_lock);
	}
	pthread_mutex_unlock(&pcycle_head_lock);
	printf("leaving minSP:%d\n",(int)arg);
}
