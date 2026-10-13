/*
AI_SCRIPT.C

symbols in this file:
00042690 0090:
	_encounter_get_platoon (0000)
00042720 0010:
	_ai_script_initialize (0000)
00042730 0010:
	_ai_script_dispose (0000)
00042740 0010:
	_ai_script_initialize_for_new_map (0000)
00042750 0010:
	_ai_script_dispose_from_old_map (0000)
00042760 0130:
	_ai_index_from_string (0000)
00042890 00f0:
	_ai_index_to_string (0000)
00042980 0120:
	_ai_index_platoon_iterator_new (0000)
00042AA0 0070:
	_ai_index_platoon_iterator_next (0000)
00042B10 0100:
	_ai_index_squad_iterator_new (0000)
00042C10 00e0:
	_ai_index_squad_iterator_next (0000)
00042CF0 00d0:
	_ai_index_actor_iterator_new (0000)
00042DC0 0070:
	_ai_index_actor_iterator_next (0000)
00042E30 00a0:
	_object_list_from_ai_reference (0000)
00042ED0 0220:
	_ai_scripting_attach_unit (0000)
000430F0 0040:
	_ai_scripting_attach_units (0000)
00043130 0060:
	_ai_scripting_detach_unit (0000)
00043190 0090:
	_ai_scripting_detach_units (0000)
00043220 0090:
	_ai_scripting_place (0000)
000432B0 0060:
	_ai_scripting_kill_internal (0000)
00043310 0060:
	_ai_scripting_kill (0000)
00043370 0060:
	_ai_scripting_kill_silent (0000)
000433D0 0090:
	_ai_scripting_erase (0000)
00043460 0030:
	_ai_scripting_erase_all (0000)
00043490 0020:
	_ai_scripting_deselect (0000)
000434B0 0040:
	_ai_scripting_select (0000)
000434F0 0110:
	_ai_scripting_spawn_actor (0000)
00043600 0080:
	_ai_scripting_set_respawn (0000)
00043680 0080:
	_ai_scripting_set_deaf (0000)
00043700 0080:
	_ai_scripting_set_blind (0000)
00043780 00d0:
	_ai_scripting_magically_see_unit (0000)
00043850 0040:
	_ai_scripting_magically_see_units (0000)
00043890 0090:
	_ai_scripting_timer_start (0000)
00043920 00a0:
	_ai_scripting_timer_expire (0000)
000439C0 02d0:
	_ai_scripting_count_internal (0000)
00043C90 0020:
	_ai_scripting_swarm_count (0000)
00043CB0 0020:
	_ai_scripting_nonswarm_count (0000)
00043CD0 0020:
	_ai_scripting_living_count (0000)
00043CF0 0040:
	_ai_scripting_living_fraction (0000)
00043D30 0030:
	_ai_scripting_strength (0000)
00043D60 0060:
	_ai_scripting_is_attacking (0000)
00043DC0 0090:
	_ai_scripting_attack (0000)
00043E50 0090:
	_ai_scripting_defend (0000)
00043EE0 0090:
	_ai_scripting_maneuver (0000)
00043F70 00a0:
	_ai_scripting_maneuver_enable (0000)
00044010 0430:
	_code_00044010 (0000)
00044440 0550:
	_code_00044440 (0000)
00044990 00a0:
	_ai_scripting_migrate (0000)
00044A30 00f0:
	_code_00044a30 (0000)
00044B20 0110:
	_ai_scripting_migrate_by_unit (0000)
00044C30 00e0:
	_ai_scripting_migrate_and_speak (0000)
00044D10 00f0:
	_ai_scripting_allegiance (0000)
00044E00 0050:
	_ai_scripting_allegiance_remove (0000)
00044E50 0050:
	_ai_scripting_allegiance_broken (0000)
00044EA0 0050:
	_ai_scripting_vehicle_candidate_qsort (0000)
00044EF0 0060:
	_ai_scripting_going_to_vehicle (0000)
00044F50 00a0:
	_ai_scripting_exit_vehicle (0000)
00044FF0 00a0:
	_ai_scripting_braindead (0000)
00045090 0100:
	_ai_scripting_braindead_by_unit (0000)
00045190 00a0:
	_ai_scripting_ignore (0000)
00045230 00a0:
	_ai_scripting_prefer_target (0000)
000452D0 0120:
	_code_000452d0 (0000)
000453F0 0060:
	_ai_scripting_teleport_starting_location_if_unsupported (0000)
00045450 0060:
	_ai_scripting_teleport_starting_location (0000)
000454B0 0090:
	_ai_scripting_try_to_fight_nothing (0000)
00045540 00d0:
	_ai_scripting_try_to_fight (0000)
00045610 0090:
	_ai_scripting_try_to_fight_player (0000)
000456A0 00a0:
	_ai_scripting_allow_charge (0000)
00045740 00c0:
	_ai_scripting_command_list (0000)
00045800 00a0:
	_ai_scripting_command_list_by_unit (0000)
000458A0 0090:
	_ai_scripting_command_list_advance (0000)
00045930 0070:
	_ai_scripting_command_list_advance_by_unit (0000)
000459A0 0050:
	_code_000459a0 (0000)
000459F0 0250:
	_ai_scripting_command_list_status (0000)
00045C40 00d0:
	_ai_scripting_free (0000)
00045D10 00d0:
	_ai_scripting_free_units (0000)
00045DE0 00e0:
	_ai_scripting_attach_free (0000)
00045EC0 00b0:
	_ai_scripting_force_active (0000)
00045F70 00d0:
	_ai_scripting_force_active_by_unit (0000)
00046040 00d0:
	_ai_scripting_set_return_state (0000)
00046110 00a0:
	_ai_scripting_set_current_state (0000)
000461B0 0080:
	_ai_scripting_assess_status (0000)
00046230 00a0:
	_ai_scripting_status (0000)
000462D0 0010:
	_ai_scripting_reconnect (0000)
000462E0 0090:
	_ai_scripting_playfight (0000)
00046370 01f0:
	_ai_scripting_vehicle_encounter (0000)
00046560 00a0:
	_ai_scripting_find_vehicle_enterable (0000)
00046600 0040:
	_ai_scripting_vehicle_enterable_distance (0000)
00046640 0050:
	_ai_scripting_vehicle_enterable_team (0000)
00046690 0050:
	_ai_scripting_vehicle_enterable_actor_type (0000)
000466E0 00a0:
	_ai_scripting_vehicle_enterable_actors (0000)
00046780 00a0:
	_ai_scripting_vehicle_enterable_disable (0000)
00046820 0070:
	_ai_scripting_look_at_object (0000)
00046890 0050:
	_ai_scripting_stop_looking (0000)
000468E0 00a0:
	_ai_scripting_automatic_migration_target (0000)
00046980 0080:
	_ai_scripting_follow_target_disable (0000)
00046A00 0080:
	_ai_scripting_follow_target_players (0000)
00046A80 0090:
	_ai_scripting_follow_target_unit (0000)
00046B10 00b0:
	_ai_scripting_follow_target_ai (0000)
00046BC0 0080:
	_ai_scripting_follow_distance (0000)
00046C40 0070:
	_ai_scripting_conversation (0000)
00046CB0 0060:
	_ai_scripting_conversation_stop (0000)
00046D10 0060:
	_ai_scripting_conversation_advance (0000)
00046D70 0010:
	_ai_scripting_conversation_line (0000)
00046D80 0010:
	_ai_scripting_conversation_status (0000)
00046D90 00b0:
	_ai_scripting_link_activation (0000)
00046E40 0090:
	_ai_scripting_berserk (0000)
00046ED0 0070:
	_ai_scripting_set_team (0000)
00046F40 00a0:
	_ai_scripting_allow_dormant (0000)
00046FE0 00d0:
	_ai_scripting_magically_see_encounter (0000)
000470B0 00a0:
	_ai_scripting_magically_see_players (0000)
00047150 0010:
	_ai_scripting_retreat (0000)
00047160 0150:
	_ai_scripting_go_to_vehicle_internal (0000)
000472B0 0080:
	_ai_scripting_go_to_vehicle (0000)
00047330 0080:
	_ai_scripting_go_to_vehicle_override (0000)
000473B0 0170:
	_ai_scripting_renew (0000)
0024BE60 0001:
	_ai_script_squad_separator (0000)
0024BE68 004d:
	??_C@_0EN@GCPCCILO@platoon_absolute_index?$DO?$DN0?5?$CG?$CG?5pla@ (0000)
0024BEB8 006b:
	??_C@_0GL@DJKDNA@platoon_index?$DO?$DN0?5?$CG?$CG?5platoon_inde@ (0000)
0024BF24 0020:
	??_C@_0CA@NFDLKDJF@ai_string?5?$CG?$CG?5ai_index_reference?$AA@ (0000)
0024BF44 001e:
	??_C@_0BO@MOPGHLE@c?3?2halo?2SOURCE?2ai?2ai_script?4c?$AA@ (0000)
0024BF64 0009:
	??_C@_08EAMFHCBD@iterator?$AA@ (0000)
0024BF70 0042:
	??_C@_0EC@OOBEBCBB@ai_attach?3?5could?5not?5find?5a?5squa@ (0000)
0024BFB8 0058:
	??_C@_0FI@EBLCLMKD@ai_attach?3?5no?5actor?5variant?5spec@ (0000)
0024C010 001d:
	??_C@_0BN@JAFMEMLM@?$CFs?3?5ai_attach_unit?50x?$CF04X?5?$CFs?$AA@ (0000)
0024C030 001a:
	??_C@_0BK@DKGEFPIC@?$CFs?3?5ai_detach_unit?50x?$CF04X?$AA@ (0000)
0024C04C 0010:
	??_C@_0BA@GMNPJHGI@?$CFs?3?5ai_place?5?$CFs?$AA@ (0000)
0024C05C 000f:
	??_C@_0P@BLMJLDDB@?$CFs?3?5ai_kill?5?$CFs?$AA@ (0000)
0024C06C 0016:
	??_C@_0BG@OLKJPCEP@?$CFs?3?5ai_kill_silent?5?$CFs?$AA@ (0000)
0024C084 0010:
	??_C@_0BA@OAGPLBNE@?$CFs?3?5ai_erase?5?$CFs?$AA@ (0000)
0024C094 0011:
	??_C@_0BB@DAJIABOO@?$CFs?3?5ai_erase_all?$AA@ (0000)
0024C0A8 0016:
	??_C@_0BG@MJMIDNGO@?$CFs?3?5ai_spawn_actor?5?$CFs?$AA@ (0000)
0024C0C0 0019:
	??_C@_0BJ@EEKIPIPP@?$CFs?3?5ai_set_respawn?5?$CFs?5?$CFs?$AA@ (0000)
0024C0DC 0004:
	??_C@_03MCADLMAF@off?$AA@ (0000)
0024C0E0 0003:
	??_C@_02LIELOMNJ@on?$AA@ (0000)
0024C0E4 0016:
	??_C@_0BG@NGLFOKP@?$CFs?3?5ai_set_deaf?5?$CFs?5?$CFs?$AA@ (0000)
0024C0FC 0017:
	??_C@_0BH@HLGNJJOK@?$CFs?3?5ai_set_blind?5?$CFs?5?$CFs?$AA@ (0000)
0024C114 0024:
	??_C@_0CE@JALKNAAG@?$CFs?3?5ai_magically_see_unit?5?$CFs?50x?$CF@ (0000)
0024C138 0016:
	??_C@_0BG@LFPOGIIF@?$CFs?3?5ai_timer_start?5?$CFs?$AA@ (0000)
0024C150 0017:
	??_C@_0BH@JJJMMONM@?$CFs?3?5ai_timer_expire?5?$CFs?$AA@ (0000)
0024C168 003d:
	??_C@_0DN@EGOGAENI@?$CIcount_type?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIcount_type@ (0000)
0024C1A8 0011:
	??_C@_0BB@JNDCBAGE@?$CFs?3?5ai_attack?5?$CFs?$AA@ (0000)
0024C1BC 0011:
	??_C@_0BB@DELPBLFI@?$CFs?3?5ai_defend?5?$CFs?$AA@ (0000)
0024C1D0 0013:
	??_C@_0BD@CBLKAFAE@?$CFs?3?5ai_maneuver?5?$CFs?$AA@ (0000)
0024C1E4 001d:
	??_C@_0BN@CEFNBAKE@?$CFs?3?5ai_maneuver_enable?5?$CFs?5?$CFs?$AA@ (0000)
0024C204 0038:
	??_C@_0DI@LFBKAFPO@?5?5?$CFs?5?9?$DO?5?$CFs?5?$CIno?5matching?5squads?5f@ (0000)
0024C23C 0025:
	??_C@_0CF@LALAHEHA@?5?5?$CFs?5?9?$DO?5?$CFs?5?$CIno?5matching?5types?5fo@ (0000)
0024C264 001a:
	??_C@_0BK@CDOPAPNJ@?5?5?$CFs?5?9?$DO?5?$CFs?5?$CIsame?9type?5?$CFs?$CJ?$AA@ (0000)
0024C280 001b:
	??_C@_0BL@GFNCBEPH@?5?5?$CFs?5?9?$DO?5?$CFs?5?$CIsame?9actor?5?$CFs?$CJ?$AA@ (0000)
0024C2A0 005c:
	??_C@_0FM@JEMPJMEH@?$CIfound_squad_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIfou@ (0000)
0024C2FC 001d:
	??_C@_0BN@BJIJNCFF@?5?5?$CFs?5?9?$DO?5?$CFs?5?$CIsame?9variant?5?$CFs?$CJ?$AA@ (0000)
0024C31C 000f:
	??_C@_0P@HILOFCFK@?5?5?$CFs?5unchanged?$AA@ (0000)
0024C330 008c:
	??_C@_0IM@CIJDMBDM@?$CItarget_squad_indices?$FLcurrent_sq@ (0000)
0024C3C0 0053:
	??_C@_0FD@PAODIOCF@?$CIcurrent_squad_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIc@ (0000)
0024C414 0009:
	??_C@_08DLMOHGLB@squad?5?$CFs?$AA@ (0000)
0024C420 0063:
	??_C@_0GD@DHCPGCGK@?$CIsource_iterator?4squad_index?5?$DO?$DN?5@ (0000)
0024C484 0015:
	??_C@_0BF@OECKHMKF@?$CFs?3?5ai_migrate?5?$CFs?5?$CFs?$AA@ (0000)
0024C49C 0005:
	??_C@_04NPEDKLDA@unit?$AA@ (0000)
0024C4A4 0026:
	??_C@_0CG@IJGCILMG@?$CFs?3?5ai_migrate_by_unit?5?$DMsome?5guy@ (0000)
0024C4D0 0050:
	??_C@_0FA@GAKJCPBM@ai_migrate_and_speak?3?5unknown?5sp@ (0000)
0024C520 0022:
	??_C@_0CC@JKLCPGKI@?$CFs?3?5ai_migrate_and_speak?5?$CFs?5?$CFs?5?$CF@ (0000)
0024C544 0018:
	??_C@_0BI@ONAJBOCN@?$CFs?3?5ai_allegiance?5?$CFd?5?$CFd?$AA@ (0000)
0024C55C 001f:
	??_C@_0BP@ICGEEHN@?$CFs?3?5ai_allegiance_remove?5?$CFd?5?$CFd?$AA@ (0000)
0024C57C 0017:
	??_C@_0BH@HFKAOKKA@?$CFs?3?5ai_exit_vehicle?5?$CFs?$AA@ (0000)
0024C594 0017:
	??_C@_0BH@NDIAEFNI@?$CFs?3?5ai_braindead?5?$CFs?5?$CFs?$AA@ (0000)
0024C5AC 0028:
	??_C@_0CI@LMFNPANN@?$CFs?3?5ai_braindead_by_unit?5?$DMsome?5g@ (0000)
0024C5D4 0020:
	??_C@_0CA@GOECGNLE@?$CFs?3?5ai_disregard?5?$DMsome?5guys?$DO?5?$CFs?$AA@ (0000)
0024C5F4 0024:
	??_C@_0CE@HDJGLAIB@?$CFs?3?5ai_prefer_target?5?$DMsome?5guys?$DO@ (0000)
0024C618 0034:
	??_C@_0DE@FNEPLHAJ@?$CFs?3?5ai_teleport_starting_locatio@ (0000)
0024C64C 0025:
	??_C@_0CF@JAILDLKJ@?$CFs?3?5ai_teleport_starting_locatio@ (0000)
0024C674 001f:
	??_C@_0BP@MEILCKDB@?$CFs?3?5ai_try_to_fight_nothing?5?$CFs?$AA@ (0000)
0024C694 001a:
	??_C@_0BK@BDNNJAGD@?$CFs?3?5ai_try_to_fight?5?$CFs?5?$CFs?$AA@ (0000)
0024C6B0 001e:
	??_C@_0BO@OIEPGOJB@?$CFs?3?5ai_try_to_fight_player?5?$CFs?$AA@ (0000)
0024C6D0 001a:
	??_C@_0BK@MBMJKENK@?$CFs?3?5ai_allow_charge?5?$CFs?5?$CFs?$AA@ (0000)
0024C6EC 0006:
	??_C@_05LAPONLG@false?$AA@ (0000)
0024C6F4 0005:
	??_C@_04LOAJBDKD@true?$AA@ (0000)
0024C6FC 001a:
	??_C@_0BK@ECBKDHID@?$CFs?3?5ai_command_list?5?$CFs?5?$CFd?$AA@ (0000)
0024C718 0026:
	??_C@_0CG@KDJJNHNP@?$CFs?3?5ai_command_list_by_unit?5?$DMuni@ (0000)
0024C740 001f:
	??_C@_0BP@PAICMAGO@?$CFs?3?5ai_command_list_advance?5?$CFs?$AA@ (0000)
0024C760 0030:
	??_C@_0DA@BIDMGCPJ@?$CFs?3?5ai_command_list_advance_by_u@ (0000)
0024C790 000f:
	??_C@_0P@MPAKFNGN@?$CFs?3?5ai_free?5?$CFs?$AA@ (0000)
0024C7A0 001f:
	??_C@_0BP@HEGBFLAM@?$CFs?3?5ai_free_units?5?$DMsome?5units?$DO?$AA@ (0000)
0024C7C0 0036:
	??_C@_0DG@OFCIAFCL@?$CFs?3?5ai_attach_free?5?$CFs?5cannot?5be?5@ (0000)
0024C7F8 001d:
	??_C@_0BN@IEADAMKD@?$CFs?3?5ai_attach_free?50x?$CF04X?5?$CFs?$AA@ (0000)
0024C818 001a:
	??_C@_0BK@NGFIDLOP@?$CFs?3?5ai_force_active?5?$CFs?5?$CFs?$AA@ (0000)
0024C838 0063:
	??_C@_0GD@LKDLLFKG@ai_force_active_by_unit?3?5unit?5is@ (0000)
0024C89C 002b:
	??_C@_0CL@INBDPDM@?$CFs?3?5ai_force_active_by_unit?5?$DMsom@ (0000)
0024C8C8 001e:
	??_C@_0BO@GELBKNIM@?$CFs?3?5ai_set_return_state?5?$CFs?5?$CFd?$AA@ (0000)
0024C8E8 001f:
	??_C@_0BP@OMBCPLPA@?$CFs?3?5ai_set_current_state?5?$CFs?5?$CFd?$AA@ (0000)
0024C908 0011:
	??_C@_0BB@DKMLPAFC@?$CFs?3?5ai_status?5?$CFs?$AA@ (0000)
0024C91C 0017:
	??_C@_0BH@LGGEBDHB@?$CFs?3?5ai_playfight?5?$CFs?5?$CFs?$AA@ (0000)
0024C934 0028:
	??_C@_0CI@NHKIHJII@?$CFs?3?5ai_vehicle_encounter?5?$DMsome?5u@ (0000)
0024C95C 003e:
	??_C@_0DO@IHGIOMGJ@ai_vehicle_enterable?3?5too?5many?5e@ (0000)
0024C99C 0031:
	??_C@_0DB@BDJKMKHA@?$CFs?3?5ai_vehicle_enterable_distanc@ (0000)
0024C9D0 0030:
	??_C@_0DA@FDMBDBPG@?$CFs?3?5ai_vehicle_enterable_team?5?$DMs@ (0000)
0024CA00 0036:
	??_C@_0DG@LLMHMJE@?$CFs?3?5ai_vehicle_enterable_actor_t@ (0000)
0024CA38 0043:
	??_C@_0ED@KEKJMIKK@ai_vehicle_enterable_actors?3?5too@ (0000)
0024CA7C 0032:
	??_C@_0DC@OHFPMGGD@?$CFs?3?5ai_vehicle_enterable_actors?5@ (0000)
0024CAB0 0030:
	??_C@_0DA@KCDIODEC@?$CFs?3?5ai_vehicle_enterable_disable@ (0000)
0024CAE0 0030:
	??_C@_0DA@MEBODMJO@?$CFs?3?5ai_look_at_object?5?$DMsome?5unit@ (0000)
0024CB10 0020:
	??_C@_0CA@PDOKPGNA@?$CFs?3?5ai_stop_looking?5?$DMsome?5unit?$DO?$AA@ (0000)
0024CB30 0028:
	??_C@_0CI@LKLFKNDG@?$CFs?3?5ai_automatic_migration_targe@ (0000)
0024CB58 0020:
	??_C@_0CA@HJOJOKEG@?$CFs?3?5ai_follow_target_disable?5?$CFs?$AA@ (0000)
0024CB78 0020:
	??_C@_0CA@LPFMOJGF@?$CFs?3?5ai_follow_target_players?5?$CFs?$AA@ (0000)
0024CB98 0029:
	??_C@_0CJ@PMNFIMPA@?$CFs?3?5ai_follow_target_unit?5?$CFs?5?$DMso@ (0000)
0024CBC4 001e:
	??_C@_0BO@NGDADOD@?$CFs?3?5ai_follow_target_ai?5?$CFs?5?$CFs?$AA@ (0000)
0024CBE4 001f:
	??_C@_0BP@FMPPICBE@?$CFs?3?5ai_follow_distance?5?$CFs?5?$CF?41f?$AA@ (0000)
0024CC04 0017:
	??_C@_0BH@IGJNNOIC@?$CFs?3?5ai_conversation?5?$CFs?$AA@ (0000)
0024CC1C 001c:
	??_C@_0BM@HGJNKNLN@?$CFs?3?5ai_conversation_stop?5?$CFs?$AA@ (0000)
0024CC38 001f:
	??_C@_0BP@IBOEPNGC@?$CFs?3?5ai_conversation_advance?5?$CFs?$AA@ (0000)
0024CC58 006a:
	??_C@_0GK@GMJOKPPN@ai_link_activation?3?5cannot?5link?5@ (0000)
0024CCC4 001d:
	??_C@_0BN@OAHKKHJB@?$CFs?3?5ai_link_activation?5?$CFs?5?$CFs?$AA@ (0000)
0024CCE4 0012:
	??_C@_0BC@GFJPNDGO@?$CFs?3?5ai_berserk?5?$CFs?$AA@ (0000)
0024CCF8 001b:
	??_C@_0BL@MKHHLLAC@?$CFs?3?5ai_allow_dormant?5?$CFs?5?$CFs?$AA@ (0000)
0024CD14 0025:
	??_C@_0CF@EKNEFNDI@?$CFs?3?5ai_magically_see_encounter?5?$CF@ (0000)
0024CD3C 0020:
	??_C@_0CA@ENGIEHJO@?$CFs?3?5ai_magically_see_players?5?$CFs?$AA@ (0000)
0024CD5C 0022:
	??_C@_0CC@LGDPOIEJ@?$CFs?3?5ai_go_to_vehicle?5?$CFs?50x?$CF04X?5?$CF@ (0000)
0024CD80 002b:
	??_C@_0CL@HNAPEBL@?$CFs?3?5ai_go_to_vehicle_override?5?$CFs@ (0000)
0024CDAC 0010:
	??_C@_0BA@GHFHPDND@?$CFs?3?5ai_renew?5?$CFs?$AA@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "ai/ai.h"
#include "ai/ai_runtime.h"
#include "ai/actions.h"
#include "ai/actor_iterators.h"
#include "ai/actor_definitions.h"
#include "ai/actor_types.h"
#include "ai/actor_looking.h"
#include "ai/actors.h"
#include "ai/ai_communication.h"
#include "ai/ai_debug.h"
#include "ai/ai_scenario_definitions.h"
#include "ai/encounters.h"
#include "ai/props.h"
#include "ai/ai_script.h"
#include "cseries/errors.h"
#include "cache/cache_files.h"
#include "game/game.h"
#include "game/game_allegiance.h"
#include "game/players.h"
#include "hs/hs.h"
#include "hs/object_lists.h"
#include "memory/data.h"
#include "objects/objects.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "units/bipeds.h"
#include "units/units.h"
#include "units/vehicle_scripting.h"

/* ---------- constants */

enum
{
	_ai_count_living = 0,
	_ai_count_swarm,
	_ai_count_nonswarm,
	NUMBER_OF_AI_COUNT_TYPES,
};

enum ai_reference_type
{
	_ai_reference_type_encounter = 0,
	_ai_reference_type_platoon,
	_ai_reference_type_squad,
	NUMBER_OF_AI_REFERENCE_TYPES,
};

enum
{
	MAXIMUM_ACTIVATION_LINK_INDICES_PER_ENCOUNTER = 3,
	MAXIMUM_AI_ENTERABLE_VEHICLES = 32,
	MAXIMUM_AI_INDICES_PER_ENTERABLE_VEHICLE = 6,
};

/* actor default states (actors.c keeps this enum file-local too) */
enum actor_default_state
{
	actor_default_state_none = 0,
	actor_default_state_asleep,
	actor_default_state_alert,
	actor_default_state_moving_repeat_position,
	actor_default_state_moving_loop,
	actor_default_state_moving_loop_back_and_forth,
	actor_default_state_moving_loop_randomly,
	actor_default_state_moving_randomly,
	actor_default_state_guarding,
	actor_default_state_guarding_at_guard_point,
	actor_default_state_searching,
	actor_default_state_fleeing,
	number_of_actor_default_states,
};

/* actor_state_data.mode (actors.c keeps this enum file-local too) */
enum
{
	_actor_mode_braindead = 0,
	_actor_mode_alert = 2,
	_actor_mode_combat,
};

enum
{
	_actor_combat_status_none = 0,
};

/* ai unit effect types (ai.h does not yet declare these) */
enum
{
	_ai_unit_effect_bump = 0,
	_ai_unit_effect_shooting,
	_ai_unit_effect_death_scream,
	_ai_unit_effect_magic_sight,
	NUMBER_OF_AI_UNIT_EFFECTS,
};

/* actor_external_orders.desired_target_type (actors.h does not yet declare these) */
enum
{
	_desired_target_none = 0,
	_desired_target_ai,
	_desired_target_player,
	NUMBER_OF_DESIRED_TARGET_TYPES,
};

/* encounter_datum.follow_target_type (encounters.h does not yet declare these) */
enum
{
	_follow_target_none = 0,
	_follow_target_players,
	_follow_target_unit,
	_follow_target_ai,
	NUMBER_OF_FOLLOW_TARGET_TYPES,
};

/* ---------- macros */

/* ---------- structures */

struct ai_script_vehicle_enterable_data
{
	long vehicle_index;
	real radius;
	short team_bitmask;
	short actor_type_bitmask;
	short ai_indices_count;
	word __pad0E;
	long ai_indices[MAXIMUM_AI_INDICES_PER_ENTERABLE_VEHICLE];
};

struct ai_script_globals_data
{
	boolean ai_active;
	boolean ai_initialized_for_map;
	byte __unknown2[0x3B4];
	short enterable_vehicle_count;
	struct ai_script_vehicle_enterable_data enterable_vehicles[MAXIMUM_AI_ENTERABLE_VEHICLES];
};

typedef char ai_script_squad_iterator_size_assert[
	sizeof(struct ai_script_squad_iterator) == 0x14 ? 1 : -1];

struct actor_iterator
{
	struct data_iterator encounter_iterator;
	boolean iterated_encounterless_list;
	boolean active_only;
	byte pad[2];
	long index;
	long next_index;
};

struct encounter_actor_iterator
{
	long encounter_index;
	long index;
	long next_index;
};

struct ai_script_vehicle_candidate
{
	long actor_index;
	real distance_squared;
	boolean already_going_to_vehicle;
	byte pad[3];
};

struct ai_script_conversation_definition
{
	char name[32];
	byte unknown[84];
};

typedef char ai_script_actor_iterator_size_assert[
	sizeof(struct actor_iterator) == 0x1C ? 1 : -1];
typedef char ai_script_actor_reference_iterator_size_assert[
	sizeof(struct ai_script_actor_reference_iterator) == 0x18 ? 1 : -1];
typedef char ai_script_actor_reference_iterator_actor_index_offset_assert[
	offsetof(struct ai_script_actor_reference_iterator, actor_index) == 0x10 ? 1 : -1];
typedef char ai_script_vehicle_enterable_size_assert[
	sizeof(struct ai_script_vehicle_enterable) == 0xC ? 1 : -1];
typedef char ai_script_vehicle_enterable_radius_offset_assert[
	offsetof(struct ai_script_vehicle_enterable, radius) == 0x4 ? 1 : -1];
typedef char ai_script_vehicle_enterable_team_offset_assert[
	offsetof(struct ai_script_vehicle_enterable, team_bitmask) == 0x8 ? 1 : -1];
typedef char ai_script_vehicle_enterable_actor_type_offset_assert[
	offsetof(struct ai_script_vehicle_enterable, actor_type_bitmask) == 0xA ? 1 : -1];
typedef char ai_script_vehicle_enterable_data_size_assert[
	sizeof(struct ai_script_vehicle_enterable_data) == 0x28 ? 1 : -1];
typedef char ai_script_globals_enterable_vehicle_count_offset_assert[
	offsetof(struct ai_script_globals_data, enterable_vehicle_count) == 0x3B6 ? 1 : -1];
typedef char ai_script_globals_enterable_vehicles_offset_assert[
	offsetof(struct ai_script_globals_data, enterable_vehicles) == 0x3B8 ? 1 : -1];
typedef char ai_script_platoon_iterator_size_assert[
	sizeof(struct ai_script_platoon_iterator) == 0xC ? 1 : -1];
typedef char ai_script_vehicle_candidate_size_assert[
	sizeof(struct ai_script_vehicle_candidate) == 0xC ? 1 : -1];
typedef char ai_script_vehicle_candidate_distance_offset_assert[
	offsetof(struct ai_script_vehicle_candidate, distance_squared) == 0x4 ? 1 : -1];
typedef char ai_script_vehicle_candidate_state_offset_assert[
	offsetof(struct ai_script_vehicle_candidate, already_going_to_vehicle) == 0x8 ? 1 : -1];
/* ---------- prototypes */

static long ai_scripting_assess_status(
	long actor_index);
static void ai_scripting_teleport_starting_location_private(
	long ai_reference,
	boolean only_if_unsupported);
static void ai_scripting_kill_internal(
	long ai_reference,
	boolean silent);
static long ai_scripting_count_internal(
	long ai_reference,
	short count_type,
	long *original_count_reference,
	real *strength_reference);
static int ai_scripting_vehicle_candidate_qsort(
	void const *candidate0,
	void const *candidate1);
static void ai_scripting_go_to_vehicle_internal(
	long ai_reference,
	long unit_index,
	char const *seat_substring_name,
	boolean override_previous_vehicles);
static short ai_scripting_migrate_find_target_squad(
	long source_encounter_index,
	short source_squad_index,
	struct actor_definition *source_actor_definition,
	struct actor_variant_definition *source_variant_definition,
	boolean same_encounter,
	long target_ai_reference,
	char const *debug_description);
static void ai_scripting_migrate_internal(
	long source_ai_reference,
	long target_ai_reference,
	boolean generate_stimulus,
	boolean advancing_stimulus);
static void ai_scripting_migrate_by_unit_internal(
	long unit_index,
	long target_ai_reference,
	boolean generate_stimulus,
	boolean advancing_stimulus);
static short ai_scripting_command_list_status_internal(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control);

/* ---------- globals */

extern struct ai_script_globals_data *ai_globals;

char const ai_script_squad_separator = '/';

/* ---------- public code */

void ai_script_initialize(
	void)
{
	return;
}

void ai_script_dispose(
	void)
{
	return;
}

void ai_script_initialize_for_new_map(
	void)
{
	return;
}

void ai_script_dispose_from_old_map(
	void)
{
	return;
}

void ai_index_to_string(
	long ai_reference,
	struct scenario *scenario,
	char *buffer,
	long buffer_size)
{
	if (ai_reference == NONE)
	{
		csstrncpy(buffer, "none", buffer_size);
	}
	else
	{
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&scenario->ai_encounters,
			ai_reference & UNSIGNED_SHORT_MAX,
			struct encounter_definition);

		switch ((unsigned long)ai_reference >> 30)
		{
		case _ai_reference_type_encounter:
			_snprintf(buffer, buffer_size, "%s", encounter_definition->name);
			break;
		case _ai_reference_type_platoon:
			_snprintf(
				buffer,
				buffer_size,
				"%s/%s",
				encounter_definition->name,
				TAG_BLOCK_GET_ELEMENT(
					&encounter_definition->platoons,
					(ai_reference >> 16) & UNSIGNED_CHAR_MAX,
					struct platoon_definition)->name);
			break;
		case _ai_reference_type_squad:
			_snprintf(
				buffer,
				buffer_size,
				"%s/%s",
				encounter_definition->name,
				TAG_BLOCK_GET_ELEMENT(
					&encounter_definition->squads,
					(ai_reference >> 16) & UNSIGNED_CHAR_MAX,
					struct squad_definition)->name);
			break;
		default:
			csstrncpy(buffer, "<error>", buffer_size);
			break;
		}
	}

	return;
}

boolean ai_index_from_string(
	struct scenario *scenario,
	char const *ai_string,
	long *ai_index_reference)
{
	long ai_reference = NONE;
	boolean success;
	char const *separator;

	match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 87, ai_string && ai_index_reference);

	if (!_stricmp(ai_string, "none"))
	{
		success = TRUE;
	}
	else
	{
		separator = strrchr(ai_string, ai_script_squad_separator);
		if (!separator)
		{
			long encounter_index = scenario_get_encounter_by_name(scenario, ai_string);

			if (encounter_index != NONE)
				ai_reference = encounter_index & UNSIGNED_SHORT_MAX;
		}
		else
		{
			long encounter_name_length = separator - ai_string;

			if (encounter_name_length <= TAG_STRING_LENGTH)
			{
				char encounter_name[TAG_STRING_LENGTH+1];
				long encounter_index;

				csstrncpy(encounter_name, ai_string, encounter_name_length);
				encounter_name[encounter_name_length] = 0;

				encounter_index = scenario_get_encounter_by_name(scenario, encounter_name);
				if (encounter_index != NONE)
				{
					struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
						&scenario->ai_encounters,
						encounter_index,
						struct encounter_definition);
					long squad_index = encounter_definition_get_squad_by_name(
						encounter_definition,
						separator + 1);

					if (squad_index != NONE)
					{
						ai_reference = (_ai_reference_type_squad << 30) |
							((squad_index & UNSIGNED_CHAR_MAX) << 16) |
							(encounter_index & UNSIGNED_SHORT_MAX);
					}
					else
					{
						long platoon_index = encounter_definition_get_platoon_by_name(
							encounter_definition,
							separator + 1);

						if (platoon_index != NONE)
						{
							ai_reference = (_ai_reference_type_platoon << 30) |
								((platoon_index & UNSIGNED_CHAR_MAX) << 16) |
								(encounter_index & UNSIGNED_SHORT_MAX);
						}
					}
				}
			}
		}

		success = ai_reference != NONE;
	}

	*ai_index_reference = ai_reference;

	return success;
}

void ai_index_squad_iterator_new(
	long ai_reference,
	struct ai_script_squad_iterator *iterator)
{
	struct scenario *scenario = global_scenario_try_and_get();

	match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 282, iterator);

	iterator->encounter_index = ai_reference & UNSIGNED_SHORT_MAX;

	if (scenario &&
		ai_globals->ai_initialized_for_map &&
		VALID_INDEX(iterator->encounter_index, scenario->ai_encounters.count))
	{
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			iterator->encounter_index & UNSIGNED_SHORT_MAX,
			struct encounter_definition);

		switch ((unsigned long)ai_reference >> 30)
		{
		case _ai_reference_type_encounter:
		case _ai_reference_type_platoon:
			iterator->squad_index = NONE;
			iterator->next_squad_index = 0;
			iterator->last_squad_index = encounter_definition->squads.count - 1;
			if (((unsigned long)ai_reference >> 30) == _ai_reference_type_encounter)
				iterator->required_platoon_index = NONE;
			else
				iterator->required_platoon_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;
			break;
		case _ai_reference_type_squad:
		{
			short squad_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;

			if (VALID_INDEX(squad_index, encounter_definition->squads.count))
			{
				iterator->squad_index = NONE;
				iterator->last_squad_index = squad_index;
				iterator->next_squad_index = squad_index;
				iterator->required_platoon_index = NONE;
			}
			else
			{
				iterator->encounter_index = NONE;
			}
			break;
		}
		default:
			iterator->encounter_index = NONE;
			break;
		}
	}
	else
	{
		iterator->encounter_index = NONE;
	}

	return;
}

struct squad_datum *ai_index_squad_iterator_next(
	struct ai_script_squad_iterator *iterator)
{
	struct squad_datum *squad = NULL;

	match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 351, iterator);

	if (iterator->encounter_index != NONE)
	{
		struct encounter_datum *encounter = encounter_get(iterator->encounter_index);
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			iterator->encounter_index & UNSIGNED_SHORT_MAX,
			struct encounter_definition);

		while (iterator->next_squad_index <= iterator->last_squad_index)
		{
			struct squad_definition *squad_definition;

			iterator->squad_index = iterator->next_squad_index;
			iterator->next_squad_index++;
			squad_definition = TAG_BLOCK_GET_ELEMENT(
				&encounter_definition->squads,
				iterator->squad_index,
				struct squad_definition);

			if (iterator->required_platoon_index == NONE ||
				squad_definition->platoon_index == iterator->required_platoon_index)
			{
				squad = encounter_get_squad(encounter, iterator->squad_index);
				break;
			}
		}
	}

	return squad;
}

void ai_index_platoon_iterator_new(
	long ai_reference,
	struct ai_script_platoon_iterator *iterator)
{
	struct scenario *scenario = global_scenario_try_and_get();

	match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 191, iterator);

	iterator->encounter_index = ai_reference & UNSIGNED_SHORT_MAX;

	if (scenario &&
		ai_globals->ai_initialized_for_map &&
		VALID_INDEX(iterator->encounter_index, scenario->ai_encounters.count))
	{
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			iterator->encounter_index & UNSIGNED_SHORT_MAX,
			struct encounter_definition);

		switch ((unsigned long)ai_reference >> 30)
		{
		case _ai_reference_type_encounter:
			iterator->platoon_index = 0;
			iterator->last_platoon_index = encounter_definition->platoons.count - 1;
			break;
		case _ai_reference_type_platoon:
		case _ai_reference_type_squad:
			if (((unsigned long)ai_reference >> 30) == _ai_reference_type_squad)
			{
				long squad_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;

				if (VALID_INDEX(squad_index, encounter_definition->squads.count))
				{
					iterator->platoon_index = TAG_BLOCK_GET_ELEMENT(
						&encounter_definition->squads,
						squad_index,
						struct squad_definition)->platoon_index;
				}
				else
				{
					iterator->platoon_index = NONE;
				}
			}
			else
			{
				iterator->platoon_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;
			}

			if (VALID_INDEX(iterator->platoon_index, encounter_definition->platoons.count))
				iterator->last_platoon_index = iterator->platoon_index;
			else
				iterator->encounter_index = NONE;
			break;
		default:
			iterator->encounter_index = NONE;
			break;
		}
	}
	else
	{
		iterator->encounter_index = NONE;
	}

	return;
}

struct platoon_datum *ai_index_platoon_iterator_next(
	struct ai_script_platoon_iterator *iterator)
{
	struct platoon_datum *platoon = NULL;

	match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 265, iterator);

	if (iterator->encounter_index != NONE &&
		iterator->platoon_index <= iterator->last_platoon_index)
	{
		platoon = encounter_get_platoon(
			encounter_get(iterator->encounter_index),
			iterator->platoon_index);
		iterator->platoon_index++;
	}

	return platoon;
}

void ai_index_actor_iterator_new(
	long ai_reference,
	struct ai_script_actor_reference_iterator *iterator)
{
	struct scenario *scenario = global_scenario_try_and_get();

	match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 384, iterator);

	iterator->encounter_index = ai_reference & UNSIGNED_SHORT_MAX;

	if (scenario &&
		ai_globals->ai_initialized_for_map &&
		VALID_INDEX(iterator->encounter_index, scenario->ai_encounters.count))
	{
		struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_encounters,
			iterator->encounter_index & UNSIGNED_SHORT_MAX,
			struct encounter_definition);

		iterator->platoon_index = NONE;
		iterator->squad_index = NONE;

		switch ((unsigned long)ai_reference >> 30)
		{
		case _ai_reference_type_encounter:
			break;
		case _ai_reference_type_platoon:
			iterator->platoon_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;
			break;
		case _ai_reference_type_squad:
			iterator->squad_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;
			break;
		default:
			iterator->encounter_index = NONE;
			break;
		}

		if (iterator->encounter_index != NONE)
		{
			encounter_actor_iterator_new(
				(struct encounter_actor_iterator *)&iterator->actor_encounter_index,
				iterator->encounter_index);
		}
	}
	else
	{
		iterator->encounter_index = NONE;
	}

	return;
}

struct actor_datum *ai_index_actor_iterator_next(
	struct ai_script_actor_reference_iterator *iterator)
{
	struct actor_datum *actor;

	match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 442, iterator);

	do
	{
		actor = encounter_actor_iterator_next(
			(struct encounter_actor_iterator *)&iterator->actor_encounter_index);
	}
	while (actor &&
		((iterator->squad_index != NONE &&
			iterator->squad_index != actor->meta.squad_index) ||
		(iterator->platoon_index != NONE &&
			iterator->platoon_index != actor->meta.platoon_index)));

	return actor;
}

long object_list_from_ai_reference(
	long ai_reference)
{
	long object_list_index = NONE;

	if (ai_reference != NONE)
	{
		object_list_index = object_list_new();
		if (object_list_index != NONE)
		{
			struct ai_script_actor_reference_iterator iterator;
			struct actor_datum *actor;

			ai_index_actor_iterator_new(ai_reference, &iterator);
			for (actor = ai_index_actor_iterator_next(&iterator);
				actor;
				actor = ai_index_actor_iterator_next(&iterator))
			{
				long swarm_unit_index;

				if (actor->meta.unit_index != NONE)
					object_list_add(object_list_index, actor->meta.unit_index);

				for (swarm_unit_index = actor->meta.swarm_unit_index;
					swarm_unit_index != NONE;)
				{
					struct unit_datum *unit = unit_get(swarm_unit_index);

					object_list_add(object_list_index, swarm_unit_index);
					swarm_unit_index = unit->unit.swarm_next_unit_index;
				}
			}
		}
	}

	return object_list_index;
}

void ai_scripting_reconnect(
	void)
{
	return;
}

short ai_scripting_conversation_line(
	short conversation_index)
{
	return ai_conversation_line(conversation_index);
}

short ai_scripting_conversation_status(
	short conversation_index)
{
	return ai_conversation_status(conversation_index);
}

void ai_scripting_retreat(
	long ai_reference)
{
	ai_scripting_maneuver(ai_reference);

	return;
}

void ai_scripting_maneuver(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_maneuver %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		struct ai_script_platoon_iterator iterator;
		struct platoon_datum *platoon;

		ai_index_platoon_iterator_new(ai_reference, &iterator);
		for (platoon = ai_index_platoon_iterator_next(&iterator);
			platoon;
			platoon = ai_index_platoon_iterator_next(&iterator))
		{
			platoon->maneuvering = TRUE;
		}
	}

	return;
}

void ai_scripting_attack(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_attack %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		struct ai_script_platoon_iterator iterator;
		struct platoon_datum *platoon;

		ai_index_platoon_iterator_new(ai_reference, &iterator);
		for (platoon = ai_index_platoon_iterator_next(&iterator);
			platoon;
			platoon = ai_index_platoon_iterator_next(&iterator))
		{
			platoon->defending = FALSE;
		}
	}

	return;
}

void ai_scripting_defend(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_defend %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		struct ai_script_platoon_iterator iterator;
		struct platoon_datum *platoon;

		ai_index_platoon_iterator_new(ai_reference, &iterator);
		for (platoon = ai_index_platoon_iterator_next(&iterator);
			platoon;
			platoon = ai_index_platoon_iterator_next(&iterator))
		{
			platoon->defending = TRUE;
		}
	}

	return;
}

void ai_scripting_maneuver_enable(
	long ai_reference,
	boolean enable)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_maneuver_enable %s %s", hs_runtime_get_executing_thread_name(), ai_name, enable ? "on" : "off");
	}

	if (ai_reference != NONE)
	{
		struct ai_script_platoon_iterator iterator;
		struct platoon_datum *platoon;

		ai_index_platoon_iterator_new(ai_reference, &iterator);
		for (platoon = ai_index_platoon_iterator_next(&iterator);
			platoon;
			platoon = ai_index_platoon_iterator_next(&iterator))
		{
			platoon->maneuver_disable = !enable;
		}
	}

	return;
}

void ai_scripting_berserk(
	long ai_reference,
	boolean berserk)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_berserk %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		struct ai_script_actor_reference_iterator iterator;
		struct actor_datum *actor;

		ai_index_actor_iterator_new(ai_reference, &iterator);
		for (actor = ai_index_actor_iterator_next(&iterator);
			actor;
			actor = ai_index_actor_iterator_next(&iterator))
		{
			actor_berserk(iterator.actor_index, berserk);
		}
	}

	return;
}

void ai_scripting_playfight(
	long ai_reference,
	boolean playfight)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_playfight %s %s", hs_runtime_get_executing_thread_name(), ai_name, playfight ? "on" : "off");
	}

	if (ai_reference != NONE)
		encounter_get(ai_reference & UNSIGNED_SHORT_MAX)->playfighting = playfight;

	return;
}

void ai_scripting_braindead(
	long ai_reference,
	boolean braindead)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_braindead %s %s", hs_runtime_get_executing_thread_name(), ai_name, braindead ? "on" : "off");
	}

	if (ai_reference != NONE)
	{
		struct ai_script_actor_reference_iterator iterator;
		struct actor_datum *actor;

		ai_index_actor_iterator_new(ai_reference, &iterator);
		for (actor = ai_index_actor_iterator_next(&iterator);
			actor;
			actor = ai_index_actor_iterator_next(&iterator))
		{
			actor_braindead(iterator.actor_index, braindead);
		}
	}

	return;
}

void ai_scripting_allow_charge(
	long ai_reference,
	boolean allow)
{
	struct ai_script_actor_reference_iterator iterator;
	struct actor_datum *actor;

	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_allow_charge %s %s", hs_runtime_get_executing_thread_name(), ai_name, allow ? "true" : "false");
	}

	ai_index_actor_iterator_new(ai_reference, &iterator);
	for (actor = ai_index_actor_iterator_next(&iterator);
		actor;
		actor = ai_index_actor_iterator_next(&iterator))
	{
		actor->external_orders.disable_charging = !allow;
	}

	return;
}

void ai_scripting_allow_dormant(
	long ai_reference,
	boolean allow)
{
	struct ai_script_squad_iterator iterator;
	struct squad_datum *squad;

	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_allow_dormant %s %s", hs_runtime_get_executing_thread_name(), ai_name, allow ? "true" : "false");
	}

	ai_index_squad_iterator_new(ai_reference, &iterator);
	for (squad = ai_index_squad_iterator_next(&iterator);
		squad;
		squad = ai_index_squad_iterator_next(&iterator))
	{
		squad->disable_dormant = !allow;
	}

	return;
}

void ai_scripting_timer_start(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_timer_start %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		struct ai_script_squad_iterator iterator;
		struct squad_datum *squad;

		ai_index_squad_iterator_new(ai_reference, &iterator);
		for (squad = ai_index_squad_iterator_next(&iterator);
			squad;
			squad = ai_index_squad_iterator_next(&iterator))
		{
			squad->delay_timer_started = TRUE;
		}
	}

	return;
}

void ai_scripting_timer_expire(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_timer_expire %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		struct ai_script_squad_iterator iterator;
		struct squad_datum *squad;

		ai_index_squad_iterator_new(ai_reference, &iterator);
		for (squad = ai_index_squad_iterator_next(&iterator);
			squad;
			squad = ai_index_squad_iterator_next(&iterator))
		{
			encounter_squad_timer_expire(iterator.encounter_index, iterator.squad_index);
		}
	}

	return;
}

void ai_scripting_follow_target_disable(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_follow_target_disable %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
		encounter_get(ai_reference & UNSIGNED_SHORT_MAX)->follow_target_type = _follow_target_none;

	return;
}

void ai_scripting_automatic_migration_target(
	long ai_reference,
	boolean automatic)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_automatic_migration_target %s %s", hs_runtime_get_executing_thread_name(), ai_name, automatic ? "on" : "off");
	}

	if (ai_reference != NONE)
	{
		struct ai_script_squad_iterator iterator;
		struct squad_datum *squad;

		ai_index_squad_iterator_new(ai_reference, &iterator);
		for (squad = ai_index_squad_iterator_next(&iterator);
			squad;
			squad = ai_index_squad_iterator_next(&iterator))
		{
			squad->automatic_migration_target = automatic;
		}
	}

	return;
}

void ai_scripting_follow_target_players(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_follow_target_players %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
		encounter_get(ai_reference & UNSIGNED_SHORT_MAX)->follow_target_type = _follow_target_players;

	return;
}

void ai_scripting_follow_distance(
	long ai_reference,
	real distance)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_follow_distance %s %.1f", hs_runtime_get_executing_thread_name(), ai_name, distance);
	}

	if (ai_reference != NONE)
		encounter_get(ai_reference & UNSIGNED_SHORT_MAX)->follow_target_distance = distance;

	return;
}

void ai_scripting_follow_target_unit(
	long ai_reference,
	long unit_index)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_follow_target_unit %s <some unit>", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		struct encounter_datum *encounter = encounter_get(ai_reference & UNSIGNED_SHORT_MAX);

		if (unit_index == NONE)
		{
			encounter->follow_target_type = _follow_target_none;
		}
		else
		{
			encounter->follow_target_type = _follow_target_unit;
			encounter->follow_target_unit_index = unit_index;
		}
	}

	return;
}

void ai_scripting_follow_target_ai(
	long ai_reference,
	long target_ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];
		char target_ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		/* BUG (preserved for exact matching): January formats ai_reference again instead of
		 * target_ai_reference, so the log prints the first name twice. A corrected build should
		 * pass target_ai_reference here.
		 */
		ai_index_to_string(ai_reference, global_scenario_get(), target_ai_name, sizeof(target_ai_name));
		error(_error_silent, "%s: ai_follow_target_ai %s %s", hs_runtime_get_executing_thread_name(), ai_name, target_ai_name);
	}

	if (ai_reference != NONE)
	{
		struct encounter_datum *encounter = encounter_get(ai_reference & UNSIGNED_SHORT_MAX);

		if (target_ai_reference == NONE)
		{
			encounter->follow_target_type = _follow_target_none;
		}
		else
		{
			encounter->follow_target_type = _follow_target_ai;
			encounter->follow_target_ai_index = target_ai_reference;
		}
	}

	return;
}

void ai_scripting_force_active(
	long ai_reference,
	boolean force)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_force_active %s %s", hs_runtime_get_executing_thread_name(), ai_name, force ? "true" : "false");
	}

	if (ai_globals->ai_initialized_for_map && ai_reference != NONE)
	{
		long encounter_index = ai_reference & UNSIGNED_SHORT_MAX;
		struct scenario *scenario = global_scenario_get();

		if (VALID_INDEX(encounter_index, scenario->ai_encounters.count))
			encounter_get(encounter_index)->force_active = force;
	}

	return;
}

void ai_scripting_deselect(
	void)
{
	if (ai_globals->ai_initialized_for_map)
	{
		ai_debug_select_actor(NONE, NONE);
	}

	return;
}

void ai_scripting_attach_units(
	long object_list_index,
	long ai_reference)
{
	long reference_index;
	long unit_index = object_list_get_first(object_list_index, &reference_index);

	while (unit_index != NONE)
	{
		ai_scripting_attach_unit(unit_index, ai_reference);
		unit_index = object_list_get_next(object_list_index, &reference_index);
	}

	return;
}

void ai_scripting_magically_see_units(
	long ai_reference,
	long object_list_index)
{
	long reference_index;
	long unit_index = object_list_get_first(object_list_index, &reference_index);

	while (unit_index != NONE)
	{
		ai_scripting_magically_see_unit(ai_reference, unit_index);
		unit_index = object_list_get_next(object_list_index, &reference_index);
	}

	return;
}

void ai_scripting_set_respawn(
	long ai_reference,
	boolean respawn)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];
		ai_index_to_string(
			ai_reference,
			global_scenario_get(),
			ai_name,
			sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_set_respawn %s %s",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			respawn ? "on" : "off");
	}

	if (ai_reference != NONE)
		encounter_set_respawn(ai_reference & UNSIGNED_SHORT_MAX, respawn);

	return;
}

void ai_scripting_set_deaf(
	long ai_reference,
	boolean deaf)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];
		ai_index_to_string(
			ai_reference,
			global_scenario_get(),
			ai_name,
			sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_set_deaf %s %s",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			deaf ? "on" : "off");
	}

	if (ai_reference != NONE)
		encounter_set_deaf(ai_reference & UNSIGNED_SHORT_MAX, deaf);

	return;
}

void ai_scripting_set_blind(
	long ai_reference,
	boolean blind)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];
		ai_index_to_string(
			ai_reference,
			global_scenario_get(),
			ai_name,
			sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_set_blind %s %s",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			blind ? "on" : "off");
	}

	if (ai_reference != NONE)
		encounter_set_blind(ai_reference & UNSIGNED_SHORT_MAX, blind);

	return;
}

struct ai_script_vehicle_enterable *ai_scripting_find_vehicle_enterable(
	long vehicle_index)
{
	struct ai_script_vehicle_enterable_data *vehicle_enterable = NULL;

	if (vehicle_index != NONE)
	{
		short vehicle_enterable_count = ai_globals->enterable_vehicle_count;
		short vehicle_enterable_index;

		for (vehicle_enterable_index = 0;
			vehicle_enterable_index < vehicle_enterable_count;
			vehicle_enterable_index++)
		{
			if (ai_globals->enterable_vehicles[vehicle_enterable_index].vehicle_index == vehicle_index)
				break;
		}

		if (vehicle_enterable_index >= MAXIMUM_AI_ENTERABLE_VEHICLES)
		{
			error(
				_error_silent,
				"ai_vehicle_enterable: too many enterable vehicles (max is %d)",
				MAXIMUM_AI_ENTERABLE_VEHICLES);
		}
		else
		{
			vehicle_enterable = &ai_globals->enterable_vehicles[vehicle_enterable_index];
			if (vehicle_enterable_index >= vehicle_enterable_count)
			{
				csmemset(vehicle_enterable, 0, sizeof(*vehicle_enterable));
				vehicle_enterable->vehicle_index = vehicle_index;
				vehicle_enterable->radius = 8.0f;
				ai_globals->enterable_vehicle_count++;
			}
		}
	}

	return (struct ai_script_vehicle_enterable *)vehicle_enterable;
}

void ai_scripting_vehicle_enterable_distance(
	long unit_index,
	real distance)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_vehicle_enterable_distance <some vehicle>",
			hs_runtime_get_executing_thread_name());
	}

	if (unit_index != NONE)
	{
		struct ai_script_vehicle_enterable *vehicle_enterable =
			ai_scripting_find_vehicle_enterable(unit_index);
		if (vehicle_enterable)
			vehicle_enterable->radius = distance;
	}

	return;
}

void ai_scripting_vehicle_enterable_team(
	long unit_index,
	long team_index)
{
	short team = team_index;

	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_vehicle_enterable_team <some vehicle> %d",
			hs_runtime_get_executing_thread_name(),
			team);
	}

	if (unit_index != NONE)
	{
		struct ai_script_vehicle_enterable *vehicle_enterable =
			ai_scripting_find_vehicle_enterable(unit_index);
		if (vehicle_enterable)
			vehicle_enterable->team_bitmask |= 1 << team;
	}

	return;
}

void ai_scripting_vehicle_enterable_actor_type(
	long unit_index,
	long actor_type)
{
	short type = actor_type;

	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_vehicle_enterable_actor_type <some vehicle> %d",
			hs_runtime_get_executing_thread_name(),
			type);
	}

	if (unit_index != NONE)
	{
		struct ai_script_vehicle_enterable *vehicle_enterable =
			ai_scripting_find_vehicle_enterable(unit_index);
		if (vehicle_enterable)
			vehicle_enterable->actor_type_bitmask |= 1 << type;
	}

	return;
}

void ai_scripting_vehicle_enterable_actors(
	long unit_index,
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(
			ai_reference,
			global_scenario_get(),
			ai_name,
			sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_vehicle_enterable_actors <some vehicle> %s",
			hs_runtime_get_executing_thread_name(),
			ai_name);
	}

	if (unit_index != NONE && ai_reference != NONE)
	{
		struct ai_script_vehicle_enterable_data *vehicle_enterable =
			(struct ai_script_vehicle_enterable_data *)ai_scripting_find_vehicle_enterable(unit_index);

		if (vehicle_enterable)
		{
			if (vehicle_enterable->ai_indices_count < MAXIMUM_AI_INDICES_PER_ENTERABLE_VEHICLE)
			{
				vehicle_enterable->ai_indices[vehicle_enterable->ai_indices_count++] = ai_reference;
			}
			else
			{
				error(
					_error_silent,
					"ai_vehicle_enterable_actors: too many groups of actors (max is %d)",
					MAXIMUM_AI_INDICES_PER_ENTERABLE_VEHICLE);
			}
		}
	}

	return;
}

void ai_scripting_vehicle_enterable_disable(
	long unit_index)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_vehicle_enterable_disable <some vehicle>",
			hs_runtime_get_executing_thread_name());
	}

	if (unit_index != NONE)
	{
		short vehicle_enterable_index;

		for (vehicle_enterable_index = 0;
			vehicle_enterable_index < ai_globals->enterable_vehicle_count;
			vehicle_enterable_index++)
		{
			if (ai_globals->enterable_vehicles[vehicle_enterable_index].vehicle_index == unit_index)
			{
				ai_globals->enterable_vehicle_count--;
				if (vehicle_enterable_index < ai_globals->enterable_vehicle_count)
				{
					ai_globals->enterable_vehicles[vehicle_enterable_index] =
						ai_globals->enterable_vehicles[ai_globals->enterable_vehicle_count];
				}

				break;
			}
		}
	}

	return;
}

void ai_scripting_detach_unit(
	long unit_index)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_detach_unit 0x%04X",
			hs_runtime_get_executing_thread_name(),
			unit_index & UNSIGNED_SHORT_MAX);
	}

	if (unit_index != NONE)
	{
		long actor_index = unit_get(unit_index)->unit.actor_index;
		if (actor_index != NONE)
			actor_delete(actor_index, FALSE);
	}

	return;
}

void ai_scripting_detach_units(
	long object_list_index)
{
	long reference_index;
	long unit_index = object_list_get_first(object_list_index, &reference_index);

	while (unit_index != NONE)
	{
		ai_scripting_detach_unit(unit_index);
		unit_index = object_list_get_next(object_list_index, &reference_index);
	}

	return;
}

void ai_scripting_kill(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];
		ai_index_to_string(
			ai_reference,
			global_scenario_get(),
			ai_name,
			sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_kill %s",
			hs_runtime_get_executing_thread_name(),
			ai_name);
	}

	ai_scripting_kill_internal(ai_reference, FALSE);

	return;
}

void ai_scripting_kill_silent(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];
		ai_index_to_string(
			ai_reference,
			global_scenario_get(),
			ai_name,
			sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_kill_silent %s",
			hs_runtime_get_executing_thread_name(),
			ai_name);
	}

	ai_scripting_kill_internal(ai_reference, TRUE);

	return;
}

short ai_scripting_swarm_count(
	long ai_reference)
{
	return ai_scripting_count_internal(ai_reference, _ai_count_swarm, NULL, NULL);
}

short ai_scripting_nonswarm_count(
	long ai_reference)
{
	return ai_scripting_count_internal(ai_reference, _ai_count_nonswarm, NULL, NULL);
}

short ai_scripting_living_count(
	long ai_reference)
{
	return ai_scripting_count_internal(ai_reference, _ai_count_living, NULL, NULL);
}

real ai_scripting_living_fraction(
	long ai_reference)
{
	real result = 0.0f;
	long original_count;
	long count = ai_scripting_count_internal(
		ai_reference,
		_ai_count_living,
		&original_count,
		NULL);

	if (original_count > 0)
		result = (real)count / (real)original_count;

	return result;
}

real ai_scripting_strength(
	long ai_reference)
{
	real strength = 0.0f;
	ai_scripting_count_internal(ai_reference, _ai_count_living, NULL, &strength);
	return strength;
}

boolean ai_scripting_is_attacking(
	long ai_reference)
{
	struct ai_script_platoon_iterator iterator;
	struct platoon_datum *platoon;
	boolean attacking = FALSE;

	if (ai_reference != NONE)
	{
		ai_index_platoon_iterator_new(ai_reference, &iterator);

		platoon = ai_index_platoon_iterator_next(&iterator);
		while (platoon)
		{
			if (!platoon->defending)
			{
				attacking = TRUE;
				break;
			}

			platoon = ai_index_platoon_iterator_next(&iterator);
		}
	}

	return attacking;
}

short ai_scripting_going_to_vehicle(
	long unit_index)
{
	struct actor_iterator iterator;
	struct actor_datum *actor;
	short count = 0;

	actor_iterator_new(&iterator, TRUE);
	actor = actor_iterator_next(&iterator);
	while (actor)
	{
		if (actor->state.action == _actor_action_vehicle &&
			actor->state.action_data.vehicle.vehicle_index == unit_index)
		{
			count++;
		}

		actor = actor_iterator_next(&iterator);
	}

	return count;
}

static short ai_scripting_migrate_find_target_squad(
	long source_encounter_index,
	short source_squad_index,
	struct actor_definition *source_actor_definition,
	struct actor_variant_definition *source_variant_definition,
	boolean same_encounter,
	long target_ai_reference,
	char const *debug_description)
{
	struct scenario *scenario = global_scenario_get();
	struct encounter_definition *target_encounter_definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_encounters,
		DATUM_INDEX_TO_ABSOLUTE_INDEX(target_ai_reference),
		struct encounter_definition);
	struct ai_script_squad_iterator iterator;
	struct squad_datum *squad;
	short first_squad_index = NONE;
	short matching_type_index = NONE;
	short matching_actor_index = NONE;
	short matching_variant_index = NONE;
	short matching_squad_index = NONE;
	short found_squad_index = NONE;

	ai_index_squad_iterator_new(target_ai_reference, &iterator);
	for (squad = ai_index_squad_iterator_next(&iterator);
		squad;
		squad = ai_index_squad_iterator_next(&iterator))
	{
		struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
			&target_encounter_definition->squads,
			iterator.squad_index,
			struct squad_definition);
		struct actor_variant_definition *actor_variant_definition = NULL;
		struct actor_definition *actor_definition = NULL;
		short actor_palette_index = squad_definition->actor_palette_index;

		if (VALID_INDEX(actor_palette_index, scenario->ai_actor_palette.count))
		{
			struct tag_reference *actor_palette_entry = TAG_BLOCK_GET_ELEMENT(
				&scenario->ai_actor_palette,
				actor_palette_index,
				struct tag_reference);

			if (actor_palette_entry->index != NONE &&
				tag_get_group_tag(actor_palette_entry->index) == ACTOR_VARIANT_DEFINITION_TAG)
			{
				actor_variant_definition = actor_variant_definition_get(actor_palette_entry->index);
				if (actor_variant_definition->actor_reference.index != NONE)
				{
					actor_definition = actor_definition_get(
						actor_variant_definition->actor_reference.index);
				}
			}
		}

		if (matching_squad_index == NONE &&
			same_encounter &&
			source_squad_index == iterator.squad_index)
		{
			matching_squad_index = iterator.squad_index;
		}
		if (matching_variant_index == NONE &&
			source_variant_definition &&
			actor_variant_definition &&
			source_variant_definition == actor_variant_definition)
		{
			matching_variant_index = iterator.squad_index;
		}
		if (matching_actor_index == NONE &&
			source_actor_definition &&
			actor_definition &&
			source_actor_definition == actor_definition)
		{
			matching_actor_index = iterator.squad_index;
		}
		if (matching_type_index == NONE &&
			source_actor_definition &&
			actor_definition &&
			source_actor_definition->type == actor_definition->type)
		{
			matching_type_index = iterator.squad_index;
		}
		if (first_squad_index == NONE)
		{
			first_squad_index = iterator.squad_index;
		}
	}

	if (matching_squad_index != NONE)
	{
		found_squad_index = matching_squad_index;
		if (ai_debug.print_migration)
		{
			error(_error_silent, "  %s unchanged", debug_description);
		}
	}
	else if (matching_variant_index != NONE)
	{
		found_squad_index = matching_variant_index;
		if (ai_debug.print_migration)
		{
			struct squad_definition *target_squad_definition = TAG_BLOCK_GET_ELEMENT(
				&target_encounter_definition->squads,
				found_squad_index,
				struct squad_definition);
			char const *source_variant_name = "<error>";

			if (source_encounter_index != NONE)
			{
				struct encounter_definition *source_encounter_definition = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_encounters,
					DATUM_INDEX_TO_ABSOLUTE_INDEX(source_encounter_index),
					struct encounter_definition);

				if (source_squad_index != NONE)
				{
					struct squad_definition *source_squad_definition = TAG_BLOCK_GET_ELEMENT(
						&source_encounter_definition->squads,
						source_squad_index,
						struct squad_definition);

					if (VALID_INDEX(
						source_squad_definition->actor_palette_index,
						scenario->ai_actor_palette.count))
					{
						struct tag_reference *actor_palette_entry = TAG_BLOCK_GET_ELEMENT(
							&scenario->ai_actor_palette,
							source_squad_definition->actor_palette_index,
							struct tag_reference);

						if (actor_palette_entry->index != NONE)
						{
							source_variant_name = tag_get_name(actor_palette_entry->index);
						}
					}
				}
			}

			error(
				_error_silent,
				"  %s -> %s (same-variant %s)",
				debug_description,
				target_squad_definition->name,
				source_variant_name);
		}
	}
	else if (matching_actor_index != NONE)
	{
		found_squad_index = matching_actor_index;
		if (ai_debug.print_migration)
		{
			struct squad_definition *target_squad_definition = TAG_BLOCK_GET_ELEMENT(
				&target_encounter_definition->squads,
				found_squad_index,
				struct squad_definition);
			char const *actor_name = "<error>";

			if (source_variant_definition)
			{
				long actor_definition_index = source_variant_definition->actor_reference.index;

				actor_name = actor_definition_index == NONE ?
					"<none>" :
					tag_get_name(actor_definition_index);
			}
			error(
				_error_silent,
				"  %s -> %s (same-actor %s)",
				debug_description,
				target_squad_definition->name,
				actor_name);
		}
	}
	else if (matching_type_index != NONE)
	{
		found_squad_index = matching_type_index;
		if (ai_debug.print_migration)
		{
			struct squad_definition *target_squad_definition = TAG_BLOCK_GET_ELEMENT(
				&target_encounter_definition->squads,
				found_squad_index,
				struct squad_definition);

			error(
				_error_silent,
				"  %s -> %s (same-type %s)",
				debug_description,
				target_squad_definition->name,
				actor_type_get_name(source_actor_definition->type));
		}
	}
	else if (first_squad_index != NONE)
	{
		found_squad_index = first_squad_index;
		if (ai_debug.print_migration)
		{
			error(
				_error_silent,
				"  %s -> %s (no matching types found)",
				debug_description,
				TAG_BLOCK_GET_ELEMENT(
					&target_encounter_definition->squads,
					found_squad_index,
					struct squad_definition)->name);
		}
	}
	else if (target_encounter_definition->squads.count > 0)
	{
		found_squad_index = 0;
		if (ai_debug.print_migration)
		{
			error(
				_error_silent,
				"  %s -> %s (no matching squads found, going to squad 0)",
				debug_description,
				TAG_BLOCK_GET_ELEMENT(
					&target_encounter_definition->squads,
					found_squad_index,
					struct squad_definition)->name);
		}
	}

	if (found_squad_index != NONE)
	{
		match_assert(
			"c:\\halo\\SOURCE\\ai\\ai_script.c",
			1532,
			(found_squad_index >= 0) && (found_squad_index < target_encounter_definition->squads.count));
	}

	return found_squad_index;
}

static void ai_scripting_migrate_internal(
	long source_ai_reference,
	long target_ai_reference,
	boolean generate_stimulus,
	boolean advancing_stimulus)
{
	if (source_ai_reference != NONE && target_ai_reference != NONE)
	{
		long source_encounter_index = source_ai_reference & UNSIGNED_SHORT_MAX;
		long target_encounter_index = target_ai_reference & UNSIGNED_SHORT_MAX;

		if (source_encounter_index != NONE && target_encounter_index != NONE)
		{
			struct scenario *scenario = global_scenario_get();
			struct encounter_datum *source_encounter = encounter_get(source_encounter_index);
			struct encounter_datum *target_encounter = encounter_get(target_encounter_index);
			struct encounter_definition *source_encounter_definition = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters,
				DATUM_INDEX_TO_ABSOLUTE_INDEX(source_encounter_index),
				struct encounter_definition);
			struct encounter_definition *target_encounter_definition = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters,
				DATUM_INDEX_TO_ABSOLUTE_INDEX(target_encounter_index),
				struct encounter_definition);
			boolean same_encounter = source_encounter_index == target_encounter_index;
			short target_squad_indices[MAXIMUM_SQUADS_PER_ENCOUNTER];

			csmemset(target_squad_indices, NONE, sizeof(target_squad_indices));

			{
				struct ai_script_squad_iterator source_iterator;
				struct squad_datum *squad;

				ai_index_squad_iterator_new(source_ai_reference, &source_iterator);
				for (squad = ai_index_squad_iterator_next(&source_iterator);
					squad;
					squad = ai_index_squad_iterator_next(&source_iterator))
				{
					match_assert(
						"c:\\halo\\SOURCE\\ai\\ai_script.c",
						1571,
						(source_iterator.squad_index >= 0) && (source_iterator.squad_index < MAXIMUM_SQUADS_PER_ENCOUNTER));
					if (squad->current_count > 0 || source_encounter->is_prevehicle_encounter)
					{
						struct squad_definition *source_squad_definition = TAG_BLOCK_GET_ELEMENT(
							&source_encounter_definition->squads,
							source_iterator.squad_index,
							struct squad_definition);
						struct actor_definition *source_actor_definition = NULL;
						struct actor_variant_definition *source_variant_definition = NULL;

						if (VALID_INDEX(
							source_squad_definition->actor_palette_index,
							scenario->ai_actor_palette.count))
						{
							struct tag_reference *actor_palette_entry = TAG_BLOCK_GET_ELEMENT(
								&scenario->ai_actor_palette,
								source_squad_definition->actor_palette_index,
								struct tag_reference);

							if (actor_palette_entry->index != NONE &&
								tag_get_group_tag(actor_palette_entry->index) == ACTOR_VARIANT_DEFINITION_TAG)
							{
								source_variant_definition = actor_variant_definition_get(
									actor_palette_entry->index);
								if (source_variant_definition->actor_reference.index != NONE)
								{
									source_actor_definition = actor_definition_get(
										source_variant_definition->actor_reference.index);
								}
							}
						}

						sprintf(temporary, "squad %s", source_squad_definition->name);
						target_squad_indices[source_iterator.squad_index] = ai_scripting_migrate_find_target_squad(
							source_encounter_index,
							source_iterator.squad_index,
							source_actor_definition,
							source_variant_definition,
							same_encounter,
							target_ai_reference,
							temporary);
					}
				}
			}

			{
				struct encounter_actor_iterator iterator;
				struct actor_datum *actor;

				encounter_actor_iterator_new(&iterator, source_encounter_index);
				for (actor = encounter_actor_iterator_next(&iterator);
					actor;
					actor = encounter_actor_iterator_next(&iterator))
				{
					short current_squad_index = actor->meta.squad_index;

					match_assert(
						"c:\\halo\\SOURCE\\ai\\ai_script.c",
						1617,
						(current_squad_index >= 0) && (current_squad_index < MAXIMUM_SQUADS_PER_ENCOUNTER));
					if (target_squad_indices[current_squad_index] != NONE &&
						(!same_encounter || target_squad_indices[current_squad_index] != current_squad_index))
					{
						match_assert(
							"c:\\halo\\SOURCE\\ai\\ai_script.c",
							1629,
							(target_squad_indices[current_squad_index] >= 0) && (target_squad_indices[current_squad_index] < target_encounter_definition->squads.count));
						actor_change_encounter(
							iterator.index,
							target_encounter_index,
							target_squad_indices[current_squad_index]);
						if (generate_stimulus)
						{
							actor_stimulus_maneuvering(
								iterator.index,
								advancing_stimulus,
								FALSE);
						}
					}
				}
			}

			if (source_encounter->is_prevehicle_encounter)
			{
				struct actor_iterator iterator;
				struct actor_datum *actor;

				actor_iterator_new(&iterator, FALSE);
				for (actor = actor_iterator_next(&iterator);
					actor;
					actor = actor_iterator_next(&iterator))
				{
					if (DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.prevehicle_encounter_index) ==
						DATUM_INDEX_TO_ABSOLUTE_INDEX(source_encounter_index))
					{
						short current_squad_index = actor->meta.prevehicle_squad_index;

						match_assert(
							"c:\\halo\\SOURCE\\ai\\ai_script.c",
							1655,
							(current_squad_index >= 0) && (current_squad_index < MAXIMUM_SQUADS_PER_ENCOUNTER));
						if (target_squad_indices[current_squad_index] != NONE &&
							(!same_encounter || target_squad_indices[current_squad_index] != current_squad_index))
						{
							match_assert(
								"c:\\halo\\SOURCE\\ai\\ai_script.c",
								1667,
								(target_squad_indices[current_squad_index] >= 0) && (target_squad_indices[current_squad_index] < target_encounter_definition->squads.count));
							actor->meta.prevehicle_encounter_index = target_encounter_index;
							actor->meta.prevehicle_squad_index = target_squad_indices[current_squad_index];
						}
					}
				}

				if (!same_encounter)
				{
					/* BUG (original): January tests the reference type of the already-masked encounter
					 * index (test edi,0xC0000000), so the source flag is also cleared for platoon and squad
					 * migrations (the later HCEA build clears it unconditionally). A corrected build would
					 * presumably test the type of source_ai_reference instead.
					 */
					if (((unsigned long)source_encounter_index >> 30) == _ai_reference_type_encounter)
					{
						source_encounter->is_prevehicle_encounter = FALSE;
					}
					target_encounter->is_prevehicle_encounter = TRUE;
				}
			}

			{
				struct encounter_actor_iterator iterator;
				struct actor_datum *actor;

				encounter_actor_iterator_new(&iterator, NONE);
				for (actor = encounter_actor_iterator_next(&iterator);
					actor;
					actor = encounter_actor_iterator_next(&iterator))
				{
					if (DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.disconnected_encounter_index) ==
						DATUM_INDEX_TO_ABSOLUTE_INDEX(source_encounter_index))
					{
						short current_squad_index = actor->meta.disconnected_squad_index;

						match_assert(
							"c:\\halo\\SOURCE\\ai\\ai_script.c",
							1700,
							(current_squad_index >= 0) && (current_squad_index < MAXIMUM_SQUADS_PER_ENCOUNTER));
						if (target_squad_indices[current_squad_index] != NONE &&
							(!same_encounter || target_squad_indices[current_squad_index] != current_squad_index))
						{
							match_assert(
								"c:\\halo\\SOURCE\\ai\\ai_script.c",
								1712,
								(target_squad_indices[current_squad_index] >= 0) && (target_squad_indices[current_squad_index] < target_encounter_definition->squads.count));
							actor->meta.disconnected_encounter_index = target_encounter_index;
							actor->meta.disconnected_squad_index = target_squad_indices[current_squad_index];
							if (!same_encounter &&
								target_encounter_definition->runtime_structure_bsp_reference_index ==
									global_structure_bsp_index_get())
							{
								encounterless_detach_actor(iterator.index);
								encounter_attach_actor(
									iterator.index,
									actor->meta.disconnected_encounter_index,
									actor->meta.disconnected_squad_index,
									TRUE);
							}
						}
					}
				}
			}

			if (source_encounter->team_index != target_encounter->team_index)
			{
				ai_update_team_status();
			}
			encounters_update_dirty_status();
		}
	}

	return;
}

void ai_scripting_migrate(
	long source_ai_reference,
	long target_ai_reference)
{
	if (ai_debug.print_migration || ai_debug.print_scripting)
	{
		char source_name[512];
		char target_name[512];

		ai_index_to_string(
			source_ai_reference,
			global_scenario_get(),
			source_name,
			sizeof(source_name));
		ai_index_to_string(
			target_ai_reference,
			global_scenario_get(),
			target_name,
			sizeof(target_name));
		error(
			_error_silent,
			"%s: ai_migrate %s %s",
			hs_runtime_get_executing_thread_name(),
			source_name,
			target_name);
	}

	ai_scripting_migrate_internal(
		source_ai_reference,
		target_ai_reference,
		FALSE,
		FALSE);

	return;
}

static void ai_scripting_migrate_by_unit_internal(
	long unit_index,
	long target_ai_reference,
	boolean generate_stimulus,
	boolean advancing_stimulus)
{
	struct unit_datum *unit = unit_get(unit_index);
	long actor_index = unit->unit.actor_index != NONE ?
		unit->unit.actor_index :
		unit->unit.swarm_actor_index;

	if (actor_index != NONE && target_ai_reference != NONE)
	{
		struct actor_datum *actor = actor_get(actor_index);
		struct actor_definition *actor_definition = actor_definition_get(actor->meta.definition_index);
		struct actor_variant_definition *actor_variant_definition =
			actor_variant_definition_get(actor->meta.variant_definition_index);
		long target_encounter_index = target_ai_reference & UNSIGNED_SHORT_MAX;
		boolean same_encounter = DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index) ==
			DATUM_INDEX_TO_ABSOLUTE_INDEX(target_encounter_index);
		short target_squad_index = ai_scripting_migrate_find_target_squad(
			actor->meta.encounter_index,
			actor->meta.squad_index,
			actor_definition,
			actor_variant_definition,
			same_encounter,
			target_ai_reference,
			"unit");

		if (target_squad_index != NONE &&
			(!same_encounter || target_squad_index != actor->meta.squad_index))
		{
			actor_change_encounter(actor_index, target_encounter_index, target_squad_index);
			if (generate_stimulus)
			{
				actor_stimulus_maneuvering(actor_index, advancing_stimulus, FALSE);
			}
		}
	}

	return;
}

void ai_scripting_migrate_by_unit(
	long object_list_index,
	long target_ai_reference)
{
	if (ai_debug.print_migration || ai_debug.print_scripting)
	{
		char target_name[512];

		ai_index_to_string(
			target_ai_reference,
			global_scenario_get(),
			target_name,
			sizeof(target_name));
		error(
			_error_silent,
			"%s: ai_migrate_by_unit <some guys> %s",
			hs_runtime_get_executing_thread_name(),
			target_name);
	}

	if (object_list_index != NONE && target_ai_reference != NONE)
	{
		long reference_index;
		long unit_index = object_list_get_first(object_list_index, &reference_index);

		while (unit_index != NONE)
		{
			struct unit_datum *unit = unit_try_and_get(unit_index);

			if (unit)
			{
				long child_object_index;

				ai_scripting_migrate_by_unit_internal(
					unit_index,
					target_ai_reference,
					FALSE,
					FALSE);
				child_object_index = unit->object.first_child_object_index;
				while (child_object_index != NONE)
				{
					struct object_datum *child_object = object_get(child_object_index);

					if (TEST_FLAG(_object_mask_unit, child_object->object.type))
					{
						ai_scripting_migrate_by_unit_internal(
							child_object_index,
							target_ai_reference,
							FALSE,
							FALSE);
					}
					child_object_index = child_object->object.next_object_index;
				}
			}

			unit_index = object_list_get_next(object_list_index, &reference_index);
		}

		ai_update_team_status();
		encounters_update_dirty_status();
	}

	return;
}

void ai_scripting_migrate_and_speak(
	long source_ai_reference,
	long target_ai_reference,
	char const *speech_type)
{
	boolean advancing_stimulus;

	if (ai_debug.print_migration || ai_debug.print_scripting)
	{
		char source_name[512];
		char target_name[512];

		ai_index_to_string(
			source_ai_reference,
			global_scenario_get(),
			source_name,
			sizeof(source_name));
		ai_index_to_string(
			target_ai_reference,
			global_scenario_get(),
			target_name,
			sizeof(target_name));
		/* BUG (preserved for exact matching): the format has four %s conversions
		 * but January supplies only three values, so the fourth conversion reads
		 * past the end of the argument list.
		 * Evidence: January's .rdata holds the 34-byte literal
		 * "%s: ai_migrate_and_speak %s %s %s", which has four conversions, while
		 * its call site cleans 0x14 bytes - five dwords, i.e. only three values -
		 * and loads speech_type only afterwards, at +0x7d. The same defect
		 * survives unrepaired in the 2020 build.
		 * Consequence: with January's prologue the fourth slot lands on the saved
		 * EDI home, so vsprintf dereferences the caller's entry EDI as a char *.
		 * The branch is gated on ai_debug.print_migration or
		 * ai_debug.print_scripting. Both default to false, but they are exposed as
		 * the HaloScript globals ai_print_migration and ai_print_scripting, so a
		 * scenario script or the debug console can reach this path at runtime.
		 * A corrected build should pass speech_type as the fourth value.
		 */
		error(
			_error_silent,
			"%s: ai_migrate_and_speak %s %s %s",
			hs_runtime_get_executing_thread_name(),
			source_name,
			target_name);
	}

	if (_stricmp(speech_type, "advance") == 0)
	{
		advancing_stimulus = TRUE;
	}
	else
	{
		if (_stricmp(speech_type, "retreat") != 0)
		{
			error(
				_error_silent,
				"ai_migrate_and_speak: unknown speech type '%s' (must be 'advance' or 'retreat')",
				speech_type);
		}
		advancing_stimulus = FALSE;
	}

	ai_scripting_migrate_internal(
		source_ai_reference,
		target_ai_reference,
		TRUE,
		advancing_stimulus);

	return;
}

void ai_scripting_allegiance_remove(
	short team1_index,
	short team2_index)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_allegiance_remove %d %d",
			hs_runtime_get_executing_thread_name(),
			team1_index,
			team2_index);
	}

	if (team1_index != NONE && team2_index != NONE)
		game_allegiance_remove(team1_index, team2_index);

	return;
}

boolean ai_scripting_allegiance_broken(
	short team1_index,
	short team2_index)
{
	boolean broken = FALSE;

	if (team1_index != NONE && team2_index != NONE)
	{
		broken =
			game_team_is_ally(team1_index, team2_index) &&
			game_team_is_enemy(team1_index, team2_index);
	}

	return broken;
}

boolean ai_scripting_conversation(
	long conversation_index)
{
	short conversation = conversation_index;

	if (ai_debug.print_scripting)
	{
		struct scenario *scenario;
		char const *conversation_name;

		scenario = global_scenario_get();
		conversation_name = "<error>";
		if (VALID_INDEX(conversation, scenario->ai_conversations.count))
		{
			conversation_name = TAG_BLOCK_GET_ELEMENT(
				&scenario->ai_conversations,
				conversation,
				struct ai_script_conversation_definition)->name;
		}

		error(
			_error_silent,
			"%s: ai_conversation %s",
			hs_runtime_get_executing_thread_name(),
			conversation_name);
	}

	return ai_conversation(conversation, TRUE);
}

void ai_scripting_conversation_stop(
	long conversation_index)
{
	short conversation = conversation_index;

	if (ai_debug.print_scripting)
	{
		struct scenario *scenario;
		char const *conversation_name;

		scenario = global_scenario_get();
		conversation_name = "<error>";
		if (VALID_INDEX(conversation, scenario->ai_conversations.count))
		{
			conversation_name = TAG_BLOCK_GET_ELEMENT(
				&scenario->ai_conversations,
				conversation,
				struct ai_script_conversation_definition)->name;
		}

		error(
			_error_silent,
			"%s: ai_conversation_stop %s",
			hs_runtime_get_executing_thread_name(),
			conversation_name);
	}

	ai_conversation_stop(conversation);

	return;
}

void ai_scripting_conversation_advance(
	long conversation_index)
{
	short conversation = conversation_index;

	if (ai_debug.print_scripting)
	{
		struct scenario *scenario;
		char const *conversation_name;

		scenario = global_scenario_get();
		conversation_name = "<error>";
		if (VALID_INDEX(conversation, scenario->ai_conversations.count))
		{
			conversation_name = TAG_BLOCK_GET_ELEMENT(
				&scenario->ai_conversations,
				conversation,
				struct ai_script_conversation_definition)->name;
		}

		error(
			_error_silent,
			"%s: ai_conversation_advance %s",
			hs_runtime_get_executing_thread_name(),
			conversation_name);
	}

	ai_conversation_advance(conversation);

	return;
}

void ai_scripting_stop_looking(
	long unit_index)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_stop_looking <some unit>",
			hs_runtime_get_executing_thread_name());
	}

	if (unit_index != NONE)
	{
		long actor_index = unit_get(unit_index)->unit.actor_index;
		if (actor_index != NONE)
			actor_look_secondary_stop(actor_index);
	}

	return;
}

void ai_scripting_go_to_vehicle(
	long ai_reference,
	long unit_index,
	char const *seat_substring_name)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];
		ai_index_to_string(
			ai_reference,
			global_scenario_get(),
			ai_name,
			sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_go_to_vehicle %s 0x%04X %s",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			unit_index & UNSIGNED_SHORT_MAX,
			seat_substring_name);
	}

	ai_scripting_go_to_vehicle_internal(ai_reference, unit_index, seat_substring_name, FALSE);

	return;
}

void ai_scripting_go_to_vehicle_override(
	long ai_reference,
	long unit_index,
	char const *seat_substring_name)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];
		ai_index_to_string(
			ai_reference,
			global_scenario_get(),
			ai_name,
			sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_go_to_vehicle_override %s 0x%04X %s",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			unit_index & UNSIGNED_SHORT_MAX,
			seat_substring_name);
	}

	ai_scripting_go_to_vehicle_internal(ai_reference, unit_index, seat_substring_name, TRUE);

	return;
}

void ai_scripting_erase_all(
	void)
{
	if (ai_debug.print_scripting)
	{
		error(_error_silent, "%s: ai_erase_all", hs_runtime_get_executing_thread_name());
	}

	ai_erase(NONE, NONE, NONE, FALSE);

	return;
}

void ai_scripting_select(
	long ai_reference)
{
	if (ai_globals->ai_initialized_for_map)
	{
		long encounter_index;

		if (ai_reference == NONE)
			encounter_index = NONE;
		else
			encounter_index = ai_reference & UNSIGNED_SHORT_MAX;

		ai_debug_select_encounter(encounter_index);
	}

	return;
}

short ai_scripting_status(
	long ai_reference)
{
	short status = 0;
	struct ai_script_actor_reference_iterator iterator;

	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_status %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	ai_index_actor_iterator_new(ai_reference, &iterator);
	while (ai_index_actor_iterator_next(&iterator))
	{
		short actor_status = ai_scripting_assess_status(iterator.actor_index);

		status = MAX(status, actor_status);
	}

	return status;
}

void ai_scripting_spawn_actor(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_spawn_actor %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_globals->ai_initialized_for_map && ai_reference != NONE)
	{
		long encounter_index = ai_reference & UNSIGNED_SHORT_MAX;
		short squad_index = ((unsigned long)ai_reference >> 30) == _ai_reference_type_squad ?
			((unsigned long)ai_reference >> 16) & UNSIGNED_CHAR_MAX : NONE;

		if (squad_index == NONE &&
			((unsigned long)ai_reference >> 30) == _ai_reference_type_platoon)
		{
			struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters,
				DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index),
				struct encounter_definition);
			short search_index;

			for (search_index = 0;
				search_index < encounter_definition->squads.count;
				search_index++)
			{
				struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
					&encounter_definition->squads,
					search_index,
					struct squad_definition);

				if (squad_definition->platoon_index == (((unsigned long)ai_reference >> 16) & UNSIGNED_CHAR_MAX))
				{
					squad_index = search_index;
					break;
				}
			}
		}

		if (squad_index != NONE)
			encounter_spawn_actor(encounter_index, squad_index);
	}

	return;
}

void ai_scripting_vehicle_encounter(
	long unit_index,
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_vehicle_encounter <some unit> %s",
			hs_runtime_get_executing_thread_name(),
			ai_name);
	}

	if (unit_index != NONE)
	{
		struct unit_datum *vehicle = unit_get(unit_index);
		short target_encounter_index = NONE;
		short target_squad_index = NONE;

		if (ai_reference != NONE)
		{
			struct scenario *scenario = global_scenario_get();
			short encounter_index = ai_reference;

			if (VALID_INDEX(encounter_index, scenario->ai_encounters.count))
			{
				struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_encounters,
					DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index),
					struct encounter_definition);
				short squad_index = 0;

				switch ((unsigned long)ai_reference >> 30)
				{
				case _ai_reference_type_encounter:
					break;
				case _ai_reference_type_platoon:
				{
					short platoon_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;

					for (squad_index = 0;
						squad_index < encounter_definition->squads.count;
						squad_index++)
					{
						struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
							&encounter_definition->squads,
							squad_index,
							struct squad_definition);

						if (squad_definition->platoon_index == platoon_index)
						{
							break;
						}
					}

					if (squad_index >= encounter_definition->squads.count)
					{
						squad_index = 0;
					}
					break;
				}
				case _ai_reference_type_squad:
					squad_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;
					break;
				default:
					match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 3154, !"unreachable");
					break;
				}

				if (VALID_INDEX(squad_index, encounter_definition->squads.count))
				{
					target_encounter_index = ai_reference;
					target_squad_index = squad_index;
				}
			}
		}

		if (target_encounter_index != NONE && target_squad_index != NONE)
		{
			short previous_encounter_index = vehicle->unit.fake_encounter_index;

			if (previous_encounter_index != NONE)
			{
				struct encounter_actor_iterator iterator;
				struct actor_datum *actor;

				encounter_actor_iterator_new(&iterator, previous_encounter_index);
				for (actor = encounter_actor_iterator_next(&iterator);
					actor;
					actor = encounter_actor_iterator_next(&iterator))
				{
					if (actor->input.vehicle_index == unit_index)
					{
						actor_change_encounter(
							iterator.index,
							target_encounter_index,
							target_squad_index);
					}
				}
			}
		}

		vehicle->unit.fake_encounter_index = target_encounter_index;
		vehicle->unit.fake_squad_index = target_squad_index;
	}

	return;
}

void ai_scripting_attach_unit(
	long unit_index,
	long ai_reference)
{
	struct scenario *scenario = global_scenario_get();

	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, scenario, ai_name, sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_attach_unit 0x%04X %s",
			hs_runtime_get_executing_thread_name(),
			unit_index & UNSIGNED_SHORT_MAX,
			ai_name);
	}

	if (ai_globals->ai_initialized_for_map &&
		unit_index != NONE &&
		ai_reference != NONE)
	{
		long encounter_index = ai_reference & UNSIGNED_SHORT_MAX;

		if (VALID_INDEX(encounter_index, scenario->ai_encounters.count))
		{
			struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters,
				DATUM_INDEX_TO_ABSOLUTE_INDEX(encounter_index),
				struct encounter_definition);
			long squad_index = 0;

			if (((unsigned long)ai_reference >> 30) == _ai_reference_type_squad)
			{
				squad_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;
			}
			else if (((unsigned long)ai_reference >> 30) == _ai_reference_type_platoon)
			{
				long platoon_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;
				long search_index;

				for (search_index = 0;
					search_index < encounter_definition->squads.count;
					search_index++)
				{
					if (TAG_BLOCK_GET_ELEMENT(
						&encounter_definition->squads,
						search_index,
						struct squad_definition)->platoon_index == platoon_index)
					{
						squad_index = search_index;
						break;
					}
				}
			}

			if (VALID_INDEX(squad_index, encounter_definition->squads.count))
			{
				struct squad_definition *squad_definition = TAG_BLOCK_GET_ELEMENT(
					&encounter_definition->squads,
					squad_index,
					struct squad_definition);

				if (squad_definition->actor_palette_index != NONE)
				{
					struct tag_reference *actor_palette_entry = TAG_BLOCK_GET_ELEMENT(
						&scenario->ai_actor_palette,
						squad_definition->actor_palette_index,
						struct tag_reference);

					if (actor_palette_entry->index != NONE)
					{
						long actor_definition_index = actor_variant_definition_get(
							actor_palette_entry->index)->actor_reference.index;

						if (actor_definition_index != NONE)
						{
							actor_create_for_unit(
								TEST_FLAG(
									actor_definition_get(actor_definition_index)->flags,
									_actor_definition_swarm_actor_bit),
								unit_index,
								actor_palette_entry->index,
								encounter_index,
								squad_index,
								FALSE,
								NONE,
								TEST_FLAG(
									encounter_definition->flags,
									_encounter_braindead_bit),
								squad_definition->initial_state,
								squad_definition->default_state,
								NONE,
								0);
							encounters_update_dirty_status();

							return;
						}
					}
				}

				error(
					_error_silent,
					"ai_attach: no actor variant specified for %s/%s, cannot create actor to attach to biped",
					encounter_definition->name,
					squad_definition->name);
			}
			else
			{
				error(
					_error_silent,
					"ai_attach: could not find a squad in encounter %s to attach actor",
					encounter_definition->name);
			}
		}
	}

	return;
}

void ai_scripting_allegiance(
	short team_index0,
	short team_index1)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_allegiance %d %d",
			hs_runtime_get_executing_thread_name(),
			team_index0,
			team_index1);
	}

	if (team_index0 != NONE && team_index1 != NONE)
	{
		short special_team_index = NONE;
		short incident_decay_time = NONE;
		short incident_threshold = NONE;
		boolean requires_communication = FALSE;
		boolean suspicious = FALSE;

		if (team_index0 == _game_team_player)
			special_team_index = team_index1;
		else if (team_index1 == _game_team_player)
			special_team_index = team_index0;

		switch (special_team_index)
		{
		case _game_team_human:
		case _game_team_sentinel:
		{
			short incident_decay_times[NUMBER_OF_GAME_DIFFICULTY_LEVELS] =
				{300, 450, 1200, 2700};

			suspicious = TRUE;
			incident_threshold = 5;
			incident_decay_time = incident_decay_times[game_difficulty_level_get()];
			requires_communication = special_team_index == _game_team_human;
			break;
		}
		}

		game_allegiance_create(
			team_index0,
			suspicious && team_index0 == special_team_index,
			team_index1,
			suspicious && team_index1 == special_team_index,
			incident_threshold,
			incident_decay_time,
			requires_communication);
	}

	return;
}

void ai_scripting_renew(
	long ai_reference)
{
	struct ai_script_actor_reference_iterator iterator;
	struct actor_datum *actor;

	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_renew %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	ai_index_actor_iterator_new(ai_reference, &iterator);
	for (actor = ai_index_actor_iterator_next(&iterator);
		actor;
		actor = ai_index_actor_iterator_next(&iterator))
	{
		if (actor->meta.unit_index != NONE)
		{
			struct actor_variant_definition *actor_variant_definition =
				actor_variant_definition_get(actor->meta.variant_definition_index);
			struct unit_datum *unit = unit_get(actor->meta.unit_index);

			unit->object.body_vitality = unit->object.maximum_body_vitality > 0.0f ? 1.0f : 0.0f;
			unit->object.shield_vitality = unit->object.maximum_shield_vitality > 0.0f ? 1.0f : 0.0f;

			if (actor_variant_definition->grenade_combat.grenade_type != NONE)
			{
				short desired_grenade_count = random_range(
					actor_variant_definition->items.grenades_lower_bound,
					actor_variant_definition->items.grenades_upper_bound + 1);
				short grenade_count = unit_get_grenade_count(
					actor->meta.unit_index,
					unit_get_current_grenade_type(actor->meta.unit_index));

				if (grenade_count < desired_grenade_count)
				{
					unit_add_grenade_type_to_inventory(
						actor->meta.unit_index,
						actor_variant_definition->grenade_combat.grenade_type,
						desired_grenade_count - grenade_count);
				}
			}
		}
	}

	return;
}

void ai_scripting_braindead_by_unit(
	long object_list_index,
	boolean braindead)
{
	long reference_index;
	long unit_index = object_list_get_first(object_list_index, &reference_index);

	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_braindead_by_unit <some guys> %s",
			hs_runtime_get_executing_thread_name(),
			braindead ? "on" : "off");
	}

	while (unit_index != NONE)
	{
		struct unit_datum *unit = unit_try_and_get(unit_index);

		if (unit)
		{
			long child_object_index;

			if (unit->unit.actor_index != NONE)
			{
				actor_braindead(unit->unit.actor_index, braindead);
			}
			else if (unit->unit.swarm_actor_index != NONE)
			{
				actor_braindead(unit->unit.swarm_actor_index, braindead);
			}

			child_object_index = unit->object.first_child_object_index;
			while (child_object_index != NONE)
			{
				struct object_datum *child_object = object_get(child_object_index);

				if (TEST_FLAG(_object_mask_unit, child_object->object.type))
				{
					struct unit_datum *child = (struct unit_datum *)child_object;

					if (child->unit.actor_index != NONE)
					{
						actor_braindead(child->unit.actor_index, braindead);
					}
					else if (child->unit.swarm_actor_index != NONE)
					{
						actor_braindead(child->unit.swarm_actor_index, braindead);
					}
				}

				child_object_index = child_object->object.next_object_index;
			}
		}

		unit_index = object_list_get_next(object_list_index, &reference_index);
	}

	return;
}

void ai_scripting_force_active_by_unit(
	long unit_index,
	boolean force)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_force_active_by_unit <some unit> %s",
			hs_runtime_get_executing_thread_name(),
			force ? "true" : "false");
	}

	if (unit_index != NONE)
	{
		long actor_index = unit_get(unit_index)->unit.actor_index;

		if (actor_index != NONE)
		{
			struct actor_datum *actor = actor_get(actor_index);

			if (actor->meta.encounterless)
			{
				actor->meta.force_active = force;
			}
			else
			{
				struct encounter_definition *encounter_definition = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_encounters,
					actor->meta.encounter_index & UNSIGNED_SHORT_MAX,
					struct encounter_definition);

				error(
					_error_silent,
					"ai_force_active_by_unit: unit is a member of encounter %s/%s, you must use ai_force_active instead",
					encounter_definition->name,
					TAG_BLOCK_GET_ELEMENT(
						&encounter_definition->squads,
						actor->meta.squad_index,
						struct squad_definition)->name);
			}
		}
	}

	return;
}

void ai_scripting_attach_free(
	long unit_index,
	long actor_variant_definition_index)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_attach_free 0x%04X %s",
			hs_runtime_get_executing_thread_name(),
			unit_index & UNSIGNED_SHORT_MAX,
			actor_variant_definition_index == NONE ?
				"<error>" :
				tag_name_strip_path(tag_get_name(actor_variant_definition_index)));
	}

	if (ai_globals->ai_initialized_for_map &&
		unit_index != NONE &&
		actor_variant_definition_index != NONE)
	{
		long actor_definition_index =
			actor_variant_definition_get(actor_variant_definition_index)->actor_reference.index;

		if (actor_definition_index != NONE)
		{
			if (TEST_FLAG(
				actor_definition_get(actor_definition_index)->flags,
				_actor_definition_swarm_actor_bit))
			{
				error(
					_error_silent,
					"%s: ai_attach_free %s cannot be used for swarm actors",
					hs_runtime_get_executing_thread_name(),
					tag_name_strip_path(tag_get_name(actor_variant_definition_index)));
			}
			else
			{
				actor_create_for_unit(
					FALSE,
					unit_index,
					actor_variant_definition_index,
					NONE,
					NONE,
					FALSE,
					NONE,
					FALSE,
					actor_default_state_alert,
					actor_default_state_none,
					NONE,
					0);
			}
		}
	}

	return;
}

void ai_scripting_try_to_fight(
	long ai_reference,
	long target_ai_reference)
{
	struct ai_script_actor_reference_iterator iterator;
	struct actor_datum *actor;

	if (ai_debug.print_scripting)
	{
		char ai_name[256];
		char target_ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		ai_index_to_string(target_ai_reference, global_scenario_get(), target_ai_name, sizeof(target_ai_name));
		error(
			_error_silent,
			"%s: ai_try_to_fight %s %s",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			target_ai_name);
	}

	ai_index_actor_iterator_new(ai_reference, &iterator);
	for (actor = ai_index_actor_iterator_next(&iterator);
		actor;
		actor = ai_index_actor_iterator_next(&iterator))
	{
		actor->external_orders.desired_target_type = _desired_target_ai;
		actor->external_orders.desired_target_ai_index = target_ai_reference;
	}

	return;
}

void ai_scripting_set_return_state(
	long ai_reference,
	short default_state)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_set_return_state %s %d",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			default_state);
	}

	if (VALID_INDEX(default_state, number_of_actor_default_states))
	{
		struct ai_script_actor_reference_iterator iterator;
		struct actor_datum *actor;

		ai_index_actor_iterator_new(ai_reference, &iterator);
		for (actor = ai_index_actor_iterator_next(&iterator);
			actor;
			actor = ai_index_actor_iterator_next(&iterator))
		{
			short action_class = actor_action_class(iterator.actor_index);

			actor->state.default_state = default_state;

			if (actor->state.combat_status == _actor_combat_status_none &&
				(action_class == _action_class_noncombat ||
					action_class == _action_class_passive ||
					action_class == _action_class_transitory))
			{
				actor_action_set_default_state(iterator.actor_index, NONE);
			}
		}
	}

	return;
}

void ai_scripting_magically_see_unit(
	long ai_reference,
	long unit_index)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_magically_see_unit %s 0x%04X",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			unit_index & UNSIGNED_SHORT_MAX);
	}

	if (ai_reference != NONE && unit_index != NONE)
	{
		struct ai_script_actor_reference_iterator iterator;
		struct actor_datum *actor;

		ai_index_actor_iterator_new(ai_reference, &iterator);
		for (actor = ai_index_actor_iterator_next(&iterator);
			actor;
			actor = ai_index_actor_iterator_next(&iterator))
		{
			long prop_index;

			if (actor->meta.encounter_index != NONE)
				encounter_force_activate(actor->meta.encounter_index);

			prop_index = prop_get_base_by_unit_index(iterator.actor_index, unit_index, TRUE, FALSE);
			if (prop_index != NONE)
				actor_handle_unit_effect(iterator.actor_index, prop_index, _ai_unit_effect_magic_sight);
		}
	}

	return;
}

void ai_scripting_magically_see_encounter(
	long ai_reference,
	long target_ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];
		char target_ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		ai_index_to_string(target_ai_reference, global_scenario_get(), target_ai_name, sizeof(target_ai_name));
		error(
			_error_silent,
			"%s: ai_magically_see_encounter %s %s",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			target_ai_name);
	}

	if (ai_reference != NONE && target_ai_reference != NONE)
	{
		struct ai_script_actor_reference_iterator iterator;
		struct actor_datum *actor;

		ai_index_actor_iterator_new(target_ai_reference, &iterator);
		for (actor = ai_index_actor_iterator_next(&iterator);
			actor;
			actor = ai_index_actor_iterator_next(&iterator))
		{
			if (actor->meta.unit_index != NONE)
			{
				ai_scripting_magically_see_unit(ai_reference, actor->meta.unit_index);
			}
			else if (actor->meta.swarm_unit_index != NONE)
			{
				ai_scripting_magically_see_unit(ai_reference, actor->meta.swarm_unit_index);
			}
		}
	}

	return;
}

void ai_scripting_link_activation(
	long ai_reference,
	long link_ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];
		char link_ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		ai_index_to_string(link_ai_reference, global_scenario_get(), link_ai_name, sizeof(link_ai_name));
		error(
			_error_silent,
			"%s: ai_link_activation %s %s",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			link_ai_name);
	}

	if (ai_reference != NONE && link_ai_reference != NONE)
	{
		if (!encounter_link_activation(ai_reference & UNSIGNED_SHORT_MAX, link_ai_reference))
		{
			error(
				_error_silent,
				"ai_link_activation: cannot link to another encounter, MAXIMUM_ACTIVATION_LINK_INDICES_PER_ENCOUNTER is %d",
				MAXIMUM_ACTIVATION_LINK_INDICES_PER_ENCOUNTER);
		}
	}

	return;
}

void ai_scripting_free(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_free %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		struct ai_script_actor_reference_iterator iterator;
		struct actor_datum *actor;

		ai_index_actor_iterator_new(ai_reference, &iterator);
		for (actor = ai_index_actor_iterator_next(&iterator);
			actor;
			actor = ai_index_actor_iterator_next(&iterator))
		{
			match_assert(
				"c:\\halo\\SOURCE\\ai\\ai_script.c",
				2769,
				actor->meta.encounter_index != NONE);

			actor_flush_position_indices(iterator.actor_index);
			encounter_detach_actor(iterator.actor_index, FALSE);
			encounterless_attach_actor(iterator.actor_index);
		}

		encounters_update_dirty_status();
	}

	return;
}

void ai_scripting_free_units(
	long object_list_index)
{
	long reference_index;
	long unit_index = object_list_get_first(object_list_index, &reference_index);
	short freed_count = 0;

	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_free_units <some units>",
			hs_runtime_get_executing_thread_name());
	}

	while (unit_index != NONE)
	{
		struct unit_datum *unit = unit_try_and_get(unit_index);

		if (unit &&
			unit->unit.actor_index != NONE &&
			actor_get(unit->unit.actor_index)->meta.encounter_index != NONE)
		{
			actor_flush_position_indices(unit->unit.actor_index);
			encounter_detach_actor(unit->unit.actor_index, FALSE);
			encounterless_attach_actor(unit->unit.actor_index);
			freed_count++;
		}

		unit_index = object_list_get_next(object_list_index, &reference_index);
	}

	if (freed_count > 0)
		encounters_update_dirty_status();

	return;
}

void ai_scripting_exit_vehicle(
	long ai_reference)
{
	struct ai_script_actor_reference_iterator iterator;
	struct actor_datum *actor;

	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_exit_vehicle %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	ai_index_actor_iterator_new(ai_reference, &iterator);
	for (actor = ai_index_actor_iterator_next(&iterator);
		actor;
		actor = ai_index_actor_iterator_next(&iterator))
	{
		if (actor->input.vehicle_index != NONE && actor->meta.unit_index != NONE)
			unit_try_and_exit_seat(actor->meta.unit_index);
	}

	return;
}

void ai_scripting_magically_see_players(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_magically_see_players %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		struct data_iterator iterator;
		struct player_datum *player;

		data_iterator_new(&iterator, player_data);
		for (player = data_iterator_next(&iterator);
			player;
			player = data_iterator_next(&iterator))
		{
			ai_scripting_magically_see_unit(ai_reference, player->unit_index);
		}
	}

	return;
}

void ai_scripting_set_current_state(
	long ai_reference,
	short current_state)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[512];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_set_current_state %s %d",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			current_state);
	}

	if (VALID_INDEX(current_state, number_of_actor_default_states))
	{
		struct ai_script_actor_reference_iterator iterator;

		ai_index_actor_iterator_new(ai_reference, &iterator);
		while (ai_index_actor_iterator_next(&iterator))
			actor_action_set_default_state(iterator.actor_index, current_state);
	}

	return;
}

void ai_scripting_command_list(
	long ai_reference,
	short command_list_index)
{
	struct ai_script_actor_reference_iterator iterator;

	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_command_list %s %d",
			hs_runtime_get_executing_thread_name(),
			ai_name,
			command_list_index);
	}

	ai_index_actor_iterator_new(ai_reference, &iterator);
	while (ai_index_actor_iterator_next(&iterator))
	{
		struct action_state_data action_data;

		if (action_obey_command_list_setup(
			iterator.actor_index,
			command_list_index,
			&action_data.obey))
		{
			actor_action_change(iterator.actor_index, _actor_action_obey, &action_data);
		}
	}

	return;
}

void ai_scripting_command_list_by_unit(
	long unit_index,
	short command_list_index)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_command_list_by_unit <unit> %d",
			hs_runtime_get_executing_thread_name(),
			command_list_index);
	}

	if (unit_index != NONE)
	{
		struct unit_datum *unit = unit_try_and_get(unit_index);

		if (unit && unit->unit.actor_index != NONE)
		{
			struct actor_datum *actor = actor_get(unit->unit.actor_index);
			struct action_state_data action_data;

			if (action_obey_command_list_setup(
				unit->unit.actor_index,
				command_list_index,
				&action_data.obey))
			{
				actor_action_change(unit->unit.actor_index, _actor_action_obey, &action_data);
			}
		}
	}

	return;
}

void ai_scripting_try_to_fight_nothing(
	long ai_reference)
{
	struct ai_script_actor_reference_iterator iterator;
	struct actor_datum *actor;

	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_try_to_fight_nothing %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	ai_index_actor_iterator_new(ai_reference, &iterator);
	for (actor = ai_index_actor_iterator_next(&iterator);
		actor;
		actor = ai_index_actor_iterator_next(&iterator))
	{
		actor->external_orders.desired_target_type = _desired_target_none;
	}

	return;
}

void ai_scripting_try_to_fight_player(
	long ai_reference)
{
	struct ai_script_actor_reference_iterator iterator;
	struct actor_datum *actor;

	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_try_to_fight_player %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	ai_index_actor_iterator_new(ai_reference, &iterator);
	for (actor = ai_index_actor_iterator_next(&iterator);
		actor;
		actor = ai_index_actor_iterator_next(&iterator))
	{
		actor->external_orders.desired_target_type = _desired_target_player;
	}

	return;
}

void ai_scripting_ignore(
	long object_list_index,
	boolean ignore)
{
	long reference_index;
	long unit_index = object_list_get_first(object_list_index, &reference_index);

	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_disregard <some guys> %s",
			hs_runtime_get_executing_thread_name(),
			ignore ? "on" : "off");
	}

	while (unit_index != NONE)
	{
		struct unit_datum *unit = unit_try_and_get(unit_index);

		if (unit)
			SET_FLAG(unit->unit.flags, _unit_ignored_by_actors_bit, ignore);

		unit_index = object_list_get_next(object_list_index, &reference_index);
	}

	return;
}

void ai_scripting_prefer_target(
	long object_list_index,
	boolean prefer)
{
	long reference_index;
	long unit_index = object_list_get_first(object_list_index, &reference_index);

	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_prefer_target <some guys> %s",
			hs_runtime_get_executing_thread_name(),
			prefer ? "on" : "off");
	}

	while (unit_index != NONE)
	{
		struct unit_datum *unit = unit_try_and_get(unit_index);

		if (unit)
			SET_FLAG(unit->unit.flags, _unit_preferred_target_bit, prefer);

		unit_index = object_list_get_next(object_list_index, &reference_index);
	}

	return;
}

static void ai_scripting_teleport_starting_location_private(
	long ai_reference,
	boolean only_if_unsupported)
{
	struct ai_script_actor_reference_iterator iterator;
	struct actor_datum *actor;

	ai_index_actor_iterator_new(ai_reference, &iterator);
	for (actor = ai_index_actor_iterator_next(&iterator);
		actor;
		actor = ai_index_actor_iterator_next(&iterator))
	{
		if (actor->meta.unit_index == NONE)
			continue;
		if (only_if_unsupported &&
			(actor->input.vehicle_index != NONE ||
			biped_approximate_surface_index(actor->meta.unit_index, NULL) != NONE))
		{
			continue;
		}
		if (actor->meta.encounter_index == NONE)
			continue;

		{
			struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
				&global_scenario_get()->ai_encounters,
				DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
				struct encounter_definition);
			struct squad_definition *squad = TAG_BLOCK_GET_ELEMENT(
				&encounter->squads,
				actor->meta.squad_index,
				struct squad_definition);
			short starting_location_index = encounter_get_actor_starting_location(
				actor->meta.encounter_index,
				actor->meta.squad_index,
				TRUE);

			if (starting_location_index != NONE)
			{
				struct actor_starting_location const *starting_location = TAG_BLOCK_GET_ELEMENT(
					&squad->starting_locations,
					starting_location_index,
					struct actor_starting_location);
				real_vector3d forward;

				vector3d_from_angle(&forward, starting_location->facing);
				object_set_position(actor->meta.unit_index, &starting_location->position, &forward, NULL);
				object_reset(actor->meta.unit_index);
				actor_move_halt(iterator.actor_index);
			}
		}
	}

	return;
}

void ai_scripting_teleport_starting_location_if_unsupported(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_teleport_starting_location_if_unsupported %s",
			hs_runtime_get_executing_thread_name(),
			ai_name);
	}

	ai_scripting_teleport_starting_location_private(ai_reference, TRUE);

	return;
}

void ai_scripting_teleport_starting_location(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(
			_error_silent,
			"%s: ai_teleport_starting_location %s",
			hs_runtime_get_executing_thread_name(),
			ai_name);
	}

	ai_scripting_teleport_starting_location_private(ai_reference, FALSE);

	return;
}

void ai_scripting_command_list_advance(
	long ai_reference)
{
	struct ai_script_actor_reference_iterator iterator;

	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_command_list_advance %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	ai_index_actor_iterator_new(ai_reference, &iterator);
	while (ai_index_actor_iterator_next(&iterator))
		action_obey_advance_command_list(iterator.actor_index);

	return;
}

void ai_scripting_erase(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_erase %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		long squad_index = ((unsigned long)ai_reference >> 30) == _ai_reference_type_squad ?
			(ai_reference >> 16) & UNSIGNED_CHAR_MAX : NONE;
		long platoon_index = ((unsigned long)ai_reference >> 30) == _ai_reference_type_platoon ?
			(ai_reference >> 16) & UNSIGNED_CHAR_MAX : NONE;

		ai_erase(ai_reference & UNSIGNED_SHORT_MAX, platoon_index, squad_index, FALSE);
	}

	return;
}

void ai_scripting_place(
	long ai_reference)
{
	if (ai_debug.print_scripting)
	{
		char ai_name[256];

		ai_index_to_string(ai_reference, global_scenario_get(), ai_name, sizeof(ai_name));
		error(_error_silent, "%s: ai_place %s", hs_runtime_get_executing_thread_name(), ai_name);
	}

	if (ai_reference != NONE)
	{
		long squad_index = ((unsigned long)ai_reference >> 30) == _ai_reference_type_squad ?
			(ai_reference >> 16) & UNSIGNED_CHAR_MAX : NONE;
		long platoon_index = ((unsigned long)ai_reference >> 30) == _ai_reference_type_platoon ?
			(ai_reference >> 16) & UNSIGNED_CHAR_MAX : NONE;

		encounter_create(ai_reference & UNSIGNED_SHORT_MAX, platoon_index, squad_index);
	}

	return;
}

void ai_scripting_look_at_object(
	long unit_index,
	long object_index)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_look_at_object <some unit> <some object>",
			hs_runtime_get_executing_thread_name());
	}

	if (unit_index != NONE && object_index != NONE)
	{
		long actor_index = unit_get(unit_index)->unit.actor_index;

		if (actor_index != NONE)
		{
			struct direction_specification direction;

			direction.type = _direction_specification_object;
			direction.object_index = object_index;

			actor_look_secondary(
				actor_index,
				_secondary_look_scripted,
				_secondary_look_priority_default,
				&direction);
		}
	}

	return;
}

void ai_scripting_set_team(
	long ai_reference,
	short team_index)
{
	long encounter_index = ai_reference & UNSIGNED_SHORT_MAX;
	struct encounter_actor_iterator iterator;
	struct actor_datum *actor;

	encounter_get(encounter_index)->team_index = team_index;

	encounter_actor_iterator_new(&iterator, encounter_index);
	for (actor = encounter_actor_iterator_next(&iterator);
		actor;
		actor = encounter_actor_iterator_next(&iterator))
	{
		actor_set_team(iterator.index, team_index);
	}

	ai_update_team_status();

	return;
}

void ai_scripting_command_list_advance_by_unit(
	long unit_index)
{
	if (ai_debug.print_scripting)
	{
		error(
			_error_silent,
			"%s: ai_command_list_advance_by_unit <some unit>",
			hs_runtime_get_executing_thread_name());
	}

	if (unit_index != NONE)
	{
		struct unit_datum *unit = unit_try_and_get(unit_index);

		if (unit)
		{
			if (unit->unit.actor_index != NONE)
			{
				action_obey_advance_command_list(unit->unit.actor_index);
			}
			else if (unit->unit.swarm_actor_index != NONE)
			{
				action_obey_advance_command_list(unit->unit.swarm_actor_index);
			}
		}
	}

	return;
}

static short ai_scripting_command_list_status_internal(
	long actor_index,
	long unit_index,
	short command_list_index,
	struct obey_individual_simple_control *simple_control,
	struct obey_individual_complex_control *complex_control)
{
	struct ai_command_list_definition *command_list = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_command_lists,
		command_list_index,
		struct ai_command_list_definition);
	struct ai_command_definition *command = NULL;
	short status;

	if (simple_control->current_command_index < command_list->commands.count)
	{
		command = TAG_BLOCK_GET_ELEMENT(
			&command_list->commands,
			simple_control->current_command_index,
			struct ai_command_definition);
	}

	if (!command)
	{
		status = 1;
	}
	else if (TEST_FLAG(simple_control->metadata_flags, _obey_metadata_waiting_for_advance_notification_bit))
	{
		status = 2;
	}
	else
	{
		status = 3;
	}

	return status;
}

short ai_scripting_command_list_status(
	long object_list_index)
{
	short result = 0;
	long game_time = game_time_get();
	long reference_index;
	long unit_index = object_list_get_first(object_list_index, &reference_index);

	while (unit_index != NONE)
	{
		struct unit_datum *unit = unit_try_and_get(unit_index);

		if (unit)
		{
			struct actor_datum *actor = NULL;
			short status = 0;

			if (unit->unit.actor_index != NONE)
			{
				actor = actor_get(unit->unit.actor_index);
				match_assert(
					"c:\\halo\\SOURCE\\ai\\ai_script.c",
					2688,
					!actor->meta.swarm);
				if (actor->state.action == _actor_action_obey)
				{
					status = ai_scripting_command_list_status_internal(
						unit->unit.actor_index,
						unit_index,
						actor->state.action_data.obey.command_list_index,
						&actor->state.action_data.obey.simple_control,
						&actor->state.action_data.obey.complex_control);
				}
			}
			else if (unit->unit.swarm_actor_index != NONE)
			{
				actor = actor_get(unit->unit.swarm_actor_index);
				match_assert(
					"c:\\halo\\SOURCE\\ai\\ai_script.c",
					2701,
					actor->meta.swarm);
				if (actor->state.action == _actor_action_obey &&
					actor->meta.swarm_cache_index != NONE)
				{
					struct swarm_datum *swarm = swarm_get(actor->meta.swarm_cache_index);
					short swarm_unit_index = 0;

					while (swarm_unit_index < swarm->unit_count &&
						swarm->unit_indices[swarm_unit_index] != unit_index)
					{
						swarm_unit_index++;
					}

					if (swarm_unit_index < swarm->unit_count)
					{
						struct swarm_component_datum *component = swarm_component_get(
							swarm->component_indices[swarm_unit_index]);

						if (TEST_FLAG(component->flags, _swarm_component_obey_bit))
						{
							status = ai_scripting_command_list_status_internal(
								unit->unit.swarm_actor_index,
								unit_index,
								actor->state.action_data.obey.command_list_index,
								&component->obey,
								NULL);
						}
					}
				}
			}

			if (!status &&
				actor &&
				actor->state.last_command_list_time != NONE &&
				actor->state.last_command_list_time + 150 >= game_time)
			{
				status = 1;
			}
			result = MAX(result, status);
		}

		unit_index = object_list_get_next(object_list_index, &reference_index);
	}

	return result;
}

/* ---------- private code */

static int ai_scripting_vehicle_candidate_qsort(
	void const *candidate0,
	void const *candidate1)
{
	struct ai_script_vehicle_candidate const *vehicle0 = candidate0;
	struct ai_script_vehicle_candidate const *vehicle1 = candidate1;

	if (vehicle0->already_going_to_vehicle != vehicle1->already_going_to_vehicle)
		return vehicle0->already_going_to_vehicle ? 1 : -1;

	if (vehicle0->distance_squared < vehicle1->distance_squared)
		return -1;

	if (vehicle0->distance_squared > vehicle1->distance_squared)
		return 1;

	return 0;
}

static void ai_scripting_go_to_vehicle_internal(
	long ai_reference,
	long unit_index,
	char const *seat_substring_name,
	boolean override_previous_vehicles)
{
	struct unit_datum *vehicle;
	real_point3d vehicle_origin;
	short seat_indices[16];
	struct ai_script_vehicle_candidate candidates[64];
	struct ai_script_actor_reference_iterator iterator;
	struct actor_datum *actor;
	short candidate_count;
	short available_seat_count;
	short candidate_index;

	vehicle = unit_try_and_get(unit_index);
	if (ai_reference == NONE || vehicle == NULL)
		return;

	candidate_count = 0;
	object_get_origin(unit_index, &vehicle_origin);
	available_seat_count = vehicle_scripting_find_available_seats(
		unit_index,
		seat_substring_name,
		NONE,
		seat_indices,
		NUMBEROF(seat_indices));
	if (available_seat_count <= 0)
		return;

	ai_index_actor_iterator_new(ai_reference, &iterator);
	actor = ai_index_actor_iterator_next(&iterator);
	while (actor)
	{
		if ((word)candidate_count < NUMBEROF(candidates))
		{
			real dx;
			real dy;
			real dz;

			candidates[candidate_count].actor_index = iterator.actor_index;
			dx = vehicle_origin.x - actor->input.position.body_position.x;
			dy = vehicle_origin.y - actor->input.position.body_position.y;
			dz = vehicle_origin.z - actor->input.position.body_position.z;

			candidates[candidate_count].distance_squared =
				dy * dy + (dx * dx + dz * dz);
			candidates[candidate_count].already_going_to_vehicle =
				actor->state.action == _actor_action_vehicle;
			candidate_count++;
		}

		actor = ai_index_actor_iterator_next(&iterator);
	}

	qsort(candidates, candidate_count, sizeof(candidates[0]), ai_scripting_vehicle_candidate_qsort);

	for (candidate_index = 0; candidate_index < candidate_count; candidate_index++)
	{
		if (candidates[candidate_index].already_going_to_vehicle &&
			!override_previous_vehicles)
		{
			break;
		}

		actor_action_try_to_enter_vehicle(
			candidates[candidate_index].actor_index,
			unit_index,
			NULL,
			NONE,
			available_seat_count,
			seat_indices);
	}

	return;
}

static long ai_scripting_assess_status(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);

	if (!actor->meta.active)
		return 0;

	if (actor->state.mode < _actor_mode_combat)
		return 1;

	if (!actor->state.combat_status)
		return 2;

	if (actor->target.target_type < _actor_target_definite_orphan)
		return 3;

	if (actor->target.target_type < _actor_target_visible_enemy)
		return 4;

	if (actor->orders.combat.shoot_at_target || actor->orders.combat.throw_grenade)
		return 6;

	return 5;
}

static void ai_scripting_kill_internal(
	long ai_reference,
	boolean silent)
{
	struct ai_script_actor_reference_iterator iterator;

	if (ai_reference == NONE)
		return;

	ai_index_actor_iterator_new(ai_reference, &iterator);
	while (ai_index_actor_iterator_next(&iterator))
		actor_kill(iterator.actor_index, silent, FALSE);

	return;
}

static long ai_scripting_count_internal(
	long ai_reference,
	short count_type,
	long *original_count_reference,
	real *strength_reference)
{
	long count = 0;
	long original_count = 0;
	real strength = 0.0f;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\ai_script.c",
		1029,
		(count_type >= 0) && (count_type < NUMBER_OF_AI_COUNT_TYPES));

	if (ai_reference != NONE)
	{
		struct scenario *scenario = global_scenario_get();

		switch ((unsigned long)ai_reference >> 30)
		{
		case 0:
		{
			long encounter_index = ai_reference & UNSIGNED_SHORT_MAX;
			if (VALID_INDEX(encounter_index, scenario->ai_encounters.count))
			{
				struct encounter_datum *encounter = encounter_get(encounter_index);

				switch (count_type)
				{
				case _ai_count_living:
					count = encounter->current_count;
					break;
				case _ai_count_swarm:
					count = encounter->current_swarm_count;
					break;
				case _ai_count_nonswarm:
					count = FLOOR(encounter->current_count - encounter->current_swarm_count, 0);
					break;
				default:
					match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 1060, !"unreachable");
					break;
				}

				original_count = encounter->original_count;
				strength = encounter->current_strength_fraction;
			}
			break;
		}
		case 1:
		{
			long encounter_index = ai_reference & UNSIGNED_SHORT_MAX;
			if (VALID_INDEX(encounter_index, scenario->ai_encounters.count))
			{
				struct encounter_datum *encounter = encounter_get(encounter_index);
				short platoon_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;

				if (VALID_INDEX(platoon_index, encounter->platoon_count))
				{
					struct platoon_datum *platoon = encounter_get_platoon(encounter, platoon_index);

					switch (count_type)
					{
					case _ai_count_living:
						count = platoon->current_count;
						break;
					case _ai_count_swarm:
						count = platoon->current_swarm_count;
						break;
					case _ai_count_nonswarm:
						count = FLOOR(platoon->current_count - platoon->current_swarm_count, 0);
						break;
					default:
						match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 1096, !"unreachable");
						break;
					}

					original_count = platoon->original_count;
					strength = platoon->current_strength_fraction;
				}
			}
			break;
		}
		case 2:
		{
			long encounter_index = ai_reference & UNSIGNED_SHORT_MAX;
			if (VALID_INDEX(encounter_index, scenario->ai_encounters.count))
			{
				struct encounter_datum *encounter = encounter_get(encounter_index);
				short squad_index = (ai_reference >> 16) & UNSIGNED_CHAR_MAX;

				if (VALID_INDEX(squad_index, encounter->squad_count))
				{
					struct squad_datum *squad = encounter_get_squad(encounter, squad_index);

					switch (count_type)
					{
					case _ai_count_living:
						count = squad->current_count;
						break;
					case _ai_count_swarm:
						count = squad->current_swarm_count;
						break;
					case _ai_count_nonswarm:
						count = FLOOR(squad->current_count - squad->current_swarm_count, 0);
						break;
					default:
						match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 1133, !"unreachable");
						break;
					}

					original_count = squad->original_count;
					strength = squad->current_strength_fraction;
				}
			}
			break;
		}
		default:
			match_assert("c:\\halo\\SOURCE\\ai\\ai_script.c", 1143, !"unreachable");
			break;
		}
	}

	if (original_count_reference)
		*original_count_reference = original_count;
	if (strength_reference)
		*strength_reference = strength;

	return count;
}
