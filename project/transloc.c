/* *
 *  Author: Jatin Shah, Kenneth Chin, Seh Leng Lim
 *  Date: 19 January 2001
 *  
 *  Genome: Set of all chromosomes
 *  Gene : regions of Chromosome
 *  Please read the "readme" file for more details
 *  
 * */
#include "minsp.c"


/* Construct a genome graph */
genome *makegraph(char *in)
{
    genome *genome_head;
    chromosome *chromo_head, *prev_chromo, *temp_chromo;
    gene *gene_head, *prev_gene, *temp_gene;
    int c, temp;
    int i, j, nchromo, ngene, *gene_per_chromo;
    FILE *g1 = fopen(in, "rw");

    genome_head = malloc(sizeof(genome));

    /* Counting the number of chromosomes */
    nchromo = 0;
    genome_head->ngene = 0;
    while ((c = fgetc(g1)) != EOF)
	if ((c == LF) || (c == CR))
	    nchromo++;
    genome_head->nchromo = nchromo;
   // printf("Number of chromosome: %d\n", nchromo);

    /* Counting the number of genes in each chromosome */
    gene_per_chromo = (malloc(sizeof(int) * nchromo));
    ngene = 0;
    i = 0;
    fseek(g1, 0, SEEK_SET);
    while ((c = fgetc(g1)) != EOF) {
	if ((c == SP) || (c == LF) || (c == CR)) {
	    ngene++;
	    if ((c == LF) || (c == CR)) {
		gene_per_chromo[i] = ngene;
		genome_head->ngene += ngene;
	//	printf("Chromosome %d has %d gene(s)\n", i,
	//	       gene_per_chromo[i]);
		i++;
		ngene = 0;
	    }
	}
    }

    /* Constructing the genome graph
       1. allocating memory
       2. setting up appropriate links (e.g. black edges) */

    for (i = 0; i < nchromo; i++) {
	temp_chromo = malloc(sizeof(chromosome));
	if (i == 0) {		//i.e. the 1st chromosome;
	    chromo_head = temp_chromo;
	    temp_chromo->prev = NULL;
	} else {
	    prev_chromo->next = temp_chromo;
	    temp_chromo->prev = prev_chromo;
	}
	temp_chromo->next = NULL;
	prev_chromo = temp_chromo;

	for (j = 0; j < gene_per_chromo[i]; j++) {
	    temp_gene = malloc(sizeof(gene));
	    if (j == 0) {	//i.e. the 1st gene;
		gene_head = temp_gene;
		temp_gene->prev = NULL;
		temp_gene->left.chromoid = i;
		temp_gene->right.chromoid = i;
	    } else {
		prev_gene->next = temp_gene;
		temp_gene->prev = prev_gene;
		temp_gene->left.chromoid = i;
		temp_gene->right.chromoid = i;

	    }
	    temp_gene->next = NULL;
	    prev_gene = temp_gene;
	}
	//printf("Chromosome %d is allocated with memory!\n", i);
	temp_chromo->name = i;
	temp_chromo->g = gene_head;
    }

    genome_head->name = 0;
   // printf("Initializing genome %d\n", genome_head->name);
    genome_head->c = chromo_head;

    temp = 0;
    temp_chromo = genome_head->c;
    temp_gene = temp_chromo->g;
    fseek(g1, 0, SEEK_SET);
    while ((c = fgetc(g1)) != EOF) {
	if ((c == LF) || (c == CR)) {
	   //printf("Last Gene %d is assigned\n", temp);
	    temp_gene->name = temp;
	    temp_gene->left.name = 0;
	    temp_gene->right.name = 1;
	    temp_gene->left.parent = temp_gene;
	    temp_gene->right.parent = temp_gene;
	    temp_gene->left.gray = NULL;
	    temp_gene->right.gray = NULL;
	    if (temp_chromo->next != NULL) {
		temp_chromo = temp_chromo->next;
		temp_gene = temp_chromo->g;
	    }
	    temp = 0;
	} else if (c == SP) {
	   //printf("Gene %d is assigned\n", temp);
	    temp_gene->name = temp;
	    temp_gene->left.name = 0;
	    temp_gene->right.name = 1;
	    temp_gene->left.parent = temp_gene;
	    temp_gene->right.parent = temp_gene;
	    temp_gene->left.gray = NULL;
	    temp_gene->right.gray = NULL;
	    temp_gene = temp_gene->next;
	    temp = 0;
	} else {
	    temp *= 10;
	    c -= 48;
	    temp += c;
	}
    }
    return genome_head;
}

/* Get the gene data-structure indexed by the number */
gene *lookup_gene(genome * genome_head, int name)
{
    chromosome *chromo_head;
    gene *gene_head;

    chromo_head = genome_head->c;
    gene_head = chromo_head->g;

    while (chromo_head != NULL) {
	while (gene_head != NULL) {
	    if (gene_head->name == name)
		return gene_head;
	    gene_head = gene_head->next;
	}
	chromo_head = chromo_head->next;
	if (chromo_head != NULL)
	    gene_head = chromo_head->g;
    };
    return NULL;
}

/* Add the grey edges in the genome */
void *add_gray_edges(void *arg)
{
    int tid = (int) arg;	// thread ID;
    int i;

    chromosome *chromo_head;
    gene *gene2_head;
    gene *temp_gene1, *temp_gene2;

    chromo_head = genome2_head->c;
    for (i = 0; i < tid; i++)
	chromo_head = chromo_head->next;

    gene2_head = chromo_head->g;

    while (gene2_head != NULL) {
	temp_gene1 = lookup_gene(genome1_head, gene2_head->name);
	if (gene2_head->next != NULL) {
	    temp_gene2 = lookup_gene(genome1_head, gene2_head->next->name);
	    temp_gene1->right.gray = &(temp_gene2->left);
	    temp_gene2->left.gray = &(temp_gene1->right);
	}
	gene2_head = gene2_head->next;
    }
}

/* Count the number of cycles in genome 2 */
void *count2forg2(void *arg)
{
    int tid = (int) arg;	// thread ID;
    int i;
    int count = 0;

    chromosome *chromo_head;
    gene *gene2_head;
    gene *temp_gene1, *temp_gene2;

    chromo_head = genome2_head->c;
    for (i = 0; i < tid; i++)
	chromo_head = chromo_head->next;

    gene2_head = chromo_head->g;
    while (gene2_head != NULL) {
	if (classify_cycle(gene2_head) == 2) {

	    count++;

	}
	gene2_head = gene2_head->next;
    }

    pthread_mutex_lock(&(g2cycle_lock));
    g2cycle += count;
    pthread_mutex_unlock(&(g2cycle_lock));

}

/* Count the number of cycles in genome 1 */
void *count2forg1(void *arg)
{
    int tid = (int) arg;	// thread ID;
    int i;
    int count = 0;

    chromosome *chromo_head;
    gene *gene1_head;
    gene *temp_gene1, *temp_gene2;

    chromo_head = genome1_head->c;
    for (i = 0; i < tid; i++)
	chromo_head = chromo_head->next;

    gene1_head = chromo_head->g;

    while (gene1_head != NULL) {
	if (classify_cycle(gene1_head) == 2)
	    count++;
	gene1_head = gene1_head->next;
    }
    pthread_mutex_lock(&(g1cycle_lock));
    g1cycle += count;
    pthread_mutex_unlock(&(g1cycle_lock));

}

/* Updation of chromoid on chromosome -- done when the process queue is exhausted */
void *update_chromoid(genome * genome)
{
    int i;
    chromosome *chromo_head;
    gene *gene_head;

    i = 0;
    chromo_head = genome->c;
    gene_head = genome->c->g;

    while (gene_head != NULL) {
	gene_head->left.chromoid = i;
	gene_head->right.chromoid = i;
	gene_head = gene_head->next;
    }

    for (i = 1; i < genome->nchromo; i++) {

	chromo_head = chromo_head->next;
	gene_head = chromo_head->g;

	while (gene_head != NULL) {
	    gene_head->left.chromoid = i;
	    gene_head->right.chromoid = i;
	    gene_head = gene_head->next;


	}
    }

}

/* Return value: 0: not a cycle
                 1: proper translocation
                 2: cycle of length 2
                 3: otherwise */
int classify_cycle(gene * g)
{
    gene *nextg;
    gene *startg;
    subgene leftsubg;
    subgene *graysubg;
    int flag = 1;
    int minSP = 1;

    if (g == NULL)
	return 0;
    else {
	startg = g;
	nextg = g;
    }
    //printf("gene %d in chromosome %d\n", g->name, g->left.chromoid);
    /* Detecting cycle of length 2 */
    if (startg->next != NULL) {
	if (startg == startg->next->left.gray->parent) {
	    startg->right.done = 1;
	    startg->next->left.done = 1;
	    //printf("Gene(%d,%d) is done\n", startg->name,
		  // startg->next->name);
	    return 2;
	}
    };

    while ((flag) || ((nextg != startg) && (nextg != NULL))) {
	flag = 0;

	if (nextg == NULL) {
	    return 0;
	}

	if (nextg->next != NULL)
	    nextg = nextg->next;
	else
	    return 0;

	leftsubg = nextg->left;

	if (leftsubg.gray != NULL) {
	    graysubg = leftsubg.gray;
	    nextg = graysubg->parent;

	    if (nextg->left.chromoid < startg->left.chromoid)
		return 0;
	    else if (nextg->left.chromoid > startg->left.chromoid)
		minSP = 0;
	} else {
	    return 0;
	}
    }
    if (minSP == 0)
	return 1;
    else
	return 3;
}

/* Detect if there is an improper translocation starting at gene g */
int detect_imp_translocation(gene * g)
{
    subgene *graysubg;
    gene *nextg;
    gene *startg;
    subgene leftsubg;
    subgene rightsubg;

    startg = g;
    if (g->next != NULL)
	nextg = g->next;
    else
	return 0;
    leftsubg = nextg->left;

    if (leftsubg.gray != NULL) {
	graysubg = leftsubg.gray;
	nextg = graysubg->parent;
    } else
	return 0;

    if (nextg->prev != NULL)
	nextg = nextg->prev;
    else
	return 0;

    rightsubg = nextg->right;

    if (rightsubg.gray != NULL) {
	graysubg = rightsubg.gray;
	nextg = graysubg->parent;
    } else
	return 0;

    if (nextg == startg)
	return 1;
    else
	return 0;
}

/* Creation of task queue of cycles representing proper translocation */
void *detect_cycle(void *arg)
{
    // Note: Assume one chromosome per thread

    chromosome *chromo_head;
    gene *gene_head;
    cycle *temp_cycle;
    int tid = (int) arg;	// thread ID;
    int i;

    chromo_head = genome1_head->c;
    for (i = 0; i < tid; i++)
	chromo_head = chromo_head->next;

    gene_head = chromo_head->g;
    while (gene_head != NULL) {
	if (classify_cycle(gene_head) == 1) {	//i.e. proper translocation;
	    temp_cycle = malloc(sizeof(cycle));
	    temp_cycle->g = gene_head;
	    pthread_mutex_lock(&(pcycle_head_lock));
	    pthread_mutex_lock(&(pcycle_tail_lock));
	    if ((pcycle_head == NULL) && (pcycle_tail == NULL)) {
		pcycle_head = temp_cycle;
		pcycle_tail = temp_cycle;
		pcycle = 1;
	    } else {
		pcycle_tail->next = temp_cycle;
		pcycle_tail = pcycle_tail->next;
		pcycle++;
	    }
	    //printf("Gene %d is the head of a proper translocation\n",
		//   gene_head->name);
	    //printf("%d\n", pcycle_tail->g->name);
	    pthread_mutex_unlock(&(pcycle_head_lock));
	    pthread_mutex_unlock(&(pcycle_tail_lock));
	}
	gene_head = gene_head->next;
    }
}

/* Perform Valid Translocation */
/* Take a cycle from task queue and perform valid translocation */
void *process_cycle(void *arg)
{

    gene *g;
    pthread_mutex_lock(&(pcycle_head_lock));

    if (pcycle_head != NULL) {
	g = (pcycle_head->g);
	pcycle_head = pcycle_head->next;
    }
    pthread_mutex_unlock(&(pcycle_head_lock));
    if (g != NULL)
	vptranslocation(g,arg);

}


/* Check if the translocation is valid proper translocation
 * is_vptrans1 and is_transvp2 cover the two different cases  
 * Please refer to figure 3 in the main Hannenhalli paper
 * */
int is_vptrans1(gene * g)
{
    subgene *clvge1_left, *clvge1_right, *clvge2_left, *clvge2_right;
    subgene *v;
    gene *temp_gene;
    gene *temp_gene2, *temp_gene3;
    int swap = 1;		// look for black edge first;
    clvge1_left = &(g->right);
    clvge1_right = &(clvge1_left->parent->next->left);	// follow black edge;
    clvge2_left = clvge1_right->gray;	// follow gray edge;
    clvge2_right = &(clvge2_left->parent->next->left);	// follow black edge;

   //printf("%d has id %d\n", clvge1_left->parent->name,
	  // clvge1_left->chromoid);
   //printf("%d has id %d\n", clvge1_right->parent->name,
	  // clvge1_right->chromoid);
   //printf("%d has id %d\n", clvge2_left->parent->name,
	  // clvge2_left->chromoid);
   //printf("%d has id %d\n", clvge2_right->parent->name,
	  // clvge2_right->chromoid);

    /* Ensuring the Z path spans across two different chromosomes */
    if (clvge1_right->chromoid == clvge2_left->chromoid) {
	printf("vptrans1: error 1\n");
	return 0;
    }
    v = clvge2_right->gray;	// follow gray edge;
    if (v == clvge1_left)	// i.e a cycle of length 2, but not minSP;
	return 1;
    else {
	/* Check if v is in L1 or R2 */

	while (clvge1_left != v) {
	    //printf("Checking %d ...\n", v->parent->name * 10 + v->name);
	    temp_gene = clvge1_left->parent;
	    while ((temp_gene != v->parent) && (temp_gene != NULL))
		temp_gene = temp_gene->prev;
	    if (temp_gene == NULL) {	// i.e. v->parent not inside L1;
		//printf("Not inside L1\n");
		temp_gene = clvge2_right->parent;
		while ((temp_gene != v->parent) && (temp_gene != NULL))
		    temp_gene = temp_gene->next;
		if (temp_gene == NULL) {	// i.e. v->parent not inside R2;
		    //printf("Not inside R2\n");
		    return 1;
		}
		//printf("%d is inside R2\n", v->parent->name);
	    } else
		  printf("%d is inside L1\n", v->parent->name);
	    if (swap)
		v = &(v->parent->next->left);
	    else
		v = v->gray;
	    swap = swap ^ 1;
	}
    }
    printf("vptrans1: error 2\n");
    return 0;
}

int is_vptrans2(gene * g)
{
    subgene *clvge1_left, *clvge1_right, *clvge2_left, *clvge2_right;
    subgene *v;
    gene *temp_gene;
    int swap = 1;		// look for gray edge first;
    clvge1_left = &(g->right);
    clvge1_right = &(clvge1_left->parent->next->left);	// follow black edge;
    clvge2_left = clvge1_right->gray;	// follow gray edge;
    clvge2_right = &(clvge2_left->parent->next->left);	// follow black edge;

    /* Ensuring the Z path spans across two different chromosomes */
    if (clvge1_right->chromoid == clvge2_left->chromoid) {
	//printf("vptrans2: error 2\n");
	return 0;
    }

    v = clvge1_right->gray;	// follow gray edge;
    if (v == clvge2_left)	// i.e a cycle of length 2, but not minSP;
	return 1;
    else {
	/* Check if v is in R1 or L2 */

	while (clvge2_left != v) {
	   // printf("Checking %d ...\n", v->parent->name * 10 + v->name);
	    temp_gene = clvge1_right->parent;
	    while ((temp_gene != v->parent) && (temp_gene != NULL))
		temp_gene = temp_gene->next;
	    if (temp_gene == NULL) {	// i.e. v not inside R1;
		//printf("Not inside R1\n");
		temp_gene = clvge2_left->parent;
		while ((temp_gene != v->parent) && (temp_gene != NULL))
		    temp_gene = temp_gene->prev;
		if (temp_gene == NULL) {	// i.e. v not inside L2;
		    //printf("Not inside L2\n");
		    return 1;
		}
		//printf("%d is inside L2\n", v->parent->name);
	    } else
		printf("%d is inside L2\n", v->parent->name);
	    if (swap)
		v = &(v->parent->next->left);
	    else
		v = v->gray;
	    swap = swap ^ 1;
	}
    }
    printf("vptrans2: error 2\n");
    return 0;
}

int is_vptrans(gene * g)
{
    if (is_vptrans1(g) && is_vptrans2(g)) {
      //printf("Valid proper translocation on Gene %d\n", g->name);
	return 1;
    };
    //printf("Invalid proper translocation on Gene %d\n", g->name);
    return 0;
}

/* Perform a valid proper translocation */
void vptranslocation(gene * g,void* arg)
{
    gene *clvge1_left, *clvge1_right, *clvge2_left, *clvge2_right;
    gene *temp_gene, *temp_next, *start_gene;
    int flag, length2;
    start_gene = g;
    temp_gene = g;
    length2 = 0;


    flag = 0;
    while ((temp_gene != start_gene) || !(flag)) {
	flag = 1;
	if (is_vptrans(temp_gene)) {
	    /* Performing translocation */
	    clvge1_left = temp_gene;
	    clvge1_right = clvge1_left->next;
	    clvge2_left = clvge1_right->left.gray->parent;
	    clvge2_right = clvge2_left->next;
	    printf("Before proper translocation by thread %d on Genome 1 : gene %d and Genome 2 : gene %d\n",(int)arg,clvge1_left->name,clvge2_right->name);  	
		print_genome(genome1_head);

	    temp_next = clvge1_left->next; 	
            clvge1_left->next = clvge2_left->next;
	    clvge2_left->next->prev = clvge1_left;
	    clvge2_left->next = temp_next;
	    temp_next->prev = clvge2_left;
 	    printf("After proper translocation by thread %d on Genome 1 : gene %d and Genome 2 : gene %d\n",(int)arg,clvge1_left->name,clvge2_right->name);  	
  	    print_genome(genome1_head);

	    break;
	} else {
	    //printf("Failed the is_vptrans\n");
	    //printf("chromoid = %d\n", temp_gene->left.chromoid);
	    temp_gene = temp_gene->next->left.gray->parent;
	}
    }
    /* Setting done = 1 for length of cycle 2 generated from translocation
     */
    clvge1_right->left.done = 1;
    clvge2_left->right.done = 1;

    start_gene = temp_gene;
    //printf("Start gene = %d.%d\n", start_gene->name,
	//   start_gene->right.name);
    //printf("Next = %d.%d\n", start_gene->next->name,
	//   start_gene->next->left.name);
    //printf("Checking length-2-cycle ... \n");
    /* Checking length-2-cycle */
    /*if (&(start_gene->right) == start_gene->next->left.gray)
       length2 = 1;
       else
       length2 = 0;
       } */
}

int main(int argc, char *argv[])
{
    gene *temp_gene;
    pthread_t thrids[NUM_THREADS];
    chromosome *chromo_head;
    char *s1 = strdup(argv[1]);
    char *s2 = strdup(argv[2]);
    int i, j;
    int stop;

    pcycle = 0;
    stop = 0;

    pthread_mutex_init(&(pcycle_head_lock), NULL);
    pthread_mutex_init(&(pcycle_tail_lock), NULL);
    pthread_mutex_init(&(g1cycle_lock), NULL);
    pthread_mutex_init(&(g2cycle_lock), NULL);

    genome1_head = makegraph(s1);
    genome2_head = makegraph(s2);

    /* Adding gray edges */
    //printf("Start adding grey edges ...\n");

    for (i = 0; i < genome1_head->nchromo; i++)
	pthread_create(&thrids[i], NULL, add_gray_edges, (void *) i);
    for (i = 0; i < genome1_head->nchromo; i++)
	pthread_join(thrids[i], NULL);

    printf("Genome1 is as follows \n");
    print_genome(genome1_head);

    printf("Target Genome2 is as follows \n");
    print_genome(genome2_head);


    while (!stop) {
	pcycle_head = NULL;
	pcycle_tail = pcycle_head;
	pcycle = 0;
	g2cycle = 0;
	g1cycle = 0;

	// Detect all cycles and add them to task queue 
	for (i = 0; i < genome1_head->nchromo; i++)
	    pthread_create(&thrids[i], NULL, detect_cycle, (void *) i);
	for (i = 0; i < genome1_head->nchromo; i++)
	    pthread_join(thrids[i], NULL);


	if (pcycle_head != NULL) {
	    for (i = 0; i < pcycle; i++)
		pthread_create(&thrids[i], NULL, process_cycle,
			       (void *) i);
	    for (i = 0; i < pcycle; i++)
		pthread_join(thrids[i], NULL);
	    update_chromoid(genome1_head);

	} else {
	    //printf("entered minsp\n");
	    pcycle_head = NULL;
	    pcycle_tail = NULL;;
	    p2cycle_head = p2cycle_tail = NULL;
	    pthread_mutex_init(&pcycle_2head_lock, NULL);
	    pthread_mutex_init(&pcycle_2tail_lock, NULL);

	    for (i = 0; i < genome1_head->nchromo; i++)
		pthread_create(&thrids[i], NULL, detect_minsp, (void *) i);
	    for (i = 0; i < genome1_head->nchromo; i++)
		pthread_join(thrids[i], NULL);
#define MAXPROCESSES 2
	    for (i = 0; i < MAXPROCESSES; i++)
		pthread_create(&thrids[i], NULL, beginminSP, (void *) i);
	    for (i = 0; i < MAXPROCESSES; i++)
		pthread_join(thrids[i], NULL);
	    update_chromoid(genome1_head);
	    //printf("Genome1 after min SP is as follows \n");

	    for (i = 0; i < genome1_head->nchromo; i++) {
		if (i == 0)
		    chromo_head = genome1_head->c;
		else
		    chromo_head = chromo_head->next;
		temp_gene = chromo_head->g;

		while (temp_gene != NULL) {
		    //printf("%d ",
			 //  temp_gene->name * 100 +
			  // temp_gene->left.chromoid * 10 +
			 //  temp_gene->right.chromoid);
		    temp_gene = temp_gene->next;
		}
		//printf("\n");
	    }



	}
	for (i = 0; i < genome1_head->nchromo; i++)
	    pthread_create(&thrids[i], NULL, count2forg1, (void *) i);
	for (i = 0; i < genome1_head->nchromo; i++)
	    pthread_join(thrids[i], NULL);

	g2cycle = genome2_head->ngene - genome2_head->nchromo;

	if (g2cycle != g1cycle) {

	    stop = 0;
	} else
	    stop = 1;


    }

    printf("Translocation achieved\n");
    printf("Genome1 is as follows \n");

    print_genome(genome1_head);

    return 1;
}
