/*
GAME_ENGINE.C

symbols in this file:
00096780 0020:
	_game_globals_get_weapon (0000)
000967A0 0070:
	_game_engine_get_team_score (0000)
00096810 0020:
	_linear_to_non_linear_alpha (0000)
00096830 0020:
	_game_engine_dispose (0000)
00096850 0040:
	_code_00096850 (0000)
00096890 0120:
	_code_00096890 (0000)
000969B0 0020:
	_game_engine_dispose_from_old_map (0000)
000969D0 0020:
	_game_engine_game_ending (0000)
000969F0 0020:
	_game_engine_game_starting (0000)
00096A10 0040:
	_players_in_game (0000)
00096A50 0020:
	_game_engine_statistics_append (0000)
00096A70 0020:
	_game_engine_handle_client_message (0000)
00096A90 0020:
	_game_engine_handle_server_message (0000)
00096AB0 0030:
	_sort_statistic_buffer (0000)
00096AE0 0050:
	_sort_statistic_buffer_ranking (0000)
00096B30 0010:
	_code_00096b30 (0000)
00096B40 0010:
	_code_00096b40 (0000)
00096B50 0050:
	_code_00096b50 (0000)
00096BA0 0070:
	_code_00096ba0 (0000)
00096C10 0020:
	_code_00096c10 (0000)
00096C30 0050:
	_get_postgame_hilite_colors (0000)
00096C80 0060:
	_code_00096c80 (0000)
00096CE0 0020:
	_game_engine_post_rasterize_objects (0000)
00096D00 0030:
	_code_00096d00 (0000)
00096D30 0100:
	_code_00096d30 (0000)
00096E30 00a0:
	_code_00096e30 (0000)
00096ED0 0150:
	_code_00096ed0 (0000)
00097020 00b0:
	_code_00097020 (0000)
000970D0 0050:
	_game_engine_load_stage (0000)
00097120 0030:
	_game_engine_playlist_begin (0000)
00097150 0060:
	_game_engine_get_current_stage (0000)
000971B0 0030:
	_game_engine_end_game (0000)
000971E0 0020:
	_game_engine_allow_pick_up (0000)
00097200 0050:
	_game_engine_player_damaged_player (0000)
00097250 00a0:
	_code_00097250 (0000)
000972F0 0040:
	_game_engine_player_is_out_of_lives (0000)
00097330 0100:
	_game_engine_should_spawn_player (0000)
00097430 0070:
	_game_engine_player_get_team_index (0000)
000974A0 0040:
	_game_engine_prespawn_player_update (0000)
000974E0 0010:
	_game_engine_running (0000)
000974F0 0020:
	_game_engine_can_score (0000)
00097510 0020:
	_game_engine_force_single_screen (0000)
00097530 0030:
	_get_blink_alpha (0000)
00097560 0010:
	_code_00097560 (0000)
00097570 00f0:
	_code_00097570 (0000)
00097660 0100:
	_game_engine_rasterize_message (0000)
00097760 00e0:
	_game_engine_picking_up (0000)
00097840 0080:
	_code_00097840 (0000)
000978C0 00a0:
	_game_engine_player_get_custom_motion_sensor_positions (0000)
00097960 0030:
	_get_flag_definition_index (0000)
00097990 0030:
	_get_ball_definition_index (0000)
000979C0 0040:
	_game_engine_switch_to_postgame (0000)
00097A00 0010:
	_game_engine_get_variant (0000)
00097A10 0020:
	_game_engine_get_goal_in_use (0000)
00097A30 0060:
	_game_engine_get_goal_position (0000)
00097A90 0080:
	_game_engine_set_goal_position (0000)
00097B10 0020:
	_game_engine_clear_goal_position (0000)
00097B30 00d0:
	_game_engine_render_nav_points (0000)
00097C00 0020:
	_code_00097c00 (0000)
00097C20 0030:
	_game_engine_infinite_grenades (0000)
00097C50 0020:
	_game_engine_has_teams (0000)
00097C70 0030:
	_game_engine_display_team_indicators (0000)
00097CA0 0030:
	_game_engine_has_shield (0000)
00097CD0 0060:
	_weapon_definition_index_to_list_index (0000)
00097D30 0030:
	_list_index_to_weapon_definition_index (0000)
00097D60 00c0:
	_game_engine_remap_vehicle (0000)
00097E20 0190:
	_game_engine_remap_weapon (0000)
00097FB0 0070:
	_game_engine_man_out (0000)
00098020 0030:
	_game_engine_state_message (0000)
00098050 0090:
	_game_engine_player_get_change_color (0000)
000980E0 0070:
	_game_engine_player_has_flag (0000)
00098150 0050:
	_game_engine_player_depower_active_camo (0000)
000981A0 00e0:
	_get_place_name (0000)
00098280 0170:
	_game_engine_get_place (0000)
000983F0 0040:
	_game_engine_hud_draw_motion_sensor (0000)
00098430 0020:
	_game_engine_test_flag (0000)
00098450 0020:
	_game_engine_test_trait (0000)
00098470 00a0:
	_netgame_flag_verify_no_team_duplicates (0000)
00098510 0070:
	_netgame_flag_verify_team_range (0000)
00098580 0070:
	_game_engine_playlist_next (0000)
000985F0 0090:
	_build_game_variant_slayer (0000)
00098680 0090:
	_build_game_variant_slayer_pro (0000)
00098710 0090:
	_build_game_variant_elimination (0000)
000987A0 0090:
	_build_game_variant_phantoms (0000)
00098830 0090:
	_build_game_variant_endurance (0000)
000988C0 0090:
	_build_game_variant_rockets (0000)
00098950 0090:
	_build_game_variant_snipers (0000)
000989E0 0090:
	_build_game_variant_team_slayer (0000)
00098A70 0090:
	_build_game_variant_oddball (0000)
00098B00 0090:
	_build_game_variant_team_oddball (0000)
00098B90 0090:
	_build_game_variant_reverse_tag (0000)
00098C20 00a0:
	_build_game_variant_accumulation (0000)
00098CC0 00a0:
	_build_game_variant_juggernaut (0000)
00098D60 00a0:
	_build_game_variant_stalker (0000)
00098E00 0080:
	_build_game_variant_king (0000)
00098E80 0090:
	_build_game_variant_king_pro (0000)
00098F10 0080:
	_build_game_variant_crazy_king (0000)
00098F90 0090:
	_build_game_variant_team_king (0000)
00099020 0090:
	_build_game_variant_ctf (0000)
000990B0 0090:
	_build_game_variant_ctf_pro (0000)
00099140 0090:
	_build_game_variant_invasion (0000)
000991D0 0090:
	_build_game_variant_iron_ctf (0000)
00099260 0090:
	_build_game_variant_race (0000)
000992F0 0090:
	_build_game_variant_rally (0000)
00099380 0090:
	_build_game_variant_team_race (0000)
00099410 0090:
	_build_game_variant_team_rally (0000)
000994A0 0030:
	_game_engine_override_map_name (0000)
000994D0 0020:
	_game_engine_override_game_variant (0000)
000994F0 01a0:
	_rasterize_in_game_score_draw_line (0000)
00099690 0060:
	_game_engine_hud_draw_messages (0000)
000996F0 0080:
	_game_engine_player_has_stealth_weapon (0000)
00099770 0110:
	_game_engine_weapon_fired (0000)
00099880 0030:
	_code_00099880 (0000)
000998B0 00c0:
	_ticks_to_unicode_time_string (0000)
00099970 0060:
	_game_engine_flag_reset (0000)
000999D0 01b0:
	_game_engine_variant_cleanup (0000)
00099B80 0010:
	_game_engine_allow_pause (0000)
00099B90 0270:
	_code_00099b90 (0000)
00099E00 0020:
	_game_engine_draw_object_in_motion_sensor (0000)
00099E20 0020:
	_game_engine_allow_dynamic_lighting (0000)
00099E40 0020:
	_game_engine_allow_integrated_lights (0000)
00099E60 0030:
	_game_engine_force_autopickup (0000)
00099E90 0060:
	_game_engine_initialize (0000)
00099EF0 0100:
	_multiple_teams_alive (0000)
00099FF0 00f0:
	_team_has_players (0000)
0009A0E0 0020:
	_game_engine_should_end_game (0000)
0009A100 0080:
	_code_0009a100 (0000)
0009A180 0230:
	_populate_statistic_buffer (0000)
0009A3B0 0080:
	_code_0009a3b0 (0000)
0009A430 0060:
	_postgame_statistic_get_rating (0000)
0009A490 01f0:
	_code_0009a490 (0000)
0009A680 01c0:
	_code_0009a680 (0000)
0009A840 0100:
	_code_0009a840 (0000)
0009A940 0920:
	_code_0009a940 (0000)
0009B260 0080:
	_code_0009b260 (0000)
0009B2E0 0020:
	_code_0009b2e0 (0000)
0009B300 00a0:
	_match_game_type (0000)
0009B3A0 0150:
	_code_0009b3a0 (0000)
0009B4F0 0070:
	_code_0009b4f0 (0000)
0009B560 0060:
	_game_engine_update_player_always_invis (0000)
0009B5C0 00e0:
	_game_engine_update_non_deterministic (0000)
0009B6A0 0040:
	_code_0009b6a0 (0000)
0009B6E0 0090:
	_code_0009b6e0 (0000)
0009B770 0010:
	_code_0009b770 (0000)
0009B780 0060:
	_game_show_score_team (0000)
0009B7E0 00d0:
	_game_show_score_you_ally_enemy (0000)
0009B8B0 0080:
	_game_show_score_extended (0000)
0009B930 0020:
	_game_show_score (0000)
0009B950 0110:
	_find_netgame_flags (0000)
0009BA60 0040:
	_find_netgame_flag (0000)
0009BAA0 0130:
	_code_0009baa0 (0000)
0009BBD0 0150:
	_game_engine_postspawn_player_update (0000)
0009BD20 00d0:
	_game_engine_get_damage_multiplier (0000)
0009BDF0 03e0:
	_game_engine_update_teleporter (0000)
0009C1D0 0170:
	_game_engine_get_distance_rating_for_spawn (0000)
0009C340 0120:
	_code_0009c340 (0000)
0009C460 00b0:
	_code_0009c460 (0000)
0009C510 0060:
	_game_engine_get_starting_location_rating (0000)
0009C570 0220:
	_game_engine_get_variant_by_name (0000)
0009C790 0130:
	_game_engine_remap_equipment (0000)
0009C8C0 0070:
	_game_engine_remap_object_definition (0000)
0009C930 0140:
	_game_engine_get_state_message (0000)
0009CA70 00c0:
	_game_engine_did_player_win_default (0000)
0009CB30 0030:
	_game_engine_did_player_win (0000)
0009CB60 0080:
	_code_0009cb60 (0000)
0009CBE0 0040:
	_netgame_flag_verify_team_exists (0000)
0009CC20 0060:
	_netgame_verify_spawn_points (0000)
0009CC80 0070:
	_netgame_verify_equipment (0000)
0009CCF0 0280:
	_game_engine_verify_current_map (0000)
0009CF70 0010:
	_game_engine_playlist_initialize (0000)
0009CF80 0080:
	_game_engine_initialize_for_new_map (0000)
0009D000 0140:
	_game_engine_player_added (0000)
0009D140 0480:
	_code_0009d140 (0000)
0009D5C0 09e0:
	_game_engine_post_rasterize_post_game (0000)
0009DFA0 02f0:
	_game_engine_update (0000)
0009E290 0340:
	_game_engine_player_killed (0000)
0009E5D0 00a0:
	_game_engine_nonplayer_post_rasterize (0000)
0009E670 0350:
	_code_0009e670 (0000)
0009E9C0 0140:
	_code_0009e9c0 (0000)
0009EB00 0060:
	_game_engine_post_rasterize (0000)
0025B190 0008:
	__real@3ffe666660000000 (0000)
0025B198 0002:
	??_C@_11LOCGONAA@?$AA?$AA@ (0000)
0025B19C 0019:
	??_C@_0BJ@IAIEOKNM@ui?2multiplayer_game_text?$AA@ (0000)
0025B1B8 001d:
	??_C@_0BN@BLHEDFPC@weapon_is_flag?$CIweapon_index?$CJ?$AA@ (0000)
0025B1D8 0022:
	??_C@_0CC@HKAICMNO@c?3?2halo?2SOURCE?2game?2game_engine?4@ (0000)
0025B1FC 003c:
	??_C@_0DM@INOFLLBD@?$CIitem?9?$DOobject?4scale?5?$DO?$DN?50?45f?$CJ?5?$CG?$CG?5@ (0000)
0025B238 0014:
	??_C@_0BE@PEGHDBAP@NULL?5?$CB?$DN?5game_engine?$AA@ (0000)
0025B24C 0014:
	??_C@_0BE@CKCLKIJA@variant?5?$CG?$CG?5map_name?$AA@ (0000)
0025B260 001a:
	??_C@_0BK@EKPIBKAB@dead_player_index?5?$CB?$DN?5NONE?$AA@ (0000)
0025B27C 000c:
	??_C@_0M@CLNODINK@game_engine?$AA@ (0000)
0025B288 0008:
	__real@3f53104b57cf969e (0000)
0025B290 0007:
	??_C@_06IEOJBDIK@object?$AA@ (0000)
0025B298 0097:
	??_C@_0JH@PNHLNBOL@?$CBallow_pick_up?5?$HM?$HM?5?$CBTEST_FLAG?$CIwea@ (0000)
0025B330 001a:
	??_C@_0BK@LOIEOAAE@global_goal?$FLindex?$FN?4in_use?$AA@ (0000)
0025B34C 0004:
	__real@3f2147ae (0000)
0025B350 0015:
	??_C@_0BF@MOJDLNAG@NONE?5?$CB?$DN?5lookup_index?$AA@ (0000)
0025B368 001d:
	??_C@_0BN@OIBHHACP@place?4place?5?$DM?5maximum_places?$AA@ (0000)
0025B388 002c:
	??_C@_0CM@IEEKPKHB@?$CI?$CBall_tied?5?$HM?$HM?5?$CItied?$CJ?$CJ?5?$HM?$HM?5?$CI1?5?$DN?$DN?5g@ (0000)
0025B3B4 001e:
	??_C@_0BO@HPNHAHOG@levels?2test?2carousel?2carousel?$AA@ (0000)
0025B3D4 000c:
	??_C@_1M@CDCAHPKB@?$AA?$CF?$AAs?$AA?3?$AA?$CF?$AAs?$AA?$AA@ (0000)
0025B3E0 0008:
	??_C@_17PMDFIHDK@?$AA0?$AA?$CF?$AAd?$AA?$AA@ (0000)
0025B3E8 0006:
	??_C@_15KNBIKKIN@?$AA?$CF?$AAd?$AA?$AA@ (0000)
0025B3F0 0004:
	??_C@_13HOIJIPNN@?$AA?5?$AA?$AA@ (0000)
0025B3F8 0046:
	??_C@_0EG@GDPJDEHO@NETGAME?5CODE?5FAILURE?3?5game_engin@ (0000)
0025B440 001b:
	??_C@_0BL@FBPHGMEB@player?9?$DOteam_index?5?$CB?$DN?5NONE?$AA@ (0000)
0025B45C 0013:
	??_C@_0BD@JDPLED@player_index?$CB?$DNNONE?$AA@ (0000)
0025B470 002b:
	??_C@_0CL@DLMKKJM@player_count?5?$DM?5MULTIPLAYER_MAXIM@ (0000)
0025B49C 0022:
	??_C@_0CC@FPLNHLNP@place?$DMMULTIPLAYER_MAXIMUM_PLAYER@ (0000)
0025B4C0 0013:
	??_C@_0BD@EGHPGJBP@found?5local?5player?$AA@ (0000)
0025B4D4 001d:
	??_C@_0BN@LMCPNAFN@player_count?$DN?$CFd?0?5maxcount?$DN?$CFd?$AA@ (0000)
0025B4F8 0008:
	__real@3fc0bf25a0000000 (0000)
0025B500 0015:
	??_C@_0BF@IHIEPAI@NONE?5?$CB?$DN?5player_index?$AA@ (0000)
0025B518 0016:
	??_C@_0BG@OJNPNGFL@failed?5to?5teleport?5?$CFd?$AA@ (0000)
0025B530 0008:
	__real@3fe3333340000000 (0000)
0025B538 000a:
	??_C@_09DOOCKEDN@team_king?$AA@ (0000)
0025B544 0005:
	??_C@_04PJOEONHN@king?$AA@ (0000)
0025B54C 0008:
	??_C@_07LIOCHKOH@ironctf?$AA@ (0000)
0025B554 0004:
	??_C@_03JHHHHEKD@ctf?$AA@ (0000)
0025B558 0008:
	??_C@_07JHGHBFJP@oddball?$AA@ (0000)
0025B560 000d:
	??_C@_0N@JBNPINGA@accumulation?$AA@ (0000)
0025B570 000d:
	??_C@_0N@CPCFMJOB@team_oddball?$AA@ (0000)
0025B580 0008:
	??_C@_07IAHNGGND@stalker?$AA@ (0000)
0025B588 000c:
	??_C@_0M@MLMHALFL@elimination?$AA@ (0000)
0025B594 000c:
	??_C@_0M@NOCMPHHF@team_slayer?$AA@ (0000)
0025B5A0 0007:
	??_C@_06CBFFIGEC@slayer?$AA@ (0000)
0025B5A8 0006:
	??_C@_05GCEFBECL@rally?$AA@ (0000)
0025B5B0 000a:
	??_C@_09KMEIIIPA@team_race?$AA@ (0000)
0025B5BC 0005:
	??_C@_04GLEOMBLA@race?$AA@ (0000)
0025B5C4 0004:
	__real@3f0ccccd (0000)
0025B5C8 003b:
	??_C@_0DL@MBPNEOKF@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B604 003b:
	??_C@_0DL@FDFHGCGI@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B640 003e:
	??_C@_0DO@GDPJBCBM@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B680 003d:
	??_C@_0DN@GLOHFEFE@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B6C0 003a:
	??_C@_0DK@COLEDOMN@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B700 0047:
	??_C@_0EH@GJGBEMDH@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B748 0047:
	??_C@_0EH@BMGHBGKH@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B790 004a:
	??_C@_0EK@DHDLIGMN@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B7E0 0049:
	??_C@_0EJ@FACIONE@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B830 004f:
	??_C@_0EP@CGHPHLAM@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B880 004f:
	??_C@_0EP@MJLNBADC@NETGAME?5MAP?5FAILURE?3?5failed?5to?5f@ (0000)
0025B8D0 0039:
	??_C@_0DJ@MPJKFIIJ@NETGAME?5MAP?5FAILURE?3?5duplicate?5r@ (0000)
0025B90C 0031:
	??_C@_0DB@KDFIELID@NETGAME?5MAP?5FAILURE?3?5missing?5rac@ (0000)
0025B940 0034:
	??_C@_0DE@KIBANADL@NETGAME?5MAP?5FAILURE?3?5missing?5odd@ (0000)
0025B974 0031:
	??_C@_0DB@IKODHNKI@NETGAME?5MAP?5FAILURE?3?5missing?5hil@ (0000)
0025B9A8 0035:
	??_C@_0DF@EHBELDKM@NETGAME?5MAP?5FAILURE?3?5ctf?5flag?5ou@ (0000)
0025B9E0 0032:
	??_C@_0DC@DCAAPLNK@NETGAME?5MAP?5FAILURE?3?5duplicate?5c@ (0000)
0025BA14 0030:
	??_C@_0DA@LKKJMHEH@NETGAME?5MAP?5FAILURE?3?5missing?5ctf@ (0000)
0025BA48 0057:
	??_C@_0FH@ODDMDCIB@failed?5to?5initialize?5custome?5gam@ (0000)
0025BAA0 000d:
	??_C@_0N@MABFIMPF@title_string?$AA@ (0000)
0025BAB0 001e:
	??_C@_1BO@NNCACCCJ@?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?$CF?$AAd?$AA?$AA@ (0000)
0025BAD0 001a:
	??_C@_1BK@JPAKMPFP@?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?$CF?$AAd?$AA?$AA@ (0000)
0025BAEC 0016:
	??_C@_1BG@EEDCCJKF@?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?$CF?$AAd?$AA?$AA@ (0000)
0025BB04 0012:
	??_C@_1BC@DGONEHBJ@?$AA?5?$AA?7?$AA?5?$AA?7?$AA?5?$AA?7?$AA?$CF?$AAs?$AA?$AA@ (0000)
0025BB18 000e:
	??_C@_1O@HAEHHN@?$AA?5?$AA?7?$AA?5?$AA?7?$AA?$CF?$AAs?$AA?$AA@ (0000)
0025BB28 000a:
	??_C@_19GCIMBEIF@?$AA?5?$AA?7?$AA?$CF?$AAs?$AA?$AA@ (0000)
0025BB34 0026:
	??_C@_1CG@MDCDIADO@?$AA?7?$AA?$CF?$AAs?$AA?7?$AA?$CF?$AAs?$AA?7?$AA?$CF?$AAs?$AA?7?$AA?$CF?$AAs?$AA?7?$AA?$CF?$AAs?$AA?7?$AA?$CF?$AAs?$AA?$AA@ (0000)
0025BB5C 0012:
	??_C@_0BC@FNEEDLLE@ambient_computers?$AA@ (0000)
0025BB70 0012:
	??_C@_0BC@GOFPLFEH@ambient_machinery?$AA@ (0000)
0025BB84 000f:
	??_C@_0P@PLBDAHNN@ambient_nature?$AA@ (0000)
0025BB94 0014:
	??_C@_1BE@EFJKLKFF@?$AA?7?$AA?$CF?$AAs?$AA?7?$AA?$CF?$AAs?$AA?7?$AA?$CF?$AAs?$AA?$AA@ (0000)
0025BBA8 001b:
	??_C@_0BL@NPJONDMM@NONE?5?$CB?$DN?5local_player_index?$AA@ (0000)
002DE3E0 0020:
	_debug_player_color (0000)
0043E498 047c:
	_global_goal (0000)
	_global_variant (0400)
	_game_engine (0468)
	_global_autogenerate_list (0474)
	_global_autogenerate_count (0478)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries_windows.h"
#include "errors.h"
#include "aim_assist.h"
#include "game_engine.h"
#include "game_engine_place.h"
#include "game_engine_runtime.h"

#include "bitmaps/bitmap_group.h"
#include "game_globals.h"
#include "interface/interface.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "interface/hud_messaging.h"
#include "interface/terminal.h"
#include "interface/ui_widget.h"
#include "input/input.h"
#include "items/equipment_definitions.h"
#include "items/item_definitions.h"
#include "items/weapon_definitions.h"
#include "items/weapons.h"
#include "main/main.h"
#include "math/integer_math.h"
#include "networking/network_game_globals.h"
#include "networking/network_server_manager.h"
#include "objects.h"
#include "objects/damage_effect_definitions.h"
#include "physics/collision_features.h"
#include "player_control.h"
#include "player_rumble.h"
#include "players.h"
#include "render/render.h"
#include "saved games/player_profile.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "text/draw_string.h"
#include "text/font_group.h"
#include "text/text_group.h"
#include "text/unicode.h"
#include "units/bipeds.h"
#include "units/units.h"

#ifdef HALO_LINUX
/* network_game_globals.c's */
boolean network_game_distributed_client(void);
/* port/linux/game/network_distributed.c's */
void network_distributed_player_killed(long *killing_player_index, long *killing_object_index,
	long dead_player_index, boolean *friendly_fire);
#endif

/* ---------- constants */

enum
{
#ifdef HALO_LINUX
	/* port: the native builds' session limit (halo_port_limits.h) */
	MULTIPLAYER_MAXIMUM_PLAYERS = HALO_PORT_MAXIMUM_NETWORK_PLAYERS,
	/* ui\multiplayer_game_text only has strings for the first 16 places */
	NUMBER_OF_PLACE_STRINGS = 16,
#else
	MULTIPLAYER_MAXIMUM_PLAYERS = 16,
#endif
};

/* ---------- macros */

#define hud_globals_definition_get(index) \
	((struct hud_globals_definition *)tag_get('hudg', (index)))
#define item_collection_definition_get(index) \
	((struct item_collection_definition *)tag_get( \
		ITEM_COLLECTION_DEFINITION_TAG, \
		(index)))

/* ---------- structures */

struct game_engine_goal
{
	real_point3d position;
	boolean in_use;
	byte padD[3];
	long target_object_index;
	short team_index;
	short pad16;
	long player_index;
	short nav_point_index;
	byte pad1E[2];
};

struct game_engine_globals
{
	unsigned long flags;
	long next_team_index;
	real postgame_timer;
	real postgame_progress;
	long postgame_state;
	real hud_message_timers[MAXIMUM_LOCAL_PLAYERS];
};

struct game_engine_stage
{
	char map_name[64];
	struct game_variant variant;
};

struct postgame_statistic_entry
{
	long values[7];
};

struct rasterizer_debug_options
{
	byte unused0[0x32];
	short postgame_player_list_debug;
};

union game_engine_update_iterator
{
	struct data_iterator data;
	struct object_iterator object;
};

typedef char verify_postgame_statistic_entry_size[
	sizeof(struct postgame_statistic_entry) == 0x1C ? 1 : -1];
typedef char verify_hud_globals_single_player_font_index_offset[
	offsetof(
		struct hud_globals_definition,
		messaging.single_player_font.index) == 0x54 ? 1 : -1];
typedef char verify_hud_globals_multi_player_font_index_offset[
	offsetof(
		struct hud_globals_definition,
		messaging.multi_player_font.index) == 0x64 ? 1 : -1];
typedef char verify_hud_globals_carnage_report_bitmap_index_offset[
	offsetof(
		struct hud_globals_definition,
		carnage_report_bitmap.index) == 0x3D4 ? 1 : -1];
typedef char verify_game_engine_goal_size[sizeof(struct game_engine_goal) == 0x20 ? 1 : -1];
typedef char verify_game_engine_globals_postgame_timer_offset[
	offsetof(struct game_engine_globals, postgame_timer) == 0x8 ? 1 : -1];
typedef char verify_game_engine_globals_next_team_index_offset[
	offsetof(struct game_engine_globals, next_team_index) == 0x4 ? 1 : -1];
typedef char verify_game_engine_globals_postgame_progress_offset[
	offsetof(struct game_engine_globals, postgame_progress) == 0xC ? 1 : -1];
typedef char verify_game_engine_globals_postgame_state_offset[
	offsetof(struct game_engine_globals, postgame_state) == 0x10 ? 1 : -1];
typedef char verify_game_engine_globals_hud_message_timers_offset[
	offsetof(struct game_engine_globals, hud_message_timers) == 0x14 ? 1 : -1];
typedef char verify_game_engine_globals_size[
	sizeof(struct game_engine_globals) == 0x24 ? 1 : -1];
typedef char verify_game_engine_stage_variant_offset[
	offsetof(struct game_engine_stage, variant) == 0x40 ? 1 : -1];
typedef char verify_game_engine_stage_size[
	sizeof(struct game_engine_stage) == 0xA8 ? 1 : -1];
typedef char verify_scenario_netgame_flag_size[
	sizeof(struct scenario_netgame_flag) == 0x94 ? 1 : -1];

typedef char verify_scenario_netgame_equipment_size[
	sizeof(struct scenario_netgame_equipment) == 0x90 ? 1 : -1];

typedef char verify_scenario_starting_equipment_size[
	sizeof(struct scenario_starting_equipment) == 0xCC ? 1 : -1];
typedef char verify_item_permutation_definition_size[
	sizeof(struct item_permutation_definition) == 0x54 ? 1 : -1];

/* ---------- prototypes */

long hud_get_nav_point_render_type(
	long local_player_index,
	real_point3d const *position,
	struct game_engine_goal const *goal,
	long object_index);

void custom_render_nav_point(
	long local_player_index,
	struct game_engine_goal const *goal,
	word nav_point_index,
	long render_type);

void game_engine_playlist_next(
	long parameter0,
	long parameter1,
	long playlist_type);

static void game_engine_build_lighting(
	void);

static boolean is_place_tied(
	struct postgame_statistic_entry const *entry);

int __cdecl sort_statistic_buffer(
	void const *entry0_pointer,
	void const *entry1_pointer);

int __cdecl sort_statistic_buffer_ranking(
	void const *entry0_pointer,
	void const *entry1_pointer);

static void drawline(
	wchar_t const *string,
	long row_index,
	short justification);

static boolean game_engine_infinite_grenades_internal(
	void);

static long select_players_to_display(
	long statistic,
	long player_index,
	struct postgame_statistic_entry *output,
	long maximum_count);

static void netgame_flag_verify_no_team_duplicates(
	short flag_type,
	char const *error_message);

static void netgame_flag_verify_team_range(
	short flag_type,
	short minimum_index,
	short maximum_index,
	char const *error_message);

static void netgame_verify_spawn_points(
	short game_type,
	short unused_team_index,
	short minimum_count,
	char const *error_message);

static void netgame_verify_equipment(
	short game_type,
	char const *error_message);

long game_engine_did_player_win_default(
	long player_index);

void game_show_score_extended(
	long player_index,
	long score,
	long team_index);

boolean multiple_teams_alive(
	void);

struct network_game_server *global_network_game_server_get(
	void);

boolean player_ui_game_variant_specified(
	struct game_variant *variant);

static void game_engine_post_rasterize_in_game(
	void);

static void game_engine_rasterize_in_game_score(
	long player_index,
	real alpha);

static void game_engine_predict_resources(
	void);

static void game_engine_verify_current_map(
	void);

void game_engine_intialize_queued_sounds(
	void);

void game_engine_post_rasterize_post_game(
	void);

void rasterizer_draw_unicode_string(
	rectangle2d const *bounds,
	short parameter1,
	short parameter2,
	short parameter3,
	wchar_t const *string);

struct bitmap_data *bitmap_group_try_and_get_bitmap(
	long bitmap_group_index,
	short bitmap_index);

long hud_get_font_index(
	void);

real_argb_color *hud_get_text_color(
	real_argb_color *color);

static boolean internal_rasterize_score(
	long player_index,
	long parameter1,
	long parameter2,
	wchar_t *message,
	long message_character_count);

static void game_engine_update_player_no_shield(
	long player_index);

static void game_engine_update_teleporter(
	long player_index);

static void game_engine_update_weapons(
	void);

static void game_engine_update_item_spawn(
	void);

void game_engine_update_multiplayer_sound(
	void);

void sound_class_set_gain(
	char const *name,
	real gain,
	short interpolation_ticks);

void network_game_server_switch_to_postgame(
	struct network_game_server *server);

void player_effect_screen_flash(
	long player_index,
	struct screen_flash_definition const *screen_flash,
	real scale);

short unit_get_local_player_index(
	long unit_index);

boolean game_team_is_enemy(
	short team_index0,
	short team_index1);

short player_get_starting_location_count(
	void);

struct player_starting_location *player_get_starting_location(
	short starting_location_index);

boolean team_has_players(
	long team_index);

short find_nav_point(
	char const *name);

static struct postgame_statistic_entry *game_engine_get_player_place(
	struct postgame_statistic_entry *entry,
	long player_index);

long postgame_statistic_get_rating(
	long player_index,
	long parameter1,
	long parameter2);


long populate_statistic_buffer(
	struct postgame_statistic_entry *entries,
	long parameter1,
	long parameter2);

static boolean find_closest_player_callback(
	long object_index,
	long const *excluded_player_index);

static long find_closest_player_index(
	long player_index);

static void internal_rasterize_target_name(
	long player_index);

static long adjust_score_for_ranking(
	long score,
	long player_index);

struct game_variant *build_game_variant_race(
	struct game_variant *variant);
struct game_variant *build_game_variant_team_race(
	struct game_variant *variant);
struct game_variant *build_game_variant_rally(
	struct game_variant *variant);
struct game_variant *build_game_variant_slayer(
	struct game_variant *variant);
struct game_variant *build_game_variant_team_slayer(
	struct game_variant *variant);
struct game_variant *build_game_variant_elimination(
	struct game_variant *variant);
struct game_variant *build_game_variant_stalker(
	struct game_variant *variant);
struct game_variant *build_game_variant_team_oddball(
	struct game_variant *variant);
struct game_variant *build_game_variant_accumulation(
	struct game_variant *variant);
struct game_variant *build_game_variant_oddball(
	struct game_variant *variant);
struct game_variant *build_game_variant_ctf(
	struct game_variant *variant);
struct game_variant *build_game_variant_iron_ctf(
	struct game_variant *variant);
struct game_variant *build_game_variant_king(
	struct game_variant *variant);
struct game_variant *build_game_variant_team_king(
	struct game_variant *variant);

/* ---------- globals */

short debug_player_color = NONE;

#ifdef HALO_LINUX
/* port: slayer's kill-in-order target uses the player's absolute index as
its goal index, so every player needs a goal slot */
struct game_engine_goal global_goal[MAX(32, MULTIPLAYER_MAXIMUM_PLAYERS)] = { 0 };
#else
struct game_engine_goal global_goal[32] = { 0 };
#endif
struct game_variant global_variant = { 0 };
struct game_engine *game_engine = NULL;

extern struct game_engine_globals game_engine_globals;
extern struct game_engine_stage global_stage;
extern struct game_engine *game_engines[];
extern long timeout_for_endgame_sound;
extern struct rasterizer_debug_options rasterizer_debug_options;

/* ---------- public code */

long game_globals_get_weapon(
	struct game_globals *game_globals,
	long weapon_list_index)
{
	struct tag_reference *weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		weapon_list_index,
		struct tag_reference);

	return weapon->index;
}

long game_engine_get_team_score(
	long team_index)
{
	struct data_iterator iterator;
	struct player_datum *player;

	data_iterator_new(&iterator, player_data);
	player = (struct player_datum *)data_iterator_next(&iterator);
	while (player)
	{
		if (player->team_index == team_index)
			return game_engine->get_player_score(iterator.datum_index, TRUE);

		player = (struct player_datum *)data_iterator_next(&iterator);
	}

	return 0;
}

real linear_to_non_linear_alpha(
	real linear_alpha)
{
	real non_linear_alpha = (real)pow((double)linear_alpha, 1.9f);

	return non_linear_alpha;
}

static void initialize_player_multiplayer_data(
	long player_index)
{
	struct player_datum *player = player_get(player_index);

	player->state_message = NONE;
	player->state_message_player_index = NONE;
	player->speed_multiplier = 1.0f;
	player->unknown70 = NONE;
	player->unknown7c = NONE;
	csmemset(
		&player->statistics.multiplayer_statistics,
		0,
		sizeof(long));

	return;
}

#ifdef HALO_LINUX
/* port: English ordinal for a zero-based place past the string list's 16
("17th", "22nd", "111th"; "tied for 17th" when tied), in a static buffer */
static wchar_t *place_ordinal_string(
	long place,
	boolean tied)
{
	static wchar_t string[32];
	long number = place + 1;
	wchar_t const *suffix = L"th";

	if (number % 100 < 11 || number % 100 > 13)
	{
		switch (number % 10)
		{
		case 1:
			suffix = L"st";
			break;
		case 2:
			suffix = L"nd";
			break;
		case 3:
			suffix = L"rd";
			break;
		}
	}

	usnprintf(
		string,
		NUMBEROF(string),
		tied ? L"tied for %d%s" : L"%d%s",
		number,
		suffix);
	string[NUMBEROF(string) - 1] = 0;

	return string;
}
#endif

static wchar_t *get_place_string(
	struct postgame_statistic_entry *entry)
{
	long raw_index = entry->values[6] & 0x7F;
	long string_index;
	long string_list_index;

#ifdef HALO_LINUX
	/* port: places past the 16th are spelled out instead of clamped (the
	caller's format string marks a tie) */
	if (raw_index >= NUMBER_OF_PLACE_STRINGS)
		return place_ordinal_string(raw_index, FALSE);
#endif
	if (raw_index < 0)
		string_index = 0;
	else if (raw_index > 15)
		string_index = 15;
	else
		string_index = raw_index;

	string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
	if (string_list_index != NONE)
		return unicode_string_list_get_string(
			string_list_index,
			string_index + 36);

	return L"";
}

static long game_engine_did_team_win(
	long team_index)
{
	struct data_iterator iterator;
	struct player_datum *player;
	long result = FALSE;

	data_iterator_new(&iterator, player_data);
	player = (struct player_datum *)data_iterator_next(&iterator);
	while (player)
	{
		if (player->team_index == team_index)
		{
			result = FALSE;
			if (game_engine)
			{
				if (game_engine->did_player_win)
					result =
						game_engine->did_player_win(iterator.datum_index);
				else
					result = game_engine_did_player_win_default(
						iterator.datum_index);
			}
			break;
		}

		player = (struct player_datum *)data_iterator_next(&iterator);
	}

	return result;
}



static void game_engine_generate_title_string(
	wchar_t *title_string,
	long player_index)
{
	wchar_t *secondary_string;
	wchar_t life_string[128];
	wchar_t score_string[256];
	long string_list_index;
	wchar_t *format_string;

	player_get(player_index);
	secondary_string = L"";
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x36E,
		title_string);
	if (global_variant.universal_variant.lives > 0)
	{
		struct player_datum *player = player_get(player_index);
		long remaining_lives =
			global_variant.universal_variant.lives - player->statistics.deaths;

		switch (remaining_lives)
		{
		case 0:
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				secondary_string = unicode_string_list_get_string(
					string_list_index,
					0x34);
			}
			else
				secondary_string = L"";
			break;

		case 1:
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				secondary_string = unicode_string_list_get_string(
					string_list_index,
					0x35);
			}
			else
				secondary_string = L"";
			break;

		default:
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				format_string = unicode_string_list_get_string(
					string_list_index,
					0x36);
			}
			else
				format_string = L"";

			usnprintf(
				life_string,
				NUMBEROF(life_string),
				format_string,
				remaining_lives);
			life_string[NUMBEROF(life_string) - 1] = 0;
			secondary_string = life_string;
			break;
		}
	}

	if (game_engine_globals.postgame_state == 1)
	{
		long did_player_win = game_engine_did_player_win(player_index);
		boolean has_teams = FALSE;
		wchar_t *outcome_string;

		if (game_engine)
			has_teams = global_variant.universal_variant.teams;

		switch (did_player_win)
		{
		case NONE:
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				outcome_string = unicode_string_list_get_string(
					string_list_index,
					0x37);
			}
			else
				outcome_string = L"";
			ustrncpy(title_string, outcome_string, 80);
			break;

		case FALSE:
			if (has_teams)
			{
				string_list_index =
					tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					outcome_string = unicode_string_list_get_string(
						string_list_index,
						0x38);
				}
				else
					outcome_string = L"";
				ustrncpy(title_string, outcome_string, 80);
			}
			else
			{
				string_list_index =
					tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					outcome_string = unicode_string_list_get_string(
						string_list_index,
						0x39);
				}
				else
					outcome_string = L"";
				ustrncpy(title_string, outcome_string, 80);
			}
			break;

		case TRUE:
			if (has_teams)
			{
				string_list_index =
					tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					outcome_string = unicode_string_list_get_string(
						string_list_index,
						0x3A);
				}
				else
					outcome_string = L"";
				ustrncpy(title_string, outcome_string, 80);
			}
			else
			{
				string_list_index =
					tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					outcome_string = unicode_string_list_get_string(
						string_list_index,
						0x3B);
				}
				else
					outcome_string = L"";
				ustrncpy(title_string, outcome_string, 80);
			}
			break;
		}
	}
	else if (game_engine && global_variant.universal_variant.teams)
	{
		wchar_t team0_name[8];
		wchar_t team1_name[8];
		long team0_score;
		long team1_score;

		game_engine->format_team_name(
			0,
			team0_name);
		game_engine->format_team_name(
			1,
			team1_name);
		team0_score = game_engine_get_team_score(0);
		team1_score = game_engine_get_team_score(1);

		if (team0_score > team1_score)
		{
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				format_string =
					unicode_string_list_get_string(string_list_index, 0x3C);
			}
			else
				format_string = L"";

			usnprintf(
				title_string,
				80,
				format_string,
				team0_name,
				team1_name,
				secondary_string);
		}
		else if (team0_score < team1_score)
		{
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				format_string =
					unicode_string_list_get_string(string_list_index, 0x3D);
			}
			else
				format_string = L"";

			usnprintf(
				title_string,
				80,
				format_string,
				team1_name,
				team0_name,
				secondary_string);
		}
		else
		{
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				format_string =
					unicode_string_list_get_string(string_list_index, 0x3E);
			}
			else
				format_string = L"";

			usnprintf(
				title_string,
				80,
				format_string,
				team1_name,
				secondary_string);
		}
	}
	else
	{
		struct postgame_statistic_entry entry;

		entry =
			*game_engine_get_player_place(&entry, player_index);
		game_engine->format_player_score(player_index, score_string);

		if (is_place_tied(&entry))
		{
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				format_string = unicode_string_list_get_string(
					string_list_index,
					0x3F);
			}
			else
				format_string = L"";

			usnprintf(
				title_string,
				80,
				format_string,
				get_place_string(&entry),
				score_string,
				secondary_string);
		}
		else
		{
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
			{
				format_string = unicode_string_list_get_string(
					string_list_index,
					0x40);
			}
			else
				format_string = L"";

			usnprintf(
				title_string,
				80,
				format_string,
				get_place_string(&entry),
				score_string,
				secondary_string);
		}
	}

	title_string[79] = 0;

	return;
}

static void rasterize_in_game_score_draw_line(
	wchar_t const *string,
	boolean brighten,
	real_argb_color *color,
	long row_index)
{
	rectangle2d bounds = render.camera.window_bounds;
	short narrow_tab_stops[3];
	short wide_tab_stops[3];
	short *tab_stops;
	boolean splitscreen;
	long font_index;

	splitscreen = local_player_count() > 1;
	font_index = hud_get_font_index();
	narrow_tab_stops[0] = 80;
	narrow_tab_stops[1] = 125;
	narrow_tab_stops[2] = 200;
	wide_tab_stops[0] = 130;
	wide_tab_stops[1] = 195;
	wide_tab_stops[2] = 315;

	if (bounds.x1 - bounds.x0 > 320)
		tab_stops = wide_tab_stops;
	else
		tab_stops = narrow_tab_stops;

	if (row_index)
		draw_string_set_tab_stops(tab_stops, 3);
	else
		draw_string_set_tab_stops(NULL, 0);

	offset_rectangle2d(
		&bounds,
		-render.camera.viewport_bounds.x0,
		-render.camera.viewport_bounds.y0);

	if (font_index != NONE)
	{
		struct font_header *font = font_definition_get(font_index);
		long row_offset = 4 * (splitscreen == FALSE) + 4;
		long line_height = font->leading_height;

		if (!splitscreen)
			line_height += font->descending_height;
		line_height += font->ascending_height;

		if (brighten)
		{
			color->red += 0.4f;
			color->green += 0.4f;
			color->blue += 0.4f;

			if (color->red > 1.0f)
				color->red = 1.0f;
			if (color->green > 1.0f)
				color->green = 1.0f;
			if (color->blue > 1.0f)
				color->blue = 1.0f;
		}

		row_index += row_offset;
		bounds.y0 = (short)(row_index * line_height);
		bounds.y1 = (short)((row_index + 1) * line_height);
		draw_string_set_draw_mode(font_index, NONE, 0, 0, color);
		rasterizer_draw_unicode_string(&bounds, 0, 0, 0, string);
	}

	draw_string_set_tab_stops(NULL, 0);

	return;
}

long populate_statistic_buffer(
	struct postgame_statistic_entry *entries,
	long statistic,
	long invert_parameter)
{
	long player_count;
	struct player_datum *iterator_player;
	struct player_datum *player;
	struct postgame_statistic_entry *entry;
	boolean invert;
	long entry_index;
	long previous_value;
	long entry_value;
	struct data_iterator iterator;

	player_count = 0;
	if (statistic == 4)
		invert = (boolean)invert_parameter == 0;
	else
		invert = (boolean)invert_parameter;

	data_iterator_new(&iterator, player_data);
	iterator_player =
		(struct player_datum *)data_iterator_next(&iterator);
	if (iterator_player)
	{
		entry = entries;
		do
		{
			if (player_count >= MULTIPLAYER_MAXIMUM_PLAYERS)
			{
				match_assert(
					"c:\\halo\\SOURCE\\game\\game_engine.c",
					0x2C8,
					player_count < MULTIPLAYER_MAXIMUM_PLAYERS);
			}
			else
			{
				entry->values[0] = iterator.datum_index;
				player_count++;
				entry++;
			}

			iterator_player =
				(struct player_datum *)data_iterator_next(&iterator);
		} while (iterator_player);
	}

	entry = entries;
	for (entry_index = 0; entry_index < player_count; entry_index++, entry++)
	{
		long player_index = entry->values[0];

		player = player_get(player_index);
		switch (statistic)
		{
		case 0:
			entry->values[2] = 0;
			if (game_engine->get_player_score)
			{
				entry->values[2] = adjust_score_for_ranking(
					game_engine->get_player_score(player_index, FALSE),
					player_index);
			}
			entry->values[3] = player->statistics.kills[0];
			entry->values[5] = player->statistics.assists[0];
			entry->values[4] = player->statistics.deaths;
			break;

		case 1:
			entry->values[1] = 0;
			if (game_engine->get_player_score)
			{
				entry->values[1] = adjust_score_for_ranking(
					game_engine->get_player_score(player_index, FALSE),
					player_index);
			}
			break;

		case 2:
			entry->values[1] = player->statistics.kills[0];
			break;

		case 3:
			entry->values[1] = player->statistics.assists[0];
			break;

		case 4:
			entry->values[1] = player->statistics.deaths;
			break;

		default:
			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine.c",
				0x2FD,
				!"unreachable");
			break;
		}

		if (invert)
			entry->values[1] = -entry->values[1];
	}

	if (statistic == 0)
	{
		qsort(
			entries,
			player_count,
			sizeof(*entries),
			sort_statistic_buffer_ranking);
	}
	else
	{
		qsort(
			entries,
			player_count,
			sizeof(*entries),
			sort_statistic_buffer);
	}

	entry_index = 0;
	if (player_count > 0)
	{
		entry = entries;
		do
		{
			struct postgame_statistic_entry *previous = entry - 1;

			if (entry_index != 0 &&
				!((previous_value = previous->values[2],
						entry_value = entry->values[2],
						entry_value < previous_value ||
						entry_value > previous_value) ||
					(previous_value = previous->values[3],
						entry_value = entry->values[3],
						entry_value < previous_value ||
						entry_value > previous_value) ||
					(previous_value = previous->values[4],
						entry_value = entry->values[4],
						entry_value < previous_value ||
						entry_value > previous_value) ||
					(previous_value = previous->values[5],
						entry_value = entry->values[5],
						entry_value < previous_value ||
						entry_value > previous_value)))
			{
				previous->values[6] |= 0x80000000;
				entry->values[6] = previous->values[6];
			}
			else
			{
				entry->values[6] = entry_index;
			}

			entry_index++;
			entry++;
		} while (entry_index < player_count);
	}

	return player_count;
}






static long select_players_to_display(
	long statistic,
	long player_index,
	struct postgame_statistic_entry *output,
	long maximum_count)
{
	struct postgame_statistic_entry entries[MULTIPLAYER_MAXIMUM_PLAYERS];
	long player_count;
	boolean debug;
	long entry_index;

	player_count = populate_statistic_buffer(entries, statistic, 0);
	debug = rasterizer_debug_options.postgame_player_list_debug == 'E';
	if (debug)
	{
		terminal_printf(
			global_real_argb_white,
			"player_count=%d, maxcount=%d",
			player_count,
			maximum_count);
	}

	for (entry_index = 0; entry_index < player_count; entry_index++)
	{
		player_get(entries[entry_index].values[0]);
	}

	if (player_count > maximum_count)
	{
		struct postgame_statistic_entry local_entries[MAXIMUM_LOCAL_PLAYERS];
		long local_player_count = 0;
		long local_index;

		for (entry_index = maximum_count; entry_index < player_count; entry_index++)
		{
			struct player_datum *player = player_get(entries[entry_index].values[0]);

			if (player && player->local_player_index != NONE)
			{
				if (debug)
				{
					terminal_printf(
						global_real_argb_white,
						"found local player");
				}

				local_entries[local_player_count] = entries[entry_index];
				local_player_count++;
			}
		}

		for (local_index = 0; local_index < local_player_count; local_index++)
		{
			long insertion_index;

			for (insertion_index = maximum_count - 1; insertion_index >= 0; insertion_index--)
			{
				struct player_datum *player = player_get(entries[insertion_index].values[0]);

				if (player->local_player_index == NONE)
				{
					csmemmove(
						&entries[insertion_index],
						&entries[insertion_index + 1],
						maximum_count * sizeof(struct postgame_statistic_entry) -
							(insertion_index + 1) * sizeof(struct postgame_statistic_entry));
					entries[maximum_count - 1] = local_entries[local_index];
					break;
				}
			}
		}
	}

	csmemcpy(
		output,
		entries,
		MIN(maximum_count, player_count) *
			sizeof(struct postgame_statistic_entry));
	return MIN(maximum_count, player_count);
}

static void game_engine_rasterize_in_game_score(
	long player_index,
	real alpha)
{
	wchar_t row_string[256];
	wchar_t score_string[256];
	struct postgame_statistic_entry entries[6];
	wchar_t title_string[80];
	real_argb_color text_color;
	real_argb_color team_colors[2];
	real_argb_color color;
	boolean has_teams = FALSE;
	boolean is_current_player;
	long entry_count;
	long entry_index;
	long *entry_placement;
	long string_list_index;
	wchar_t *column_name;
	wchar_t *score_name;

	if (game_engine)
		has_teams = global_variant.universal_variant.teams;

	game_engine_generate_title_string(title_string, player_index);
	entry_count = select_players_to_display(
		0,
		player_index,
		entries,
		NUMBEROF(entries));

	color.alpha = alpha;
	color.red = 0.7f;
	color.green = 0.7f;
	color.blue = 0.7f;
	rasterize_in_game_score_draw_line(title_string, FALSE, &color, 0);

	color.red = 0.5f;
	color.green = 0.5f;
	color.blue = 0.5f;
	color.alpha = alpha;

	string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
	if (string_list_index != NONE)
		column_name = unicode_string_list_get_string(string_list_index, 0x43);
	else
		column_name = L"";

	string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
	if (string_list_index != NONE)
		score_name = unicode_string_list_get_string(string_list_index, 0x44);
	else
		score_name = L"";

	game_engine->format_score_name(score_string);
	usprintf(row_string, L"\t%s\t%s\t%s", column_name, score_name, score_string);
	rasterize_in_game_score_draw_line(row_string, FALSE, &color, 1);

	entry_index = 0;
	if (entry_count > 0)
	{
		entry_placement = &entries[0].values[6];
		do
		{
			long entry_player_index = entry_placement[-6];
			struct player_datum *player =
				player_try_and_get(entry_player_index);
			wchar_t *status_string;
			wchar_t *place_string;
			long raw_place;
			long place_index;
			real_argb_color *row_color;

			is_current_player = player_index == entry_player_index;
			if (player)
			{
				color = *hud_get_text_color(&text_color);
				player = player_get(entry_player_index);

				team_colors[0].alpha = alpha;
				team_colors[0].red = 0.6f;
				team_colors[0].green = 0.3f;
				team_colors[0].blue = 0.3f;
				team_colors[1].alpha = alpha;
				team_colors[1].red = 0.3f;
				team_colors[1].green = 0.3f;
				team_colors[1].blue = 0.6f;
				color.alpha = alpha;

				game_engine->format_player_score(
					entry_player_index,
					score_string);

				if (game_engine_player_is_out_of_lives(entry_player_index))
				{
					string_list_index =
						tag_loaded('ustr', "ui\\multiplayer_game_text");
					if (string_list_index != NONE)
						status_string =
							unicode_string_list_get_string(string_list_index, 0x8A);
					else
						status_string = L"";
				}
				else if (player->quit_out_of_game)
				{
					string_list_index =
						tag_loaded('ustr', "ui\\multiplayer_game_text");
					if (string_list_index != NONE)
						status_string =
							unicode_string_list_get_string(string_list_index, 0x8B);
					else
						status_string = L"";
				}
				else
					status_string = score_string;

				raw_place = *entry_placement & 0x7F;
				if (raw_place < 0)
					place_index = 0;
				else if (raw_place > 15)
					place_index = 15;
				else
					place_index = raw_place;

				string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
				if (string_list_index != NONE)
				{
					place_string = unicode_string_list_get_string(
						string_list_index,
						place_index + 36);
				}
				else
					place_string = L"";
#ifdef HALO_LINUX
				/* port: places past the 16th are spelled out instead of
				clamped (the rows never marked ties) */
				if (raw_place >= NUMBER_OF_PLACE_STRINGS)
					place_string = place_ordinal_string(raw_place, FALSE);
#endif

				usprintf(
					row_string,
					L"\t%s\t%s\t%s",
					place_string,
					player->name,
					status_string);

				if (has_teams)
					row_color = &team_colors[PIN(player->team_index, 0, 1)];
				else
					row_color = &color;

				rasterize_in_game_score_draw_line(
					row_string,
					is_current_player,
					row_color,
					entry_index + 2);
			}

			entry_index++;
			entry_placement += NUMBEROF(entries[0].values);
		}
		while (entry_index < entry_count);
	}

	return;
}

static void drawline(
	wchar_t const *string,
	long row_index,
	short justification)
{
	rectangle2d bounds = render.camera.window_bounds;
	long line_height = 18;
	long line_spacing = 8;

	offset_rectangle2d(
		&bounds,
		-render.camera.viewport_bounds.x0,
		-render.camera.viewport_bounds.y0);
	bounds.y0 = (short)(row_index * line_height);
	bounds.y1 = (short)((row_index + 1) * line_height + line_spacing);
	draw_string_set_format(NONE, justification, 0);
	rasterizer_draw_unicode_string(&bounds, 0, 0, 0, string);

	return;
}

void game_engine_post_rasterize_post_game(
	void)
{
#ifdef HALO_LINUX
	/* port: sized like every buffer handed to populate_statistic_buffer */
	struct postgame_statistic_entry entries[MULTIPLAYER_MAXIMUM_PLAYERS];
#else
	struct postgame_statistic_entry entries[16];
#endif
	wchar_t score_string[256];
	wchar_t row_string[256];
	short tab_stops[6];
	real_argb_color winner_color;
	real_argb_color normal_color;
	real_argb_color hilite_color;
	long font_index;
	long entry_count;

	if (!game_engine)
		return;

	tab_stops[0] = 50;
	tab_stops[1] = 125;
	tab_stops[2] = 250;
	tab_stops[3] = 350;
	tab_stops[4] = 410;
	tab_stops[5] = 500;

	font_index = hud_globals->messaging.single_player_font.index;
	get_postgame_hilite_colors(
		&winner_color,
		&normal_color,
		&hilite_color);
	draw_string_set_draw_mode(
		font_index,
		NONE,
		2,
		8,
		&winner_color);
	draw_string_set_color(&winner_color);
	draw_string_set_format(NONE, 0, 0);

	{
		struct hud_globals_definition *hud_definition =
			hud_globals_definition_get(
				interface_get_tag_index(_interface_hud_globals));
		rectangle2d bounds;

		bounds.x0 = 0;
		bounds.y0 = 0;
		bounds.x1 = 640;
		bounds.y1 = 480;
		if (bitmap_group_try_and_get_bitmap(hud_definition->carnage_report_bitmap.index, 0))
		{
			draw_bitmap_in_rect(
				bitmap_group_try_and_get_bitmap(hud_definition->carnage_report_bitmap.index, 0),
				&bounds,
				&bounds,
				NULL,
				NONE,
				NULL,
				TRUE);
		}
	}

	if (global_variant.universal_variant.teams)
	{
		short team_tab_stops[6] = { 50, 200, 300, 350, 410, 500 };
		long team_order[2] = { 0, 1 };
		wchar_t const *team_formats[2];
		long string_list_index;
		long team_row;
		long red_team_won;

		red_team_won = game_engine_did_team_win(0);
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		team_formats[0] =
			string_list_index != NONE ?
				unicode_string_list_get_string(string_list_index, 0x41) :
				L"";
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		team_formats[1] =
			string_list_index != NONE ?
				unicode_string_list_get_string(string_list_index, 0x42) :
				L"";

		if (!red_team_won)
		{
			team_order[0] = 1;
			team_order[1] = 0;
		}

		draw_string_set_tab_stops(team_tab_stops, NUMBEROF(team_tab_stops));
		for (team_row = 0; team_row < 2; team_row++)
		{
			long team_index = team_order[team_row];

			game_engine->format_team_name(
				team_index,
				score_string);
			usnprintf(
				row_string,
				NUMBEROF(row_string),
				team_formats[team_index],
				score_string);
			row_string[NUMBEROF(row_string) - 1] = 0;
			drawline(row_string, team_row + 4, 0);
		}
	}

	{
		long string_list_index;
		wchar_t const *column_strings[5];

		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		column_strings[0] =
			string_list_index != NONE ?
				unicode_string_list_get_string(string_list_index, 0x43) :
				L"";
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		column_strings[1] =
			string_list_index != NONE ?
				unicode_string_list_get_string(string_list_index, 0x44) :
				L"";
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		column_strings[2] =
			string_list_index != NONE ?
				unicode_string_list_get_string(string_list_index, 0x45) :
				L"";
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		column_strings[3] =
			string_list_index != NONE ?
				unicode_string_list_get_string(string_list_index, 0x46) :
				L"";
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		column_strings[4] =
			string_list_index != NONE ?
				unicode_string_list_get_string(string_list_index, 0x47) :
				L"";

		game_engine->format_score_name(score_string);
		usnprintf(
			row_string,
			NUMBEROF(row_string),
			L"\t%s\t%s\t%s\t%s\t%s\t%s",
			column_strings[0],
			column_strings[1],
			score_string,
			column_strings[2],
			column_strings[3],
			column_strings[4]);
		row_string[NUMBEROF(row_string) - 1] = 0;
		draw_string_set_tab_stops(tab_stops, NUMBEROF(tab_stops));
		drawline(row_string, 7, 0);
	}

	entry_count = select_players_to_display(0, NONE, entries, 12);
	{
		long entry_index;

		for (entry_index = 0; entry_index < entry_count; entry_index++)
		{
			long player_index = entries[entry_index].values[0];
			long draw_row = entry_index + 8;
			struct player_datum *player = player_get(player_index);
			real_argb_color team_colors[2];
			long place;
			long string_list_index;
			wchar_t const *place_string;

			if (player->local_player_index != NONE)
				draw_string_set_color(&normal_color);
			else
				draw_string_set_color(&winner_color);
			draw_string_set_tab_stops(tab_stops, NUMBEROF(tab_stops));
			place = PIN(entries[entry_index].values[6] & 0x7F, 0, 15);
			string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
			place_string =
				string_list_index != NONE ?
					unicode_string_list_get_string(string_list_index, place + 36) :
					L"";
#ifdef HALO_LINUX
			/* port: places past the 16th are spelled out instead of clamped
			(the rows never marked ties) */
			if ((entries[entry_index].values[6] & 0x7F) >= NUMBER_OF_PLACE_STRINGS)
			{
				place_string = place_ordinal_string(
					entries[entry_index].values[6] & 0x7F,
					FALSE);
			}
#endif
			usnprintf(row_string, NUMBEROF(row_string), L" \t%s", place_string);
			row_string[NUMBEROF(row_string) - 1] = 0;
			drawline(row_string, draw_row, 0);
			draw_string_set_color(&winner_color);

			if (global_variant.universal_variant.teams)
			{
				long team_index = player->team_index;

				team_colors[0].red = 0.8f;
				team_colors[0].green = 0.4f;
				team_colors[0].blue = 0.4f;
				team_colors[0].alpha = 1.0f;
				team_colors[1].red = 0.4f;
				team_colors[1].green = 0.4f;
				team_colors[1].blue = 0.8f;
				team_colors[1].alpha = 1.0f;
				draw_string_set_color(
					&team_colors[PIN(team_index, 0, 1)]);
			}
			usnprintf(row_string, NUMBEROF(row_string), L" \t \t%s", player->name);
			row_string[NUMBEROF(row_string) - 1] = 0;
			drawline(row_string, draw_row, 0);
			draw_string_set_color(&winner_color);

			if (!postgame_statistic_get_rating(player_index, 1, 0))
				draw_string_set_color(&hilite_color);
			game_engine->format_player_score(
				player_index,
				score_string);
			usnprintf(row_string, NUMBEROF(row_string), L" \t \t \t%s", score_string);
			row_string[NUMBEROF(row_string) - 1] = 0;
			drawline(row_string, draw_row, 0);
			draw_string_set_color(&winner_color);

			if (!postgame_statistic_get_rating(player_index, 2, 0))
				draw_string_set_color(&hilite_color);
			usnprintf(
				row_string,
				NUMBEROF(row_string),
				L" \t \t \t \t%d",
				(long)player->statistics.kills[0]);
			row_string[NUMBEROF(row_string) - 1] = 0;
			drawline(row_string, draw_row, 0);
			draw_string_set_color(&winner_color);

			if (!postgame_statistic_get_rating(player_index, 3, 0))
				draw_string_set_color(&hilite_color);
			usnprintf(
				row_string,
				NUMBEROF(row_string),
				L" \t \t \t \t \t%d",
				(long)player->statistics.assists[0]);
			row_string[NUMBEROF(row_string) - 1] = 0;
			drawline(row_string, draw_row, 0);
			draw_string_set_color(&winner_color);

			if (!postgame_statistic_get_rating(player_index, 4, 0))
				draw_string_set_color(&hilite_color);
			usnprintf(
				row_string,
				NUMBEROF(row_string),
				L" \t \t \t \t \t \t%d",
				(long)player->statistics.deaths);
			row_string[NUMBEROF(row_string) - 1] = 0;
			drawline(row_string, draw_row, 0);
			draw_string_set_tab_stops(tab_stops, NUMBEROF(tab_stops));
		}
	}

	{
		real_argb_color prompt_color;
		long string_list_index;
		wchar_t const *prompt;
		struct network_game_server *server;
		rectangle2d bounds;

		bounds = render.camera.window_bounds;
		prompt_color = winner_color;
		prompt_color.alpha = game_engine_globals.postgame_progress;
		bounds.y0 = 410;
		bounds.x0 = 70;
		offset_rectangle2d(
			&bounds,
			-render.camera.viewport_bounds.x0,
			-render.camera.viewport_bounds.y0);
		draw_string_set_tab_stops(NULL, 0);
		draw_string_set_color(&prompt_color);

		server = global_network_game_server_get();
		if (server)
		{
			bounds.x0 = 380;
			string_list_index =
				tag_loaded('ustr', "ui\\multiplayer_game_text");
			if (string_list_index != NONE)
				prompt =
					unicode_string_list_get_string(string_list_index, 0x48);
			else
				prompt = L"";
			draw_string_and_hack_in_icons(
				&bounds,
				NULL,
				NULL,
				0,
				prompt,
				FALSE);
			return;
		}

		bounds.x0 = 520;
		string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
		if (string_list_index != NONE)
			prompt = unicode_string_list_get_string(string_list_index, 0x49);
		else
			prompt = L"";
		draw_string_and_hack_in_icons(
			&bounds,
			NULL,
			NULL,
			0,
			prompt,
			FALSE);
	}

	return;
}

static long find_closest_player_index(
	long player_index)
{
	struct player_datum *player = player_get(player_index);
	long best_object_index = NONE;

	if (player->local_player_index != NONE &&
		player_control_get_autoaim_level(player->local_player_index) > 0.0f)
	{
		best_object_index =
			player_control_get_target_object_index(player->local_player_index);
	}

	if (best_object_index == NONE)
	{
		long object_indices[32];
		real_point3d camera_position;
		real_vector3d facing_direction;
		long object_count;
		long object_index;

		unit_get_camera_position(player->unit_index, &camera_position);
		player_control_get_facing_direction(
			player->local_player_index,
			&facing_direction);
		object_count = find_objects_from_point_vector(
			&camera_position,
			&facing_direction,
			(boolean (*)(long, void *))find_closest_player_callback,
			&player_index,
			NUMBEROF(object_indices),
			object_indices);

		for (object_index = 0; object_index < object_count; object_index++)
		{
			long candidate_object_index = object_indices[object_index];
			struct unit_datum *candidate = unit_get(candidate_object_index);
			real x = candidate->object.position.x - camera_position.x;
			real y = candidate->object.position.y - camera_position.y;
			real z = candidate->object.position.z - camera_position.z;
			real distance_squared = y * y + (x * x + z * z);
			real_point3d target_position;
			real_vector3d target_direction;
			real target_distance;
			real target_angle;

#ifdef HALO_N64
			/* (the cheap tests first, the ray last: a ray to every unit
			along the view, for each view's HUD, was most of its rays) */
			extern boolean n64_autoaim_compute_target_within(long object_index, real_point3d const *position,
				real_vector3d const *direction, long unit_index, real maximum_angle, real_point3d *target_position,
				real_vector3d *target_direction, real *target_distance, real *target_angle);

			if (distance_squared < 400.0f &&
				(candidate->unit.active_camouflage < 1.0f ||
				player->unknown7c == player_index_from_unit_index(candidate_object_index)) &&
				n64_autoaim_compute_target_within(
					object_indices[object_index],
					&camera_position,
					&facing_direction,
					player->unit_index,
					0.13083334267139435f,
					&target_position,
					&target_direction,
					&target_distance,
					&target_angle))
			{
				best_object_index = object_indices[object_index];
			}
#else
			if ((candidate->unit.active_camouflage < 1.0f ||
				player->unknown7c == player_index_from_unit_index(candidate_object_index)) &&
				autoaim_compute_target(
					object_indices[object_index],
					&camera_position,
					&facing_direction,
					player->unit_index,
					&target_position,
					&target_direction,
					&target_distance,
					&target_angle) &&
				fabs(target_angle) < 0.13083334267139435 &&
				distance_squared < 400.0f &&
				distance_squared < 900.0f)
			{
				best_object_index = object_indices[object_index];
			}
#endif
		}
	}

	if (best_object_index != NONE)
		best_object_index = player_index_from_unit_index(best_object_index);

	return best_object_index;
}

long game_engine_remap_equipment(
	long equipment_definition_index)
{
	struct equipment_definition *equipment;
	long weapon_list_index;
	boolean remap_equipment;

	if (equipment_definition_index == NONE)
		equipment = NULL;
	else
		equipment = equipment_definition_get(equipment_definition_index);

	weapon_list_index =
		weapon_definition_index_to_list_index(equipment_definition_index);
	remap_equipment = weapon_list_index == 12 || weapon_list_index == 13;
	if (!remap_equipment)
	{
		if (equipment)
		{
			if (equipment->equipment.powerup_type == 2)
			{
				if (TEST_FLAG(global_variant.universal_variant.flags, 3))
					return NONE;
			}
			else if (equipment->equipment.powerup_type == 3)
			{
				if (TEST_FLAG(global_variant.universal_variant.flags, 4))
					return NONE;
			}
		}
	}
	else
	{
		switch (global_variant.universal_variant.weapon_set)
		{
		case 3:
			weapon_list_index = 13;
			break;

		case 9:
			weapon_list_index = 12;
			break;

		case 10:
			weapon_list_index = NONE;
			break;
		}

		if (!TEST_FLAG(game_engine_globals.flags, 2))
		{
			if (game_engine_infinite_grenades_internal())
			{
				weapon_list_index = NONE;
			}
		}

		if (TEST_FLAG(game_engine_globals.flags, 3))
		{
			if (real_seed_random(get_global_random_seed_address()) > 0.3f)
				weapon_list_index = NONE;
		}
		else if (TEST_FLAG(game_engine_globals.flags, 2))
		{
			if (real_seed_random(get_global_random_seed_address()) > 0.55f)
				weapon_list_index = NONE;
		}

	}

	if (remap_equipment)
		return list_index_to_weapon_definition_index(weapon_list_index);

	return equipment_definition_index;
}

int __cdecl sort_statistic_buffer(
	void const *entry0_pointer,
	void const *entry1_pointer)
{
	struct postgame_statistic_entry const *entry0 = entry0_pointer;
	struct postgame_statistic_entry const *entry1 = entry1_pointer;
	long result = 0;
	long entry0_value;
	long entry1_value;

	entry0_value = entry0->values[1];
	entry1_value = entry1->values[1];
	if (entry1_value < entry0_value)
		return -1;
	if (entry1_value > entry0_value)
		result = 1;

	return result;
}

int __cdecl sort_statistic_buffer_ranking(
	void const *entry0_pointer,
	void const *entry1_pointer)
{
	struct postgame_statistic_entry const *entry0 = entry0_pointer;
	struct postgame_statistic_entry const *entry1 = entry1_pointer;
	long result = 0;
	long entry0_value;
	long entry1_value;

	entry0_value = entry0->values[2];
	entry1_value = entry1->values[2];
	if (entry1_value < entry0_value)
		result = -1;
	else if (entry1_value > entry0_value)
		result = 1;
	else
	{
		entry0_value = entry0->values[3];
		entry1_value = entry1->values[3];
		if (entry1_value < entry0_value)
			result = -1;
		else if (entry1_value > entry0_value)
			result = 1;
		else
		{
			entry0_value = entry0->values[4];
			entry1_value = entry1->values[4];
			if (entry1_value < entry0_value)
				result = 1;
			else if (entry1_value > entry0_value)
				result = -1;
			else
			{
				entry0_value = entry0->values[5];
				entry1_value = entry1->values[5];
				if (entry1_value < entry0_value)
					result = -1;
				else if (entry1_value > entry0_value)
					result = 1;
			}
		}
	}

	return result;
}

static boolean is_place_tied(
	struct postgame_statistic_entry const *entry)
{
	boolean result =
		(entry->values[6] & (unsigned long)0x80000000) > 0;

	return result;
}

static long place_get_position(
	struct postgame_statistic_entry const *entry)
{
	return entry->values[6] & ~FLAG(31);
}

static void get_selected_color(
	real_argb_color *color)
{
	color->red = 0.98f;
	color->green = 0.96f;
	color->blue = 0.96f;
	color->alpha = 1.0f;

	return;
}

static boolean can_delete_item(
	long weapon_index)
{
	boolean result = TRUE;

	if (weapon_try_and_get(weapon_index) &&
		weapon_is_flag(weapon_index))
	{
		result = FALSE;
	}

	return result;
}

static boolean multiplayer_message_internal(
	long player_index,
	long parameter1,
	long parameter2,
	wchar_t *message,
	long message_character_count)
{
	boolean result = FALSE;

	if (game_engine->format_message)
	{
		result = game_engine->format_message(
			player_index,
			parameter1,
			parameter2,
			message,
			message_character_count);
	}

	if (!result)
	{
		result = internal_rasterize_score(
			player_index,
			parameter1,
			parameter2,
			message,
			message_character_count);
	}

	return result;
}


static void multiplayer_message(
	long player_index,
	long parameter1,
	long parameter2)
{
	struct player_datum *player = player_get(player_index);

	if (player->local_player_index != NONE)
	{
		wchar_t message[1024];

		if (multiplayer_message_internal(
			player_index,
			parameter1,
			parameter2,
			message,
			NUMBEROF(message)))
		{
			message[NUMBEROF(message) - 1] = 0;
			hud_print_message(player->local_player_index, message);
		}
	}

	return;
}

static void game_show_score_one_player(
	long player_index,
	long parameter1,
	long parameter2)
{
	if (parameter1 != NONE)
		multiplayer_message(player_index, parameter1, parameter2);

	return;
}

void game_show_score_team(
	long team_index,
	long score)
{
	struct data_iterator iterator;
	struct player_datum *player;

	data_iterator_new(&iterator, player_data);
	player = (struct player_datum *)data_iterator_next(&iterator);
	while (player)
	{
		if (player->team_index == team_index)
		{
			game_show_score_one_player(iterator.datum_index, score, NONE);
		}

		player = (struct player_datum *)data_iterator_next(&iterator);
	}

	return;
}

void game_show_score_you_ally_enemy(
	long player_index,
	long you_score,
	long ally_score,
	long enemy_score,
	long other_player_index)
{
	struct player_datum *local_player;
	struct data_iterator iterator;
	struct player_datum *player;
	long score;

	local_player = player_get(player_index);

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0xB1A,
		NONE != player_index);

	data_iterator_new(&iterator, player_data);
	player = (struct player_datum *)data_iterator_next(&iterator);
	while (player)
	{
		if (iterator.datum_index == player_index)
		{
			if (you_score != NONE)
			{
				game_show_score_one_player(
					iterator.datum_index,
					you_score,
					other_player_index);
			}
		}
		else
		{
			if (game_team_is_enemy(
				(short)local_player->team_index,
				(short)player->team_index))
			{
				score = enemy_score;
			}
			else
				score = ally_score;

			if (score != NONE)
			{
				game_show_score_one_player(
					iterator.datum_index,
					score,
					other_player_index);
			}
		}

		player = (struct player_datum *)data_iterator_next(&iterator);
	}

	return;
}

void game_show_score_extended(
	long player_index,
	long score,
	long team_index)
{
	if (player_index != NONE)
	{
		game_show_score_one_player(player_index, score, team_index);
	}
	else
	{
		struct data_iterator iterator;

		data_iterator_new(&iterator, player_data);
		while (data_iterator_next(&iterator))
		{
			game_show_score_one_player(
				iterator.datum_index,
				score,
				team_index);
		}
	}

	return;
}

long players_in_game(
	void)
{
	struct data_iterator iterator;
	long player_count = 0;

	data_iterator_new(&iterator, player_data);
	while (data_iterator_next(&iterator))
		player_count++;

	return player_count;
}

static void game_engine_press_start_to_begin(
	void)
{
	return;
}

static boolean goal_matches_player(
	long goal_index,
	struct player_datum *player,
	long player_index)
{
	boolean result = FALSE;

	if (game_engine)
	{
		struct game_engine_goal *goal = &global_goal[goal_index];

		if (game_engine->player_can_see_goal)
		{
			if (goal->in_use)
				result = game_engine->player_can_see_goal(player_index, goal_index);
		}
		else if (
			goal->in_use &&
			(goal->target_object_index == NONE || player_index == goal->target_object_index) &&
			(goal->team_index == NONE || player->team_index == goal->team_index) &&
			(goal->player_index == NONE || player_index != goal->player_index))
		{
			result = TRUE;
		}
	}

	return result;
}

static boolean nearby_vehicle(
	long player_index,
	struct player_starting_location const *starting_location)
{
	struct location location;
	long object_indices[16];
	short object_count;
	short object_index;

	player_get(player_index);
	scenario_location_from_point(&location, &starting_location->position);
	object_count = objects_in_sphere(
		0,
		0x11F,
		&location,
		&starting_location->position,
		0.1f,
		object_indices,
		NUMBEROF(object_indices));

	for (object_index = 0; object_index < object_count; object_index++)
	{
		struct object_datum *object = object_get(object_indices[object_index]);

		match_assert(
			"c:\\halo\\SOURCE\\game\\game_engine.c",
			0xEE9,
			object);

		if (object->object.type == _object_type_vehicle)
		{
			struct vehicle_datum *vehicle =
				(struct vehicle_datum *)object_get_and_verify_type(
					object_indices[object_index],
					_object_mask_vehicle);

			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine.c",
				0xEEE,
				vehicle);

			if (vehicle)
				return TRUE;

			return FALSE;
		}
	}

	return FALSE;
}

static real game_engine_get_friendly_bonus(
	long player_index,
	real_point3d const *position)
{
	struct player_datum *player = player_get(player_index);
	struct data_iterator iterator;
	struct player_datum *other_player;
	real rating = 0.0f;

	data_iterator_new(&iterator, player_data);
	other_player = (struct player_datum *)data_iterator_next(&iterator);
	while (other_player)
	{
		if (player->team_index == other_player->team_index &&
			other_player->unit_index != NONE)
		{
			real_point3d origin;
			real distance;

			object_get_origin(other_player->unit_index, &origin);
			distance = distance3d(&origin, position);
			if (distance >= 1.0f && distance <= 6.0f)
			{
				rating += (real)pow(
					(double)(1.0f - (distance - 1.0f) * 0.2f),
					(double)0.6f);
			}
		}

		other_player = (struct player_datum *)data_iterator_next(&iterator);
	}

	if (rating > 3.0f)
		rating = 3.0f;

	return rating * 3.0f + 1.0f;
}

/*
 * NonMatching: this source is retained with its caller because VC7 otherwise
 * removes the two exact private helpers above.  The remaining binary delta is
 * an EBX/EDI allocation mirror; relocation count, order, and destinations
 * already agree with the January object.
 */

static real default_starting_location_rate_function(
	long player_index,
	struct player_starting_location const *starting_location)
{
	struct player_datum *player = player_get(player_index);
	real rating = 1.0f;

	if (game_engine_running() &&
		game_engine_test_flag(0) &&
		player->team_index != starting_location->team_index)
	{
		rating = 0.0f;
	}

	if (rating > 0.0f)
	{
		rating *= game_engine_get_distance_rating_for_spawn(
			player_index,
			&starting_location->position);
	}

	if (game_engine_running() && rating > 0.0f && game_engine_has_teams())
	{
		rating *= game_engine_get_friendly_bonus(
			player_index,
			&starting_location->position);
	}

	if (game_engine_running() && game_engine->starting_location_rating)
	{
		rating *= game_engine->starting_location_rating(
			player_index,
			starting_location);
	}

	return rating;
}

static boolean test_any_gamepad_button(
	long button_index)
{
	boolean result = FALSE;
	long gamepad_index;

	for (gamepad_index = 0; gamepad_index < 4; gamepad_index++)
	{
		struct gamepad_state const *gamepad_state =
			input_get_gamepad_state((short)gamepad_index);

		if (gamepad_state && gamepad_state->buttons[button_index])
		{
			result = TRUE;
			break;
		}
	}

	return result;
}

boolean match_game_type(
	long game_type,
	long count,
	short const *game_types)
{
	boolean result;
	long index;

	if (game_engine)
	{
		result = FALSE;
		for (index = 0; index < count; index++)
		{
			short entry = game_types[index];

			result = result | (entry == game_type);
			if (entry == 12)
			{
				result = result | TRUE;
			}
			else if (entry == 13)
			{
				result = result | (game_type != 1);
			}
			else if (entry == 14)
			{
				result = result | (game_type != 1 && game_type != 5);
			}
		}
	}
	else
	{
		result = TRUE;
		for (index = 0; index < count; index++)
			result = result & (game_types[index] == 0);
	}

	return result;
}

static boolean game_engine_infinite_grenades_internal(
	void)
{
	boolean infinite_grenades = FALSE;

	if (!TEST_FLAG(game_engine_globals.flags, _game_engine_disable_infinite_grenades_bit))
		infinite_grenades = TEST_FLAG(global_variant.universal_variant.flags, _game_variant_infinite_grenades_bit);

	return infinite_grenades;
}

static boolean find_closest_player_callback(
	long object_index,
	long const *excluded_player_index)
{
	long excluded_player = *excluded_player_index;
	boolean result = FALSE;
	struct object_datum *object = object_get(object_index);

	if (!TEST_FLAG(object->object.flags, _object_invisible_bit) &&
		TEST_FLAG(_object_mask_biped, object->object.type) &&
		!TEST_FLAG(object->object.damage_flags, _object_dead_bit) &&
		player_index_from_unit_index(object_index) != excluded_player)
	{
		result = TRUE;
	}

	return result;
}

static void game_engine_update_purge(
	void)
{
	struct object_iterator iterator;
	long cutoff_time = game_time_get() - 900;

	object_iterator_new(&iterator, _object_mask_item, 0);
	/* (a client of the distributed netcode removes items when the host does,
	port/linux/game/network_distributed.c) */
	while (
#ifdef HALO_LINUX
		!network_game_distributed_client() &&
#endif
		object_iterator_next(&iterator))
	{
		struct item_datum *item = item_get(iterator.index);

#ifdef HALO_N64
		/* Release expired items before this tick's scheduled replacements. */
		if (item->item.last_owned_time <= cutoff_time &&
#else
		if (item->item.last_owned_time < cutoff_time &&
#endif
			!TEST_FLAG(item->item.flags, _item_attached_to_unit_bit))
		{
			long item_index = iterator.index;
			if (can_delete_item(item_index))
#ifdef HALO_N64
				object_delete_immediately(iterator.index);
#else
				object_delete(iterator.index);
#endif
		}
	}

	object_iterator_new(&iterator, _object_mask_biped, 0);
	while (object_iterator_next(&iterator))
	{
		struct object_datum *object;

		cutoff_time = 900;
		object = object_get(iterator.index);

		if (object->object.idle_ticks > (short)cutoff_time &&
			TEST_FLAG(object->object.damage_flags, _object_dead_bit))
		{
			object_delete(iterator.index);
		}
	}

	return;
}

void game_engine_flag_reset(
	long weapon_index,
	real_point3d const *position)
{
	if (weapon_index != NONE)
	{
		struct weapon_datum *weapon = weapon_get(weapon_index);

		object_set_position(weapon_index, position, global_forward3d, global_up3d);
		object_reset(weapon_index);
		SET_FLAG(weapon->weapon.flags, _weapon_runtime_game_engine_active_bit, FALSE);
		weapon->item.last_owned_time = game_time_get();
		weapon->item.ignore_object_index = NONE;
	}

	return;
}

void ticks_to_unicode_time_string(
	long ticks,
	unsigned long character_count,
	wchar_t *string)
{
	long total_seconds;
	long minutes;
	long seconds;
	wchar_t minute_string[64];
	wchar_t second_string[64];

	total_seconds = ticks / 30;
	minutes = total_seconds / 60;
	seconds = total_seconds - minutes * 60;

	if (minutes == 0)
		usnprintf(minute_string, NUMBEROF(minute_string), L" ");
	else
		usnprintf(minute_string, NUMBEROF(minute_string), L"%d", minutes);

	if (seconds <= 9)
		usnprintf(second_string, NUMBEROF(second_string), L"0%d", seconds);
	else
		usnprintf(second_string, NUMBEROF(second_string), L"%d", seconds);

	usnprintf(string, character_count, L"%s:%s", minute_string, second_string);

	return;
}

void game_engine_playlist_initialize(
	void)
{
	game_engine_playlist_next(0, 0, 2);

	return;
}

void game_engine_playlist_begin(
	void)
{
	main_set_multiplayer_map_name(global_stage.map_name);
	game_set_game_variant(&global_stage.variant);

	if (!network_game_is_active())
		main_reset_map();

	return;
}

boolean game_engine_get_current_stage(
	struct game_variant *variant,
	char *map_name)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_engine.c", 0x918, variant && map_name);

	csmemcpy(variant, &global_stage.variant, sizeof(*variant));
	csstrncpy(map_name, global_stage.map_name, sizeof(global_stage.map_name)-1);
	map_name[sizeof(global_stage.map_name)-1] = 0;

	return TRUE;
}

long list_index_to_weapon_definition_index(
	long weapon_list_index)
{
	long weapon_definition_index = NONE;

	if (weapon_list_index!=NONE)
	{
		struct game_globals *game_globals = scenario_get_game_globals();
		struct tag_reference *weapon = TAG_BLOCK_GET_ELEMENT(
			&game_globals->weapon_list,
			weapon_list_index,
			struct tag_reference);

		weapon_definition_index = weapon->index;
	}

	return weapon_definition_index;
}

long weapon_definition_index_to_list_index(
	long weapon_definition_index)
{
	struct game_globals *game_globals = scenario_get_game_globals();
	struct tag_reference *weapon;
	long result;
	long weapon_list_index;

	if (game_globals->weapon_list.count)
		weapon = TAG_BLOCK_GET_ELEMENT(
			&game_globals->weapon_list,
			0,
			struct tag_reference);
	else
		weapon = NULL;

	result = NONE;
	for (weapon_list_index = 0;
		weapon_list_index < game_globals->weapon_list.count;
		weapon_list_index++, weapon++)
	{
		if (weapon_definition_index == weapon->index)
		{
			result = weapon_list_index;
			break;
		}
	}

	return result;
}

void game_engine_state_message(
	long player_index,
	long state_message,
	long state_message_player_index)
{
	struct player_datum *player = player_get(player_index);

	player->state_message = state_message;
	player->state_message_player_index = state_message_player_index;

	return;
}

void game_engine_player_depower_active_camo(
	long player_index)
{
	if (player_index!=NONE)
	{
		struct player_datum *player = player_get(player_index);

		if (player->unit_index!=NONE)
		{
			struct unit_datum *unit = unit_get(player->unit_index);

			if (TEST_FLAG(unit->unit.flags, _unit_active_camouflaged_bit))
				unit->unit.active_camouflage = 0.5f;
		}
	}

	return;
}

float get_blink_alpha(
	void)
{
	long phase = system_milliseconds()%2700;

	return sin(phase * (3.14159265358979/2700.0));
}

void game_engine_rasterize_message(
	wchar_t const *message,
	real alpha)
{
	rectangle2d bounds;
	real_argb_color color;
	long font_index;
	long terminal_font_index;

	if (local_player_count())
		font_index = hud_globals->messaging.multi_player_font.index;
	else
		font_index = hud_globals->messaging.single_player_font.index;

	bounds = render.camera.window_bounds;

	terminal_font_index = interface_get_tag_index(_interface_font_terminal);
	draw_string_set_draw_mode(
		terminal_font_index,
		NONE,
		0,
		0,
		global_real_argb_white);

	color.alpha = alpha;
	color.red = 0.45882353f;
	color.green = 0.7294118f;
	color.blue = 1.0f;

	offset_rectangle2d(
		&bounds,
		-render.camera.viewport_bounds.x0,
		-render.camera.viewport_bounds.y0);

	bounds.y1 = (short)((5 * bounds.y0 + bounds.y1) / 6 + 9);
	bounds.y0 = bounds.y1 - 15;

	draw_string_set_draw_mode(font_index, NONE, 2, 8, &color);
	draw_string_set_color(&color);
	rasterizer_draw_unicode_string(&bounds, 0, 0, 0, message);
	draw_string_set_format(NONE, 0, 0);
	draw_string_set_tab_stops(NULL, 0);

	return;
}

static void game_engine_post_rasterize_in_game(
	void)
{
	long local_player_index;
	long player_index;
	struct player_datum *player;
	struct gamepad_state const *gamepad;
	real fade;

	local_player_index = render.local_player_index;
	player_index = local_player_get_player_index(local_player_index);
	player = player_get(player_index);

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x770,
		NONE != local_player_index);
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x771,
		NULL != game_engine);

	if (game_engine && player)
		internal_rasterize_target_name(player_index);

	gamepad = input_get_gamepad_state(local_player_index);
	fade = game_engine_globals.hud_message_timers[local_player_index];
	if ((!gamepad ||
		!gamepad->buttons[_gamepad_binary_button_back]) &&
		game_engine_globals.postgame_state != 1)
	{
#ifdef HALO_LINUX
		/* a frame is no longer a tick (render_interpolation.c): fade in half
		a second, not in 15 frames */
		fade -= 0.06666667f * main_get_seconds_elapsed() * TICKS_PER_SECOND;
#else
		fade -= 0.06666667f;
#endif
	}
	else
	{
#ifdef HALO_LINUX
		fade += 0.06666667f * main_get_seconds_elapsed() * TICKS_PER_SECOND;
#else
		fade += 0.06666667f;
#endif
	}

	if (fade < 0.0f)
	{
		fade = 0.0f;
		goto result;
	}

	if (fade > 1.0f)
		fade = 1.0f;
	if (fade > 0.0f)
	{
		real alpha = (real)pow((double)fade, 1.9f);
		game_engine_rasterize_in_game_score(player_index, alpha);
	}

result:
	game_engine_globals.hud_message_timers[local_player_index] = fade;

	return;
}

long game_engine_player_get_team_index(
	long player_index)
{
	long team_index = 1;

	match_assert("c:\\halo\\SOURCE\\game\\game_engine.c", 0xC11, game_engine);

	if (!game_engine->team_index_override)
		team_index = player_get(player_index)->local_player_index%2;

	return team_index;
}

void game_engine_update_player_always_invis(
	long player_index)
{
	if (game_engine)
	{
		if ((TEST_FLAG(global_variant.universal_variant.flags, _game_variant_always_invisible_bit) ||
			game_engine->test_trait && game_engine->test_trait(player_index, 1)) &&
			player_get(player_index)->unit_index!=NONE)
		{
			player_handle_powerup_minor(player_index, 0, 15);
		}
	}

	return;
}

boolean game_engine_player_has_flag(
	long player_index)
{
	boolean has_flag = FALSE;

	if (player_index!=NONE)
	{
		struct player_datum *player = player_get(player_index);

		if (player->unit_index!=NONE)
		{
			struct unit_datum *unit = unit_get(player->unit_index);
			long weapon_index;

			for (weapon_index = 0; weapon_index<MAXIMUM_WEAPONS_PER_UNIT; weapon_index++)
			{
				long weapon_object_index = unit->unit.weapon_object_indices[weapon_index];

				if (weapon_object_index!=NONE && weapon_is_flag(weapon_object_index))
				{
					has_flag = TRUE;
					break;
				}
			}
		}
	}

	return has_flag;
}

void game_show_score(
	long player_index,
	long score)
{
	game_show_score_extended(player_index, score, NONE);

	return;
}

void get_postgame_hilite_colors(
	real_argb_color *winner_color,
	real_argb_color *normal_color,
	real_argb_color *hilite_color)
{
	winner_color->red = 0.45882353f;
	winner_color->green = 0.7294118f;
	winner_color->blue = 1.0f;
	winner_color->alpha = 1.0f;

	normal_color->red = 1.0f;
	normal_color->green = 1.0f;
	normal_color->blue = 0.0f;
	normal_color->alpha = 1.0f;

	get_selected_color(hilite_color);

	return;
}

boolean game_engine_running(
	void)
{
	boolean running = game_engine!=NULL;

	return running;
}

#ifdef HALO_N64
/* the N64's pause menu (port/n64/game/n64_game.c): 0 while the game is
played, 1 to 3 once it has ended (no pause then) */
long n64_game_engine_postgame_state(
	void)
{
	return game_engine_globals.postgame_state;
}
#endif

boolean game_engine_force_single_screen(
	void)
{
	boolean force_single_screen = FALSE;

	if (game_engine &&
		game_engine_globals.postgame_state>=2 &&
		game_engine_globals.postgame_state<=3)
	{
		force_single_screen = TRUE;
	}

	return force_single_screen;
}

void game_engine_dispose(
	void)
{
	if (game_engine)
	{
		if (game_engine->dispose)
			game_engine->dispose();

		game_engine = NULL;
	}

	return;
}

static void game_engine_update_player_no_shield(
	long player_index)
{
	if (game_engine_has_shield(player_index))
		return;

	{
		struct player_datum *player;

		player = player_get(player_index);
		if (player->unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(player->unit_index);

			unit->object.shield_vitality = 0.0f;
			unit->object.maximum_shield_vitality = 0.0f;
		}
	}

	return;
}

static void game_engine_build_lighting(
	void)
{
	long player_count = 0;
	long object_count = 0;

	{
		struct data_iterator iterator;

		data_iterator_new(&iterator, player_data);
		while (data_iterator_next(&iterator))
			player_count++;
	}

	if (global_variant.universal_variant.vehicle_set != 1)
	{
		/* Engine type 5 is assigned by the four race variant builders below. */
		if (global_variant.game_engine_index == 5)
		{
			struct scenario *scenario = global_scenario_get();
			short flag_index;

			for (flag_index = 0;
				flag_index < scenario->netgame_flags.count;
				flag_index++)
			{
				struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
					&scenario->netgame_flags,
					flag_index,
					struct scenario_netgame_flag);

				/* The target counts netgame flag type 4 for race games. */
				if (flag->type == 4)
					object_count++;
			}

			if (object_count > player_count)
				object_count = player_count;
		}
		else
		{
			struct object_iterator object_iterator;

			object_iterator_new(
				&object_iterator,
				_object_mask_vehicle,
				0);
			while (object_iterator_next(&object_iterator))
				object_count++;
		}
	}

	if ((player_count > 8 && object_count >= 2) ||
		player_count >= 13)
	{
		game_engine_globals.flags |= FLAG(0);
	}

	if (player_count > 4 ||
		object_count >= 4 ||
		TEST_FLAG(game_engine_globals.flags, 0))
	{
		game_engine_globals.flags |= FLAG(1);
	}

	if (player_count >= 5)
		game_engine_globals.flags |= FLAG(2);

	if (player_count >= 9)
		game_engine_globals.flags |= FLAG(3);

	return;
}

void game_engine_dispose_from_old_map(
	void)
{
	if (game_engine && game_engine->dispose_from_old_map)
		game_engine->dispose_from_old_map();

	return;
}

void game_engine_game_ending(
	void)
{
	if (game_engine && game_engine->game_ending)
		game_engine->game_ending();

	return;
}

void game_engine_game_starting(
	void)
{
	if (game_engine)
	{
		if (game_engine->game_starting)
			game_engine->game_starting();

		game_engine_build_lighting();
	}

	return;
}

void game_engine_statistics_append(
	long statistic)
{
	if (game_engine && game_engine->statistics_append)
		game_engine->statistics_append(statistic);

	return;
}

void game_engine_handle_client_message(
	void *message)
{
	if (game_engine && game_engine->handle_client_message)
		game_engine->handle_client_message(message);

	return;
}

void game_engine_handle_server_message(
	void *message)
{
	if (game_engine && game_engine->handle_server_message)
		game_engine->handle_server_message(message);

	return;
}

void game_engine_post_rasterize_objects(
	void)
{
	if (game_engine && game_engine->post_rasterize_objects)
		game_engine->post_rasterize_objects();

	return;
}

void game_engine_post_rasterize(
	void)
{
	if (game_engine)
	{
		switch (game_engine_globals.postgame_state)
		{
		case 0:
		case 1:
			game_engine_post_rasterize_in_game();
			break;
		case 2:
		case 3:
			game_engine_post_rasterize_post_game();
			break;
		default:
			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine.c",
				0x7B7,
				!"unreachable");
			break;
		}
	}

	return;
}

void game_engine_nonplayer_post_rasterize(
	void)
{
	if (game_engine)
	{
		switch (game_engine_globals.postgame_state)
		{
		case 0:
		case 1:
			game_engine_press_start_to_begin();
			break;

		case 2:
		case 3:
			{
				rectangle2d window_bounds;
				long local_player_index;

				game_engine_post_rasterize_post_game();
				window_bounds.x0 = 0;
				window_bounds.x1 = 640;
				window_bounds.y0 = 0;
				window_bounds.y1 = 480;

				for (local_player_index = 0;
					local_player_index < MAXIMUM_LOCAL_PLAYERS;
					local_player_index++)
				{
					render_ui_widgets_postgame(
						(short)local_player_index,
						&window_bounds);
					rumble_player_clear((short)local_player_index);
				}
			}
			break;

		default:
			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine.c",
				0xE3B,
				!"unreachable");
			break;
		}
	}

	return;
}

static void game_engine_update_teleporter(
	long player_index)
{
	static int blocked_message_delay = 0;
	static long fade_function = 0;
	static long screen_flash_type = 6;
	static real max_intensity = 1.0f;
	static real alpha = 0.5f;
	static real red = 0.35f;
	static real green = 1.0f;
	static real blue = 0.35f;
	static real duration = 1.0f;
	struct scenario *scenario = global_scenario_get();
	struct player_datum *player = player_get(player_index);
	struct unit_datum *unit;
	struct scenario_netgame_flag *source_flag;
	struct scenario_netgame_flag *destination_flag;
	real_vector3d forward;
	long source_flag_index;
	long destination_flag_index;

	if (player->unit_index == NONE)
		return;

	unit = unit_get(player->unit_index);
	if (player->unknown70 != NONE)
	{
		struct scenario_netgame_flag *previous_flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			player->unknown70,
			struct scenario_netgame_flag);
		real_vector3d delta;

		vector_from_points3d(
			&previous_flag->position,
			&unit->object.position,
			&delta);
		if (delta.j * delta.j + delta.i * delta.i + delta.k * delta.k > 1.0f)
			player->unknown70 = NONE;
	}

	source_flag_index = NONE;
	find_netgame_flags(
		&unit->object.position,
		0.5f,
		0.0f,
		_netgame_flag_teleporter_source,
		NONE,
		1,
		&source_flag_index);
	if (source_flag_index == NONE ||
		source_flag_index == player->unknown70)
	{
		return;
	}

	source_flag = TAG_BLOCK_GET_ELEMENT(
		&scenario->netgame_flags,
		source_flag_index,
		struct scenario_netgame_flag);
	destination_flag_index = NONE;
	find_netgame_flags(
		NULL,
		0.0f,
		0.0f,
		_netgame_flag_teleporter_target,
		source_flag->team_index,
		1,
		&destination_flag_index);
	if (destination_flag_index != NONE)
	{
		destination_flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			destination_flag_index,
			struct scenario_netgame_flag);

		{
			struct collision_feature_list features;
			struct player_datum *unit_player;
			real_point3d position;
			real height;
			real radius;

			unit = unit_get(player->unit_index);
			forward = unit->object.forward;
			unit_player = player_get(player_index);
			biped_get_physics_pill(
				unit_player->unit_index,
				&position,
				&height,
				&radius);
			position = destination_flag->position;
			{
				struct collision_plane point_test_result;

				if (collision_get_features_in_sphere(
						0x200380,
						&position,
						height + radius * 2.0f,
						height,
						radius,
						NONE,
						&features) &&
					collision_features_test_point(
						&features,
						&position,
						&point_test_result))
				{
					if (point_test_result.object_index != NONE)
					{
						if (TEST_FLAG(
								_object_mask_unit,
								object_get(
									point_test_result.object_index)
									->object.type))
						{
							struct unit_datum *blocking_unit = unit_get(
								point_test_result.object_index);
							if (blocking_unit->unit.player_index != NONE)
							{
								struct player_datum *blocking_player =
									player_get(blocking_unit->unit.player_index);
								blocking_player->is_blocking_teleporter = TRUE;
								blocking_player->telefrag_timeout++;
							}
						}
					}

					if (blocked_message_delay > 0)
					{
						blocked_message_delay--;
						return;
					}

					blocked_message_delay = 120;
					{
						long string_list_index =
							tag_loaded('ustr', "ui\\multiplayer_game_text");
						wchar_t const *message;

						if (string_list_index != NONE)
							message = unicode_string_list_get_string(
								string_list_index,
								0x65);
						else
							message = L"";
						hud_print_message(
							unit_get_local_player_index(unit_player->unit_index),
							message);
					}
					return;
				}
			}
		}

		if (player->local_player_index != NONE)
		{
			game_engine_play_multiplayer_sound(0x1B);
			if (player->local_player_index != NONE)
			{
				struct screen_flash_definition screen_flash = { 0 };

				screen_flash.fade_function = fade_function;
				screen_flash.type = screen_flash_type;
				screen_flash.duration = duration;
				screen_flash.priority = 2;
				screen_flash.max_intensity = max_intensity;
				screen_flash.zero_scale_factor = 0.0f;
				screen_flash.screen_flash_color.alpha = alpha;
				screen_flash.screen_flash_color.red = red;
				screen_flash.screen_flash_color.green = green;
				screen_flash.screen_flash_color.blue = blue;
				player_effect_screen_flash(
					player_index,
					&screen_flash,
					1.0f);
			}
		}

		{
			real angle =
				arctangent(forward.j, forward.i) +
				destination_flag->facing -
				source_flag->facing;

			forward.i = cosine(angle);
			forward.j = sine(angle);
			normalize3d(&forward);
			object_set_position(
				player->unit_index,
				&destination_flag->position,
				&forward,
				NULL);
			if (player->local_player_index != NONE)
				player_control_set_facing(player->local_player_index, &forward);

			player->unknown70 = find_netgame_flag(
				&unit->object.position,
				1.0f,
				0.0f,
				_netgame_flag_teleporter_source,
				NONE);
		}
		return;
	}

	console_printf(
		FALSE,
		"failed to teleport %d",
		source_flag->team_index);

	return;
}

void game_engine_update(
	void)
{
	union game_engine_update_iterator iterator;
	struct player_datum *player;

	if (!game_engine)
		return;

	game_engine_update_multiplayer_sound();
	game_engine_update_purge();
	game_engine_update_weapons();
	game_engine_update_item_spawn();

	data_iterator_new(&iterator.data, player_data);
	while (data_iterator_next(&iterator.data))
	{
		game_engine_update_player_no_shield(iterator.data.datum_index);

		{
			long player_index = iterator.data.datum_index;

			if (game_engine)
			{
				boolean always_invisible = FALSE;

				if (TEST_FLAG(
					global_variant.universal_variant.flags,
					_game_variant_always_invisible_bit))
				{
					always_invisible = TRUE;
				}
				else if (game_engine->test_trait)
					always_invisible =
						game_engine->test_trait(player_index, 1);

				if (always_invisible &&
					player_get(player_index)->unit_index != NONE)
				{
					player_handle_powerup_minor(player_index, 0, 15);
				}
			}

			game_engine_update_teleporter(iterator.data.datum_index);

			/* (a client of the distributed netcode has the host's scores,
			flags, balls and hills: game_engine_read_network_state) */
			if (game_engine->player_update_each_tick
#ifdef HALO_LINUX
				&& !network_game_distributed_client()
#endif
				)
			{
				void (*player_update)(long) =
					game_engine->player_update_each_tick;
				player_update(iterator.data.datum_index);
			}
		}

	}

	if (game_engine->unknown44
#ifdef HALO_LINUX
		&& !network_game_distributed_client()
#endif
		)
	{
		game_engine->unknown44();
	}

	switch (game_engine_globals.postgame_state)
	{
	case 0:
		/* (a client of the distributed netcode ends the game when the host
		has) */
		if (
#ifdef HALO_LINUX
			!network_game_distributed_client() &&
#endif
			game_engine_should_end_game())
		{
			game_engine_end_game();
		}
		break;

	case 1:
		if (game_engine_globals.postgame_timer <= 2.0f &&
			!TEST_FLAG(game_engine_globals.flags, 4))
		{
			sound_class_set_gain("", 0.0f, 30);
			sound_class_set_gain("ambient_nature", 0.2f, 30);
			sound_class_set_gain("ambient_machinery", 0.2f, 30);
			sound_class_set_gain("ambient_computers", 0.2f, 30);
			game_engine_globals.flags |= FLAG(4);
		}

		game_engine_globals.postgame_timer -= 1.0f / TICKS_PER_SECOND;
		if (game_engine_globals.postgame_timer <= 0.0f)
		{
			struct network_game_server *server;

			game_engine_globals.postgame_progress = 0.0f;
			game_engine_globals.postgame_state = 2;
			game_engine_globals.postgame_timer = 5.0f;

			data_iterator_new(&iterator.data, player_data);
			player = (struct player_datum *)data_iterator_next(&iterator.data);
			while (player)
			{
				if (player->unit_index != NONE)
					unit_kill(player->unit_index);
				if (player->local_player_index != NONE)
					rumble_player_clear(player->local_player_index);

				player = (struct player_datum *)data_iterator_next(&iterator.data);
			}

			object_iterator_new(
				&iterator.object,
				_object_mask_vehicle,
				0);
			while (object_iterator_next(&iterator.object))
			{
				object_delete(iterator.object.index);
			}

			server = global_network_game_server_get();
			if (server)
				network_game_server_switch_to_postgame(
					global_network_game_server_get());
		}
		break;

	case 2:
	case 3:
		break;

	default:
		match_assert(
			"c:\\halo\\SOURCE\\game\\game_engine.c",
			0xA03,
			!"unreachable");
		break;
	}

	return;
}


enum game_engine_message
{
	_game_engine_message_welcome = 0,
	_game_engine_message_killed_by_unknown,
	_game_engine_message_killed_by_biped,
	_game_engine_message_killed_by_vehicle,
	_game_engine_message_killed_by_player,
	_game_engine_message_killed_by_friendly_fire,
	_game_engine_message_killed_by_self,
	_game_engine_message_double_kill,
	_game_engine_message_killed_enemy,
	_game_engine_message_triple_kill,
	_game_engine_message_multi_kill,
	_game_engine_message_five_kills_in_row,
	_game_engine_message_ten_kills_in_a_row,
	_game_engine_message_killed_friendly,
	_game_engine_message_multi_kill_with_score,
	_game_engine_message_triple_kill_with_score,
	_game_engine_message_double_kill_with_score,
	_game_engine_message_ten_kills_in_a_row_with_score,
	_game_engine_message_five_kills_in_row_with_score,
	_game_engine_message_killed_enemy_with_score,
	_game_engine_message_winner,
	_game_engine_message_team_winner,
	_game_engine_message_show_score,
	_game_engine_message_odd_man_out,
	_game_engine_message_out_of_lives,
	_game_engine_message_respawn_timer,
	_game_engine_message_waiting_for_space_to_clear,
	_game_engine_message_player_quit_self,
	_game_engine_message_quit,
	_game_engine_message_press_back_for_score,
	_game_engine_message_time_left,
	NUMBER_OF_GAME_ENGINE_MESSAGES,
};

enum game_engine_postgame_state
{
	_game_engine_postgame_state_active = 0,
	_game_engine_postgame_state_delay,
	_game_engine_postgame_state_rasterize_delay,
	_game_engine_postgame_state_rasterize,
};

enum
{
	_multiplayer_sound_countdown_for_respawn = 0x1D,
	_multiplayer_sound_respawn = 0x1F,
	_equipment_created_at_rest_bit = 0,
};

void game_engine_player_killed(
	long killing_player_index,
	long killing_object_index,
	long dead_player_index,
	boolean friendly_fire)
{
	struct player_datum *dead_player = player_get(dead_player_index);
	boolean player_kill;
	boolean same_player;
	boolean valid_players;
	long message = NONE;

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0xA31,
		dead_player_index != NONE);

	if (!game_engine)
		return;

#ifdef HALO_LINUX
	/* the distributed netcode: a client's copy of a death has the host's
	killer (port/linux/game/network_distributed.c) */
	network_distributed_player_killed(&killing_player_index, &killing_object_index, dead_player_index,
		&friendly_fire);
#endif
	dead_player->death_time = game_time_get();
	if (game_engine->player_killed_player)
	{
		game_engine->player_killed_player(
			killing_player_index,
			killing_object_index,
			dead_player_index,
			friendly_fire);
	}

	same_player = killing_player_index == dead_player_index;
	if (killing_player_index != NONE && dead_player_index != NONE)
		valid_players = TRUE;
	else
		valid_players = FALSE;
	player_kill = !friendly_fire && valid_players && !same_player;

	dead_player->respawn_timer =
		dead_player->respawn_penalty + global_variant.universal_variant.respawn_time;
	if (global_variant.universal_variant.respawn_time_growth > 0)
	{
		dead_player->respawn_penalty += global_variant.universal_variant.respawn_time_growth;
		dead_player->respawn_penalty =
			MIN(
				dead_player->respawn_penalty,
				global_variant.universal_variant.respawn_time_growth * 5);
		if (player_kill && killing_player_index != NONE)
		{
			struct player_datum *killing_player =
				player_get(killing_player_index);
			killing_player->respawn_penalty -= global_variant.universal_variant.respawn_time_growth;
			killing_player->respawn_penalty =
				MAX(killing_player->respawn_penalty, 0);
		}
	}

	if (!player_kill)
		dead_player->respawn_timer += global_variant.universal_variant.suicide_penalty;

	dead_player->respawn_timer = MAX(dead_player->respawn_timer, 90);
	dead_player = player_get(dead_player_index);

	if (!dead_player->quit_out_of_game)
	{
		if (killing_player_index == NONE)
		{
			if (killing_object_index == NONE)
				message = _game_engine_message_killed_by_unknown;
			else
			{
				struct object_datum *object = object_get(killing_object_index);

				unit_try_and_get(killing_object_index);
				switch (object->object.type)
				{
				case _object_type_biped:
					message = _game_engine_message_killed_by_biped;
					break;
				case _object_type_vehicle:
					message = _game_engine_message_killed_by_vehicle;
					break;
				default:
					message = _game_engine_message_killed_by_unknown;
					break;
				}
			}
		}
		else if (killing_player_index == dead_player_index)
			message = _game_engine_message_killed_by_self;
		else
			message =
				_game_engine_message_killed_by_player + (friendly_fire != FALSE);

		game_show_score_extended(
			dead_player_index,
			message,
			killing_player_index);
	}
	else
	{
		struct data_iterator iterator;

		data_iterator_new(&iterator, player_data);
		while (data_iterator_next(&iterator))
		{
			multiplayer_message(
				iterator.datum_index,
				_game_engine_message_quit,
				dead_player_index);
		}
	}

	if (message == _game_engine_message_killed_by_friendly_fire)
	{
		if (killing_player_index != NONE)
		{
			multiplayer_message(
				killing_player_index,
				_game_engine_message_killed_friendly,
				dead_player_index);
		}
		else
		{
			struct data_iterator iterator;

			data_iterator_new(&iterator, player_data);
			while (data_iterator_next(&iterator))
			{
				multiplayer_message(
					iterator.datum_index,
					_game_engine_message_killed_friendly,
					dead_player_index);
			}
		}
	}
	else if (message == _game_engine_message_killed_by_player)
	{
		struct player_datum *killing_player =
			player_get(killing_player_index);

		if (killing_player->statistics.multiple_kills >=
			_game_engine_message_killed_by_player)
		{
			message = _game_engine_message_multi_kill;
		}
		else if (killing_player->statistics.multiple_kills == 3)
			message = _game_engine_message_triple_kill;
		else if (killing_player->statistics.multiple_kills == 2)
			message = _game_engine_message_double_kill;
		else if (killing_player->statistics.kills_in_a_row == 5)
			message = _game_engine_message_five_kills_in_row;
		else
		{
			message = (killing_player->statistics.kills_in_a_row % 5)
				? _game_engine_message_killed_enemy
				: _game_engine_message_ten_kills_in_a_row;
		}

		game_show_score_extended(
			killing_player_index,
			message,
			dead_player_index);
	}

	return;
}

void game_engine_update_non_deterministic(
	real delta_seconds)
{
	if (game_engine)
	{
		switch (game_engine_globals.postgame_state)
		{
		case 2:
			rumble_clear_all_now();
			game_engine_globals.postgame_timer -= delta_seconds;
			if (game_engine_globals.postgame_timer <= 0.0f)
				game_engine_globals.postgame_state = 3;
			break;

		case 3:
			rumble_clear_all_now();
			game_engine_globals.postgame_progress += delta_seconds;
			if (game_engine_globals.postgame_progress > 1.0f)
				game_engine_globals.postgame_progress = 1.0f;

#ifdef HALO_N64
			/* back to the N64's own menu (port/n64/src/n64_menu.c), which
			shows the results: any player's A or START once the scores are
			up, or by itself after fifteen seconds */
			game_engine_globals.postgame_timer -= delta_seconds;
			if (game_engine_globals.postgame_progress >= 1.0f &&
				(test_any_gamepad_button(_gamepad_analog_button_a) ||
				test_any_gamepad_button(_gamepad_binary_button_start) ||
				game_engine_globals.postgame_timer < -15.0f))
			{
				main_goto_main_menu();
			}
#else
			if (test_any_gamepad_button(0) || test_any_gamepad_button(12))
			{
				if (global_network_game_server_get())
				network_game_server_reset_to_pregame(
					global_network_game_server_get());
			}
			else if (test_any_gamepad_button(1) || test_any_gamepad_button(13))
			{
				network_game_abort();
			}
#endif
			break;
		}
	}

	return;
}

boolean game_engine_display_team_indicators(
	void)
{
	boolean display = FALSE;

	if (game_engine)
		display = TEST_FLAG(global_variant.universal_variant.flags, _game_variant_unknown1_bit) && global_variant.universal_variant.teams;

	return display;
}

boolean game_engine_can_score(
	void)
{
	boolean can_score = TRUE;

	if (game_engine)
		can_score = game_engine_globals.postgame_state==0;

	return can_score;
}

boolean game_engine_infinite_grenades(
	long player_index)
{
	boolean infinite_grenades = FALSE;

	if (game_engine &&
		player_index!=NONE &&
		!TEST_FLAG(game_engine_globals.flags, _game_engine_disable_infinite_grenades_bit))
	{
		infinite_grenades = TEST_FLAG(global_variant.universal_variant.flags, _game_variant_infinite_grenades_bit);
	}

	return infinite_grenades;
}

boolean game_engine_has_shield(
	long player_index)
{
	boolean has_shield = TRUE;

	if (game_engine && player_index!=NONE)
		has_shield = !TEST_FLAG(global_variant.universal_variant.flags, _game_variant_no_shields_bit);

	return has_shield;
}

boolean game_engine_draw_object_in_motion_sensor(
	long unit_index)
{
	boolean draw_object = TRUE;

	if (game_engine)
		draw_object = TEST_FLAG(global_variant.universal_variant.flags, _game_variant_draw_object_in_motion_sensor_bit);

	return draw_object;
}

boolean game_engine_hud_draw_motion_sensor(
	long player_index)
{
	boolean draw_motion_sensor = TRUE;

	if (game_engine)
	{
		boolean default_draw_motion_sensor;

		draw_motion_sensor =
			TEST_FLAG(global_variant.universal_variant.flags, _game_variant_draw_object_in_motion_sensor_bit);
		default_draw_motion_sensor = global_variant.universal_variant.goal_radar == 0;

		if (global_variant.game_engine_index == 2 && !global_variant.game_engine_variant.slayer.kill_in_order)
			default_draw_motion_sensor = FALSE;

		draw_motion_sensor |= default_draw_motion_sensor;
	}

	return draw_motion_sensor;
}

boolean game_engine_player_has_stealth_weapon(
	long player_index)
{
	boolean has_stealth_weapon = FALSE;

	if (player_index != NONE)
	{
		struct player_datum *player = player_get(player_index);
		if (player->unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(player->unit_index);
			long current_weapon_index = unit->unit.current_weapon_index;

			if (current_weapon_index != NONE)
			{
				long weapon_index =
					unit->unit.weapon_object_indices[current_weapon_index];
				if (weapon_index != NONE)
				{
					struct weapon_datum *weapon = weapon_get(weapon_index);
					if (weapon->definition_index != NONE)
					{
						struct weapon_definition *weapon_definition =
							weapon_definition_get(weapon->definition_index);
						has_stealth_weapon = TEST_FLAG(
							weapon_definition->weapon.flags,
							_weapon_does_not_depower_active_camo_bit);
					}
				}
			}
		}
	}

	return has_stealth_weapon;
}

void game_engine_weapon_fired(
	long player_index)
{
	if (game_engine && player_index != NONE)
	{
		struct player_datum *player = player_get(player_index);
		long unit_index = player->unit_index;

		if (unit_index != NONE)
		{
			struct unit_datum *unit = unit_get(unit_index);
			long weapon_index = unit_inventory_get_weapon(
				unit_index,
				unit_get(unit_index)->unit.current_weapon_index);
			real active_camouflage_decrease = 0.1f;

			if (game_engine_player_has_stealth_weapon(player_index))
			{
				active_camouflage_decrease = 0.0f;
			}
			else if (weapon_index != NONE)
			{
				struct weapon_datum *weapon = weapon_get(weapon_index);
				struct weapon_definition *weapon_definition =
					weapon_definition_get(weapon->definition_index);

				if (0.0f != weapon_definition->weapon.active_camo_ding)
					active_camouflage_decrease =
						weapon_definition->weapon.active_camo_ding;
			}

			if (unit->unit.active_camouflage >= 0.05f)
			{
				unit->unit.active_camouflage -= active_camouflage_decrease;
				unit->unit.cause_for_camo_regrowth = 1;
				unit->unit.active_camouflage =
					MAX(0.05f, unit->unit.active_camouflage);
			}
		}
	}

	return;
}

wchar_t *get_place_name(
	struct game_engine_place place)
{
	long lookup_index;
	long string_list_index;

#ifdef HALO_LINUX
	/* port: a session can have more places than the string list names */
	match_vassert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x1316,
		place.place < MULTIPLAYER_MAXIMUM_PLAYERS,
		"place.place < maximum_places");
#else
	match_vassert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x1316,
		place.place < 16,
		"place.place < maximum_places");
#endif

	if ((place.flags & FLAG(2)) && TEST_FLAG(place.flags, 0))
		lookup_index = 35;
	else if ((place.flags & FLAG(2)) && place.place == 0)
		lookup_index = 33;
	else if ((place.flags & FLAG(2)) && place.place == 1)
		lookup_index = 34;
	else if (TEST_FLAG(place.flags, 1))
		lookup_index = 32;
#ifdef HALO_LINUX
	/* port: places past the 16th are spelled out; a tie is marked like the
	string list's tied places */
	else if (place.place >= NUMBER_OF_PLACE_STRINGS)
		return place_ordinal_string(place.place, TEST_FLAG(place.flags, _place_tied));
#endif
	else
	{
		lookup_index = place.place;
		if (TEST_FLAG(place.flags, 0))
			lookup_index += 16;
	}

	match_vassert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x1331,
		lookup_index != NONE,
		"NONE != lookup_index");

	string_list_index = tag_loaded('ustr', "ui\\multiplayer_game_text");
	if (string_list_index != NONE)
		return unicode_string_list_get_string(
			string_list_index,
			lookup_index + 0x66);

	return L"";
}

long find_netgame_flags(
	real_point3d const *position,
	real radius,
	real height,
	short type,
	short team_index,
	long maximum_count,
	long *flag_indices)
{
	real radius_squared = radius * radius;
	long found_count = 0;
	short flag_index;
	struct scenario *scenario;

	scenario = global_scenario_get();
	flag_index = 0;

#ifdef HALO_N64
	/* each type's flags in index order, listed once a map: every player's
	teleporter check (game_engine_update_teleporters) read every flag of
	the map each tick, two per player; the same flags in the same order */
	if (type >= 0 && type < 16 && scenario->netgame_flags.count <= 256)
	{
		extern long n64_map_generation;
		static long generation = -1;
		static void const *address;
		static long count;
		static short first[17];
		static unsigned char indices[256];
		short list_index;

		if (generation != n64_map_generation || address != scenario->netgame_flags.address ||
			count != scenario->netgame_flags.count)
		{
			short kind;
			short listed = 0;

			for (kind = 0; kind < 16; kind++)
			{
				first[kind] = listed;
				for (flag_index = 0; flag_index < scenario->netgame_flags.count; flag_index++)
				{
					if (TAG_BLOCK_GET_ELEMENT(&scenario->netgame_flags, flag_index, struct scenario_netgame_flag)->type == kind)
						indices[listed++] = (unsigned char)flag_index;
				}
			}
			first[16] = listed;
			generation = n64_map_generation;
			address = scenario->netgame_flags.address;
			count = scenario->netgame_flags.count;
		}
		for (list_index = first[type]; list_index < first[type + 1]; list_index++)
		{
			struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
				&scenario->netgame_flags,
				indices[list_index],
				struct scenario_netgame_flag);

			if (team_index != NONE && team_index != flag->team_index)
				continue;
			if (position)
			{
				if (radius >= 0.0f &&
					distance_squared3d(&flag->position, position) > radius_squared)
				{
					continue;
				}
				if (height > 0.0f &&
					fabs(flag->position.z - position->z) > height)
				{
					continue;
				}
			}
			if (found_count < maximum_count)
				flag_indices[found_count++] = indices[list_index];
		}
		return found_count;
	}
#endif
	for (; flag_index < scenario->netgame_flags.count; flag_index++)
	{
		struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			flag_index,
			struct scenario_netgame_flag);

		if (type != NONE && type != flag->type)
			continue;

		if (team_index != NONE && team_index != flag->team_index)
			continue;

		if (position)
		{
			if (radius >= 0.0f &&
				distance_squared3d(&flag->position, position) > radius_squared)
			{
				continue;
			}

			if (height > 0.0f &&
				fabs(flag->position.z - position->z) > height)
			{
				continue;
			}
		}

		if (found_count < maximum_count)
			flag_indices[found_count++] = flag_index;
	}

	return found_count;
}

long find_netgame_flag(
	real_point3d const *position,
	real radius,
	real height,
	short type,
	short team_index)
{
	long flag_index = NONE;

	find_netgame_flags(
		position,
		radius,
		height,
		type,
		team_index,
		1,
		&flag_index);

	return flag_index;
}

/* playlist data: in the Aug-15-2001 build the playlist functions were the only users of these two globals;
   this build no longer reads them. their definition point here, with the playlist code, is inferred (the
   Aug-2001 users and the later builds' layout), not attested */
void *global_autogenerate_list = NULL;
long global_autogenerate_count = 0;

void game_engine_playlist_next(
	long parameter0,
	long parameter1,
	long playlist_type)
{
	char *map_name;
	struct game_variant variant;

	csstrcpy(global_stage.map_name, "levels\\test\\carousel\\carousel");
	map_name = main_get_multiplayer_map_name();
	if (map_name && map_name[0])
	{
		csstrncpy(global_stage.map_name, map_name, sizeof(global_stage.map_name) - 1);
		global_stage.map_name[sizeof(global_stage.map_name) - 1] = 0;
	}

	if (player_ui_game_variant_specified(&variant))
		csmemcpy(&global_stage.variant, &variant, sizeof(global_stage.variant));

	return;
}

boolean game_engine_should_end_game(
	void)
{
	boolean should_end_game = FALSE;

#ifdef HALO_N64
	{
		/* (Firefight's players are one team against the Covenant, who
		aren't players: with two or more, one team was all there was and
		the game ended as it began. n64_firefight.c ends it itself, when
		everyone is out) */
		extern int n64_firefight_active(void);

		if (n64_firefight_active())
			return FALSE;
	}
#endif
	if (game_engine && !multiple_teams_alive())
		should_end_game = TRUE;

	return should_end_game;
}

void game_engine_clear_goal_position(
	short goal_index)
{
	csmemset(&global_goal[goal_index], 0, sizeof(struct game_engine_goal));

	return;
}

long get_flag_definition_index(
	void)
{
	struct game_globals *game_globals;
	struct game_globals_multiplayer_information *multiplayer_information;

	global_scenario_get();
	game_globals = scenario_get_game_globals();
	multiplayer_information = TAG_BLOCK_GET_ELEMENT(
		&game_globals->multiplayer_information,
		0,
		struct game_globals_multiplayer_information);

	return multiplayer_information->flag.index;
}

long get_ball_definition_index(
	void)
{
	struct game_globals *game_globals;
	struct game_globals_multiplayer_information *multiplayer_information;

	global_scenario_get();
	game_globals = scenario_get_game_globals();
	multiplayer_information = TAG_BLOCK_GET_ELEMENT(
		&game_globals->multiplayer_information,
		0,
		struct game_globals_multiplayer_information);

	return multiplayer_information->ball.index;
}

void game_engine_override_map_name(
	char const *map_name)
{
	if (map_name && map_name[0])
		csstrncpy(global_stage.map_name, map_name, sizeof(global_stage.map_name)-1);

	return;
}

void game_engine_override_game_variant(
	struct game_variant const *variant)
{
	if (variant)
		csmemcpy(&global_stage.variant, variant, sizeof(global_stage.variant));

	return;
}

void game_engine_switch_to_postgame(
	void)
{
	if (game_engine_globals.postgame_state==0)
	{
		if (global_network_game_server_get())
		{
			game_engine_globals.postgame_state = 1;
			game_engine_globals.postgame_timer = 7.0f;
		}
		else
		{
			game_engine_globals.postgame_state = 3;
		}
	}

	return;
}

void game_engine_load_stage(
	char const *map_name)
{
	if (!map_name || csstrcmp(global_stage.map_name, map_name))
		main_set_multiplayer_map_name(global_stage.map_name);

	game_set_game_variant(&global_stage.variant);
	if (!network_game_is_active())
		main_reset_map();

	return;
}

void game_engine_end_game(
	void)
{
	if (game_engine_globals.postgame_state==0)
	{
		game_engine_globals.postgame_state = 1;
		game_engine_globals.postgame_timer = 7.0f;
		game_engine_play_multiplayer_sound(1);
		ui_widgets_close_all();
	}

	return;
}

void game_engine_player_damaged_player(
	long damaging_player_index,
	long dead_player_index,
	boolean damage_type)
{
	match_assert("c:\\halo\\SOURCE\\game\\game_engine.c", 0xA20, dead_player_index != NONE);

#ifdef HALO_LINUX
	/* (a client of the distributed netcode replaying the host's damage has
	the host's game type state, game_engine_read_network_state) */
	if (network_game_distributed_client())
		return;
#endif
	if (game_engine && game_engine->player_damaged_player)
		game_engine->player_damaged_player(damaging_player_index, dead_player_index, damage_type);

	return;
}

static boolean game_engine_player_is_odd_man_out(
	long player_index)
{
	struct player_datum *player = player_get(player_index);
	boolean result = FALSE;

	if (global_variant.universal_variant.odd_man_out && player->unit_index==NONE)
	{
		struct data_iterator iterator;
		struct player_datum *other_player;

		result = TRUE;
		data_iterator_new(&iterator, player_data);
		other_player = (struct player_datum *)data_iterator_next(&iterator);
		while (other_player)
		{
			if (other_player->unit_index==NONE &&
				other_player!=player &&
				(other_player->death_time>player->death_time ||
					(other_player->death_time==player->death_time &&
						DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)<
							DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.datum_index))))
			{
				result = FALSE;
			}

			other_player = (struct player_datum *)data_iterator_next(&iterator);
		}
	}

	return result;
}

boolean game_engine_player_is_out_of_lives(
	long player_index)
{
	boolean out_of_lives = FALSE;

	if (global_variant.universal_variant.lives>0)
	{
		struct player_datum *player = player_get(player_index);

		if (player->unit_index==NONE && player->statistics.deaths>=global_variant.universal_variant.lives)
			out_of_lives = TRUE;
	}

	return out_of_lives;
}

short game_engine_player_get_custom_motion_sensor_positions(
	long player_index,
	real_point2d *positions,
	byte *goal_indices,
	short maximum_count)
{
	short count = 0;

	if (game_engine && global_variant.universal_variant.goal_radar == 0 && player_index != NONE)
	{
		struct player_datum *player = player_get(player_index);
		long goal_index = 0;
		struct game_engine_goal *goal = global_goal;

		do
		{
			if (goal_matches_player(goal_index, player, player_index) && count < maximum_count)
			{
				goal_indices[count] = goal_index;
				positions[count].x = goal->position.x;
				positions[count].y = goal->position.y;
				count++;
			}

			goal++;
			goal_index++;
		}
		/* January uses the adjacent global_variant address as the signed loop bound. */
#ifdef HALO_LINUX
		/* port: bounded by the goal array itself, which is larger here and
		need not sit next to global_variant */
		while (goal < global_goal + NUMBEROF(global_goal));
#else
		while ((long)goal < (long)&global_variant);
#endif
	}

	return count;
}

#ifdef HALO_N64
static long n64_goal_render_types[4][NUMBEROF(global_goal)];
static unsigned short n64_goal_draws[4];
#endif

void game_engine_render_nav_points(
	long local_player_index)
{
#ifdef HALO_N64
	n64_goal_draws[local_player_index & 3]++;
#endif
	if (game_engine &&
		global_variant.universal_variant.goal_radar == 1 &&
		(short)local_player_index != NONE)
	{
		long player_index = local_player_get_player_index((short)local_player_index);

		if (player_index != NONE)
		{
			struct player_datum *player = player_get(player_index);

			if (player->unit_index != NONE)
			{
				real_point3d head_position;

				unit_get_head_position(player->unit_index, &head_position);
				{
					long goal_index = 0;
					struct game_engine_goal *goal = global_goal;

					do
					{
						if (goal_matches_player(goal_index, player, player_index))
						{
#ifdef HALO_N64
							/* (a ray from the head to each goal every HUD draw: each
							goal tests again every fourth draw, staggered) */
							long render_type;

							if (((n64_goal_draws[local_player_index & 3] + goal_index) & 3) == 0)
							{
								n64_goal_render_types[local_player_index & 3][goal_index] = hud_get_nav_point_render_type(
									local_player_index,
									&head_position,
									goal,
									NONE);
							}
							render_type = n64_goal_render_types[local_player_index & 3][goal_index];
#else
							long render_type = hud_get_nav_point_render_type(
								local_player_index,
								&head_position,
								goal,
								NONE);
#endif

							custom_render_nav_point(
								local_player_index,
								goal,
								(word)goal->nav_point_index,
								render_type);
						}

						goal++;
						goal_index++;
					}
#ifdef HALO_LINUX
					/* port: bounded by the goal array itself, as above */
					while (goal < global_goal + NUMBEROF(global_goal));
#else
					while ((long)goal < (long)&global_variant);
#endif
				}
			}
		}
	}

	return;
}

boolean game_engine_hud_draw_messages(
	long player_index)
{
	boolean draw_messages = TRUE;

	if (game_engine && player_index!=NONE)
	{
		struct player_datum *player = player_get(player_index);

		if (player->local_player_index!=NONE &&
			game_engine_globals.hud_message_timers[player->local_player_index]>0.0f)
		{
			draw_messages = FALSE;
		}
	}

	return draw_messages;
}

boolean game_engine_force_autopickup(
	long unit_index,
	long weapon_index)
{
	boolean force_autopickup = FALSE;

	if (game_engine && global_variant.game_engine_index==1 && weapon_is_flag(weapon_index))
		force_autopickup = TRUE;

	return force_autopickup;
}

boolean game_engine_allow_pick_up(
	long unit_index,
	long weapon_index)
{
	boolean allow_pick_up = TRUE;

	if (game_engine && game_engine->allow_pick_up)
		allow_pick_up = game_engine->allow_pick_up(unit_index, weapon_index);

	return allow_pick_up;
}

boolean game_engine_picking_up(
	long unit_index,
	long weapon_index)
{
	boolean allow_pick_up = TRUE;

	if (game_engine)
	{
		struct weapon_datum *weapon = weapon_try_and_get(weapon_index);

		if (weapon && weapon_is_flag(weapon_index))
		{
			if (TEST_FLAG(weapon->weapon.flags, _weapon_runtime_game_engine_active_bit))
			{
				SET_FLAG(weapon->weapon.flags, _weapon_runtime_game_engine_active_bit, FALSE);

				if (game_engine->weapon_dropped)
					game_engine->weapon_dropped(weapon_index);
			}

			SET_FLAG(weapon->weapon.flags, _weapon_runtime_game_engine_active_bit, TRUE);

			if (game_engine->picking_up)
			{
				long player_index = player_index_from_unit_index(unit_index);

				allow_pick_up = game_engine->picking_up(
					weapon_index,
					player_index);
			}

			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine.c",
				0xF58,
				!allow_pick_up ||
					!TEST_FLAG(weapon->weapon.flags, _weapon_must_be_readied_bit) ||
					!unit_has_weapon_with_flag(unit_index, _weapon_must_be_readied_bit));
		}
	}

	return allow_pick_up;
}

boolean game_engine_test_flag(
	long flag)
{
	boolean result = FALSE;

	if (game_engine && game_engine->test_flag)
		result = game_engine->test_flag(flag);

	return result;
}

boolean game_engine_test_trait(
	long trait,
	long value)
{
	boolean result = FALSE;

	if (game_engine && game_engine->test_trait)
		result = game_engine->test_trait(trait, value);

	return result;
}

void game_engine_prespawn_player_update(
	long player_index)
{
	if (game_engine)
	{
		if (game_engine->prespawn_player_update)
			game_engine->prespawn_player_update(player_index);
		else
		{
			struct player_datum *player = player_get(player_index);
			player->team_index = player->local_player_index % 2;
		}
	}

	return;
}

static struct postgame_statistic_entry *game_engine_get_player_place(
	struct postgame_statistic_entry *entry,
	long player_index)
{
#ifdef HALO_LINUX
	/* port: populate_statistic_buffer writes an entry per player */
	struct postgame_statistic_entry entries[MULTIPLAYER_MAXIMUM_PLAYERS];
#else
	struct postgame_statistic_entry entries[16];
#endif
	long place = 0;

	populate_statistic_buffer(entries, 0, 0);
	if (entries[place].values[0] != player_index)
	{
		do
		{
			place++;
			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine.c",
				0x362,
				place<MULTIPLAYER_MAXIMUM_PLAYERS);
		}
		while (entries[place].values[0] != player_index);
	}

	*entry = entries[place];

	return entry;
}

struct game_variant *build_game_variant_king(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.respawn_time = 150;
	result.universal_variant.score_to_win = 2;
	result.flags = 1;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_index = 4;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.king.moving_hill = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_crazy_king(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.score_to_win = 2;
	result.game_engine_variant.king.moving_hill = 1;
	result.universal_variant.vehicle_set = 2;
	result.flags = 1;
	result.game_engine_index = 4;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_slayer(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.game_engine_index = 2;
	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.vehicle_set = 2;
	result.universal_variant.goal_radar = 0;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 15;
	result.universal_variant.suicide_penalty = 300;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.slayer.no_death_bonus = 0;
	result.game_engine_variant.slayer.no_kill_penalty = 0;
	result.game_engine_variant.slayer.kill_in_order = 0;
	result.flags = 1;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_team_slayer(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.respawn_time = 300;
	result.universal_variant.suicide_penalty = 300;
	result.game_engine_index = 2;
	result.universal_variant.teams = TRUE;
	result.universal_variant.vehicle_set = 2;
	result.flags = 1;
	result.universal_variant.goal_radar = 0;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 50;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.slayer.no_death_bonus = 0;
	result.game_engine_variant.slayer.no_kill_penalty = 0;
	result.game_engine_variant.slayer.kill_in_order = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_elimination(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.game_engine_index = 2;
	result.universal_variant.lives = 1;
	result.universal_variant.vehicle_set = 2;
	result.flags = 1;
	result.universal_variant.goal_radar = 0;
	result.universal_variant.health = 1.0f;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 25;
	result.universal_variant.suicide_penalty = 300;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.slayer.no_death_bonus = 0;
	result.game_engine_variant.slayer.no_kill_penalty = 0;
	result.game_engine_variant.slayer.kill_in_order = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_phantoms(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x2D) | 0x12;
	result.game_engine_index = 2;
	result.universal_variant.respawn_time = 150;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.vehicle_set = 2;
	result.universal_variant.suicide_penalty = 150;
	result.game_engine_variant.slayer.no_death_bonus = 1;
	result.game_engine_variant.slayer.no_kill_penalty = 1;
	result.game_engine_variant.slayer.kill_in_order = 1;
	result.flags = 1;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 10;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_endurance(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.game_engine_index = 2;
	result.universal_variant.odd_man_out = TRUE;
	result.universal_variant.respawn_time_growth = 300;
	result.universal_variant.vehicle_set = 2;
	result.flags = 1;
	result.universal_variant.suicide_penalty = 300;
	result.universal_variant.goal_radar = 0;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 5;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.score_to_win = 10;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.slayer.no_death_bonus = 0;
	result.game_engine_variant.slayer.no_kill_penalty = 0;
	result.game_engine_variant.slayer.kill_in_order = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_rockets(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x1D) | 0x22;
	result.game_engine_index = 2;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.vehicle_set = 2;
	result.flags = 1;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 25;
	result.universal_variant.suicide_penalty = 300;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 6;
	result.game_engine_variant.slayer.no_death_bonus = 0;
	result.game_engine_variant.slayer.no_kill_penalty = 0;
	result.game_engine_variant.slayer.kill_in_order = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_snipers(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x1D) | 0x22;
	result.game_engine_index = 2;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.vehicle_set = 2;
	result.flags = 1;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 150;
	result.universal_variant.score_to_win = 15;
	result.universal_variant.suicide_penalty = 300;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 4;
	result.game_engine_variant.slayer.no_death_bonus = 0;
	result.game_engine_variant.slayer.no_kill_penalty = 0;
	result.game_engine_variant.slayer.kill_in_order = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_oddball(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.respawn_time = 150;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.vehicle_set = 1;
	result.game_engine_variant.oddball.ball_spawn_count = 1;
	result.game_engine_variant.oddball.ball_spawn_delay = 1;
	result.flags = 1;
	result.game_engine_index = 3;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 2;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.oddball.random_start = 0;
	result.game_engine_variant.oddball.oddball_ball_type = 0;
	result.game_engine_variant.oddball.trait_with_ball = 0;
	result.game_engine_variant.oddball.trait_without_ball = 0;
	result.game_engine_variant.oddball.speed_with_ball = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_team_oddball(
	struct game_variant *variant)
{
	struct game_variant result;

	result.universal_variant.flags = (result.universal_variant.flags & ~0x1C) | 0x23;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.lives = 0;
	result.universal_variant.teams = TRUE;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.vehicle_set = 1;
	result.universal_variant.respawn_time_growth = 0;
	result.game_engine_variant.oddball.ball_spawn_count = 1;
	result.universal_variant.weapon_set = 0;
	result.flags = 1;
	result.game_engine_variant.oddball.ball_spawn_delay = 0;
	result.game_engine_variant.oddball.random_start = 0;
	result.game_engine_variant.oddball.oddball_ball_type = 0;
	result.game_engine_variant.oddball.trait_with_ball = 0;
	result.game_engine_variant.oddball.trait_without_ball = 0;
	result.game_engine_variant.oddball.speed_with_ball = 0;
	result.game_engine_index = 3;
	result.universal_variant.health = 1.0f;
	result.universal_variant.respawn_time = 300;
	result.universal_variant.score_to_win = 2;
	result.universal_variant.suicide_penalty = 150;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_reverse_tag(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3D) | 2;
	result.universal_variant.respawn_time = 150;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.vehicle_set = 1;
	result.game_engine_variant.oddball.ball_spawn_count = 1;
	result.game_engine_variant.oddball.ball_spawn_delay = 1;
	result.game_engine_variant.oddball.oddball_ball_type = 1;
	result.flags = 1;
	result.game_engine_index = 3;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 2;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.oddball.random_start = 0;
	result.game_engine_variant.oddball.trait_with_ball = 0;
	result.game_engine_variant.oddball.trait_without_ball = 0;
	result.game_engine_variant.oddball.speed_with_ball = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_accumulation(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3D) | 2;
	result.universal_variant.respawn_time = 150;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.vehicle_set = 1;
	result.game_engine_variant.oddball.oddball_ball_type = 1;
	result.flags = 1;
	result.game_engine_index = 3;
	result.universal_variant.goal_radar = 2;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 5;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.oddball.ball_spawn_count = 16;
	result.game_engine_variant.oddball.ball_spawn_delay = 0;
	result.game_engine_variant.oddball.random_start = 0;
	result.game_engine_variant.oddball.trait_with_ball = 0;
	result.game_engine_variant.oddball.trait_without_ball = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_juggernaut(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.respawn_time = 150;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_variant.oddball.ball_spawn_count = 1;
	result.game_engine_variant.oddball.trait_with_ball = 2;
	result.flags = 1;
	result.game_engine_variant.oddball.oddball_ball_type = 2;
	result.game_engine_index = 3;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 10;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.oddball.ball_spawn_delay = 0;
	result.game_engine_variant.oddball.random_start = 0;
	result.game_engine_variant.oddball.trait_without_ball = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_stalker(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3D) | 2;
	result.universal_variant.respawn_time = 150;
	result.universal_variant.suicide_penalty = 150;
	result.game_engine_variant.oddball.ball_spawn_count = 1;
	result.game_engine_index = 3;
	result.game_engine_variant.oddball.trait_with_ball = 1;
	result.game_engine_variant.oddball.trait_without_ball = 3;
	result.flags = 1;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_variant.oddball.oddball_ball_type = 2;
	result.game_engine_variant.oddball.speed_with_ball = 2;
	result.universal_variant.goal_radar = 0;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 10;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.oddball.ball_spawn_delay = 0;
	result.game_engine_variant.oddball.random_start = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_king_pro(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x1C) | 0x23;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.score_to_win = 2;
	result.flags = 1;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_index = 4;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 300;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.suicide_penalty = 450;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.king.moving_hill = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_team_king(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.score_to_win = 2;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.vehicle_set = 2;
	result.universal_variant.teams = TRUE;
	result.game_engine_variant.king.moving_hill = 1;
	result.flags = 1;
	result.game_engine_index = 4;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 300;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.weapon_set = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_slayer_pro(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.game_engine_index = 2;
	result.universal_variant.flags = (result.universal_variant.flags & ~0x1C) | 0x23;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_variant.slayer.no_death_bonus = 1;
	result.game_engine_variant.slayer.no_kill_penalty = 1;
	result.flags = 1;
	result.universal_variant.goal_radar = 0;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 25;
	result.universal_variant.suicide_penalty = 450;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.slayer.kill_in_order = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_ctf(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.game_engine_index = 1;
	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.teams = TRUE;
	result.flags = 1;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 300;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 3;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.weapon_set = 0;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_variant.ctf.assault = 0;
	result.game_engine_variant.ctf.flag_at_home_to_score = 0;
	result.game_engine_variant.ctf.flag_must_reset = 0;
	result.game_engine_variant.ctf.reset_on_capture = 0;
	result.game_engine_variant.ctf.single_flag_time = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_ctf_pro(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.game_engine_index = 1;
	result.universal_variant.flags = (result.universal_variant.flags & ~0x1C) | 0x23;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.teams = TRUE;
	result.game_engine_variant.ctf.flag_at_home_to_score = 1;
	result.flags = 1;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 300;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 3;
	result.universal_variant.suicide_penalty = 450;
	result.universal_variant.weapon_set = 0;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_variant.ctf.assault = 0;
	result.game_engine_variant.ctf.flag_must_reset = 0;
	result.game_engine_variant.ctf.reset_on_capture = 0;
	result.game_engine_variant.ctf.single_flag_time = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_invasion(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.game_engine_index = 1;
	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.teams = TRUE;
	result.game_engine_variant.ctf.assault = 1;
	result.flags = 1;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 5;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 3;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.weapon_set = 0;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_variant.ctf.flag_at_home_to_score = 0;
	result.game_engine_variant.ctf.flag_must_reset = 0;
	result.game_engine_variant.ctf.reset_on_capture = 0;
	result.game_engine_variant.ctf.single_flag_time = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_iron_ctf(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.game_engine_index = 1;
	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.teams = TRUE;
	result.game_engine_variant.ctf.flag_must_reset = 1;
	result.flags = 1;
	result.universal_variant.health = 2.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 450;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 3;
	result.universal_variant.suicide_penalty = 150;
	result.universal_variant.weapon_set = 0;
	result.universal_variant.vehicle_set = 4;
	result.game_engine_variant.ctf.assault = 0;
	result.game_engine_variant.ctf.flag_at_home_to_score = 0;
	result.game_engine_variant.ctf.reset_on_capture = 0;
	result.game_engine_variant.ctf.single_flag_time = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_race(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.goal_radar = 1;
	result.flags = 1;
	result.game_engine_index = 5;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 3;
	result.universal_variant.suicide_penalty = 300;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_variant.race.race_type = 0;
	result.game_engine_variant.race.team_scoring = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_rally(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_variant.race.race_type = 2;
	result.flags = 1;
	result.game_engine_index = 5;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 15;
	result.universal_variant.suicide_penalty = 300;
	result.universal_variant.teams = FALSE;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.race.team_scoring = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_team_race(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.teams = TRUE;
	result.flags = 1;
	result.game_engine_index = 5;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.score_to_win = 3;
	result.universal_variant.suicide_penalty = 300;
	result.universal_variant.weapon_set = 0;
	result.universal_variant.vehicle_set = 2;
	result.game_engine_variant.race.race_type = 0;
	result.game_engine_variant.race.team_scoring = 0;

	*variant = result;

	return variant;
}

struct game_variant *build_game_variant_team_rally(
	struct game_variant *variant)
{
	struct game_variant result = { 0 };

	result.game_engine_index = 5;
	result.universal_variant.flags = (result.universal_variant.flags & ~0x3C) | 3;
	result.universal_variant.goal_radar = 1;
	result.universal_variant.score_to_win = 5;
	result.universal_variant.teams = TRUE;
	result.universal_variant.vehicle_set = 2;
	result.flags = 1;
	result.game_engine_variant.race.race_type = 2;
	result.universal_variant.health = 1.0f;
	result.universal_variant.lives = 0;
	result.universal_variant.odd_man_out = FALSE;
	result.universal_variant.respawn_time = 0;
	result.universal_variant.respawn_time_growth = 0;
	result.universal_variant.suicide_penalty = 300;
	result.universal_variant.weapon_set = 0;
	result.game_engine_variant.race.team_scoring = 0;

	*variant = result;

	return variant;
}

long postgame_statistic_get_rating(
	long player_index,
	long parameter1,
	long parameter2)
{
#ifdef HALO_LINUX
	/* port: populate_statistic_buffer writes an entry per player */
	struct postgame_statistic_entry entries[MULTIPLAYER_MAXIMUM_PLAYERS];
#else
	struct postgame_statistic_entry entries[16];
#endif
	long rating;
	long entry_count;
	long entry_index;

	entry_count = populate_statistic_buffer(entries, parameter1, parameter2);
	rating = 0;
	if (entries[0].values[0] != player_index)
	{
		for (entry_index = 1; entry_index < entry_count; entry_index++)
		{
			if (entries[entry_index - 1].values[1] != entries[entry_index].values[1])
				rating++;

			if (entries[entry_index].values[0] == player_index)
				break;
		}
	}

	return rating;
}

long game_engine_did_player_win(
	long player_index)
{
	long result = 0;

	if (game_engine)
	{
		if (game_engine->did_player_win)
			result = game_engine->did_player_win(player_index);
		else
			result = game_engine_did_player_win_default(player_index);
	}

	return result;
}

boolean multiple_teams_alive(
	void)
{
	boolean result = FALSE;

	if (players_in_game() <= 1)
		return TRUE;
	else
	{
		struct data_iterator iterator;
		struct player_datum *player;
		long team_index = NONE;

		data_iterator_new(&iterator, player_data);
		player = (struct player_datum *)data_iterator_next(&iterator);
		while (player)
		{
			if (!player->quit_out_of_game &&
				(player->unit_index != NONE ||
					(!game_engine_player_is_odd_man_out(iterator.datum_index) &&
						!game_engine_player_is_out_of_lives(
							iterator.datum_index))))
			{
				match_assert(
					"c:\\halo\\SOURCE\\game\\game_engine.c",
					0x1BE,
					player->team_index != NONE);

				if (player->team_index != team_index)
				{
					if (team_index != NONE)
					{
						result = TRUE;
						break;
					}

					team_index = player->team_index;
				}
			}

			player = (struct player_datum *)data_iterator_next(&iterator);
		}
	}

	return result;
}

boolean team_has_players(
	long team_index)
{
	boolean result = FALSE;

	if (players_in_game() <= 1)
	{
		result = TRUE;
		goto done;
	}

	{
		struct data_iterator iterator;
		struct player_datum *player;

		data_iterator_new(&iterator, player_data);
		player = (struct player_datum *)data_iterator_next(&iterator);
		while (player)
		{
			if (!player->quit_out_of_game &&
				(player->unit_index != NONE ||
					(!game_engine_player_is_odd_man_out(iterator.datum_index) &&
						!game_engine_player_is_out_of_lives(
							iterator.datum_index))))
			{
				match_assert(
					"c:\\halo\\SOURCE\\game\\game_engine.c",
					0x1F7,
					player->team_index != NONE);

				/* Preserve the retail first-eligible-player behavior. */
				if (player->team_index == team_index)
					result = TRUE;

				break;
			}

			player = (struct player_datum *)data_iterator_next(&iterator);
		}
	}

done:
	return result;
}

long game_engine_did_player_win_default(
	long player_index)
{
	long result;

	if (global_variant.universal_variant.teams)
	{
		long team0_score = game_engine_get_team_score(0);
		long team1_score = game_engine_get_team_score(1);
		struct player_datum *player = player_get(player_index);

		if (multiple_teams_alive())
		{
			if (team0_score == team1_score)
				goto tied;

			result = team0_score <= team1_score;
		}
		else
		{
			result = !team_has_players(0);
		}

		if (result != NONE)
			result = player->team_index == result;
		else
			goto tied;
	}
	else
	{
		struct postgame_statistic_entry entry;

		entry = *game_engine_get_player_place(&entry, player_index);
		if (is_place_tied(&entry) &&
			!place_get_position(&entry))
			goto tied;
		else
			result = place_get_position(&entry) == 0;
	}

	return result;

tied:
	return NONE;
}

static long game_engine_get_type(
	void)
{
	long game_engine_type = NONE;

	if (game_engine)
		game_engine_type = game_engine->type;

	return game_engine_type;
}

struct game_variant *game_engine_get_variant(
	void)
{
	return &global_variant;
}

struct game_variant *game_engine_get_variant_by_name(
	struct game_variant *variant,
	char const *name)
{
	struct game_variant temporary;
	struct game_variant result;

	csmemset(&result, 0, sizeof(result));

	if (csstrcmp(name, "race") == 0)
		result = *build_game_variant_race(&temporary);
	else if (csstrcmp(name, "team_race") == 0)
		result = *build_game_variant_team_race(&temporary);
	else if (csstrcmp(name, "rally") == 0)
		result = *build_game_variant_rally(&temporary);
	else if (csstrcmp(name, "slayer") == 0)
		result = *build_game_variant_slayer(&temporary);
	else if (csstrcmp(name, "team_slayer") == 0)
		result = *build_game_variant_team_slayer(&temporary);
	else if (csstrcmp(name, "elimination") == 0)
		result = *build_game_variant_elimination(&temporary);
	else if (csstrcmp(name, "stalker") == 0)
		result = *build_game_variant_stalker(&temporary);
	else if (csstrcmp(name, "team_oddball") == 0)
		result = *build_game_variant_team_oddball(&temporary);
	else if (csstrcmp(name, "accumulation") == 0)
		result = *build_game_variant_accumulation(&temporary);
	else if (csstrcmp(name, "oddball") == 0)
		result = *build_game_variant_oddball(&temporary);
	else if (csstrcmp(name, "ctf") == 0)
		result = *build_game_variant_ctf(&temporary);
	else if (csstrcmp(name, "ironctf") == 0)
		result = *build_game_variant_iron_ctf(&temporary);
	else if (csstrcmp(name, "king") == 0)
		result = *build_game_variant_king(&temporary);
	else if (csstrcmp(name, "team_king") == 0)
		result = *build_game_variant_team_king(&temporary);

	*variant = result;

	return variant;
}

boolean game_engine_get_goal_in_use(
	short goal_index)
{
	return global_goal[goal_index].in_use;
}

real_point3d *game_engine_get_goal_position(
	real_point3d *position,
	short index)
{
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0xFE2,
		global_goal[index].in_use);
	*position = global_goal[index].position;

	return position;
}

void game_engine_set_goal_position(
	short goal_index,
	real_point3d const *position,
	real height,
	char const *name,
	long target_object_index,
	short team_index,
	long player_index)
{
	global_goal[goal_index].player_index = player_index;
	global_goal[goal_index].nav_point_index = find_nav_point(name);
	global_goal[goal_index].in_use = TRUE;
	global_goal[goal_index].position = *position;
	global_goal[goal_index].team_index = team_index;
	global_goal[goal_index].position.z += height + 0.63f;
	global_goal[goal_index].target_object_index = target_object_index;

	return;
}

boolean game_engine_man_out(
	long player_index)
{
	struct player_datum *player = player_get(player_index);

	if (player->quit_out_of_game)
		goto man_out;

	if (global_variant.universal_variant.lives>0)
	{
		player = player_get(player_index);
		if (player->unit_index==NONE &&
			player->statistics.deaths>=global_variant.universal_variant.lives)
		{
			goto man_out;
		}
	}

	if (!game_engine_player_is_odd_man_out(player_index))
		return FALSE;

man_out:
	return TRUE;
}

real_rgb_color *game_engine_player_get_change_color(
	real_rgb_color *change_color,
	long player_index)
{
	struct player_datum *player = player_get(player_index);
	real_rgb_color result;

	if (global_variant.universal_variant.teams)
	{
		if (player->team_index == 0)
			result = *global_real_rgb_red;
		else
			result = *global_real_rgb_blue;
	}
	else
	{
		long color_index = player->network_player_data.primary_color_index;

		if (debug_player_color != NONE)
			color_index = debug_player_color;

		result = player_profile_get_rgb_color(color_index);
	}

	*change_color = result;

	return change_color;
}

boolean game_engine_has_teams(
	void)
{
	boolean has_teams = FALSE;

	if (game_engine)
		has_teams = global_variant.universal_variant.teams;

	return has_teams;
}

boolean game_engine_allow_pause(
	void)
{
	return game_engine_globals.postgame_state==0;
}

boolean game_engine_allow_dynamic_lighting(
	void)
{
	boolean allow_dynamic_lighting = TRUE;

	if (game_engine)
		allow_dynamic_lighting = !TEST_FLAG(game_engine_globals.flags, _game_engine_allow_dynamic_lighting_bit);

	return allow_dynamic_lighting;
}

boolean game_engine_allow_integrated_lights(
	long object_index)
{
	boolean allow_integrated_lights = TRUE;

	if (game_engine)
		allow_integrated_lights = !TEST_FLAG(game_engine_globals.flags, _game_engine_allow_integrated_lights_bit);

	return allow_integrated_lights;
}

void game_engine_variant_cleanup(
	struct game_variant *variant)
{
	struct game_variant original = *variant;

	variant->human_readable_game_description[NUMBEROF(variant->human_readable_game_description) - 1] = 0;
	variant->game_engine_index = PIN(variant->game_engine_index, 1, 5);
	variant->universal_variant.teams = !!variant->universal_variant.teams;
	variant->universal_variant.odd_man_out = !!variant->universal_variant.odd_man_out;
	variant->universal_variant.respawn_time_growth = MAX(variant->universal_variant.respawn_time_growth, 0);
	variant->universal_variant.respawn_time = MAX(variant->universal_variant.respawn_time, 0);
	variant->universal_variant.suicide_penalty = MAX(variant->universal_variant.suicide_penalty, 0);
	variant->universal_variant.lives = MAX(variant->universal_variant.lives, 0);
	variant->universal_variant.health = PIN(variant->universal_variant.health, 0.25f, 4.0f);
	variant->universal_variant.weapon_set = PIN(variant->universal_variant.weapon_set, 0, 10);
	variant->universal_variant.vehicle_set = PIN(variant->universal_variant.vehicle_set, 0, 4);

	switch (variant->game_engine_index)
	{
	case 1:
		variant->game_engine_variant.ctf.assault = !!variant->game_engine_variant.ctf.assault;
		variant->game_engine_variant.ctf.reset_on_capture = !!variant->game_engine_variant.ctf.reset_on_capture;
		variant->game_engine_variant.ctf.flag_must_reset = !!variant->game_engine_variant.ctf.flag_must_reset;
		variant->universal_variant.teams = TRUE;
		variant->game_engine_variant.ctf.flag_at_home_to_score = !!variant->game_engine_variant.ctf.flag_at_home_to_score;
		variant->game_engine_variant.ctf.single_flag_time = FLOOR(variant->game_engine_variant.ctf.single_flag_time, 0);
		break;

	case 2:
		variant->game_engine_variant.slayer.no_death_bonus = !!variant->game_engine_variant.slayer.no_death_bonus;
		variant->game_engine_variant.slayer.no_kill_penalty = !!variant->game_engine_variant.slayer.no_kill_penalty;
		variant->game_engine_variant.slayer.kill_in_order = !!variant->game_engine_variant.slayer.kill_in_order;
		break;
	}

	if (csmemcmp(&original, variant, sizeof(original)) != 0)
	{
		struct game_variant cleaned;

		csmemcpy(&cleaned, &original, sizeof(cleaned));
		csmemcpy(&cleaned, variant, sizeof(cleaned));
		error(
			_error_silent,
			"NETGAME CODE FAILURE: game_engine_variant_cleanup changed the variant");
	}

	return;
}

static void game_engine_predict_resources(
	void)
{
	struct game_globals *game_globals;
	struct game_globals_multiplayer_information *multiplayer_information;
	struct game_globals_vehicle *vehicle;
	struct tag_reference *weapon;
	long weapon_indices[10];
	long weapon_index;

	game_globals = scenario_get_game_globals();
	multiplayer_information = TAG_BLOCK_GET_ELEMENT(
		&game_globals->multiplayer_information,
		0,
		struct game_globals_multiplayer_information);

	switch (global_variant.universal_variant.vehicle_set)
	{
	case 2:
		vehicle = TAG_BLOCK_GET_ELEMENT(
			&multiplayer_information->vehicles,
			0,
			struct game_globals_vehicle);
		object_definition_predict(vehicle->vehicle.index);
		break;

	case 3:
		vehicle = TAG_BLOCK_GET_ELEMENT(
			&multiplayer_information->vehicles,
			1,
			struct game_globals_vehicle);
		object_definition_predict(vehicle->vehicle.index);
		break;

	case 4:
		vehicle = TAG_BLOCK_GET_ELEMENT(
			&multiplayer_information->vehicles,
			2,
			struct game_globals_vehicle);
		object_definition_predict(vehicle->vehicle.index);
		break;

	default:
	{
		struct tag_block *vehicles = &multiplayer_information->vehicles;

		vehicle = TAG_BLOCK_GET_ELEMENT(
			vehicles,
			0,
			struct game_globals_vehicle);
		object_definition_predict(vehicle->vehicle.index);
		vehicle = TAG_BLOCK_GET_ELEMENT(
			vehicles,
			1,
			struct game_globals_vehicle);
		object_definition_predict(vehicle->vehicle.index);
		vehicle = TAG_BLOCK_GET_ELEMENT(
			vehicles,
			2,
			struct game_globals_vehicle);
		object_definition_predict(vehicle->vehicle.index);
		break;
	}
	}

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		12,
		struct tag_reference);
	object_definition_predict(weapon->index);

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		13,
		struct tag_reference);
	object_definition_predict(weapon->index);

	if (global_variant.game_engine_index == 3)
	{
		game_globals = scenario_get_game_globals();
		weapon = TAG_BLOCK_GET_ELEMENT(
			&game_globals->weapon_list,
			10,
			struct tag_reference);
		object_definition_predict(weapon->index);
	}

	if (global_variant.game_engine_index == 1)
	{
		game_globals = scenario_get_game_globals();
		weapon = TAG_BLOCK_GET_ELEMENT(
			&game_globals->weapon_list,
			11,
			struct tag_reference);
		object_definition_predict(weapon->index);
	}

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		0,
		struct tag_reference);
	weapon_indices[0] = weapon->index;

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		1,
		struct tag_reference);
	weapon_indices[1] = weapon->index;

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		2,
		struct tag_reference);
	weapon_indices[2] = weapon->index;

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		3,
		struct tag_reference);
	weapon_indices[3] = weapon->index;

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		4,
		struct tag_reference);
	weapon_indices[4] = weapon->index;

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		5,
		struct tag_reference);
	weapon_indices[5] = weapon->index;

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		6,
		struct tag_reference);
	weapon_indices[6] = weapon->index;

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		7,
		struct tag_reference);
	weapon_indices[7] = weapon->index;

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		8,
		struct tag_reference);
	weapon_indices[8] = weapon->index;

	game_globals = scenario_get_game_globals();
	weapon = TAG_BLOCK_GET_ELEMENT(
		&game_globals->weapon_list,
		9,
		struct tag_reference);
	weapon_indices[9] = weapon->index;

	weapon_index = 0;
	while (weapon_index < 10)
	{
		object_definition_predict(
			game_engine_remap_weapon(weapon_indices[weapon_index]));
		weapon_index++;
	}

	return;
}

void game_engine_initialize(
	struct game_variant *variant)
{
	csmemset(&game_engine_globals, 0, sizeof(game_engine_globals));
	game_engine_globals.postgame_state = 0;

	if (variant && variant->game_engine_index)
	{
		global_variant = *variant;
		game_engine_variant_cleanup(&global_variant);
		game_engine = game_engines[variant->game_engine_index];
	}

	return;
}

void game_engine_initialize_for_new_map(
	void)
{
	if (game_engine)
	{
		game_engine_verify_current_map();
		game_engine_intialize_queued_sounds();
		csmemset(global_goal, 0, sizeof(global_goal));
		game_engine_globals.next_team_index = 0;
		timeout_for_endgame_sound = 0;

		if (game_engine->initialize_for_new_map &&
			!game_engine->initialize_for_new_map())
		{
			error(
				_error_silent,
				"failed to initialize custome game engine for new map, reverting to default game engine");

			if (game_engine)
			{
				if (game_engine->dispose)
					game_engine->dispose();

				game_engine = NULL;
			}
		}

		game_engine_predict_resources();
	}

	return;
}

void game_engine_player_added(
	long player_index)
{
	initialize_player_multiplayer_data(player_index);

	if (game_engine)
	{
		struct player_datum *player = player_get(player_index);
		long *next_team_index = &game_engine_globals.next_team_index;

		if (global_variant.universal_variant.teams)
		{
#ifdef HALO_N64
			/* the team the N64's menu chose (main.c create_local_players),
			or n64_bots.c its bots': not dealt out in turn */
			player->team_index =
				(signed char)(player->network_player_data.team_index & 1);
#else
			if (global_network_game_client_get())
			{
				player->team_index =
					(signed char)player->network_player_data.team_index % 2;
			}
			else
			{
				player->network_player_data.team_index =
					(char)*next_team_index;
				player->team_index =
					(signed char)*next_team_index;
				*next_team_index = (*next_team_index + 1) % 2;
			}
#endif
		}
		else
		{
#ifdef HALO_LINUX
			/* port: a free-for-all team is the player's own slot, which stays
			below the player limit and is the same on every machine */
			long team_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index);

			player->network_player_data.team_index =
				(char)team_index;
			player->team_index =
				(signed char)team_index;
#else
			player->network_player_data.team_index =
				(char)*next_team_index;
			player->team_index =
				(signed char)*next_team_index;
			(*next_team_index)++;
#endif
		}

		if (player_index != NONE)
		{
			multiplayer_message(player_index, 0, NONE);
		}
		else
		{
			struct data_iterator iterator;

			data_iterator_new(&iterator, player_data);
			while (data_iterator_next(&iterator))
				multiplayer_message(iterator.datum_index, 0, NONE);
		}

		if (game_engine->player_added)
			game_engine->player_added(player_index);
	}

	return;
}

real game_engine_get_distance_rating_for_spawn(
	long player_index,
	real_point3d const *position)
{
	boolean has_teams = game_engine ? global_variant.universal_variant.teams : FALSE;
	struct player_datum *player;
	struct data_iterator iterator;
	struct player_datum *other_player;
	real rating;

	player = player_get(player_index);
	rating = 1.0f;
	data_iterator_new(&iterator, player_data);
	other_player = (struct player_datum *)data_iterator_next(&iterator);
	while (other_player)
	{
		if (other_player->unit_index!=NONE)
		{
			real_point3d origin;
			real distance;

			object_get_origin(other_player->unit_index, &origin);
			distance = distance3d(&origin, position);

			if (!has_teams ||
				other_player->team_index!=player->team_index ||
				!(distance>0.25f))
			{
				if (distance<0.25f)
					rating = 0.0f;
				else if (distance<1.0f)
					rating *= 0.1f;

				if (other_player->team_index!=player->team_index)
				{
					if (distance<2.0f)
						rating = 0.0f;
					else if (!(distance>5.0f))
						rating = (distance-2.0f)*rating*0.33333334f;
				}
			}
		}

		other_player = (struct player_datum *)data_iterator_next(&iterator);
	}

	return rating;
}


real game_engine_get_starting_location_rating(
	long player_index,
	struct player_starting_location const *starting_location)
{
	/* NonMatching with the same EBX/EDI mirror as code_0009c460. */
	if (!match_game_type(game_engine_get_type(), 4, starting_location->game_types))
		return 0.0f;

	if (nearby_vehicle(player_index, starting_location))
		return 0.0f;

	return default_starting_location_rate_function(player_index, starting_location);
}

real game_engine_get_damage_multiplier(
	long damaging_player_index,
	long damaged_player_index)
{
	real result = 1.0f;

	if (game_engine)
		result /= PIN(global_variant.universal_variant.health, 0.25f, 4.0f);

	if (damaging_player_index != NONE &&
		damaged_player_index != NONE)
	{
		if (game_engine_test_trait(damaging_player_index, 2))
		{
			result *= 1.5f;
		}

		if (game_engine_test_trait(damaged_player_index, 3))
		{
			result *= 0.5f;
		}
	}

	return result;
}

long game_engine_remap_vehicle(
	long vehicle_definition_index)
{
	long result = vehicle_definition_index;

	if (game_engine)
	{
		struct game_globals *game_globals;
		struct game_globals_multiplayer_information *multiplayer_information;
		struct tag_block *vehicles;
		struct game_globals_vehicle *vehicle0;
		struct game_globals_vehicle *vehicle1;
		struct game_globals_vehicle *vehicle2;
		struct game_globals_vehicle *vehicle;

		game_globals = scenario_get_game_globals();
		multiplayer_information = TAG_BLOCK_GET_ELEMENT(
			&game_globals->multiplayer_information,
			0,
			struct game_globals_multiplayer_information);
		vehicle0 = TAG_BLOCK_GET_ELEMENT(
			&multiplayer_information->vehicles,
			0,
			struct game_globals_vehicle);
		vehicles = &multiplayer_information->vehicles;
		vehicle1 = TAG_BLOCK_GET_ELEMENT(vehicles, 1, struct game_globals_vehicle);
		vehicle2 = TAG_BLOCK_GET_ELEMENT(vehicles, 2, struct game_globals_vehicle);

		if (result != vehicle0->vehicle.index &&
			result != vehicle1->vehicle.index &&
			result != vehicle2->vehicle.index)
		{
#ifdef HALO_N64
			/* Keep every authored PC vehicle in the globals palette, including
			Banshee, turret and Rocket Warthog. Xbox palettes remain unchanged. */
			short i;
			result = NONE;
			for (i = 3; i < vehicles->count; i++)
			{
				vehicle = TAG_BLOCK_GET_ELEMENT(vehicles, i, struct game_globals_vehicle);
				if (vehicle->vehicle.index == vehicle_definition_index)
				{
					result = vehicle_definition_index;
					break;
				}
			}
#else
			result = NONE;
#endif
		}

		switch (global_variant.universal_variant.vehicle_set)
		{
		case 1:
			result = NONE;
			break;

		case 2:
			vehicle = TAG_BLOCK_GET_ELEMENT(
				vehicles,
				0,
				struct game_globals_vehicle);
			if (vehicle->vehicle.index != result)
				result = NONE;
			break;

		case 3:
			vehicle = TAG_BLOCK_GET_ELEMENT(
				vehicles,
				1,
				struct game_globals_vehicle);
			if (vehicle->vehicle.index != result)
				result = NONE;
			break;

		case 4:
			vehicle = TAG_BLOCK_GET_ELEMENT(
				vehicles,
				2,
				struct game_globals_vehicle);
			if (vehicle->vehicle.index != result)
				result = NONE;
			break;
		}
	}

#ifdef HALO_N64
	{
		extern long n64_game_vehicle_override(long definition_index);
		result = n64_game_vehicle_override(result);
	}
#endif
	return result;
}

long game_engine_remap_weapon(
	long weapon_definition_index)
{
	long weapon_list_index =
		weapon_definition_index_to_list_index(weapon_definition_index);

#ifdef HALO_N64
	/* Xbox's standard list treats its unused PC weapon slot as a rocket
	launcher. The expansion contains the real PC weapon; retain it for
	the normal weapon set and its explicit starting loadouts. */
	if (weapon_definition_index != NONE && global_variant.universal_variant.weapon_set == 0)
	{
		char const *name = tag_get_name(weapon_definition_index);
		if (!strcmp(name, "weapons\\flamethrower\\flamethrower") ||
			!strcmp(name, "weapons\\plasma_cannon\\plasma_cannon"))
			return weapon_definition_index;
	}
#endif

	if (weapon_list_index == 10 ||
		weapon_list_index == 11 ||
		weapon_list_index == NONE)
	{
		return weapon_definition_index;
	}

	if (weapon_list_index == 1)
		weapon_list_index = 7;

	switch (global_variant.universal_variant.weapon_set)
	{
	case 1:
		switch (weapon_list_index)
		{
		case 3:
		case 4:
		case 6:
		case 7:
			weapon_list_index = 5;
			break;

		case 5:
		default:
			weapon_list_index = 4;
			break;
		}
		break;

	case 2:
		switch (weapon_list_index)
		{
		case 3:
		case 4:
		case 6:
		case 7:
			weapon_list_index = 6;
			break;

		case 5:
		default:
			weapon_list_index = 0;
			break;
		}
		break;

	case 3:
		if (weapon_list_index >= 3 && weapon_list_index <= 5)
			weapon_list_index = 5;
		else
			weapon_list_index = 6;
		break;

	case 4:
		if (weapon_list_index != 4 && weapon_list_index != 9)
			weapon_list_index = 9;
		break;

	case 5:
		switch (weapon_list_index)
		{
		case 4:
			weapon_list_index = 0;
			break;

		case 9:
			weapon_list_index = 8;
			break;
		}
		break;

	case 6:
		weapon_list_index = 7;
		break;

	case 7:
		weapon_list_index = 8;
		break;

	case 8:
		switch (weapon_list_index)
		{
		case 0:
		case 3:
		case 4:
		case 9:
			weapon_list_index = 8;
			break;
		}
		break;

	case 9:
		switch (weapon_list_index)
		{
		case 3:
		case 6:
			weapon_list_index = 0;
			break;

		case 5:
			weapon_list_index = 4;
			break;
		}
		break;
	}

	return list_index_to_weapon_definition_index(weapon_list_index);
}

long game_engine_remap_object_definition(
	long definition_index)
{
	short object_type;

	if (!game_engine_running() || definition_index==NONE)
	{
		return definition_index;
	}

	object_type = object_definition_get(definition_index)->object.type;
	if (object_type==_object_type_vehicle)
	{
		return game_engine_remap_vehicle(definition_index);
	}
	if (object_type==_object_type_weapon)
	{
		return game_engine_remap_weapon(definition_index);
	}
	if (object_type==_object_type_equipment)
	{
		return game_engine_remap_equipment(definition_index);
	}

	return definition_index;
}

/* ---------- private code */

static void netgame_flag_verify_no_team_duplicates(
	short flag_type,
	char const *error_message)
{
	struct scenario *scenario = global_scenario_get();
	short flag_index;

	for (flag_index = 0;
		flag_index < scenario->netgame_flags.count;
		flag_index++)
	{
		struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			flag_index,
			struct scenario_netgame_flag);
		short duplicate_index;

		if (flag_type != flag->type)
			continue;

		for (duplicate_index = flag_index + 1;
			duplicate_index < scenario->netgame_flags.count;
			duplicate_index++)
		{
			struct scenario_netgame_flag *duplicate = TAG_BLOCK_GET_ELEMENT(
				&scenario->netgame_flags,
				duplicate_index,
				struct scenario_netgame_flag);

			if (flag_type == duplicate->type &&
				duplicate->team_index == flag->team_index)
			{
				error(
					_error_silent,
					error_message,
					duplicate->team_index);
			}
		}
	}

	return;
}

static void update_weapon_inventory(
	long weapon_index)
{
	struct weapon_datum *weapon;

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x80A,
		weapon_index != NONE);
	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x80B,
		weapon_is_flag(weapon_index));

	weapon = weapon_get(weapon_index);
	if (weapon->object.parent_object_index == NONE &&
		!TEST_FLAG(weapon->item.flags, _item_attached_to_unit_bit) &&
		TEST_FLAG(
			weapon->weapon.flags,
			_weapon_runtime_game_engine_active_bit))
	{
		weapon->weapon.flags &=
			~FLAG(_weapon_runtime_game_engine_active_bit);

		if (game_engine->weapon_dropped)
			game_engine->weapon_dropped(weapon_index);
	}

	return;
}

static void game_engine_update_weapons(
	void)
{
	struct object_iterator iterator;

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x828,
		NULL != game_engine);

	object_iterator_new(
		&iterator,
		_object_mask_item,
		0);
	while (object_iterator_next(&iterator))
	{
		struct item_datum *item = item_get(iterator.index);

		if (TEST_FLAG(item->item.flags, _item_attached_to_unit_bit))
		{
			item->object.scale = 1.f;
		}
		else
		{
			struct item_definition *definition =
				item_definition_get(item->definition_index);

			item->object.scale =
				definition->item.scale != 0.f
					? definition->item.scale
					: 1.f;

			match_assert(
				"c:\\halo\\SOURCE\\game\\game_engine.c",
				0x83E,
				(item->object.scale >= 0.5f) &&
					(item->object.scale <= 3.f));
		}

		/*
		 * Slot 0x38 is populated by the CTF and oddball engines. It receives
		 * each objective weapon during the per-tick item scan.
		 */
		if (game_engine->objective_weapon_update)
		{
			struct weapon_datum *weapon = weapon_try_and_get(iterator.index);

			if (weapon && weapon_is_flag(iterator.index))
			{
				update_weapon_inventory(iterator.index);
				game_engine->objective_weapon_update(
					iterator.index,
					weapon);
			}
		}
	}

	return;
}

static void netgame_flag_verify_team_range(
	short flag_type,
	short minimum_index,
	short maximum_index,
	char const *error_message)
{
	struct scenario *scenario = global_scenario_get();
	short flag_index;

	for (flag_index = 0;
		flag_index < scenario->netgame_flags.count;
		flag_index++)
	{
		struct scenario_netgame_flag *flag = TAG_BLOCK_GET_ELEMENT(
			&scenario->netgame_flags,
			flag_index,
			struct scenario_netgame_flag);

		if (flag_type == flag->type &&
			(flag->team_index < minimum_index ||
				flag->team_index > maximum_index))
		{
			error(
				_error_silent,
				error_message,
				flag->team_index);
		}
	}

	return;
}

static void netgame_verify_equipment(
	short game_type,
	char const *error_message)
{
	long matching_count = 0;
	struct scenario *scenario = global_scenario_get();
	short equipment_index;

	for (equipment_index = 0;
		equipment_index < scenario->netgame_equipment.count;
		equipment_index++)
	{
		struct scenario_netgame_equipment *equipment =
			TAG_BLOCK_GET_ELEMENT(
				&scenario->netgame_equipment,
				equipment_index,
				struct scenario_netgame_equipment);

		if (match_game_type(
			game_type,
			4,
			equipment->game_type))
		{
			matching_count++;
		}
	}

	if (matching_count == 0)
		error(_error_silent, error_message);

	return;
}

static void netgame_flag_verify_team_exists(
	short flag_type,
	short flag_index,
	char const *error_message)
{
	if (find_netgame_flag(
		NULL,
		0.f,
		0.f,
		flag_type,
		flag_index) == NONE)
	{
		error(
			_error_silent,
			error_message,
			flag_index);
	}

	return;
}

static void game_engine_verify_current_map(
	void)
{
	netgame_flag_verify_team_exists(
		_netgame_flag_ctf_flag,
		0,
		"NETGAME MAP FAILURE: missing ctf flag [team %d]");
	netgame_flag_verify_team_exists(
		_netgame_flag_ctf_flag,
		1,
		"NETGAME MAP FAILURE: missing ctf flag [team %d]");

	netgame_flag_verify_no_team_duplicates(
		_netgame_flag_ctf_flag,
		"NETGAME MAP FAILURE: duplicate ctf flag [team %d]");
	netgame_flag_verify_team_range(
		_netgame_flag_ctf_flag,
		0,
		1,
		"NETGAME MAP FAILURE: ctf flag out of range [team %d]");

	netgame_flag_verify_team_exists(
		_netgame_flag_hill,
		0,
		"NETGAME MAP FAILURE: missing hill flag [team %d]");
	netgame_flag_verify_team_exists(
		_netgame_flag_hill,
		1,
		"NETGAME MAP FAILURE: missing hill flag [team %d]");
	netgame_flag_verify_team_exists(
		_netgame_flag_oddball_ball_spawn,
		0,
		"NETGAME MAP FAILURE: missing oddball flag [team %d]");
	netgame_flag_verify_team_exists(
		_netgame_flag_oddball_ball_spawn,
		1,
		"NETGAME MAP FAILURE: missing oddball flag [team %d]");
	netgame_flag_verify_team_exists(
		_netgame_flag_race_track,
		0,
		"NETGAME MAP FAILURE: missing race flag [team %d]");
	netgame_flag_verify_team_exists(
		_netgame_flag_race_track,
		1,
		"NETGAME MAP FAILURE: missing race flag [team %d]");

	netgame_flag_verify_no_team_duplicates(
		_netgame_flag_race_track,
		"NETGAME MAP FAILURE: duplicate race track flag [team %d]");

	/* BUG (preserved for exact matching): January passes team index zero for
	 * both CTF checks, and netgame_verify_spawn_points never reads that
	 * formal parameter.
	 * A corrected build should filter starting locations by an authoritatively
	 * recovered team-index field before reporting per-team counts.
	 */
	netgame_verify_spawn_points(
		game_engine_ctf,
		0,
		4,
		"NETGAME MAP FAILURE: failed to find enough spawn points for ctf team 0 (%d/%d)");
	netgame_verify_spawn_points(
		game_engine_ctf,
		0,
		4,
		"NETGAME MAP FAILURE: failed to find enough spawn points for ctf team 1 (%d/%d)");
	netgame_verify_spawn_points(
		game_engine_slayer,
		0,
		4,
		"NETGAME MAP FAILURE: failed to find enough spawn points for slayer %d/%d");
	netgame_verify_spawn_points(
		game_engine_oddball,
		0,
		4,
		"NETGAME MAP FAILURE: failed to find enough spawn points for oddball %d/%d");
	netgame_verify_spawn_points(
		game_engine_king,
		0,
		4,
		"NETGAME MAP FAILURE: failed to find enough spawn points for king %d/%d");
	netgame_verify_spawn_points(
		game_engine_race,
		0,
		4,
		"NETGAME MAP FAILURE: failed to find enough spawn points for race %d/%d");

	netgame_verify_equipment(
		game_engine_ctf,
		"NETGAME MAP FAILURE: failed to find any equipment for ctf");
	netgame_verify_equipment(
		game_engine_slayer,
		"NETGAME MAP FAILURE: failed to find any equipment for slayer");
	netgame_verify_equipment(
		game_engine_oddball,
		"NETGAME MAP FAILURE: failed to find any equipment for oddball");
	netgame_verify_equipment(
		game_engine_king,
		"NETGAME MAP FAILURE: failed to find any equipment for king");
	netgame_verify_equipment(
		game_engine_race,
		"NETGAME MAP FAILURE: failed to find any equipment for race");

	return;
}

static void internal_rasterize_target_name(
	long player_index)
{
	struct player_datum *player;
	long target_player_index;

	player = player_get(player_index);
	target_player_index = NONE;
	if (player->local_player_index != NONE && player->unit_index != NONE)
	{
		target_player_index = find_closest_player_index(player_index);
		if (target_player_index == player_index)
			target_player_index = NONE;
	}

#ifdef HALO_LINUX
	/* This is drawn once a frame, several frames per tick
	(render_interpolation.c): the hold time counts ticks, as it did on the
	Xbox. */
	{
		static long last_game_times[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
		long *last_game_time = NULL;

		if (player->local_player_index >= 0 &&
			player->local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS)
		{
			last_game_time = &last_game_times[player->local_player_index];
		}
		if (!last_game_time || *last_game_time != game_time_get())
		{
			if (last_game_time)
				*last_game_time = game_time_get();
#endif
	if (player->unknown7c != target_player_index)
	{
		if (player->target_hold_time > 0)
			player->target_hold_time--;
		if (player->target_hold_time == 0)
			player->unknown7c = target_player_index;
	}
	else if (player->target_hold_time < 15)
	{
		player->target_hold_time++;
	}
#ifdef HALO_LINUX
		}
	}
#endif

	if (player->unknown7c != NONE)
	{
		struct player_datum *target_player = player_get(player->unknown7c);
		wchar_t target_name[12] = { 0 };
		long hold_time = player->target_hold_time;
		real alpha;

		if (hold_time >= 10)
			hold_time = 10;
		ustrncpy(target_name, target_player->name, NUMBEROF(target_name) - 1);
		target_name[NUMBEROF(target_name) - 1] = 0;
		alpha = (real)pow((double)(hold_time * 0.1f), 1.9f) * 0.5f;
		game_engine_rasterize_message(target_name, alpha);
	}

	return;
}

static boolean internal_rasterize_score(
	long player_index,
	long parameter1,
	long parameter2,
	wchar_t *message,
	long message_character_count)
{
	struct player_datum *player;
	struct player_datum *other_player;
	long score;
	long string_list_index;
	boolean result;

	result = TRUE;
	player = player_get(player_index);
	score = 0;

#define GET_GAME_ENGINE_HUD_FORMAT(index) \
	(((string_list_index = tag_loaded( \
		'ustr', \
		"ui\\multiplayer_game_text")) != NONE) \
		? unicode_string_list_get_string(string_list_index, (index)) \
		: L"")

	if (game_engine &&
		game_engine->test_flag &&
		game_engine->test_flag(1))
	{
		switch (parameter1)
		{
		case 10:
			parameter1 = 14;
			break;
		case 9:
			parameter1 = 15;
			break;
		case 7:
			parameter1 = 16;
			break;
		case 12:
			parameter1 = 17;
			break;
		case 11:
			parameter1 = 18;
			break;
		case 8:
			parameter1 = 19;
			break;
		default:
			break;
		}

		if (parameter1 >= 14 &&
			parameter1 <= 19)
		{
			score = game_engine->get_player_score(player_index, TRUE);
		}
	}

	switch (parameter1)
	{
	case 0:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x4A),
			player->name);
		break;
	case 1:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x4B),
			player->name);
		break;
	case 2:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x4C),
			player->name);
		break;
	case 3:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x4D),
			player->name);
		break;
	case 4:
		other_player = player_get(parameter2);
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x4E),
			player->name,
			other_player->name);
		break;
	case 5:
		other_player = player_get(parameter2);
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x4F),
			player->name,
			other_player->name);
		break;
	case 28:
		other_player = player_get(parameter2);
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x50),
			other_player->name);
		break;
	case 6:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x51),
			player->name);
		break;
	case 13:
		other_player = player_get(parameter2);
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x52),
			other_player->name);
		break;
	case 10:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x53),
			message_character_count);
		game_engine_play_multiplayer_sound(0x10);
		break;
	case 9:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x54),
			message_character_count);
		game_engine_play_multiplayer_sound(0xF);
		break;
	case 7:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x55),
			message_character_count);
		game_engine_play_multiplayer_sound(0xE);
		break;
	case 12:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x56),
			message_character_count);
		game_engine_play_multiplayer_sound(0x11);
		break;
	case 11:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x57),
			message_character_count);
		game_engine_play_multiplayer_sound(0x12);
		break;
	case 8:
		other_player = player_get(parameter2);
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x58),
			other_player->name);
		break;
	case 14:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x59),
			score);
		game_engine_play_multiplayer_sound(0x10);
		break;
	case 15:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x5A),
			score);
		game_engine_play_multiplayer_sound(0xF);
		break;
	case 16:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x5B),
			score);
		game_engine_play_multiplayer_sound(0xE);
		break;
	case 17:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x5C),
			score);
		game_engine_play_multiplayer_sound(0x11);
		break;
	case 18:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x5D),
			score);
		game_engine_play_multiplayer_sound(0x12);
		break;
	case 19:
		other_player = player_get(parameter2);
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x5E),
			other_player->name,
			score);
		break;
	case 23:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x5F),
			message_character_count);
		break;
	case 24:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x60),
			message_character_count);
		break;
	case 25:
		usnprintf(
			message,
			message_character_count,
			GET_GAME_ENGINE_HUD_FORMAT(0x61),
			parameter2);
		break;
	case 26:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x62),
			message_character_count);
		break;
	case 27:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x63),
			message_character_count);
		break;
	case 29:
		ustrncpy(
			message,
			GET_GAME_ENGINE_HUD_FORMAT(0x64),
			message_character_count);
		break;
	default:
		result = FALSE;
		break;
	}

#undef GET_GAME_ENGINE_HUD_FORMAT

	message[message_character_count - 1] = 0;
	return result;
}

static long adjust_score_for_ranking(
	long score,
	long player_index)
{
	struct player_datum *player;
	long result = 0;

	match_assert(
		"c:\\halo\\SOURCE\\game\\game_engine.c",
		0x29D,
		player_index!=NONE);

	player = player_get(player_index);
	if (score < -1000)
		score = -1000;
	score += 1000;
	if (player->statistics.deaths <
		global_variant.universal_variant.lives)
	{
		result = 0x40000000;
	}
	if (!player->quit_out_of_game)
		result |= 0x20000000;

	return result | score;
}

static long item_collection_get_total(
	struct tag_block const *permutations)
{
	struct item_permutation_definition const *permutation =
		(struct item_permutation_definition const *)permutations->address;
	long permutation_count = permutations->count;
	long result = 0;
	long permutation_index;

	for (permutation_index = 0;
		permutation_index < permutation_count;
		permutation_index++)
	{
		result = (long)(
			(real)result + permutation[permutation_index].weight);
	}

	return result;
}

static long random_item(
	long item_collection_index)
{
	struct item_collection_definition *definition =
		item_collection_definition_get(item_collection_index);
	struct tag_block *permutations = &definition->permutations;
	long permutation_count = permutations->count;
	long remaining_weight = seed_random_range(
		get_global_random_seed_address(),
		0,
		(short)item_collection_get_total(permutations));
	struct item_permutation_definition const *permutation =
		permutations->address;
	long permutation_index = 0;

	while (permutation_index < permutation_count)
	{
		remaining_weight =
			(long)((real)remaining_weight -
				permutation[permutation_index].weight);
		if (remaining_weight < 0)
			return permutation[permutation_index].item.index;

		permutation_index++;
	}

	return NONE;
}


static void game_engine_update_item_spawn(
	void)
{
	struct scenario *scenario = global_scenario_get();
	short equipment_index;

#ifdef HALO_LINUX
	/* a client of the distributed netcode has the host's items
	(port/linux/game/network_distributed.c) */
	if (network_game_distributed_client())
		return;
#endif

	for (equipment_index = 0;
		equipment_index < scenario->netgame_equipment.count;
		equipment_index++)
	{
		struct scenario_netgame_equipment *equipment =
			TAG_BLOCK_GET_ELEMENT(
				&scenario->netgame_equipment,
				equipment_index,
				struct scenario_netgame_equipment);
		if (match_game_type(
			game_engine_get_type(),
			NUMBEROF(equipment->game_type),
			equipment->game_type))
		{
			long respawn_period = 30 * TICKS_PER_SECOND;
			short respawn_time = equipment->spawn_time;

			if (respawn_time == 0)
			{
				if (equipment->item_collection.index != NONE)
				{
					struct item_collection_definition *item_collection =
						item_collection_definition_get(
							equipment->item_collection.index);

					respawn_time = item_collection->spawn_time;
					if (respawn_time != 0)
						respawn_period = respawn_time * TICKS_PER_SECOND;
				}
			}
			else
			{
				respawn_period = respawn_time * TICKS_PER_SECOND;
			}

			if (game_time_get() % respawn_period == 0)
			{
				long definition_index =
					random_item(equipment->item_collection.index);
				struct object_placement_data placement_data;
				long object_index;

				object_placement_data_new(
					&placement_data,
					definition_index,
					NONE);
				placement_data.position = equipment->position;
				object_index = object_new(&placement_data);
				if (object_index != NONE)
				{
					struct item_datum *item = item_get(object_index);

					object_set_garbage(object_index, FALSE);
					if (TEST_FLAG(
						equipment->flags,
						_equipment_created_at_rest_bit))
					{
						SET_FLAG(item->object.flags, _object_at_rest_bit, TRUE);
					}
					item->item.last_owned_time +=
						respawn_period - 30 * TICKS_PER_SECOND;
				}
			}
		}
	}

	return;
}

static void handle_custom_starting_equipment(
	long unit_index,
	long *fragmentation_grenade_count,
	long *plasma_grenade_count)
{
	struct scenario *scenario = global_scenario_get();
	struct scenario_starting_equipment *starting_equipment;
	long starting_equipment_index = 0;

	if (scenario->scenario_starting_equipment.count <= 0)
		return;

	while (TRUE)
	{
		starting_equipment = TAG_BLOCK_GET_ELEMENT(
			&scenario->scenario_starting_equipment,
			starting_equipment_index,
			struct scenario_starting_equipment);
		if (match_game_type(
			game_engine_get_type(),
			4,
			starting_equipment->game_type))
		{
			break;
		}

		starting_equipment_index++;
		if (starting_equipment_index <
			scenario->scenario_starting_equipment.count)
			continue;

		return;
	}

	{
		boolean first_weapon = TRUE;
		long remaining_item_collection_count = 5;
		long *item_collection =
			&starting_equipment->item_collection[0].index;

		do
		{
			if (*item_collection != NONE)
			{
				long definition_index = random_item(*item_collection);
				struct object_placement_data placement_data;
				long weapon_index;
#ifdef HALO_N64
				/* debugging: port/n64/game/n64_render_game.c */
				extern long n64_debug_spawn_weapon(long definition_index, boolean first_weapon);

				definition_index = n64_debug_spawn_weapon(definition_index, first_weapon);
#endif

				object_placement_data_new(
					&placement_data,
					definition_index,
					NONE);
				weapon_index = object_new(&placement_data);
				if (weapon_index != NONE)
				{
					struct object_datum *weapon = object_get_and_verify_type(
						weapon_index,
						_object_mask_item);

					if (!first_weapon &&
						unit_has_weapon_definition_index(
							unit_index,
							weapon->definition_index))
					{
						object_delete(weapon_index);
					}
					else
					{
						unit_add_weapon_to_inventory(
							unit_index,
							weapon_index,
							first_weapon ? 2 : 0);
						first_weapon = FALSE;
					}
				}
			}

			item_collection += sizeof(starting_equipment->item_collection[0]) /
				sizeof(*item_collection);
			remaining_item_collection_count--;
		}
		while (remaining_item_collection_count != 0);

		if (TEST_FLAG(starting_equipment->flags, 0))
		{
			*fragmentation_grenade_count = 0;
			*plasma_grenade_count = 0;
		}

		if (TEST_FLAG(starting_equipment->flags, 1))
		{
			*plasma_grenade_count += *fragmentation_grenade_count;
			*fragmentation_grenade_count = 0;
		}
	}

	return;
}

void game_engine_postspawn_player_update(
	long player_index)
{
	struct player_datum *player;
	long unit_index;
	struct game_globals_grenade *fragmentation_grenade;
	struct game_globals_grenade *plasma_grenade;
	long fragmentation_grenade_count;
	long plasma_grenade_count;

	if (!game_engine)
		return;

	if (game_engine->player_update)
	{
		game_engine->player_update(player_index);
		return;
	}

	player = player_get(player_index);
	unit_index = player->unit_index;
	fragmentation_grenade = TAG_BLOCK_GET_ELEMENT(
		&scenario_get_game_globals()->grenades,
		_unit_grenade_human_fragmentation,
		struct game_globals_grenade);
	plasma_grenade = TAG_BLOCK_GET_ELEMENT(
		&scenario_get_game_globals()->grenades,
		_unit_grenade_covenant_plasma,
		struct game_globals_grenade);
	plasma_grenade_count = plasma_grenade->maximum_count;
	fragmentation_grenade_count = fragmentation_grenade->maximum_count;

	if (TEST_FLAG(game_engine_globals.flags, 3))
	{
		fragmentation_grenade_count = 1;
		plasma_grenade_count = 1;
	}
	else if (TEST_FLAG(game_engine_globals.flags, 2))
	{
		fragmentation_grenade_count = 2;
		plasma_grenade_count = 2;
	}

	{
		long starting_fragmentation_grenade_count =
			fragmentation_grenade_count;
		long starting_plasma_grenade_count = 0;

#ifdef HALO_N64
		/* (a Pro variant: a pistol and an assault rifle instead, port/n64/
		game/n64_game.c) */
		extern boolean n64_game_pro_weapons(long unit_index);

		if (n64_game_pro_weapons(unit_index))
			;
		else
#endif
		if (!TEST_FLAG(global_variant.universal_variant.flags, 5))
		{
			handle_custom_starting_equipment(
				unit_index,
				&starting_fragmentation_grenade_count,
				&starting_plasma_grenade_count);
		}

		if (game_engine_infinite_grenades_internal())
		{
			starting_plasma_grenade_count =
				plasma_grenade_count;
			starting_fragmentation_grenade_count =
				fragmentation_grenade_count;
		}

		if (unit_index != NONE)
		{
			struct unit_datum *unit = object_get_and_verify_type(
				unit_index,
				_object_mask_unit);

			switch (global_variant.universal_variant.weapon_set)
			{
			case 3:
				starting_plasma_grenade_count +=
					starting_fragmentation_grenade_count;
				starting_fragmentation_grenade_count = 0;
				break;
			case 9:
				starting_fragmentation_grenade_count +=
					starting_plasma_grenade_count;
				starting_plasma_grenade_count = 0;
				break;
			case 10:
				if (!game_engine_infinite_grenades_internal())
				{
					starting_fragmentation_grenade_count = 0;
					starting_plasma_grenade_count = 0;
				}
				break;
			}

			starting_fragmentation_grenade_count = MIN(
				starting_fragmentation_grenade_count,
				fragmentation_grenade_count);
			starting_plasma_grenade_count = MIN(
				starting_plasma_grenade_count,
				plasma_grenade_count);
			unit->unit.grenade_counts[
				_unit_grenade_human_fragmentation] =
				(char)starting_fragmentation_grenade_count;
			unit->unit.grenade_counts[
				_unit_grenade_covenant_plasma] =
				(char)starting_plasma_grenade_count;
		}
	}

	return;
}




boolean game_engine_get_state_message(
	long player_index,
	wchar_t *message,
	long message_character_count)
{
	struct player_datum *player;
	long state_message_parameter;
	long state_message;
	long respawn_timer;
	boolean result = FALSE;

	if (game_engine)
	{
		player = player_get(player_index);

		if (player->state_message >= _game_engine_message_odd_man_out &&
			player->state_message <=
				_game_engine_message_waiting_for_space_to_clear)
		{
			player->state_message = NONE;
		}

		if (player->unit_index == NONE)
		{
			state_message_parameter = 0;
			if (player->quit_out_of_game == TRUE)
				state_message = _game_engine_message_player_quit_self;
			else if (game_engine_player_is_out_of_lives(player_index))
				state_message = _game_engine_message_out_of_lives;
			else if (game_engine_player_is_odd_man_out(player_index))
				state_message = _game_engine_message_odd_man_out;
			else
			{
				respawn_timer = player->respawn_timer;
				if (respawn_timer > 0)
				{
					state_message = _game_engine_message_respawn_timer;
					state_message_parameter = respawn_timer / TICKS_PER_SECOND;
				}
				else
					state_message =
						_game_engine_message_waiting_for_space_to_clear;
			}

			result = multiplayer_message_internal(
				player_index,
				state_message,
				state_message_parameter,
				message,
				message_character_count);
		}
		else if (game_time_get() < 15 * TICKS_PER_SECOND)
		{
			result = multiplayer_message_internal(
				player_index,
				_game_engine_message_press_back_for_score,
				NONE,
				message,
				message_character_count);
		}
		else if (player->state_message != NONE)
		{
			result = multiplayer_message_internal(
				player_index,
				player->state_message,
				player->state_message_player_index,
				message,
				message_character_count);
		}
	}

	return result;
}

boolean game_engine_should_spawn_player(
	long player_index)
{
	boolean should_spawn = FALSE;

	if (game_engine)
	{
		struct player_datum *player = player_get(player_index);

		if (player->quit_out_of_game == TRUE)
		{
			should_spawn = FALSE;
		}
		else if (player->statistics.deaths == 0)
		{
			should_spawn = TRUE;
		}
		else if (game_engine_player_is_out_of_lives(player_index))
		{
			should_spawn = FALSE;
		}
		else if (game_engine_player_is_odd_man_out(player_index))
		{
			should_spawn = FALSE;
		}
		else if (game_engine_globals.postgame_state ==
				_game_engine_postgame_state_rasterize ||
			game_engine_globals.postgame_state ==
				_game_engine_postgame_state_rasterize_delay)
		{
			should_spawn = FALSE;
		}
		else
		{
			long respawn_timer = player->respawn_timer;

			if (respawn_timer > 0)
			{
				if (player->local_player_index != NONE)
				{
					if (respawn_timer == 90)
					{
						game_engine_play_multiplayer_sound(
							_multiplayer_sound_countdown_for_respawn);
					}
					else if (respawn_timer == 60)
					{
						game_engine_play_multiplayer_sound(
							_multiplayer_sound_countdown_for_respawn);
					}
					else if (respawn_timer == 30)
					{
						game_engine_play_multiplayer_sound(
							_multiplayer_sound_countdown_for_respawn);
					}
					else if (respawn_timer == 1)
					{
						game_engine_play_multiplayer_sound(
							_multiplayer_sound_respawn);
					}
				}

				player->respawn_timer--;
				should_spawn = player->respawn_timer == 0;
			}
			else
			{
				should_spawn = TRUE;
			}
		}

		if (should_spawn &&
			game_time_get() > 3 &&
			game_time_get() % 32 !=
				DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index) % 32)
		{
			should_spawn = FALSE;
		}
	}

	return should_spawn;
}

struct game_engine_place game_engine_get_place(
	long player_index,
	enum get_score_type score_type)
{
	struct player_datum *player;
	long group_count;
	boolean tied;
	boolean all_tied;
	struct game_engine_place result;

	player = player_get(player_index);
	all_tied = TRUE;
	tied = FALSE;
	group_count = 1;
	result.place = 0;

	if (game_engine->get_player_score)
	{
		struct data_iterator iterator;
		struct player_datum *other_player;
#ifdef HALO_LINUX
		/* port: free-for-all team indices run up to the player limit */
		unsigned long team_mask[BIT_VECTOR_SIZE_IN_LONGS(MULTIPLAYER_MAXIMUM_PLAYERS)] = { 0 };
#else
		unsigned long team_mask = 0;
#endif
		long score = game_engine->get_player_score(player_index, score_type);

		data_iterator_new(&iterator, player_data);
		other_player = (struct player_datum *)data_iterator_next(&iterator);
		while (other_player)
		{
			boolean different_player;

			if (score_type == _get_score_team)
			{
				different_player =
					other_player->team_index != player->team_index;
			}
			else
				different_player = iterator.datum_index != player_index;

			if (different_player &&
				score_type == _get_score_team)
			{
#ifdef HALO_LINUX
				/* a team outside the mask (none yet) counts on its own */
				if (VALID_INDEX(other_player->team_index, MULTIPLAYER_MAXIMUM_PLAYERS))
				{
					if (BIT_VECTOR_TEST_FLAG(team_mask, other_player->team_index))
						different_player = FALSE;
					else
						BIT_VECTOR_SET_FLAG(team_mask, other_player->team_index, TRUE);
				}
#else
				if (TEST_FLAG(team_mask, other_player->team_index))
					different_player = FALSE;
				else
					SET_FLAG(team_mask, other_player->team_index, TRUE);
#endif
			}

			if (different_player)
			{
				long other_score = game_engine->get_player_score(
					iterator.datum_index,
					score_type);

				group_count++;
				if (other_score != score)
					all_tied = FALSE;

				if (other_score > score)
					result.place++;
				else if (other_score == score)
					tied = TRUE;
			}

			other_player =
				(struct player_datum *)data_iterator_next(&iterator);
		}

		match_vassert(
			"c:\\halo\\SOURCE\\game\\game_engine.c",
			0x1377,
			!all_tied || tied || group_count == 1,
			"(!all_tied || (tied)) || (1 == group_count)");
	}

	all_tied &= tied;
	result.flags = score_type != _get_score_team
		? 0
		: FLAG(_place_team);
	SET_FLAG(result.flags, _place_tied, tied);
	SET_FLAG(
		result.flags,
		_place_all_tied,
		all_tied);
	SET_FLAG(
		result.flags,
		_place_two_groups,
		group_count == 2);

	return result;
}

static void netgame_verify_spawn_points(
	short game_type,
	short unused_team_index,
	short minimum_count,
	char const *error_message)
{
	short starting_location_count;
	long matching_count;
	short starting_location_index;

	starting_location_count = player_get_starting_location_count();
	matching_count = 0;
	for (starting_location_index = 0;
		starting_location_index < starting_location_count;
		starting_location_index++)
	{
		struct player_starting_location *starting_location =
			player_get_starting_location(starting_location_index);

		if (match_game_type(
			game_type,
			4,
			starting_location->game_types))
		{
			matching_count++;
		}
	}

	if (matching_count < minimum_count)
	{
		error(
			_error_silent,
			error_message,
			matching_count,
			minimum_count);
	}

	return;
}

#ifdef HALO_LINUX
long game_engine_slayer_write_network_state(byte *buffer, long size);
void game_engine_slayer_read_network_state(byte const *buffer, long size);
long game_engine_ctf_write_network_state(byte *buffer, long size);
void game_engine_ctf_read_network_state(byte const *buffer, long size);
long game_engine_oddball_write_network_state(byte *buffer, long size);
void game_engine_oddball_read_network_state(byte const *buffer, long size);
long game_engine_king_write_network_state(byte *buffer, long size);
void game_engine_king_read_network_state(byte const *buffer, long size);
long game_engine_race_write_network_state(byte *buffer, long size);
void game_engine_race_read_network_state(byte const *buffer, long size);

/* the distributed netcode (port/linux/game/network_distributed.c): the
current game type's state (scores, and what else every machine must agree
on), which the host sends its clients; the size written, 0 for none */
long game_engine_write_network_state(
	byte *buffer,
	long size)
{
	long postgame_state;
	long written;

	if (!game_engine || size < (long)sizeof(postgame_state))
		return 0;
	/* whether the game is over, then the game type's */
	postgame_state = game_engine_globals.postgame_state;
	csmemcpy(buffer, &postgame_state, sizeof(postgame_state));
	buffer += sizeof(postgame_state);
	size -= sizeof(postgame_state);
	switch (game_engine_get_type())
	{
	case game_engine_ctf: written = game_engine_ctf_write_network_state(buffer, size); break;
	case game_engine_slayer: written = game_engine_slayer_write_network_state(buffer, size); break;
	case game_engine_oddball: written = game_engine_oddball_write_network_state(buffer, size); break;
	case game_engine_king: written = game_engine_king_write_network_state(buffer, size); break;
	case game_engine_race: written = game_engine_race_write_network_state(buffer, size); break;
	default: written = 0; break;
	}
	return sizeof(postgame_state) + written;
}

/* a client: the host's */
void game_engine_read_network_state(
	byte const *buffer,
	long size)
{
	long postgame_state;

	if (!game_engine || size < (long)sizeof(postgame_state))
		return;
	csmemcpy(&postgame_state, buffer, sizeof(postgame_state));
	buffer += sizeof(postgame_state);
	size -= sizeof(postgame_state);
	/* the game ended on the host */
	if (postgame_state != 0 && game_engine_globals.postgame_state == 0)
		game_engine_end_game();
	switch (game_engine_get_type())
	{
	case game_engine_ctf: game_engine_ctf_read_network_state(buffer, size); break;
	case game_engine_slayer: game_engine_slayer_read_network_state(buffer, size); break;
	case game_engine_oddball: game_engine_oddball_read_network_state(buffer, size); break;
	case game_engine_king: game_engine_king_read_network_state(buffer, size); break;
	case game_engine_race: game_engine_race_read_network_state(buffer, size); break;
	default: break;
	}
}
#endif
