/* Header for FidoNet messages structures e.d. */

#define byte unsigned char

/* Stored Message (as in *.MSG) */
struct msgPART1 {
	byte fromUserName[36];
	byte toUserName[36];
	byte subject[72];
	byte DateTime[20];
	byte timesreadLO;	byte timesreadHI;
	byte destNodeLO;	byte destNodeHI;
	byte origNodeLO;	byte origNodeHI;
	byte costLO;		byte costHI;
	byte origNetLO;		byte origNetHI;
	byte destNetLO;		byte destNetHI;
};
struct msgOPT1 {
	byte destZoneLO;	byte destZoneHI;
	byte origZoneLO;	byte origZoneHI;
	byte destPointLO;	byte destPointHI;
	byte origPointLO;	byte origPointHI;
};
struct msgPART2 {
	byte replyToLO;		byte replyToHI;
	byte AttributeLO;	byte AttributeHI;
	byte nextReplyLO;	byte nextReplyHI;
	/* Text starts here */
};

struct msg {
	byte fromUserName[36];
	byte toUserName[36];
	byte subject[72];
	byte DateTime[20];
	byte timesreadLO;	byte timesreadHI;
	byte destNodeLO;	byte destNodeHI;
	byte origNodeLO;	byte origNodeHI;
	byte costLO;		byte costHI;
	byte origNetLO;		byte origNetHI;
	byte destNetLO;		byte destNetHI;

	byte destZoneLO;	byte destZoneHI;
	byte origZoneLO;	byte origZoneHI;
	byte destPointLO;	byte destPointHI;
	byte origPointLO;	byte origPointHI;

	byte replyToLO;		byte replyToHI;
	byte AttributeLO;	byte AttributeHI;
	byte nextReplyLO;	byte nextReplyHI;
	/* Text starts here */
};

#define PMID1	02
#define PMID2	00

/* Packed Message (as contained in *.PKT) */
struct PackedMessage {
	/* Line with 2 bytes (02 and 00) */
	byte origNodeLO;	byte origNodeHI;
	byte destNodeLO;	byte destNodeHI;
	byte origNetLO;		byte origNetHI;
	byte destNetLO;		byte destNetHI;
	byte AttributeLO;	byte AttributeHI;
	byte costLO;		byte costHI;
	byte DateTime[20];
	/* toUserName max 36 0 terminated */
	/* fromUserName max 36 0 terminated */
	/* subject max 72 0 terminated */
	/* Text starts here */
};

/* PacketHeader (as in *.PKT) */
struct PacketHeader {
	byte origNodeLO;	byte origNodeHI;
	byte destNodeLO;	byte destNodeHI;
	byte yearLO;		byte yearHI;
	byte monthLO;		byte monthHI;
	byte dayLO;			byte dayHI;
	byte hourLO;		byte hourHI;
	byte minuteLO;		byte minuteHI;
	byte secondLO;		byte secondHI;
	byte baudLO;		byte baudHI;
	byte ID1;			byte ID2;
	byte origNetLO;		byte origNetHI;
	byte destNetLO;		byte destNetHI;
	byte prodcode;
	byte serialno;
	byte password[8];
	byte origZoneLO;	byte origZoneHI;
	byte destZoneLO;	byte destZoneHI;
	byte fill[20];
	/* 0 or more packets */
	/* followed by 2 bytes 00H */
};
