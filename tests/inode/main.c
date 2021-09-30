#include <stdio.h>
#include "../../lib/inode.h"

int main() {
    inode_t inode;

    inode_init(&inode, IF_FILE, 10, 1, 1, 1);
    printf("%d\n", inode.type);

    struct tm * timeinfo;
    timeinfo = localtime(&(inode.created_at));
    printf("%s", asctime(timeinfo));

    return 0;
}