//
//  KTLuaCChannel.cpp
//  KTLuaCChannel
//
//  Created by cc on 2020/7/3.
//  Copyright © 2020 kam. All rights reserved.
//

#include "KTLuaCChannel.h"


lua_State *KT::OpenLuaState() {
    lua_State *state = luaL_newstate();
    luaL_openlibs(state);
    return state;
}

void KT::CloseLuaState(lua_State *l) {
    if (!l) return;
    lua_close(l);
}

void KT::EvaluateScriptInLua(lua_State *state, const char *luaScript) {
    luaL_loadstring(state, luaScript);
    lua_call(state, 0, 0);
}

void KT::RegisterNativeFuntionToLua(lua_State *state, const char *funcName, lua_CFunction funcPtr) {
    lua_register(state, funcName, funcPtr);
}

void KT::CallLuaFunction(lua_State *state, const char *functionName, void *obj, const luaL_Reg *meta, int entryNum) {
    lua_getglobal(state, functionName);
    int nargs = 0;
    if (obj) {
        nargs++;
        void **ptr = (void **)lua_newuserdata(state, sizeof(obj));
        *ptr = obj;
        lua_createtable(state, 0, entryNum);
        luaL_setfuncs(state, meta, 0);
        
        lua_pushvalue(state, -1);
        lua_setfield(state, -2, "__index");
        lua_setmetatable(state, -2);
    }
    lua_call(state, nargs, 0);
}

