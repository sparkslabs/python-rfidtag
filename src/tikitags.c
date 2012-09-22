//sudo /etc/init.d/pcscd stop
//gcc -Wall -W -L/usr/lib/ -lusb tikitags.c -o tiki

#include <usb.h>
#include <stdio.h>

char INIT1[15] = { 0x6f, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x14, 0x00, 0x00, 0x08 };
char INIT2[15] = { 0x6f, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x14, 0x04, 0x00, 0x06 };
char INIT3[21] = { 0x6f, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x06, 0xD4, 0x32, 0x05, 0x00, 0x00, 0x00 };
char INIT4[15] = { 0x6f, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xC0, 0x00, 0x00, 0x04 };
char status1[10] = { 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
char power_up[10] = { 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
char power_down[10] = { 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
char abort_pipe[10] = { 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
char poll1[20] = { 0x6f, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x05, 0xd4, 0x60, 0x01, 0x01, 0x10 };
char get1[15] = { 0x6f, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x05 };
char recvbuf[64] = { 0x00 };            //full data hex array recieved from reader (includes header, full in specifed in ACR122 API, etc)
char id1[16] = { 0x00 };                //id of first tag found, [0]=reader found on, [1:8]=tag ID
char id2[16] = { 0x00 };                //id of second tag if two tags are found on one reader
unsigned long long id1_int = -1;
unsigned long long id2_int = -1;
int readerID = -1;
char count = 0;                         //insturction count, [6] of each header is the count. May not be necessary...
int numoftiki = 0;                      //number of tikitag readers
int seektime = 0;		//in ms
int looprate = 0;		//in ms max of 1000ms (1sec)
struct usb_device* tikidev[10];
struct usb_dev_handle* tiki[10];

 /*
  ** tiki_xchange(udp, *outp, out_len, *inp, in_len)
  **      send a command to the reader and gets the answer
  **      returns 0 if all goes well; -1 if not
  **LOCKED IN
  */
int
tiki_xchange (struct usb_dev_handle *udp, char *outp, int out_len, char *inp,
	      int in_len)
{
  int bytes_sent;
  int bytes_recv;

  count++;
  if ((bytes_sent =
       usb_bulk_write (udp, 0x02, (char *) outp, out_len, 10)) < 0)
    {
      if ((bytes_sent =
	   usb_bulk_write (udp, 0x02, (char *) outp, out_len, 10)) < 0)
	{
	  perror ("usb_bulk_write");
	}
    }
  if ((bytes_recv = usb_bulk_read (udp, 0x82, (char *) inp, in_len, 10)) < 0)
    {
      if ((bytes_recv =
	   usb_bulk_read (udp, 0x82, (char *) inp, in_len, 100)) < 0)
	{
	  perror ("usb_bulk_read");
	  return (-1);
	}
    }

  return bytes_recv;
}

 /*
  **
  **Power up and send Init Commands from working python code
  **        Init3[]-Immediate result of polling command
  **LOCKED IN
  */

int
tiki_setup (struct usb_dev_handle *udp)
{
  power_up[6] = count;
  if (tiki_xchange
      (udp, power_up, sizeof (power_up), recvbuf, sizeof (recvbuf)) < 0)
    {
      perror ("xchange");
      return (-1);
    }

  INIT1[6] = count;
  if (tiki_xchange (udp, INIT1, sizeof (INIT1), recvbuf, sizeof (recvbuf)) <
      0)
    {
      perror ("xchange");
      return (-1);
    }
  INIT2[6] = count;
  if (tiki_xchange (udp, INIT2, sizeof (INIT2), recvbuf, sizeof (recvbuf)) <
      0)
    {
      perror ("xchange");
      return (-1);
    }
  INIT3[6] = count;
  if (tiki_xchange (udp, INIT3, sizeof (INIT3), recvbuf, sizeof (recvbuf)) <
      0)
    {
      perror ("xchange");
      return (-1);
    }
  INIT4[6] = count;
  if (tiki_xchange (udp, INIT4, sizeof (INIT4), recvbuf, sizeof (recvbuf)) <
      0)
    {
      perror ("xchange");
      return (-1);
    }
  return (0);
}

unsigned long long
tiki_getID1 ()
{
  return id1_int;
}

unsigned long long
tiki_getID2 ()
{
  return id2_int;
}

int
tiki_getReaderID ()
{
  return readerID;
}

/*
**
**Tikitag Initalization Sequence
**  Returns numbe rof devices found
**LOCKED IN
*/
int
tiki_init (void)
{
  struct usb_bus *busses, *bus;
  struct usb_device *dev;

  int i = 0;

  usb_init ();
  usb_find_busses ();
  usb_find_devices ();

  busses = usb_get_busses ();
  for (bus = busses; bus; bus = bus->next)
    {
      for (dev = bus->devices; dev; dev = dev->next)
	{
	  if (dev->descriptor.idVendor == 0x072f &&	/* ACS */
	      dev->descriptor.idProduct == 0x90cc)	/* ACR122 */
	    {
	      printf ("Found tikitag reader %d!\n", numoftiki);
	      tikidev[numoftiki] = dev;
	      numoftiki++;
	    }
	  //printf("works\n");
	}
    }
  if (numoftiki == 0)
    {
      printf ("Didn't Find tikitag reader...\n");
      exit (-1);
    }
  for (i = 0; i < numoftiki; i++)
    {

      if ((tiki[i] = usb_open (tikidev[i])) == NULL)
	{
	  perror ("usb_open");
	  exit (-1);
	}
       /*      
      if(usb_claim_interface(tiki[i], 0)<0)
       {
	  perror("usb_claim_interface");
	  exit(-1);
       }
       */
      //printf("Open[%d]!\n",i+1);
    }
  for (i = 0; i < numoftiki; i++)
    {
      if ((tiki_setup (tiki[i])) < 0)
	{
	  perror ("tiki_init");
	  exit(-1);
	}
    }
  return (numoftiki);
}

 /*
  **
  **Decode Input from tag to just return UID
  **       Reader seems to be able to read a max of two at a time
  **LOCKED IN
  */
int
tiki_decode (char *inp, int reader)
{
  int numtags = inp[12];
  int idlen1 = inp[19];
  int idlen2 = inp[33];
  int i;

  if (numtags == 0)
    {
      //printf("Reader[%d]:No tags present\n\n", reader+1);
      return (-1);
    }
  readerID = reader;
  if (numtags >= 1)		//One Tag
    {
      //id1[0] = reader + 1;
      for (i = 0; i < 8; i++)	//Load ID
	{
	  id1[i] = inp[i + 18];
	}
      //printf("Reader[%d]:ID = ", reader+1);                //Print ID
      id1_int = 0;
      for (i = 0; i < idlen1 + 1; i++)
	{
	  id1_int += id1[i] << (i * 8);
	}
      //printf("%u", id1_int);
      //printf("\n\n");
    }
  if (numtags == 2)		//Two Tags
    {
      id2[0] = reader + 1;
      for (i = 0; i < 8; i++)	//Load ID2
	{
	  id2[i] = inp[i + 32];
	}
      //printf("Reader[%d]:ID2 = ", reader+1);
      for (i = 0; i < idlen2 + 1; i++)
	{
	  id2_int += id2[i] << (i * 8);
	}
      //printf("%u", id2_int);
      //printf("\n\n");
    }
  return numtags;
}

 /*
  **
  **tiki_config config seektime and looprate
  **
  **LOCKED IN
  */
int
tiki_config (int time, int rate)
{
  seektime = time;

  if (rate < 1000)
    looprate = rate;
  if (rate > 1000)
    looprate = 1000;
  return (0);
}

 /*
  **
  **tiki close - power down reader and close usb communication
  **    
  **LOCKED IN
  */
int
tiki_close (void)
{
  int i = 0;
  for (i = 0; i < numoftiki; i++)
    {
      if (tiki_xchange
	  (tiki[i], power_down, sizeof (power_down), recvbuf,
	   sizeof (recvbuf)) < 0)
	{
	  perror ("xchange");
	}
      //printf("closing[%d]\n\n", i+1);
      if (usb_close (tiki[i]) < 0)
	{
	  perror ("usb_close");
	  return (-1);
	}
    }
  return (0);
}

 /*
  **
  **tiki_seek - searches for time set in tiki_config (time) at the rate specfied in tiki_config (rate)
  **             rate is how often program checks for a tag, once tag is found store in global variable
  **             ID1 is two are found after a poll they are stored in ID1 and ID2
  **
  **LOCKED IN
  */
int
tiki_seek (void)
{				//seektime/(looprate+65ms)=iterations (65ms or 95ms is roughly time to run through one iteration)
  int t = 0;
  int i = 0;
  int recvb;
  int numtags;
  while (t < (seektime / (looprate + 95)))
    {

      for (i = 0; i < numoftiki; i++)
	{
          //printf("haha %d\n", count);
	  poll1[6] = count;
	  if ((recvb =
	       tiki_xchange (tiki[i], poll1, sizeof (poll1), recvbuf,
			     sizeof (recvbuf))) < 0)
	    {
	      perror ("xchange");
	      return (-1);
	    }
	  get1[6] = count;
	  get1[14] = recvbuf[11];
	  if ((recvb =
	       tiki_xchange (tiki[i], get1, sizeof (get1), recvbuf,
			     sizeof (recvbuf))) < 0)
	    {
	      perror ("xchange");
	    }
	  //Add debug mode for Printing
	  numtags = tiki_decode (recvbuf, i);
	  if (numtags > 0)	//check num of tags
		  return numtags;		//return num of tags
	}
      t++;
      usleep (looprate * 1000);	//convert ms to us
    }
  return 0;
}
