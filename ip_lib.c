/*
 Created by Sebastiano Vascon on 23/03/20.
*/
/*ID: 23 Andrea Roveroni 880092, Matteo Checchin 879904, Leonardo Fasolato 880653*/
#include <stdio.h>
#include "ip_lib.h"
#include "bmp.h"

/*--------------------------------------------------*/

/*Controlla che due matrici abbiano le stesse dimensioni*/
void check_dimensions(ip_mat *a, ip_mat *b){
    if(!(a->w == b->w && a->h == b->h && a->k == b->k)){
        printf("ip_mat must have same size!\n");
        exit(1);
    }
}
/*Controlla che la allocazione della memoria sia andata a buon fine*/
void check_malloc(int valid){
    if(!valid){
        printf("Malloc returned NULL pointer\n");
        exit(1);
    }
}

/* Inizializza una ip_mat con dimensioni h w e k. Ogni elemento è inizializzato a v.
 * Inoltre crea un vettore di stats per contenere le statische sui singoli canali.
 * */
ip_mat * ip_mat_create(unsigned int h, unsigned int w, unsigned int k, float v){ /*checchin*/
    unsigned int i,j,q;
    if(h > 0 && w > 0 && k > 0){
        ip_mat *pointer;
        
        pointer=(ip_mat*)malloc(sizeof(ip_mat));
        check_malloc(pointer != NULL);
        
        pointer->data=(float ***)malloc(h*sizeof(float **));/*Alloco il vettore delle righe*/
        check_malloc(pointer->data != NULL);
        pointer->h=h;
        pointer->w=w;
        pointer->k=k;
        
        for(i=0;i<h;i++){
            pointer->data[i]=(float **)malloc(w*sizeof(float *));/*Alloco il vettore delle colonne*/
            check_malloc(pointer->data[i] != NULL);
            
            for(j=0;j<w;j++){
                pointer->data[i][j]=(float *)malloc(k*sizeof(float));/*Alloco il vettore dei canali*/
                check_malloc(pointer->data[i][j] != NULL);
                
                for(q=0;q<k;q++){
                    set_val(pointer,i,j,q,v);/*Assegno v ad ogni elemento*/
                }
            }   
        }
        
        pointer->stat=(stats*)malloc(sizeof(stats) * k);/*Alloco il vettore per le stats*/
        check_malloc(pointer->stat != NULL);
        
        for(i=0;i<k;i++){
            (pointer->stat)[i].min = v;
            (pointer->stat)[i].max = v;
            (pointer->stat)[i].mean = v;
        }
        return pointer;
    }
    else{
        printf("H, W e K must be > 0\n");

        exit(1);
        
    }

}

/* Libera la memoria (data, stat e la struttura) */
void ip_mat_free(ip_mat *a){ /*checchin*/
    unsigned int i,j;
    if(a){
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
}

/* Calcola il valore minimo, il massimo e la media per ogni canale
 * e li salva dentro la struttura ip_mat stats
 * */
void compute_stats(ip_mat * t){/*rove*/
    unsigned int k;
    for(k = 0; k < t->k; k++){
        unsigned int i, j;
        float min = FLT_MAX, max = FLT_MIN, mean = 0, sum = 0;
        for(i = 0; i < t->h; i++){
            for(j = 0; j < t->w; j++){
                float val = get_val(t, i, j, k);
                sum += val;
                if(val < min)
                    min = val;
                if(val > max)
                    max = val;
            }
        }
        mean = sum / (i * j);
        (t->stat)[k].min = min;
        (t->stat)[k].max = max;
        (t->stat)[k].mean = mean;
    }
}

/* Esegue la somma di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output. */
ip_mat * ip_mat_sum(ip_mat * a, ip_mat * b){/*faso*/
    unsigned int i,j,q;
    ip_mat *pointer;
    check_dimensions(a, b);
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                set_val(pointer,i,j,q,get_val(a,i,j,q) + get_val(b,i,j,q)); 
            }
        }   
    }
    compute_stats(pointer);
    
    return pointer;
}


/* Esegue la sottrazione di due ip_mat (tutte le dimensioni devono essere identiche)
 * e la restituisce in output.
 * */
ip_mat * ip_mat_sub(ip_mat * a, ip_mat * b){/*faso*/
    unsigned int i,j,q;
    ip_mat *pointer;
    check_dimensions(a, b);
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                set_val(pointer,i,j,q,get_val(a,i,j,q) - get_val(b,i,j,q)); 
            }
        }   
    }
    compute_stats(pointer);        
    return pointer;
}


/* Moltiplica un ip_mat per uno scalare c. Si moltiplica c per tutti gli elementi di "a"
 * e si salva il risultato in un nuovo tensore in output. */
ip_mat * ip_mat_mul_scalar(ip_mat *a, float c){/*faso*/
    unsigned int i,j,q;
    ip_mat *pointer;
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                set_val(pointer,i,j,q,(get_val(a,i,j,q) * c));
            }
        }   
    }
    compute_stats(pointer);
    return pointer;
}


/* Aggiunge ad un ip_mat uno scalare c e lo restituisce in un nuovo tensore in output. */
ip_mat *  ip_mat_add_scalar(ip_mat *a, float c){/*faso*/
    unsigned int i,j,q;
    ip_mat *pointer;
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                set_val(pointer,i,j,q,get_val(a,i,j,q) + c);
            }
        }   
    }
    compute_stats(pointer);
    return pointer;
}

/* Calcola la media di due ip_mat a e b e la restituisce in output.*/
ip_mat * ip_mat_mean(ip_mat * a, ip_mat * b){/*faso*/
    unsigned int i,j,q;
    ip_mat *pointer;
    check_dimensions(a, b);
    pointer = ip_mat_create(a->h,a->w,a->k,0.0);
    
    for(i=0;i<a->h;i++){
        for(j=0;j<a->w;j++){
            for(q=0;q<a->k;q++){
                    float medium_value;
                    medium_value = (get_val(a,i,j,q) + get_val(b,i,j,q)) / 2.0;
                    set_val(pointer,i,j,q,medium_value);
            }
        }   
    }
    compute_stats(pointer);
    return pointer;
}

/* Crea una copia di una ip_mat e lo restituisce in output */
ip_mat * ip_mat_copy(ip_mat * in){/*rove*/
    unsigned int i, j, k;
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
    compute_stats(matr);
    return matr;
}

/* Inizializza una ip_mat con dimensioni w h e k.
 * Ogni elemento è generato da una gaussiana con media mean e deviazione std */
void ip_mat_init_random(ip_mat * t, float mean, float std){/*rove*/
    unsigned int i, j, k;
    if(t){
        for(i=0; i < t->h;i++){
            for(j = 0; j < t->w; j++){
                for(k = 0; k < t->k; k++){
                    /*Distribuzione normale moltiplicata per la varianza e sommata alla media*/
                    float val = get_normal_random(mean, std);
                    set_val(t, i, j, k, val);
                }
            }
        }
        compute_stats(t);
    }
    else{
        printf("ip_mat must be initialized first!\n");
        exit(1);
    }
}

/* Restituisce una sotto-matrice, ovvero la porzione individuata da:
 * t->data[row_start...row_end][col_start...col_end][0...k]
 * La terza dimensione la riportiamo per intero, stiamo in sostanza prendendo un sottoinsieme
 * delle righe e delle colonne.
 * */
ip_mat * ip_mat_subset(ip_mat * t, unsigned int row_start, unsigned int row_end, unsigned int col_start, unsigned int col_end){/*rove*/
    unsigned int numr, numc, i, j, k;
    
    /*Controllo che la sottomatrice abbia range accettabili*/
    if(row_start <= t->h && row_start < row_end && col_start <= t->w && col_start < col_end){
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
        compute_stats(submat);        
        return submat;
    }
    else{
        printf("ip_mat_subset range not acceptable\n");
        exit(1);
    }
}



/* Concatena due ip_mat su una certa dimensione.
 * Ad esempio:
 * ip_mat_concat(ip_mat * a, ip_mat * b, 0);
 *      produrrà un nuovo ip_mat di dimensioni:
 *      out.h = a.h + b.h
 *      out.w = a.w = b.w
 *      out.k = a.k = b.k
 *
 * ip_mat_concat(ip_mat * a, ip_mat * b, 1);
 *      produrrà un nuovo ip_mat di dimensioni:
 *      out.h = a.h = b.h
 *      out.w = a.w + b.w
 *      out.k = a.k = b.k
 *
 * ip_mat_concat(ip_mat * a, ip_mat * b, 2);
 *      produrrà un nuovo ip_mat di dimensioni:
 *      out.h = a.h = b.h
 *      out.w = a.w = b.w
 *      out.k = a.k + b.k
 * */

ip_mat * ip_mat_concat(ip_mat * a, ip_mat * b, int dimensione){/*chec*/
    unsigned int i,j,q,count=0,dim=0;
    ip_mat *result;
    result=NULL;
    
    switch(dimensione){
        case 0: /*CASE 0*/
            if(a->w == b->w && a->k == b->k){
                dim=(a->h)+(b->h);
                
                result=ip_mat_create(dim,a->w,a->k,0.);
                
                for(i=0;i<dim;i++){                    
                    for(j=0;j<a->w;j++){
                        for(q=0;q<a->k;q++){
                            if(i==a->h){
                                count=0;
                            }
                            if(i<a->h){
                                set_val(result,i,j,q,get_val(a,count,j,q));
                            }else{
                                set_val(result,i,j,q,get_val(b,count,j,q));
                            }
                        }
                    }
                    count++;
                }              
            }else{
                printf("Sizes of matrix must be the same\n");
                exit(1);
            }
            
            break;
        case 1:/*CASE 1*/
            
            if(a->h == b->h && a->k == b->k){
                
                dim=(a->w)+(b->w);
                result=ip_mat_create(a->h,dim,a->k,0.);
                
                for(q=0;q<a->k;q++){
                    for(i=0;i<a->h;i++){
                        count=0;
                        for(j=0;j<dim;j++){
                            if(j==a->w){
                                count=0;
                            }
                            if(j<a->w){
                                set_val(result,i,j,q,get_val(a,i,count,q));
                            }
                            else{
                                set_val(result,i,j,q,get_val(b,i,count,q));
                            }
                            count++;
                        }
                    }
                }
            }else{
                printf("Sizes of matrix must be the same\n");
                exit(1);
            }
            
            break;
        case 2:/*CASE 2*/
            if(a->h == b->h && a->w == b->w){
                dim=(a->k)+(b->k);
                result=ip_mat_create(a->h,a->w,dim,0.);
            
                for(i=0;i<a->h;i++){
                    count=0;
                    for(j=0;j<a->w;j++){
                        count=0;
                        for(q=0;q<dim;q++){
                            if(q==a->k){
                                count=0;
                            }
                            if(q<a->k){
                                set_val(result,i,j,q,get_val(a,i,j,count));
                            }else{
                                set_val(result,i,j,q,get_val(b,i,j,count));
                            }
                            count++;
                        }
                    }
                    
                }                
            }else{
                printf("Sizes of matrix must be the same\n");
                exit(1);
            }
            
            break;
        default:
            printf("Dimension must be between 0 and 2\n");
            exit(1);
    }
    compute_stats(result);   
    return result;
}

/**** PARTE 2: SEMPLICI OPERAZIONI SU IMMAGINI ****/
/* Operazione di brightening: aumenta la luminosità dell'immagine
 * aggiunge ad ogni pixel un certo valore*/
ip_mat * ip_mat_brighten(ip_mat * a, float bright){/*rove*/
    ip_mat *nuova;
    nuova = ip_mat_add_scalar(a, bright);
    return nuova;
}

/* Operazione di corruzione con rumore gaussiano:
 * Aggiunge del rumore gaussiano all'immagine, il rumore viene enfatizzato
 * per mezzo della variabile amount.
 * out = a + gauss_noise*amount
 * */
ip_mat * ip_mat_corrupt(ip_mat * a, float amount){/*rove*/
    unsigned int i, j, k;
    ip_mat *nuova;
    nuova = ip_mat_copy(a);
    for(i = 0; i < a->h; i++){
        for(j = 0; j < a->w; j++){
            for(k = 0; k < a->k; k++){
                float val = get_val(nuova, i, j, k);
                val += get_normal_random(0, amount / 2);
                set_val(nuova, i, j, k, val);
            }
        }
    }
    compute_stats(nuova);
    return nuova;
}


/* Converte un'immagine RGB ad una immagine a scala di grigio.
 * Quest'operazione viene fatta calcolando la media per ogni pixel sui 3 canali
 * e creando una nuova immagine avente per valore di un pixel su ogni canale la media appena calcolata.
 * Avremo quindi che tutti i canali saranno uguali.
 * */
ip_mat * ip_mat_to_gray_scale(ip_mat * in){/*faso*/
    unsigned int i, j, k;
    float media=0.0;
    ip_mat *matr;
    matr = ip_mat_create(in->h, in->w, in->k, 0.);
    
    for(i=0; i < in->h;i++){
        for(j = 0; j < in->w; j++){
            media=0.0;
            for(k = 0; k < in->k; k++){
                media+=get_val(in, i, j, k);
            }
            
            for(k = 0; k < in->k; k++){
                set_val(matr, i, j, k,media/3.);
            }
            
        }
    }
    compute_stats(matr);
    return matr;
}


/* Effettua la fusione (combinazione convessa) di due immagini */
ip_mat * ip_mat_blend(ip_mat * a, ip_mat * b, float alpha){/*faso*/
    unsigned int i,j,q;
    float blended = 0.0;
    ip_mat *c;
    check_dimensions(a, b);
    if(alpha >= 0 && alpha <= 1){
        c = ip_mat_create(a->h,a->w,a->k,0.0);
        
        for(i=0;i<a->h;i++){
            for(j=0;j<a->w;j++){
                for(q=0;q<a->k;q++){
                    blended = alpha*get_val(a,i,j,q)+((1-alpha)*get_val(b,i,j,q));
                    set_val(c,i,j,q,blended);
                }
            }   
        }
        compute_stats(c);
        return c;
    }
    else{
        printf("Alpha must be between 0 and 1!\n");
        exit(1);
    }
}

/**** PARTE 3: CONVOLUZIONE E FILTRI *****/

/* Aggiunge un padding all'immagine. Il padding verticale è pad_h mentre quello
 * orizzontale è pad_w.
 * L'output sarà un'immagine di dimensioni:
 *      out.h = a.h + 2*pad_h;
 *      out.w = a.w + 2*pad_w;
 *      out.k = a.k
 * con valori nulli sui bordi corrispondenti al padding e l'immagine "a" riportata
 * nel centro
 * */
ip_mat * ip_mat_padding(ip_mat * a, unsigned int pad_h, unsigned int pad_w){/*faso*/
    unsigned int i, j, k;
    ip_mat *matr;
    matr = ip_mat_create((a->h)+2*pad_h, (a->w)+2*pad_w, a->k, 0.);
    
    for(i=pad_h; i < (matr->h)-pad_h;i++){
        for(j = pad_w; j < (matr->w)-pad_w; j++){
            for(k = 0; k < matr->k; k++){
                set_val(matr, i, j, k,get_val(a,i-pad_h,j-pad_w,k));
            }
        }
    }
    compute_stats(matr);
    return matr;
}

/* Effettua una riscalatura dei dati tale che i valori siano in [0,new_max].
 * Utilizzate il metodo compute_stat per ricavarvi il min, max per ogni canale.
 *
 * I valori sono scalati tramite la formula valore-min/(max - min)
 *
 * Si considera ogni indice della terza dimensione indipendente, quindi l'operazione
 * di scalatura va ripetuta per ogni "fetta" della matrice 3D.
 * Successivamente moltiplichiamo per new_max gli elementi della matrice in modo da ottenere un range
 * di valori in [0,new_max].
 * */
void rescale(ip_mat * t, float new_max){/*rove*/
    unsigned int i, j, k;
    
    for(i = 0; i < t->h; i++){
        for(j = 0; j < t->w; j++){
            for(k = 0; k < t->k; k++){
                float val = get_val(t, i, j, k);
                val = (val - (t->stat)[k].min) / ((t->stat)[k].max - (t->stat)[k].min);
                val *= new_max;
                set_val(t, i, j, k, val);
            }
        }
    }
    compute_stats(t);
}

/* Nell'operazione di clamping i valori <low si convertono in low e i valori >high in high.*/
void clamp(ip_mat * t, float low, float high){/*rove*/
    unsigned int i, j, k;
    
    for(i = 0; i < t->h; i++){
        for(j = 0; j < t->w; j++){
            for(k = 0; k < t->k; k++){
                float val = get_val(t, i, j, k);
                if(val < low)
                    val = low;
                if(val > high)
                    val = high;
                set_val(t, i, j, k, val);
            }
        }
    }
    compute_stats(t);
}


/* Effettua la convoluzione di un ip_mat "a" con un ip_mat "f".
 * La funzione restituisce un ip_mat delle stesse dimensioni di "a".
 * */
ip_mat * ip_mat_convolve(ip_mat * a, ip_mat * f){/*tutti*/
    unsigned int i, j, k, r=0, c=0;
    float product;
    ip_mat *conv,*sub_mat,*extended;
    
    if(f->k == a->k){
        conv=ip_mat_create(a->h,a->w,a->k,0.);
    
        extended=ip_mat_padding(a,(f->h-1)/2,(f->w-1)/2);
        
        for(r=0;r< conv->h;r++){
            for(c=0;c<conv->w;c++){
                
                sub_mat=ip_mat_subset( extended,r,(f->h)+r ,c,(f->w)+c );
                
                for(k=0;k < sub_mat->k;k++){
                    product=0.0;
                    for(i = 0; i < sub_mat->h; i++){
                        for(j = 0; j < sub_mat->w; j++){
                            product += ( get_val(sub_mat,i,j,k)*get_val(f,i,j,k) );
                        }
                    }
                    set_val(conv,r,c,k,product);
                }
                
                ip_mat_free(sub_mat);
            }
        }
        
        ip_mat_free(extended);
        
        compute_stats(conv);
        return conv; 
    }else{
        printf("Sizes of matrix must be the same\n");
        exit(1);
    }
}

/* Crea un filtro di sharpening */
ip_mat * create_sharpen_filter(){/*faso*/
    
    int sharp_kernel[3][3] = { {0,-1,0},{-1,5,-1},{0,-1,0} };
    unsigned int i,j,k;
    ip_mat *sharpened;
    sharpened = ip_mat_create(3,3,3,0.0);
    
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            for(k=0;k<3;k++)
                set_val(sharpened,i,j,k,sharp_kernel[i][j]);
        
    compute_stats(sharpened);
    return sharpened;
    
}


ip_mat * create_emboss_filter(){/*chec*/
    ip_mat *filter;
    unsigned int i,j,k;
    
    int kernel[3][3] = {
    {-2, -1, 0},
    {-1, 1, 1},
    {0, 1, 2}
    };
    
    filter=ip_mat_create(3,3,3,0.);
    
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            for(k=0;k<3;k++)
                set_val(filter,i,j,k,kernel[i][j]);
            
    compute_stats(filter);
    return filter;
}


/* Crea un filtro medio per la rimozione del rumore */
ip_mat * create_average_filter(unsigned int h, unsigned int w, unsigned int k){/*faso*/
    unsigned int i,j,q;
    ip_mat *average;
    average = ip_mat_create(w,h,k,0.0);
    
    for(i=0;i<h;i++)
        for(j=0;j<w;j++)
            for(q=0;q<k;q++)
                set_val(average,i,j,q, (1./(w*h)) );
            
    compute_stats(average);        
    return average;
}

/* Crea un filtro per rilevare i bordi */
ip_mat * create_edge_filter(){/*rove*/
    /*  -1  -1  -1
        -1   8  -1
        -1  -1  -1*/
    unsigned int i;
    ip_mat *kernel;
    kernel = ip_mat_create(3, 3, 3, -1);
    
    for(i=0;i<3;i++)
        set_val(kernel, 1, 1, i, 8);
    
    compute_stats(kernel);
    return kernel;
}


ip_mat * create_gaussian_filter(unsigned int h, unsigned int w, unsigned int k, float sigma){/*tutti*/
        
    unsigned int i,j,q;
    int cx,cy,x,y;
    
    float gauss,sum;
    
    ip_mat *gaussian;
    gaussian = ip_mat_create(w,h,k,0.0);
    cx=(w-1)/2;
    cy=(h-1)/2;
    
    for(q=0;q<k;q++){
        sum=0;
        for(i=0;i<h;i++){
            for(j=0;j<w;j++){
                x=i-cx;
                y=j-cy;
                gauss=(1./(2.*PI*sigma*sigma))*exp((-(x*x + y*y) / (2.*sigma * sigma)));
                sum += gauss;
                set_val(gaussian,i,j,q, gauss);
            }
        }
        for(i=0;i<h;i++){
            for(j=0;j<w;j++){
                gauss=(get_val(gaussian,i,j,q))/sum;
                set_val(gaussian,i,j,q, gauss);
            }
        }
    }
    compute_stats(gaussian);  
    return gaussian;
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

    compute_stats(t);

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

float get_normal_random(float media, float std){

    float y1 = ( (float)(rand()) + 1. )/( (float)(RAND_MAX) + 1. );
    float y2 = ( (float)(rand()) + 1. )/( (float)(RAND_MAX) + 1. );
    float num = cos(2*PI*y2)*sqrt(-2.*log(y1));

    return media + num*std;
}
