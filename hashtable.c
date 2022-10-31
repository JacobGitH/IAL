/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  for (int i = 0; i < HT_SIZE; i++)
  {
      (*table)[i] = NULL;
  }
  
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int hashed_key = get_hash(key);
  ht_item_t *item = (*table)[hashed_key];


    while(item != NULL){
      if(item->key == key){
        return item;
      }
      item = item->next;
    }
  
  
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  ht_item_t *item = ht_search(table, key);

  if (item != NULL)
  {
    item->value = value;
  }else{
    int hashed_key = get_hash(key);
    
    ht_item_t* item_new = (ht_item_t*) malloc(sizeof(ht_item_t));
    if(item_new == NULL){
      return;
    }
    item_new->key = key;
    item_new->value = value;
    item_new->next = NULL;

    ht_item_t *item_old = (*table)[hashed_key];

    if(item_old){
    (*table)[hashed_key] = item_new;
      item_new->next = item_old;
    }
    (*table)[hashed_key] = item_new;
  }


  
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item = ht_search(table, key);
  if(item != NULL){
    return &(item->value);
  }
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int hashed_key = get_hash(key);
  ht_item_t *item = (*table)[hashed_key];
  ht_item_t *delete = (*table)[hashed_key];


    while (item != NULL)
    {
      if (item->key == key){
       (*table)[hashed_key] = item->next;
       free(item);
      }
      else if(item->next->key == key){
        if(item->next->next != NULL){
          delete = item->next;
          item->next = item->next->next;
          free(delete);
          break;
        }else{
          free(item->next);
        }  
      }
          item = item->next;
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  ht_item_t *item;
  ht_item_t *next_item;

  for (int i = 0; i < HT_SIZE; i++)
  {
    if((*table)[i] != NULL){
      next_item = (*table)[i];

      while(next_item->next != NULL){
        item = next_item;
        next_item = next_item->next;
        free(item);
      }
      free(next_item);
      (*table)[i] = NULL;
    }
  }
}
