Gateway IP para red LoRa con base de datos SQL
===========================================================

Envio y recepcion de Gps apartir del puerto UART De la shield LoRa Dragino HAT/GPS,Despues de la recepcion de la informacion, se almacena en una base de datos Mysql de manera de respaldo de información

=================================

**Compatible Con** 

 [Dragino Lora GPS HAT][13]

<img src="http://wiki.dragino.com/images/d/d6/Lora_GPS_HAT.png" height="25%" width="25%" alt="Raspberry PI Lora Gateway/Node">   



### INSTRUCCIONES GENERALES 
================================

En primer punto es diferente la instalacion para el server como para el client, sin embargo exiten algunos pasos iniciales que se deben realizar en las dos Raspberrys


Conetando la raspberry a una pantalla por medio del HDMI, lo primero que se debe realizar, es escribir la linea siguiente en la linea de comandos 

recordar
user: pi
contraseña:rasberry 


```shell
$ sudo raspi-config
```

seleccionar la opcion 5 
 - Interfacing Options
  - se debe Habilitar lo siguiente: 
```shell
            SSH                 
```


Git
```shell
   $ sudo apt install git                  
```


automake 
```shell
   $ sudo apt install automake                  
```


Clonar el repositorio
```shell
git clone https://github.com/cidte/samber.git
```

**Libreria a Instalar***

**bcm2835**

Se debe instalar la libreria bcm2835 la cual se utiliza para acceder a los pines GPIO para controlar el módulo RFM95
   1. Antes de Descargar la libreria puedes ver la pagina de la libreria en http://www.airspayce.com/mikem/bcm
   2. Descargar la libreria en el siguiente enlace, con el siguiente comando 
```shell
 wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz
```
   3. El paso siguiente es ubicar la carpeta de descargas y desde la terminal escribir
    
```shell
   tar zxvf bcm2835-1.xx.tar.gz
    cd bcm2835-1.xx
    ./configure
    make
   sudo make check
   sudo make install
```

 **Wiringpi**

```shell
 $ sudo apt install wiringpi
```


**Base de datos**
escribir en la linea de comandos 
```shell
$ sudo apt-get install mysql-server
```
Se debe instalar la libreria de mariaDB 

```shell
sudo apt-get install libmariadbclient-dev
```

***Nota***

Las siguientes instrucciones son del Cliente, sin embargo tambien se utilizan en el Server.


## Cliente 
=========================================================================================================

Una Ves habilitado SPI, VNC y SSH. Se prosigue a Realizar lo siguiente, escribir en la linea de comandos:

```shell
 $ sudo nano /boot/config.txt
```
Agregar las siguientes lineas al final 

```shell
dtparam=spi=on
core_freq=250
enable_uart=1
force_turbo=1
```
Para guardar cambios ctrl x

Enseguida escribir:

```shell
$ sudo nano /boot/cmdline.txt
```

Borrar el contenido existente y agregar:

```shell
dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline fsck.repair=yes rootwait
```
Para guardar cambios ctrl x, por ultimo  presionar (y)

para deshabilitar el bluetooth incorporado

```shell
$ sudo systemctl disable hciuart
```

Ahora edite /lib/systemd/system/hciuart.server y reemplace ttyAMA0 con ttyS0.

```shell
$ sudo nano /lib/systemd/system/hciuart.service
```

Remplazar 

```shell 
"After=dev-serial1.device" 
```
por 
```Shell 
"After=dev-ttyS0.device"
```

Para finalizar se debe introducir lo siguiente:
```shell
sudo apt-get update
sudo apt ugrade
sudo reboot
```
Para Vertificar el correcto funcionamiento 
 ```shell
$ sudo cat /dev/ttyS0
```

Para mayor información [Aqui][1]





**CREAR BASE DE DATOS  DE MYSQL**

Dado que durante la instalación de MySQL se crea un root sin contraseña, es necesario
eliminar la cuenta creada por defecto y crear una con contraseña.
Para ello se ingresa a MySQL con privilegios de superusuario mediante el usuario root.

```shell
$ sudo mysql -u root
```

Una vez conectado al gestor de base de datos se procede a eliminar la cuenta de root de
MySQL.

```shell
MariaDB> DROP USER 'root'@'localhost';
```

El siguiente comando crea una nueva cuenta de root con acceso desde localhost y con
contraseña.

```shell
MariaDB> CREATE USER 'samber'@'localhost' IDENTIFIED BY 'cidte';
```

El comando de GRANT ALL PRIVILEGES otorga privilegios de crear, editar y eliminar tablas
de la base de datos al usuario que se le indique.

```shell
MariaDB> GRANT ALL PRIVILEGES ON *.* TO 'samber'@'localhost';
```

Para actualizar los privilegios y efectuar los cambios en el usuario se utiliza el comando
FLUSH PRIVILEGES.

```shell
MariaDB> FLUSH PRIVILEGES;
```

Una vez terminadas las modificaciones en el usuario se termina la conexión con el gestor de
base de datos.

MariaDB> quit

**Entrar a una base de datos mysql** 

Con el comando siguientes es para acceder a la base de datos
```shell
 mysql -u samber -p
```
nos pedira que ingresar la contraseña.


– Primero es necesario ver las bases de datos actuales, con el comando ```shell SHOW DATABASES ``` , y
se debe crear  una nueva base de datos llamada “GPS”.

```shell
>CREATE DATABASE GPS;
```
Ahora si vemos las bases de datos nuevamente ( SHOW DATABASES), veremos que
nuestra base de datos “GPS” esta creada.

– Ahora se debe seleccionar la base de datos para poder trabajar con ella:

```shell
 >USE GPS;
```

– Una vez seleccionada la base de datos a utilizar se podra realizar consultas a ella.
– Ahora es necesario crear  una tabla que sera con la que se  trabajara :

```shell
>CREATE TABLE nombre de tabla ( nombre de primer columna INT, Nombre de segunda columna VARCHAR (70));
```

se puede observar que despues del nombre esta la palabra INT este corresponde al tipo de datos .. en este caso es de tipo entero 

En la segunda columna es de tipo VARCHAR por ser cadena y debe estar entreparentesis el tamaño ()
un ejemplo es:

```shell
>CREATE TABLE gprmc ( Node INT, GPRMC VARCHAR (71));
```

se ha  creado la tabla “gprmc” con dos campos, el campo “GPRMC” que es un varchar
(string) de longitud 70, y un campo “Node” que es un numero de tipo entero.
– para una consulta  a la tabla para obtener los registros existentes, como aun no añade
ningún registro debería decir que esta vacía.

```shell
>SELECT * FROM gprmc;
```
Para crear otra tabla se hace el mismo procedimiento Y asi sucesivamente las demas que se  necesiten.

```shell
CREATE TABLE gprmc ( Node INT, GPRMC VARCHAR (71));
CREATE TABLE gpvtg ( Node INT, GPVTG VARCHAR (38));
CREATE TABLE gptxt ( Node INT, GPTXT VARCHAR (33));
CREATE TABLE gpgga ( Node INT, GPGGA VARCHAR (74));
CREATE TABLE gpgsa ( Node INT, GPGSA VARCHAR (62));
CREATE TABLE gpgsv ( Node INT, GPGSV VARCHAR (69));
CREATE TABLE gpgll ( Node INT, GPGLL VARCHAR (50));
```

para salir solo con CTRL C





##  Server
============================================================================================================



**ACCESO REMOTO A MYSQL**

Por defecto, el servidor MySQL limita los accesos a la dirección local denegando el servicio a
todas las peticiones de conexiones entrantes, adicionalmente, cada usuario se puede limitar a un segmento de red o una dirección IP en específico.

Para habilitar las peticiones remotas se debe de modificar el archivo /et c/m y sql /mar i ad b.con f .d /50−
ser ver.cn f .


```shell
$ sudo nano /etc/mysql/mariadb.conf.d/50-server.cnf
```


El parámetro bind−address = 127,0,0,1 delimita las direcciones IP donde se pueden realizar
las conexiones entrantes, para permitir la conexión desde cualquier dirección IP este se cambia
por 0,0,0,0.

```shell
bind-address = 0.0.0.0
```

Para que los cambios surtan efecto se debe de reiniciar el servicio de MySQL.

```shell
$ sudo systemctl restart mysql.service
$ sudo systemctl restart mariadb.service
```

Para verificar que los cambios funcionaron se consulta si el puerto 3306 de MySQL está
abierto.

```shell
$ sudo netstat -anp | grep 3306
```

Mostrando como resultado que el puerto de MySQL está en escucha

```shell
pi@raspserver:~ $ sudo netstat -anp | grep 3306
tcp 0 0 0.0.0.0:3306 0.0.0.0:* LISTEN 529/mysqld
```


Dado que durante la instalación de MySQL se crea un root sin contraseña, es necesario
eliminar la cuenta creada por defecto y crear una con contraseña.
Para ello se ingresa a MySQL con privilegios de superusuario mediante el usuario root.

```shell
$ sudo mysql -u root
```

Una vez conectado al gestor de base de datos se procede a eliminar la cuenta de root de
MySQL.

```shell
MariaDB> DROP USER 'root'@'localhost';
```

Dentro de MySQL con el usuario root se debe de crear un usuario que tenga permiso de
conectarse desde cualquier IP. 

```shell
MariaDB>CREATE USER 'samber'@'%' IDENTIFIED BY 'cidte';

```

El comando de GRANT ALL PRIVILEGES otorga privilegios de crear, editar y eliminar tablas
de la base de datos al usuario que se le indique. El símbolo ’ %’ admite todas las direcciones IP.
Una vez creado el usuario, se le asignan permisos como administrador.

```shell
MariaDB>GRANT ALL PRIVILEGES ON *.* TO 'samber'@'%';
```


Para actualizar los privilegios y efectuar los cambios en el usuario se utiliza el comando
FLUSH PRIVILEGES.

```shell
MariaDB> FLUSH PRIVILEGES;
```

Una vez terminadas las modificaciones en el usuario se termina la conexión con el gestor de
base de datos.

```shell
MariaDB> quit
```


Una vez creado el usuario, se puede comprobar que este tiene acceso desde cualquier dirección, en el caso de Linux se puede utilizar un cliente de MySQL.


```shell
sudo apt-get install mysql-client
```

Para acceder a la base de datos remotamente se indica la dirección IP del HOST y el usuario
con el que se desea conectar.


```shell
mysql -h HOST -u USERNAME -p
```


**CREAR BASE DE DATOS  DENTRO DE MYSQL**

Entrar a una base de datos mysql 

Con el comando siguientes es para acceder a la base de datos
```shell
 mysql -u samber -p
```
nos pedira que ingresar la contraseña.


– Primero es necesario ver las bases de datos actuales, con el comando ```shell SHOW DATABASES ``` , y
se debe crear  una nueva base de datos llamada “GPS”.

```shell
>CREATE DATABASE GPS;
```
Ahora si vemos las bases de datos nuevamente ( SHOW DATABASES), veremos que
nuestra base de datos “GPS” esta creada.

– Ahora se debe seleccionar la base de datos para poder trabajar con ella:

```shell
 >USE GPS;
```

– Una vez seleccionada la base de datos a utilizar se podra realizar consultas a ella.
– Ahora es necesario crear  una tabla que sera con la que se  trabajara :

```shell
>CREATE TABLE nombre de tabla ( nombre de primer columna INT, Nombre de segunda columna VARCHAR (70));
```

se puede observar que despues del nombre esta la palabra INT este corresponde al tipo de datos .. en este caso es de tipo entero 

En la segunda columna es de tipo VARCHAR por ser cadena y debe estar entreparentesis el tamaño ()
un ejemplo es:

```shell
>CREATE TABLE gprmc ( Node INT, GPRMC VARCHAR (71),GPRMC_S VARCHAR (71));
```

se ha  creado la tabla “gprmc” con dos campos, el campo “GPRMC” que es un varchar
(string) de longitud 70, y un campo “Node” que es un numero de tipo entero.
– para una consulta  a la tabla para obtener los registros existentes, como aun no añade
ningún registro debería decir que esta vacía.

```shell
>SELECT * FROM gprmc;
```
Para crear otra tabla se hace el mismo procedimiento Y asi sucesivamente las demas que se  necesiten.

```shell
CREATE TABLE gpvtg ( Node INT, GPVTG VARCHAR (38),GPVTG_S VARCHAR (38));
CREATE TABLE gptxt ( Node INT, GPTXT VARCHAR (33),GPTXT_S VARCHAR (33));
CREATE TABLE gpgga ( Node INT, GPGGA VARCHAR (74),GPGGA_S VARCHAR (74));
CREATE TABLE gpgsa ( Node INT, GPGSA VARCHAR (62),GPGSA_S VARCHAR (62));
CREATE TABLE gpgsv ( Node INT, GPGSV VARCHAR (69),GPGSV_S VARCHAR (69));
CREATE TABLE gpgll ( Node INT, GPGLL VARCHAR (50),GPGLL_S VARCHAR (50));
```


para salir solo con CTRL C



Mayor informacion [Aqui][2]




## Ejecucion del Programa 
============================================================================================================


1. Abrir la Terminal y Buscar la ubicaciones de la carpeta Samber
2. seguir la siguiente ruta
/samber/client o /samber/server
3. Escribir  
```shell  
make 
```
4. al hacer esto nos apareceran el ejecutable:ya depende que carpeta se eliga. El modo client(Modo de enviar informacion)
y el Server(Modo de recepcion de Informacion)
5. El codigo para el Envio de informacion es

```shell
Sudo./rf95_client
```
<img src="https://raw.githubusercontent.com/cidte/samber/master/pictures/Client_funcionando.png" alt="RF95 client">

6. la otra opcion es utilizar el Lora Dragino en modo de recepcion es

```shell
sudo./rf95_server
```
<img src="https://raw.githubusercontent.com/cidte/samber/master/pictures/Server_funcionando.png" alt="RF95 client">





 
========================================



[1]:http://wiki.dragino.com/index.php?title=Getting_GPS_to_work_on_Raspberry_Pi_3_Model_B
[2]:https://profvanessaborjas.files.wordpress.com/2013/03/mysql_en_c_sec_undersecurity.pdf
[13]: https://github.com/dragino/Lora




