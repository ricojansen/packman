#include "packman.h"

int modifypacket(struct packet *pakt,struct reroutetab *rrt,struct kludges *kl) {
struct msglist *walkmsg;
struct fidaddr chg;

	printmsg(3,"ModifyPacket\n");

	/* Change the packet header origin address */
	chg.Zone=unsplit(pakt->pkh.origZone);
	chg.Net=unsplit(pakt->pkh.origNet);
	chg.Node=unsplit(pakt->pkh.origNode);
	chg.Point=0;
	dochange(&chg,rrt->orgroot);
	split(pakt->pkh.origZone,chg.Zone);
	split(pakt->pkh.origNet,chg.Net);
	split(pakt->pkh.origNode,chg.Node);

	/* Change the packet header destination address */
	chg.Zone=unsplit(pakt->pkh.destZone);
	chg.Net=unsplit(pakt->pkh.destNet);
	chg.Node=unsplit(pakt->pkh.destNode);
	chg.Point=0;
	dochange(&chg,rrt->dstroot);
	split(pakt->pkh.destZone,chg.Zone);
	split(pakt->pkh.destNet,chg.Net);
	split(pakt->pkh.destNode,chg.Node);

	/* Change all packed messages */
	walkmsg=pakt->root;
	while(walkmsg!=NULL) {
		/* Change message origin */
		chg.Zone=0;
		chg.Net=unsplit(walkmsg->pmsg.origNet);
		chg.Node=unsplit(walkmsg->pmsg.origNode);
		chg.Point=0;
		dochange(&chg,rrt->orgroot);
		split(walkmsg->pmsg.origNet,chg.Net);
		split(walkmsg->pmsg.origNode,chg.Node);
		/* Change message destination */
		chg.Zone=0;
		chg.Net=unsplit(walkmsg->pmsg.destNet);
		chg.Node=unsplit(walkmsg->pmsg.destNode);
		chg.Point=0;
		dochange(&chg,rrt->dstroot);
		split(walkmsg->pmsg.destNet,chg.Net);
		split(walkmsg->pmsg.destNode,chg.Node);
		/* Change seenby line, path kludge and origin line */
		changeseenby(&(walkmsg->Text),rrt->dstroot);
        changekludge(&(walkmsg->Text),kl->root,rrt);
		changeorigin(&(walkmsg->Text),rrt->orgroot);
		changetearline(&(walkmsg->Text));
		walkmsg=walkmsg->next;
	}
	return(0);
}

int changekludge(char **text,struct kludgelist *kll,struct reroutetab *rrt) {
int pos;
	printmsg(3,"ChangeKludge\n");
	pos=0;
	/* seek the kludge */
	while((*text)[pos]!='\0') {
		while((*text)[pos]!=CTRLA && (*text)[pos]!='\0') pos++;
		if ((*text)[pos]=='\0') continue;
		pos++;
		kludgereplace(text,&pos,kll,rrt);
	}
	return(0);
}
