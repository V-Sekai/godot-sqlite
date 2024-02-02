/**************************************************************************/
/*  test_sqlite_sqlquery.h                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef TEST_SQLITE_H
#define TEST_SQLITE_H

#include "core/variant/variant.h"
#include "tests/test_macros.h"

#include "../godot_sqlite.h"

namespace TestSqlite {

Ref<SQLite> db;

void setup() {
	db = Ref<SQLite>(memnew(SQLite));
}

void teardown() {
	db->close();
}

TEST_CASE("[Modules][SQLite] handle invalid queries") {
	setup();

	CHECK(db->open_in_memory());

	String query = "INVALID SQL QUERY;";
	Ref<SQLiteQuery> result = db->create_query(query);
	CHECK(result->execute(Array()) == Variant());

	teardown();
}

TEST_CASE("[Modules][SQLite] database operations") {
	setup();

	CHECK(db->open_in_memory());

	String query = "CREATE TABLE IF NOT EXISTS users ("
				   "id integer PRIMARY KEY,"
				   "first_name text NOT NULL,"
				   "last_name text NOT NULL,"
				   "email text NOT NULL"
				   ");";
	Ref<SQLiteQuery> result = db->create_query(query);
	CHECK(result->execute(Array()) == Array());

	query = "SELECT * FROM users;";
	result = db->create_query(query);
	Array rows = result->execute(Array());

	if (rows.is_empty()) {
		query = "INSERT INTO users (first_name, last_name, email) VALUES ('godot', 'engine', 'user@users.org');";
		result = db->create_query(query);
		CHECK(result->execute(Array()) == Array());
	} else {
		for (int i = 0; i < rows.size(); ++i) {
			CHECK(rows[i] != Variant());
		}
	}

	teardown();
}
} //namespace TestSqlite

#endif // TEST_SQLITE_H
