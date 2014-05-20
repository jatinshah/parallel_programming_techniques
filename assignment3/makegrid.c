#include <stdlib.h>
#include <stdio.h>

int main(int argc,char *argv[]){
	if(argc < 4){
		printf("Error: makegrid grid-file size seed\n");
		exit(0);
	}

	FILE *fp=fopen(argv[1],"w");

	srandom(atoi(argv[3]));
	int size=atoi(argv[2]);
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++)
			fprintf(fp,"%d ",random()&1);
		fprintf(fp,"\n");
	}
	fclose(fp);
}
