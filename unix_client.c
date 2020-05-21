#include "apue.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

char *socket_path = "\0viraj";

static void die(const char *m) { perror(m); exit(1); }

//function for stream socket for client
void stream_client(int argc, char *argv[]){
  struct sockaddr_un addr;
  char buf[100];
  int fd,rc;

  if (argc > 1) socket_path=argv[2];

  //creating abstract stream socket for client
  if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  if (*socket_path == '\0') {
    *addr.sun_path = '\0';
    strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
  } else {
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
  }

  //tryin to connect to the stream server socket
  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(-1);
  }

  //writing a message to send to the server stream socket
  while( (rc=read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    if (write(fd, buf, rc) != rc) {
      if (rc > 0) fprintf(stderr,"partial write");
      else {
        perror("write error");
        exit(-1);
      }
    }
  }

}

//function for datagram type client socket
void dgram_client(int argc, char *argv[]){
  if (argc != 3) {
        fprintf(stderr, "usage: ./unix_client <sock-type> <msg>\n");
        exit(1);
    }

    const char *name = "unixsocket";
    const char *msg = argv[2];
    int num_repeat = 3;

    struct sockaddr_un  un;

    if (strlen(name) >= sizeof(un.sun_path)) {
        errno = ENAMETOOLONG;
        die("name too long");
    }

    int fd, len, i;

    // creating datagram socket for client
    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
        die("socket failed");

    // filling in the socket address structure with server's address
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

    //sending the datagram message to the server socket
    for (i = 0; i < num_repeat; i++) {
        int n = sendto(fd, msg, strlen(msg), 0, (struct sockaddr *)&un, len);
        if (n < 0) 
            die("send failed");
        else 
            printf("sent %d bytes\n", n);
    }

    close(fd);

}

//main function, checking which type of Unix socket is requested
int main(int argc, char *argv[]) {

if (!strcmp(argv[1], "stream")){
  stream_client(argc, argv);
}
else if(!strcmp(argv[1], "dgram")){
  dgram_client(argc, argv);
}
else if(!strcmp(argv[1], "seqpacket")){
  printf("Work in progress for SOCK_SEQPACKET\n");
}
else{
  printf("No suck socket type in Unix\n");
}
  
  return 0;
}
