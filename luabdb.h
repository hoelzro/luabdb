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

#ifndef LUA_BDB_H
#define LUA_BDB_H

#define LUABDB_DB "DB*"
#define LUABDB_ENV "DB_ENV*"
#define LUABDB_CURSOR "DBC*"
#define LUABDB_LOG "DB_LOGC*"
#define LUABDB_MEMPOOL "DB_MPOOLFILE*"
#define LUABDB_SEQ "DB_SEQUENCE*"
#define LUABDB_TXN "DB_TXN*"

#define luabdb_todb(L, narg) ((DB *) luaL_checkudata(L, narg, LUABDB_DB))
#define luabdb_toenv(L, narg) ((DB_ENV *) luaL_checkudata(L, narg, LUABDB_ENV))
#define luabdb_tocursor(L, narg) ((DBC *) luaL_checkudata(L, narg, LUABDB_CURSOR))
#define luabdb_tolog(L, narg) ((DB_LOGC *) luaL_checkudata(L, narg, LUABDB_LOG))
#define luabdb_tomempool(L, narg) ((DB_MPOOLFILE *) luaL_checkudata(L, narg, LUABDB_MEMPOOL))
#define luabdb_toseq(L, narg) ((DB_SEQUENCE *) luaL_checkudata(L, narg, LUABDB_SEQ))
#define luabdb_totxn(L, narg) ((DB_TXN *) luaL_checkudata(L, narg, LUABDB_TXN))

#endif
