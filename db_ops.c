/*
* Copyright (c) 2010 Rob Hoelz <rob@hoelz.ro>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include "luabdb_private.h"

#include <db.h>
#include <lauxlib.h>
#include <lualib.h>
#include <string.h>

static int db_op_put(lua_State *L)
{
    DB *db;

    DB_TXN *txn = NULL;
    DBT key;
    DBT value;
    size_t key_len;
    size_t value_len;
    int flags = 0;
    int status;

    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));

    db = luabdb_todb(L, 1);
    key.data = (void *) luaL_checklstring(L, 2, &key_len);
    value.data = (void *) luaL_checklstring(L, 3, &value_len);
    key.size = key_len;
    value.size = value_len;

    status = db->put(db, txn, &key, &value, flags);
    handle_error(status);

    return 0;
}

static int db_op_get(lua_State *L)
{
    DB *db;

    DB_TXN *txn = NULL;
    DBT key;
    DBT value;
    size_t key_len;
    size_t value_len;
    int flags = 0;
    int status;

    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));

    db = luabdb_todb(L, 1);
    key.data = (void *) luaL_checklstring(L, 2, &key_len);
    key.size = key_len;
    value.flags = DB_DBT_MALLOC;

    status = db->get(db, txn, &key, &value, flags);
    handle_error(status);

    lua_pushlstring(L, value.data, value.size);
    free(value.data);

    return 1;
}

static int db_op_close(lua_State *L)
{
    DB *db;
    int flags = 0;

    db = luabdb_todb(L, 1);
    db->close(db, flags);
    return 0;
}

#define _(name) { #name, db_op_##name }
#define u_(name) { #name, luabdb_unimplemented }

static luaL_Reg db_funcs[] = {
    _(put),
    _(get),
    _(close),
    { NULL, NULL }
};
#undef _

int init_db_ops(lua_State *L)
{
    luaL_getmetatable(L, LUABDB_DB);
    lua_getfield(L, -1, "__index");
    luaL_register(L, NULL, db_funcs);

    return 0;
}
