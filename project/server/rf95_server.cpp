// rf95_server.cpp
// Contributed by Charles-Henri Hallard based on sample RH_NRF24 by Mike Poublon
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <mysql/mysql.h>
#include <errno.h>
#include <wiringSerial.h>
#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <RH_RF69.h>
#include <RH_RF95.h>
#include <math.h>
#include <iostream>

using namespace std;

#define PI 3.14159265358979323846

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
#define RF_FREQUENCY  915.00
#define RF_NODE_ID    1

//MYSQL
#define HOST "10.1.135.91"//"192.168.0.8"
#define USER "samber" 
#define PASS "cidte"
#define DB "GPS"
// Variables 
char Node [3];
char Cliente[74];
char Server[74];
char GPS[700]={} ;

char tabla[]= ""; 
char buffer[6]={};
char Infor[74];
char Cort[74];
int band;
int bandera=0;
int siz;
int Dat;
char l1[10],lo1[10],l2[10],lo2[10];
double lat1,lon1,lat2,lon2;
char A[10],A2[10];
double alt1,alt2;
double Altura;




// Create an instance of a driver
RH_RF95 rf95(RF_CS_PIN, RF_IRQ_PIN);
//RH_RF95 rf95(RF_CS_PIN);

// funciones
//void muestra(MYSQL* con,char* consulta0,MYSQL_ROW row,MYSQL_RES *res);
void agrega (MYSQL* con, char *tabla, char* Node,char*Cliente,char*Server);
double distancia (float lat1,float lon1 , float lat2, float lon2);
double busca (char Cliente,char Server); 

double Latitud (double la1);
double Longitud (double lo1);

//Flag for Ctrl-C
volatile sig_atomic_t force_exit = false;

void sig_handler(int sig)
{
  printf("\n%s Break received, exiting!\n", __BASEFILE__);
  force_exit=true;
}


//Main Function
int main (int argc, const char* argv[] )
{
	//MYSQL
MYSQL *con;
MYSQL_ROW row;
MYSQL_RES *res;
char consulta0 [1024];
int contador=0;

  unsigned long led_blink = 0;
  
  signal(SIGINT, sig_handler);
  printf( "%s\n", __BASEFILE__);

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
  // Now we can enable Rising edge detection
  bcm2835_gpio_ren(RF_IRQ_PIN);
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
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
    // you can set transmitter powers from 5 to 23 dBm:
    //  driver.setTxPower(23, false);
    // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
    // transmitter RFO pins and not the PA_BOOST pins
    // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true. 
    // Failure to do that will result in extremely low transmit powers.
    // rf95.setTxPower(14, true);


    // RF95 Modules don't have RFO pin connected, so just use PA_BOOST
    // check your country max power useable, in EU it's +14dB
    rf95.setTxPower(14, false);

    // You can optionally require this module to wait until Channel Activity
    // Detection shows no activity on the channel before transmitting by setting
    // the CAD timeout to non-zero:
    //rf95.setCADTimeout(10000);

    // Adjust Frequency
    rf95.setFrequency(RF_FREQUENCY);
    
    // If we need to send something
    rf95.setThisAddress(RF_NODE_ID);
    rf95.setHeaderFrom(RF_NODE_ID);
    
    // Be sure to grab all node packet 
    // we're sniffing to display, it's a demo
    rf95.setPromiscuous(true);

 // We're ready to listen for incoming message
    rf95.setModeRx();
    
   con=mysql_init(NULL);	
    if(!mysql_real_connect(con, HOST, USER, PASS, DB, 3306, NULL,0))
				{	
	fprintf(stderr, "%s\n", mysql_error(con));
	 exit(1);
				} 

    printf( " OK NodeID=%d @ %3.2fMHz\n", RF_NODE_ID, RF_FREQUENCY );
    printf( "Listening packet...\n" );

//PUERTO  SERIAL
while (!force_exit) {


 
uint8_t fd;
if ((fd = serialOpen ("/dev/ttyS0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
   // return 1 ;
  }


 for (int j=0;j<700;j++)
  {
	  GPS[j]=serialGetchar (fd);
}

     serialClose(fd);
     //printf("*GPS**** %s **GPS******",GPS);

//break;




//

    //Begin the main body of code
    	
   //while (!force_exit) {
  //int dos=1;
 
 

 
 
 
#ifdef RF_IRQ_PIN
      // We have a IRQ pin ,pool it instead reading
      // Modules IRQ registers from SPI in each loop
      
      // Rising edge fired ?
      if (bcm2835_gpio_eds(RF_IRQ_PIN)) {
        // Now clear the eds flag by setting it to 1
        bcm2835_gpio_set_eds(RF_IRQ_PIN);
        //printf("Packet Received, Rising event detect for pin GPIO%d\n", RF_IRQ_PIN);
#endif
 //while (dos<10)
  //{
        if (rf95.available()) { 
#ifdef RF_LED_PIN
          led_blink = millis();
          digitalWrite(RF_LED_PIN, HIGH);
#endif


          // Should be a message for us now
          uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
          uint8_t len  = sizeof(buf);
          uint8_t from = rf95.headerFrom();
          uint8_t to   = rf95.headerTo();
          uint8_t id   = rf95.headerId();
          uint8_t flags= rf95.headerFlags();;
          int8_t rssi  = rf95.lastRssi();
   
     
          if (rf95.recv(buf, &len)) {
            printf("Packet [%02d]  #%d => #%d %ddB: ",contador,  from, to, rssi);
            contador = contador+1;
          // printbuffer(buf, len);
            
//memset(&buffer,' ', sizeof(buffer));
char Node[3];
int Nod = from ;
snprintf(Node, 3, "%d", Nod);

char *GPR;
char *Find;
char *p;
strncpy (buffer, (char*)buf,6);
//printf("buffer ***%s****\n",buffer);



GPR = strstr((char*)buf,buffer);
//printf("GPR***%s****\n",GPR);
Find = strstr(GPS,buffer);
//printf("FIND ***%s****\n",Find);

if (GPR!=NULL) {
	
   if (!strncmp( GPR, "$GPRMC", 6 ))
    {
				strcpy(Cliente,(char*)buf);	
				//strcpy(Server,Infor);
				siz=71;
				printf( "%s",buf);
				strcpy(tabla,"gprmc");
				Dat=1;


//printf (" %s %s",l1,l2);
    
    }
    else if (!strncmp( GPR, "$GPVTG", 6 ))
    
     {
					strcpy(Cliente,(char*)buf);
					//strcpy(Server,Infor);	
					siz=39;
					printf( "%s",buf);
					strcpy(tabla,"gpvtg");
	}
	else if (!strncmp( GPR, "$GPTXT", 6 ))
     {
					   strcpy(Cliente,(char*)buf);
					  //strcpy(Server,Infor);
					   siz=33;
					   printf( "%s",buf);
					   strcpy(tabla,"gptxt");
	}
	else if (!strncmp( GPR, "$GPGGA", 6 ))
     {
						strcpy(Cliente,(char*)buf);
						strncpy(A,Cliente+52,6);
						Dat=2;
						siz=74;
						printf( "%s \n",buf);
						//printf("Altura de %s",A);
						strcpy(tabla,"gpgga");
	}
	else if (!strncmp( GPR, "$GPGSA", 6 ))
     {
						
	          			  strcpy(Cliente,(char*)buf);
					  	  //strcpy(Server,Infor);
					  	  siz=62;
					  	  printf( "%s",buf);
						  strcpy(tabla,"gpgsa");
																
	}
	else if (!strncmp( GPR, "$GPGSV", 6 ))
     {

							strcpy(Cliente,(char*)buf);
						   	//strcpy(Server,Infor);
						   	 siz=69;
						   	 printf( "%s",buf);
						    strcpy(tabla,"gpgsv");
	}
	else if (!strncmp( GPR, "$GPGLL", 6 ))
     {
						strcpy(Cliente,(char*)buf);
						//strcpy(Server,Infor);
						siz=50;
						printf( "%s",buf);
						strcpy(tabla,"gpgll");
	}
}	
//printf("llega aqui 0");				
  	
	if ( (Find!=NULL) && (Find[0]=='$'))
		{						
						strncpy (Cort,Find,siz);	
						//printf(" *** Cort **** %s \n",Cort);
						char * Gs;
						Gs=strchr(Cort,'*');
						if (Gs!=NULL) {
						int gv = Gs-Cort;
						int ind= gv+3; 
						
						strncpy (Server,Cort,ind);
					 printf("server Ok" );
					}


 			//printf("llega aqui 1");				
  
				if (Dat==1)
				{
				strncpy(l1,Cliente+20,9);
				lat1=Latitud(atof(l1));
				strncpy(l2,Cliente+32,10);
				lon1=Longitud(atof(l2));
				strncpy(lo1,Server+20,9);
				lat2=Latitud(atof(lo1));
				strncpy(lo2,Server+32,10);
				lon2=Longitud(atof(lo2));
				//printf (" %s %s",lo1,lo2);	

				double dis=distancia(lat1,lon1,lat2,lon2);  
				printf("la dista en metros es  %f ",dis);
				Dat=0;
				}

				else if (Dat==2)
				{
				strncpy(A2,Server+52,6);
				//printf("Altura S %s",A2);
				alt1=(atof(A));
				alt2=(atof(A2));

				Altura=abs(alt1-alt2);
				printf("diferencia de Altura  %f",Altura);
				Dat=0;	
				}	

//printf("llega aqui 2");				

} // Null 
agrega(con,tabla,Node,Cliente,Server); 
  
				memset(&Find,' ', sizeof(Find));
				memset(&Cort,' ', sizeof(Cort));
				memset(&Server,' ', sizeof(Server));
				memset(&Infor,' ', sizeof(Infor));  
				memset(&buffer,' ', sizeof(buffer));
   
  if (bandera==6){
			bandera=0;
		}
		else {
			bandera++;
		}  
 
 }       
} 
           else {
            Serial.print("receive failed");
          }
          printf("\n");
        }
        
#ifdef RF_IRQ_PIN
      //}
#endif
      
#ifdef RF_LED_PIN
      // Led blink timer expiration ?
      if (led_blink && millis()-led_blink>200) {
        led_blink = 0;
        digitalWrite(RF_LED_PIN, LOW);
      }
#endif
      // Let OS doing other tasks
      // For timed critical appliation you can reduce or delete
      // this delay, but this will charge CPU usage, take care and monitor
      bcm2835_delay(5);
    }
  }
mysql_close(con);
fprintf(stdout,"\n .-> Desconectado a base de datos: %s\n",DB);

#ifdef RF_LED_PIN
  digitalWrite(RF_LED_PIN, LOW );
#endif
  printf( "\n%s Ending\n", __BASEFILE__ );
  bcm2835_close();
 
  return 0;
}


void agrega(MYSQL* con,char*tabla, char* Node,char*Cliente,char*Server)
{
char consulta[1024];
sprintf(consulta,"INSERT INTO %s VALUES ('%s','%s','%s')",tabla,Node,Cliente,Server);
if(mysql_query(con,consulta)==0) fprintf(stdout,"\n Datos insertados con exito\n");
}

double distancia (float lat1,float lon1 , float lat2, float lon2)
{
int   R = 6371000; // km
double dLat = ((lat2-lat1)*(M_PI/180) );
double dLon = ((lon2-lon1))*(M_PI/180);;
double a = (sin(dLat/2) * sin(dLat/2) +cos(lat1) *cos(lat2) *sin(dLon/2) * sin(dLon/2));        
double  c = 2 * atan2(sqrt(a),sqrt(1-a));
double   d = (R * c);
//printf("la distancia %f metros",d);	


//double dist=(acos((sin((lat1))*sin((lat2))) + (cos((lat1))*cos((lat2))*cos((lon1-lon2)))));
//double d = (dist*111.13384)*1000; 
//printf("la dist en mts es  %f  ",d);

return d;
}


//double distancia (float lat1,float lon1 , float lat2, float lon2)
//{

 //double lats =(lat1 - lat2);
 //double lngs =(lon1 - lon2);
 
            ////Paso a metros
            //double latm = lats * 60 * 1852;
            //double lngm = (lngs * cos(lat1 * PI / 180)) * 60 * 1852;
            //double d = sqrt(pow(latm, 2) + pow(lngm, 2))/100;
            //printf( " dist en %f metros",d);

	
//return d;
//}



double Latitud (double la1)
{
double lat1=(la1/100);
float Latitud1;
float conv;
conv= ((lat1)-floor(lat1));
Latitud1=(5*conv/3)+floor(lat1);
return Latitud1 ;
}



double Longitud (double lo1)
{
double lon1=lo1/100;
float Longitud1;
float conv;
conv= ((lon1)-floor(lon1));
Longitud1=(5*conv/3)+floor(lon1);

return Longitud1 ;
}



