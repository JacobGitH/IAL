/*
 * BinĂĄrny vyhÄžadĂĄvacĂ­ strom â iteratĂ­vna varianta
 *
 * S vyuĹžitĂ­m dĂĄtovĂ˝ch typov zo sĂşboru btree.h, zĂĄsobnĂ­kov zo sĂşborov stack.h a
 * stack.c a pripravenĂ˝ch kostier funkciĂ­ implementujte binĂĄrny vyhÄžadĂĄvacĂ­
 * strom bez pouĹžitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * InicializĂĄcia stromu.
 *
 * UĹžĂ­vateÄž musĂ­ zaistiĹĽ, Ĺže incializĂĄcia sa nebude opakovane volaĹĽ nad
 * inicializovanĂ˝m stromom. V opaÄnom prĂ­pade mĂ´Ĺže dĂ´jsĹĽ k Ăşniku pamĂ¤te (memory
 * leak). KeÄĹže neinicializovanĂ˝ ukazovateÄž mĂĄ nedefinovanĂş hodnotu, nie je
 * moĹžnĂŠ toto detegovaĹĽ vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * NĂĄjdenie uzlu v strome.
 *
 * V prĂ­pade Ăşspechu vrĂĄti funkcia hodnotu true a do premennej value zapĂ­ĹĄe
 * hodnotu danĂŠho uzlu. V opaÄnom prĂ­pade funckia vrĂĄti hodnotu false a premennĂĄ
 * value ostĂĄva nezmenenĂĄ.
 *
 * Funkciu implementujte iteratĂ­vne bez pouĹžitia vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while (tree)
  {
    if(tree->key == key){
      *value = tree->value;
      return true;
    }else if(key < tree->key){
        tree = tree->left;
    }else if(key > tree->key){
        tree = tree->right;
    }else{
      return false;
    }
  }
  return false;
}

/*
 * VloĹženie uzlu do stromu.
 *
 * PokiaÄž uzol so zadanĂ˝m kÄžĂşÄom v strome uĹž existuje, nahraÄte jeho hodnotu.
 * Inak vloĹžte novĂ˝ listovĂ˝ uzol.
 *
 * VĂ˝slednĂ˝ strom musĂ­ spÄşĹaĹĽ podmienku vyhÄžadĂĄvacieho stromu â ÄžavĂ˝ podstrom
 * uzlu obsahuje iba menĹĄie kÄžĂşÄe, pravĂ˝ vĂ¤ÄĹĄie.
 *
 * Funkciu implementujte iteratĂ­vne bez pouĹžitia vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *tree_node = *tree;


  if(tree_node == NULL){
    bst_node_t *tree_malloc = malloc(sizeof(bst_node_t));
    tree_malloc->key = key;
    tree_malloc->value = value;
    tree_malloc->left = NULL;
    tree_malloc->right = NULL;

    (*tree) = tree_malloc;
  }

  while(tree_node != NULL){
    if (tree_node->key == key){
      tree_node->value = value;
      break;
    }else if(tree_node->key > key){
      if(tree_node->left != NULL){
        tree_node = tree_node->left;
      }else{
        bst_node_t *tree_malloc = malloc(sizeof(bst_node_t));
        tree_malloc->key = key;
        tree_malloc->value = value;
        tree_malloc->left = NULL;
        tree_malloc->right = NULL;

        tree_node->left = tree_malloc;
        break;
      }

    }else{
      if(tree_node->right != NULL){
        tree_node = tree_node->right;
      }else{
        bst_node_t *tree_malloc = malloc(sizeof(bst_node_t));
        tree_malloc->key = key;
        tree_malloc->value = value;
        tree_malloc->left = NULL;
        tree_malloc->right = NULL;

        tree_node->right = tree_malloc;
        break;
      }
    }
  }
}

/*
 * PomocnĂĄ funkcia ktorĂĄ nahradĂ­ uzol najpravejĹĄĂ­m potomkom.
 *
 * KÄžĂşÄ a hodnota uzlu target budĂş nahradenĂŠ kÄžĂşÄom a hodnotou najpravejĹĄieho
 * uzlu podstromu tree. NajpravejĹĄĂ­ potomok bude odstrĂĄnenĂ˝. Funkcia korektne
 * uvoÄžnĂ­ vĹĄetky alokovanĂŠ zdroje odstrĂĄnenĂŠho uzlu.
 *
 * Funkcia predpokladĂĄ Ĺže hodnota tree nie je NULL.
 *
 * TĂĄto pomocnĂĄ funkcia bude vyuĹžitĂĄ pri implementĂĄcii funkcie bst_delete.
 *
 * Funkciu implementujte iteratĂ­vne bez pouĹžitia vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *tree_node = *tree;
  bst_node_t *rightmost_node = target;

  while(tree_node->right && tree_node != NULL){
    rightmost_node = tree_node;
    tree_node = tree_node->right;
  }
  if(tree_node != NULL){
    if(tree_node->left != NULL){
      target->value = tree_node->value;
      target->key = tree_node->key;
      rightmost_node->right = tree_node->left;
      free(tree_node);
    }else{
      target->value = tree_node->value;
      target->key = tree_node->key;
      if(target == rightmost_node){
        rightmost_node->left = NULL;
      }else{
        rightmost_node->right = NULL;
      }
      free(tree_node);
    }
  }
}

/*
 * OdstrĂĄnenie uzlu v strome.
 *
 * PokiaÄž uzol so zadanĂ˝m kÄžĂşÄom neexistuje, funkcia niÄ nerobĂ­.
 * PokiaÄž mĂĄ odstrĂĄnenĂ˝ uzol jeden podstrom, zdedĂ­ ho otec odstrĂĄnenĂŠho uzla.
 * PokiaÄž mĂĄ odstrĂĄnenĂ˝ uzol oba podstromy, je nahradenĂ˝ najpravejĹĄĂ­m uzlom
 * ÄžavĂŠho podstromu. NajpravejĹĄĂ­ uzol nemusĂ­ byĹĽ listom!
 * Funkcia korektne uvoÄžnĂ­ vĹĄetky alokovanĂŠ zdroje odstrĂĄnenĂŠho uzlu.
 *
 * Funkciu implementujte iteratĂ­vne pomocou bst_replace_by_rightmost a bez
 * pouĹžitia vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_delete(bst_node_t **tree, char key) {//todo fck me
    bst_node_t *tree_node = *tree;
    bst_node_t *father_node= *tree;

    while(tree_node){
      if(tree_node->key > key){
        if(tree_node->left != NULL){
          father_node = tree_node;
          tree_node = tree_node->left;
        }else{
          break;
        }
      }else if(tree_node->key < key){
        if(tree_node->right != NULL){
          father_node = tree_node;
          tree_node = tree_node->right;
        }else{
          break;
        }
      }else{
        if(tree_node->left != NULL && tree_node->right != NULL){
          bst_replace_by_rightmost(tree_node, &tree_node->left);
        }else if(tree_node->right == NULL && tree_node->left == NULL){ 
          if(tree_node == *tree){
            free(tree_node);
            break;
          }
          if(father_node->right == tree_node){
            father_node->right = NULL;
          }else{
            father_node->left = NULL;
          }
          free(tree_node);
          break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }else{ 
          if(father_node != NULL){
            if(father_node->left == tree_node){
              if(tree_node->left != NULL){
                father_node->left = tree_node->left;
              }else if(tree_node->right != NULL){
                father_node->left = tree_node->right;
              }
            }
          }
            if(father_node != NULL){
              if(father_node->right == tree_node){
                if(tree_node->right != NULL){
                  father_node->right = tree_node->right;
                }else if(tree_node->right != NULL){
                  father_node->right = tree_node->left;
                }
              }
          }else{
            if(tree_node->left != NULL){
              (*tree) = tree_node->left;
            }else if(tree_node->right != NULL){
              (*tree) = tree_node->right;
            }
          }
          free(tree_node);
          break;
      }
    }
  }
}



/*
 * ZruĹĄenie celĂŠho stromu.
 *
 * Po zruĹĄenĂ­ sa celĂ˝ strom bude nachĂĄdzaĹĽ v rovnakom stave ako po
 * inicializĂĄcii. Funkcia korektne uvoÄžnĂ­ vĹĄetky alokovanĂŠ zdroje ruĹĄenĂ˝ch
 * uzlov.
 *
 * Funkciu implementujte iteratĂ­vne pomocou zĂĄsobnĂ­ku uzlov a bez pouĹžitia
 * vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_dispose(bst_node_t **tree) {
  bst_node_t *tree_node = *tree;
  stack_bst_t stack;
  bst_node_t *top_node = NULL;
  stack_bst_init(&stack);

  if(tree_node != NULL){
    stack_bst_push(&stack, (*tree));

  while(stack_bst_empty(&stack) != false){
    top_node = stack_bst_pop(&stack);

    if(top_node != NULL){
      if(top_node->left != NULL){
        stack_bst_push(&stack, top_node->left);
      }
      if(top_node->right != NULL){
        stack_bst_push(&stack, top_node->right);
      }
      free(top_node);
    }
  }
  *tree = NULL;
  }
}

/*
 * PomocnĂĄ funkcia pre iteratĂ­vny preorder.
 *
 * PrechĂĄdza po Äžavej vetve k najÄžavejĹĄiemu uzlu podstromu.
 * Nad spracovanĂ˝mi uzlami zavola bst_print_node a uloĹžĂ­ ich do zĂĄsobnĂ­ku uzlov.
 *
 * Funkciu implementujte iteratĂ­vne pomocou zĂĄsobnĂ­ku uzlov a bez pouĹžitia
 * vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *tree_node = tree;

  while(tree_node){
    stack_bst_push(to_visit, tree_node);
    bst_print_node(tree_node);
    tree_node = tree_node->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuĂĄlne spracovĂĄvanĂ˝ uzol nad nĂ­m zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratĂ­vne pomocou funkcie bst_leftmost_preorder a
 * zĂĄsobnĂ­ku uzlov bez pouĹžitia vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_preorder(bst_node_t *tree) {
  bst_node_t *tree_node = tree;
  stack_bst_t stack;
  stack_bst_init(&stack);

  if(tree_node != NULL){
    bst_leftmost_preorder(tree_node, &stack);

    while(stack_bst_empty(&stack) == false){
      tree_node = stack_bst_top(&stack);
      stack_bst_pop(&stack);
      //bst_print_node(tree_node); // nevim jestli tu ma bejt print kdyz je print i v preorderu, god end this misery
      bst_leftmost_preorder(tree_node->right, &stack);
    }
  }
}

/*
 * PomocnĂĄ funkcia pre iteratĂ­vny inorder.
 *
 * PrechĂĄdza po Äžavej vetve k najÄžavejĹĄiemu uzlu podstromu a ukladĂĄ uzly do
 * zĂĄsobnĂ­ku uzlov.
 *
 * Funkciu implementujte iteratĂ­vne pomocou zĂĄsobnĂ­ku uzlov a bez pouĹžitia
 * vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *tree_node = tree;
  while(tree_node != NULL){
    stack_bst_push(to_visit, tree_node);
    tree_node = tree_node->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuĂĄlne spracovĂĄvanĂ˝ uzol nad nĂ­m zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratĂ­vne pomocou funkcie bst_leftmost_inorder a
 * zĂĄsobnĂ­ku uzlov bez pouĹžitia vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_inorder(bst_node_t *tree) { //todo pls work 
  bst_node_t *tree_node = tree;
  stack_bst_t stack;
  stack_bst_init(&stack);

  bst_leftmost_inorder(tree_node, &stack);

  while(stack_bst_empty(&stack) == false){

    tree_node = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    bst_print_node(tree_node);
    bst_leftmost_inorder(tree_node->right, &stack);
  }
}

/*
 * PomocnĂĄ funkcia pre iteratĂ­vny postorder.
 *
 * PrechĂĄdza po Äžavej vetve k najÄžavejĹĄiemu uzlu podstromu a ukladĂĄ uzly do
 * zĂĄsobnĂ­ku uzlov. Do zĂĄsobnĂ­ku bool hodnĂ´t ukladĂĄ informĂĄciu Ĺže uzol
 * bol navĹĄtĂ­venĂ˝ prvĂ˝ krĂĄt.
 *
 * Funkciu implementujte iteratĂ­vne pomocou zĂĄsobnĂ­kov uzlov a bool hodnĂ´t a bez pouĹžitia
 * vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  bst_node_t *tree_node = tree;

  while(tree_node != NULL){
    stack_bst_push(to_visit, tree_node);
    stack_bool_push(first_visit, true);
    tree_node = tree_node->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuĂĄlne spracovĂĄvanĂ˝ uzol nad nĂ­m zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratĂ­vne pomocou funkcie bst_leftmost_postorder a
 * zĂĄsobnĂ­kov uzlov a bool hodnĂ´t bez pouĹžitia vlastnĂ˝ch pomocnĂ˝ch funkciĂ­.
 */
void bst_postorder(bst_node_t *tree) {
   bst_node_t *tree_node = tree;
  stack_bst_t stack;
  stack_bst_init(&stack);
  stack_bool_t stack2;
  stack_bool_init(&stack2);

  bst_leftmost_postorder(tree_node, &stack, &stack2);

  while(stack_bst_empty(&stack) == false){
    tree_node = stack_bst_top(&stack);
    stack_bst_pop(&stack);

    if(stack_bool_pop(&stack2)){
      stack_bst_push(&stack, tree_node);
      stack_bool_push(&stack2, false);
      bst_leftmost_postorder(tree_node->right, &stack, &stack2);
    }else{
      bst_print_node(tree_node);
    }
  }
}
