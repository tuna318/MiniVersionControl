#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "account_handler.h"

const char *createAccount(char *email, char *name, char *password)
{
	int rc = sqlite3_open("test.db", &db);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s\n",
				sqlite3_errmsg(db));
		sqlite3_close(db);
		return FAIL_DB;
	}

	char *sql_select = "SELECT * FROM users WHERE name = ?";

	rc = sqlite3_prepare_v2(db, sql_select, -1, &res, 0);

	if (rc == SQLITE_OK)
	{
		sqlite3_bind_text(res, 1, name, -1, 0);
	}
	else
	{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		return FAIL_DB;
	}

	int step = sqlite3_step(res);
	if (step == SQLITE_ROW)
	{
		printf("This account existed in system!\n");
		return NULL;
	}
	else
	{
		char *sql_insert = "INSERT INTO users(name, password, email) VALUES(?, ?, ?)";
		if (sqlite3_prepare_v2(db, sql_insert, -1, &res, 0) == SQLITE_OK)
		{
			sqlite3_bind_text(res, 1, name, -1, 0);
			sqlite3_bind_text(res, 2, password, -1, 0);
			sqlite3_bind_text(res, 3, email, -1, 0);
		}
		else
		{
			fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
			return FAIL_DB;
		}
		rc = sqlite3_step(res);
		if (rc != SQLITE_DONE)
		{
			printf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
			return FAIL_DB;
		}
		sqlite3_finalize(res);
		return name;
	}
}

const char *loginAuth(char *email, char *password)
{

	int rc = sqlite3_open("test.db", &db);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s\n",
				sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	char *sql_select = "SELECT * FROM users WHERE (email = ? AND password = ?)";
	if (sqlite3_prepare_v2(db, sql_select, -1, &res, 0) == SQLITE_OK)
	{

		sqlite3_bind_text(res, 1, email, -1, 0);
		sqlite3_bind_text(res, 2, password, -1, 0);
	}
	else
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	if (sqlite3_step(res) == SQLITE_ROW)
	{
		return sqlite3_column_text(res, 1);
	}
	else
	{
		printf("Email or password not correct\n");
		return NULL;
	}
}

const char *createRepo(char *userName, char *nameRepo)
{

	int rc = sqlite3_open("test.db", &db);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s\n",
				sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	char *sql_select = "SELECT * FROM repositories WHERE name_repo = ?";
	if (sqlite3_prepare_v2(db, sql_select, -1, &res, 0) == SQLITE_OK)
	{

		sqlite3_bind_text(res, 1, nameRepo, -1, 0);
	}
	else
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	if (sqlite3_step(res) == SQLITE_ROW)
	{
		printf("Name of repositories existed\n");
		return NULL;
	}
	else
	{
		char *sql_insert = "INSERT INTO repositories(user_name, name_repo) VALUES(?, ?)";
		if (sqlite3_prepare_v2(db, sql_insert, -1, &res, 0) == SQLITE_OK)
		{
			sqlite3_bind_text(res, 1, userName, -1, 0);
			sqlite3_bind_text(res, 2, nameRepo, -1, 0);
		}
		else
		{
			fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
			return FAIL_DB;
		}
		rc = sqlite3_step(res);
		if (rc != SQLITE_DONE)
		{
			printf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
			return FAIL_DB;
		}
		sqlite3_finalize(res);
		return nameRepo;
	}
}

const char *isExistingRepo(char *userName, char *nameRepo)
{
	int rc = sqlite3_open("test.db", &db);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s\n",
				sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	char *sql_select = "SELECT user_name FROM repositories WHERE name_repo = ?";
	if (sqlite3_prepare_v2(db, sql_select, -1, &res, 0) == SQLITE_OK)
	{

		sqlite3_bind_text(res, 1, nameRepo, -1, 0);
	}
	else
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	rc = sqlite3_step(res);
	if (rc != SQLITE_ROW)
	{
		printf("Repository does not exist\n");
		return NULL;
	}
	else
	{
		return nameRepo;
	}
}

char *listRepo(char *userName)
{
	int rc = sqlite3_open("test.db", &db);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s\n",
				sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	char *sql_select = "SELECT name_repo FROM repositories WHERE user_name = ?";
	if (sqlite3_prepare_v2(db, sql_select, -1, &res, 0) == SQLITE_OK)
	{
		sqlite3_bind_text(res, 1, userName, -1, 0);
	}
	else
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));

	rc = sqlite3_step(res);
	if (rc != SQLITE_ROW)
	{
		printf("User has no repository\n");
		return NULL;
	}
	char *nameRepos = malloc(100 * (sizeof(char)));
	*nameRepos = '\0';
	while (rc == SQLITE_ROW)
	{
		char *x = (char *)sqlite3_column_text(res, 0);
		strcat(nameRepos, x);
		strcat(nameRepos, "\n");
		rc = sqlite3_step(res);
	};

	return nameRepos;
}

int main()
{
	char *name = listRepo("tuna");
	if (name == NULL)
		return 0;
	else
		printf("%s\n", name);
}