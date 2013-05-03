#include "packman.h"

/* Deallocates used memory from reroute table and kludge table */
int cleanup(struct reroutetab *rrt,struct kludges *kl) {
struct rrlist *wlk;
struct kludgelist *walkto,*wlkto;

	while(rrt->orgroot!=NULL) {				/* walk along origin list and free all */
		wlk=rrt->orgroot;
		rrt->orgroot=rrt->orgroot->next;
		free((void *)wlk);
	}
	while(rrt->dstroot!=NULL) {				/* walk along destination list and free all */
		wlk=rrt->dstroot;
		rrt->dstroot=rrt->dstroot->next;
		free((void *)wlk);
	}
	walkto=kl->root;
	while(walkto!=NULL) {					/* walk along kludge list free replacement and */
		wlkto=walkto;						/* kludge specification */
		walkto=walkto->next;
		free((void *)wlkto->klg.kludge);
		if (wlkto->klg.spec!=NULL) {
			free((void *)wlkto->klg.spec);
		}
		if (wlkto->klg.repl!=NULL && wlkto->klg.repl!=NIL) {
			free((void *)wlkto->klg.repl);
		}
		free((void *)wlkto);
	}
	return(0);
}

/* Free memory occupied by packet structure */
int cleanuppacket(struct packet *pakt) {
struct msglist *walk,*ttmp;

	walk=pakt->root;
	while(walk!=NULL) {						/* deallocate all packed messages in the packet */
		ttmp=walk;
		walk=walk->next;
		free((void *)ttmp->Text);
		free((void *)ttmp);
	}
	pakt->root=NULL;
	pakt->thread=NULL;
	return(0);
}

/* Free memory occupied by namelist */
int cleanupnamelist(struct namelist *root) {
struct namelist *tmp;

	while(root!=NULL) {
		tmp=root;
		root=root->next;
		free(tmp);
	}
	return(0);
}
