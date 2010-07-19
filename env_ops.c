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

static int env_op_close(lua_State *L)
{
    return 0;
}

#define _(name) { #name, env_op_##name }
#define u_(name) { #name, luabdb_unimplemented }

static luaL_Reg env_funcs[] = {
    _(close),
    { NULL, NULL }
};

#undef _
#undef u_

static int env__gc(lua_State *L)
{
    lua_pushcfunction(L, env_op_close);
    lua_pushvalue(L, 1);
    lua_pcall(L, 1, 0, 0);
    return 0;
}

static int env__tostring(lua_State *L)
{
    lua_pushfstring(L, "env: %p", lua_topointer(L, 1));
    return 1;
}

int init_env_ops(lua_State *L)
{
    luaL_getmetatable(L, LUABDB_ENV);
    lua_pushcfunction(L, env__gc);
    lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, env__tostring);
    lua_setfield(L, -2, "__tostring");
    lua_getfield(L, -1, "__index");
    luaL_register(L, NULL, env_funcs);

    return 0;
}
