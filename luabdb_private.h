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

#ifndef LUA_BDB_PRIVATE_H
#define LUA_BDB_PRIVATE_H

#include <db.h>
#include <lua.h>
#include "luabdb.h"

int luabdb_unimplemented(lua_State *L);

int init_flags(lua_State *L);
int init_db_ops(lua_State *L);
int init_db_config(lua_State *L);
int init_cursor_ops(lua_State *L);
int init_env_ops(lua_State *L);
int init_env_config(lua_State *L);
int init_lock_ops(lua_State *L);
int init_lock_config(lua_State *L);
int init_log_ops(lua_State *L);
int init_log_config(lua_State *L);
int init_memory_pool_ops(lua_State *L);
int init_memory_pool_config(lua_State *L);
int init_mutex_ops(lua_State *L);
int init_mutex_config(lua_State *L);
int init_replication_ops(lua_State *L);
int init_replication_config(lua_State *L);
int init_sequence_ops(lua_State *L);
int init_sequence_config(lua_State *L);
int init_txn_ops(lua_State *L);
int init_txn_config(lua_State *L);

#define handle_error(status)\
    if(status) {\
        lua_pushnil(L);\
        lua_pushstring(L, db_strerror(status));\
        lua_pushinteger(L, status);\
        return 3;\
    }

DB **luabdb_createdbp(lua_State *L);
DB_ENV **luabdb_createenvp(lua_State *L);

#endif
