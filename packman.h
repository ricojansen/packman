/* PackMan.h */

/* 
   Header file for reroute, contains all prototypes, structures and most of
   the defines
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "fido.h"	/* A header file with fidonet structures */


/* A 4D fidonet address */
struct fidaddr {
	int Zone;
	int	Net;
	int Node;
	int Point;
};

/* List in wich changes are stored */
struct rrlist {
	struct rrlist *next;
	struct fidaddr in,out;
};

/* List containing the packed messages */
struct msglist {
		struct msglist *next;
		struct PackedMessage pmsg;
		char   FromUser[36];
		char   ToUser[36];
		char   Subject[72];
		char   *Text;
};

/* Structure containing a full '.PKT' file */
struct packet {
	struct PacketHeader pkh;
	struct msglist *root,*thread;
};

/* Structure containing all replacement info */
struct reroutetab {
	struct rrlist *orgroot,*orgthread;
	struct rrlist *dstroot,*dstthread;
};

struct kludge {
	char *kludge;
	char *spec;
	char *repl;
};

struct kludgelist {
	struct kludgelist *next;
	struct kludge klg;
};

struct kludges {
	struct kludgelist *root;
};

struct config {
	char *PACK;
	char *UNPACK;
	char *FILES;
	char *INBOUND;
	char *OUTBOUND;
	int  VERBOSE;
	int  LOGLEVEL;
	char *LOGFILE;
	char *BACKUP;
	char *TEARLINE;
	char *ORIGIN;
};

struct namelist {
	char name[13]; /* 8 + 1 + 3 + 1 (12345678.PKT) terminated with 0 */
	struct namelist *next;
};

/* 2 defines to split a 16 bit word in 2*8 bit and vice versa */
#define unsplit(x) (int)((int)x ## LO+256*(int)x ## HI)
#define split(x,y) { int qqo; qqo=y; x ## HI=(byte)(qqo>>8);x ## LO=(byte)(qqo-256*(int)x ## HI); }

/* Constants */
#define CTRLA			(0x01)
#define CARRIAGERETURN	(0x0D)
#define CR CARRIAGERETURN
#define LINEFEED		(0x0A)

#define WILD	(01)
#define NOWILD  (00)
#define WILDCARD (-1)

#define NOMATCH (0)
#define RETAIN  (1)
#define USEOUT  (2)

/* Define to see if a character is in 0..9 range */
#define isnum(c) (isalnum(c) && !isalpha(c))

/* defines to create a finite state machine */
#define state(s) s: symbol=*line;line++; keep##s
#define nextstate(n) goto n


/* cleanup.c */
int cleanup(struct reroutetab *rrt,struct kludges *kl);
int cleanuppacket(struct packet *pkt);
int cleanupnamelist(struct namelist *root);

/* io.c */
int readpacket(char *filename,struct packet *pakt);
int addmessage(FILE *pkt,struct msglist *msgl,struct packet *pakt);
int getstring(char *string,int maxsize,FILE *pkt);
int writepacket(char *filename,struct packet *pakt);
int putstring(char *text,FILE *pkt);

/* modifypacket.c */
int modifypacket(struct packet *pakt,struct reroutetab *rrt,struct kludges *kl);
int changekludge(char **text,struct kludgelist *kl,struct reroutetab *rrt);

/* change.c */
int changeseenby(char **text,struct rrlist *root);
int changeorigin(char **text,struct rrlist *root);
int changestr(char **text,int *pos,struct rrlist *root);
int doaddress(char **text,int *pos,char **rep,struct reroutetab *rrt,int keep);
int doword(char **text,int *pos,char **rep,int keep);
int donum(char **text,int *pos,char **rep,int keep);
int changetearline(char **text);

/* kludge.c */
int kludgereplace(char **text,int *pos,struct kludgelist *kl,struct reroutetab *rrt);
int parsespec(char **text,int *pos,struct kludge *klg,struct reroutetab *rrt);
int parserepl(char **rep);

/* changeaddr.c */
int changeaddr(char **text,int pos,int len,struct rrlist *root,struct fidaddr *fd,int type);
int dochange(struct fidaddr *chg,struct rrlist *root);
char decide(int in,int out,int chg);

/* parse.c */
int parseaddress(char *line,struct fidaddr *fid,char wild);

/* misc.c */
int replacestring(char **text,int replen,int pos,char *str);
int printmsg(int level,char *msg);
int getnum(int *num,char **line);
int strtoupper(char *str);

/* config.c */
int readtab(char *filename,struct reroutetab *rrt);
int parseline(char *line,struct reroutetab *rrt);
int parsedoubleaddress(char *line,struct rrlist *pl);
int readlist(char *name,struct kludges *kl);
int readconfig(char *name);

/* pack.c */
int pack(char *name,struct namelist *root);
int unpack(char *name,struct namelist **root);
int findnames(struct namelist **root,char *name);
int extractpackets(char *argv[],struct reroutetab *rrt,struct kludges *kl);
int addpackets(char *argv[],struct reroutetab *rrt,struct kludges *kl);

/* Program name used on several places */
extern char *progname;
extern char *NIL;
extern struct config cfg;
extern FILE *logfile;
