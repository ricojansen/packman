/* kludge.c */

#include "packman.h"

int kludgereplace(char **text,int *pos,struct kludgelist *klq,struct reroutetab *rrt) {
struct kludgelist *wlk;
int qq;
char txt[255];

	printmsg(3,"KludgeReplace\n");

	printmsg(1,"Kludge  : ");
	qq=0;
	while((*text)[*pos+qq]!=CR) {
		txt[qq]=(*text)[*pos+qq];
		qq++;
	}
	txt[qq]='\0';
	strcat(txt," -> ");
	printmsg(1,txt);

	wlk=klq;
	while(strncmp(wlk->klg.kludge,(*text)+*pos,strlen(wlk->klg.kludge))!=0) {
		wlk=wlk->next;
		if (wlk==NULL) {
			printmsg(1,"Not found\n");
			return(-1);
		}
	}
	if (wlk->klg.repl==NULL) {
			printmsg(1,"No Action\n");
			return(0);
	}
	if (wlk->klg.repl==NIL) {
		int len=0;
		printmsg(1,"Remove\n");
		while((*text)[*pos+len]!=CARRIAGERETURN) len++;
		len++; /* to catch CR */
		(*pos)--;
		replacestring(text,len+1,*pos,""); /* len+1 and pos-1 to catch CTRLA */
        return(0);
	} else {
		printmsg(1,"Modify\n");
        (*pos)+=strlen(wlk->klg.kludge);
        parsespec(text,pos,&(wlk->klg),rrt);
        return(0);
    }
}

int parsespec(char **text,int *pos,struct kludge *klg,struct reroutetab *rrt) {
char symbol;
char *line;
char *rep;
int  repeat,type=0,keep=0,len;

	printmsg(3,"ParseSpec\n");
	line=klg->spec;
	rep=klg->repl;
	state(LOOP):	switch(symbol) {
						case '%' : nextstate(TOKEN);
						case '\0': nextstate(keepEND);
						default  : if ((*text)[*pos]!=symbol) {
									printmsg(0,"Kludge spefication doesn't match\n");
									type=-1;
									nextstate(keepEND);
								   } else {
									rep++;
									(*pos)++;
									nextstate(LOOP);
								   }
					}
	state(TOKEN):	if (isnum(symbol)) {
						line--;
						getnum(&repeat,&line);
						nextstate(TYPE);
					} else {
						switch(symbol) {
							case '%' : if ((*text)[*pos]!=symbol) {
										printmsg(0,"Kludge specification doesn't match\n");
										type=-1;
										nextstate(keepEND);
									   } else {
										rep++;
										(*pos)++;
										keep=0;
										nextstate(LOOP);
									   }
							case 's' : if (!keep) {
										   len=0;
										   while((*text)[*pos+len]!=CR) len++;
										   replacestring(text,len,*pos,rep);
									   }
									   keep=0;
									   nextstate(keepEND);
							case 'k' : keep=1;
									   nextstate(TOKEN);
							default:   printmsg(0,"Invalid kludge specification\n");
									   type=-1;
									   nextstate(keepEND);
						}
					}
	state(TYPE):	switch(symbol) {
						case 'a' : if (repeat==0) { char *oldrep;
										oldrep=rep;
										while((*text)[*pos]!=CR) {
											doaddress(text,pos,&rep,rrt,keep);
											rep=oldrep;
											if ((*text)[*pos]!=CR) {
												while(!isnum((*text)[*pos])) (*pos)++;
											}
										}
								   } else {
										while(repeat--) {
											doaddress(text,pos,&rep,rrt,keep);
										}
								   }
								   keep=0;
								   nextstate(LOOP);
						case 'w' : if (repeat==0) { char *oldrep;
										oldrep=rep;
										while(*rep!='\0') {
											doword(text,pos,&rep,keep);
											rep=oldrep;
										}
								   } else {
										while(repeat--) {
											doword(text,pos,&rep,keep);
										}
								   }
								   keep=0;
								   nextstate(LOOP);
						case 'n' : if (repeat==0) { char *oldrep;
										oldrep=rep;
										while(*rep!='\0') {
											donum(text,pos,&rep,keep);
											rep=oldrep;
										}
								   } else {
										while(repeat--) {
											donum(text,pos,&rep,keep);
										}
								   }
								   keep=0;
								   nextstate(LOOP);
						default:   keep=0;
								   printmsg(0,"Unknown token in kludge specification\n");
								   nextstate(LOOP);
					}
	state(END):		return(type);
}

int parserepl(char **rep) {
char *line;
char symbol;
int type=0;

	printmsg(3,"ParseRepl\n");
	line=*rep;
	state(BEGIN):	switch(symbol) {
						case '%' : nextstate(TYPE);
						default:   printmsg(0,"Replacement specification doesn't match\n");
								   type=-1;
								   nextstate(keepEND);
					}
	state(TYPE):	switch(symbol) {
						case 'o' : type=1;
								   nextstate(keepEND);
						case 'd' : type=2;
								   nextstate(keepEND);
						default:   printmsg(0,"Unknown token in replacement specification\n");
								   type=-1;
								   nextstate(keepEND);
					}
	state(END):		*rep=line;
					return(type);
}
