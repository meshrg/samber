// rf95_client.cpp
//
// Example program showing how to use RH_RF95 on Raspberry Pi
// Uses the bcm2835 library to access the GPIO pins to drive the RFM95 module
// Requires bcm2835 library to be already installed
// http://www.airspayce.com/mikem/bcm2835/
// Use the Makefile in this directory:
// cd example/raspi/rf95
// make
// sudo ./rf95_client
//
// Contributed by Charles-Henri Hallard based on sample RH_NRF24 by Mike Poublon

#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
//#include <time.h>


#include <wiringSerial.h>
#include <RH_RF69.h>
#include <RH_RF95.h>

// define hardware used change to fit your need
// Uncomment the board you have, if not listed 
// uncommment custom board and set wiring tin custom section

// LoRasPi board 
// see https://github.com/hallard/LoRasPI
//#define BOARD_LORASPI

// iC880A and LinkLab Lora Gateway Shield (if RF module plugged into)
// see https://github.com/ch2i/iC880A-Raspberry-PI
//#define BOARD_IC880A_PLATE

// Raspberri PI Lora Gateway for multiple modules 
// see https://github.com/hallard/RPI-Lora-Gateway
//#define BOARD_PI_LORA_GATEWAY

// Dragino Raspberry PI hat
// see https://github.com/dragino/Lora
#define BOARD_DRAGINO_PIHAT

// Now we include RasPi_Boards.h so this will expose defined 
// constants with CS/IRQ/RESET/on board LED pins definition
#include "../RasPiBoards.h"

// Our RFM95 Configuration 
#define RF_FREQUENCY  868.00
#define RF_GATEWAY_ID 1 
#define RF_NODE_ID    10

// Create an instance of a driver
RH_RF95 rf95(RF_CS_PIN, RF_IRQ_PIN);
//RH_RF95 rf95(RF_CS_PIN);

//Flag for Ctrl-C
volatile sig_atomic_t force_exit = false;

void sig_handler(int sig)
{
  printf("\n%s Break received, exiting!\n", __BASEFILE__);
  force_exit=true;
}


int main (int argc, const char* argv[] )
{
  static unsigned long last_millis;
  static unsigned long led_blink = 0;
  
  signal(SIGINT, sig_handler);
  printf( "%s\n", __BASEFILE__);
  


uint8_t fd;
if ((fd = serialOpen ("/dev/ttyS0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
   // return 1 ;
  }
 
char GPS [700]={} ;
  for (int i=0;i<700;i++)
  {
	  GPS[i]=serialGetchar (fd);
}
   char * GPRM;
   printf ("Los valores del gps son  \" %s \"...\n",GPS);
printf(" **************************");
//printf(" **************************");
printf (" \n");
	
   GPRM=strchr(GPS,'G');
   int pkt=1; 
   int Info=0;
   char GPRMC[70]={};
   char GPVT[37]={};
   char GPTX[32]={};
   char GPGG[73]={};
   char GPGSA[50]={};
   char GPGSV[67]={};
   char GPGLL[44]={};
   uint8_t PRM [70]={};		
   uint8_t data[73]={};
   uint8_t PVT [37]={};
   uint8_t PTX[32]={};
   uint8_t PGG[73]={};
   uint8_t GSA[53]={};
   uint8_t GSV[67]={};
   uint8_t GLL[44]={};


  if (!bcm2835_init()) {
    fprintf( stderr, "%s bcm2835_init() Failed\n\n", __BASEFILE__ );
    return 1;
  }
  
  printf( "RF95 CS=GPIO%d", RF_CS_PIN);

#ifdef RF_LED_PIN
  pinMode(RF_LED_PIN, OUTPUT);
  digitalWrite(RF_LED_PIN, HIGH );
#endif

#ifdef RF_IRQ_PIN
  printf( ", IRQ=GPIO%d", RF_IRQ_PIN );
  // IRQ Pin input/pull down 
  pinMode(RF_IRQ_PIN, INPUT);
  bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
#endif
  
#ifdef RF_RST_PIN
  printf( ", RST=GPIO%d", RF_RST_PIN );
  // Pulse a reset on module
  pinMode(RF_RST_PIN, OUTPUT);
  digitalWrite(RF_RST_PIN, LOW );
  bcm2835_delay(150);
  digitalWrite(RF_RST_PIN, HIGH );
  bcm2835_delay(100);
#endif

#ifdef RF_LED_PIN
  printf( ", LED=GPIO%d", RF_LED_PIN );
  digitalWrite(RF_LED_PIN, LOW );
#endif

  if (!rf95.init()) {
    fprintf( stderr, "\nRF95 module init failed, Please verify wiring/module\n" );
  } else {
    printf( "\nRF95 module seen OK!\r\n");

#ifdef RF_IRQ_PIN
    // Since we may check IRQ line with bcm_2835 Rising edge detection
    // In case radio already have a packet, IRQ is high and will never
    // go to low so never fire again 
    // Except if we clear IRQ flags and discard one if any by checking
    rf95.available();

    // Now we can enable Rising edge detection
    bcm2835_gpio_ren(RF_IRQ_PIN);
#endif

    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
    // you can set transmitter powers from 5 to 23 dBm:
    //rf95.setTxPower(23, false); 
    // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
    // transmitter RFO pins and not the PA_BOOST pins
    // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true. 
    // Failure to do that will result in extremely low transmit powers.
    //rf95.setTxPower(14, true);

    rf95.setTxPower(14, false); 

    // You can optionally require this module to wait until Channel Activity
    // Detection shows no activity on the channel before transmitting by setting
    // the CAD timeout to non-zero:
    //rf95.setCADTimeout(10000);

    // Adjust Frequency
    rf95.setFrequency( RF_FREQUENCY );

    // This is our Node ID
    rf95.setThisAddress(RF_NODE_ID);
    rf95.setHeaderFrom(RF_NODE_ID);
    
    // Where we're sending packet
    rf95.setHeaderTo(RF_GATEWAY_ID);  
  //GPS();
    printf("RF95 node #%d init OK @ %3.2fMHz\n", RF_NODE_ID, RF_FREQUENCY );

    last_millis = millis();
    
    int l=1;
while (l=1)
{  
 
    //Begin the main body of code
    while (!force_exit) {
// int l=1;
//while (l=1)
//{ 
    
      //printf( "millis()=%ld last=%ld diff=%ld\n", millis() , last_millis,  millis() - last_millis );

       //Send every 5 seconds
      if ( millis() - last_millis > 5000 ) {
        //last_millis = millis();

#ifdef RF_LED_PIN
        led_blink = millis();
        digitalWrite(RF_LED_PIN, HIGH);
#endif
//

//while (GPRM!=NULL)
  //{
	 // memset(&GPVT,' ',sizeof(GPVT));
	 
    int i=GPRM-GPS;
    //printf(" el v i es = %d \n",i);
    GPRM=strchr(GPRM+1,'G');
    
   if (GPS[i+1] == 'P' )
    {       
      if (GPS[i+2] == 'R' && GPS[i+3] == 'M' ) {
	    
        int j;
        int k;
        for (k=i-1,j=0;(k<i+69);k++,j++)
	    { 
		GPRMC[j]=GPS[k];
		uint8_t *mc = (uint8_t *)GPRMC;
		PRM[j] = mc[j];
	//	PRM[j] = GPS[k];
		}
		
		printf (" \n");
	  
	   // printf (" en uint %s \n",PRM); 
	   // printf("se encontro PRM  \n");
	   
	    } 
	      else if (GPS[i+2] == 'V' && GPS[i+3] == 'T' )
	       { 
		
         int qq;
         int w;
         for (w=i-1,qq=0;(w<i+37);w++,qq++)
	     { 
		GPVT[qq]=GPS[w];
		 uint8_t *vt = (uint8_t *)GPVT;
		PVT[qq] = vt[qq];
		//PVT[q] = GPS[w];
	//	printf("%c",PVT);
		 }
		printf (" \n");
		printf (" \n");  
	   // printf ("%s \n",GPVT); 
		// printf("se encontro VT  \n"); 
		   }
		else if (GPS[i+2] == 'T' && GPS[i+3] == 'X' )
	     { 
			 memset(&GPVT,' ', sizeof(GPVT));
			 
           int e;
           int r;
           for (r=i-1,e=0;(r<i+32);r++,e++)
	       { 
		 GPTX[e]=GPS[r];
		uint8_t *tx = (uint8_t *)GPTX;
		PTX[e] = tx[e];
		   // PTX[e] = GPS[r];
		 //   printf("%c",PTX);
		   
		   }
			printf (" \n"); 

	       //printf ("%s \n",GPTX); 
           //printf("se encontro TX  \n"); 	
	     }
     
	if (GPS[i+2] == 'G') 
		{
			if (GPS[i+2] == 'G' && GPS[i+3] == 'G' )
		   { 
             int t;
             int y;
             for (y=i-1,t=0;(y<i+77);y++,t++)
	         { 
		     GPGG[t]=GPS[y];
		    uint8_t *gg = (uint8_t *)GPGG;
		   PGG[t] = gg[t];
		 //   printf("%c",PGG);
		   
		   }
printf (" \n"); 
	       // printf ("%s \n",GPGG); 
		 //	printf("se encontro GG  \n"); 
			}
		else if (GPS[i+3] == 'S' && GPS[i+4] == 'A' ) {
			
             int u;
             int o;
             for (o=i-1,u=0;(o<i+53);o++,u++)
	         { 
				GPGSA[u]=GPS[o];
		     uint8_t *sa = (uint8_t *)GPGSA;
		     GSA[u] = sa[u];
		     //GSA[u] = GPS[o];
		   //  printf("%c",GSA);
		   
		     }
			printf (" \n"); 
	        // printf ("%s \n",GPGSA); 
			// printf("se encontro GSA  \n"); 
			}
		else if (GPS[i+3] == 'S'  && GPS[i+4] == 'V' ) {
			
             int p;
             int s;
             for (s=i-1,p=0;(s<i+67);s++,p++)
	         { 
		  GPGSV[p]=GPS[s];
		    uint8_t *sv = (uint8_t *)GPGSV;
		      GSV[p] = sv[p];
		     // GSV[p] = GPS[s];
		      
		  //  printf("%c",GSV);
		     }
		 //	 printf (" \n"); 
	     //   printf ("%s \n",GPGSV); 
		//	printf("se encontro GSV  \n"); 
			}
         else if (GPS[i+3] == 'L' && GPS[i+4] == 'L' ) {
			// memset(&GPGG,' ', sizeof(GPGG));
			// memset(&GPGLL,' ', sizeof(GPGLL));
		//	memset(&GPTX,' ', sizeof(GPTX));
			
             int d;
             int f;
             for (f=i-1,d=0;(f<i+48);f++,d++)
	         { 
		      GPGLL[d]=GPS[f];
		      uint8_t *ll = (uint8_t *)GPGLL;
		      GLL[d] = ll[d];
		    // GLL[d] = GPS[f];
		    // printf("%c",GLL);
		    //  return GLL;
		     }
		    
			printf (" \n"); 
	        // printf ("%s \n",GPGLL); 
		//	printf("se encontro GGL  \n"); 
			}
	    }
 //printf ("%s \n",data);
  }
  //data[70]={};
  switch( pkt )   
{  
    case 1:
   if (PRM[0] != 0)
    { 
		
		Info=1;
		printf (" \n");
		printf (" \n");
    printf(" Case Num 1");
   // printf (" valor de pkt ( debe ser 1 aki) %d  \n",pkt); 
    printf (" valor de  PRM %s  \n",PRM);
      
			char * grm;
            grm=strchr(GPRMC,'*');
		    int gr = grm-GPRMC;
		    printf(" el *  es = %d \n ",gr);
			grm=strchr(grm+1,'*'); 
		    int gpr=gr+2; 
	 //  
      
        for (int z=0;(z<=gpr);z++)
	      { 
		data[z] = PRM[z];
		  }
        printf (" valor de  data2 %s  \n",data);
    
 }
        pkt++; 
         printf (" valor de pkt %d  \n",pkt); 
        
        break;
          
    case 2 : 
    if (PVT[0] == '$')
    {
		Info=1;
		printf (" \n");
		printf (" \n");
        printf (" \n");
        printf(" Case Num 2");
         //data[31]
		// memset(&data[0], 0, sizeof(data));
		// printf (" valor de PVT %s  \n",GPVT);
		 //Indicador
            char * gvt;
            gvt=strchr(GPVT,'*');
		    int gav = gvt-GPVT;
		    printf(" el *  es = %d \n ",gav);
			gvt=strchr(gvt+1,'*');
		 	int gpv=gav+2; 
	 //
		  
    for (int qq=0;(qq<=gpv);qq++)
	     {  
    data[qq]=PVT[qq] ;
       }
          
       printf (" valor de dat %s  \n",data);
   } 
        pkt++;  
         printf (" valor de pkt %d  \n",pkt);
       
        break; 
         
    case 3 :  
    if (PTX[0] == '$')
    {
		Info=1;
		data[31]; 
		printf (" \n");
		  printf (" \n");
       
     printf(" Case Num 3");
      printf (" valor de PTX %s  \n",PTX);
      //Indicador
            char * Gx;
            Gx=strchr(GPTX,'*');
		     int gax = Gx-GPTX;
		 printf(" el *  es = %d \n ",gax);
			Gx=strchr(Gx+1,'*');
		 	 
		int GTx=gax+2; 
	 //	 
     for (int f=0;(f<=GTx);f++)
	       { 
		   
		    data[f]= PTX[f];
     }
        
     printf (" valor de dat %s  \n",data); 
 }
        pkt++; 
        printf (" valor de pkt %d  \n",pkt); 
      
        break; 
     case 4 : 
    if (PGG[0] =='$')
    {
		Info=1;
		  printf (" \n");
         printf (" \n");
     printf(" Case Num 4");
		data[50]; 
		 printf (" valor de PGG %s  \n",GPGG);
	            
	            char * Gg;
            Gg=strchr(GPGG,'*');
		     int ga = Gg-GPGG;
		 printf(" el *  es = %d \n ",ga);
			Gg=strchr(Gg+1,'*');
		 	 
		int idg=ga+2; 	 
		 
    for (int g=0;(g<=idg);g++)
	         { 
		   data[g]=PGG[g];
	           }
		   printf (" valor de dat %s  \n",data); 
	   }
        pkt++; 
      printf (" valor de pkt %d  \n",pkt);    
        break;
        case 5 : 
         if (GSA[0] =='$')
    {  
		Info=1; 
		  printf (" \n");
         printf (" \n");   
     printf(" Case Num 5");
	    data[31];
	   
		 printf (" valor de GSA %s  \n",GSA);
	 char * Gss;
            Gss=strchr(GPGSA,'*');
		     int gsaa = Gss-GPGSA;
		 printf(" el *  es = %d \n ",gsaa);
			Gss=strchr(Gss,'*');
		 	 
		int is=gsaa+2;	 
		 
    for (int hh=0;(hh<=is);hh++)
	         { 
		     data[hh]=GSA[hh]; 
		     } 
		        Info=1;
		   printf (" valor de dat %s  \n",data);   
	   }
        pkt++;
         printf (" valor de pkt %d  \n",pkt);  
        break;
    
         case 6:  
     if (GSV[0] =='$')
    {  
		
		Info=1;  
		  printf (" \n");
         printf (" \n");      
     printf(" Case Num 6");

		//data[66];
 //     memset(&data[0], 0, sizeof(data));	
		 	 printf (" valor de GSV %s  \n",GSV);
		 	
		  char * Gs;
            Gs=strchr(GPGSV,'*');
		     int gv = Gs-GPGSV;
		 printf(" el *  es = %d \n ",gv);
			Gs=strchr(Gs+1,'*');
		 	 
		int ind=gv+2; 	 
     for (int j=0;(j<=ind);j++)
	         { 
		      data[j]=GSV[j]; 
		     }
		        Info=1;
		     	 printf (" valor de dat %s  \n",data);
			 }
        pkt++;  
         printf (" valor de pkt %d  \n",pkt);
        break;
        
       case 7:
        if (GLL[0] != 0)
    {
		 Info=1;
		 printf (" \n");
         printf (" \n");
         printf(" Case Num 7");
		 data[50];
		 printf (" valor de GLL %s  \n",GLL);
   	
		  char * gll;
          gll=strchr(GPGLL,'*');
		  int gl = gll-GPGLL;
		  printf(" el *  es = %d \n ",gl);
		  gll=strchr(gll+1,'*');
		 	 
		  int lg=gl+2; 
   
   for (int k=0;(k<=lg);k++)
	         {
		     data[k]=GLL[k];
		 } 
 printf (" valor de dat %s  \n",data); 
	} 
//memset(&data[0], 0, sizeof(data));	
        pkt=1; 
         printf (" valor de pkt se supone que es 1 pkt=1 %d  \n",pkt); 
        break;    
         
}
printf (" valor de dat %d  \n",pkt);  
 //printf (" \n");
 if (data[0]=='$')
 {
   if (Info==1)
      {
  uint8_t len = sizeof(data);
  printf("Enviando %02d bytes to node #%d => ", len, RF_GATEWAY_ID );
        printbuffer(data, len);
        printf("\n" );
      rf95.send(data, len);
       rf95.waitPacketSent();
        //strcpy(data,"");
       printf("estoy esperando");
       usleep(2000000);
       memset(&data,' ', sizeof(data));
       
    }
		memset(&GPGSV,' ', sizeof(GPGSV));
       // memset(&GPRMC,' ', sizeof(GPRMC));
    //   memset(&GPGG,' ', sizeof(GPGG));
     //  memset(&GPGLL,' ', sizeof(GPGLL));
        
 }
//}
break;
}
}
      // printf ("****%s",data);
  //uint8_t data[] = "Hola Mundo";
 

       }
       
	
	//
        // Send a message to rf95_server
        //GPS();
        //printf(" %s",RMC);
        
/*
        // Now wait for a reply
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        if (rf95.waitAvailableTimeout(1000)) { 
          // Should be a reply message for us now   
          if (rf95.recv(buf, &len)) {
            printf("got reply: ");
            printbuffer(buf,len);
            printf("\nRSSI: %d\n", rf95.lastRssi());
          } else {
            printf("recv failed");
          }
        } else {
          printf("No reply, is rf95_server running?\n");
        }
*/
        
     // }

#ifdef RF_LED_PIN
      // Led blink timer expiration ?
      if (led_blink && millis()-led_blink>200) {
        led_blink = 0;
        digitalWrite(RF_LED_PIN, LOW);
      }
#endif
      
      // Let OS doing other tasks
      // Since we do nothing until each 5 sec
      bcm2835_delay(100);
    }
//}
  //}

#ifdef RF_LED_PIN
  digitalWrite(RF_LED_PIN, LOW );
#endif
  printf( "\n%s Ending\n", __BASEFILE__ );
  bcm2835_close();
  return 0;
}
