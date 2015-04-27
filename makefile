all: servers client

servers: forked secuential threaded pre-forked pre-threaded

forked:
	gcc forked.c lib/error.c lib/http.c lib/read_line.c lib/socket.c lib/pr_cpu_time.c -o forked

secuential:
	gcc secuential.c lib/error.c lib/http.c lib/read_line.c lib/socket.c lib/pr_cpu_time.c -o secuential

threaded:
	gcc threaded.c lib/error.c lib/http.c lib/read_line.c lib/socket.c lib/pr_cpu_time.c -o threaded -pthread

pre-threaded:
	gcc pre-threaded.c lib/error.c lib/http.c lib/read_line.c lib/socket.c lib/pr_cpu_time.c -o pre-threaded -pthread

pre-forked:
	gcc pre-forked.c lib/error.c lib/http.c lib/read_line.c lib/socket.c lib/pr_cpu_time.c -o pre-forked

client:
	gcc client.c lib/socket.c lib/error.c lib/http.c lib/read_line.c -o client -pthread
