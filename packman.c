/* PackMan.c */

/*
   This program modifies the fidonet addresses in a packet using the file
   'packman.tab'. It is used like an archiver.
   It also modifies kludges using 'packman.ksl' and 'packman.krl'
*/

#include "packman.h"

struct config cfg;
char *progname;
char *NIL= "NIL";
FILE *logfile;

char *Version = "1.30";
char *Announce = "Made by Rico Jansen for the HIN";
char *Foul = "This PackMan version has been hacked!!";

int stripname(char **name,char *called);

void main(int argc,char **argv) {
struct reroutetab rrt;
struct kludges kl;
char *cmplname,*wlk,configplace[255],txt[128];
int walk,chkv=1968,chka=1968,chkf=1968;
long logtime;

	stripname(&progname,argv[0]);
	fprintf(stdout,"%s version %s\n%s\n",progname,Version,Announce);
	fprintf(stdout,"Alpha Release\n");
	if (argc<3) {
		fprintf(stderr,"Usage : %s <x|a> <arcmail> [packet] ... [packet]\n",progname);
	} else {
		/* Configuration at MAIL: */
#ifdef USE
		strcpy(configplace,"MAIL:");
		strcat(configplace,progname);
#else
		strcpy(configplace,progname);
		fprintf(stdout,"Programming Version\n");
#endif
		readconfig(configplace); 		/* read configuration */
		cmplname=(char *)malloc(sizeof(char)*(strlen(cfg.FILES)+strlen(progname)));
		strcpy(cmplname,cfg.FILES);
		strcat(cmplname,progname);

		if (cfg.LOGFILE!=NULL) {
			strcpy(txt,cfg.FILES);
			strcat(txt,cfg.LOGFILE);
			if ((logfile=fopen(txt,"a"))==NULL) {
				fprintf(stderr,"%s : Can't open logfile %s\nLogging Disabled\n",progname,configplace);
			}
		} else {
			logfile=NULL;
		}

		/* Print header */
		time(&logtime);
		sprintf(txt,"\n%s %s at %s",progname,Version,ctime(&logtime));
		printmsg(1,txt);

		/* Print configuration */
		printmsg(2,"Configuration :\n");
		sprintf(txt,"PACK     : <%s>\nUNPACK   : <%s>\n",cfg.PACK,cfg.UNPACK);
		printmsg(2,txt);
		sprintf(txt,"FILES    : <%s>\nBACKUP   : <%s>\n",cfg.FILES,cfg.BACKUP);
		printmsg(2,txt);
		sprintf(txt,"VERBOSE  : <%d>\nLOGLEVEL : <%d>\n",cfg.VERBOSE,cfg.LOGLEVEL);
		printmsg(2,txt);
		sprintf(txt,"LOGFILE  : <%s>\n",cfg.LOGFILE);
		printmsg(2,txt);
		sprintf(txt,"OUTBOUND : <%s>\nINBOUND  : <%s>\n\n",cfg.OUTBOUND,cfg.INBOUND);
		printmsg(2,txt);
		sprintf(txt,"TEARLINE : <%s>\nORIGIN   : <%s>\n\n",cfg.TEARLINE,cfg.ORIGIN);
		printmsg(2,txt);

		rrt.orgroot=NULL;				/* initialize routing structure */
		rrt.orgthread=NULL;
		rrt.dstroot=NULL;
		rrt.dstthread=NULL;
		readtab(cmplname,&rrt);			/* read routing table */
		kl.root=NULL;
		readlist(cmplname,&kl);			/* read kludge lists */
		switch(argv[1][0]) { 			/* Which command */
			case 'a': addpackets(argv,&rrt,&kl); /* Add packets to archive */
					  break;
			case 'x': extractpackets(argv,&rrt,&kl); /* Extract packets from archive */
					  break;
			default : fprintf(stderr,"%s : Unknown command\n",progname);
					  break;
		}
		cleanup(&rrt,&kl);		/* deallocate memory used */

		fclose(logfile);
	}
	exit(0);
}

/* removes the path from the called string and does some other stuff */
int stripname(char **name,char *called) {
char *pos;

	pos=called+strlen(called);
	while(*pos!='\\' && *pos!='/' && pos!=called) pos--;
	if (pos!=called) pos++;
	(*name)=malloc(sizeof(char)*(strlen(pos)+1));
	strcpy((*name),pos);
	if (strlen((*name))!=7) {
		fprintf(stderr,"Hey I haven't got my original name!\n");
	} else {
		(*name)[0]=tolower((*name)[0]);
		(*name)[1]=tolower((*name)[1]);
		(*name)[2]=tolower((*name)[2]);
		(*name)[3]=tolower((*name)[3]);
		(*name)[4]=tolower((*name)[4]);
		(*name)[5]=tolower((*name)[5]);
		(*name)[6]=tolower((*name)[6]);
	}
	return(0);
}
