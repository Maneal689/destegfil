#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int get_offset(int fd){
    int offset;
    lseek(fd, 10, SEEK_SET);
    read(fd, &offset, 4);
    return (offset);
}

unsigned int get_msg_size(int fd){
    int i = 15;
    unsigned int result = 0;
    unsigned char buffer;

    while (i >= 0){
        result = result << 2;
        read(fd, &buffer, 1);
        result = result | (buffer & 3);
        i--;
    }
    return (result);
}

void get_msg(int fd, char *msg, int msg_size){
    int i = 0;
    int ind_msg = 0;
    unsigned char buffer;

    while (ind_msg < msg_size){
        read(fd, &buffer, 1);
        ind_msg = (int) (i / 4);
        msg[ind_msg] = msg[ind_msg] << 2;
        msg[ind_msg] = msg[ind_msg] | (buffer & 3);
        i++;
    }
}

int main(int argc, char **argv){
    int fd = open(argv[1], O_RDONLY);
    unsigned int msg_size;
    char *msg;
    int offset;

    if (fd == -1){
        printf("Erreur lors de l'ouverture du fichier.\n");
        return (-1);
    }
    offset = get_offset(fd);
    lseek(fd, offset, SEEK_SET);
    msg_size = get_msg_size(fd);
    msg = calloc(msg_size + 1, sizeof(char));
    get_msg(fd, msg, msg_size);
    printf("Message: %s\n", msg);
    free(msg);
    close(fd);
}
