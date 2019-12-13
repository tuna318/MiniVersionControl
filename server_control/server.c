#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

void createAccount(char *name, char *password, char *email, sqlite3 *db, int rc)
{

	sqlite3_stmt *res; // single SQL statement
	char *sql_select = "SELECT * FROM users WHERE name = ?";

	rc = sqlite3_prepare_v2(db, sql_select, -1, &res, 0);

	if (rc == SQLITE_OK)
	{
		sqlite3_bind_text(res, 1, name, -1, 0);
	}
	else
	{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	int step = sqlite3_step(res);
	if (step == SQLITE_ROW)
	{
		printf("This account existed in system!\n");
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
			return;
		}
		printf("Create account success!\n");
	}
}

const char *login(char *email, char *password, sqlite3 *db)
{
	sqlite3_stmt *res;
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
		// printf("email: %s - pass: %s\n", sqlite3_column_text(res, 3), sqlite3_column_text(res, 2));
		return sqlite3_column_text(res, 1);
	}
	else
	{
		printf("Email or password not correct\n");
		return NULL;
	}
}

int writeFile(char *nameRepo,const char *name)
{

	FILE *file = fopen(nameRepo, "a");
	if (file == NULL)
	{
		printf("Can't open file %s\n", nameRepo);
		return 1;
	}
	else
	{
		fprintf(file, "%s\n", name);
		return 0;
	}
}

void createRepo(char *email, char *password, char *nameRepo, sqlite3 *db)
{

	const char *name = login(email, password, db);
	if (name == NULL)
	{
		printf("Can't create repository. Account is not correct!\n");
		exit(1);
	}
	else
	{
		if (writeFile(nameRepo, name) == 0)
			printf("Create repository success\n");
		else
			exit(1);
	}
}

int main(void)
{
	sqlite3 *db; //a db
	char *err_msg = 0;

	int rc = sqlite3_open("test.db", &db);

	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s\n",
						sqlite3_errmsg(db));
		sqlite3_close(db);

		return 1;
	}
	createRepo("hanh.shin.1998@gmail.com", "123456", "network", db);
	return 0;
}