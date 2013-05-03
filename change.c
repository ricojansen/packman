#include "packman.h"

/* walk through a line address by address */
int changestr(char **text,int *pos,struct rrlist *root) {
int type,lentxt;
struct fidaddr fd;

	printmsg(3,"ChangeStr\n");
	while((*text)[*pos]!=CARRIAGERETURN) {
		if ((type=parseaddress((*text)+(*pos),&fd,NOWILD))==-1) { /* See if it is a address */
			printmsg(0,"Address parsing error\n");
			while((*text)[*pos]!=' ' && (*text)[*pos]!='\0' && (*text)[*pos]!=CARRIAGERETURN) (*pos)++;
		} else {
			/* address found now change it */
			lentxt=0;
			while((*text)[*pos+lentxt]!=' ' && (*text)[*pos+lentxt]!=CARRIAGERETURN) lentxt++;
			changeaddr(text,*pos,lentxt,root,&fd,type);
			while((*text)[*pos]!=' ' && (*text)[*pos]!=CARRIAGERETURN) (*pos)++;
		}
		if ((*text)[*pos]==' ') (*pos)++; /* txt points to next address */
	}
	return(0);
}

/* change the origin line */
int changeorigin(char **text,struct rrlist *root) {
int pos;
struct fidaddr fd;
int type,len;

int begin,end,left;

	printmsg(3,"ChangeOrigin\n");
	pos=0;
	/* find the origin line */
	while((*text)[pos]!='\0') {
		while((*text)[pos]!='\0' && (*text)[pos]!=CARRIAGERETURN) pos++;
		if ((*text)[pos]==CARRIAGERETURN) pos++;
		if ((*text)[pos]=='\0') break;
		if (strncmp((*text)+pos," * Origin: ",11)==0) {
			/* Store begin of origin */
			begin=pos+11;;
			while((*text)[pos]!=CARRIAGERETURN) pos++;
			while((*text)[pos]!='(') pos--;
			/* Store end of origin end begin of address */
			left=pos-1;
			while(!isnum((*text)[pos]) && (*text)[pos]!=CARRIAGERETURN) pos++;
			if ((*text)[pos]==CR || (type=parseaddress((*text)+pos,&fd,NOWILD))==-1) {
				printmsg(0,"Address parsing error\n");
			} else {
				len=0;
				while((*text)[pos+len]!=')') len++;
				/* change the address in the origin line */
	            changeaddr(text,pos,len,root,&fd,type);
				/* If no origin defined let it be */
				if (cfg.ORIGIN!=NULL) {
					end=left;
					while((*text)[end]!=CARRIAGERETURN) end++;
					/* See if it fits */
					if (strlen(cfg.ORIGIN)<(80-(end-left)-11)) {
						(*text)[left]=' ';
						replacestring(text,left-begin,begin,cfg.ORIGIN);
					}
				}
			}
		}
	}
	return(0);
}

/* modify the seenby line */
int changeseenby(char **text,struct rrlist *root) {
int pos;
	printmsg(3,"ChangeSEENBY\n");
	pos=0;
	/* seek the seenby lines */
	while((*text)[pos]!='\0') {
		while((*text)[pos]!='\0' && (*text)[pos]!=CARRIAGERETURN) pos++;
		if ((*text)[pos]=='\0') break;
		pos++;
		if (strncmp((*text)+pos,"SEEN-BY: ",9)==0) {
			pos+=9;
			/* seek through the line for addresses to be changed */
			changestr(text,&pos,root);
		}
	}
	return(0);
}

int doaddress(char **text,int *pos,char **rep,struct reroutetab *rrt,int keep) {
int len;
int pos2;
struct fidaddr fd;
int type,typ;
struct rrlist *root;

	printmsg(3,"DoAddress\n");
	if ((type=parserepl(rep))==-1) {
		return(-1);
	} else {
		if ((typ=parseaddress((*text)+*pos,&fd,NOWILD))==-1) {
			return(-1);
		} else {
			switch(type) {
				case 1 :root=rrt->orgroot;
						break;
				case 2 :root=rrt->dstroot;
						break;
				default:printmsg(0,"Illegal program flow\n");
						return(-1);
			}
			type=typ;
			if (typ>8) {
				typ-=8;
			}
			pos2=*pos;
			while(typ--) {
				(*pos)++;
				while(isnum((*text)[*pos])) {
					(*pos)++;
				}
			}
			len=(*pos)-pos2;
			if (!keep) changeaddr(text,pos2,len,root,&fd,type);
		}
	}
	return(0);
}

int doword(char **text,int *pos,char **rep,int keep) {
int lenrep;
char *txtrep;

	printmsg(3,"DoWord\n");
	lenrep=0;
	txtrep=*rep;
	while(*txtrep!=' ' || *txtrep!='\0') {
		txtrep++;
		lenrep++;
	}
	txtrep=(char *)malloc(sizeof(char)*(lenrep+1));
	txtrep[lenrep]='\0';
	while(lenrep--) {
		txtrep[lenrep]=(*rep)[lenrep];
	}
	if (!keep) replacestring(text,strlen(txtrep),*pos,txtrep);
	(*pos)+=strlen(txtrep);
	(*rep)+=lenrep;
	return(0);
}

int donum(char **text,int *pos,char **rep,int keep) {
int num;
char txtrep[20];

	printmsg(3,"DoNum\n");
	if (getnum(&num,rep)==-1) {
		printmsg(0,"Wasn't a number in replacement specification\n");
		return(-1);
	} else {
		sprintf(txtrep,"%d",num);
		if (!keep) replacestring(text,strlen(txtrep),*pos,txtrep);
		(*pos)+=strlen(txtrep);
		return(0);
	}
}

/* change the tearline line */
int changetearline(char **text) {
int pos;
int len;

	printmsg(3,"ChangeTearline\n");

	/* If tearline replacement not requested so don't bother finding it */
	if (cfg.TEARLINE!=NULL) {
		pos=0;
		/* find the tearline line */
		while((*text)[pos]!='\0') {
			while((*text)[pos]!='\0' && (*text)[pos]!=CARRIAGERETURN) pos++;
			if ((*text)[pos]==CARRIAGERETURN) pos++;
			if ((*text)[pos]=='\0') break;
			if (strncmp((*text)+pos,"--- ",4)==0) {
				printmsg(3,"Found Tearline\n");
				pos+=4;
				len=0;
				while(((*text)+pos)[len]!=CARRIAGERETURN) len++;
				replacestring(text,len,pos,cfg.TEARLINE);
			}
		}
	}
	return(0);
}
