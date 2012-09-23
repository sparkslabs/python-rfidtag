int rfidtag_init(void);
int rfidtag_config(int time, int rate);
int rfidtag_close(void);
int rfidtag_seek(void);
unsigned long long rfidtag_getID1();
unsigned long long rfidtag_getID2();
int rfidtag_getReaderID();


