#include "account_handler.h"

char *createAccount(char *email, char *name, char *password)
{	
	char *db_path = get_db_location();

	int rc = sqlite3_open(db_path, &db);

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
		sqlite3_finalize(res);
		sqlite3_close(db);
		return FAIL_DB;
	}

	int step = sqlite3_step(res);
	if (step == SQLITE_ROW)
	{
		printf("This account existed in system!\n");
		sqlite3_finalize(res);
		sqlite3_close(db);
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
			sqlite3_finalize(res);
			sqlite3_close(db);
			return FAIL_DB;
		}
		rc = sqlite3_step(res);
		if (rc != SQLITE_DONE)
		{
			printf("execution failed: %s\n", sqlite3_errmsg(db));
			sqlite3_finalize(res);
			sqlite3_close(db);
			return FAIL_DB;
		}
		return name;
	}
}

char *loginAuth(char *email, char *password)
{
	char *db_path = get_db_location();

	int rc = sqlite3_open(db_path, &db);

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
	{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(res);
		sqlite3_close(db);
		return FAIL_DB;
	}
	if (sqlite3_step(res) == SQLITE_ROW)
	{
		char *name = malloc(50 * (sizeof(char)));
		*name = '\0';		
		strcpy(name, sqlite3_column_text(res, 1));
		sqlite3_finalize(res);
		sqlite3_close(db);
		return name;
	}
	else
	{
		printf("Email or password not correct\n");
		sqlite3_finalize(res);
		sqlite3_close(db);
		return NULL;
	}
}

char *createRepo(char *userName, char *nameRepo)
{
	char *db_path = get_db_location();

	int rc = sqlite3_open(db_path, &db);

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
	{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(res);
		sqlite3_close(db);
		return FAIL_DB;
	}
	if (sqlite3_step(res) == SQLITE_ROW)
	{
		printf("Name of repositories existed\n");
		sqlite3_finalize(res);
		sqlite3_close(db);
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
			sqlite3_finalize(res);
			sqlite3_close(db);
			return FAIL_DB;
		}
		rc = sqlite3_step(res);
		if (rc != SQLITE_DONE)
		{
			printf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
			sqlite3_finalize(res);
			sqlite3_close(db);
			return FAIL_DB;
		}
		sqlite3_finalize(res);
		sqlite3_close(db);
		return nameRepo;
	}
}

char *isExistingRepo(char *userName, char *nameRepo)
{	
	char *db_path = get_db_location();

	int rc = sqlite3_open(db_path, &db);

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
	{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(res);
		sqlite3_close(db);
		return FAIL_DB;
	}
	rc = sqlite3_step(res);
	if (rc != SQLITE_ROW)
	{
		printf("Repository does not exist\n");
		sqlite3_finalize(res);
		sqlite3_close(db);
		return NULL;
	}
	else
	{
		sqlite3_finalize(res);
		sqlite3_close(db);
		return nameRepo;
	}
}

char *listRepo(char *userName)
{	
	char *db_path = get_db_location();

	int rc = sqlite3_open(db_path, &db);

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
	{
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(res);
		sqlite3_close(db);
	}

	rc = sqlite3_step(res);
	if (rc != SQLITE_ROW)
	{
		printf("User has no repository\n");
		sqlite3_finalize(res);
		sqlite3_close(db);
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

	sqlite3_finalize(res);
	sqlite3_close(db);
	return nameRepos;
}