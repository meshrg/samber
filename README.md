Gateway IP para red LoRa con base de datos SQL
===========================================================
Envio y recepcion de Gps apartir del puerto UART De la shield LoRa Dragino HAT/GPS,Despues de la recepcion de la informacion, se almacena en una base de datos Mysql de manera de respaldo de información

=================================

**Compatible Con**    

 [Dragino Lora GPS HAT][13]

<img src="http://wiki.dragino.com/images/d/d6/Lora_GPS_HAT.png" height="25%" width="25%" alt="Raspberry PI Lora Gateway/Node">   



### Instalacion en Raspberry PI
================================
En primer punto es diferente la instalacion para el server como para el client, sin embargo exiten algunos pasos iniciales que se deben realizar en las dos Raspberrys



## Cliente 
================================

Clonar el repositorio
```shell
https://github.com/cidte/samber.git
```



##Server
================================
Clonar el repositorio
```shell
https://github.com/cidte/samber.git
```
**Base de datos**
escribir en la linea de comandos 
```shell
sudo apt-get install mysql-server
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

El siguiente comando crea una nueva cuenta de root con acceso desde localhost y con
contraseña.

```shell
MariaDB>CREATE USER 'Samber'@'localhost' IDENTIFIED BY 'cidte';

```

El comando de GRANT ALL PRIVILEGES otorga privilegios de crear, editar y eliminar tablas
de la base de datos al usuario que se le indique.

```shell
MariaDB>GRANT ALL PRIVILEGES ON *.* TO 'Samber'@'localhost';
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



**INSTALACIÓN DE SERVIDOR HTTP APACHE**

El servidor HTTP Apache se instala a través del gestor de paquetes de Linux.
```shell
$ sudo apt-get install apache2
```

Una vez finalizada la instalación, se puede corroborar la correcta instalación del servicio
accediendo a través de un navegador a la dirección de localhost. La página por defecto de apache
se muestra en la Figura 1.


**INSTALANDO PHP**
Al instalar PHP se deben agregar los conectores con apache y mysql.

```shell
$ sudo apt-get install php7.0 libapache2-mod-php7.0 php7.0-mysql
```


**INSTALANDO PHP MY ADMIN**
phpMyAdmin permite manipular bases de datos de MySQL a través de una interfaz web de
forma gráfica, utilizando herramientas php y apache.
Con ayuda del gestor de paquetes de Linux se instala phpMyAdmin.

```shell
$ sudo apt-get install phpmyadmin
```


Durante la instalación, se elige usar Apache como servidor web.


Adicionalmente preguntará si se desea instalar la base de datos dbconfig-common, a lo que
se le indicará que No.


un segmento de red o una dirección IP en específico.
Para habilitar las peticiones remotas se debe de modificar el archivo etc con el siguiente comando 


```shell
$ sudo nano /etc/mysql/mariadb.conf.d/50-server.cnf
```

El parámetro bi nd −ad d r ess = 127,0,0,1 delimita las direcciones IP donde se pueden realizar
las conexiones entrantes, para permitir la conexión desde cualquier dirección IP este se cambia
por 0,0,0,0.
bind-address= 0.0.0.0

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



**CREAR BASE DE DATOS  DENTRO DE MYSQL**

Entrar a una base de datos mysql 

Con el comando siguientes es para acceder a la base de datos
```shell
 mysql -u Samber -p
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
```shell
– Una vez seleccionada la base de datos a utilizar se podra realizar consultas a ella.
– Ahora es necesario crear  una tabla que sera con la que se  trabajara :

```shell
>CREATE TABLE nombre de tabla ( nombre de primer columna INT, Nombre de segunda columna VARCHAR (70));
```shell

se puede observar que despues del nombre esta la palabra INT este corresponde al tipo de datos .. en este caso es de tipo entero 

En la segunda columna es de tipo VARCHAR por ser cadena y debe estar entreparentesis el tamaño ()
un ejemplo es:

```shell
CREATE TABLE gprmc ( Node INT, GPRMC VARCHAR (70));
```

se ha  creado la tabla “gprmc” con dos campos, el campo “GPRMC” que es un varchar
(string) de longitud 70, y un campo “Node” que es un numero de tipo entero.
– para una consulta  a la tabla para obtener los registros existentes, como aun no añade
ningún registro debería decir que esta vacía.

```shell
>SELECT * FROM gpŕmc
```
Para crear otra tabla se hace el mismo procedimiento Y asi sucesivamente las demas que se  necesiten.

```shell
>CREATE TABLE gpvtg ( Node INT, GPVTG VARCHAR (36));
```

para salir solo con CTRL C






### CIDTE 
PASANTE DE INGENIERIA EN COMUNICACIONES Y ELECTRONICA MARTIN ABRAHAM DE LARA MEDINA
ASESOR DR. VIKTOR IVAN RODRIGUEZ ABDALA 
========================================




[13]: https://github.com/dragino/Lora




