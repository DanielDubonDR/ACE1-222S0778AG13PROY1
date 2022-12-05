# Práctica 3 - Manual Técnico

## INTEGRANTES
| Nombre                           | Carné                                      |
|:--------------------------------:|:------------------------------------------:|
| Daniel Reginaldo Dubón Rodríguez | 201901772                                  |
| Erick Ivan Mayorga Rodríguez     | 201901758                                 	|
| Edmy Marleny Mendoza Pol         | 201901212                                	|
| Kevin Steve Martínez Lemus       | 202004816                  				|

## INTRODUCCIÓN

Se realizó un sistema automático que soluciona la distribución de paquetes hacia las puertas de embarque de una bodega de clasificación de paquetes dependiendo del tamaño de los mismos por medio de bandas transportadoras. La red funciona para varios usuarios y el administrador, presenta diversos estados como mensajes en una pantalla LCD, presenta los registros y estadísticas de los paquetes enviados dependiendo del tamaño, teniendo todos los registros así como los usuarios del sistema guardados en la memoria EEPROM. Se utilizó una red de Arduinos usando comunicación serial además de una aplicación Bluetooth para el manejo del sistema. 

## COMPONENTES UTILIZADOS
- <b>3 Arduinos Mega</b> <br>
Para controlar los diferentes modulos y darle funcionamiento a las partes
<br>
<img src="./Imagenes/arduinoMega.png" alt="drawing" style="width:100px;"/><br>
- <b>1 Teclado alfanumérico matricial</b> <br>
Para poder ingresar caracteres en la apliacion
<br>
<img src="./Imagenes/tecladoMatricial.png" alt="drawing" style="width:100px;"/><br>
- <b>8 Motores stepper</b> <br>
Para poder mover las bandas de distribucion
<br>
<img src="./Imagenes/motorStepper.png" alt="drawing" style="width:100px;"/><br>
- <b>1 Servomotor </b><br>
Para poder redirigir el paquete a su banda correspondiente
<br>
<img src="./Imagenes/Servomotor.png" alt="drawing" style="width:100px;"/><br>
- <b>1 Pantalla LCD </b><br>
 Para visualizar informacion sobre el estado de la aplicacion
<br>
<img src="./Imagenes/LCD.png" alt="drawing" style="width:100px;"/><br>
- <b>1 Buzzer <br></b>
 Para poder escuchar la alarma cuando se despachan los paquetes
<br>
<img src="./Imagenes/Buzzer.png" alt="drawing" style="width:100px;"/><br>
- <b>1 Módulo Bluetooth</b> <br>
  Para poder recibir datos a travez de la aplicacion por medio de bluetooth
<br>
<img src="./Imagenes/moduloBluetooth.png" alt="drawing" style="width:100px;"/><br>
- <b>40 Leds de dos colores </b><br>
  Para simular el avance de los paquetes
<br>
<img src="./Imagenes/Leds.png" alt="drawing" style="width:100px;"/><br>
- <b>8 Demultiplexores 74154 </b><br>
  Para poder controlar los leds de una forma mas eficiente
<br>
<img src="./Imagenes/demultiplexor.png" alt="drawing" style="width:100px;"/><br>
- <b>4 drivers  L293D</b><br>
 Para poder controlar algunos motores stepers
<br>
<img src="./Imagenes/driver.png" alt="drawing" style="width:100px;"/><br>

## RED DE ARDUINOS
Para la desarrollar la red de arduinos se utilizó comunicación serial.

<img src="./Imagenes/red.png" alt="drawing" style="width:100px;"/><br>

Se utilizaron los arduinos de la siguiente manera:

- **Maestro :** El dispositivo maestro se encarga de:
    - Controlar la pantalla LCD
	- Recabar la información recibida
	- Guardar la información en memoria EEPROM
	- CRUD de Usuarios
	- Calcular estadísticas
	- Verificar el Login del usuario
  
<img src="./Imagenes/maestro.png" alt="drawing" style="width:100px;"/><br>

- **Esclavo 1:** El dispositivo esclavo manejará:
	- El movimiento del paquete en la banda transportadora
	- Control de servomotor
	- Registro de cantidad de paquetes disponibles
  
<img src="./Imagenes/esclavo1.png" alt="drawing" style="width:100px;"/><br>

- **Esclavo 2:** El dispositivo esclavo manejará:
	- Control de motores stepper
	- Indicador de envío de despacho por medio del buzzer

<img src="./Imagenes/esclavo2.png" alt="drawing" style="width:100px;"/><br>
	
## CODIGO UTILIZADO
### Maestro
Las variables globales nos permitirán definir las entradas, la salidas y manejar los datos correspondientes a los estados del mismo. <br>

<img src="./Imagenes/globales.png" alt="drawing" style="width:100px;"/>

Se definieron los caracteres especiales que se mostrarán en la pantalla LCD. <br>

<img src="./Imagenes/caracteres.png" alt="drawing" style="width:100px;"/>

En la sección de declaración de pines se inicia la comunicación serial entre los tres arduinos por medio del comando Serial#begin(9600), donde # es el número de puerto serial del arduino maestro utilizado.

<img src="./Imagenes/pinesM.png" alt="drawing" style="width:100px;"/>

Con el método `msgInicio` se muestra el mensaje de inicio teniendo en la línea superior los caracteres especiales formando "🡆Bienvenido🡄" y en la línea inferior se tiene "Grupo13-SeccionA" de la pantalla LCD.

<img src="./Imagenes/msgInicio.png" alt="drawing" style="width:100px;"/>

Se utilizó la variable `estado` para saber que debe de ejecutar el arduino maestro, ya sea iniciar sesión, utilizar la opción del CRUD, ver reportes, gestionar paquetes o cerrar sesión del administrador. 

<img src="./Imagenes/Estado.png" alt="drawing" style="width:100px;"/>

Para guardar la información recibida, así como los usuarios, se utilizó la memoria EEPROM usando los comandos EEPROM.get(), EEPROM.put() y EEPROM.read().

<img src="./Imagenes/EEPROM.png" alt="drawing" style="width:100px;"/>


### Esclavo 1
Las variables globales nos permitirán definir las entradas, la salidas y manejar los datos correspondientes a los estados del mismo.

<img src="./Imagenes/globalesE1.png" alt="drawing" style="width:100px;"/>

En la sección de declaración de pines se inicia la comunicación serial entre los tres arduinos por medio del comando Serial#begin(9600), donde # es el número de puerto serial del arduino maestro utilizado. 

<img src="./Imagenes/pinesE1.png" alt="drawing" style="width:100px;"/>

Se utilizó las funciones `paqueteSmall`, `paqueteMiddle` y `paqueteBig` para visualizar el movimiento del paquete por medio de leds. 

<img src="./Imagenes/paquete.png" alt="drawing" style="width:100px;"/>

Para visualizar los paquetes se utilizó leds de dos colores junto con las funciones `pintarAzul` y `pintarAmarillo` para indicar el color de led a utilizar según el tamaño de paquete. 

<img src="./Imagenes/pintarLed.png" alt="drawing" style="width:100px;"/>

Se utilizó una variable `estadoBanda` y `estadoAux` para indicar el movimiento de motores según el tamaño de paquete.  
<img src="./Imagenes/EstadoE1.png" alt="drawing" style="width:100px;"/>

# Esclavo 2
Las variables globales nos permitirán definir las entradas, la salidas y manejar los datos correspondientes a los estados del mismo.

<img src="./Imagenes/globalesE2.png" alt="drawing" style="width:100px;"/>

En la sección de declaración de pines se inicia la comunicación serial entre arduinos por medio del comando Serialbegin(9600).

<img src="./Imagenes/pinesE2.png" alt="drawing" style="width:100px;"/>

Se utilizó la función `motorCorrer` para lograr el movimiento del motor stepper. 

<img src="./Imagenes/motorCorrer.png" alt="drawing" style="width:100px;"/>

Se utilizó la función `buzz#` donde # es el número de linea de la banda transportadora en uso para lograr el tono según el despacho de paquetes.

<img src="./Imagenes/Buzz.png" alt="drawing" style="width:100px;"/>

Se utilizó la variable `msg` para determinar el motor stepper o buzzer a activar. 

<img src="./Imagenes/EstadoE2.png" alt="drawing" style="width:100px;"/>
