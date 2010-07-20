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

DB_ENV *luabdb_toenv(lua_State *L, int narg)
{
    DB_ENV **envp = (DB_ENV **) luaL_checkudata(L, narg, LUABDB_ENV);
    if(! *envp) {
        luaL_error(L, "Attempt to use a closed DB_ENV handle (%p)", lua_topointer(L, narg));
    }
    return *envp;
}

DB_ENV **luabdb_createenvp(lua_State *L)
{
    DB_ENV **envp;

    envp = lua_newuserdata(L, sizeof(DB_ENV *));
    luaL_getmetatable(L, LUABDB_ENV);
    lua_setmetatable(L, -2);

    return envp;
}

static int env_op_close(lua_State *L)
{
    DB_ENV **envp;
    u_int32_t flags = 0;

    luabdb_toenv(L, 1); /* for error checking */
    envp = (DB_ENV **) luaL_checkudata(L, 1, LUABDB_ENV);
    (*envp)->close(*envp, flags);
    *envp = NULL;
    return 0;
}

#define _(name) { #name, env_op_##name }
#define u_(name) { #name, luabdb_unimplemented }

static luaL_Reg env_funcs[] = {
    _(close),
    u_(add_data_dir),
    u_(cdsgroup_begin),
    u_(cdsgroup_begin),
    u_(dbremove),
    u_(dbrename),
    u_(err),
    u_(failchk),
    u_(fileid_reset),
    u_(get_cache_max),
    u_(get_cachesize),
    u_(get_cachesize),
    u_(get_create_dir),
    u_(get_data_dirs),
    u_(get_encrypt_flags),
    u_(get_errfile),
    u_(get_errpfx),
    u_(get_flags),
    u_(get_home),
    u_(get_intermediate_dir_mode),
    u_(get_lg_bsize),
    u_(get_lg_dir),
    u_(get_lg_filemode),
    u_(get_lg_max),
    u_(get_lg_regionmax),
    u_(get_lk_conflicts),
    u_(get_lk_detect),
    u_(get_lk_max_lockers),
    u_(get_lk_max_locks),
    u_(get_lk_max_objects),
    u_(get_lk_partitions),
    u_(get_lk_priority),
    u_(get_mp_max_openfd),
    u_(get_mp_max_write),
    u_(get_mp_mmapsize),
    u_(get_mp_pagesize),
    u_(get_mp_tablesize),
    u_(get_msgfile),
    u_(get_open_flags),
    u_(get_shm_key),
    u_(get_thread_count),
    u_(get_timeout),
    u_(get_timeout),
    u_(get_timeout),
    u_(get_tmp_dir),
    u_(get_tx_max),
    u_(get_tx_timestamp),
    u_(get_verbose),
    u_(lock_detect),
    u_(lock_get),
    u_(lock_id),
    u_(lock_id_free),
    u_(lock_put),
    u_(lock_stat),
    u_(lock_stat_print),
    u_(lock_vec),
    u_(log_archive),
    u_(log_cursor),
    u_(log_file),
    u_(log_flush),
    u_(log_get_config),
    u_(log_printf),
    u_(log_put),
    u_(log_set_config),
    u_(log_stat),
    u_(log_stat_print),
    u_(log_verify),
    u_(lsn_reset),
    u_(memp_fcreate),
    u_(memp_register),
    u_(memp_stat),
    u_(memp_stat_print),
    u_(memp_sync),
    u_(memp_trickle),
    u_(mutex_alloc),
    u_(mutex_free),
    u_(mutex_get_align),
    u_(mutex_get_increment),
    u_(mutex_get_max),
    u_(mutex_get_tas_spins),
    u_(mutex_lock),
    u_(mutex_set_align),
    u_(mutex_set_increment),
    u_(mutex_set_max),
    u_(mutex_set_tas_spins),
    u_(mutex_stat),
    u_(mutex_stat_print),
    u_(mutex_unlock),
    u_(open),
    u_(remove),
    u_(rep_elect),
    u_(rep_get_clockskew),
    u_(rep_get_config),
    u_(rep_get_limit),
    u_(rep_get_nsites),
    u_(rep_get_priority),
    u_(rep_get_request),
    u_(rep_get_timeout),
    u_(rep_process_message),
    u_(rep_set_clockskew),
    u_(rep_set_config),
    u_(rep_set_limit),
    u_(rep_set_nsites),
    u_(rep_set_priority),
    u_(rep_set_request),
    u_(rep_set_timeout),
    u_(rep_set_transport),
    u_(rep_start),
    u_(rep_stat),
    u_(rep_stat_print),
    u_(rep_sync),
    u_(repmgr_add_remote_site),
    u_(repmgr_get_ack_policy),
    u_(repmgr_set_ack_policy),
    u_(repmgr_set_local_site),
    u_(repmgr_site_list),
    u_(repmgr_start),
    u_(repmgr_stat),
    u_(repmgr_stat_print),
    u_(set_alloc),
    u_(set_app_dispatch),
    u_(set_cache_max),
    u_(set_cachesize),
    u_(set_cachesize),
    u_(set_create_dir),
    u_(set_data_dir),
    u_(set_encrypt),
    u_(set_errcall),
    u_(set_errfile),
    u_(set_errpfx),
    u_(set_event_notify),
    u_(set_feedback),
    u_(set_flags),
    u_(set_intermediate_dir_mode),
    u_(set_isalive),
    u_(set_lg_bsize),
    u_(set_lg_dir),
    u_(set_lg_filemode),
    u_(set_lg_max),
    u_(set_lg_regionmax),
    u_(set_lk_conflicts),
    u_(set_lk_detect),
    u_(set_lk_max_lockers),
    u_(set_lk_max_locks),
    u_(set_lk_max_objects),
    u_(set_lk_partitions),
    u_(set_lk_priority),
    u_(set_mp_max_openfd),
    u_(set_mp_max_write),
    u_(set_mp_mmapsize),
    u_(set_mp_pagesize),
    u_(set_mp_tablesize),
    u_(set_msgcall),
    u_(set_msgfile),
    u_(set_shm_key),
    u_(set_thread_count),
    u_(set_thread_id),
    u_(set_thread_id_string),
    u_(set_timeout),
    u_(set_timeout),
    u_(set_timeout),
    u_(set_tmp_dir),
    u_(set_tx_max),
    u_(set_tx_timestamp),
    u_(set_verbose),
    u_(stat_print),
    u_(txn_applied),
    u_(txn_begin),
    u_(txn_checkpoint),
    u_(txn_recover),
    u_(txn_stat),
    u_(txn_stat_print),
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
