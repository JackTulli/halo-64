/* invader-lite: stand-in for Rat In a Tube's C++ API. compile_scripts()
   yields no scripts, globals or nodes: a scenario built with this has no
   scripts (the N64 port does not run Halo scripts: source/hs is excluded). */
#ifndef RIAT_STUB_HPP
#define RIAT_STUB_HPP
#include "riat.h"
#include <vector>
#include <cstdint>
#include <cstddef>
#include <stdexcept>
namespace RIAT {
    struct CompileError {
        const char *get_file() const noexcept { return ""; }
        std::size_t get_line() const noexcept { return 0; }
        std::size_t get_column() const noexcept { return 0; }
        const char *get_reason() const noexcept { return ""; }
    };
    struct Node {
        RIATNodeTypeC node_type;
        int value_type;
        std::uint16_t index_union;
        union { std::int32_t long_int; bool boolean; std::int16_t short_int; float real; std::size_t offset; } node_data;
        std::size_t next_node;
        const char *string_data;
        const char *file;
        std::size_t line;
        std::size_t column;
    };
    struct Script { const char *name; int return_type; int script_type; std::size_t first_node; };
    struct Global { const char *name; int value_type; std::size_t first_node; };
    struct ScriptParameter { const char *name; int value_type; };
    class CompilerScriptResult {
    public:
        std::vector<CompileError> get_warnings() const { return {}; }
        std::vector<Script> get_scripts() const { return {}; }
        std::vector<Global> get_globals() const { return {}; }
        std::vector<Node> get_nodes() const { return {}; }
        std::vector<ScriptParameter> get_script_parameters_for_script(const Script &) const { return {}; }
    };
    class Compiler {
    public:
        explicit Compiler(RIATCompileTarget) {}
        void read_script_data(const std::uint8_t *, std::size_t, const char *) {}
        CompilerScriptResult compile_scripts() { return {}; }
    };
}
#endif
