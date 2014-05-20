void print_genome(genome *genome_head)
{
	int i;
	chromosome *chromo_head;
	gene *temp_gene;

	for(i=0;i<genome_head->nchromo;i++){
		if (i == 0)
			chromo_head = genome_head->c;
		else
			chromo_head = chromo_head->next;
		temp_gene = chromo_head->g;

		while(temp_gene != NULL){
			printf("%d ",temp_gene->name);
			temp_gene = temp_gene->next;
		}
		printf("\n");
	}
}
