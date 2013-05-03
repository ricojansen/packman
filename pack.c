/* pack.c */
#include "packman.h"

/* extractpackets : unpacks all messages and processes them */
int extractpackets(char *argv[],struct reroutetab *rrt,struct kludges *kl) {
struct packet pakt;
struct namelist *root,*wlk;
char txt[128];

	pakt.thread=NULL;
	pakt.root=NULL;
	root=NULL;
	/* Unpack messages */
	if (cfg.INBOUND!=NULL) {
		strcpy(txt,cfg.INBOUND);
		strcat(txt,argv[2]);
	} else {
		strcpy(txt,argv[2]);
	}
	unpack(txt,&root);
	wlk=root;
	while(wlk!=NULL) {
		sprintf(txt,"\nProcessing : %s\n",wlk->name);
		printmsg(1,txt);
		/* If INBOUND is defined get packets there otherwise from current dir */
		if (cfg.INBOUND!=NULL) {
			strcpy(txt,cfg.INBOUND);
			strcat(txt,wlk->name);
		} else {
			strcpy(txt,wlk->name);
		}
		readpacket(txt,&pakt);				/* read packet */
		modifypacket(&pakt,rrt,kl);			/* modify packet */
		writepacket(txt,&pakt);				/* write packet */
		/* If BACKUP is defined write them there too */
		if (cfg.BACKUP!=NULL) {
			strcpy(txt,cfg.BACKUP);
			strcat(txt,wlk->name);
			writepacket(txt,&pakt);
		}
		cleanuppacket(&pakt);
		wlk=wlk->next;
	}
	cleanupnamelist(root);
	return(0);
}

/* addpackets : processes all messages and packs them */
int addpackets(char *argv[],struct reroutetab *rrt,struct kludges *kl) {
struct packet pakt;
struct namelist *root,*wlk;
int ind;
char txt[128];

	pakt.thread=NULL;
	pakt.root=NULL;
	root=NULL;
	ind=3;
	/* Create list of packets to process */
	while(argv[ind]!=NULL) {
		wlk=malloc(sizeof(struct namelist));
		if (wlk==NULL) {
			printmsg(0,"Addpacket : Memory allocation error\n");
			return(-1);
		}
		wlk->next=root;
		root=wlk;
		strncpy(wlk->name,argv[ind],12);
		wlk->name[12]='\0';
		ind++;
	}
	/* Process packets */
	wlk=root;
	while(wlk!=NULL) {
		sprintf(txt,"\nProcessing : %s\n",wlk->name);
		printmsg(1,txt);
		/* If OUTBOUND is defined get packets there otherwise from current dir */
		if (cfg.OUTBOUND!=NULL) {
			strcpy(txt,cfg.OUTBOUND);
			strcat(txt,wlk->name);
		} else {
			strcpy(txt,wlk->name);
		}
		readpacket(txt,&pakt);				/* read packet */
		modifypacket(&pakt,rrt,kl);			/* modify packet */
		writepacket(txt,&pakt);				/* write packet */
		/* If BACKUP is defined write them there too */
		if (cfg.BACKUP!=NULL) {
			strcpy(txt,cfg.BACKUP);
			strcat(txt,wlk->name);
			writepacket(txt,&pakt);
		}
		cleanuppacket(&pakt);
		wlk=wlk->next;
	}
	/* pack messages */
	if (cfg.OUTBOUND!=NULL) {
		strcpy(txt,cfg.OUTBOUND);
		strcat(txt,argv[2]);
	} else {
		strcpy(txt,argv[2]);
	}
	pack(txt,root);
	cleanupnamelist(root);
	return(0);
}

/* pack messages using archiver */
int pack(char *name,struct namelist *root) {
char *buf;
char buf2[128];
int  len;
struct namelist *wlk;

	wlk=root;
	printmsg(3,"Pack\n");
	buf=malloc(sizeof(char)*(strlen(cfg.PACK)+strlen(name)+3));
	if (buf==NULL) {
		printmsg(0,"Can't allocate memory\n");
		return(-1);
	}
	sprintf(buf,"%s %s ",cfg.PACK,name);
	while(wlk!=NULL) {
		sprintf(buf2,"%s%s ",cfg.OUTBOUND,wlk->name);
		len=strlen(buf);
		replacestring(&buf,0,strlen(buf),buf2);
		wlk=wlk->next;
	}
	system(buf);
	return(0);
}

/* unpack messages using archivers */
int unpack(char *name,struct namelist **root) {
char *buf;
int pos;

	printmsg(3,"UnPack\n");
	buf=malloc(sizeof(char)*(strlen(cfg.UNPACK)+strlen(name)+strlen(">tmp ")+3));
	if (buf==NULL) {
		printf(0,"Can't allocate memory","");
		return(-1);
	}
	pos=0;
	while(cfg.UNPACK[pos]!=' ') {
		buf[pos]=cfg.UNPACK[pos];
		pos++;
	}
	buf[pos]=cfg.UNPACK[pos];
	pos++;
	buf[pos]='\0';
	strcat(buf,">tmp ");
	strcat(buf,&(cfg.UNPACK[pos]));
	strcat(buf," ");
	strcat(buf,name);
	system(buf);
	findnames(root,"tmp");

	system("delete tmp"); /* System dependent */

	return(0);
}

/* Scans through a output redirection file for messages extracted */
int findnames(struct namelist **root,char *name) {
FILE *nam;
struct namelist *wlk;
char line[512];
char *pos;

	if ((nam=fopen(name,"r"))==NULL) {
		printf(0,"Can't open file",name);
		return(-1);
	}
	while(fgets(line,512,nam)!=NULL) {
		pos=line;
		strtoupper(line);
		while(strncmp(pos,".PKT",4)!=0 && (pos-line)<512 && strcmp(pos,"\n")!=0) pos++;
		if ((pos-line)<512 && strcmp(pos,"\n")!=0) {
			wlk=(struct namelist *)malloc(sizeof(struct namelist));
			pos-=8;
			strncpy(wlk->name,pos,12);
			wlk->name[12]='\0';
			wlk->next=(*root);
			*root=wlk;
		}
	}
	printmsg(2,"Files extracted :\n");
	wlk=*root;
	while(wlk!=NULL) {
		printmsg(2,wlk->name);
		wlk=wlk->next;
	}

	fclose(nam);
	return(0);
}
