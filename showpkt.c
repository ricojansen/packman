#include "packman.h"

char *progname;
struct config cfg;
char NIL[]= { "NIL",0 };

void main(int argc,char *argv[]) {
struct packet pck;
	progname=argv[0];
	if (argc!=2) {
		fprintf(stdout,"Usage : %s <packet>\n",progname);
	} else {
		pck.root=NULL;
		pck.thread=NULL;
		cfg.VERBOSE=0;
		if (readpacket(argv[1],&pck)==-1) {
			fprintf(stderr,"%s : Error occured during reading\n",progname);
		} else {
			printpacket(&pck);
		}
	}
}
