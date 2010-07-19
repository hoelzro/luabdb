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

#include <db.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "luabdb_private.h"

/* must recieve an absolute index! */
#define get_option(L, narg, name, fetch)\
    lua_getfield(L, narg, #name);\
    if(! lua_isnil(L, -1)) {\
        name = fetch(L, -1);\
    }\
    lua_pop(L, 1);\
    lua_pushnil(L);\
    lua_setfield(L, narg, #name);

static u_int32_t luabdb_getflags(lua_State *L, int narg)
{
    u_int32_t flags = 0;
    int type;

    type = lua_type(L, narg);

    if(type == LUA_TNUMBER) {
        flags = lua_tointeger(L, narg);
    } else if(type == LUA_TTABLE) {
        size_t size;
        int i;

        size = lua_objlen(L, narg);
        for(i = 1; i <= size; i++) {
            lua_rawgeti(L, narg, i);
            flags |= luaL_checkinteger(L, -1);
            lua_pop(L, 1);
        }
    } else {
        return luaL_error(L, "either an integer or a table is expected for flags");
    }

    return flags;
}

static int luabdb_getmode(lua_State *L, int narg)
{
    int mode = 0;
    int type;

    type = lua_type(L, narg);

    if(type == LUA_TNUMBER) {
        mode = lua_tointeger(L, narg);
    } else if(type == LUA_TTABLE) {
        int read = 0;
        int write = 0;
        int execute = 0;
        int top;

        lua_pushvalue(L, narg);
        top = lua_gettop(L);

        get_option(L, top, read, lua_toboolean);
        get_option(L, top, write, lua_toboolean);
        get_option(L, top, execute, lua_toboolean);
        mode = read | (write << 1) | (execute << 2);

        lua_pushnil(L);
        if(lua_next(L, top)) {
            return luaL_error(L, "Invalid option in mode table: %s", lua_tostring(L, -2));
        }
        lua_pop(L, 1);
    } else {
        return luaL_error(L, "either an integer or a table is expected for mode");
    }

    return mode;
}

static int luabdb_open(lua_State *L)
{
    DB **dbp;
    int status;

    DB_ENV *env = NULL;
    DB_TXN *txn = NULL;
    const char *file = NULL;
    const char *database = NULL;
    DBTYPE type = DB_UNKNOWN;
    u_int32_t flags = 0;
    int mode = 0;

    if(! lua_isnoneornil(L, 1)) {
        luaL_checktype(L, 1, LUA_TTABLE);

        get_option(L, 1, env, luabdb_toenv);
        get_option(L, 1, txn, luabdb_totxn);
        get_option(L, 1, file, luaL_checkstring);
        get_option(L, 1, database, luaL_checkstring);
        get_option(L, 1, type, luaL_checkinteger);
        get_option(L, 1, flags, luabdb_getflags);
        get_option(L, 1, mode, luabdb_getmode);

        lua_pushnil(L);
        if(lua_next(L, 1)) {
            return luaL_error(L, "Invalid option to bdb.open: %s", lua_tostring(L, -2));
        }
    }

    dbp = lua_newuserdata(L, sizeof(DB *));
    luaL_getmetatable(L, LUABDB_DB);
    lua_setmetatable(L, -2);

    status = db_create(dbp, env, 0);
    handle_error(status);
    status = (*dbp)->open(*dbp, txn, file, database, type, flags, mode);
    handle_error(status);

    return 1;
}

static luaL_Reg luabdb_functions[] = {
    { "open", luabdb_open },
    { NULL, NULL }
};

static int init_metatables(lua_State *L)
{
    const char *types[] = {
        LUABDB_DB,
        LUABDB_ENV,
        LUABDB_CURSOR,
        LUABDB_LOG,
        LUABDB_MEMPOOL,
        LUABDB_SEQ,
        LUABDB_TXN,
        NULL
    };
    const char **type;
    for(type = types; *type; type++) {
        luaL_newmetatable(L, *type);
        lua_newtable(L);
        lua_setfield(L, -2, "__index");
        lua_pushboolean(L, 0);
        lua_setfield(L, -2, "__metatable");
        lua_pop(L, 1);
    }
    return 0;
}

int luabdb_unimplemented(lua_State *L)
{
    lua_Debug ar;
    lua_getstack(L, 0, &ar);
    lua_getinfo(L, "n", &ar);

    return luaL_error(L, "Function '%s' is not implemented!", ar.name);
}

int luaopen_bdb(lua_State *L)
{
    lua_CFunction init_funcs[] = {
        init_metatables,
        init_flags,
        init_db_ops,
        init_db_config,
        init_cursor_ops,
        init_env_ops,
        init_env_config,
        init_lock_ops,
        init_lock_config,
        init_log_ops,
        init_log_config,
        init_memory_pool_ops,
        init_memory_pool_config,
        init_mutex_ops,
        init_mutex_config,
        init_replication_ops,
        init_replication_config,
        init_sequence_ops,
        init_sequence_config,
        init_txn_ops,
        init_txn_config,
        NULL
    };
    lua_CFunction *init_func;

    const char *libraryName = luaL_checkstring(L, 1);
    lua_newtable(L);
    luaL_register(L, libraryName, luabdb_functions);

    for(init_func = init_funcs; *init_func; init_func++) {
        lua_pushcfunction(L, *init_func);
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
    }

    return 1;
}
