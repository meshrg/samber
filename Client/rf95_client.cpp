// rf95_client.cpp
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
#define RF_FREQUENCY  915.00
#define RF_GATEWAY_ID 1 
#define RF_NODE_ID    10

// Create an instance of a driver
RH_RF95 rf95(RF_CS_PIN, RF_IRQ_PIN);
//RH_RF95 rf95(RF_CS_PIN);

//Flag for Ctrl-C
volatile sig_atomic_t force_exit = false;
//void lectura_GPS ();
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
 
 int alto=10; 
int bandera=0;
int siz;		
int contador=0; 
char info[74]={};		
uint8_t data[74]={};

 
	  
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
 int l=1;
    last_millis = millis();
   while (!force_exit) {
	       
   
while (l=1)
{  

    //Begin the main body of code
 
		
		uint8_t fd;
if ((fd = serialOpen ("/dev/ttyS0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
   // return 1 ;
  }

 //uint8_t fd  
 char GPS [700]={} ;
 for (int j=0;j<700;j++)
  {
	  GPS[j]=serialGetchar (fd);
}
  //lectura_GPS ();
   char * GPRM;
	   GPRM=strchr(GPS,'$');
	 serialClose(fd);

      //printf( "millis()=%ld last=%ld diff=%ld\n", millis() , last_millis,  millis() - last_millis );

       //Send every 5 seconds
      if ( millis() - last_millis > 5000 ) {
        //last_millis = millis();

#ifdef RF_LED_PIN
        led_blink = millis();
        digitalWrite(RF_LED_PIN, HIGH);
#endif
	  
int i=GPRM-GPS;	
char *GPR;
const char * ABR[7] = {"$GPRMC","$GPVTG","$GPTXT","$GPGGA","$GPGSA","$GPGSV","$GPGLL"};  



 GPR = strstr(GPS,ABR[bandera]);
   if (!strncmp( GPR, "$GPRMC", 5 ))
    {
      siz=71;
    }
    else if (!strncmp( GPR, "$GPVTG", 5 ))
     {
		siz=38;
	}
	else if (!strncmp( GPR, "$GPTXT", 5 ))
     {
		siz=33;
	}
	else if (!strncmp( GPR, "$GPGGA", 5 ))
     {
		siz=73;
	}
	else if (!strncmp( GPR, "$GPGSA", 6 ))
     {
		siz=57;
	}
	else if (!strncmp( GPR, "$GPGSV", 6 ))
     {
		siz=68;
	}
	else if (!strncmp( GPR, "$GPGLL", 5 ))
     {
		siz=49;
	}
	
	 strncpy (info, GPR,siz);     
						
	 
				     char * Gs;
					Gs=strchr(info,'*');
					int gv = Gs-info;
					int ind= gv+3; 
				strncpy ((char *)data, info, ind);
				
		if (bandera==6){
			bandera=0;
		}
		else {
			bandera++;
		}
		if (contador==alto) {
return EXIT_SUCCESS;
 }


 if (data[0]=='$')
 {

  uint8_t len = sizeof(data);
  printf("Enviando [%d] al node #%d => ",contador,RF_GATEWAY_ID );
	   contador= contador+1;
       printbuffer(data, len);
         printf (" \n");printf (" \n");printf (" \n");
        //printf (" se envio %s  \n",data);
        printf("\n" );
       rf95.send(data, len);
       rf95.waitPacketSent();
       //printf("estoy esperando");
       printf (" \n");printf (" \n");
       usleep(200000);//0
       memset(&data,' ', sizeof(data));     
       memset(&info,' ', sizeof(info));
 }
 
break;
     }
   }
       }
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


 //void lectura_GPS (){
 //uint8_t fd; 
 //char GPS [1000]={} ;
 //for (int j=0;j<1000;j++)
  //{
	  //GPS[j]=serialGetchar (fd);
//}

//}
