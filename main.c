#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ip_lib.h"
#include "bmp.h"


int main () {
    ip_mat *p;
    
    p=ip_mat_create(3,3,3,1.);
    
    ip_mat_show(p);
    ip_mat_free(p);
    return 0;
}
