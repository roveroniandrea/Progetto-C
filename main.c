#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ip_lib.h"
#include "bmp.h"


int main () {
    ip_mat *p, *sub;
    
    p=ip_mat_create(3,3,3,1.);
    sub = ip_mat_subset(p, 0, 2, 1, 2);
    
    ip_mat_show(p);
    ip_mat_show(sub);
    
    ip_mat_free(p);
    ip_mat_free(sub);
    return 0;
}
