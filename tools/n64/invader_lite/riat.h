/* invader-lite: stand-in for Rat In a Tube's C header (the riat submodule is
   Rust and is not built: no Cargo/Corrosion needed). Only the names Invader's
   sources use. */
#ifndef RIAT_STUB_H
#define RIAT_STUB_H
#include <stddef.h>
#include <stdint.h>
typedef enum RIATCompileTarget {
    RIAT_HaloCEXboxNTSC,
    RIAT_HaloCEGBX,
    RIAT_HaloCEGBXDemo,
    RIAT_HaloCustomEdition,
    RIAT_HaloCEA
} RIATCompileTarget;
typedef enum RIATNodeTypeC {
    RIAT_FunctionCall,
    RIAT_ScriptCall,
    RIAT_GlobalVariable,
    RIAT_LocalVariable,
    RIAT_StaticValue,
    RIAT_Primitive
} RIATNodeTypeC;
#endif
