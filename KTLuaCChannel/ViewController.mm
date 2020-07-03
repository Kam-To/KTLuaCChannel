//
//  ViewController.m
//  KTLuaCChannel
//
//  Created by cc on 2020/7/2.
//  Copyright © 2020 kam. All rights reserved.
//

#import "ViewController.h"
#include "KTLuaCChannel.h"

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


@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self LuaCallNativeFunction];
    [self NativeCallLuaFunction];
}

static int KTNativeFunction(lua_State *state) {
    // TODO: wrap
    int parameterCount = lua_gettop(state);
    for (int i = 1; i <= parameterCount; i++) {
        int type = lua_type(state, i);
        printf("Got parameter for Lua (type %d)\n", type);
    }
    return 0; // return count anything
}

- (void)LuaCallNativeFunction {
    lua_State *state = KT::OpenLuaState();
    
    const char *functionInLuaName = "callingInLua";
    KT::RegisterNativeFuntionToLua(state, functionInLuaName, KTNativeFunction);
    
    const char *script = "callingInLua(32, \"string\");";
    KT::EvaluateScriptInLua(state, script);
    
    KT::CloseLuaState(state);
}

- (void)NativeCallLuaFunction {
    
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
    
    function callByNativeNoParameter()
        print("Just print something.");
    end
    )";

    KT::EvaluateScriptInLua(state, script);
    
    KT::CallLuaFunction(state, "callByNativeNoParameter", KTLuaWrappedNull());
    
    NativeObject *obj = new NativeObject();
    obj->floatMember = 7.0;
    obj->integerMember = 12;
    obj->boolMember = false;
    KT::CallLuaFunction(state, "callByNative", KTLuaWrapped(NativeObject, obj));
    printf("member after updated(%lf, %lld, %d)", obj->floatMember, obj->integerMember, obj->boolMember);
    delete obj;
}

@end
