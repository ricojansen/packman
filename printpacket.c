/* printpacket.c */

#include "packman.h"

int printheader(struct packet *pakt) {
	fprintf(stdout,"origAddress : %d:%d/%d\n",unsplit(pakt->pkh.origZone),unsplit(pakt->pkh.origNet),unsplit(pakt->pkh.origNode));
	fprintf(stdout,"destAddress : %d:%d/%d\n",unsplit(pakt->pkh.destZone),unsplit(pakt->pkh.destNet),unsplit(pakt->pkh.destNode));
	fprintf(stdout,"Date        : %2.2d-%2.2d-%4.4d\n",unsplit(pakt->pkh.day),unsplit(pakt->pkh.month),unsplit(pakt->pkh.year));
	fprintf(stdout,"Time        : %2.2d:%2.2d:%2.2d\n",unsplit(pakt->pkh.hour),unsplit(pakt->pkh.minute),unsplit(pakt->pkh.second));
	fprintf(stdout,"Baud        : %d\n",unsplit(pakt->pkh.baud));
	fprintf(stdout,"ID's        : %2.2d %2.2d\n",pakt->pkh.ID1,pakt->pkh.ID2);
	fprintf(stdout,"prodcode    : %2.2d\n",pakt->pkh.prodcode);
	fprintf(stdout,"serialno    : %2.2d\n",pakt->pkh.serialno);
	fprintf(stdout,"Password    : '%s'\n\n",pakt->pkh.password);
	return(0);
}



/* prints packet header and a part of the packet contents */
int printpacket(struct packet *pakt) {
struct msglist *walk;
char *txt;

	fprintf(stdout,"origAddress : %d:%d/%d\n",unsplit(pakt->pkh.origZone),unsplit(pakt->pkh.origNet),unsplit(pakt->pkh.origNode));
	fprintf(stdout,"destAddress : %d:%d/%d\n",unsplit(pakt->pkh.destZone),unsplit(pakt->pkh.destNet),unsplit(pakt->pkh.destNode));
	fprintf(stdout,"Date        : %2.2d-%2.2d-%4.4d\n",unsplit(pakt->pkh.day),unsplit(pakt->pkh.month),unsplit(pakt->pkh.year));
	fprintf(stdout,"Time        : %2.2d:%2.2d:%2.2d\n",unsplit(pakt->pkh.hour),unsplit(pakt->pkh.minute),unsplit(pakt->pkh.second));
	fprintf(stdout,"Baud        : %d\n",unsplit(pakt->pkh.baud));
	fprintf(stdout,"ID's        : %2.2d %2.2d\n",pakt->pkh.ID1,pakt->pkh.ID2);
	fprintf(stdout,"prodcode    : %2.2d\n",pakt->pkh.prodcode);
	fprintf(stdout,"serialno    : %2.2d\n",pakt->pkh.serialno);
	fprintf(stdout,"Password    : '%s'\n\n",pakt->pkh.password);
	walk=pakt->root;
	while(walk!=NULL) {
		fprintf(stdout,"origAddress : %d/%d\n",unsplit(walk->pmsg.origNet),unsplit(walk->pmsg.origNode));
		fprintf(stdout,"destAddress : %d/%d\n",unsplit(walk->pmsg.destNet),unsplit(walk->pmsg.destNode));
		fprintf(stdout,"Attribute   : %d\n",unsplit(walk->pmsg.Attribute));
		fprintf(stdout,"Cost        : %d\n",unsplit(walk->pmsg.cost));
		fprintf(stdout,"Date        : %s\n",walk->pmsg.DateTime);
		fprintf(stdout,"FromUser    : %s\n",walk->FromUser);
		fprintf(stdout,"ToUser      : %s\n",walk->ToUser);
		fprintf(stdout,"Subject     : %s\n",walk->Subject);
		txt=walk->Text;
		while(*txt!='\0') {
			fputc(*txt,stdout);
			if (*txt==CARRIAGERETURN) fputs("\n",stdout);
			txt++;
		}
		walk=walk->next;
		fputs("\n",stdout);
	}
	return(0);
}
