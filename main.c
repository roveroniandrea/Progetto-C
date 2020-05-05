#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ip_lib.h"
#include "bmp.h"


int main () {
    ip_mat *p,*p1,*sum,*sub,*mul_s,*sum_s,*media;
    
    p=ip_mat_create(3,3,3,1.0);
    p1=ip_mat_create(3,3,3,1.0);
    
    sum=ip_mat_sum(p,p1);
    sub=ip_mat_sub(p,p1);
    mul_s=ip_mat_mul_scalar(p,4.56);
    sum_s=ip_mat_add_scalar(p,3);
    media=ip_mat_mean(sum_s,mul_s);
    
    
    ip_mat_show(sum);
    ip_mat_show(sub);
    ip_mat_show(mul_s);
    ip_mat_show(sum_s);
    ip_mat_show(media);
    
    
    ip_mat_free(p);
    ip_mat_free(p1);
    ip_mat_free(sum);
    ip_mat_free(sub);
    ip_mat_free(mul_s);
    ip_mat_free(sum_s);
    ip_mat_free(media);
    
    
    return 0;
}
