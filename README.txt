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
  5. read_line:
  6. pr_cpu_time: ayuda con la recoleccion y presentacion de las estadisticas.

Como Usarlo

Para compilarlo el archivo Makerfile crea los ejecutables del programa que son 5: secuential, forked, threaded, pre-forked, pre-threaded y client. Con el siguiente comando:

make -f makefile

Este comando se utiliza para crear los ejecutables, como requisito los archivos deben estar en la misma carpeta de los servidores 
se puede hacer ejecucion simultania de servidor o servidores y cliente o clientes en una misma maquina o en diferentes maquinas conectas en la misma sub-red.

Ejecucion

Se ejecuta en el siguiente orden 

client 

  programa <Ip Address> <puerto> <Archivo> <N-threads> <N-ciclos>
	./client 192.168.137.220 2015 1000M.jpg 5 100 

servers sin pre

servers con pre




	




