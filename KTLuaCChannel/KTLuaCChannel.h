//
//  KTLuaCChannel.hpp
//  KTLuaCChannel
//
//  Created by cc on 2020/7/3.
//  Copyright © 2020 kam. All rights reserved.
//

#ifndef KTLuaCChannel_hpp
#define KTLuaCChannel_hpp

#include "lua.hpp"

// concatenate class name and member name
#define KTLuaMemberFunctionName(className, memberName)\
className##_##memberName

// create accessor (getter and setter) for class member
#define KTLuaBaseMemberAccessor(className, memberName, pushFunc, popFunc)\
int KTLuaMemberFunctionName(className, memberName)(lua_State *l) {\
    int pCount = lua_gettop(l);\
    className **ptr = (className **)lua_touserdata(l, 1);\
    className *obj = *ptr;\
    int retCnt = 0;\
    if (pCount == 1) {\
        pushFunc(l, obj->memberName);\
        retCnt = 1;\
    } else if (pCount == 2) {\
        obj->memberName = popFunc(l, 2);\
    }\
    return retCnt;\
}
// float member
#define KTLuaFloatMemberAccessor(className, memberName) KTLuaBaseMemberAccessor(className, memberName, lua_pushnumber, lua_tonumber)
// int member
#define KTLuaIntegerMemberAccessor(className, memberName) KTLuaBaseMemberAccessor(className, memberName, lua_pushinteger, lua_tointeger)
// bool member
#define KTLuaBoolMemberAccessor(className, memberName) KTLuaBaseMemberAccessor(className, memberName, lua_pushboolean, lua_toboolean)


// create meta entry for class member
#define KTLuaMetaEntry(className, entryName)\
{#entryName, KTLuaMemberFunctionName(className, entryName)}\

// create meta with entry
#define KTLuaCreateMeta(className, ...)\
static const struct luaL_Reg className##Meta[] = {\
__VA_ARGS__\
{nullptr, nullptr}\
}

// parameter wrapper
#define KTLuaWrapped(class, objPtr)\
objPtr, class##Meta, (sizeof(class##Meta) / sizeof(class##Meta[0]) - 1)

// null parameter wrapper
#define KTLuaWrappedNull()\
NULL, NULL, 0


namespace KT {
    lua_State *OpenLuaState();
    void CloseLuaState(lua_State *l);
    void EvaluateScriptInLua(lua_State *state, const char *luaScript);
    void RegisterNativeFuntionToLua(lua_State *state, const char *funcName, lua_CFunction funcPtr);
    void CallLuaFunction(lua_State *state, const char *functionName, void *obj, const luaL_Reg *meta, int entryNum);
}


#endif /* KTLuaCChannel_hpp */
