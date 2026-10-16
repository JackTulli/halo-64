"""Synthetic regression for the native PC vehicle-placement game-type gate.

Run from the repository with ``python -m unittest tools.n64.test_pc_vehicle_mode``.
Requires a host GCC-compatible C compiler (CC, or cc/gcc/clang on PATH).
HALO_SOURCE_ROOT optionally selects another source checkout. No maps, converted
assets, SDK, emulator, or retail data are needed. Temporary binaries are removed.
"""
from pathlib import Path
import os
import re
import shlex
import shutil
import subprocess
import tempfile
import unittest


def c_block(source, declaration):
    """Extract one actual definition, ignoring braces in comments and strings."""
    match = re.search(declaration, source)
    if not match:
        raise AssertionError("Missing C definition: " + declaration)
    start = source.index("{", match.end())
    token = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|[{}]', re.S)
    depth = 0
    for brace in token.finditer(source, start):
        if brace.group() == "{":
            depth += 1
        elif brace.group() == "}":
            depth -= 1
            if depth == 0:
                return source[match.start():brace.end()]
    raise AssertionError("Unclosed C definition: " + declaration)


PREAMBLE = r'''
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef unsigned char byte;
typedef int boolean;
#define TRUE 1
#define FALSE 0
#define NONE -1
#define TEST_FLAG(v,b) ((v) & (1u << (b)))
#define _scenario_object_placement_not_automatic_bit 0
#define CHECK(c) do { if (!(c)) { fprintf(stderr,"line %d: %s\n",__LINE__,#c); exit(1); } } while (0)
struct vector {float x,y,z;};
/* Only the fields used by the extracted function are stubbed. The synthetic
 * row retains the native 120-byte extent and flags at bytes 90..91. */
struct scenario_object_datum {
    int16_t palette_entry_index, name_index;
    uint16_t placement_flags;
    int16_t variant_number;
    struct vector position, rotation;
    byte remaining[88];
};
_Static_assert(sizeof(struct scenario_object_datum) == 120, "synthetic row extent");
struct object_placement_data {struct vector position,forward,up;int16_t variant_number;};
struct scenario_object_palette_entry {struct {long index;} reference;};
struct tag_block {struct scenario_object_palette_entry *entries;long count;void *address;};
struct scenario {struct tag_block vehicle_palette,vehicles;};
struct object_type_definition {long placement_tag_block_element_size;};
struct object_type_definition vehicle_data_definition={120};
struct globals_stub {int initial_placement;};
struct variant_stub {short game_engine_index;};
static struct scenario scenario;
static struct globals_stub globals;
static struct globals_stub *object_globals = &globals;
static struct variant_stub variant;
static struct {int firefight,vehicle_set;} n64_choice;
static struct {char name[64];} n64_map;
static int engine_running, existing_name, prepare_calls, allocation_calls, place_calls, name_calls;
static long allocated_object;
static int optional_content_enabled;
static struct scenario *global_scenario_get(void) {return &scenario;}
static int game_engine_running(void) {return engine_running;}
static struct variant_stub *game_engine_get_variant(void) {return &variant;}
/* The independent optional-content policy is an external dependency here. */
int n64_game_pc_vehicle_enabled(void) {return optional_content_enabled;}
#define TAG_BLOCK_GET_ELEMENT(p,i,t) (&(p)->entries[i])
static long object_name_list_lookup(short n) {(void)n;return existing_name;}
static void object_placement_data_new(struct object_placement_data *p,long d,long n)
{(void)p;(void)d;(void)n;prepare_calls++;}
static void vectors3d_from_euler_angles3d(struct vector *f,struct vector *u,struct vector *r)
{(void)f;(void)u;(void)r;}
static long object_new(struct object_placement_data *p) {(void)p;allocation_calls++;return allocated_object;}
static void object_type_place(long n,struct scenario_object_datum *p) {(void)n;(void)p;place_calls++;}
static void object_name_list_new(long n,short name) {(void)n;(void)name;name_calls++;}
'''


CASES = r'''
static struct scenario_object_palette_entry entry;
static struct tag_block other_palette;
static struct scenario_object_datum row;
static unsigned checks;
static void reset(const char *map,short engine,unsigned mask)
{
    memset(&row,0,sizeof(row));row.name_index=NONE;
    ((byte *)&row)[90]=(byte)(mask >> 8);((byte *)&row)[91]=(byte)mask;
    strcpy(n64_map.name,map);variant.game_engine_index=engine;
    entry.reference.index=42;scenario.vehicle_palette.entries=&entry;other_palette.entries=&entry;
    globals.initial_placement=TRUE;engine_running=TRUE;n64_choice.firefight=FALSE;
    n64_choice.vehicle_set=0;scenario.vehicles.count=0;scenario.vehicles.address=NULL;
    scenario.vehicle_palette.count=1;vehicle_data_definition.placement_tag_block_element_size=120;
    existing_name=NONE;optional_content_enabled=TRUE;allocated_object=17;
}
static void expect(struct tag_block *palette,int admitted)
{
    prepare_calls=allocation_calls=place_calls=name_calls=0;
    CHECK(object_new_from_scenario(&row,palette)==(admitted ? 17 : NONE));
    /* Rejected rows must never begin placement setup or allocate an object. */
    CHECK(prepare_calls==admitted);CHECK(allocation_calls==admitted);CHECK(place_calls==admitted);
    CHECK(name_calls==(admitted && row.name_index!=NONE));checks++;
}
static void modes(void)
{
    /* Each synthetic row allows exactly one named game type. These constants
     * describe the HEK ScenarioVehicle flags, not implementation switch cases. */
    static const struct {short engine;unsigned flag;} one_type[]={
        {game_engine_slayer,0x0100}, {game_engine_ctf,0x0200},
        {game_engine_king,0x0400}, {game_engine_oddball,0x0800}
    };
    for(unsigned row_type=0;row_type<4;row_type++)for(unsigned active=0;active<4;active++){
        reset("deathisland",one_type[active].engine,one_type[row_type].flag);
        expect(&scenario.vehicle_palette,row_type==active);
    }
    for(unsigned active=0;active<4;active++){
        reset("deathisland",one_type[active].engine,0x000f);expect(&scenario.vehicle_palette,FALSE);
        reset("deathisland",one_type[active].engine,0x0000);expect(&scenario.vehicle_palette,FALSE);
        reset("deathisland",one_type[active].engine,0xf000);expect(&scenario.vehicle_palette,FALSE);
        reset("deathisland",one_type[active].engine,0x0f00);expect(&scenario.vehicle_palette,TRUE);
        reset("deathisland",one_type[active].engine,0x0f0f);expect(&scenario.vehicle_palette,TRUE);
    }
    reset("deathisland",game_engine_ctf,0x0002);expect(&scenario.vehicle_palette,FALSE);
    reset("deathisland",game_engine_slayer,0x0202);expect(&scenario.vehicle_palette,FALSE);
    reset("deathisland",game_engine_ctf,0x0202);expect(&scenario.vehicle_palette,TRUE);
}
static void map_scope(void)
{
    static const char *pc[]={"dangercanyon","deathisland","gephyrophobia","icefields","infinity","timberland"};
    static const char *original[]={"beavercreek","bloodgulch","boardingaction","carousel","chillout",
        "damnation","hangemhigh","longest","prisoner","putput","ratrace","sidewinder","wizard"};
    for(unsigned i=0;i<6;i++){
        reset(pc[i],game_engine_slayer,0x0200);expect(&scenario.vehicle_palette,FALSE);
        reset(pc[i],game_engine_slayer,0x0100);expect(&scenario.vehicle_palette,TRUE);
    }
    for(unsigned i=0;i<13;i++){
        reset(original[i],game_engine_slayer,0);expect(&scenario.vehicle_palette,TRUE);
        reset(original[i],game_engine_ctf,0x0100);expect(&scenario.vehicle_palette,TRUE);
    }
    reset("custommap",game_engine_slayer,0);expect(&scenario.vehicle_palette,TRUE);
    reset("deathisland_extra",game_engine_slayer,0);expect(&scenario.vehicle_palette,TRUE);
    reset("",game_engine_slayer,0);expect(&scenario.vehicle_palette,TRUE);
}
static void bypasses(void)
{
    reset("deathisland",game_engine_slayer,0);globals.initial_placement=FALSE;expect(&scenario.vehicle_palette,TRUE);
    reset("deathisland",game_engine_slayer,0);globals.initial_placement=FALSE;row.placement_flags=1;expect(&scenario.vehicle_palette,TRUE);
    reset("deathisland",game_engine_slayer,0);expect(&other_palette,TRUE);
    reset("deathisland",game_engine_slayer,0);n64_choice.firefight=TRUE;expect(&scenario.vehicle_palette,TRUE);
    reset("deathisland",game_engine_slayer,0);engine_running=FALSE;expect(&scenario.vehicle_palette,TRUE);
    reset("deathisland",game_engine_none,0);expect(&scenario.vehicle_palette,TRUE);
    reset("deathisland",game_engine_race,0);expect(&scenario.vehicle_palette,TRUE);
    reset("deathisland",number_of_game_engines+1,0);expect(&scenario.vehicle_palette,TRUE);
}
static void lifecycle(void)
{
    reset("deathisland",game_engine_slayer,0x0100);row.placement_flags=1;expect(&scenario.vehicle_palette,FALSE);
    reset("deathisland",game_engine_slayer,0x0100);row.palette_entry_index=NONE;expect(&scenario.vehicle_palette,FALSE);
    reset("deathisland",game_engine_slayer,0x0100);entry.reference.index=NONE;expect(&scenario.vehicle_palette,FALSE);
    reset("deathisland",game_engine_slayer,0x0100);row.name_index=3;existing_name=17;expect(&scenario.vehicle_palette,FALSE);
    reset("deathisland",game_engine_slayer,0x0100);row.name_index=3;expect(&scenario.vehicle_palette,TRUE);
    reset("deathisland",game_engine_slayer,0x0100);allocated_object=NONE;
    prepare_calls=allocation_calls=place_calls=name_calls=0;
    CHECK(object_new_from_scenario(&row,&scenario.vehicle_palette)==NONE);
    CHECK(prepare_calls==1 && allocation_calls==1 && place_calls==0 && name_calls==0);checks++;
}
static void build_guards(void)
{
    reset("deathisland",game_engine_slayer,0x0200);expect(&scenario.vehicle_palette,TRUE);
    reset("deathisland",game_engine_ctf,0);expect(&scenario.vehicle_palette,TRUE);
}
static void optional_gate(void)
{
#ifdef HAVE_OPTIONAL_CONTENT_GATE
    reset("bloodgulch",game_engine_slayer,0);row.placement_flags=N64_PC_VEHICLE_FLAG;expect(&scenario.vehicle_palette,TRUE);
    optional_content_enabled=FALSE;expect(&scenario.vehicle_palette,FALSE);
    reset("deathisland",game_engine_slayer,0x0200);row.placement_flags=N64_PC_VEHICLE_FLAG;expect(&scenario.vehicle_palette,FALSE);
    reset("deathisland",game_engine_slayer,0x0100);row.placement_flags=N64_PC_VEHICLE_FLAG;expect(&scenario.vehicle_palette,TRUE);
    optional_content_enabled=FALSE;expect(&scenario.vehicle_palette,FALSE);expect(&other_palette,TRUE);
#endif
}
static void crowded_pc_map(void)
{
#if defined(HALO_N64_PC_EXPANSION) && !defined(HALO_N64_CAMPAIGN)
    struct scenario_object_datum rows[20];
    struct scenario_object_palette_entry palette[6];
    reset("icefields",game_engine_slayer,0x0f00);
    for(int k=0;k<6;k++)palette[k].reference.index=100+(k%5);
    scenario.vehicle_palette.entries=palette;scenario.vehicle_palette.count=6;
    scenario.vehicles.address=rows;scenario.vehicles.count=20;n64_choice.vehicle_set=2;
    for(int i=0;i<20;i++){
        rows[i]=row;rows[i].palette_entry_index=i%5;
        ((byte*)&rows[i])[88]=(i/5)%2;
    }
    for(int i=0;i<20;i++){
        allocation_calls=0;
        CHECK(object_new_from_scenario(&rows[i],&scenario.vehicle_palette)==(i<10?17:NONE));
        CHECK(allocation_calls==(i<10));checks++;
    }
    /* Renewal uses stable authored rank, even with no live predecessor. */
    CHECK(n64_game_scenario_vehicle_allowed(&rows[0]));
    CHECK(!n64_game_scenario_vehicle_allowed(&rows[10]));
    n64_choice.vehicle_set=3;CHECK(!n64_game_scenario_vehicle_allowed(&rows[10]));
    /* Palette aliases refer to the same vehicle definition. */
    rows[10].palette_entry_index=5;CHECK(!n64_game_scenario_vehicle_allowed(&rows[10]));
    /* Ineligible earlier pads must not suppress the first eligible pad. */
    rows[0].placement_flags=1;CHECK(n64_game_scenario_vehicle_allowed(&rows[10]));
    rows[0].placement_flags=0;((byte*)&rows[0])[90]=2;
    CHECK(n64_game_scenario_vehicle_allowed(&rows[10]));
    ((byte*)&rows[0])[90]=15;
    n64_choice.vehicle_set=0;CHECK(n64_game_scenario_vehicle_allowed(&rows[10]));
    n64_choice.vehicle_set=2;strcpy(n64_map.name,"bloodgulch");
    CHECK(n64_game_scenario_vehicle_allowed(&rows[10]));
    strcpy(n64_map.name,"icefields");CHECK(!n64_game_scenario_vehicle_allowed(&row));
    scenario.vehicles.count=513;CHECK(!n64_game_scenario_vehicle_allowed(&rows[0]));
    scenario.vehicles.count=20;vehicle_data_definition.placement_tag_block_element_size=119;
    CHECK(!n64_game_scenario_vehicle_allowed(&rows[0]));
#endif
}
int main(int argc,char **argv)
{
    CHECK(argc==2);
    if(!strcmp(argv[1],"modes"))modes();
    else if(!strcmp(argv[1],"maps"))map_scope();
    else if(!strcmp(argv[1],"bypasses"))bypasses();
    else if(!strcmp(argv[1],"lifecycle"))lifecycle();
    else if(!strcmp(argv[1],"guards"))build_guards();
    else if(!strcmp(argv[1],"optional"))optional_gate();
    else if(!strcmp(argv[1],"crowding"))crowded_pc_map();
    else CHECK(FALSE);
    printf("PASS %s: %u placement checks\n",argv[1],checks);
    return 0;
}
'''


class VehicleModePlacementTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        root = Path(os.environ.get("HALO_SOURCE_ROOT", Path(__file__).resolve().parents[2]))
        game = (root / "port/n64/game/n64_game.c").read_text()
        objects = (root / "source/objects/objects.c").read_text()
        engine = (root / "source/game/game_engine.h").read_text()
        actual_enum = c_block(engine, r"enum\s+game_engine_type\s*(?=\{)") + ";\n"
        helper = c_block(game, r"static\s+boolean\s+n64_pc_vehicle_spawn_allowed\s*\([^;]*?\)")
        bridge = c_block(game, r"boolean\s+n64_game_scenario_vehicle_allowed\s*\([^;]*?\)")
        placement = c_block(objects, r"long\s+object_new_from_scenario\s*\([^;]*?\)")
        optional = "n64_game_pc_vehicle_enabled" in placement
        optional_header = ""
        if optional:
            optional_header = (root / "port/n64/src/n64_pc_content.h").read_text()
            optional_header += "\n#define HAVE_OPTIONAL_CONTENT_GATE 1\n"
        # Retain the actual surrounding guard, so a guard regression is visible.
        helper_start = game.index(helper)
        bridge_end = game.index(bridge) + len(bridge)
        guard_start = game.rfind("\n#if", 0, helper_start)
        guard_end = game.index("#endif", bridge_end) + len("#endif")
        if guard_start < 0 or "#endif" in game[guard_start:helper_start]:
            raise AssertionError("Vehicle policy functions lost their compile guard")
        guarded_helper = game[guard_start:guard_end] + "\n"
        text = PREAMBLE + actual_enum + optional_header + guarded_helper + placement + CASES
        configured = os.environ.get("CC")
        if configured:
            compiler = [configured] if Path(configured).is_file() else shlex.split(configured)
        else:
            located = next((shutil.which(name) for name in ("cc", "gcc", "clang") if shutil.which(name)), None)
            if not located:
                raise unittest.SkipTest("A GCC-compatible host C compiler is required (set CC).")
            compiler = [located]
        cls.temp = tempfile.TemporaryDirectory(prefix="halo-vehicle-mode-")
        cls.addClassCleanup(cls.temp.cleanup)
        folder = Path(cls.temp.name)
        source = folder / "placement.c"
        source.write_text(text)
        cls.executables = {}
        cls.optional = optional
        for variant, defines in {
            "pc": ["HALO_N64_PC_EXPANSION"],
            "vanilla": [],
            "campaign": ["HALO_N64_CAMPAIGN"],
            "campaign_pc": ["HALO_N64_CAMPAIGN", "HALO_N64_PC_EXPANSION"],
        }.items():
            executable = folder / (variant + (".exe" if os.name == "nt" else ""))
            command = compiler + ["-std=c11", "-O2"] + ["-D" + define for define in defines] + [str(source), "-o", str(executable)]
            compiled = subprocess.run(command, capture_output=True, text=True, timeout=60)
            if compiled.returncode:
                raise AssertionError("C fixture compilation failed:\n" + compiled.stdout + compiled.stderr)
            cls.executables[variant] = executable

    def run_case(self, case, variant="pc"):
        result = subprocess.run([str(self.executables[variant]), case], capture_output=True, text=True, timeout=15)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_allowed_bits_are_distinct_from_defaults_and_other_game_types(self):
        self.run_case("modes")

    def test_pc_all_map_limits_duplicate_pads_and_preserves_default(self):
        self.run_case("crowding")

    def test_six_pc_maps_filter_and_thirteen_original_maps_bypass(self):
        self.run_case("maps")

    def test_scripted_other_palette_firefight_and_unsupported_engine_bypass(self):
        self.run_case("bypasses")

    def test_existing_placement_lifecycle_and_allocation_failure(self):
        self.run_case("lifecycle")

    def test_vanilla_and_campaign_compilation_boundaries(self):
        for variant in ("vanilla", "campaign", "campaign_pc"):
            with self.subTest(variant=variant):
                self.run_case("guards", variant)

    def test_optional_content_gate_composes_before_allocation(self):
        if not self.optional:
            self.skipTest("This checkout has no independent optional-content gate.")
        self.run_case("optional")


if __name__ == "__main__":
    unittest.main()
