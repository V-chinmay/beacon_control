beaconcontroller:main.o marvelmind_control.o mm_socket.o
	gcc main.o mm_socket.o marvelmind_control.o -ldashapi -lpthread -o beaconcontroller

main.o:main.c
	gcc -c main.c

marvelmind_control.o:marvelmind_control.c
	gcc -c marvelmind_control.c

mm_socket.o:mm_socket.c
	gcc -c mm_socket.c
clean:
	rm *.o*
