#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ip_lib.h"
#include "bmp.h"


int main () {
    Bitmap *final, *loaded, *l2;
    ip_mat *matloaded, *edgekernel, *matfinal, *matloaded2;
    
    loaded = bm_load("flower.bmp");
    l2 = bm_load("mongolfiere.bmp");
    
    matloaded = bitmap_to_ip_mat(loaded);
    matloaded2 = bitmap_to_ip_mat(l2);
    edgekernel = create_edge_filter();
    
    matfinal = ip_mat_convolve(matloaded, edgekernel);
    /*matfinal = ip_mat_blend(matloaded, matloaded2, 1);*/
    /*rescale(matfinal, 255);*/
    clamp(matfinal, 0, 255);
    
    final = ip_mat_to_bitmap(matfinal);
    bm_save(final, "Prova.bmp");
    
    bm_free(loaded);
    bm_free(final);
    bm_free(l2);
    ip_mat_free(matloaded);
    ip_mat_free(edgekernel);
    ip_mat_free(matfinal);
    ip_mat_free(matloaded2);
    
    return 0;
}
