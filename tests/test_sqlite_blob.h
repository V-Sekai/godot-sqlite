/**************************************************************************/
/*  test_sqlite_blob.h                                                    */
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

#ifndef TEST_SQLITE_BLOB_H
#define TEST_SQLITE_BLOB_H

#include "core/io/marshalls.h"
#include "core/variant/variant.h"
#include "tests/test_macros.h"

#include "../godot_sqlite.h"

namespace TestSqliteBlob {

Ref<SQLite> db;
String db_path = "user://bytes_db.sqlite";

void setup() {
	db = Ref<SQLite>(memnew(SQLite));
	CHECK(db->open_in_memory());
}

void teardown() {
	db->close();
}

void create_table() {
	String query = "CREATE TABLE IF NOT EXISTS byte_data ("
				   "id integer PRIMARY KEY,"
				   "dict BLOB NOT NULL"
				   ");";
	Ref<SQLiteQuery> result = db->create_query(query);
	result->execute(Array());
}

TEST_CASE("[Modules][SQLite] open database") {
	setup();

	CHECK_FALSE(db->open("res://non_existent.db"));

	teardown();
}

TEST_CASE("[Modules][SQLite] fetch data from byte_data table") {
	setup();
	create_table();

	Dictionary data;
	data["time_created"] = Time::get_singleton()->get_datetime_dict_from_system(true);
	PackedByteArray bytes = VariantUtilityFunctions::var_to_bytes(data);

	String query = "INSERT INTO byte_data VALUES (?, ?);";
	Array args;
	args.push_back(1);
	args.push_back(bytes);
	Ref<SQLiteQuery> result = db->create_query(query);
	result->execute(args);

	query = "SELECT dict FROM byte_data WHERE id=1 LIMIT 1;";
	result = db->create_query(query);
	Array rows = result->execute(Array());
	CHECK_FALSE(rows.is_empty());

	teardown();
}

TEST_CASE("[Modules][SQLite] fetch data from non-existent table") {
	setup();

	String query = "SELECT * FROM non_existent_table;";
	Ref<SQLiteQuery> result = db->create_query(query);
	Array rows = result->execute(Array());
	CHECK(rows.is_empty());

	teardown();
}

} //namespace TestSqliteBlob

#endif // TEST_SQLITE_BLOB_H
