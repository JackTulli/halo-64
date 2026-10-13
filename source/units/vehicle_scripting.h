#ifndef __VEHICLE_SCRIPTING_H
#define __VEHICLE_SCRIPTING_H

short vehicle_scripting_find_available_seats(
	long vehicle_index,
	char const *seat_substring_name,
	short seat_desire_type,
	short *seat_indices,
	short maximum_seat_count);

#endif // __VEHICLE_SCRIPTING_H
