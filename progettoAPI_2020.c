#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 512 // definzione del valore massimo arrai stringhe in ingresso caratteri massimo di 1024 nel caso modifica
#define START 20 // comando in ingresso formato da ind1,ind2,carattere
#define DEFAULT -1874919423
#define SALVACOMANDI 300
#define INCREMENTO 1000000 // usata per incrementare la dimensione della memoria array per salvataggio di memori

/*DEFINIZONE DELLA STRUTTURA DATI PER LE RIGHE*/
typedef struct {
    char *stringa;
}riga_t;
typedef struct{
    riga_t *stringa;
    int used;
    int size;
}array_riga;
void initArrayRiga(array_riga *a){
    a->size = INCREMENTO;
    a->used = 0;
    a->stringa = (riga_t*)malloc(sizeof(riga_t)*INCREMENTO);
}
void insertRiga(array_riga *a,riga_t riga){
    if (a->used == a->size) {
        a->size = INCREMENTO + a->size;
        a->stringa = (riga_t*)realloc(a->stringa,a->size*sizeof(riga_t));
    }
    a->stringa[a->used].stringa=riga.stringa;
    a->used++;
}
/*DEFINIZIONE STRUTTURA DELLA MIA LISTA*/
typedef struct node_s{
    riga_t *string;
    struct node_s *next;
    struct node_s *prev;
}node_t;
struct node_s *head;
struct node_s *tail;
typedef node_t *listaStringhe;
/*DEFINIZIONE STRUTTURA PER LE UNDO*/
typedef struct {
    int DimensioneLista;
    int primoIndirizzo;
    int secondoIndirizzo;
    char comando;
    struct node_s *tail;
    struct node_s *head;
}input_t;

typedef struct{
    input_t *input;
    int used;
    int size;
}array_t;
void initArray(array_t *a){
    a->size = 2;
    a->input = (input_t *)malloc(2 * sizeof(input_t));
    a->used = 0;
}
void insertArray(array_t *a, input_t ingresso){
    if (a->used == a->size) {
        a->size = INCREMENTO + a->size;
        a->input = (input_t *)realloc(a->input,a->size * sizeof(input_t));
    }
    a->input[a->used].DimensioneLista = ingresso.DimensioneLista;
    a->input[a->used].primoIndirizzo = ingresso.primoIndirizzo;
    a->input[a->used].secondoIndirizzo = ingresso.secondoIndirizzo;
    a->input[a->used].comando = ingresso.comando;
    a->input[a->used].head = ingresso.head;
    a->input[a->used].tail = ingresso.tail;
    a->used++;
}
/*DEFINZIONE DELLA STRUTTURA PER IL SALVATAGGIO DEGLI STATI*/
typedef struct{
    int dimensioneLista;
    int operazioneEseguita;
    struct node_s *tail;
    struct node_s *head;
} input_stato;
typedef struct{
    input_stato *input;
    int used;
    int size;
} array_stato;
void initArrayStato (array_stato *a){
    a->size = INCREMENTO;
    a->input = (input_stato *)malloc(INCREMENTO*sizeof(input_stato));
    a->used = 0;
}
void insertArrayStato(array_stato *a, input_stato ingresso){
    if (a->used == a->size) {
        a->size = INCREMENTO + a->size;
        a->input = (input_stato *)realloc(a->input,a->size * sizeof(input_stato));
    }
    a->input[a->used].dimensioneLista = ingresso.dimensioneLista;
    a->input[a->used].operazioneEseguita = ingresso.operazioneEseguita;
    a->input[a->used].head = ingresso.head;
    a->input[a->used].tail = ingresso.tail;
    a->used++;
}

/*FUNZIONI DA USARE NEL PROGRAMMA*/

listaStringhe insertTesto(listaStringhe lista,array_riga *array);
void sostituisciStringa(listaStringhe *lista,int posizione,input_t *input,array_riga *array);
void creaCopiaLista(listaStringhe *lista,input_stato *input,int dimensioneLista);
void stampaOutput(listaStringhe lista,int primoIndirizzo,int secondoIndirizzo,int dimensioneLista);
void delete(listaStringhe *lista,int primoIndirizzo,int secondoIndirizzo,int *dimensioneLista,input_t *input);
void eseguiUndoRedo(listaStringhe *lista,array_t *memoriaUndo,int *totale,int *dimensioneLista,int *posizioneCorrente,int conteggioOperazioni, array_stato *memoriaStato,array_stato *statoAggiuntivo);

int main(int argc, const char * argv[]) {
    /* DEFINZIONE DELLE VARIABLI GENERALI */
    char comandoEsecuzione[START];
    char stringa [MAX];
    static int primoIndirizzo;
    static int secondoIndirizzo;
    char comando = '\0';
    const char s[2] = ",";
    int dimensioneLista = 0;
    int conteggioOperazioni = 0;
    int totale = DEFAULT;
    char comandoPrecedente = '\0';
    int posizioneAggiornata = 0;
    int conteggioOperazioniStato = '\0';
    int controllo = 1;
    int dimensioneUndo = 0;
    /* DEFINZIONE DELLE MEMROIE UTILIZZATE */
    array_riga arrayRighe;
    initArrayRiga(&arrayRighe);
    riga_t ingressoRiga;
    listaStringhe testo = NULL;
    array_t arrayMemoriaUndo;
    initArray(&arrayMemoriaUndo);
    input_t inputUndo;
    array_stato arraySalvataggioStato;
    initArrayStato(&arraySalvataggioStato);
    input_stato inputSalvataggioStato;
    array_stato salvataggioStatoAggiuntivo;
    initArrayStato(&salvataggioStatoAggiuntivo);
    head = NULL;
    tail = NULL;
    while (comando != 'q') {
        //node_t *newNode = NULL;
        inputUndo.head = NULL;
        inputUndo.tail = NULL;
        inputSalvataggioStato.head = NULL;
        inputSalvataggioStato.tail = NULL;
        primoIndirizzo = 0;
        secondoIndirizzo = 0;
        dimensioneUndo = 0;
        memset(comandoEsecuzione,0,strlen(comandoEsecuzione));
        if(fgets(comandoEsecuzione,START,stdin) == NULL) return 1;
        /* FUNZIONE PER DIVIDERE IL COMANDO NELLE SUE COMPONENTI */
        char *token;
        int lunghezza = strlen(comandoEsecuzione);
        token = strtok(comandoEsecuzione,s);
        primoIndirizzo = atoi(token);
        int lunghezzaToken = strlen(token);
        comando = comandoEsecuzione[lunghezza - 2];
        comandoEsecuzione[lunghezza-2] = '\n';
        secondoIndirizzo = atoi(&comandoEsecuzione[lunghezzaToken+1]);
        switch (comando) {
            case 'c':
                if (comandoPrecedente == 'r' || comandoPrecedente == 'u') {
                    if(totale != DEFAULT) eseguiUndoRedo(&testo,&arrayMemoriaUndo,&totale,&dimensioneLista,&posizioneAggiornata,conteggioOperazioni,&arraySalvataggioStato,&salvataggioStatoAggiuntivo);
                    conteggioOperazioni = posizioneAggiornata;
                    arrayMemoriaUndo.used = conteggioOperazioni;
                    int ultimaPossSalvata = 0;
                    int check = 0;
                    for (int i = 0;i < arraySalvataggioStato.used ;i++) {
                        if (arraySalvataggioStato.input[i].operazioneEseguita >= posizioneAggiornata) {
                            if ((posizioneAggiornata % SALVACOMANDI == 0) && (posizioneAggiornata != 0)) {
                                ultimaPossSalvata = arraySalvataggioStato.input[i].operazioneEseguita;
                                arraySalvataggioStato.used = i+1;
                                check = 1;
                                break;
                            }
                            if (i == 0) {
                                ultimaPossSalvata = 0;
                            }else{
                                ultimaPossSalvata = arraySalvataggioStato.input[i-1].operazioneEseguita;
                            }
                            arraySalvataggioStato.used = i;
                            check = 1;
                            break;
                        }
                    }
                    if (check == 1) {
                        conteggioOperazioniStato = posizioneAggiornata - ultimaPossSalvata;
                    }
                    controllo = 1;
                }
                posizioneAggiornata ++;
                conteggioOperazioni++;
                conteggioOperazioniStato ++;
                for (int i = primoIndirizzo; i <= secondoIndirizzo; i++) {
                    if (fgets(stringa,MAX,stdin) == NULL) return 1;
                    ingressoRiga.stringa = (char*)malloc(sizeof(char)*strlen(stringa)+1);
                    strcpy(ingressoRiga.stringa,stringa);
                    insertRiga(&arrayRighe,ingressoRiga);
                    if (dimensioneLista >= i ) {
                        sostituisciStringa(&testo,i,&inputUndo,&arrayRighe);
                        dimensioneUndo++;
                        continue;
                    }
                    testo = insertTesto(testo,&arrayRighe);
                }
                if (secondoIndirizzo > dimensioneLista) dimensioneLista = secondoIndirizzo;
                inputUndo.primoIndirizzo = primoIndirizzo;
                inputUndo.secondoIndirizzo = secondoIndirizzo;
                inputUndo.comando = comando;
                inputUndo.DimensioneLista = dimensioneUndo;
                insertArray(&arrayMemoriaUndo,inputUndo);
                if (conteggioOperazioniStato == SALVACOMANDI) {
                    creaCopiaLista(&testo,&inputSalvataggioStato,dimensioneLista);
                    inputSalvataggioStato.operazioneEseguita = conteggioOperazioni;
                    inputSalvataggioStato.dimensioneLista = dimensioneLista;
                    insertArrayStato(&arraySalvataggioStato,inputSalvataggioStato);
                    conteggioOperazioniStato = 0;
                }
                if (fgets(stringa,MAX,stdin) == NULL) return 1;
                break;

            case 'd':
                if (comandoPrecedente == 'r' || comandoPrecedente == 'u') {
                    if(totale != DEFAULT) eseguiUndoRedo(&testo,&arrayMemoriaUndo,&totale,&dimensioneLista,&posizioneAggiornata,conteggioOperazioni,&arraySalvataggioStato,&salvataggioStatoAggiuntivo);
                    conteggioOperazioni = posizioneAggiornata;
                    arrayMemoriaUndo.used = conteggioOperazioni;
                    int ultimaPossSalvata = 0;
                    int check = 0;
                    for (int i = 0;i < arraySalvataggioStato.used ;i++) {
                        if (arraySalvataggioStato.input[i].operazioneEseguita >= posizioneAggiornata) {
                            if ((posizioneAggiornata % SALVACOMANDI == 0) && (posizioneAggiornata != 0)) {
                                ultimaPossSalvata = arraySalvataggioStato.input[i].operazioneEseguita;
                                arraySalvataggioStato.used = i+1;
                                check = 1;
                                break;
                            }
                            if (i == 0) {
                                ultimaPossSalvata = 0;
                            }else{
                                ultimaPossSalvata = arraySalvataggioStato.input[i-1].operazioneEseguita;
                            }
                            arraySalvataggioStato.used = i;
                            check = 1;
                            break;
                        }
                    }
                    if (check == 1) {
                        conteggioOperazioniStato = posizioneAggiornata - ultimaPossSalvata;
                    }
                    controllo = 1;
                }
                posizioneAggiornata++;
                conteggioOperazioni ++;
                conteggioOperazioniStato++;
                delete(&testo,primoIndirizzo,secondoIndirizzo,&dimensioneLista,&inputUndo);
                if (primoIndirizzo == 0) inputUndo.primoIndirizzo = 1;
                else inputUndo.primoIndirizzo = primoIndirizzo;
                inputUndo.secondoIndirizzo = secondoIndirizzo;
                inputUndo.comando = comando;
                if (inputUndo.head == NULL) inputUndo.DimensioneLista = 0;
                insertArray(&arrayMemoriaUndo,inputUndo);
                if (conteggioOperazioniStato == SALVACOMANDI) {
                    creaCopiaLista(&testo,&inputSalvataggioStato,dimensioneLista);
                    inputSalvataggioStato.operazioneEseguita = conteggioOperazioni;
                    inputSalvataggioStato.dimensioneLista = dimensioneLista;
                    insertArrayStato(&arraySalvataggioStato,inputSalvataggioStato);
                    conteggioOperazioniStato = 0;
                }
                break;
            case 'p':
                if(totale != DEFAULT) eseguiUndoRedo(&testo,&arrayMemoriaUndo,&totale,&dimensioneLista,&posizioneAggiornata,conteggioOperazioni,&arraySalvataggioStato,&salvataggioStatoAggiuntivo);
                stampaOutput(testo,primoIndirizzo,secondoIndirizzo,dimensioneLista);
                break;
            case 'u':
                if (((arrayMemoriaUndo.used % SALVACOMANDI != 0)||(arrayMemoriaUndo.used < SALVACOMANDI)) && (controllo == 1)) {
                    controllo = 0;
                    creaCopiaLista(&testo,&inputSalvataggioStato,dimensioneLista);
                    inputSalvataggioStato.operazioneEseguita = conteggioOperazioni;
                    inputSalvataggioStato.dimensioneLista = dimensioneLista;
                    insertArrayStato(&arraySalvataggioStato,inputSalvataggioStato);
                    conteggioOperazioniStato = 0;
                }
                if (totale == DEFAULT) totale = -primoIndirizzo;
                else totale = totale-primoIndirizzo;
                if ((posizioneAggiornata + totale)<=0) totale = -posizioneAggiornata;
                break;
            case 'r':
                if (totale == DEFAULT) totale = primoIndirizzo;
                else totale = totale +primoIndirizzo;
                if ((totale + posizioneAggiornata >= conteggioOperazioni)) totale = conteggioOperazioni - posizioneAggiornata;
                break;
        }
        if (comando != 'p') comandoPrecedente = comando;
    }
    return 0;
}

/*FUNZIONE PER INSERIRE RIGHE IN TESTO*/
listaStringhe insertTesto(listaStringhe lista,array_riga *array){
    listaStringhe newNode = malloc(sizeof(node_t));
    newNode->string = &array->stringa[array->used-1];
    newNode->next = NULL;
    if (lista == NULL) {
        newNode->prev = NULL;
        head = newNode;
        tail = newNode;
        return newNode;
    }else{
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
        return lista;
    }
}
/* FUNZIONE PER LA SOSTITUZIONE DI UN ELEMENTO */
void sostituisciStringa(listaStringhe *lista,int posizione,input_t *input,array_riga *array){
    listaStringhe newNode = malloc(sizeof(node_t)),testa = (*lista);
    newNode->next = NULL;
    for (int i = 1;i<posizione;i++) testa = testa->next;
    newNode->string = testa->string;
    testa->string = &array->stringa[array->used-1];
    if (input->head == NULL) {
        input->head = newNode;
        input->tail = newNode;
        return;
    }else{
        newNode->prev = input->tail;
        input->tail->next = newNode;
        input->tail = newNode;
        return;
    }
}
/*FUNZIONE PER CREARE UNA COPIA DI PUNTATORI ALLA MIA LISTA*/
void creaCopiaLista(listaStringhe *lista,input_stato *input,int dimensioneLista){
    listaStringhe testa = (*lista);
    node_t *newNode = (node_t*)malloc(sizeof(node_t)*dimensioneLista);
    for (int i = 0;i<dimensioneLista;i++) {
        newNode[i].string = testa->string;
        newNode[i].next = NULL;
        if (input->head == NULL) {
            newNode[i].prev = NULL;
            input->head = &newNode[i];
            input->tail = &newNode[i];
        }else{
            newNode[i].prev = input->tail;
            input->tail->next = &newNode[i];
            input->tail = &newNode[i];
        }
        testa = testa->next;
    }
}
/*FUNZIONE PER STAMAPRE*/
void stampaOutput(listaStringhe lista,int primoIndirizzo,int secondoIndirizzo,int dimensioneLista){
    listaStringhe testa = lista;
    if (primoIndirizzo == 0) {
        fputs(".\n",stdout);
        primoIndirizzo++;
    }
    if (primoIndirizzo < dimensioneLista/2) {
      for (int i = 1;i<primoIndirizzo;i++){
          if(testa == NULL) break;
          testa = testa->next;
      }
    }else{
      testa = tail;
      for (int i = dimensioneLista;i>primoIndirizzo;i--){
          if(testa == NULL) break;
          testa = testa->prev;
      }

    }
    for (int i = primoIndirizzo;i<= secondoIndirizzo;i++) {
        if (testa == NULL) {
            fputs(".\n",stdout);
            continue;
        }
        fputs(testa->string->stringa,stdout);
        testa = testa->next;
    }
}
/* FUNZIONE PER OPERAZIONE DI DELETE*/
void delete(listaStringhe *lista,int primoIndirizzo,int secondoIndirizzo,int *dimensioneLista,input_t *input){
    listaStringhe testa = (*lista);
    if ((*lista) == NULL || secondoIndirizzo == 0) return;
    if (primoIndirizzo == 0) primoIndirizzo ++;
    if (primoIndirizzo > (*dimensioneLista))return;
    if (secondoIndirizzo >= (*dimensioneLista)) {
        if (primoIndirizzo == 1) {
            (*lista) = NULL;
            input->head = head;
            input->tail = tail;
            head = NULL;
            tail = NULL;
            input->DimensioneLista = (*dimensioneLista);
            (*dimensioneLista) = 0;
            return;
        }else{
            for (int i=1;i<primoIndirizzo;i++) testa = testa->next;
            input->head = testa;
            input->tail = tail;
            tail = testa->prev;
            testa->prev->next = NULL;
            testa->prev = NULL;
            input->DimensioneLista = (*dimensioneLista)-primoIndirizzo+1;
            (*dimensioneLista) = (*dimensioneLista)-((*dimensioneLista)-primoIndirizzo+1);
            return;
        }
    }
    if (secondoIndirizzo < (*dimensioneLista)) {
        for (int i=1;i<primoIndirizzo;i++) testa = testa->next;
        if (primoIndirizzo == 1 && primoIndirizzo == secondoIndirizzo) {
            input->head = testa;
            input->tail = testa;
            testa->next->prev = NULL;
            head = testa->next;
            testa->next = NULL;
            (*lista) = head;
            input->DimensioneLista = 1;
            (*dimensioneLista) = (*dimensioneLista) - 1;
            return;
        }
        if (primoIndirizzo == 1 && primoIndirizzo != secondoIndirizzo) {
            listaStringhe secondoElemento = testa;
            for (int i = primoIndirizzo;i<secondoIndirizzo; i++) secondoElemento = secondoElemento->next;
            input->head = testa;
            input->tail = secondoElemento;
            secondoElemento->next->prev = NULL;
            head = secondoElemento->next;
            secondoElemento->next = NULL;
            testa->prev = NULL;
            (*lista) = head;
            input->DimensioneLista = (secondoIndirizzo-primoIndirizzo+1);
            (*dimensioneLista) = (*dimensioneLista) - (secondoIndirizzo-primoIndirizzo+1);
            return;
        }
        if (primoIndirizzo != 1 && primoIndirizzo == secondoIndirizzo) {
            testa->prev->next = testa->next;
            testa->next->prev = testa->prev;
            testa->next = NULL;
            testa->prev = NULL;
            input->head = testa;
            input->tail = testa;
            (*lista) = head;
            input->DimensioneLista = 1;
            (*dimensioneLista) = (*dimensioneLista) - 1;
            return;
        }
        if (primoIndirizzo != 1 && primoIndirizzo != secondoIndirizzo) {
            listaStringhe secondoElemento = testa;
            for (int i=primoIndirizzo;i<secondoIndirizzo;i++) secondoElemento = secondoElemento->next;
            testa->prev->next = secondoElemento->next;
            secondoElemento->next->prev = testa->prev;
            testa->prev = NULL;
            secondoElemento->next = NULL;
            input->head = testa;
            input->tail = secondoElemento;
            (*lista) = head;
            input->DimensioneLista = secondoIndirizzo - primoIndirizzo + 1;
            (*dimensioneLista) = (*dimensioneLista) - (secondoIndirizzo - primoIndirizzo + 1);
            return;
        }
    }
}

/*OPERAZIONE DI UNDO E REDO*/
void eseguiUndoRedo(listaStringhe *lista,array_t *memoriaUndo,int *totale,int *dimensioneLista,int *posizioneCorrente,int conteggioOperazioni, array_stato *memoriaStato,array_stato *statoAggiuntivo){
    int check = 0;
    int cellaPartenza = 0;
    int cellaDestinazione = 0;
    int dimensioneMemoria = 0;
    node_t *node = NULL;
    listaStringhe listaAusiliaria = NULL;
    if ((*totale) == 0) {
        (*totale) = DEFAULT;
        return;
    }
    if (*totale < 0) {
        cellaPartenza = *posizioneCorrente;
        cellaDestinazione = *posizioneCorrente - abs(*totale);
        statoAggiuntivo->used = 0;
        input_stato inputStatoAggiuntivo;
        inputStatoAggiuntivo.head = NULL;
        inputStatoAggiuntivo.tail = NULL;
        inputStatoAggiuntivo.operazioneEseguita = (*posizioneCorrente);
        creaCopiaLista(lista,&inputStatoAggiuntivo,*dimensioneLista);
        inputStatoAggiuntivo.dimensioneLista = *dimensioneLista;
        insertArrayStato(statoAggiuntivo,inputStatoAggiuntivo);
    }
    if (*totale > 0) {
        cellaDestinazione = *posizioneCorrente + *totale;
        cellaPartenza = memoriaStato->input[memoriaStato->used-1].operazioneEseguita;
        listaAusiliaria = memoriaStato->input[memoriaStato->used-1].head;
        dimensioneMemoria = memoriaStato->input[memoriaStato->used-1].dimensioneLista;
        check=1;
        if (cellaDestinazione >= conteggioOperazioni) {
            (*posizioneCorrente) = conteggioOperazioni;
            if (memoriaStato->input[memoriaStato->used-1].head == NULL) {
                *lista = NULL;
                head = NULL;
                tail = NULL;
                *dimensioneLista = 0;
                *totale = DEFAULT;
                //liberare quelllo che cera nel corpo della lista??
                return;
            }else{
                listaAusiliaria = memoriaStato->input[memoriaStato->used -1].head;
                if (*lista == NULL) {
                    head = NULL;
                    tail = NULL;
                    node = (node_t*)malloc(sizeof(node_t)*dimensioneMemoria);
                    for (int i = 0;i<dimensioneMemoria;i++) {
                        node[i].string = listaAusiliaria->string;
                        node[i].next = NULL;
                        if (head == NULL) {
                            node[i].prev = NULL;
                            head = &node[i];
                            tail = &node[i];
                            (*lista) = &node[i];
                        }else{
                            node[i].prev = tail;
                            tail->next = &node[i];
                            tail = &node[i];
                        }
                        listaAusiliaria = listaAusiliaria->next;
                    }
                    *lista = head;
                    (*dimensioneLista) = memoriaStato->input[memoriaStato->used-1].dimensioneLista;
                    *totale = DEFAULT;
                    return;
                }
                listaStringhe testa = (*lista);
                while (listaAusiliaria != NULL) {
                    if (testa == NULL) {
                        listaStringhe newNode;
                        newNode = malloc(sizeof(node_t));
                        newNode->string = listaAusiliaria->string;
                        newNode->next = NULL;
                        tail->next = newNode;
                        newNode->prev = tail;
                        tail = newNode;
                        testa = newNode;
                    }else testa->string = listaAusiliaria->string;
                    listaAusiliaria = listaAusiliaria->next;
                    testa = testa->next;
                }
                if (testa != NULL) {
                    tail = testa->prev;
                    testa->prev->next = NULL;
                    testa->prev = NULL;
                }
                *dimensioneLista = memoriaStato->input[memoriaStato->used-1].dimensioneLista;
                *totale = DEFAULT;
                return;
            }
        }
    }
    if (memoriaStato->used > 1) {
        for (int i=0;i<memoriaStato->used-1;i++) {
            if (cellaDestinazione <= memoriaStato->input[i].operazioneEseguita) {
                cellaPartenza = memoriaStato->input[i].operazioneEseguita;
                dimensioneMemoria = memoriaStato->input[i].dimensioneLista;
                if (memoriaStato->input[i].head == NULL) listaAusiliaria = NULL;
                else listaAusiliaria = memoriaStato->input[i].head;
                check = 1;
                break;
            }
        }
    }
    for (int i = 0;i<statoAggiuntivo->used;i++) {
        if (cellaDestinazione<= statoAggiuntivo->input[i].operazioneEseguita && statoAggiuntivo->input[i].operazioneEseguita < cellaPartenza) {
            cellaPartenza = statoAggiuntivo->input[i].operazioneEseguita;
            dimensioneMemoria = statoAggiuntivo->input[i].dimensioneLista;
            if (statoAggiuntivo->input[i].head == NULL) listaAusiliaria = NULL;
            else listaAusiliaria = statoAggiuntivo->input[i].head;
            check = 1;
            break;
        }
    }
    if (check == 1) {
        if (listaAusiliaria == NULL) {
            *lista = NULL;
            head = NULL;
            tail = NULL;
            *dimensioneLista = 0;
        }else{
            if (*lista == NULL) {
                head = NULL;
                tail = NULL;
                node = (node_t*)malloc(sizeof(node_t)*dimensioneMemoria);
                for (int i = 0;i<dimensioneMemoria;i++) {
                    node[i].string = listaAusiliaria->string;
                    node[i].next = NULL;
                    if (head == NULL) {
                        node[i].prev = NULL;
                        head = &node[i];
                        tail = &node[i];
                        (*lista) = &node[i];
                    }else{
                        node[i].prev = tail;
                        tail->next = &node[i];
                        tail = &node[i];
                    }
                    listaAusiliaria = listaAusiliaria->next;
                }
                *lista = head;
                *dimensioneLista = dimensioneMemoria;
            }else{
                listaStringhe testa = (*lista);
                while (listaAusiliaria != NULL) {
                    if (testa == NULL) {
                        listaStringhe newNode;
                        newNode = malloc(sizeof(node_t));
                        newNode->string = listaAusiliaria->string;
                        newNode->next = NULL;
                        tail->next = newNode;
                        newNode->prev = tail;
                        tail = newNode;
                        testa = newNode;
                    }else testa->string = listaAusiliaria->string;
                    listaAusiliaria = listaAusiliaria->next;
                    testa = testa->next;
                }
                if (testa != NULL) {
                    tail = testa->prev;
                    testa->prev->next = NULL;
                    testa->prev = NULL;
                }
                *dimensioneLista = dimensioneMemoria;
            }
        }
    }
    int conteggio = (cellaPartenza-cellaDestinazione)/4;
    for (int i=(cellaPartenza-1);i>(cellaDestinazione-1);i--) {

      if(i == conteggio){
        input_stato inputStatoAggiuntivo;
        inputStatoAggiuntivo.head = NULL;
        inputStatoAggiuntivo.tail = NULL;
        inputStatoAggiuntivo.operazioneEseguita = (*posizioneCorrente);
        creaCopiaLista(lista,&inputStatoAggiuntivo,*dimensioneLista);
        inputStatoAggiuntivo.dimensioneLista = *dimensioneLista;
        insertArrayStato(statoAggiuntivo,inputStatoAggiuntivo);
      }

        if (i<0) break;
        char comando = memoriaUndo->input[i].comando;
        int primoInd = memoriaUndo->input[i].primoIndirizzo;
        int secondoInd = memoriaUndo->input[i].secondoIndirizzo;
        int dimensioneMemoria = memoriaUndo->input[i].DimensioneLista;
        listaStringhe listaMemoria = memoriaUndo->input[i].head;
        if (comando == 'c') {
            if (*lista == NULL) {
                if (listaMemoria == NULL) {
                    *lista = NULL;
                    head = NULL;
                    tail = NULL;
                    *dimensioneLista = 0;
                }else{
                    head = NULL;
                    tail = NULL;
                    node = (node_t*)malloc(sizeof(node_t)*dimensioneMemoria);
                    for (int i=0;i<dimensioneMemoria;i++) {
                        node[i].string = listaMemoria->string;
                        node[i].next = NULL;
                        if (head == NULL) {
                            node[i].prev = NULL;
                            head = &node[i];
                            tail = &node[i];
                        }else{
                            node[i].prev = tail;
                            tail->next = &node[i];
                            tail = &node[i];
                        }
                        listaMemoria = listaMemoria->next;
                    }
                    *lista = head;
                    (*dimensioneLista) = dimensioneMemoria;
                }
            }else{
                listaStringhe testa = *lista;
                if (listaMemoria == NULL) {
                    for (int i=1;i<primoInd;i++) testa = testa->next;
                    if (primoInd == 1) {
                        *lista = NULL;
                        head = NULL;
                        tail = NULL;
                        *dimensioneLista = 0;
                    }else{
                        tail = testa->prev;
                        testa->prev->next = NULL;
                        testa->prev = NULL;
                        *dimensioneLista = *dimensioneLista - (secondoInd-primoInd+1);
                    }
                }else{
                    if (dimensioneMemoria < (secondoInd - primoInd)+1) {
                        for (int i=1;i<primoInd;i++) testa = testa->next;
                        while (listaMemoria != NULL) {
                            testa->string = listaMemoria->string;
                            listaMemoria = listaMemoria->next;
                            testa = testa->next;
                        }
                        if (testa != NULL) {
                            tail = testa->prev;
                            testa->prev->next = NULL;
                            testa->prev = NULL;
                            // faccio free di testa che rimane
                        }
                        *dimensioneLista = primoInd + dimensioneMemoria - 1;
                    }else{
                        for (int i=1;i<primoInd;i++) testa = testa->next;
                        while (listaMemoria != NULL) {
                            testa->string = listaMemoria->string;
                            testa = testa->next;
                            listaMemoria = listaMemoria->next;
                        }
                    }
                }
            }
        }// fine di if comando c
        if (comando == 'd') {
            if (*lista == NULL) {
                if (listaMemoria == NULL) {
                    *lista = NULL;
                    head = NULL;
                    tail = NULL;
                    *dimensioneLista = 0;
                }else{
                    head = NULL;
                    tail = NULL;
                    node = (node_t*)malloc(sizeof(node_t)*dimensioneMemoria);
                    for (int i=0;i<dimensioneMemoria;i++) {
                        node[i].string = listaMemoria->string;
                        node[i].next = NULL;
                        if (head == NULL) {
                            node[i].prev = NULL;
                            head = &node[i];
                            tail = &node[i];
                        }else{
                            node[i].prev = tail;
                            tail->next = &node[i];
                            tail = &node[i];
                        }
                        listaMemoria = listaMemoria->next;
                    }
                    *lista = head;
                    (*dimensioneLista) = dimensioneMemoria;
                }
            }else{
                listaStringhe testa = *lista;
                if (listaMemoria != NULL) {
                    struct node_s *realHead = head;
                    struct node_s *realTail = tail;
                    listaStringhe newLista = NULL;
                    node = (node_t*)malloc(sizeof(node_t)*dimensioneMemoria);
                    head = NULL;
                    tail = NULL;
                    for (int i=0;i<dimensioneMemoria;i++) {
                        node[i].string = listaMemoria->string;
                        node[i].next = NULL;
                        if (head == NULL) {
                            node[i].prev = NULL;
                            head = &node[i];
                            tail = &node[i];
                        }else{
                            node[i].prev = tail;
                            tail->next = &node[i];
                            tail = &node[i];
                        }
                        listaMemoria = listaMemoria->next;
                    }
                    newLista = head;
                    if (primoInd > *dimensioneLista) {
                        realTail->next = newLista;
                        newLista->prev = realTail;
                        head = realHead;
                        *dimensioneLista = *dimensioneLista + dimensioneMemoria;
                    }else{
                        for (int i=1;i<primoInd;i++) testa = testa->next;
                        if (primoInd ==1) {
                            tail->next = testa;
                            testa->prev = tail;
                            (*lista) = newLista;
                            tail = realTail;
                            *dimensioneLista = *dimensioneLista +dimensioneMemoria;
                        }else{
                            testa->prev->next = newLista;
                            newLista->prev = testa->prev;
                            tail->next = testa;
                            testa->prev = tail;
                            head = realHead;
                            tail = realTail;
                            *dimensioneLista = *dimensioneLista +dimensioneMemoria;
                        }
                    }
                }
            }
        }
    }
    (*posizioneCorrente) = cellaDestinazione;
    if ((*posizioneCorrente) < 0 ) {
        (*posizioneCorrente = 0);
    }
    *totale = DEFAULT;
    return;
}

