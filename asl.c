#include "/usr/include/uarm/uARMconst.h"
#include "/usr/include/uarm/uARMtypes.h"
#include "/usr/include/uarm/libuarm.h"
#include "pcb.h"
#include "asl.h"
semd_t semd_table[MAXPROC]; /*array dei SEMD con dimensione massima MAXPROC*/
semd_t* semdFree_h = NULL;  /*puntatore alla lista di SEMD liberi*/
semd_t* semd_h=NULL;  /*puntatore alla lista di SEMD attivi*/
int initialized_semd=0; /*intero globale di supporto utilizzato per l'initASL()*/

/* ASL handling functions */
 
void initASL()
{
    if (initialized_semd != MAXPROC-1) /*inizialmente initialized_semd vale 0*/
    {
        semd_table[initialized_semd].s_next = &semd_table[initialized_semd+1]; /*il campo "next" di ogni elemento della semd_table                                          punta all'elemento successivo del vettore */
        initialized_semd++;
        initASL(); /*initASL viene richiamata ricorsivamente incrementando initalized_semd in modo da collegare tutti gli elementi              della semd_table*/
    }
    else { semd_table[MAXPROC-1].s_next = NULL;
        semdFree_h = &semd_table[0]; /*semdFreeh punta al primo elemento della semd_table che è collegato a tutti gli altri. La lista                         degli elementi liberi contiene ora l'intero
                                      array di semafori*/
    }
}
void insertASL(semd_t*ptr) /*Funzione di supporto alla insertBlocked: dato un descrittore di SEMD puntato da ptr, lo inserisce in                   testa alla lista dei SEMD attivi*/
{
        semd_t*tmp=semd_h;
        semd_h=ptr;
        semd_h->s_next=tmp;
        return;
}
 
void enqueue(pcb_t**qroot,pcb_t*ptr,int*key) /*Funzione di supporto alla insertBlocked: dato l'indirizzo di un puntatore a pcb_t                              (qroot), un puntatore a pcb_t e un puntatore ad intero,
                                              scorre la lista di pcb_t puntata da *qroot fino alla fine, inserendo in coda il pcb_t                               puntato da ptr, a cui verrà dato come campo p_semAdd il valore di key*/
{
    if ((*qroot)==NULL) /*coda vuota o terminata*/
    {
        *qroot=ptr;
        (*qroot)->p_semAdd=key;
        (*qroot)->p_next=NULL;
    }
    else enqueue(&((*qroot)->p_next),ptr,key);
}
semd_t* supportSemd(int *key,semd_t* tmp) /*Funzione di supporto alla getSemd: prende in input un puntatore ad intero key e un                         puntatore a semd_t tmp inizialmente settato
                                           alla radice della lista dei SEMD attivi. La funzione scorre ricorsivamente l'intera lista                           alla ricerca dell'elemento con chiave
                                           key ritornando NULL se tale elemento non esiste*/
{
    if (tmp==NULL) /*lista vuota o terminata*/
    return NULL;
    else if  (tmp->s_semAdd==key) /*elemento trovato. Ritorno tmp*/
    {
    return tmp;
    }
    else
    {
    tmp=tmp->s_next;
    return supportSemd(key,tmp);
    }
}
semd_t* getSemd(int *key)//funzione di supporto generale
{
    semd_t* tmp=semd_h;
    tmp=supportSemd(key,tmp);
    return tmp;
} 
int insertBlocked(int *key,pcb_t* p)
{
    semd_t*tmp=getSemd(key); /*assegno a tmp un puntatore all'eventuale semaforo avente chiave key*/
    if (tmp!=NULL) /*il semaforo avente chiave key esiste*/
    {
        enqueue(&(tmp->s_procQ),p,key); /*aggiungo alla coda dei processi bloccati su quel semaforo il processo puntato da p*/
        return(0);
    }
    else if (semdFree_h!=NULL) /*ci sono ancora semafori liberi disponibili*/
    {
    tmp=semdFree_h; /*assegno a tmp il primo elemento della lista dei semd liberi*/
    semdFree_h=semdFree_h->s_next; /*aggiorno la  lista dei semd liberi rimuovendo l'elemento puntato da tmp*/
    tmp->s_semAdd=key;
    tmp->s_procQ=NULL;
    insertASL(tmp); /*aggiungo tmp ai semd attivi*/
    enqueue(&(tmp->s_procQ),p,key);
    return(0);
    }
    else return (1); /*allocazione impossibile: tutti i semafori sono utilizzati*/
}
 
void seek_and_destroy(int *key) /*Funzione di supporto alla removeBlocked: dato un puntatore ad intero key, cerca nella lista dei                    semafori attivi il semaforo puntato da key e,
                                 se lo trova, lo rimuove dalla lista inserendolo nella lista dei semd liberi*/
{
    if(semd_h->s_semAdd==key) /*semaforo trovato*/
    {
        semd_t*tmp=semd_h;
        semd_h=semd_h->s_next; /*rimuovo il semd da semd_h*/
        tmp->s_next=semdFree_h; /*aggiungo il semd alla semdFree*/
        semdFree_h=tmp;
        return;
    }
    else if(semd_h->s_next!=NULL && semd_h->s_next->s_semAdd==key) /*semaforo cercato è il next del semd_h attuale*/
    {
        semd_t*tmp=semd_h->s_next;
        semd_h->s_next=tmp->s_next; /*rimuovo il semd da semd_h*/
        tmp->s_next=semdFree_h; /*aggiungo il semd alla semdFree*/
        semdFree_h=tmp;
    }
    else
    {
        semd_h=semd_h->s_next; /*scorro la lista*/
        seek_and_destroy(key);
    }
}
 
void supportRemove(semd_t*tmp,int*key) /*funzione di supporto alla removeBlocked: controlla se in testa alla lista puntata da tmp
                        è presente il semaforo con chiave key. Se lo trova lo elimina, altrimenti chiama la
                    seek_and_destroy che lo cerca all'interno della lista dei semd attivi, ripristinando
                    poi il valore iniziale di semd_h contenuto in tmp*/
{
    if (tmp->s_semAdd==key) /*l'elemento da eliminare è quello puntato da tmp*/
    {
        semd_t*tp=semd_h;
        semd_h=semd_h->s_next; /*rimuovo il semd da semd_h*/
        tp->s_next=semdFree_h; /*aggiungo il semd alla semdFree*/
        semdFree_h=tp;
     
    }
    else
    {
    seek_and_destroy(key); /*cerco il semaforo nella lista semd_h*/
    semd_h=tmp; /*ripristino il valore originale di semd_h, che è stato modificato dalla seek_and_destroy*/
         
    }
}
 
pcb_t *removeBlocked(int *key)
{
    semd_t*tmp=getSemd(key);
    if (tmp==NULL){
        return NULL;}
    else /*il semaforo con chiave key esiste*/
    {
        pcb_t *ptr=tmp->s_procQ; /*assegno a ptr il primo processo della coda dei processi bloccati su tmp*/
        tmp->s_procQ=(tmp->s_procQ)->p_next; /*rimuovo ptr dai processi bloccati su tmp*/
        if (tmp->s_procQ==NULL) /*la coda dei processi si è svuotata*/
        {
            semd_t *temp=semd_h;
            supportRemove(temp,key);
        }
        return ptr;
    }
}
 
 
pcb_t* removeProc(pcb_t**root,pcb_t*ptr) /*Funzione di supporto alla OutBlocked: dato l'indirizzo della radice di una lista di pcb_t e
                       un puntatore a pcb_t ptr, cerca il pcb_t puntato da ptr nella lista avente radice in *root e,
                           se lo trova, lo elimina*/
{
    if(*root==NULL) return NULL;
    else if (*root==ptr)  /*elemento trovato*/
    {
    *root=(*root)->p_next;
    return ptr;
    }
    else removeProc(&((*root)->p_next),ptr); /*scorre la lista*/
}
 
 
pcb_t* outBlocked(pcb_t *p)
{
    semd_t*tmp=getSemd(p->p_semAdd);
    if (tmp!=NULL)
    {
        pcb_t*res=removeProc(&(tmp->s_procQ),p);
        return res;
    }  
    else return NULL;
}
 
pcb_t *headBlocked(int *key)
{
    semd_t*tmp=getSemd(key);
    if (tmp==NULL)
        return NULL;
    else return tmp->s_procQ;
}
 
     
