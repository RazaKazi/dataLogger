
#include "stdio.h"
#include "stdlib.h"
#include "date.h"

struct date {
    int day;
    int month;
    int year;
};

Date *date_create(char *datestr){ //dd/mm/yyyy
    if(datestr==NULL) return NULL;
    Date *d=malloc(sizeof(struct date));
    char day[3];
    char month[3];
    char year[5];
    if (d!=NULL){
        day[0] = datestr[0];
        day[1] = datestr[1];
        day[2] = '\0';
        month[0] = datestr[3];
        month[1] = datestr[4];
        month[2] = '\0';
        year[0] = datestr[6];
        year[1] = datestr[7];
        year[2] = datestr[8];
        year[3] = datestr[9];
        year[4] = '\0';
        d->day= atoi(day);
        d->month= atoi(month);
        d->year= atoi(year);
        if ((d->day>0 && d->day< 32) && (d->month>0 && d->month<32) && (d->year>0 && d->year<2100)){
            return d; //return with success
        }
        else{
            return NULL; // return as failure
        }
    }
    else{
        free (d);
        return NULL;
    } 
}

/*
 * date_duplicate creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
Date *date_duplicate(Date *d){
    if(d==NULL) return NULL;
    Date *date_to_duplicate=malloc(sizeof(Date));
    if (date_to_duplicate!=NULL){
        date_to_duplicate->year=d->year;
        date_to_duplicate->month=d->month;
        date_to_duplicate->day=d->day;
        return date_to_duplicate;
    }
    else{
        free(date_to_duplicate);
        return NULL;
    }
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2){
    //if(date1 == NULL || date2 == NULL) return NULL;
    if (date1->year>date2->year){
        return 1;
    }
    else if(date1->year<date2->year){
        return -1;
    }
    else{
        if (date1->month>date2->month){
            return 1;
        }
        else if(date1->month<date2->month){
            return -1;
        }
        else{
            if (date1->day>date2->day){
            return 1;
            }
            else if(date1->day<date2->day){
                return -1;
            }
            else{
                return 0;
            }
        }
    }
}

/*
* date_destroy returns any storage associated with `d' to the system
*/
/*
 * date_destroy returns any storage associated with `d' to the system
 */
void date_destroy(Date *d){
    free(d);
}







