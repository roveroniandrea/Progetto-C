#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include "ip_lib.h" 
#include "bmp.h" 
int main () { 
    Bitmap *final_bmp,*loaded,*loaded1,*loaded2; 
    ip_mat *filter,*p,*p1,*p2,*p3,*p_partial,*p_partial2; 
    
    printf("\n\n\tElaborazione immagini in corso, attendere ...\n\n");
    
    
    /*Load the two bmp*/
    loaded = bm_load("flower.bmp"); 
    p1 = bitmap_to_ip_mat(loaded); 
    loaded1 = bm_load("mongolfiere.bmp"); 
    p2 = bitmap_to_ip_mat(loaded1);
    loaded2 = bm_load("flower2.bmp"); 
    p3 = bitmap_to_ip_mat(loaded2);
    
    
    
    /*CORRUPT*/
    p=ip_mat_corrupt(p1,30);
    p_partial=ip_mat_concat(p1,p,1);
    final_bmp = ip_mat_to_bitmap(p_partial); 
    bm_save(final_bmp, "BMP/corrupt.bmp"); 
    ip_mat_free(p);
    ip_mat_free(p_partial);
    bm_free(final_bmp);
    
    
    /*GRAY*/
    p=ip_mat_to_gray_scale(p1);

    p_partial=ip_mat_concat(p1,p,1);
    /*clamp(p_partial,0,255);*/
    
    final_bmp = ip_mat_to_bitmap(p_partial); 
    bm_save(final_bmp, "BMP/gray_scale.bmp"); 
    ip_mat_free(p);
    ip_mat_free(p_partial);
    bm_free(final_bmp);
    
    
    /*BRIGHTEN*/
    p=ip_mat_brighten(p1,50);
    clamp(p,0,255);
    p_partial=ip_mat_concat(p1,p,1);
    final_bmp = ip_mat_to_bitmap(p_partial); 
    bm_save(final_bmp, "BMP/brighten.bmp"); 
    ip_mat_free(p);
    ip_mat_free(p_partial);
    bm_free(final_bmp);
    
    
    /*BLEND*/
    p=ip_mat_blend(p2,p3,0.5);
    p_partial=ip_mat_concat(p2,p,1);
    p_partial2=ip_mat_concat(p3,p,1);
    ip_mat_free(p);
    
    p=ip_mat_concat(p_partial,p_partial2,0);
    final_bmp = ip_mat_to_bitmap(p); 
    bm_save(final_bmp, "BMP/blend.bmp"); 
    ip_mat_free(p);
    ip_mat_free(p_partial);
    ip_mat_free(p_partial2);
    bm_free(final_bmp);
    
    
    /*SHARP*/
    filter=create_sharpen_filter();
    p = ip_mat_convolve(p1, filter); 
    clamp(p,0,255);
    p_partial=ip_mat_concat(p1,p,1);
    final_bmp = ip_mat_to_bitmap(p_partial); 
    bm_save(final_bmp, "BMP/sharp.bmp"); 
    ip_mat_free(p);
    ip_mat_free(filter);
    ip_mat_free(p_partial);
    bm_free(final_bmp);
    
    
    
    /*EDGE*/
    filter=create_edge_filter();
    p = ip_mat_convolve(p1, filter); 
    clamp(p,0,255);
    p_partial=ip_mat_concat(p1,p,1);
    final_bmp = ip_mat_to_bitmap(p_partial); 
    bm_save(final_bmp, "BMP/edge.bmp"); 
    ip_mat_free(p);
    ip_mat_free(filter);
    ip_mat_free(p_partial);
    bm_free(final_bmp);
    
    
    /*EMBOSS*/
    filter=create_emboss_filter();
    p = ip_mat_convolve(p1, filter); 
    clamp(p,0,255);
    p_partial=ip_mat_concat(p1,p,1);
    final_bmp = ip_mat_to_bitmap(p_partial); 
    bm_save(final_bmp, "BMP/emboss.bmp"); 
    ip_mat_free(p);
    ip_mat_free(filter);
    ip_mat_free(p_partial);
    bm_free(final_bmp);
        
    
    /*AVG*/
    filter=create_average_filter(5, 5, 3);
    p = ip_mat_convolve(p1, filter); 
    clamp(p,0,255);
    p_partial=ip_mat_concat(p1,p,1);
    final_bmp = ip_mat_to_bitmap(p_partial); 
    bm_save(final_bmp, "BMP/avg.bmp"); 
    ip_mat_free(p);
    ip_mat_free(filter);
    ip_mat_free(p_partial);
    bm_free(final_bmp);
    
    
    /*GAUSS*/
    filter=create_gaussian_filter(7, 7, 3, 4);
    p = ip_mat_convolve(p1, filter); 
    clamp(p,0,255);
    p_partial=ip_mat_concat(p1,p,1);
    final_bmp = ip_mat_to_bitmap(p_partial); 
    bm_save(final_bmp, "BMP/gaussian.bmp"); 
    ip_mat_free(filter);
    ip_mat_free(p);
    ip_mat_free(p_partial);
    bm_free(final_bmp);
    
    

     
    bm_free(loaded);
    bm_free(loaded1);
    bm_free(loaded2);
   
    ip_mat_free(p1);
    ip_mat_free(p2);
    ip_mat_free(p3);

    
    return 0; 
    
}
