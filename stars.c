#include <stdio.h>

int main(int argc,char *argv[]){
	int i,j,k,given;
	printf("pls give number of rows");
	scanf("%d",&given);

	for(i=0;i<=given;i++){			//gia tis dosmenes grammes
		
		for(j=i;j<given;j++){		//symplhrwse gia kathe grammh kai sthlh kena
			printf(" ");
		}
		for(j=1;j<=(2*i-1);j++){	//kai meta gemise ta kena me typo 2*i-1(gia na einai peritto exei -1)me asteria se kathe sthlh
			printf("*");		//wste ta asteria na einai se ayksoysa seira 
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}