#include "apue.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char *socket_path = "\0viraj";

static void die(const char *m) { perror(m); exit(1); }

//function to perform stream socket at server
void stream_server(int argc, char *argv[]){

  struct sockaddr_un addr;
  char buf[100];
  int fd,cl,rc;

  if (argc > 1) socket_path=argv[2];

  //creating abstract stream socket 
  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("Error: socket not created");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  if (*socket_path == '\0') {
    *addr.sun_path = '\0';
    strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
  } else {
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    unlink(socket_path);
  }

  //binding address to stream socket
  if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("Error: socket didn't bound");
    exit(-1);
  }

  //listening for client of stream socket
  if (listen(fd, 5) == -1) {
    perror("Error: socket listen failed");
    exit(-1);
  }

  //accepting the client connection whenever coming in
  while (1) {
    if ( (cl = accept(fd, NULL, NULL)) == -1) {
      perror("Error: socket accept failed");
      continue;
    }

    //reading the client message everytime
    while ( (rc=read(cl,buf,sizeof(buf))) > 0) {
      printf("read %u bytes: %.*s\n", rc, rc, buf);
    }
    if (rc == -1) {
      perror("Error: socket read unsuccesful");
      exit(-1);
    }
    else if (rc == 0) {
      printf("EOF\n");
      close(cl);
    }
  }

}

//function for datagram socket for server
void dgram_server(int argc, char *argv[]){

    if(argc != 2) {
        fprintf(stderr, "usage: ./unix_server <sock-type> \n");
        exit(1);
    }

    const char *name = "unixsocket";
    int num_to_recv = 10;

    struct sockaddr_un  un;

    if (strlen(name) >= sizeof(un.sun_path)) {
        errno = ENAMETOOLONG;
        die("Error: socket name too long");
    }

    int fd, len;

    // creating UNIX domain datagram socket for server
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
        die("Error: socket not created");

    // removing the socket file if already exists
    unlink(name);

    // filling in the socket address structure
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

    // binding the name to the file descriptor
    if (bind(fd, (struct sockaddr *)&un, len) < 0) 
        die("Error: socket didn't bound");

    char buf[num_to_recv + 1];

    //creating infinite loop to receive the message from client
    for (;;) {
        memset(buf, 0, sizeof(buf));
        int n = recv(fd, buf, num_to_recv, 0);
        if (n < 0) 
            die("Error: socket receive unsuccesful");
        else
            printf("%d bytes received: \"%s\"\n", n, buf);
    }

    close(fd);
    unlink(name);
    return;
}

//main function, checking which type of Unix socket is requested
int main(int argc, char *argv[]) {
  
if (!strcmp(argv[1], "stream")){
  stream_server(argc, argv);
}
else if(!strcmp(argv[1], "dgram")){
  dgram_server(argc, argv);
}
else if(!strcmp(argv[1], "seqpacket")){
  printf("Work in progress for SOCK_SEQPACKET\n");
}
else{
  printf("No suck socket type in Unix\n");
}

  return 0;
}