/* Source-only option gate control: cc -Iport/n64/src tests/test_n64_pc_content.c -o pc-content-test */
#include <assert.h>
#include "n64_pc_content.h"

int main(void)
{
    static const int equipment[] = {0, 0, 0, 0, 1};
    static const int vehicles[] = {0, 0, 1, 1};
    int i;
    assert(N64_WEAPONS_PC_ADDITIVE == 4);
    assert(N64_PC_EQUIPMENT_FLAG == 0x40000000u);
    assert(N64_PC_VEHICLE_FLAG == 0x4000u);
    for (i = 0; i < 5; ++i)
    {
        assert(n64_pc_equipment_enabled(i, 0) == equipment[i]);
        assert(!n64_pc_equipment_enabled(i, 1));
    }
    for (i = 0; i < 4; ++i)
    {
        assert(n64_pc_vehicle_enabled(i, 0) == vehicles[i]);
        assert(!n64_pc_vehicle_enabled(i, 1));
    }
    assert(!n64_pc_equipment_enabled(-1, 0));
    assert(!n64_pc_equipment_enabled(5, 0));
    assert(!n64_pc_vehicle_enabled(-1, 0));
    assert(!n64_pc_vehicle_enabled(4, 0));
    return 0;
}
