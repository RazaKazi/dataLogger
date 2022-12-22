
#include "date.h"
#include "tldlist.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"

struct tldnode{
    TLDNode *parentNode;
    TLDNode *rightChild;
    TLDNode *leftChild;
    TLDNode *nextNode;
    Date *date;
    long occurenceCount;
    char *domainString;
};

struct tldlist{
    long successCount;
    Date *startDate;
    Date *endDate;
    TLDNode *rootNode;
    TLDNode *lastAdded;
    long nodeCount;
};

struct tlditerator{
    TLDNode *node;
    TLDList *list;
};

/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList *tldlist_create(Date *begin, Date *end){
    if(begin == NULL || end == NULL) return NULL;
    TLDList * list= malloc(sizeof(TLDList));
    if (list!=NULL){//check if allocation failure occurred
        list->successCount=0;
        list->startDate= begin;
        list->endDate= end;
        list->rootNode= NULL;
        return list;
    }
    else{
        free(list);
        return NULL;
    }
}

/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */
void tldlist_destroy(TLDList *tld){
    TLDNode *currentNode = malloc(sizeof(tld->rootNode));
    TLDNode *tempNode2 = malloc(sizeof(tld->rootNode));
    currentNode= tld->rootNode;
    for (int i=0;i<tld->nodeCount;i++){
        tempNode2=currentNode->nextNode;
        free(currentNode);
        currentNode=tempNode2;
    }
    free(tld);
    free(currentNode);
    free(tempNode2);
}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList *tld, char *hostname, Date *d){
    char *domainString =strrchr(hostname, '.') +1;
    char *topDomain = malloc(sizeof(domainString));
    for (int i = 0; (domainString[i] != '\0'); i++) {
		domainString[i] = tolower(domainString[i]);
    }
    strcpy(topDomain, domainString);
    if ((date_compare(d,tld->startDate)>=0) && (date_compare(d,tld->endDate)<=0)){
        if (tld->rootNode == NULL){
            tld->rootNode = malloc(sizeof(TLDNode));
            tld->rootNode->parentNode=NULL;
            tld->rootNode->rightChild=NULL;
            tld->rootNode->leftChild=NULL;
            tld->rootNode->nextNode=NULL;// next node to be destroyed
            tld->rootNode->date=d;
            tld->rootNode->domainString=topDomain;
            tld->rootNode->occurenceCount++;// how many times a domain has been accessed
            tld->successCount++;// how many times a succesful addition has been made
            tld->nodeCount++;// how many nodes exist in the tree
            tld->lastAdded=tld->rootNode;
            return 1;
        }
        else{
            TLDNode *node=tld->rootNode;
            while (node!=NULL){
                int comparison = strcmp(node->domainString, topDomain);
                if (comparison>0){
                    if(node->leftChild== NULL){
                        node->leftChild = malloc(sizeof(struct tldnode));
                        node->leftChild->parentNode=node;
                        node->leftChild->date=d;
                        node->leftChild->nextNode=NULL;// next node to be destroyed
                        node->leftChild->domainString=topDomain;
                        node->leftChild->occurenceCount++;// how many times a domain has been accessed
                        tld->successCount++;// how many times a succesful addition has been made
                        tld->nodeCount++;// how many nodes exist in the tree
                        tld->lastAdded->nextNode=node->leftChild;//set the current node added to the next node of the last node which was added
                        tld->lastAdded=node->leftChild;// change last added to the current node
                        return 1;
                    } 
                    else {
                        node = node->leftChild;
                    }
                }
                else if(comparison<0){
                    if(node->rightChild== NULL){
                        node->rightChild = malloc(sizeof(struct tldnode));
                        node->rightChild->parentNode=node;
                        node->rightChild->date=d;
                        node->rightChild->nextNode=NULL;// next node to be destroyed
                        node->rightChild->domainString=topDomain;
                        node->rightChild->occurenceCount++;// how many times a domain has been accessed
                        tld->successCount++;// how many times a succesful addition has been made
                        tld->nodeCount++;// how many nodes exist in the tree
                        tld->lastAdded->nextNode=node->rightChild;//set the current node added to the next node of the last node which was added
                        tld->lastAdded=node->rightChild;// change last added to the current node
                        return 1;
                    } 
                    else {
                        node = node->rightChild;
                    }
                }
                else{
                    tld->successCount++;
                    node->occurenceCount++;
                    return 1;
                }
            }
        }
        free(topDomain);
        return 0;
    }
    else{
        return 0;
    }
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld){
    return tld->successCount;
}

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld){
    if (tld == NULL ) return NULL; 
    TLDIterator *createdIter = malloc(sizeof(TLDIterator));
    if (createdIter != NULL) {
		createdIter->node = NULL;
        createdIter->list= tld;
		return createdIter;
	}
	else {
		tldlist_iter_destroy(createdIter);
		return NULL;
	}
}

TLDNode* min(TLDNode* x) {
    while (x->leftChild != NULL){
        x = x->leftChild;
    }
    return x;
}
/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter){
    if(iter == NULL) return NULL;
    if(iter->node == NULL){
        iter->node = min(iter->list->rootNode);
        return iter->node;
    }
    if (iter->node->rightChild != NULL) {
        iter->node = min(iter->node->rightChild);
        return iter->node;
    }
    TLDNode *x= iter->node->parentNode;
    while (x!=NULL && iter->node==x->rightChild){
        iter->node=x;
        x=x->parentNode;
    }
    iter->node=x;
    return iter->node;
}




/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter){
    free (iter);
}

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node){
    return node->domainString;
}

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node){
    return node->occurenceCount;
}

