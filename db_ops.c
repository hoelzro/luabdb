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
    DB **dbp;
    int flags = 0;

    luabdb_todb(L, 1); /* for error checking */
    dbp = (DB **) luaL_checkudata(L, 1, LUABDB_DB);
    (*dbp)->close(*dbp, flags);
    *dbp = NULL;
    return 0;
}

#define _(name) { #name, db_op_##name }
#define u_(name) { #name, luabdb_unimplemented }

static luaL_Reg db_funcs[] = {
    _(put),
    _(get),
    _(close),
    u_(associate),
    u_(associate_foreign),
    u_(compact),
    u_(del),
    u_(err),
    u_(exists),
    u_(fd),
    u_(get_byteswapped),
    u_(get_dbname),
    u_(get_multiple),
    u_(get_open_flags),
    u_(get_type),
    u_(join),
    u_(key_range),
    u_(remove),
    u_(rename),
    u_(set_priority),
    u_(get_priority),
    u_(stat),
    u_(stat_print),
    u_(sync),
    u_(truncate),
    u_(upgrade),
    u_(verify),
    u_(cursor),
    u_(set_alloc),
    u_(set_cachesize),
    u_(get_cachesize),
    u_(set_create_dir),
    u_(get_create_dir),
    u_(set_dup_compare),
    u_(set_encrypt),
    u_(get_encrypt_flags),
    u_(set_errcall),
    u_(set_errfile),
    u_(get_errfile),
    u_(set_errpfx),
    u_(get_errpfx),
    u_(set_feedback),
    u_(set_flags),
    u_(get_flags),
    u_(set_lorder),
    u_(get_lorder),
    u_(set_msgcall),
    u_(set_msgfile),
    u_(get_msgfile),
    u_(set_pagesize),
    u_(get_pagesize),
    u_(set_partition),
    u_(set_partition_dirs),
    u_(get_partition_dirs),
    u_(set_append_recno),
    u_(set_bt_compare),
    u_(set_bt_compress),
    u_(set_bt_minkey),
    u_(get_bt_minkey),
    u_(set_bt_prefix),
    u_(set_re_delim),
    u_(get_re_delim),
    u_(set_re_len),
    u_(get_re_len),
    u_(set_re_pad),
    u_(get_re_pad),
    u_(set_re_source),
    u_(get_re_source),
    u_(set_h_compare),
    u_(set_h_ffactor),
    u_(get_h_ffactor),
    u_(set_h_hash),
    u_(set_h_nelem),
    u_(get_h_nelem),
    u_(set_q_extentsize),
    u_(get_q_extentsize),
    u_(get_env),
    u_(get_mpf),
    u_(get_transactional),
    { NULL, NULL }
};
#undef _

static int db__gc(lua_State *L)
{
    lua_pushcfunction(L, db_op_close);
    lua_pushvalue(L, 1);
    lua_pcall(L, 1, 0, 0);
    return 0;
}

int init_db_ops(lua_State *L)
{
    luaL_getmetatable(L, LUABDB_DB);
    lua_pushcfunction(L, db__gc);
    lua_setfield(L, -2, "__gc");
    lua_getfield(L, -1, "__index");
    luaL_register(L, NULL, db_funcs);

    return 0;
}
