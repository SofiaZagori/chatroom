#include <lib.h>
#include <stdio.h>
#include <stdlib.h> //gia to atoi

int main(int argc,char *argv[]){
	if(argc<2){
		exit(1); //an ta arguments einai perissotera apo 2 tote termatize
	}
	int given=atoi(argv[1]); //pare ton 2o arithmo apo ta arguments kai valto sthn metavlhth given
	message m;
	m.m1_i1=given; //h metavlhth message pou einai ayth poy tha metaferei ton akeraio sto syscall einai m1 dhladh akeraios	
	_syscall(PM_PROC_NR,PM_PRINTSTARS,&m); //kalese to syscall printstars kai perna san orisma thn m
}