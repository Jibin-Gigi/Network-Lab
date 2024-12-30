#include <stdio.h>
#include <unistd.h>

int main() {
    char *args[] = {"./ex", NULL};
    execvp(args[0], args);
}
