# SOA-P2
Web Server Simple

Buffer de mensajes en memoria compratida.

Creado por
	David Prado
	Jose Arias

Secciones 

	Estructura
	Como usarlo
	Ejecucion 

Estructura

Esta compuesto el dos secciones: Librerias (lib) y servidores-cliente-archivos folder, 
iniciaremos revisando servidores-cliente-archivos folder, esta compuesta por 5 servers y un cliente:

	1. Secuencial: macanismo Fifo para manejar las solicitudes.
	2. Forked: crea child al llegar la solicitud.
	3. Threaded: crea thread al llegar la solicitud.
	4. Pre-forked: crea cierta cantidad predefinida de child antes de que llegue la solicitud.
	5. Pre-threaded: crea cierta cantidad predefinida de thread antes de que llegue la solicitud.
	6. Client: se conecta a todos los servidores indentificados por la Ip Address y el puerto que utilizan, pidiendo un archivo,
	   donde catidad de pedidos y ciclos por cada pedido.
	7. makefile: 
	8. Archivos: archivos de texto(txt) y imagenes(jpg), arbitrariamente definidos 1M.txt, 10M.txt, 100M.txt, 1000M.txt; 
	   1M.jpg, 10M.jpg, 100M.jpg, 1000M.jpg; el nombre determina el peso del mismo M representando megas

El siguiente folder incluye las siguientes librerias

  1. error: ayuda con el despliegue de errores, de coneccion puertos y mas.
  2. http: manaja, la envia y recepcion del request.
  3. socket: abre, cierra, conecta, cliente y servidore por medio de la red y el puerto. 
  4. stat:
  5. read_line:simplemente lee caracteres desde un filedescriptor, hasta que encuentra una nueva linea \n.
  6. pr_cpu_time: ayuda con la recoleccion y presentacion de las estadisticas.

Como Usarlo

Para compilarlo el archivo Makerfile crea los ejecutables del programa que son 5: secuential, forked, threaded, pre-forked, pre-threaded y client. Con el siguiente comando:

make -f makefile

Este comando se utiliza para crear los ejecutables, como requisito los archivos deben estar en la misma carpeta de los servidores 
se puede hacer ejecucion simultania de servidor o servidores y cliente o clientes en una misma maquina o en diferentes maquinas conectas en la misma sub-red.

Ejecucion

Se ejecuta en el siguiente orden 

client 

  	Ejecutable <Ip Address> <puerto> <Archivo> <N-threads> <N-ciclos>
	./client 192.168.137.220 2015 1000M.jpg 5 100 

Este comando realiza la coneccion al servidor que tenga el Ip Address y Puerto de los parametros, pidiendo el archivo que tiene que estar en el servidor
en un orden de N-threads y N-ciclos para cada thread 

servers sin pre
	Ejecutable <puerto>
	./secuential 2015
	./forked 2016
	./threaded 2017


servers con pre
	Ejecutable <puerto> <N-threads>
	./pre-threaded 2018 4
	./pre-forked 2019 4

Todos los parametros deben estar separados por espacios, ademas si un servidor toma un puerto, y se inicia otro servidor con el mismo puerto
no se puede realizar la coneccion y da un error.

Durante la ejecucion los servidores imprimen cierta informacion, como la creacion de child o thread dependiendo del tipo de serverdor, 
durante este manejando pedidos termina el pedido que queda en espera para el siguiente pedido, al ser finalizado despliega estadisticas de todo el tiempo funcional. 
Si finalizas un server en media ejecucion de un pedido, se caera la coneccion del cliente

conecciones= Nthreads X Nciclos

forked
Forked new worker process with pid ####
worker process wirh pid #### terminated with status ###
/*uno para cada coneccion

threaded 
created new worker thread
/*uno para cada coneccion

pre-forked
Forked worked process # with pid ###
/* creara cuantos se haya definido en el momento de ejecucion 

pre-threaded
Pre-created worker thread #
/* creara cuantos se haya definido en el momento de ejecucion 
/*al finalizar muestra 
worker thread #, handled i# connections /*
i# siendo el numero de conecciones manejada por cada thread,

estadisticas 
user time = 0.00#, sys time = 0.0##


En el momento de ejecucion de cliente este mostrar 
	
client 
New client thread started

Estadisticas
### STATISTICS ###
User CPU time = 0.00# 
System CPU time = 0.0##
Eleapse time = 0.0#####

Para finalizar cualquiera de los ejecutables en medio de la ejecucion, se utiliza el comando de Linux Ctrl C
