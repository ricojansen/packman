#include "packman.h"

/* change an address */
int changeaddr(char **text,int pos,int lentxt,struct rrlist *root,struct fidaddr *fd,int type) {
	printmsg(3,"ChangeAddr\n");
	if (dochange(fd,root)==1) {
		char *buf;
		buf=(char *)malloc(sizeof(char)*25);
		buf[0]='\0';
		/* sort of an address is it gonna be ? */
		if (type<8) {
			switch(type) {
				case 3 : sprintf(buf,"%d:%d/%d",fd->Zone,fd->Net,fd->Node);
						 break;
				case 2 : sprintf(buf,"%d/%d",fd->Net,fd->Node);
						 break;
				case 1 : sprintf(buf,"%d",fd->Node);
						 break;
				default: printmsg(0,"Address rebuild error\n");
						 break;
			}
		} else {
			type-=8;
			switch(type) {
				case 4 : sprintf(buf,"%d:%d/%d.%d",fd->Zone,fd->Net,fd->Node,fd->Point);
						 break;
				case 3 : sprintf(buf,"%d/%d.%d",fd->Net,fd->Node,fd->Point);
						 break;
				case 2 : sprintf(buf,"%d.%d",fd->Node,fd->Point);
						 break;
				default: printmsg(0,"Address rebuild error\n");
						 break;
			}
		} /* Replace the address */
		replacestring(text,lentxt,pos,buf);
		free((void *)buf);
	}
	return(0);
}

/* Seek if the address is in the routing table */
int dochange(struct fidaddr *chg,struct rrlist *root) {
int rtn=0;
char txt[255];
char matchZone,matchNet,matchNode,matchPoint;

	printmsg(3,"DoChange\n");

	while(root!=NULL && rtn==0) {
		matchZone=matchNet=matchNode=matchPoint=NOMATCH;

		matchZone=decide(root->in.Zone,root->out.Zone,chg->Zone);
		matchNet=decide(root->in.Net,root->out.Net,chg->Net);
		matchNode=decide(root->in.Node,root->out.Node,chg->Node);
		matchPoint=decide(root->in.Point,root->out.Point,chg->Point);

		if (matchZone!=NOMATCH && matchNet!=NOMATCH && matchNode!=NOMATCH && matchPoint!=NOMATCH) {
			sprintf(txt,"Address modified from : %d:%d/%d.%d",chg->Zone,chg->Net,chg->Node,chg->Point);
			printmsg(1,txt);
			if (matchZone==USEOUT) {
				chg->Zone=root->out.Zone;
			}
			if (matchNet==USEOUT) {
				chg->Net=root->out.Net;
			}
			if (matchNode==USEOUT) {
				chg->Node=root->out.Node;
			}
			if (matchPoint==USEOUT) {
				chg->Point=root->out.Point;
			}
			sprintf(txt," to %d:%d/%d.%d\n",chg->Zone,chg->Net,chg->Node,chg->Point);
			printmsg(1,txt);
			rtn=1;
		}
		root=root->next;
	}
	return(rtn);
}

char decide(int in,int out,int chg) {
char match;

	match=NOMATCH;
	if (in==WILDCARD) {
		if (out==WILDCARD) {
			match=RETAIN;
		} else {
			match=USEOUT;			
		}
	} else {
		if (chg==in) {
			match=USEOUT;
		}
	}
	return(match);
}
