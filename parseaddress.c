/* parse.c */

#include "packman.h"

/* 
ParseAddress : Parse a fidonet address (very forgiving)

   uses a finite state machine using a method described in :
   Computer Language Volume 8, Number 5 (May 1991) article,
   "Goto Yes, goto !" by Tim Cooper

Parameters :
char *line			: pointer to first character of address to be parsed
struct fidaddr *fid	: Pointer to structure which will contain parsed address
					  (structure must be allocated)

Returnvalues :
Succes :
    x:y/z.q   4 |  3  x:y/z
      y/z.q   3 |  2    y/z
        z.q   2 |  1      z
             +8 | +0
			----+----
           return value

Failure : -1
*/
int parseaddress(char *line,struct fidaddr *fid,char wild) {
char symbol;
int transport,type;

	printmsg(3,"ParseAddress\n");
	/* Initialize structure on 0 */
	fid->Zone=fid->Net=fid->Node=fid->Point=0;
	/* First part must be a number or wildcard */
	if (wild==NOWILD || *line!='*') {
		if (getnum(&transport,&line)==-1) {
			nextstate(NUMERROR);
		}
	} else {
		transport=WILDCARD;
		line++;
	}
	state(START):	/* Here we go */
					type=1; /* Number of address fields */
					switch(symbol) {
						case ':' :	/* It is address with either 3 or 4 */
									/* address fields */
									fid->Zone=transport;
									if (*line!='*' || wild==NOWILD) {
										if (getnum(&transport,&line)==-1) {
											nextstate(NUMERROR);
										}
									} else {
										transport=WILDCARD;
										line++;
									}
									nextstate(ADDR3OR4);
						case '/' :	/* It is address with either 2 or 3 */
									/* address fields */
									fid->Net=transport;
									if (*line!='*' || wild==NOWILD) {
										if (getnum(&transport,&line)==-1) {
											nextstate(NUMERROR);
										}
									} else {
										transport=WILDCARD;
										line++;
									}
									nextstate(ADDR2OR3);
						case '.' :	/* It is address with 2 */
									/* address fields */
									fid->Node=transport;
									if (*line!='*' || wild==NOWILD) {
										if (getnum(&transport,&line)==-1) {
											nextstate(NUMERROR);
										}
									} else {
										transport=WILDCARD;
										line++;
									}
									nextstate(ADDR2);
						default:	/* It is a 1 field address */
									fid->Node=transport;
									nextstate(END);
					}

	state(ADDR3OR4):type+=1; /* Add 1 to number of fields */
					/* There needs to be a slash otherwise */
					/* it is isn't a correct address */
					if (symbol=='/') {
						fid->Net=transport;
						if (*line!='*' || wild==NOWILD) {
							if (getnum(&transport,&line)==-1) {
								nextstate(NUMERROR);
							}
						} else {
							transport=WILDCARD;
							line++;
						}
						nextstate(ADDR2OR3);
					} else {
						nextstate(ERROR);
					}

	state(ADDR2OR3):type+=1; /* Add 1 to number of fields */
					/* It going to be address with or without Point */
					if (symbol=='.') {
						fid->Node=transport;
						if (*line!='*' || wild==NOWILD) {
							if (getnum(&transport,&line)==-1) {
								nextstate(NUMERROR);
							}
						} else {
							transport=WILDCARD;
							line++;
						}
						nextstate(ADDR2);
					} else {
						fid->Node=transport;
						nextstate(END);
					}

	state(ADDR2):	type+=1; /* Add 1 to number of fields */
					type+=8; /* 4D type message */
					fid->Point=transport;
					nextstate(END);

	state(ERROR):	printmsg(0,"Parse error malformed address\n");
					type=-1;
					nextstate(END);

	state(NUMERROR):printmsg(0,"Parse error invalid address\n");
					type=-1;
					nextstate(END);

	state(END):		return(type);
}
