/*
 Created by Sebastiano Vascon on 23/03/20.
*/

#include <stdio.h>
#include "ip_lib.h"
#include "bmp.h"

/*--------------------------------------------------*/

/* Inizializza una ip_mat con dimensioni h w e k. Ogni elemento è inizializzato a v.
 * Inoltre crea un vettore di stats per contenere le statische sui singoli canali.
 * */
ip_mat * ip_mat_create(unsigned int h, unsigned int w, unsigned int k, float v){
    int i,j,q;
    if(h > 0 && w > 0 && k > 0){
        ip_mat *pointer;
        
        pointer=(ip_mat*)malloc(sizeof(ip_mat));
        pointer->data=(float ***)malloc(h*sizeof(float **));/*Alloco il vettore delle righe*/
        

        for(i=0;i<h;i++){
            pointer->data[i]=(float **)malloc(w*sizeof(float *));/*Alloco il vettore delle colonne*/
            
            for(j=0;j<w;j++){
                pointer->data[i][j]=(float *)malloc(k*sizeof(float));/*Alloco il vettore dei canali*/
                
                for(q=0;q<k;q++){
                    pointer->data[i][j][q]=v;/*Assegno v ad ogni elemento*/
                }
            }   
        }


        pointer->stat=(stats*)malloc(sizeof(stats));/*Alloco il vettore per le stats*/
        
        pointer->h=h;
        pointer->w=w;
        pointer->k=k;
        
        return pointer;
    }
    else{
        printf("H, W e K must be > 0\n");
        exit(1);
    }

}

/* Libera la memoria (data, stat e la struttura) */
void ip_mat_free(ip_mat *a){
    int i,j;

    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            free(a->data[i][j]);        
        }
        free(a->data[i]);  
    }
    
    free(a->data);  
    free(a->stat); 
    free(a);
    a=NULL;
}


/* Esegue la somma di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output. */
ip_mat * ip_mat_sum(ip_mat * a, ip_mat * b){
    
    int i,j,q;
    ip_mat *pointer;
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                
                if(get_val(a,i,j,q) && get_val(b,i,j,q))
                    set_val(pointer,i,j,q,get_val(a,i,j,q) + get_val(b,i,j,q)); 

            }
        }   
    }
    
    return pointer;
}


/* Esegue la sottrazione di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output.
 * */
ip_mat * ip_mat_sub(ip_mat * a, ip_mat * b){
    int i,j,q;
    ip_mat *pointer;
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                
                if(get_val(a,i,j,q) && get_val(b,i,j,q))
                    set_val(pointer,i,j,q,get_val(a,i,j,q) - get_val(b,i,j,q)); 
                
            }
        }   
    }
    
    return pointer;
}


/* Moltiplica un ip_mat per uno scalare c. Si moltiplica c per tutti gli elementi di "a"
 * e si salva il risultato in un nuovo tensore in output. */
ip_mat * ip_mat_mul_scalar(ip_mat *a, float c){
    int i,j,q;
    ip_mat *pointer;
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                
                if(get_val(a,i,j,q))
                    set_val(pointer,i,j,q,(get_val(a,i,j,q) * c));
            
            }
        }   
    }
    
    return pointer;
}


/* Aggiunge ad un ip_mat uno scalare c e lo restituisce in un nuovo tensore in output. */
ip_mat *  ip_mat_add_scalar(ip_mat *a, float c){
    int i,j,q;
    ip_mat *pointer;
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                
                if(get_val(a,i,j,q))
                    set_val(pointer,i,j,q,get_val(a,i,j,q) + c);
            }
        }   
    }
    
    return pointer;
}

/* Calcola la media di due ip_mat a e b e la restituisce in output.*/
ip_mat * ip_mat_mean(ip_mat * a, ip_mat * b){
    int i,j,q;
    float medium_value;
    ip_mat *pointer;
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                
                if(get_val(a,i,j,q) && get_val(b,i,j,q)){
                    medium_value = get_val(a,i,j,q) + get_val(b,i,j,q) / 2.0;
                    set_val(pointer,i,j,q,medium_value);
                }
            }
        }   
    }
    
    return pointer;
}

/* Crea una copia di una ip_mat e lo restituisce in output */
ip_mat * ip_mat_copy(ip_mat * in){
    int i, j, k;
    ip_mat *matr;
    matr = ip_mat_create(in->h, in->w, in->k, 0.);
    
    for(i=0; i < in->h;i++){
        for(j = 0; j < in->w; j++){
            for(k = 0; k < in->k; k++){
                float val = get_val(in, i, j, k);
                set_val(matr, i, j, k, val);
            }
        }
    }
    
    return matr;
}

/* Inizializza una ip_mat con dimensioni w h e k.
 * Ogni elemento è generato da una gaussiana con media mean e varianza var */
void ip_mat_init_random(ip_mat * t, float mean, float var){
    int i, j, k;
    
    for(i=0; i < t->h;i++){
        for(j = 0; j < t->w; j++){
            for(k = 0; k < t->k; k++){
                /*Distribuzione normale moltiplicata per la varianza e sommata alla media*/
                float val = get_normal_random() * var + mean;
                set_val(t, i, j, k, val);
            }
        }
    }
}

/* Restituisce una sotto-matrice, ovvero la porzione individuata da:
 * t->data[row_start...row_end][col_start...col_end][0...k]
 * La terza dimensione la riportiamo per intero, stiamo in sostanza prendendo un sottoinsieme
 * delle righe e delle colonne.
 * */
ip_mat * ip_mat_subset(ip_mat * t, unsigned int row_start, unsigned int row_end, unsigned int col_start, unsigned int col_end){
    int numr, numc, i, j, k;
    
    /*Controllo che la sottomatrice abbia range accettabili*/
    if(row_start >= 0 && row_start <= t->h && row_start < row_end && col_start >= 0 && col_start <= t->w && col_start < col_end){
        ip_mat *submat;
        numr = row_end - row_start;
        numc = col_end - col_start;
        
        submat = ip_mat_create(numr, numc, t->k, 0.);
        
        for(i = row_start; i < row_end;i++){
            for(j = col_start; j < col_end; j++){
                for(k = 0; k < t->k; k++){
                    float val = get_val(t, i, j, k);
                    set_val(submat, i - row_start, j - col_start, k, val);
                }
            }
        }
        
        return submat;
    }
    else{
        printf("ip_mat_subset range non accettabili\n");
        exit(1);
    }
}


/*--------------------------------------------------*/


void ip_mat_show(ip_mat * t){
    unsigned int i,l,j;
    printf("Matrix of size %d x %d x %d (hxwxk)\n",t->h,t->w,t->k);
    for (l = 0; l < t->k; l++) {
        printf("Slice %d\n", l);
        for(i=0;i<t->h;i++) {
            for (j = 0; j < t->w; j++) {
                printf("%f ", get_val(t,i,j,l));
            }
            printf("\n");
        }
        printf("\n");
    }
}

void ip_mat_show_stats(ip_mat * t){
    unsigned int k;

    /*compute_stats(t);*/

    for(k=0;k<t->k;k++){
        printf("Channel %d:\n", k);
        printf("\t Min: %f\n", t->stat[k].min);
        printf("\t Max: %f\n", t->stat[k].max);
        printf("\t Mean: %f\n", t->stat[k].mean);
    }
}

ip_mat * bitmap_to_ip_mat(Bitmap * img){
    unsigned int i=0,j=0;

    unsigned char R,G,B;

    unsigned int h = img->h;
    unsigned int w = img->w;

    ip_mat * out = ip_mat_create(h, w,3,0);

    for (i = 0; i < h; i++)              /* rows */
    {
        for (j = 0; j < w; j++)          /* columns */
        {
            bm_get_pixel(img, j,i,&R, &G, &B);
            set_val(out,i,j,0,(float) R);
            set_val(out,i,j,1,(float) G);
            set_val(out,i,j,2,(float) B);
        }
    }

    return out;
}

Bitmap * ip_mat_to_bitmap(ip_mat * t){

    Bitmap *b = bm_create(t->w,t->h);

    unsigned int i, j;
    for (i = 0; i < t->h; i++)              /* rows */
    {
        for (j = 0; j < t->w; j++)          /* columns */
        {
            bm_set_pixel(b, j,i, (unsigned char) get_val(t,i,j,0),
                    (unsigned char) get_val(t,i,j,1),
                    (unsigned char) get_val(t,i,j,2));
        }
    }
    return b;
}

float get_val(ip_mat * a, unsigned int i,unsigned int j,unsigned int k){
    if(i<a->h && j<a->w &&k<a->k){  /* j>=0 and k>=0 and i>=0 is non sense*/
        return a->data[i][j][k];
    }else{
        printf("Errore get_val!!!");
        exit(1);
    }
}

void set_val(ip_mat * a, unsigned int i,unsigned int j,unsigned int k, float v){
    if(i<a->h && j<a->w &&k<a->k){
        a->data[i][j][k]=v;
    }else{
        printf("Errore set_val!!!");
        exit(1);
    }
}

float get_normal_random(){
    float y1 = ( (float)(rand()) + 1. )/( (float)(RAND_MAX) + 1. );
    float y2 = ( (float)(rand()) + 1. )/( (float)(RAND_MAX) + 1. );
    return cos(2*PI*y2)*sqrt(-2.*log(y1));

}
