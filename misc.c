#include "packman.h"

/* 
ReplaceString : Replaces a string in a piece of text with another string

Parameters :
char **text		= Address of pointer to text
int replacelen	= Length of string to be replaced
int pos			= Position of string to be replaced
char *string	= String which will replace the old string

Returnvalues :
0	= Success
-1	= Failure
*/
int replacestring(char **text,int replacelen,int pos,char *string) {
int q,l,shift,len;
char *tmp,*ps;

	printmsg(3,"Replacestring\n");
	/* Calculate number of bytes to shift */
	len=strlen(string);
	shift=len-replacelen;
	ps=(*text)+pos;
	if (shift!=0) {
		if (shift<0) { /* string shrinks */
			l=strlen(ps+replacelen);
			/* Move text */
			for (q=0;q<=l;q++) ps[len+q]=ps[replacelen+q];
			/* Shrink text */
			tmp=realloc(*text,strlen(*text)+1);
			if (tmp==NULL) {
				printmsg(0,"Can't reallocate memory\n");
				return(-1);
			}
			*text=tmp;
			ps=*text+pos;
		} else { /* string expands */
			/* Expand text */
			tmp=realloc(*text,strlen(*text)+shift+1);
			if (tmp==NULL) {
				printmsg(0,"Can't reallocate memory\n");
				return(-1);
			}
			*text=tmp;
			ps=*text+pos;
			l=strlen(ps+replacelen);
			/* Move text */
			for (q=l;q>=0;q--) ps[q+len]=ps[q+replacelen];
		}
	}
	/* Do the replacement */
	for (q=0;q<len;q++) ps[q]=string[q];
	return(0);
}

/* Error message print routine, will print  msg and optional
   if VERBOSE is larger then level
*/

int printmsg(int level,char *msg) {
	if (cfg.VERBOSE>level) {
		fprintf(stderr,"%s",msg);
	}
	if (logfile!=NULL && cfg.LOGLEVEL>level) {
		fprintf(logfile,"%s",msg);
	}
	return(0);
}

/* Get a number from a string (like atoi only with error detection) */
int getnum(int *num,char **line) {
	if (isnum(**line)) {
		*num=(int)(**line-'0');
		(*line)++;
		while(isnum(**line)) {
			*num=*num*10+(int)(**line-'0');
			(*line)++;
		}
		return(0);
	} else {
		return(-1);
	}
}

/* Uppercases a whole string */
int strtoupper(char *str) {
	while(*str!='\0') {
		*str=toupper(*str);
		str++;
	}
	return(0);
}
