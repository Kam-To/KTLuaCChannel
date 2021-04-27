# KTLuaCChannel
A simple wrapper to exchange data between Lua and C/C++.

# Usage

Define a C++ object and it's Lua meta data and accessor:

```cpp
class NativeObject {
public:
    float floatMember;
    long long integerMember;
    bool boolMember;
};

KTLuaFloatMemberAccessor(NativeObject, floatMember);
KTLuaIntegerMemberAccessor(NativeObject, integerMember);
KTLuaBoolMemberAccessor(NativeObject, boolMember);
KTLuaCreateMeta(
                NativeObject,
                KTLuaMetaEntry(NativeObject, floatMember),
                KTLuaMetaEntry(NativeObject, integerMember),
                KTLuaMetaEntry(NativeObject, boolMember),
                );
```

Then we define a `callByNative` function in Lua environment, and call it from host-side:

```cpp
   
lua_State *state = KT::OpenLuaState();
const char *script = R"(
function callByNative(obj)
    local v = obj:floatMember();
    obj:floatMember(v / 2);

    local i = obj:integerMember();
    i = i + 12;
    obj:integerMember(i);

    local b = obj:boolMember();
    obj:boolMember(true);
end
)";
KT::EvaluateScriptInLua(state, script);
    
NativeObject *obj = new NativeObject();
obj->floatMember = 7.0;
obj->integerMember = 12;
obj->boolMember = false;
KT::CallLuaFunction(state, "callByNative", KTLuaWrapped(NativeObject, obj));
printf("member after updated(%lf, %lld, %d)", obj->floatMember, obj->integerMember, obj->boolMember);
delete obj;
```

👆It would output the value modified in Lua environment.
