#include <db.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct customer
{
	char surname[10];
	char givenname[20];
	char contact[10];
	char dob[10];
} CUSTOMER;

void init_DBT(DBT *key, DBT *data)
{
	memset(key,0,sizeof(DBT));
	memset(data,0,sizeof(DBT));
}

int main(void)
{
	
}
