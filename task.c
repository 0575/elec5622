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
    DB_ENV *dbenv;
    DB *dbp;    
    DBT key, data;

    int ret = 0;
    char key_cust_surname = "Li";
    CUSTOMER cust = {"Li", "Ruoyang", "0412572550", "01/11/1995"}; 


    /* initialize env handler */
    if (ret = db_env_create(&dbenv, 0)) { 
        printf("db_env_create ERROR: %s\n", db_strerror(ret));
        goto failed;
    }   

    u_int32_t flags = DB_CREATE | DB_INIT_MPOOL | DB_INIT_CDB | DB_THREAD;;  

    if (ret = dbenv->open(dbenv, "/data0/bdb_test", flags, 0)) {
        printf("dbenv->open ERROR: %s\n", db_strerror(ret));
        goto failed;
    }   

    /* initialize db handler */
    if (ret = db_create(&dbp, dbenv, 0)) {
        printf("db_create ERROR: %s\n", db_strerror(ret));
        goto failed;
    }   

    flags = DB_CREATE | DB_THREAD;

    if (ret = dbp->open(dbp, NULL, "single.db", NULL, DB_BTREE, flags, 0664)) {
        printf("dbp->open ERROR: %s\n", db_strerror(ret));
        goto failed;
    }


    /* write record */
    /* initialize DBT */
    init_DBT(&key, &data);
    key.data = &key_cust_surname;
    key.size = sizeof(key_cust_surname);
    data.data = &cust;
    data.size = sizeof(CUSTOMER);

    if (ret = dbp->put(dbp, NULL, &key, &data, DB_NOOVERWRITE)) {
        printf("dbp->put ERROR: %s\n", db_strerror(ret));
        goto failed;
    }

    /* flush to disk */
    dbp->sync(dbp, 0);

    /* get record */
    init_DBT(&key, &data);
    key.data = &key_cust_surname;
    key.size = sizeof(key_cust_surname);
    data.flags = DB_DBT_MALLOC;

    if (ret = dbp->get(dbp, NULL, &key, &data, 0)) {
        printf("dbp->get ERROR: %s\n", db_strerror(ret));
        goto failed;
    }

    CUSTOMER *info = data.data;

    printf("Surname = %d\ngivenname=%s\ncontact=%s\ndob=%d\n",
            info->surname,
            info->givenname,
            info->contact,
            info->dob);

    /* free */
    free(data.data);

    if(dbp) {
        dbp->close(dbp, 0);
    }

    if (dbenv) {
    dbenv->close(dbenv, 0);
    }

    return 0;


failed:

    if(dbp) {
        dbp->close(dbp, 0);
    }

    if (dbenv) {
        dbenv->close(dbenv, 0);
    }

    return -1;
}
