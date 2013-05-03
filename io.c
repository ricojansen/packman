#include "packman.h"

/*
ReadPacket : Reads a fidonet packet.

Parameters :
char *filename 		= Name of file to be read (full path)
struct packet *pakt = Pointer to structure of type packet
					  (structure must be allocated)

Return value :
0	= Succes
-1	= Failure
*/
int readpacket(char *filename,struct packet *pakt) {
FILE *pkt;
byte ID1,ID2;
struct msglist *msgl;
char txt[128];

	printmsg(3,"ReadPacket\n");
	/* Open the file */
	if ((pkt=fopen(filename,"r"))==NULL) {
		sprintf(txt,"Can't open file %s\n",filename);
		printmsg(0,txt);
		return(-1);
	}
	/* Get the packet header */
	if (fread((char *)&(pakt->pkh),sizeof(struct PacketHeader),1,pkt)!=1) {
		printmsg(0,"Read Error in readpacket (Header)\n");
		fclose(pkt);
		return(-1);
	}

	/* Get all packed messages */
	do {/* Check ID values */
		if (fread(&ID1,sizeof(ID1),1,pkt)!=1) {		
			printmsg(0,"Read Error in readpacket (ID1)\n");
			fclose(pkt);
			return(-1);
		} else {
			if (fread(&ID2,sizeof(ID2),1,pkt)!=1) {
				printmsg(0,"Read Error in readpacket (ID2)\n");
				fclose(pkt);
				return(-1);
			} else {
				/* Support of type 2 messages only */
				if (ID1==2 && ID2==0) {
						if ((msgl=(struct msglist *)malloc(sizeof(struct msglist)))==NULL) {
							printmsg(0,"Memory allocation error\n");
							fclose(pkt);
							return(-1);
						}
						/* Get packedmessage header */
						if (fread((char *)&(msgl->pmsg),sizeof(struct PackedMessage),1,pkt)!=1) {
							printmsg(0,"Read Error in readpackedmessages\n");
							fclose(pkt);
							return(-1);
						} else {
							/* add the message to the list in the packet structure */
							addmessage(pkt,msgl,pakt);
						}
				}
			}
		}
	} while (ID1!=0 || ID2!=0); /* Until there are no more messages */
	fclose(pkt);
	return(0);
}

/*
WritePacket : Writes a fidonet packet

Parameters:
char *filename		= Name of file to be written to (full path)
struct packet *pakt	= Pointer to packetstructure to be written

Return values:
0	= Success
-1	= Failure
*/
int writepacket(char *filename,struct packet *pakt) {
FILE *wpkt;
struct msglist *wlk;
byte ID1=2,ID2=0;
char txt[128];

	printmsg(3,"WritePacket\n");
	if ((wpkt=fopen(filename,"w"))==NULL) {
		sprintf(txt,"Can't open file %s\n",filename);
		printmsg(0,txt);
		return(-1);
	}
	/* write the packetheader */
	if (fwrite((char *)(&(pakt->pkh)),sizeof(struct PacketHeader),1,wpkt)==0) {
		printmsg(0,"Can't write to file\n");
		fclose(wpkt);
		return(-1);
	}
	/* write all packed messages */
	wlk=pakt->root;
	while(wlk!=NULL) {
		/* Write ID bytes */
		if (fwrite(&ID1,sizeof(ID1),1,wpkt)==0) {
			printmsg(0,"Can't write ID1\n");
			fclose(wpkt);
			return(-1);
		}
		if (fwrite(&ID2,sizeof(ID2),1,wpkt)==0) {
			printmsg(0,"Can't write ID2\n");
			fclose(wpkt);
			return(-1);
		}
		/* Write the packedmessage Header */
		if (fwrite((char *)&(wlk->pmsg),sizeof(struct PackedMessage),1,wpkt)==0) {
			printmsg(0,"Can't write pmsg\n");
			fclose(wpkt);
			return(-1);
		}
		/* Write the message body */
		putstring(wlk->ToUser,wpkt);
		putstring(wlk->FromUser,wpkt);
		putstring(wlk->Subject,wpkt);
		putstring(wlk->Text,wpkt);
		wlk=wlk->next;
	}
	/* write the end of the message */
	ID1=0;
	if (fwrite(&ID1,sizeof(ID1),1,wpkt)==0) {
		printmsg(0,"Can't write to ID1F\n");
		fclose(wpkt);
		return(-1);
	}
	if (fwrite(&ID2,sizeof(ID2),1,wpkt)==0) {
		printmsg(0,"Can't write to ID2F\n");
		fclose(wpkt);
		return(-1);
	}
	fclose(wpkt);
	return(0);
}

/* Write a zero terminated string to a file */
int putstring(char *text,FILE *wpkt) {
	fputs(text,wpkt);
	fputc('\0',wpkt);
	return(0);
}

/* Get a zero terminated string from a file */
int getstring(char *string,int maxsize,FILE *pkt) {
int ch,i=0;
	while(((char)(ch=fgetc(pkt)))!='\0' && ch!=EOF && i<maxsize) {
		string[i]=ch;
		i++;
	}
	string[i]='\0';
	return(0);
}


/* Starting textbuffer size */
#define TXTBUF	1024
/* Amount of text to buffered between calls to replacestring */
#define READBUF 128

/* Add a message to the packet structure */
int addmessage(FILE *pkt,struct msglist *msgl,struct packet *pakt) {
int chr;
int i,tot;
char buf[READBUF+1];

	printmsg(3,"AddMessage\n");
	if (pakt->root==NULL) {				/* first message to be added */
		pakt->root=msgl;
		pakt->root->next=NULL;
		pakt->thread=pakt->root;
	} else {							/* other messages (I use thread to add it to the tail) */
		pakt->thread->next=msgl;
		pakt->thread=pakt->thread->next;
		pakt->thread->next=NULL;
	}
	/* Get first part of Message */
	getstring(pakt->thread->ToUser,36,pkt);
	getstring(pakt->thread->FromUser,36,pkt);
	getstring(pakt->thread->Subject,72,pkt);

	/* Get the Text part of the message */
	pakt->thread->Text=(char *)malloc(TXTBUF*sizeof(char));
	pakt->thread->Text[0]='\0';
	i=0;
	tot=0;
	while((chr=fgetc(pkt))!=0 && chr!=EOF) {
			if (chr==LINEFEED) continue;	
			buf[i++]=chr;
			if (i>=READBUF) { /* We buffer READBUF bytes to speed up things */
				buf[i]='\0';
				/* replacestring is used here as an append */
				/* it will allocate extra memory if needed */
				replacestring(&(pakt->thread->Text),0,tot,buf);
				tot+=i;
				i=0;
			}
	}
	buf[i]='\0';
	/* Add the last buffered bit */
	/* replacestring is used here as an append */
	/* it will allocate extra memory if needed */
	replacestring(&(pakt->thread->Text),0,tot,buf);
	tot+=i;
	pakt->thread->Text[tot]='\0';
	return(0);
}
