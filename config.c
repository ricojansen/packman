/* config.c */
#include "packman.h"

char *unpackname=	"UNPACK=";
char *packname=		"PACK=";
char *filesname=	"FILES=";
char *outboundname=	"OUTBOUND=";
char *inboundname=	"INBOUND=";
char *verbosename=	"VERBOSE=";
char *loglevelname=	"LOGLEVEL=";
char *logfilename=	"LOGFILE=";
char *backupname=	"BACKUP=";
char *tearlinename=	"TEARLINE=";
char *originname=	"ORIGIN=";

/* Reads the configuration and puts it in the config structure */
int readconfig(char *name) {
FILE *cofg;
char line[80];
char file[80];
char *pos1,*pos2;
int len;

	/* Initialize config */
	cfg.PACK=NULL;
	cfg.UNPACK=NULL;
	cfg.FILES=NULL;
	cfg.INBOUND=NULL;
	cfg.OUTBOUND=NULL;
	cfg.VERBOSE=0;
	cfg.LOGLEVEL=0;
	cfg.LOGFILE=NULL;
	cfg.BACKUP=NULL;
	cfg.TEARLINE=NULL;
	cfg.ORIGIN=NULL;

	/* Open config file */
	strcpy(file,name);
	strcat(file,".cfg");
	if ((cofg=fopen(file,"r"))==NULL) {
		fprintf(stderr,"%s : Can't open config file <%s>\n",progname,file);
		return(-1);
	} else {
		/* Get config lines */
		while (fgets(line,80,cofg)!=NULL) {
			/* Lines preceded with ; and empty lines are skipped */
			if (line[0]!=';' && strcmp(line,"\n")!=0) {
				if (strncmp(line,unpackname,strlen(unpackname))==0) {

					/* UNPACK keyword (archiver to used for unpacking) */
					pos1=pos2=line+strlen(unpackname);
					while(strncmp(pos2,"\n",strlen("\n"))!=0) pos2++;
					len=pos2-pos1;
					cfg.UNPACK=(char *)malloc(sizeof(char)*(len+1));
					strncpy(cfg.UNPACK,pos1,len);
					cfg.UNPACK[len]='\0';
				} else if (strncmp(line,packname,strlen(packname))==0) {

					/* PACK keyword (archiver to use for packing) */
					pos1=pos2=line+strlen(packname);
					while(strncmp(pos2,"\n",strlen("\n"))!=0) pos2++;
					len=pos2-pos1;
					cfg.PACK=(char *)malloc(sizeof(char)*(len+1));
					strncpy(cfg.PACK,pos1,len);
					cfg.PACK[len]='\0';
				} else if (strncmp(line,filesname,strlen(filesname))==0) {

					/* FILES keyword (Location to where .log .tab .krl and .ksl files are) */
					pos1=pos2=line+strlen(filesname);
					while(strncmp(pos2,"\n",strlen("\n"))!=0) pos2++;
					len=pos2-pos1;
					cfg.FILES=(char *)malloc(sizeof(char)*(len+1));
					strncpy(cfg.FILES,pos1,len);
					cfg.FILES[len]='\0';
				} else if (strncmp(line,outboundname,strlen(outboundname))==0) {

					/* OUTBOUND keyword (location of .pkt files while packing) */
					pos1=pos2=line+strlen(outboundname);
					while(strncmp(pos2,"\n",strlen("\n"))!=0) pos2++;
					len=pos2-pos1;
					cfg.OUTBOUND=(char *)malloc(sizeof(char)*(len+1));
					strncpy(cfg.OUTBOUND,pos1,len);
					cfg.OUTBOUND[len]='\0';
				} else if (strncmp(line,inboundname,strlen(inboundname))==0) {

					/* INBOUND keyword (location of .pkt files while extracting) */
					pos1=pos2=line+strlen(inboundname);
					while(strncmp(pos2,"\n",strlen("\n"))!=0) pos2++;
					len=pos2-pos1;
					cfg.INBOUND=(char *)malloc(sizeof(char)*(len+1));
					strncpy(cfg.INBOUND,pos1,len);
					cfg.INBOUND[len]='\0';
				} else if (strncmp(line,verbosename,strlen(verbosename))==0) {

					/* VERBOSE keyword (Level at which messages are printed) */
					pos1=line+strlen(verbosename);
					if (getnum(&len,&pos1)==-1) {
						fprintf(stderr,"%s : VERBOSE level is not a number\n",progname);
						return(-1);
					} else {
						cfg.VERBOSE=len;
					}
				} else if (strncmp(line,loglevelname,strlen(loglevelname))==0) {

					/* LOGLEVEL keyword (Level at which message are written to the logfile) NOTUSED */
					pos1=line+strlen(loglevelname);
					if (getnum(&len,&pos1)==-1) {
						fprintf(stderr,"%s : LOGLEVEL level is not a number\n",progname);
						return(-1);
					} else {
						cfg.LOGLEVEL=len;
					}
				} else if (strncmp(line,logfilename,strlen(logfilename))==0) {

					/* LOGFILE keyword (Name of logfile EMPTY is no logging) */
					pos1=pos2=line+strlen(logfilename);
					while(strncmp(pos2,"\n",strlen("\n"))!=0) pos2++;
					len=pos2-pos1;
					if (len!=0) {
						cfg.LOGFILE=(char *)malloc(sizeof(char)*(len+1));
						strncpy(cfg.LOGFILE,pos1,len);
						cfg.LOGFILE[len]='\0';
					}
				} else if (strncmp(line,backupname,strlen(backupname))==0) {

					/* BACKUP keyword (Location to where .pkt files will be written */
					pos1=pos2=line+strlen(backupname);
					while(strncmp(pos2,"\n",strlen("\n"))!=0) pos2++;
					len=pos2-pos1;
					cfg.BACKUP=(char *)malloc(sizeof(char)*(len+1));
					strncpy(cfg.BACKUP,pos1,len);
					cfg.BACKUP[len]='\0';
				} else if (strncmp(line,tearlinename,strlen(tearlinename))==0) {

					/* TEARLINE keyword (Tearline to be used in messages) */
					pos1=pos2=line+strlen(tearlinename);
					while(strncmp(pos2,"\n",strlen("\n"))!=0) pos2++;
					len=pos2-pos1;
					cfg.TEARLINE=(char *)malloc(sizeof(char)*(len+1));
					strncpy(cfg.TEARLINE,pos1,len);
					cfg.TEARLINE[len]='\0';
				} else if (strncmp(line,originname,strlen(originname))==0) {

					/* ORIGIN keyword (Originline to be used or origin file) */
					pos1=pos2=line+strlen(originname);
					while(strncmp(pos2,"\n",strlen("\n"))!=0) pos2++;
					len=pos2-pos1;
					cfg.ORIGIN=(char *)malloc(sizeof(char)*(len+1));
					strncpy(cfg.ORIGIN,pos1,len);
					cfg.ORIGIN[len]='\0';
				}
			}
		}
	}
	fclose(cofg);
	return(0);
}

char *kludgespeclist=".ksl";
char *kludgerepllist=".krl";

/* Read .ksl and .krl files */
/* Kludge Specification List and Kludge Replacement List */
int readlist(char *name,struct kludges *kl) {
char file[80],line[128],*tline;
struct kludgelist *kll;
int len;
FILE *lst;

	printmsg(3,"ReadList\n");
	/* Open ksl file */
	strcpy(file,name);
	strcat(file,kludgespeclist);
	if ((lst=fopen(file,"r"))==NULL) {
		printmsg(0,"Couldn't open specification list file\n");
		return(-1);
	}
	
	while(fgets(line,80,lst)!=NULL) {
		if (line[0]==';' || !strcmp(line,"\n") ) {
			; /* newlines and lines begining with ';' are comment lines */
		} else {
		    if ((kll=(struct kludgelist *)malloc(sizeof(struct kludgelist)))==NULL) {
				fclose(lst);
				printmsg(0,"Can't allocate memory\n");
				return(-1);
			}
			kll->klg.kludge=NULL;
        	kll->klg.repl=NULL;
			kll->klg.spec=NULL;
            len=0;
            while(line[len]!=' ' && line[len]!=':' && line[len]!=CARRIAGERETURN && line[len]!=LINEFEED && line[len]!='\0') len++;
            if (line[len]==':') {
				len++;
			}
            if ((kll->klg.kludge=(char *)malloc((len+1)*sizeof(char)))==NULL) {
				fclose(lst);
				printmsg(0,"Can't allocate memory\n");
				return(-1);
			}
            strncpy(kll->klg.kludge,line,len);
			kll->klg.kludge[len]='\0';
            if (line[len]==CARRIAGERETURN || line[len]==LINEFEED || line[len]=='\0') {
                kll->klg.spec=NULL;
            } else {
                tline=line+len;
                len=0;
                while(tline[len]!=CARRIAGERETURN && tline[len]!=LINEFEED && tline[len]!='\0') len++;
                if ((kll->klg.spec=(char *)malloc((len+1)*sizeof(char)))==NULL) {
					fclose(lst);
					printmsg(0,"Can't allocate memory\n");
					return(-1);
				}
                strncpy(kll->klg.spec,tline,len);
				kll->klg.spec[len]='\0';
            }
            kll->next=kl->root;
            kl->root=kll;
		}
    }
    fclose(lst);
    strcpy(file,name);
    strcat(file,kludgerepllist);
    if ((lst=fopen(file,"r"))==NULL) {
		printmsg(0,"Couldn't open replacement list file\n");
		return(-1);
	}
	while(fgets(line,80,lst)!=NULL) {
		if (line[0]==';' || !strcmp(line,"\n") ) {
			; /* newlines and lines begining with ';' are comment lines */
		} else {
	        kll=kl->root;
	        while(strncmp(kll->klg.kludge,line,strlen(kll->klg.kludge))!=0) {
	            kll=kll->next;
	            if (kll==NULL) {
					printmsg(0,"Kludge replacement without kludge specification\n");
					break;
				}
	        }
	        if (kll!=NULL) {
	            tline=line+strlen(kll->klg.kludge);
	            len=0;
	            while(tline[len]!=CARRIAGERETURN && tline[len]!=LINEFEED && tline[len]!='\0') len++;
				if (len==0) kll->klg.repl=NIL;
				else {
					if ((kll->klg.repl=(char *)malloc((len+1)*sizeof(char)))==NULL) {
						fclose(lst);
						printmsg(0,"Can't allocate memory\n");
						return(-1);
					}
					strncpy(kll->klg.repl,tline,len);
					kll->klg.repl[len]='\0';
				}
	        }
		}
    }
	fclose(lst);

	kll=kl->root;
	printmsg(2,"Kludge list\n");
	while(kll!=NULL) {
		sprintf(line,"KLUDGE : <%s>\nSPEC   : <%s>\nREPL   : <%s>\n",kll->klg.kludge,kll->klg.spec,kll->klg.repl);
		printmsg(2,line);
		kll=kll->next;
	}
	printmsg(2,"\n");

	return(0);
}

char *table=".tab";

int readtab(char *filename,struct reroutetab *rrt) {
FILE *tab;
char line[80];
char file[128];
struct rrlist *wlk;
	
	printmsg(3,"ReadTab\n");
	strcpy(file,filename);
	strcat(file,table);
	if ((tab=fopen(file,"r"))==NULL) {
		sprintf(file,"Can't open : %s\n",filename);
		printmsg(0,file);
		exit(1);
	}
	while (fgets(line,80,tab)!=NULL) {
		if (line[0]==';' || !strcmp(line,"\n") ) {
			; /* newlines and lines begining with ';' are comment lines */
		} else {
			parseline(line,rrt);
		}
	}
	fclose(tab);

	/* Show all rerouting that will be done */
	printmsg(2,"Orginating Address Rerouting :\n");
	wlk=rrt->orgroot;
	while(wlk!=NULL) {
		sprintf(file,"%d:%d/%d.%d -> ",wlk->in.Zone,wlk->in.Net,wlk->in.Node,wlk->in.Point);
		printmsg(2,file);
		sprintf(file,"%d:%d/%d.%d\n",wlk->out.Zone,wlk->out.Net,wlk->out.Node,wlk->out.Point);
		printmsg(2,file);
		wlk=wlk->next;
	}
	printmsg(2,"Destination Address Rerouting :\n");
	wlk=rrt->dstroot;
	while(wlk!=NULL) {
		sprintf(file,"%d:%d/%d.%d -> ",wlk->in.Zone,wlk->in.Net,wlk->in.Node,wlk->in.Point);
		printmsg(2,file);
		sprintf(file,"%d:%d/%d.%d\n",wlk->out.Zone,wlk->out.Net,wlk->out.Node,wlk->out.Point);
		printmsg(2,file);
		wlk=wlk->next;
	}
	printmsg(2,"\n");

	return(0);
}

/* parse a line to see if it is a origin address or destination address that
   must be changed */

int parseline(char *line,struct reroutetab *rrt) {
int i;
struct rrlist *pl;
char txt[128];

	printmsg(3,"ParseLine\n");
	if ((pl=(struct rrlist *)malloc(sizeof(struct rrlist)))==NULL) {
		printmsg(0,"Can't allocate memory\n");
		return(-1);
	}
	for (i=0;i<3;i++) {
		line[i]=toupper(line[i]);
	}
	if (strncmp(line,"ORG ",4)==0) {		/* It's a origin reroute */
		if (parsedoubleaddress(&line[4],pl)==-1) return(-1);
		if (rrt->orgroot==NULL) {				/* first message to be added */
			rrt->orgroot=pl;
			rrt->orgroot->next=NULL;
			rrt->orgthread=rrt->orgroot;
		} else {							/* other messages (I use thread to add it to the tail) */
			rrt->orgthread->next=pl;
			rrt->orgthread=rrt->orgthread->next;
			rrt->orgthread->next=NULL;
		}
	} else {
		if (strncmp(line,"DST ",4)==0) {	/* It's a destination reroute */
			if (parsedoubleaddress(&line[4],pl)==-1) return(-1);
			if (rrt->dstroot==NULL) {				/* first message to be added */
				rrt->dstroot=pl;
				rrt->dstroot->next=NULL;
				rrt->dstthread=rrt->dstroot;
			} else {							/* other messages (I use thread to add it to the tail) */
				rrt->dstthread->next=pl;
				rrt->dstthread=rrt->dstthread->next;
				rrt->dstthread->next=NULL;
			}
		} else {							/* It's garbage */
			sprintf(txt,"Can't parse line : <%60s>\n",line);
			printmsg(0,txt);
			return(-1);
		}
	}
	return(0);
}

/* Get the two address involved in the rerouting */
int parsedoubleaddress(char *line,struct rrlist *pl) {
	if (parseaddress(line,&(pl->in),WILD)==-1) return(-1);
	while(*line!=' ' && *line!='\t') line++;
	while(*line==' ' || *line=='\t') line++;
	if (parseaddress(line,&(pl->out),WILD)==-1) return(-1);
	return(0);
}
