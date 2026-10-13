/*
UI_WIDGET.C

symbols in this file:
000D32C0 0020:
	_code_000d32c0 (0000)
000D32E0 0020:
	_code_000d32e0 (0000)
000D3300 0030:
	_event_controller_index_compatible_with_widget (0000)
000D3330 0030:
	_set_ui_plasma_effect_color (0000)
000D3360 00a0:
	_ui_widgets_initialize (0000)
000D3400 0010:
	_ui_widgets_safe_to_load (0000)
000D3410 0010:
	_ui_widgets_set_fade_value (0000)
000D3420 0010:
	_ui_widget_debug_show_path (0000)
000D3430 0020:
	_widget_instance_count_children (0000)
000D3450 0050:
	_widget_instance_get_nth_child (0000)
000D34A0 0030:
	_ui_widget_realloc (0000)
000D34D0 0020:
	_widget_free (0000)
000D34F0 0030:
	_ui_widgets_active (0000)
000D3520 0070:
	_ui_widgets_active_for_local_player (0000)
000D3590 0050:
	_ui_widgets_inhibit_processing (0000)
000D35E0 0020:
	_code_000d35e0 (0000)
000D3600 0490:
	_draw_bitmap_in_rect (0000)
000D3A90 0020:
	_widget_instance_get_topmost_parent (0000)
000D3AB0 0040:
	_widget_instance_get_child_index_from_parent (0000)
000D3AF0 0060:
	_widget_instance_set_visibility_recursive (0000)
000D3B50 0010:
	_main_menu_active (0000)
000D3B60 0010:
	_main_menu_is_active (0000)
000D3B70 0030:
	_main_menu_screen_is_active (0000)
000D3BA0 0050:
	_ui_set_next_level (0000)
000D3BF0 0010:
	_ui_widget_load_progress_widget (0000)
000D3C00 0010:
	_filesystem_check_thread_is_active (0000)
000D3C10 0040:
	_code_000d3c10 (0000)
000D3C50 0030:
	_display_error_when_main_menu_loaded (0000)
000D3C80 0090:
	_display_error_deferred (0000)
000D3D10 0040:
	_display_error_abort_to_dashboard_deferred (0000)
000D3D50 0070:
	_ui_start_main_menu_music (0000)
000D3DC0 0060:
	_ui_stop_main_menu_music (0000)
000D3E20 0010:
	_ui_main_menu_music_active (0000)
000D3E30 0040:
	_ui_widgets_disable_pause_game (0000)
000D3E70 0080:
	_code_000d3e70 (0000)
000D3EF0 0060:
	_code_000d3ef0 (0000)
000D3F50 0030:
	_code_000d3f50 (0000)
000D3F80 0090:
	_code_000d3f80 (0000)
000D4010 0080:
	_code_000d4010 (0000)
000D4090 0050:
	_code_000d4090 (0000)
000D40E0 0020:
	_code_000d40e0 (0000)
000D4100 0080:
	_code_000d4100 (0000)
000D4180 0040:
	_code_000d4180 (0000)
000D41C0 0040:
	_code_000d41c0 (0000)
000D4200 0050:
	_code_000d4200 (0000)
000D4250 01a0:
	_code_000d4250 (0000)
000D43F0 0070:
	_code_000d43f0 (0000)
000D4460 0060:
	_code_000d4460 (0000)
000D44C0 0060:
	_code_000d44c0 (0000)
000D4520 0130:
	_remap_sticks_for_local_player (0000)
000D4650 0030:
	_code_000d4650 (0000)
000D4680 0010:
	_code_000d4680 (0000)
000D4690 0010:
	_code_000d4690 (0000)
000D46A0 0170:
	_code_000d46a0 (0000)
000D4810 00d0:
	_code_000d4810 (0000)
000D48E0 0020:
	_code_000d48e0 (0000)
000D4900 01d0:
	_code_000d4900 (0000)
000D4AD0 0030:
	_code_000d4ad0 (0000)
000D4B00 0040:
	_code_000d4b00 (0000)
000D4B40 0020:
	_code_000d4b40 (0000)
000D4B60 0060:
	_code_000d4b60 (0000)
000D4BC0 00a0:
	_code_000d4bc0 (0000)
000D4C60 0050:
	_get_ui_rgb_white (0000)
000D4CB0 0060:
	_get_ui_argb_white (0000)
000D4D10 0050:
	_code_000d4d10 (0000)
000D4D60 0040:
	_modulate_pixel32_by_real_alpha (0000)
000D4DA0 02a0:
	_ui_widget_delete (0000)
000D5040 0050:
	_ui_widgets_close_all (0000)
000D5090 0090:
	_ui_widgets_close_all_for_local_player (0000)
000D5120 0040:
	_ui_widgets_delete_history (0000)
000D5160 0060:
	_ui_widgets_pop_stack (0000)
000D51C0 0070:
	_main_screen_shell_begin_fade (0000)
000D5230 0060:
	_ui_play_audio_feedback_sound (0000)
000D5290 02a0:
	_code_000d5290 (0000)
000D5530 0030:
	_code_000d5530 (0000)
000D5560 0360:
	_draw_string_and_hack_in_icons (0000)
000D58C0 0310:
	_code_000d58c0 (0000)
000D5BD0 0440:
	_code_000d5bd0 (0000)
000D6010 0050:
	_code_000d6010 (0000)
000D6060 00a0:
	_code_000d6060 (0000)
000D6100 00e0:
	_code_000d6100 (0000)
000D61E0 0050:
	_ui_widgets_dispose (0000)
000D6230 0200:
	_widget_event_function_list_widget_goto_next_item (0000)
000D6430 0220:
	_widget_event_function_list_widget_goto_previous_item (0000)
000D6650 04f0:
	_code_000d6650 (0000)
000D6B40 02f0:
	_code_000d6b40 (0000)
000D6E30 00b0:
	_code_000d6e30 (0000)
000D6EE0 0180:
	_render_ui_widgets_postgame (0000)
000D7060 0230:
	_render_ui_widgets (0000)
000D7290 01f0:
	_code_000d7290 (0000)
000D7480 07e0:
	_code_000d7480 (0000)
000D7C60 0220:
	_ui_widget_load_by_name_or_tag (0000)
000D7E80 0130:
	_main_screen_shell_load (0000)
000D7FB0 00e0:
	_network_game_reset_to_pregame_ui (0000)
000D8090 0380:
	_display_error (0000)
000D8410 0090:
	_display_error_abort_to_dashboard (0000)
000D84A0 0090:
	_display_error_damaged_media (0000)
000D8530 0070:
	_display_errors_deferred_until_cinematic_stop (0000)
000D85A0 0260:
	_display_scenario_help (0000)
000D8800 02a0:
	_code_000d8800 (0000)
000D8AA0 0130:
	_code_000d8aa0 (0000)
000D8BD0 03c0:
	_process_ui_widgets (0000)
00271380 0012:
	??_C@_1BC@GEHLELKL@?$AAc?$AAu?$AAs?$AAt?$AAo?$AAm?$AA?9?$AA8?$AA?$AA@ (0000)
00271394 0012:
	??_C@_1BC@DMBIFLPN@?$AAc?$AAu?$AAs?$AAt?$AAo?$AAm?$AA?9?$AA7?$AA?$AA@ (0000)
002713A8 0012:
	??_C@_1BC@IEKEDMJI@?$AAc?$AAu?$AAs?$AAt?$AAo?$AAm?$AA?9?$AA6?$AA?$AA@ (0000)
002713BC 0012:
	??_C@_1BC@JGBBJDHG@?$AAc?$AAu?$AAs?$AAt?$AAo?$AAm?$AA?9?$AA5?$AA?$AA@ (0000)
002713D0 0012:
	??_C@_1BC@COKNPEBD@?$AAc?$AAu?$AAs?$AAt?$AAo?$AAm?$AA?9?$AA4?$AA?$AA@ (0000)
002713E4 0012:
	??_C@_1BC@LDHKMMKK@?$AAc?$AAu?$AAs?$AAt?$AAo?$AAm?$AA?9?$AA3?$AA?$AA@ (0000)
002713F8 0012:
	??_C@_1BC@LMGKLMP@?$AAc?$AAu?$AAs?$AAt?$AAo?$AAm?$AA?9?$AA2?$AA?$AA@ (0000)
0027140C 0012:
	??_C@_1BC@BJHDAECB@?$AAc?$AAu?$AAs?$AAt?$AAo?$AAm?$AA?9?$AA1?$AA?$AA@ (0000)
00271420 000a:
	??_C@_19MLOAAMON@?$AAl?$AAo?$AAo?$AAk?$AA?$AA@ (0000)
0027142C 000a:
	??_C@_19OACEEDLD@?$AAm?$AAo?$AAv?$AAe?$AA?$AA@ (0000)
00271438 000a:
	??_C@_19LHGBOOBH@?$AAb?$AAa?$AAc?$AAk?$AA?$AA@ (0000)
00271444 000e:
	??_C@_1O@CNPOHNDJ@?$AAa?$AAc?$AAc?$AAe?$AAp?$AAt?$AA?$AA@ (0000)
00271454 000a:
	??_C@_19MEDLKCJO@?$AAz?$AAo?$AAo?$AAm?$AA?$AA@ (0000)
00271460 000e:
	??_C@_1O@KCCHFBLA@?$AAc?$AAr?$AAo?$AAu?$AAc?$AAh?$AA?$AA@ (0000)
00271470 0020:
	??_C@_1CA@KFDCIJKK@?$AAr?$AAo?$AAt?$AAa?$AAt?$AAe?$AA?9?$AAg?$AAr?$AAe?$AAn?$AAa?$AAd?$AAe?$AAs?$AA?$AA@ (0000)
00271490 001e:
	??_C@_1BO@POELNCJG@?$AAr?$AAo?$AAt?$AAa?$AAt?$AAe?$AA?9?$AAw?$AAe?$AAa?$AAp?$AAo?$AAn?$AAs?$AA?$AA@ (0000)
002714B0 001c:
	??_C@_1BM@IKPPIMCO@?$AAu?$AAs?$AAe?$AA?9?$AAe?$AAq?$AAu?$AAi?$AAp?$AAm?$AAe?$AAn?$AAt?$AA?$AA@ (0000)
002714CC 000a:
	??_C@_19OBFFCANM@?$AAj?$AAu?$AAm?$AAp?$AA?$AA@ (0000)
002714D8 0022:
	??_C@_1CC@OGCINJG@?$AAi?$AAn?$AAt?$AAe?$AAg?$AAr?$AAa?$AAt?$AAe?$AAd?$AA?9?$AAl?$AAi?$AAg?$AAh?$AAt?$AA?$AA@ (0000)
002714FC 0020:
	??_C@_1CA@BEHOBGA@?$AAp?$AAr?$AAi?$AAm?$AAa?$AAr?$AAy?$AA?9?$AAt?$AAr?$AAi?$AAg?$AAg?$AAe?$AAr?$AA?$AA@ (0000)
0027151C 001c:
	??_C@_1BM@HECBOACM@?$AAt?$AAh?$AAr?$AAo?$AAw?$AA?9?$AAg?$AAr?$AAe?$AAn?$AAa?$AAd?$AAe?$AA?$AA@ (0000)
00271538 000e:
	??_C@_1O@HCAHPFFC@?$AAa?$AAc?$AAt?$AAi?$AAo?$AAn?$AA?$AA@ (0000)
00271548 0018:
	??_C@_1BI@IAFIHGKJ@?$AAr?$AAi?$AAg?$AAh?$AAt?$AA?9?$AAs?$AAt?$AAi?$AAc?$AAk?$AA?$AA@ (0000)
00271560 0016:
	??_C@_1BG@GNIACJFB@?$AAl?$AAe?$AAf?$AAt?$AA?9?$AAs?$AAt?$AAi?$AAc?$AAk?$AA?$AA@ (0000)
00271578 0018:
	??_C@_1BI@KFEEPENO@?$AAr?$AAi?$AAg?$AAh?$AAt?$AA?9?$AAt?$AAh?$AAu?$AAm?$AAb?$AA?$AA@ (0000)
00271590 0016:
	??_C@_1BG@EIJMKLCG@?$AAl?$AAe?$AAf?$AAt?$AA?9?$AAt?$AAh?$AAu?$AAm?$AAb?$AA?$AA@ (0000)
002715A8 0018:
	??_C@_1BI@KIILCNEB@?$AAb?$AAa?$AAc?$AAk?$AA?9?$AAb?$AAu?$AAt?$AAt?$AAo?$AAn?$AA?$AA@ (0000)
002715C0 001a:
	??_C@_1BK@CFIHHNDP@?$AAs?$AAt?$AAa?$AAr?$AAt?$AA?9?$AAb?$AAu?$AAt?$AAt?$AAo?$AAn?$AA?$AA@ (0000)
002715DC 0016:
	??_C@_1BG@CGEEKDJO@?$AAd?$AAp?$AAa?$AAd?$AA?9?$AAr?$AAi?$AAg?$AAh?$AAt?$AA?$AA@ (0000)
002715F4 0014:
	??_C@_1BE@LFENLMOB@?$AAd?$AAp?$AAa?$AAd?$AA?9?$AAl?$AAe?$AAf?$AAt?$AA?$AA@ (0000)
00271608 0014:
	??_C@_1BE@MAHEIGBI@?$AAd?$AAp?$AAa?$AAd?$AA?9?$AAd?$AAo?$AAw?$AAn?$AA?$AA@ (0000)
0027161C 0010:
	??_C@_1BA@CNADAJOL@?$AAd?$AAp?$AAa?$AAd?$AA?9?$AAu?$AAp?$AA?$AA@ (0000)
0027162C 001c:
	??_C@_1BM@DNLFOGBJ@?$AAr?$AAi?$AAg?$AAh?$AAt?$AA?9?$AAt?$AAr?$AAi?$AAg?$AAg?$AAe?$AAr?$AA?$AA@ (0000)
00271648 001a:
	??_C@_1BK@IONNGAAO@?$AAl?$AAe?$AAf?$AAt?$AA?9?$AAt?$AAr?$AAi?$AAg?$AAg?$AAe?$AAr?$AA?$AA@ (0000)
00271664 001a:
	??_C@_1BK@DHOCGHBB@?$AAw?$AAh?$AAi?$AAt?$AAe?$AA?9?$AAb?$AAu?$AAt?$AAt?$AAo?$AAn?$AA?$AA@ (0000)
00271680 001a:
	??_C@_1BK@JINDKLEA@?$AAb?$AAl?$AAa?$AAc?$AAk?$AA?9?$AAb?$AAu?$AAt?$AAt?$AAo?$AAn?$AA?$AA@ (0000)
0027169C 0012:
	??_C@_1BC@JDEDLPGE@?$AAy?$AA?9?$AAb?$AAu?$AAt?$AAt?$AAo?$AAn?$AA?$AA@ (0000)
002716B0 0012:
	??_C@_1BC@HMBBAJIF@?$AAx?$AA?9?$AAb?$AAu?$AAt?$AAt?$AAo?$AAn?$AA?$AA@ (0000)
002716C4 0012:
	??_C@_1BC@EADHJABC@?$AAb?$AA?9?$AAb?$AAu?$AAt?$AAt?$AAo?$AAn?$AA?$AA@ (0000)
002716D8 0012:
	??_C@_1BC@KKLBENHA@?$AAa?$AA?9?$AAb?$AAu?$AAt?$AAt?$AAo?$AAn?$AA?$AA@ (0000)
002716EC 000f:
	??_C@_0P@OHNLNAJA@levels?2d40?2d40?$AA@ (0000)
002716FC 000f:
	??_C@_0P@IAIGJJBI@levels?2d20?2d20?$AA@ (0000)
0027170C 000f:
	??_C@_0P@HAMJOBDA@levels?2c40?2c40?$AA@ (0000)
0027171C 000f:
	??_C@_0P@BHJEKILI@levels?2c20?2c20?$AA@ (0000)
0027172C 000f:
	??_C@_0P@CEDKAMHM@levels?2c10?2c10?$AA@ (0000)
0027173C 000f:
	??_C@_0P@ENPIGML@levels?2b40?2b40?$AA@ (0000)
0027174C 000f:
	??_C@_0P@MEDHKOMA@levels?2b30?2b30?$AA@ (0000)
0027175C 000f:
	??_C@_0P@DPFAEPEF@levels?2a50?2a50?$AA@ (0000)
0027176C 000f:
	??_C@_0P@FIANAGMN@levels?2a30?2a30?$AA@ (0000)
0027177C 000f:
	??_C@_0P@MMBGMDIK@levels?2a10?2a10?$AA@ (0000)
0027178C 0013:
	??_C@_0BD@CKKOGMCD@widget_memory_pool?$AA@ (0000)
002717A0 0008:
	__one_over_255 (0000)
	_SECONDS_PER_MILLISECOND (0004)
002717A8 0025:
	??_C@_0CF@BFKKDAFO@c?3?2halo?2SOURCE?2interface?2ui_widg@ (0000)
002717D0 0007:
	??_C@_06KLKLDLLI@widget?$AA@ (0000)
002717D8 0024:
	??_C@_0CE@JONHIOKA@expected?5a?5valid?5local_player_in@ (0000)
002717FC 001b:
	??_C@_0BL@IGOIGKAA@widget_globals?4initialized?$AA@ (0000)
00271818 0004:
	__real@38026b7e (0000)
0027181C 0004:
	__real@43490000 (0000)
00271820 0004:
	__real@38150d23 (0000)
00271824 0004:
	__real@37e0c670 (0000)
00271828 0004:
	__real@439b8000 (0000)
0027182C 0004:
	__real@3806dd76 (0000)
00271830 000e:
	??_C@_0O@CJGPHCNG@the_main_menu?$AA@ (0000)
00271840 000e:
	??_C@_0O@LGOJGHAO@unknown?5level?$AA@ (0000)
00271850 0048:
	??_C@_0EI@DFLNAEDH@the?5old?5loading?5progress?5screen?5@ (0000)
00271898 0059:
	??_C@_0FJ@HLIPNJGE@there?5is?5already?5an?5error?5messag@ (0000)
002718F8 0051:
	??_C@_0FB@JEKLNNGC@there?5is?5already?5a?5deferred?5erro@ (0000)
0027194C 0036:
	??_C@_0DG@OLDDPPAF@?$CIindex?$DO?$DN0?$CJ?5?$CG?$CG?5?$CIindex?$DMMAXIMUM_NUM@ (0000)
00271988 0047:
	??_C@_0EH@LLPKBLA@there?5is?5already?5a?5deferred?5dash@ (0000)
002719D0 001a:
	??_C@_0BK@LDNBEGIO@title?5music?5tag?5not?5found?$AA@ (0000)
002719EC 0019:
	??_C@_0BJ@DOPIPCPC@starting?5main?5menu?5music?$AA@ (0000)
00271A08 001a:
	??_C@_0BK@GFHNHPPO@sound?2music?2title1?2title1?$AA@ (0000)
00271A24 0019:
	??_C@_0BJ@OPCCAOJF@stopping?5main?5menu?5music?$AA@ (0000)
00271A40 0012:
	??_C@_0BC@MKLFKPBF@duration_ticks?$DO?$DN0?$AA@ (0000)
00271A54 0034:
	??_C@_0DE@HOKJOLHA@out?5of?5memory?$CB?5the?5UI?5screen?5his@ (0000)
00271A88 000c:
	??_C@_0M@EABMFPBO@top?5?$CG?$CG?5data?$AA@ (0000)
00271A94 0019:
	??_C@_0BJ@FOOJDKHD@tail_child?9?$DOnext?5?$DN?$DN?5NULL?$AA@ (0000)
00271AB0 0033:
	??_C@_0DD@MMOLGAOL@?$CIchild?9?$DOprevious?5?$DN?$DN?5NULL?$CJ?5?$CG?$CG?5?$CIch@ (0000)
00271AE4 0018:
	??_C@_0BI@LBDKLIFA@next?9?$DOprevious?5?$DN?$DN?5child?$AA@ (0000)
00271AFC 0018:
	??_C@_0BI@LBKIDJB@child?9?$DOprevious?5?$DN?$DN?5NULL?$AA@ (0000)
00271B18 0041:
	??_C@_0EB@BEJBEEHM@initial_indent?$DM0?5in?5render_state@ (0000)
00271B5C 001d:
	??_C@_0BN@EEMEPEOK@31?5?$DN?$DN?5get_icon_type?$CIL?$CClook?$CC?$CJ?$AA@ (0000)
00271B7C 0024:
	??_C@_0CE@DPIKLLNB@17?5?$DN?$DN?5get_icon_type?$CIL?$CCright?9stic@ (0000)
00271BA0 001d:
	??_C@_0BN@GOCJIAID@30?5?$DN?$DN?5get_icon_type?$CIL?$CCmove?$CC?$CJ?$AA@ (0000)
00271BC0 0023:
	??_C@_0CD@BJHDPKGJ@16?5?$DN?$DN?5get_icon_type?$CIL?$CCleft?9stick@ (0000)
00271BE8 0041:
	??_C@_0EB@PKGKIBFB@?$CD?$CD?$CDWARNING?3?5possible?5memory?5leak@ (0000)
00271C2C 0022:
	??_C@_0CC@MHEDEDIP@widget?5pause?5counter?5out?5of?5whac@ (0000)
00271C50 0025:
	??_C@_0CF@EFHCMBNF@event?5handler?5failed?5to?5spawn?5wi@ (0000)
00271C78 004b:
	??_C@_0EL@BONJAKAK@a?5?8widget?5deleted?8?5event?5handler@ (0000)
00271CC4 0025:
	??_C@_0CF@OIBDABJ@widget?5?$CG?$CG?5widget_globals?4initial@ (0000)
00271CEC 001a:
	??_C@_0BK@EFAHKEKA@sound?2sfx?2ui?2flag_failure?$AA@ (0000)
00271D08 0012:
	??_C@_0BC@LFDBPFAD@sound?2sfx?2ui?2back?$AA@ (0000)
00271D1C 0015:
	??_C@_0BF@FGMCPHMP@sound?2sfx?2ui?2forward?$AA@ (0000)
00271D34 0014:
	??_C@_0BE@EOGFPNHD@sound?2sfx?2ui?2cursor?$AA@ (0000)
00271D48 003a:
	??_C@_0DK@MJICNCDL@invalid?5controller?5index?5specifi@ (0000)
00271D88 005d:
	??_C@_0FN@LKLJLEJ@_list_items_generated_from_strin@ (0000)
00271DE8 0069:
	??_C@_0GJ@EGCLHEFN@no?5child?5widget?5references?5are?5n@ (0000)
00271E58 0063:
	??_C@_0GD@BFCMHBJK@_list_items_generated_from_strin@ (0000)
00271EBC 0026:
	??_C@_0CG@GCOFCMPN@icon_index?$DO?$DN0?5?$CG?$CG?5icon_index?$DMNUM_@ (0000)
00271EE4 0006:
	??_C@_05MAJJAKPI@FALSE?$AA@ (0000)
00271EEC 0004:
	??_C@_13EJFHHPOP@?$AA?$CF?$AA?$AA@ (0000)
00271EF0 0047:
	??_C@_0EH@BDEOCECI@failed?5to?5render?5text?5box?5widget@ (0000)
00271F38 0042:
	??_C@_0EC@OINFLBMA@failed?5to?5render?5text?5box?5widget@ (0000)
00271F7C 0020:
	??_C@_1CA@EBMNPDDP@?$AA?$DM?$AAo?$AAu?$AAt?$AA?5?$AAo?$AAf?$AA?5?$AAm?$AAe?$AAm?$AAo?$AAr?$AAy?$AA?$DO?$AA?$AA@ (0000)
00271FA0 0049:
	??_C@_0EJ@ICFFKBCK@failed?5to?5render?5spinner?5list?5it@ (0000)
00271FF0 0044:
	??_C@_0EE@GFHAOED@failed?5to?5render?5spinner?5list?5it@ (0000)
00272034 0029:
	??_C@_0CJ@CLCMGDOK@failed?5to?5find?5event?5focus?5targe@ (0000)
00272060 0046:
	??_C@_0EG@GAAKHNIE@failed?5to?5spawn?5thread?5for?5files@ (0000)
002720A8 003c:
	??_C@_0DM@DBODFMFA@begining?5filesystem?5checks?5?$CG?5sav@ (0000)
002720E4 002b:
	??_C@_0CL@DAIOFIBL@widget_globals?4initialization_th@ (0000)
00272110 003d:
	??_C@_0DN@CLOOAICL@failed?5to?5set?5focus?5to?5the?5next?5@ (0000)
00272150 006d:
	??_C@_0GN@CDIJNPDM@spinner?5lists?5with?5more?5that?51?5v@ (0000)
002721C0 0033:
	??_C@_0DD@MMCOEHPE@spinner?5lists?5must?5be?5either?51?9?5@ (0000)
002721F8 0041:
	??_C@_0EB@HAOLGJEB@failed?5to?5set?5focus?5to?5the?5?$CD?$CFd?5l@ (0000)
0027223C 0034:
	??_C@_0DE@PKDHGMBO@calling?5a?5list?5widget?5function?5o@ (0000)
00272270 0016:
	??_C@_0BG@ENJMIAKH@widget?9?$DOfocused_child?$AA@ (0000)
00272288 0048:
	??_C@_0EI@GEFPBJMP@couldn?8t?5load?5conditional?5widget@ (0000)
002722D0 0029:
	??_C@_0CJ@FPJLGLJO@condition?5handler?5failed?5to?5spaw@ (0000)
00272300 0045:
	??_C@_0EF@JLKENOEN@failed?5to?5open?5widget?5because?5th@ (0000)
00272348 0019:
	??_C@_0BJ@KIDHBPBC@previous?9?$DOnext?5?$DN?$DN?5widget?$AA@ (0000)
00272364 0019:
	??_C@_0BJ@FGBHJFEB@next?9?$DOprevious?5?$DN?$DN?5widget?$AA@ (0000)
00272380 0044:
	??_C@_0EE@NFLPKLKO@failed?5to?5close?5widget?5because?5e@ (0000)
002723C8 0045:
	??_C@_0EF@EEMBCNDP@failed?5to?5reload?5widget?5because?5@ (0000)
00272410 004e:
	??_C@_0EO@PGEAPKEN@failed?5to?5give?5focus?5to?5a?5widget@ (0000)
00272460 001e:
	??_C@_0BO@HFOFCHPP@event?5handler?5function?5failed?$AA@ (0000)
00272480 002a:
	??_C@_0CK@GOFLJEI@failed?5to?5run?5ui?5widget?5event?5sc@ (0000)
002724AC 000c:
	??_C@_0M@EJOMDMOI@ui?2small_ui?$AA@ (0000)
002724B8 0016:
	??_C@_0BG@ELECBMPG@window_bounds?5?$CB?$DN?5NULL?$AA@ (0000)
002724D0 001f:
	??_C@_0BP@JAOAINDB@failed?5to?5load?5widget?5children?$AA@ (0000)
002724F0 009a:
	??_C@_0JK@BLPAHHHK@if?5the?5_widget_pass_handled_even@ (0000)
0027258C 0021:
	??_C@_0CB@BADDKMAA@widget?9?$DOlocal_player_index?$DN?$DNNONE@ (0000)
002725B0 0037:
	??_C@_0DH@DCPBMEID@widget?5?$CG?$CG?5definition?5?$CG?$CG?5event?5?$CG?$CG@ (0000)
002725E8 002c:
	??_C@_0CM@OJPOGECC@ui_widget_definition?5tag?5?8?$CFs?8?1?$CFd@ (0000)
00272614 002c:
	??_C@_0CM@MEFFCNMB@failed?5to?5create?5new?5widget?$DL?5out@ (0000)
00272640 0035:
	??_C@_0DF@PBOMEHIM@?$CIwidget_stack?$DO?$DN0?$CJ?5?$CG?$CG?5?$CIwidget_sta@ (0000)
00272678 0026:
	??_C@_0CG@GBPNNCLI@?$CIname?5?$CB?$DN?5NULL?$CJ?5?$HM?$HM?5?$CItag_index?5?$CB?$DN?5@ (0000)
002726A0 002a:
	??_C@_0CK@OFOHOPCB@failed?5to?5initialize?5the?5virtual@ (0000)
002726CC 0028:
	??_C@_0CI@EBHLJEJP@failed?5to?5load?5main?5screen?5shell@ (0000)
002726F4 001d:
	??_C@_0BN@PLHBGNEF@ui?2shell?2main_menu?2main_menu?$AA@ (0000)
00272714 0018:
	??_C@_0BI@HFFEIMKP@xbox?5command?5line?$DN?5?8?$CFs?8?$AA@ (0000)
0027272C 0006:
	??_C@_05GLGMCIFH@xdemo?$AA@ (0000)
00272734 002f:
	??_C@_0CP@FMNMOLIB@failed?5to?5load?5networked?5pregame@ (0000)
00272768 005b:
	??_C@_0FL@CANKNMEF@ui?2shell?2main_menu?2multiplayer_t@ (0000)
002727C4 002a:
	??_C@_0CK@BADOGGKF@failed?5to?5load?5map?5select?5postga@ (0000)
002727F0 0060:
	??_C@_0GA@GFIHBLPH@ui?2shell?2main_menu?2multiplayer_t@ (0000)
00272850 0035:
	??_C@_0DF@GJABDHAE@failed?5to?5load?5pregame?5screen?5af@ (0000)
00272888 0063:
	??_C@_0GD@HIMIFBNM@ui?2shell?2main_menu?2multiplayer_t@ (0000)
002728EC 0025:
	??_C@_0CF@GLGHHELH@widget?5pause?5counter?5is?5out?5of?5w@ (0000)
00272914 002f:
	??_C@_0CP@BGKPJLCC@expected?5a?5text?5box?5widget?5in?5th@ (0000)
00272944 0032:
	??_C@_0DC@JNAKMNPN@error?5screen?5widget?5tag?5not?5laye@ (0000)
00272978 0020:
	??_C@_0CA@PLEKCLAA@failed?5to?5display?5error?5message?$AA@ (0000)
00272998 0048:
	??_C@_0EI@MDMAJGJE@there?5is?5already?5an?5error?5messag@ (0000)
002729E0 0032:
	??_C@_0DC@JLPDPOMG@aborting?5to?5the?5main?5menu?5root?0?5@ (0000)
00272A18 0044:
	??_C@_0EE@HGHDGOBG@?$CIwidget_stack?$DO?$DN0?$CJ?5?$CG?$CG?5?$CIwidget_sta@ (0000)
00272A5C 001b:
	??_C@_0BL@ILOCFJOB@invalid?5local?5player?5count?$AA@ (0000)
00272A78 0028:
	??_C@_0CI@NLLLFMIK@ui?2shell?2error?2error_nonmodal_qt@ (0000)
00272AA0 0025:
	??_C@_0CF@IPCJKMOA@ui?2shell?2error?2error_modal_qtrsc@ (0000)
00272AC8 0029:
	??_C@_0CJ@MDGMGIIG@ui?2shell?2error?2error_nonmodal_ha@ (0000)
00272AF4 0026:
	??_C@_0CG@GOFPHCDA@ui?2shell?2error?2error_modal_halfs@ (0000)
00272B1C 0029:
	??_C@_0CJ@COJPHONK@ui?2shell?2error?2error_nonmodal_fu@ (0000)
00272B48 0026:
	??_C@_0CG@IDKMGEGM@ui?2shell?2error?2error_modal_fulls@ (0000)
00272B70 0059:
	??_C@_0FJ@LCJINDPJ@there?5is?5already?5a?5deferred?9for?9@ (0000)
00272BCC 001b:
	??_C@_0BL@DHJKOIEB@failed?5to?5load?5?8?$CFs?8?5widget?$AA@ (0000)
00272BE8 001b:
	??_C@_0BL@PIJOAKFD@expected?5a?5text?5box?5widget?$AA@ (0000)
00272C04 003b:
	??_C@_0DL@MFGPNPO@ui?2shell?2error?2error_abort_to_da@ (0000)
00272C40 0028:
	??_C@_0CI@JFEBNJBP@ui?2shell?2error?2error_abort_to_da@ (0000)
00272C68 0017:
	??_C@_0BH@FIKKMDKG@Noooooooooooooooooo?$CB?$CB?$CB?$AA@ (0000)
00272C80 003a:
	??_C@_0DK@KGMPEFEN@can?8t?5display?5scenario?5help?5beca@ (0000)
00272CBC 003d:
	??_C@_0DN@BOLHGMPP@can?8t?5display?5scenario?5help?$DL?5unk@ (0000)
00272CFC 0023:
	??_C@_0CD@DGCLPNFD@failed?5to?5load?5in?9game?5help?5dial@ (0000)
00272D20 002f:
	??_C@_0CP@ODEPBIAI@expected?5text?5box?5widget?5in?5play@ (0000)
00272D50 0036:
	??_C@_0DG@EMGDHLEG@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272D88 0004:
	??_C@_03JGNLDLAI@d40?$AA@ (0000)
00272D8C 0036:
	??_C@_0DG@EIOOAHPE@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272DC4 0004:
	??_C@_03JCFGEHLK@d20?$AA@ (0000)
00272DC8 0036:
	??_C@_0DG@NBLEEDPP@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272E00 0004:
	??_C@_03LAMADLB@c40?$AA@ (0000)
00272E04 0036:
	??_C@_0DG@NFDJDPEN@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272E3C 0004:
	??_C@_03PIBHPAD@c20?$AA@ (0000)
00272E40 0036:
	??_C@_0DG@NHHPIBBE@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272E78 0004:
	??_C@_03NMHMBFK@c10?$AA@ (0000)
00272E7C 0036:
	??_C@_0DG@GJAICEJK@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272EB4 0004:
	??_C@_03LDLAGENE@b40?$AA@ (0000)
00272EB8 0036:
	??_C@_0DG@GMEHDCBP@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272EF0 0004:
	??_C@_03LGPPHCFB@b30?$AA@ (0000)
00272EF4 0036:
	??_C@_0DG@HKHPOBED@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272F2C 0004:
	??_C@_03KAMHKBAN@a50?$AA@ (0000)
00272F30 0036:
	??_C@_0DG@HOPCJNPB@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272F68 0004:
	??_C@_03KEEKNNLP@a30?$AA@ (0000)
00272F6C 0036:
	??_C@_0DG@HNHGEJJP@ui?2shell?2solo_game?2player_help?2p@ (0000)
00272FA4 0004:
	??_C@_03KHMOAJNB@a10?$AA@ (0000)
00272FA8 0010:
	??_C@_0BA@CDPMFNHN@string_index?$DO?$DN0?$AA@ (0000)
00272FB8 002d:
	??_C@_0CN@FAHAPHCM@failed?5to?5load?5full?5screen?5pause@ (0000)
00272FE8 0029:
	??_C@_0CJ@OJHKNGAB@ui?2shell?2solo_game?2pause_game?2pa@ (0000)
00273018 0053:
	??_C@_0FD@MJAJHNPK@the?5ui?5seems?5to?5be?5confused?4?4?4?5a@ (0000)
0027306C 002e:
	??_C@_0CO@LCDPHEGN@failed?5to?5load?5split?5screen?5paus@ (0000)
0027309C 0036:
	??_C@_0DG@NEJLJPFI@ui?2shell?2solo_game?2pause_game?2pa@ (0000)
002730D4 0030:
	??_C@_0DA@JLINLCDA@invalid?5local?5player?5count?5for?5m@ (0000)
00273104 0033:
	??_C@_0DD@CLAJLOAB@ui?2shell?2multiplayer_game?2pause_@ (0000)
00273138 0033:
	??_C@_0DD@NDMLPFLH@ui?2shell?2multiplayer_game?2pause_@ (0000)
0027316C 002d:
	??_C@_0CN@BIDFDGIG@failed?5to?5load?5multiplayer?5pause@ (0000)
0027319C 0033:
	??_C@_0DD@KPKKNAGM@ui?2shell?2multiplayer_game?2pause_@ (0000)
002731D0 002a:
	??_C@_0CK@MAHDJCLA@invalid?5widget?5controller?5index?5@ (0000)
002731FC 0037:
	??_C@_0DH@EDIJCICF@waiting?5for?5?$CFd?5ticks?5before?5disp@ (0000)
00301DA0 413c:
	_data_00301da0 (0000)
	_widget_memory_pool (4034)
	_dashboard_abort_error (403c)
00454240 08b0:
	_bss_00454240 (0000)
*/

/* ---------- headers */

struct widget_instance;

#include "cseries.h"
#include "errors.h"
#include "bitmaps/bitmap_group.h"
#include "bitmaps/bitmaps.h"
#include "bink/bink_playback.h"
#include "bungie_net/common/thread.h"
#include "cache/texture_cache.h"
#include "cseries/cseries_windows.h"
#include "cutscene/cinematics.h"
#include "event_manager.h"
#include "game/game_engine.h"
#include "game/game_globals.h"
#include "game/player_control.h"
#include "game/players.h"
#include "hs/hs.h"
#include "input/input.h"
#include "input/input_abstraction.h"
#include "interface/attract_mode.h"
#include "interface/hud.h"
#include "interface/hud_definitions.h"
#include "interface/hud_draw.h"
#include "bitmaps/bitmap_color_conversion.h"
#include "interface/interface.h"
#include "interface/player_ui.h"
#include "interface/progress_bar.h"
#include "interface/ui_widget_game_data_input_functions.h"
#include "interface/ui_widget_event_handler_functions.h"
#include "interface/ui_widget_text_search_and_replace_functions.h"
#include "interface/virtual_keyboard.h"
#include "main/main.h"
#include "main/main_runtime.h"
#include "memory/stack_memory_pool.h"
#include "networking/network_client_manager.h"
#include "networking/network_connection.h"
#include "networking/network_game_globals.h"
#include "networking/network_server_manager.h"
#include "rasterizer/rasterizer.h"
#include "saved games/player_profile.h"
#include "saved games/playlist_profile.h"
#include "saved games/saved_game_files.h"
#include "scenario/scenario.h"
#include "shell/shell_xbox.h"
#include "sound/game_sound.h"
#include "sound/sound_definitions.h"
#include "sound/sound_manager.h"
#include "tag_files/tag_files.h"
#include "tag_files/tag_groups.h"
#include "text/draw_string.h"
#include "text/text_group.h"
#include "text/unicode.h"
#include "ui_widget.h"

/* ---------- constants */

enum
{
	WIDGET_MEMORY_POOL_SIZE = 0x4000,
	MAXIMUM_WIDGET_MEMORY_POOL_BLOCKS = 4096
};

enum
{
	_ui_widget_type_container,
	_ui_widget_type_text_box,
	_ui_widget_type_spinner_list,
	_ui_widget_type_column_list,
	_ui_widget_type_game_model,
	_ui_widget_type_movie,
	_ui_widget_type_custom,
	NUMBER_OF_UI_WIDGET_TYPES
};

enum
{
	_ui_audio_feedback_none,
	_ui_audio_feedback_cursor,
	_ui_audio_feedback_forward,
	_ui_audio_feedback_back,
	_ui_audio_feedback_flag_failure,
	NUMBER_OF_UI_AUDIO_FEEDBACK_SOUNDS
};

enum
{
	_icon_a_button,
	_icon_b_button,
	_icon_x_button,
	_icon_y_button,
	_icon_black_button,
	_icon_white_button,
	_icon_left_trigger,
	_icon_right_trigger,
	_icon_dpad_up,
	_icon_dpad_down,
	_icon_dpad_left,
	_icon_dpad_right,
	_icon_start_button,
	_icon_back_button,
	_icon_left_thumb,
	_icon_right_thumb,
	_icon_left_stick,
	_icon_right_stick,
	_icon_action,
	_icon_throw_grenade,
	_icon_primary_trigger,
	_icon_integrated_light,
	_icon_jump,
	_icon_use_equipment,
	_icon_rotate_weapons,
	_icon_rotate_grenades,
	_icon_crouch,
	_icon_zoom,
	_icon_accept,
	_icon_back,
	_icon_move,
	_icon_look,
	_icon_custom_1,
	_icon_custom_2,
	_icon_custom_3,
	_icon_custom_4,
	_icon_custom_5,
	_icon_custom_6,
	_icon_custom_7,
	_icon_custom_8,
	NUMBER_OF_ICON_TYPES
};

enum
{
	/* only the icon types below _icon_action name a button bitmap of their own;
	the rest are resolved through the local player's control preferences */
	NUM_ICONS = _icon_action
};

enum
{
	_hud_icon_use_text_bit,
	_hud_icon_use_color_bit,
	_hud_icon_absolute_width_bit,
	NUMBER_OF_HUD_ICON_FLAGS
};

enum
{
	UI_WIDGET_DEFINITION_TAG = 'DeLa',
	FONT_GROUP_TAG = 'font'
};

enum
{
	_widget_controller0,
	_widget_controller1,
	_widget_controller2,
	_widget_controller3,
	_widget_controller_any,
	NUMBER_OF_WIDGET_CONTROLLERS
};

enum
{
	/* only the bits this file tests are named; when
	_widget_always_use_tag_controller_index_bit is set a definition asking for
	any player gets no controller at all, when it is clear the same request
	inherits the invoking widget's controller */
	_widget_pass_unhandled_events_to_children_bit = 0,
	_widget_pause_game_time_bit = 1,
	_widget_flash_background_bitmap_bit = 2,
	_widget_dpad_updown_tabs_thru_children_bit = 3,
	_widget_dpad_leftright_tabs_thru_children_bit = 4,
	_widget_dpad_updown_tabs_thru_list_items_bit = 5,
	_widget_dpad_leftright_tabs_thru_list_items_bit = 6,
	_widget_dont_focus_a_specific_child_bit = 7,
	_widget_pass_unhandled_events_to_all_children_bit = 8,
	_widget_render_regardless_of_controller_index_bit = 9,
	_widget_pass_handled_events_to_all_children_bit = 10,
	_widget_return_to_main_menu_if_no_history_bit = 11,
	_widget_always_use_tag_controller_index_bit = 12,
	_widget_always_render_with_nifty_fx_bit = 13,
	_widget_dont_push_history_data_bit = 14
};

enum
{
	_child_widget_use_custom_controller_index_bit = 0,
	NUMBER_OF_CHILD_WIDGET_FLAGS
};

enum
{
	_conditional_widget_load_if_event_handler_function_fails_bit = 0,
	NUMBER_OF_CONDITIONAL_WIDGET_FLAGS
};

enum
{
	_event_handler_close_current_widget_bit,
	_event_handler_close_other_widget_bit,
	_event_handler_close_all_widgets_bit,
	_event_handler_open_widget_bit,
	_event_handler_reload_self_bit,
	_event_handler_reload_widget_bit,
	_event_handler_give_focus_to_widget_bit,
	_event_handler_run_function_bit,
	_event_handler_replace_with_other_widget_bit,
	_event_handler_go_back_to_previous_widget_bit,
	_event_handler_run_scenario_script_bit,
	_event_handler_look_for_conditional_widget_on_failure_bit,
	NUMBER_OF_EVENT_HANDLER_FLAGS
};

enum
{
	_list_items_generated_in_code,
	_list_items_generated_from_string_list_tag,
	_list_items_only_one_tooltip_entry,
	_list_single_preview_box_no_scroll,
	NUMBER_OF_LIST_FLAGS
};

enum
{
	/* only the bit this file tests is named */
	_text_box_flashing_text_bit = 2
};

enum
{
	_text_justification_left,
	_text_justification_right,
	_text_justification_center,
	NUMBER_OF_TEXT_JUSTIFICATIONS
};

enum
{
	/* EVENT_MANAGER.C owns this enumeration and publishes none of it */
	_event_type_null,
	_event_type_left_stick,
	_event_type_right_stick,
	_event_type_button,
	NUMBER_OF_EVENT_TYPES
};

enum
{
	/* the button event types are the gamepad button indices; the enumeration
	runs 0..33 and only the types this file names are listed */
	_widget_event_b_button = _gamepad_analog_button_b,
	_widget_event_dpad_up = _gamepad_binary_button_dpad_up,
	_widget_event_dpad_down = _gamepad_binary_button_dpad_down,
	_widget_event_dpad_left = _gamepad_binary_button_dpad_left,
	_widget_event_dpad_right = _gamepad_binary_button_dpad_right,
	_widget_event_back_button = _gamepad_binary_button_back,
	_widget_event_left_stick_up = NUMBER_OF_GAMEPAD_BUTTONS,
	_widget_event_left_stick_down,
	_widget_event_left_stick_left,
	_widget_event_left_stick_right,
	_widget_event_right_stick_up,
	_widget_event_right_stick_down,
	_widget_event_right_stick_left,
	_widget_event_right_stick_right,
	_widget_event_created,
	_widget_event_deleted
};

enum
{
	/* a held dpad direction repeats no faster than this */
	DPAD_EVENT_REPEAT_MILLISECONDS = 250,
	NUMBER_OF_DPAD_DIRECTIONS =
		_widget_event_dpad_right - _widget_event_dpad_up + 1
};

enum
{
	/* deferred errors wait this long into a level before they are shown */
	DEFERRED_ERROR_DELAY_TICKS = 30
};

enum
{
	WIDGET_DELETED_PLAYER_CONTROL_INHIBIT_FLAGS = 0x0FFF
};

enum
{
	/* named for the errors each result raises; SAVED_GAME_FILES.H publishes no
	enumeration of its own */
	_file_system_check_result_none,
	_file_system_check_result_not_enough_free_space,
	_file_system_check_result_too_many_saved_games,
	NUMBER_OF_FILE_SYSTEM_CHECK_RESULTS
};

/* ---------- macros */

#define SIGN(n) ((n) >= 0 ? 1 : -1)

/* The public definition header intentionally exposes only the shared prefix;
this TU owns the complete January layout used below. */
#define ui_widget_definition_get(tag_index) \
	((struct ui_widget_definition *)tag_get(UI_WIDGET_DEFINITION_TAG, (tag_index)))

/* ---------- structures */

struct stack_memory_pool_block;

struct stack_memory_pool
{
	char const *name;
	byte *base_address;
	long size;
	long maximum_block_count;
	long next_block_index;
	long bytes_used;
	long maximum_bytes_used;
	unsigned long block_count;
	unsigned long maximum_block_count_used;
	long largest_block_size;
	boolean disable_compaction;
	byte unused29[3];
	struct stack_memory_pool_block *first_block;
	struct stack_memory_pool_block *last_block;
	struct stack_memory_pool_block *blocks[1];
};

struct stack_memory_pool_medium
{
	struct stack_memory_pool pool;
	struct stack_memory_pool_block *blocks[MAXIMUM_WIDGET_MEMORY_POOL_BLOCKS - 1];
};

/* narrow views of the tag definitions this file reaches through; the owning
translation units (HUD.C, HUD_MESSAGING.C, INTERFACE.C) keep their own */

struct icon_hud_element_definition
{
	short sequence_index;
	short width_offset;
	point2d offset;
	pixel32 color;
	char frame_rate;
	byte flags;
	short text_index;
};

struct interface_tag_references_definition
{
	struct tag_reference tags[NUMBER_OF_INTERFACE_TAGS];
	byte unused[48];
};

typedef char verify_icon_hud_element_definition_size[
	sizeof(struct icon_hud_element_definition) == 0x10 ? 1 : -1];
typedef char verify_hud_globals_button_icons_offset[
	offsetof(struct hud_globals_definition, messaging.button_icons) == 0xC4 ? 1 : -1];
typedef char verify_interface_tag_references_definition_size[
	sizeof(struct interface_tag_references_definition) == 0x130 ? 1 : -1];
/* narrow views of the 'DeLa' widget definition tag and of the three block
elements this file walks; only the members this file reaches are named and
every other span is left explicitly unknown */

struct ui_widget_event_handler_reference
{
	long flags;
	short event_type;
	short function;
	struct tag_reference widget_tag;
	struct tag_reference sound_effect;
	char script[32];
};

struct ui_widget_child_reference
{
	struct tag_reference widget_tag;
	char name[32];
	long flags;
	short custom_controller_index;
	short vertical_offset;
	short horizontal_offset;
	byte unknown03A[0x50 - 0x3A];
};

struct ui_widget_conditional_reference
{
	struct tag_reference widget_tag;
	char name[32];
	long flags;
	short custom_controller_index;
	byte unknown036[0x50 - 0x36];
};

struct ui_widget_game_data_input_reference
{
	short function;
	byte unknown002[0x24 - 0x02];
};

struct ui_widget_search_and_replace_reference
{
	char search_string[32];
	short replace_function;
};

struct ui_widget_definition
{
	short type;
	short controller_index;
	char name[32];
	rectangle2d bounds;
	long flags;
	long milliseconds_to_auto_close;
	long auto_close_fade_time;
	struct tag_reference background_bitmap;
	struct tag_block game_data_inputs;
	struct tag_block event_handlers;
	struct tag_block search_and_replace_functions;
	byte unknown06C[0xEC - 0x6C];
	struct tag_reference text_label_string_list;
	struct tag_reference text_font;
	real_argb_color text_color;
	short justification;
	word text_box_flags;
	byte unknown120[0x12E - 0x120];
	short string_list_index;
	short horizontal_offset;
	short vertical_offset;
	byte unknown134[0x150 - 0x134];
	long list_flags;
	struct tag_reference list_header_bitmap;
	struct tag_reference list_footer_bitmap;
	rectangle2d list_header_bounds;
	rectangle2d list_footer_bounds;
	byte unknown184[0x1A4 - 0x184];
	struct tag_reference extended_description_widget;
	byte unknown1B4[0x2D4 - 0x1B4];
	struct tag_block conditional_widgets;
	byte unknown2E0[0x3E0 - 0x2E0];
	struct tag_block child_widgets;
};

typedef char verify_ui_widget_game_data_input_reference_size[
	sizeof(struct ui_widget_game_data_input_reference) == 0x24 ? 1 : -1];
typedef char verify_ui_widget_search_and_replace_reference_size[
	sizeof(struct ui_widget_search_and_replace_reference) == 0x22 ? 1 : -1];
typedef char verify_ui_widget_child_reference_size[
	sizeof(struct ui_widget_child_reference) == 0x50 ? 1 : -1];
typedef char verify_ui_widget_conditional_reference_size[
	sizeof(struct ui_widget_conditional_reference) == 0x50 ? 1 : -1];
typedef char verify_ui_widget_event_handler_reference_size[
	sizeof(struct ui_widget_event_handler_reference) == 0x48 ? 1 : -1];
typedef char verify_ui_widget_definition_bounds_offset[
	offsetof(struct ui_widget_definition, bounds) == 0x24 ? 1 : -1];
typedef char verify_ui_widget_definition_flags_offset[
	offsetof(struct ui_widget_definition, flags) == 0x2C ? 1 : -1];
typedef char verify_ui_widget_definition_game_data_inputs_offset[
	offsetof(struct ui_widget_definition, game_data_inputs) == 0x48 ? 1 : -1];
typedef char verify_ui_widget_definition_search_and_replace_offset[
	offsetof(struct ui_widget_definition, search_and_replace_functions) == 0x60 ? 1 : -1];
typedef char verify_ui_widget_definition_text_font_offset[
	offsetof(struct ui_widget_definition, text_font) == 0xFC ? 1 : -1];
typedef char verify_ui_widget_definition_text_color_offset[
	offsetof(struct ui_widget_definition, text_color) == 0x10C ? 1 : -1];
typedef char verify_ui_widget_definition_justification_offset[
	offsetof(struct ui_widget_definition, justification) == 0x11C ? 1 : -1];
typedef char verify_ui_widget_definition_text_box_flags_offset[
	offsetof(struct ui_widget_definition, text_box_flags) == 0x11E ? 1 : -1];
typedef char verify_ui_widget_definition_string_list_index_offset[
	offsetof(struct ui_widget_definition, string_list_index) == 0x12E ? 1 : -1];
typedef char verify_ui_widget_definition_horizontal_offset_offset[
	offsetof(struct ui_widget_definition, horizontal_offset) == 0x130 ? 1 : -1];
typedef char verify_ui_widget_definition_list_header_bitmap_offset[
	offsetof(struct ui_widget_definition, list_header_bitmap) == 0x154 ? 1 : -1];
typedef char verify_ui_widget_definition_list_header_bounds_offset[
	offsetof(struct ui_widget_definition, list_header_bounds) == 0x174 ? 1 : -1];
typedef char verify_ui_widget_definition_event_handlers_offset[
	offsetof(struct ui_widget_definition, event_handlers) == 0x54 ? 1 : -1];
typedef char verify_ui_widget_definition_background_bitmap_offset[
	offsetof(struct ui_widget_definition, background_bitmap) == 0x38 ? 1 : -1];
typedef char verify_ui_widget_definition_text_label_string_list_offset[
	offsetof(struct ui_widget_definition, text_label_string_list) == 0xEC ? 1 : -1];
typedef char verify_ui_widget_definition_list_flags_offset[
	offsetof(struct ui_widget_definition, list_flags) == 0x150 ? 1 : -1];
typedef char verify_ui_widget_definition_extended_description_offset[
	offsetof(struct ui_widget_definition, extended_description_widget) == 0x1A4 ? 1 : -1];
typedef char verify_ui_widget_definition_conditional_widgets_offset[
	offsetof(struct ui_widget_definition, conditional_widgets) == 0x2D4 ? 1 : -1];
typedef char verify_ui_widget_definition_child_widgets_offset[
	offsetof(struct ui_widget_definition, child_widgets) == 0x3E0 ? 1 : -1];
typedef char verify_ui_widget_definition_size[
	sizeof(struct ui_widget_definition) == 0x3EC ? 1 : -1];

struct ui_widget_deferred_error
{
	short error_code;
	short local_player_index;
	boolean modal;
	boolean pause_game_time;
};

struct ui_widget_deferred_cinematic_error
{
	short error_code;
	boolean modal;
	boolean pause_game_time;
};

struct widget_stack_data
{
	long previous_widget_tag;
	long focused_child_parent_widget_tag;
	short focused_child_index;
	short local_player_index;
};

struct widget_stack_node
{
	struct widget_stack_data data;
	struct widget_stack_node *next;
};

struct ui_widget_runtime_globals_prefix
{
	struct widget_instance *active_widgets[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	struct widget_stack_node *widget_stack[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	unsigned long current_system_milliseconds;
	long pause_disabled_ticks;
	short main_menu_deferred_error_code;
	short pause_game_time_count;
	real fade_to_black;
	struct ui_widget_deferred_error deferred_errors[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	short deferred_dashboard_error_code;
	boolean deferred_dashboard_optional;
	byte reserved004B;
	struct ui_widget_deferred_cinematic_error deferred_cinematic_errors[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	struct thread_reference *initialization_thread;
	short filesystem_check_result;
	boolean initialized;
	boolean dont_load_children_recursive;
	boolean debug_show_path;
	boolean processing_inhibited;
	boolean main_menu_music_active;
	boolean sound_paused;
};

struct ui_widget_bss_prefix
{
	wchar_t string_data[1024];
	struct ui_widget_runtime_globals_prefix widget_globals;
	boolean we_are_at_the_main_menu;
	byte unknown869[0x870 - 0x869];
	unsigned long dpad_event_times[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS][NUMBER_OF_DPAD_DIRECTIONS];
};

typedef char verify_ui_widget_fade_to_black_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		fade_to_black) == 0x2C ? 1 : -1];
typedef char verify_ui_widget_pause_disabled_ticks_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		pause_disabled_ticks) == 0x24 ? 1 : -1];
typedef char verify_ui_widget_main_menu_deferred_error_code_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		main_menu_deferred_error_code) == 0x28 ? 1 : -1];
typedef char verify_ui_widget_deferred_dashboard_error_code_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		deferred_dashboard_error_code) == 0x48 ? 1 : -1];
typedef char verify_ui_widget_deferred_dashboard_optional_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		deferred_dashboard_optional) == 0x4A ? 1 : -1];
typedef char verify_ui_widget_initialization_thread_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		initialization_thread) == 0x5C ? 1 : -1];
typedef char verify_ui_widget_initialized_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		initialized) == 0x62 ? 1 : -1];
typedef char verify_ui_widget_debug_show_path_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		debug_show_path) == 0x64 ? 1 : -1];
typedef char verify_ui_widget_processing_inhibited_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		processing_inhibited) == 0x65 ? 1 : -1];
typedef char verify_ui_widget_main_menu_music_active_offset[
	offsetof(
		struct ui_widget_runtime_globals_prefix,
		main_menu_music_active) == 0x66 ? 1 : -1];
typedef char verify_ui_widget_runtime_globals_prefix_size[
	sizeof(struct ui_widget_runtime_globals_prefix) == 0x68 ? 1 : -1];
typedef char verify_ui_widget_globals_offset[
	offsetof(
		struct ui_widget_bss_prefix,
		widget_globals) == 0x800 ? 1 : -1];
typedef char verify_ui_widget_main_menu_active_offset[
	offsetof(
		struct ui_widget_bss_prefix,
		we_are_at_the_main_menu) == 0x868 ? 1 : -1];
typedef char verify_ui_widget_dpad_event_times_offset[
	offsetof(
		struct ui_widget_bss_prefix,
		dpad_event_times) == 0x870 ? 1 : -1];

struct widget_animation_data
{
	short current_frame_index;
	short first_frame_index;
	short last_frame_index;
	short number_of_sprite_frames;
};

struct widget_instance
{
	long definition_tag_index;
	char const *name;
	short local_player_index;
	short horizontal_offset;
	short vertical_offset;
	short type;
	boolean visible;
	boolean render_regardless_of_controller_index;
	boolean disabled;
	boolean pause_game_time;
	boolean delete_recursion_lock;
	boolean widget_is_error_dialog;
	boolean close_if_local_player_controller_present;
	byte pad17;
	long creation_time;
	unsigned long milliseconds_to_auto_close;
	unsigned long auto_close_fade_time;
	real alpha_modifier;
	struct widget_instance *previous;
	struct widget_instance *next;
	struct widget_instance *parent;
	struct widget_instance *child;
	struct widget_instance *focused_child;
	union
	{
		struct
		{
			wchar_t *text;
			short string_list_index;
		} text_box;
		struct
		{
			short selected_index;
			/* counted back toward zero one step per rendered frame; the two
			tab functions start it at +15 and -15 and the column list renderer
			clears it */
			short last_list_tab_direction;
			void *list_items;
			word number_of_items;
			struct widget_instance *extended_description;
			wchar_t *item_text;
		} list;
	} parameters;
	struct widget_animation_data animation;
};

typedef char verify_widget_instance_size[
	sizeof(struct widget_instance) == 0x58 ? 1 : -1];
typedef char verify_widget_instance_animation_offset[
	offsetof(struct widget_instance, animation) == 0x50 ? 1 : -1];

typedef char verify_widget_instance_creation_time_offset[
	offsetof(struct widget_instance, creation_time) == 0x18 ? 1 : -1];
typedef char verify_widget_instance_alpha_modifier_offset[
	offsetof(struct widget_instance, alpha_modifier) == 0x24 ? 1 : -1];
typedef char verify_widget_instance_next_offset[
	offsetof(struct widget_instance, next) == 0x2C ? 1 : -1];
typedef char verify_widget_instance_focused_child_offset[
	offsetof(struct widget_instance, focused_child) == 0x38 ? 1 : -1];
typedef char verify_widget_instance_text_box_string_list_index_offset[
	offsetof(struct widget_instance, parameters.text_box.string_list_index) == 0x40 ? 1 : -1];

/* ---------- prototypes */

static boolean transition_to_game_in_progress(
	void);
static __inline real compute_offset_coordinate(
	long time,
	real delta_per_second);
static short get_icon_type(
	wchar_t const *string);
static void render_state_text(
	rectangle2d *bounds,
	rectangle2d *cursor_bounds,
	wchar_t const *text);
static void render_state_bitmap(
	rectangle2d *bounds,
	rectangle2d *cursor_bounds,
	pixel32 color,
	struct icon_hud_element_definition *icon);
static boolean should_flip_sticks_for_local_player(
	short local_player_index);
static unsigned long __stdcall filesystem_initialization_thread_proc(
	void *input);
static void perform_filesystem_initialization(
	void);
static void ui_widget_delete_children_recursive(
	struct widget_instance *widget);
static struct widget_instance *ui_widget_launch_widget(
	struct widget_instance *widget,
	long widget_tag_index);
static __inline boolean widget_instance_can_handle_events(
	struct widget_instance *widget);
static struct widget_instance *widget_instance_find_by_tag_index_recursive(
	struct widget_instance *widget,
	long tag_index);
static void widget_instance_give_focus_directly(
	struct widget_instance *widget,
	struct widget_instance *new_focus);
static void widget_instance_give_focus_by_tag(
	struct widget_instance *widget,
	long tag_index,
	short local_player_index);

static __inline struct widget_instance *widget_instance_get_tail_child_widget(
	struct widget_instance *widget);
static void ui_widget_add_child(
	struct widget_instance *parent,
	struct widget_instance *child);
static void push_widget(
	struct widget_stack_node **top,
	struct widget_stack_data *data);
static boolean widget_instance_can_receive_events(
	struct widget_instance *widget);
static void widget_instance_set_focused_child_by_index(
	long tag_index,
	struct widget_instance *widget,
	short child_index);
static void widget_instance_go_back_to_previous(
	struct widget_instance *widget);
static __inline struct widget_instance *widget_instance_find_by_tag_index(
	long tag_index);
static void widget_instance_reload_recursive(
	struct widget_instance *widget);
static void ui_widget_reload_by_tag(
	long tag_index);
static void event_handler_dispatch(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	struct event_record *event,
	struct ui_widget_event_handler_reference *handler,
	boolean *calling_widget_deleted);
static boolean ui_widget_load_children_recursive(
	struct widget_instance *widget,
	struct ui_widget_definition *definition);
static void widget_instance_initialize(
	struct widget_instance *widget,
	struct widget_instance *parent,
	struct ui_widget_definition *definition,
	long tag_index,
	short local_player_index,
	short widget_stack);
static __inline real widget_instance_get_cumulative_alpha_modifier(
	struct widget_instance *widget);
static boolean widget_instance_text_box_is_focused(
	struct widget_instance *widget);
static boolean string_has_icons_to_draw(
	wchar_t *string);
static long search_and_replace(
	wchar_t *search,
	wchar_t *replace,
	wchar_t **string);
static void widget_instance_render_text_box(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	rectangle2d *clip_rect,
	point2d offset,
	boolean focus);
static void widget_instance_render_spinner_list(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	rectangle2d *clip_rect,
	point2d offset,
	boolean focus);
static void widget_instance_render_column_list(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	rectangle2d *clip_rect,
	point2d offset,
	boolean focus);
static void widget_instance_render_recursive(
	struct widget_instance *widget,
	rectangle2d *clip_rect,
	point2d offset,
	boolean focus,
	boolean use_nifty_plasma_fx);
static __inline void widget_instance_update_animation_parameters(
	struct widget_instance *widget);
static __inline void spinner_list_update(
	struct widget_instance *widget);
static void column_list_update(
	struct widget_instance *widget,
	struct ui_widget_definition *definition);
static void widget_instance_tab_to_next_valid_widget(
	struct widget_instance *widget);
static void widget_instance_tab_to_previous_valid_widget(
	struct widget_instance *widget);
static void widget_instance_process_one_event_recursive(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	struct event_record *event,
	boolean *return_widget_deleted);
static boolean ui_check_for_pause_game(
	void);

/* ---------- globals */

static struct ui_widget_bss_prefix ui_widget_globals_storage;

#define string_data ui_widget_globals_storage.string_data
#define widget_globals ui_widget_globals_storage.widget_globals
#define we_are_at_the_main_menu ui_widget_globals_storage.we_are_at_the_main_menu
#define dpad_event_times ui_widget_globals_storage.dpad_event_times
extern real_argb_color ui_plasma_effect_color;
extern short local_player_index_for_draw_string_and_hack_in_icons;

/* January defines this and never references it, as we do not */
real const _one_over_255 = 1.0f / 255.0f;
real const SECONDS_PER_MILLISECOND = 0.001f;

static struct stack_memory_pool_medium __medium_widget_memory_pool =
{
	{
		"widget_memory_pool",
		NULL,
		0,
		MAXIMUM_WIDGET_MEMORY_POOL_BLOCKS
	}
};

struct stack_memory_pool *widget_memory_pool = &__medium_widget_memory_pool.pool;

static boolean main_screen_shell_first_load = TRUE;

short dashboard_abort_error = NONE;

static char const *scenario_paths[10] =
{
	"levels\\a10\\a10",
	"levels\\a30\\a30",
	"levels\\a50\\a50",
	"levels\\b30\\b30",
	"levels\\b40\\b40",
	"levels\\c10\\c10",
	"levels\\c20\\c20",
	"levels\\c40\\c40",
	"levels\\d20\\d20",
	"levels\\d40\\d40"
};

static boolean icon_is_special[NUM_ICONS] =
{
	FALSE,	/* a-button */
	FALSE,	/* b-button */
	FALSE,	/* x-button */
	FALSE,	/* y-button */
	FALSE,	/* black-button */
	FALSE,	/* white-button */
	TRUE,	/* left-trigger */
	TRUE,	/* right-trigger */
	FALSE,	/* dpad-up */
	FALSE,	/* dpad-down */
	FALSE,	/* dpad-left */
	FALSE,	/* dpad-right */
	FALSE,	/* start-button */
	FALSE,	/* back-button */
	TRUE,	/* left-thumb */
	TRUE,	/* right-thumb */
	TRUE,	/* left-stick */
	TRUE	/* right-stick */
};

static wchar_t const *icon_names[NUMBER_OF_ICON_TYPES] =
{
	L"a-button",
	L"b-button",
	L"x-button",
	L"y-button",
	L"black-button",
	L"white-button",
	L"left-trigger",
	L"right-trigger",
	L"dpad-up",
	L"dpad-down",
	L"dpad-left",
	L"dpad-right",
	L"start-button",
	L"back-button",
	L"left-thumb",
	L"right-thumb",
	L"left-stick",
	L"right-stick",
	L"action",
	L"throw-grenade",
	L"primary-trigger",
	L"integrated-light",
	L"jump",
	L"use-equipment",
	L"rotate-weapons",
	L"rotate-grenades",
	L"crouch",
	L"zoom",
	L"accept",
	L"back",
	L"move",
	L"look",
	L"custom-1",
	L"custom-2",
	L"custom-3",
	L"custom-4",
	L"custom-5",
	L"custom-6",
	L"custom-7",
	L"custom-8"
};

/* indexed by icon type - _icon_action.  The first ten entries are game control
indices for game_input_preferences.game_control_to_xbox_buttons (that enumeration
is private to INPUT_ABSTRACTION.C); the last four are icon types used directly
when the icon does not depend on the local player's control preferences. */
static char button_mappings[_icon_custom_1 - _icon_action] =
{
	2,		/* action -> action */
	6,		/* throw-grenade -> grenade */
	7,		/* primary-trigger -> primary trigger */
	5,		/* integrated-light -> flashlight */
	0,		/* jump -> jump */
	4,		/* use-equipment -> melee */
	3,		/* rotate-weapons -> switch weapons */
	1,		/* rotate-grenades -> switch grenades */
	11,		/* crouch -> zoom */
	10,		/* zoom -> crouch */
	_icon_a_button,		/* accept */
	_icon_b_button,		/* back */
	_icon_left_stick,	/* move */
	_icon_right_stick	/* look */
};

real global_ui_white_red = 0.8f;
real global_ui_white_green = 0.8f;
real global_ui_white_blue = 0.8f;


/* ---------- public code */

void set_ui_plasma_effect_color(
	real alpha,
	real red,
	real green,
	real blue)
{
	ui_plasma_effect_color.alpha = alpha;
	ui_plasma_effect_color.red = red;
	ui_plasma_effect_color.green = green;
	ui_plasma_effect_color.blue = blue;

	return;
}

boolean event_controller_index_compatible_with_widget(
	struct event_record const *event,
	struct widget_instance const *widget)
{
	short widget_controller_index;

	widget_controller_index = widget->local_player_index;
	return widget_controller_index == NONE ||
		widget_controller_index == event->controller_index;
}

void ui_widgets_safe_to_load(
	boolean safe)
{
	return;
}

void ui_widgets_inhibit_processing(
	boolean inhibit)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		1174,
		widget_globals.initialized);
	widget_globals.processing_inhibited = inhibit;

	return;
}

static __inline real compute_offset_coordinate(
	long time,
	real delta_per_second)
{
	real scaled_delta = delta_per_second * 0.001f;

	return (real)fmod(
		scaled_delta * time,
		1.0);
}

void draw_bitmap_in_rect(
	struct bitmap_data *bitmap,
	rectangle2d *rect,
	rectangle2d *bitmap_rect,
	rectangle2d *clip_rect,
	pixel32 argb,
	struct rasterizer_dynamic_screen_geometry_parameters *multitexture_params,
	boolean no_plasma)
{
	if (bitmap && rect)
	{
		real_argb_color plasma_fade = ui_plasma_effect_color;
		real_rgb_color map_tint = { 0.9f, 0.9f, 0.9f };
		real map_fade = 0.9f;
		rectangle2d default_bitmap_rect;
		real_point2d positions[NUMBER_OF_POINTS_PER_RECTANGLE];
		struct dynamic_screen_vertex vertices[NUMBER_OF_POINTS_PER_RECTANGLE];
		struct rasterizer_dynamic_screen_geometry_parameters parameters;
		real bitmap_width;
		real bitmap_height;
		real texture_width;
		real texture_height;
		real_point2d map0_offset;
		real_point2d map1_offset;
		short rectangle_x0;
		short rectangle_y0;
		short rectangle_width;
		short rectangle_height;
		short source_width;
		short source_height;
		short vertex_index;

		if (!bitmap_rect)
		{
			default_bitmap_rect.x0 = 0;
			default_bitmap_rect.y0 = 0;
			default_bitmap_rect.x1 = bitmap->width;
			default_bitmap_rect.y1 = bitmap->height;
			bitmap_rect = &default_bitmap_rect;
		}

		rectangle_width = rect->x1 - rect->x0;
		rectangle_height = rect->y1 - rect->y0;
		rectangle_x0 = rect->x0;
		rectangle_y0 = rect->y0;
		source_width = bitmap_rect->x1 - bitmap_rect->x0;
		source_height = bitmap_rect->y1 - bitmap_rect->y0;
		positions[0].x = (real)rectangle_x0;
		positions[0].y = (real)rectangle_y0;
		positions[1].x = (real)(rectangle_x0 + rectangle_width);
		positions[1].y = (real)rectangle_y0;
		positions[2].x = (real)(rectangle_x0 + rectangle_width);
		positions[2].y = (real)(rectangle_y0 + rectangle_height);
		positions[3].x = (real)rectangle_x0;
		positions[3].y = (real)(rectangle_y0 + rectangle_height);

		if (clip_rect)
		{
			if (clip_rect->x0 > rect->x0)
			{
				positions[0].x = positions[3].x = (real)clip_rect->x0;
			}
			if (clip_rect->x1 < rect->x1)
			{
				positions[1].x = positions[2].x = (real)clip_rect->x1;
			}
			if (clip_rect->y0 > rect->y0)
			{
				positions[0].y = positions[1].y = (real)clip_rect->y0;
			}
			if (clip_rect->y1 < rect->y1)
			{
				positions[2].y = positions[3].y = (real)clip_rect->y1;
			}
		}

		bitmap_width = MAX(1.0f, (real)bitmap->width);
		texture_width = MIN(
			(real)source_width / bitmap_width,
			1.0f);
		bitmap_height = MAX(1.0f, (real)bitmap->height);
		texture_height = MIN(
			(real)source_height / bitmap_height,
			1.0f);

		for (vertex_index = 0;
			vertex_index < NUMBER_OF_POINTS_PER_RECTANGLE;
			vertex_index++)
		{
			vertices[vertex_index].color = argb;
			vertices[vertex_index].texture_coordinates.x =
				(vertex_index % 3) ? texture_width : 0.0f;
			vertices[vertex_index].texture_coordinates.y =
				(vertex_index > 1) ? texture_height : 0.0f;
			vertices[vertex_index].position = positions[vertex_index];
		}

		csmemset(&parameters, 0, sizeof(parameters));
		if (no_plasma)
		{
			parameters.map_texture_scale[0].j = 1.0f;
			parameters.map_texture_scale[0].i = 1.0f;
			parameters.map_scale[0].j = 1.0f;
			parameters.map_scale[0].i = 1.0f;
			parameters.map[0] = bitmap;
		}
		else
		{
			struct bitmap_data *plasma_bitmap = TAG_BLOCK_GET_ELEMENT(
				&bitmap_group_get(
					interface_get_tag_index(_interface_bitmap_iface_map3))->bitmaps,
				0,
				struct bitmap_data);
			long time = system_milliseconds();

			map0_offset.x =
				compute_offset_coordinate(time, 0.03215434f) * 311.0f;
			map0_offset.y =
				compute_offset_coordinate(time, 0.026795285f) * 311.0f;
			map1_offset.x =
				-compute_offset_coordinate(time, 0.035536603f);
			map1_offset.x *= 201.0f;
			map1_offset.y =
				-compute_offset_coordinate(time, 0.031094525f);
			map1_offset.y *= 201.0f;

			parameters.map[0] = plasma_bitmap;
			parameters.map0_to_1_blend_function = 5;
			parameters.map_scale[0].i = 1.0f;
			parameters.map_scale[0].j = 1.0f;
			parameters.map_wrapped[0] = TRUE;
			parameters.map_anchor_screen[0] = TRUE;
			parameters.map_texture_scale[0].i = 1.0f / 311.0f;
			parameters.map_texture_scale[0].j = 1.0f / 311.0f;
			parameters.map_tint[0] = &map_tint;
			parameters.map_fade[0] = &map_fade;
			parameters.map_offset[0] = &map0_offset;
			parameters.map[1] = plasma_bitmap;
			parameters.map1_to_2_blend_function = 0;
			parameters.map_scale[1].i = 1.0f;
			parameters.map_scale[1].j = 1.0f;
			parameters.map_wrapped[1] = TRUE;
			parameters.map_anchor_screen[1] = TRUE;
			parameters.map_texture_scale[1].i = 1.0f / 201.0f;
			parameters.map_texture_scale[1].j = 1.0f / 201.0f;
			parameters.map_tint[1] = &map_tint;
			parameters.map_fade[1] = &map_fade;
			parameters.map_offset[1] = &map1_offset;
			parameters.plasma_fade = plasma_fade;
			parameters.map_fade[2] = NULL;
			parameters.map_texture_scale[2].j = 1.0f;
			parameters.map_texture_scale[2].i = 1.0f;
			parameters.map_scale[2].j = 1.0f;
			parameters.map_scale[2].i = 1.0f;
			parameters.map[2] = bitmap;
			parameters.doing_plasma_effect = TRUE;
		}

		parameters.meter_parameters = NULL;
		parameters.point_sampled = FALSE;
		parameters.framebuffer_blend_function = 0;
		rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);
	}

	return;
}

void ui_widgets_set_fade_value(
	real value)
{
	widget_globals.fade_to_black = value;

	return;
}

void ui_widget_debug_show_path(
	boolean show)
{
	widget_globals.debug_show_path = show;

	return;
}

int widget_instance_count_children(
	struct widget_instance *widget)
{
	int count;
	struct widget_instance *child;

	count = 0;
	if (widget)
	{
		for (child = widget->child; child; child = child->next)
			count++;
	}
	return count;
}

struct widget_instance *widget_instance_get_nth_child(
	struct widget_instance *widget,
	int n)
{
	int i;
	struct widget_instance *result;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		1050,
		widget);
	result = widget->child;
	for (i = 0; i < n && result; i++)
		result = result->next;

	return result;
}

void widget_instance_set_visibility_recursive(
	struct widget_instance *widget,
	boolean visible)
{
	struct widget_instance *child;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		1859,
		widget);
	widget->visible = visible;
	for (child = widget->child; child; child = child->next)
		widget_instance_set_visibility_recursive(child, visible);

	return;
}

void *ui_widget_realloc(
	void *pointer,
	word size,
	char const *file,
	unsigned long line)
{
	return pool_resize_pointer(
		widget_memory_pool,
		pointer,
		size,
		file,
		line);
}

void widget_free(
	void *ptr)
{
	dispose_pointer(widget_memory_pool, ptr);

	return;
}

void main_menu_active(
	boolean active)
{
	we_are_at_the_main_menu = active;

	return;
}

boolean main_menu_is_active(
	void)
{
	return we_are_at_the_main_menu;
}

void ui_widget_load_progress_widget(
	void)
{
	error(
		_error_silent,
		"the old loading progress screen has been replaced with glowy halo gravy");

	return;
}

boolean filesystem_check_thread_is_active(
	void)
{
	return widget_globals.initialization_thread != NULL;
}

void display_error_when_main_menu_loaded(
	short error_code)
{
	if (widget_globals.main_menu_deferred_error_code == NONE)
	{
		widget_globals.main_menu_deferred_error_code = error_code;
		return;
	}

	error(
		_error_silent,
		"there is already an error message queued for display at the main menu; ignoring this one");
	return;
}

void display_error_abort_to_dashboard_deferred(
	short error_code,
	boolean optional)
{
	if (widget_globals.deferred_dashboard_error_code == NONE)
	{
		widget_globals.deferred_dashboard_error_code = error_code;
		widget_globals.deferred_dashboard_optional = optional;
		return;
	}

	error(
		_error_silent,
		"there is already a deferred dashbaord error queued; ignoring this one!");
	return;
}

boolean ui_main_menu_music_active(
	void)
{
	return widget_globals.main_menu_music_active;
}

void ui_widgets_disable_pause_game(
	long duration_ticks)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		2519,
		duration_ticks>=0);
	widget_globals.pause_disabled_ticks = duration_ticks;

	return;
}

struct widget_instance *widget_instance_get_topmost_parent(
	struct widget_instance *widget)
{
	while (widget->parent)
		widget = widget->parent;

	return widget;
}

int widget_instance_get_child_index_from_parent(
	struct widget_instance *widget)
{
	int result = NONE;

	if (widget->parent)
	{
		struct widget_instance *child = widget->parent->child;
		int index = 0;

		while (child)
		{
			if (child == widget)
			{
				result = index;
				break;
			}
			child = child->next;
			index++;
		}
	}

	return result;
}

pixel32 modulate_pixel32_by_real_alpha(
	pixel32 argb,
	real alpha)
{
	real modulated_alpha = (argb >> 24) * alpha;

	return (fast_ftol(modulated_alpha) << 24) | (argb & 0x00FFFFFF);
}

void ui_set_next_level(
	short level)
{
	long level_index = level;

	if (level_index != NONE)
	{
		if (level_index >= 0 && level_index <= 9)
		{
			main_set_map_name(main_get_solo_level_name(level));
			main_disallow_persistent_storage();
		}
		else
		{
			error(_error_silent, "unknown level");
			main_goto_main_menu();
		}
	}
	else
	{
		main_roll_credits();
	}

	return;
}

boolean ui_widgets_active(
	void)
{
	boolean result = FALSE;

	if (widget_globals.initialized)
	{
		long local_player_index;

		for (local_player_index = 0;
			local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
			local_player_index++)
		{
			if (widget_globals.active_widgets[local_player_index])
			{
				result = TRUE;
				break;
			}
		}
	}

	return result;
}

boolean main_menu_screen_is_active(
	void)
{
	if (we_are_at_the_main_menu == TRUE &&
		widget_globals.active_widgets[0] &&
		strcmp(widget_globals.active_widgets[0]->name, "the_main_menu") == 0)
	{
		return TRUE;
	}

	return FALSE;
}

static void *pool_alloc(
	unsigned long size)
{
	return match_malloc("c:\\halo\\SOURCE\\interface\\ui_widget.c", 117, size);
}

static void pool_free(
	void *pointer)
{
	match_free("c:\\halo\\SOURCE\\interface\\ui_widget.c", 118, pointer);

	return;
}

void ui_widgets_initialize(
	void)
{
	boolean success = TRUE;
	byte *base_address;
	long local_player_index;

	base_address = pool_alloc(WIDGET_MEMORY_POOL_SIZE);
	if (base_address)
	{
		widget_memory_pool->base_address = base_address;
		widget_memory_pool->size = WIDGET_MEMORY_POOL_SIZE;
	}
	else
	{
		success = FALSE;
	}
	stack_memory_pool_reset(widget_memory_pool);

	memset(&widget_globals, 0, sizeof(widget_globals));
	widget_globals.main_menu_deferred_error_code = NONE;
	widget_globals.deferred_dashboard_error_code = NONE;
	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		widget_globals.deferred_errors[local_player_index].error_code = NONE;
		widget_globals.deferred_cinematic_errors[local_player_index].error_code = NONE;
	}
	widget_globals.initialized = success;
	widget_globals.fade_to_black = -1.0f;

	return;
}

void ui_widgets_dispose(
	void)
{
	ui_widgets_close_all();
	if (widget_memory_pool->base_address)
		pool_free(widget_memory_pool->base_address);
	widget_memory_pool->base_address = NULL;
	widget_memory_pool->size = 0;
	memset(&widget_globals, 0, sizeof(widget_globals));

	return;
}

static void pop_widget(
	struct widget_stack_node **top,
	struct widget_stack_data *data)
{
	struct widget_stack_node *node;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		2556,
		top && data);
	node = *top;
	*data = node->data;
	*top = node->next;
	dispose_pointer(widget_memory_pool, node);

	return;
}

static void dispose_widget_stack(
	struct widget_stack_node **top)
{
	while (*top)
	{
		struct widget_stack_node *node = *top;

		*top = node->next;
		dispose_pointer(widget_memory_pool, node);
	}

	return;
}

void ui_widget_delete(
	struct widget_instance *widget)
{
	struct ui_widget_definition *definition;
	long handler_index;
	long widget_index;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		469,
		widget && widget_globals.initialized);
	if (widget->delete_recursion_lock)
		return;
	widget->delete_recursion_lock = TRUE;
	if (widget->local_player_index != NONE && !widget->parent)
		player_control_inhibit_buttons(
			widget->local_player_index,
			WIDGET_DELETED_PLAYER_CONTROL_INHIBIT_FLAGS,
			TRUE);
	definition = ui_widget_definition_get(widget->definition_tag_index);
	for (handler_index = 0;
		handler_index < definition->event_handlers.count;
		handler_index++)
	{
		struct ui_widget_event_handler_reference *handler =
			(struct ui_widget_event_handler_reference *)definition->event_handlers.address + handler_index;

		if (handler->event_type == _widget_event_deleted &&
			TEST_FLAG(handler->flags, _event_handler_run_function_bit))
		{
			boolean widget_deleted = FALSE;
			boolean handled = ui_widget_event_handler_function_invoke(
				widget,
				NULL,
				handler->function,
				&widget_deleted);

			match_vassert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				520,
				!widget_deleted,
				"a 'widget deleted' event handler tried to delete the widget being deleted!");
			if (handled == TRUE &&
				TEST_FLAG(handler->flags, _event_handler_open_widget_bit))
			{
				long new_widget_tag_index = handler->widget_tag.index;

				if (new_widget_tag_index != NONE &&
					!ui_widget_launch_widget(widget, new_widget_tag_index))
				{
					error(_error_silent, "event handler failed to spawn widget");
				}
			}
		}
	}
	if (widget->pause_game_time == TRUE)
	{
		match_vassert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			544,
			widget_globals.pause_game_time_count > 0,
			"widget pause counter out of whack");
		if (--widget_globals.pause_game_time_count == 0)
		{
			if (game_time_get_paused())
			{
				game_time_set_paused(FALSE);
				if (we_are_at_the_main_menu)
				{
					main_menu_ensure_player_queues_exist();
					game_time_dispose_from_old_map();
					game_time_initialize_for_new_map();
					game_time_start();
				}
			}
			if (widget_globals.sound_paused == TRUE)
			{
				sound_pause(FALSE);
				widget_globals.sound_paused = FALSE;
			}
		}
	}
	ui_widget_delete_children_recursive(widget);
	if (widget->previous)
		widget->previous->next = widget->next;
	if (widget->next)
		widget->next->previous = widget->previous;
	if (widget->parent && widget->parent->child == widget)
		widget->parent->child = widget->next;
	switch (widget->type)
	{
	case _ui_widget_type_text_box:
		if (widget->parameters.text_box.text)
			dispose_pointer(widget_memory_pool, widget->parameters.text_box.text);
		break;
	case _ui_widget_type_spinner_list:
	case _ui_widget_type_column_list:
		if (widget->parameters.list.list_items)
		{
			error(
				_error_silent,
				"###WARNING: possible memory leak disposing of a list widget (%s)",
				definition->name ? definition->name : "<unknown>");
		}
		if (widget->parameters.list.item_text)
			dispose_pointer(widget_memory_pool, widget->parameters.list.item_text);
		if (widget->parameters.list.extended_description)
			ui_widget_delete(widget->parameters.list.extended_description);
		break;
	}
	dispose_pointer(widget_memory_pool, widget);
	for (widget_index = 0;
		widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		widget_index++)
	{
		if (widget_globals.active_widgets[widget_index] == widget)
		{
			widget_globals.active_widgets[widget_index] = NULL;
			break;
		}
	}

	return;
}

static void ui_widget_delete_children_recursive(
	struct widget_instance *widget)
{
	struct widget_instance *child = widget->child;

	while (child)
	{
		struct widget_instance *next = child->next;

		match_assert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			2750,
			child->previous == NULL);
		if (next)
		{
			match_assert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				2754,
				next->previous == child);
		}
		ui_widget_delete(child);
		if (next)
			next->previous = NULL;
		child = next;
	}

	return;
}

static struct widget_instance *ui_widget_launch_widget(
	struct widget_instance *widget,
	long widget_tag_index)
{
	struct ui_widget_definition *definition = ui_widget_definition_get(widget_tag_index);
	struct widget_instance *root;
	struct widget_instance *new_widget;
	short local_player_index;

	if (TEST_FLAG(definition->flags, _widget_always_use_tag_controller_index_bit))
	{
		switch (definition->controller_index)
		{
		case _widget_controller0:
			local_player_index = 0;
			break;
		case _widget_controller1:
			local_player_index = 1;
			break;
		case _widget_controller2:
			local_player_index = 2;
			break;
		case _widget_controller3:
			local_player_index = 3;
			break;
		case _widget_controller_any:
			local_player_index = NONE;
			break;
		default:
			match_vassert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				5380,
				FALSE,
				"invalid widget controller index specified");
			break;
		}
	}
	else
	{
		switch (definition->controller_index)
		{
		case _widget_controller0:
			local_player_index = 0;
			break;
		case _widget_controller1:
			local_player_index = 1;
			break;
		case _widget_controller2:
			local_player_index = 2;
			break;
		case _widget_controller3:
			local_player_index = 3;
			break;
		case _widget_controller_any:
			local_player_index = widget->local_player_index;
			break;
		default:
			match_vassert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				5392,
				FALSE,
				"invalid widget controller index specified");
			break;
		}
	}
	root = widget_instance_get_topmost_parent(widget);
	new_widget = ui_widget_load_by_name_or_tag(
		NULL,
		widget_tag_index,
		NULL,
		local_player_index,
		root->definition_tag_index,
		widget->parent ? widget->parent->definition_tag_index : NONE,
		(short)widget_instance_get_child_index_from_parent(widget));
	if (!new_widget)
		error(_error_silent, "event handler failed to spawn widget");

	return new_widget;
}

static __inline boolean widget_instance_can_handle_events(
	struct widget_instance *widget)
{
	struct ui_widget_definition *definition = ui_widget_definition_get(widget->definition_tag_index);

	if (!widget->disabled &&
		(definition->event_handlers.count > 0 ||
		widget->type == _ui_widget_type_spinner_list ||
		widget->type == _ui_widget_type_column_list))
	{
		return TRUE;
	}

	return FALSE;
}

static struct widget_instance *widget_instance_find_by_tag_index_recursive(
	struct widget_instance *widget,
	long tag_index)
{
	struct widget_instance *result = NULL;

	if (widget->definition_tag_index == tag_index)
	{
		result = widget;
	}
	else
	{
		struct widget_instance *child;

		for (child = widget->child; child && !result; child = child->next)
		{
			if (child->definition_tag_index == tag_index)
				result = child;
			else
				result = widget_instance_find_by_tag_index_recursive(child, tag_index);
		}
	}

	return result;
}

static void widget_instance_give_focus_directly(
	struct widget_instance *widget,
	struct widget_instance *new_focus)
{
	struct widget_instance *focused_child =
		widget_instance_get_topmost_parent(widget)->focused_child;

	if (new_focus->disabled == TRUE)
	{
		struct widget_instance *substitute;

		for (substitute = new_focus->next; substitute; substitute = substitute->next)
		{
			if (widget_instance_can_handle_events(substitute))
				break;
		}
		if (!substitute && new_focus->parent)
		{
			for (substitute = new_focus->parent->child; substitute; substitute = substitute->next)
			{
				if (widget_instance_can_handle_events(substitute))
					break;
			}
			if (substitute == new_focus->parent->focused_child)
			{
				for (substitute = new_focus->previous; substitute; substitute = substitute->previous)
				{
					if (widget_instance_can_handle_events(substitute))
						break;
				}
			}
		}
		if (substitute)
			new_focus = substitute;
	}
	if (focused_child)
	{
		if (new_focus &&
			focused_child->parent == new_focus->parent &&
			focused_child->parent)
		{
			new_focus->parent->focused_child = new_focus;

			return;
		}
		while (focused_child)
		{
			focused_child->parent->focused_child = NULL;
			focused_child = focused_child->focused_child;
		}
	}
	while (new_focus->parent)
	{
		new_focus->parent->focused_child = new_focus;
		new_focus = new_focus->parent;
	}

	return;
}

static void widget_instance_give_focus_by_tag(
	struct widget_instance *widget,
	long tag_index,
	short local_player_index)
{
	struct widget_instance *root = widget_instance_get_topmost_parent(widget);
	struct widget_instance *new_focus = widget_instance_find_by_tag_index_recursive(root, tag_index);

	if (new_focus)
		widget_instance_give_focus_directly(root, new_focus);
	else
		error(_error_silent, "failed to find event focus target widget");

	return;
}

boolean widget_event_function_list_widget_goto_next_item(
	struct widget_instance *widget,
	struct event_record *event,
	boolean *widget_deleted)
{
	struct ui_widget_definition *definition;
	struct widget_instance *child;
	long item_index;
	boolean result = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		1230,
		widget);
	definition = ui_widget_definition_get(widget->definition_tag_index);
	match_vassert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		1233,
		widget->type == _ui_widget_type_spinner_list || widget->type == _ui_widget_type_column_list,
		"calling a list widget function on a non-list widget");
	if (widget->parameters.list.list_items && widget->parameters.list.number_of_items > 0)
	{
		item_index = widget->parameters.list.selected_index + 1;
		if (item_index >= widget->parameters.list.number_of_items)
			item_index = 0;
		if (widget->type == _ui_widget_type_column_list)
		{
			child = widget_instance_get_nth_child(widget, item_index);
			if (child)
			{
				widget_instance_give_focus_by_tag(
					widget,
					child->definition_tag_index,
					widget->local_player_index);
				widget->parameters.list.selected_index = (short)item_index;
			}
			else
			{
				error(
					_error_silent,
					"failed to set focus to the #%d list item of a column list widget",
					item_index);
				result = FALSE;
			}
		}
		else if (widget->type == _ui_widget_type_spinner_list)
		{
			if (definition->child_widgets.count > 1)
			{
				match_vassert(
					"c:\\halo\\SOURCE\\interface\\ui_widget.c",
					1268,
					definition->child_widgets.count == 3,
					"spinner lists must be either 1- or 3-wide... sorry");
				if (widget->focused_child == widget->child ||
					widget->focused_child == widget->child->next)
				{
					if (widget->focused_child->next)
						widget_instance_give_focus_directly(widget, widget->focused_child->next);
				}
			}
			widget->parameters.list.selected_index = (short)item_index;
		}
	}
	else
	{
		if (widget->type == _ui_widget_type_spinner_list)
		{
			match_vassert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				1288,
				definition->child_widgets.count <= 1,
				"spinner lists with more that 1 visible item need to have code-generated lists associated with them... sorry.");
		}
		if (widget->type == _ui_widget_type_spinner_list &&
			TEST_FLAG(definition->list_flags, _list_items_generated_from_string_list_tag) &&
			definition->child_widgets.count == 0)
		{
			widget->parameters.list.selected_index++;
			if (widget->parameters.list.selected_index == widget->parameters.list.number_of_items)
				widget->parameters.list.selected_index = 0;
		}
		else
		{
			child = NULL;
			if (widget->focused_child)
			{
				item_index = widget->parameters.list.selected_index + 1;
				child = widget->focused_child->next;
				if (item_index == widget->parameters.list.number_of_items)
					child = NULL;
			}
			if (!child)
			{
				child = widget->child;
				item_index = 0;
			}
			if (child)
			{
				widget_instance_give_focus_by_tag(
					widget,
					child->definition_tag_index,
					widget->local_player_index);
				widget->parameters.list.selected_index = (short)item_index;
			}
			else
			{
				error(
					_error_silent,
					"failed to set focus to the next list item of a column widget");
				result = FALSE;
			}
		}
	}
	if (result)
		widget->parameters.list.last_list_tab_direction = 15;

	return result;
}

boolean widget_event_function_list_widget_goto_previous_item(
	struct widget_instance *widget,
	struct event_record *event,
	boolean *widget_deleted)
{
	struct ui_widget_definition *definition;
	struct widget_instance *child;
	long item_index;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		1359,
		widget);
	definition = ui_widget_definition_get(widget->definition_tag_index);
	match_vassert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		1362,
		widget->type == _ui_widget_type_spinner_list || widget->type == _ui_widget_type_column_list,
		"calling a list widget function on a non-list widget");
	if (widget->parameters.list.list_items && widget->parameters.list.number_of_items > 0)
	{
		item_index = widget->parameters.list.selected_index - 1;
		if (item_index < 0)
			item_index = widget->parameters.list.number_of_items - 1;
		if (widget->type == _ui_widget_type_column_list)
		{
			child = widget_instance_get_nth_child(widget, item_index);
			if (child)
			{
				widget_instance_give_focus_by_tag(
					widget,
					child->definition_tag_index,
					widget->local_player_index);
				widget->parameters.list.selected_index = (short)item_index;
			}
			else
			{
				error(
					_error_silent,
					"failed to set focus to the #%d list item of a column list widget",
					item_index);

				return FALSE;
			}
		}
		else if (widget->type == _ui_widget_type_spinner_list)
		{
			if (definition->child_widgets.count > 1)
			{
				match_vassert(
					"c:\\halo\\SOURCE\\interface\\ui_widget.c",
					1396,
					definition->child_widgets.count == 3,
					"spinner lists must be either 1- or 3-wide... sorry");
				if (widget->focused_child != widget->child)
				{
					match_assert(
						"c:\\halo\\SOURCE\\interface\\ui_widget.c",
						1405,
						widget->focused_child);
					if (widget->focused_child->previous)
						widget_instance_give_focus_directly(widget, widget->focused_child->previous);
				}
			}
			widget->parameters.list.selected_index = (short)item_index;
		}
	}
	else
	{
		if (widget->type == _ui_widget_type_spinner_list)
		{
			match_vassert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				1420,
				definition->child_widgets.count <= 1,
				"spinner lists with more that 1 visible item need to have code-generated lists associated with them... sorry.");
		}
		if (widget->type == _ui_widget_type_spinner_list &&
			TEST_FLAG(definition->list_flags, _list_items_generated_from_string_list_tag) &&
			definition->child_widgets.count == 0)
		{
			widget->parameters.list.selected_index--;
			if (widget->parameters.list.selected_index < 0)
				widget->parameters.list.selected_index = widget->parameters.list.number_of_items - 1;
		}
		else
		{
			child = NULL;
			if (widget->focused_child)
			{
				item_index = widget->parameters.list.selected_index - 1;
				child = widget->focused_child->previous;
			}
			if (!child)
			{
				child = widget->child;
				item_index = 0;
				while (child->next)
				{
					child = child->next;
					item_index++;
				}
			}
			widget_instance_give_focus_by_tag(
				widget,
				child->definition_tag_index,
				widget->local_player_index);
			widget->parameters.list.selected_index = (short)item_index;
		}
	}
	widget->parameters.list.last_list_tab_direction = -15;

	return TRUE;
}

void ui_widgets_close_all(
	void)
{
	long local_player_index;

	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		if (widget_globals.active_widgets[local_player_index])
			ui_widget_delete(widget_globals.active_widgets[local_player_index]);
		if (widget_globals.widget_stack[local_player_index])
			dispose_widget_stack(&widget_globals.widget_stack[local_player_index]);
	}

	return;
}

void ui_widgets_close_all_for_local_player(
	short local_player_index)
{
	long widget_index;

	match_vassert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		1154,
		local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS,
		"expected a valid local_player_index");
	for (widget_index = 0;
		widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		widget_index++)
	{
		struct widget_instance *widget = widget_globals.active_widgets[widget_index];

		if (widget && widget->local_player_index == local_player_index)
		{
			ui_widget_delete(widget);
			if (widget_globals.widget_stack[widget_index])
				dispose_widget_stack(&widget_globals.widget_stack[widget_index]);
		}
	}

	return;
}

void ui_widgets_delete_history(
	void)
{
	long local_player_index;

	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		if (widget_globals.widget_stack[local_player_index])
			dispose_widget_stack(&widget_globals.widget_stack[local_player_index]);
	}

	return;
}

void ui_widgets_pop_stack(
	short local_player_index)
{
	struct widget_stack_data data;

	if (local_player_index == NONE)
	{
		local_player_index = 0;
	}
	else
	{
		match_assert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			1204,
			(local_player_index>=0) && (local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));
	}
	if (widget_globals.widget_stack[local_player_index])
		pop_widget(&widget_globals.widget_stack[local_player_index], &data);

	return;
}

void main_screen_shell_begin_fade(
	unsigned long fade_duration_milliseconds)
{
	long local_player_index;

	ui_stop_main_menu_music();
	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		if (widget_globals.active_widgets[local_player_index] &&
			!widget_globals.active_widgets[local_player_index]->widget_is_error_dialog)
		{
			widget_globals.active_widgets[local_player_index]->auto_close_fade_time = fade_duration_milliseconds;
			widget_globals.active_widgets[local_player_index]->milliseconds_to_auto_close =
				widget_globals.current_system_milliseconds -
				widget_globals.active_widgets[local_player_index]->creation_time + 100;
			if (widget_globals.widget_stack[local_player_index])
				dispose_widget_stack(&widget_globals.widget_stack[local_player_index]);
		}
	}

	return;
}

static void play_sound_tag(
	long sound_tag_index)
{
	if (sound_tag_index != NONE)
		unspatialized_impulse_sound_new(sound_tag_index, 1.0f);

	return;
}

void ui_play_audio_feedback_sound(
	short audio_feedback)
{
	switch (audio_feedback)
	{
	case _ui_audio_feedback_cursor:
		play_sound_tag(tag_loaded(SOUND_DEFINITION_TAG, "sound\\sfx\\ui\\cursor"));
		break;
	case _ui_audio_feedback_forward:
		play_sound_tag(tag_loaded(SOUND_DEFINITION_TAG, "sound\\sfx\\ui\\forward"));
		break;
	case _ui_audio_feedback_back:
		play_sound_tag(tag_loaded(SOUND_DEFINITION_TAG, "sound\\sfx\\ui\\back"));
		break;
	case _ui_audio_feedback_flag_failure:
		play_sound_tag(tag_loaded(SOUND_DEFINITION_TAG, "sound\\sfx\\ui\\flag_failure"));
		break;
	}

	return;
}

static __inline struct widget_instance *widget_instance_get_tail_child_widget(
	struct widget_instance *widget)
{
	struct widget_instance *child = widget->child;

	if (child)
	{
		while (child->next)
			child = child->next;
	}

	return child;
}

static void ui_widget_add_child(
	struct widget_instance *parent,
	struct widget_instance *child)
{
	struct widget_instance *tail_child = NULL;
	struct widget_instance *sibling = parent->child;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		2719,
		(child->previous == NULL) && (child->next == NULL));
	while (sibling)
	{
		tail_child = sibling;
		sibling = sibling->next;
	}
	if (tail_child)
	{
		match_assert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			2729,
			tail_child->next == NULL);
		tail_child->next = child;
		child->previous = tail_child;
	}
	else
	{
		parent->child = child;
	}

	return;
}

static void push_widget(
	struct widget_stack_node **top,
	struct widget_stack_data *data)
{
	struct widget_stack_node *node = pool_new_pointer(
		widget_memory_pool,
		sizeof(struct widget_stack_node),
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		2532);

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		2534,
		top && data);
	if (node)
	{
		node->data = *data;
		node->next = *top;
		*top = node;
	}
	else
	{
		match_vwarn(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			2544,
			FALSE,
			"out of memory! the UI screen history will be hosed.");
	}

	return;
}

static boolean widget_instance_can_receive_events(
	struct widget_instance *widget)
{
	if (widget->disabled)
		return FALSE;
	if (widget->parent)
	{
		/* NOTE January carries the parent's definition over from one ancestor to
		the next: the flags it tests belong to the ancestor BELOW the one whose
		type it tests, from the second iteration onward.  Reproduced, not
		repaired - the June 2011 rebuild of this function decompiles to the same
		carried-over definition pointer */
		struct ui_widget_definition *definition =
			ui_widget_definition_get(widget->parent->definition_tag_index);
		struct widget_instance *parent;
		boolean result = TRUE;

		for (parent = widget->parent; parent && result; parent = parent->parent)
		{
			struct ui_widget_definition *parent_definition =
				ui_widget_definition_get(parent->definition_tag_index);

			result = TEST_FLAG(definition->flags, _widget_pass_unhandled_events_to_children_bit) ||
				parent->type == _ui_widget_type_spinner_list ||
				parent->type == _ui_widget_type_column_list;
			definition = parent_definition;
		}

		return result;
	}

	return TRUE;
}

static void widget_instance_set_focused_child_by_index(
	long tag_index,
	struct widget_instance *widget,
	short child_index)
{
	struct widget_instance *parent;

	if (tag_index == NONE)
		return;
	parent = widget_instance_find_by_tag_index_recursive(widget, tag_index);
	if (!parent)
		return;
	if (child_index >= 0)
	{
		struct widget_instance *child;
		long index;

		for (child = parent->child, index = 0; child; child = child->next, index++)
		{
			if (child->type == _ui_widget_type_spinner_list &&
				ui_widget_definition_get(child->definition_tag_index)->child_widgets.count > 1)
			{
				return;
			}
			if (index == child_index)
			{
				widget_instance_give_focus_directly(widget, child);
				if (child->parent &&
					(child->parent->type == _ui_widget_type_spinner_list ||
					child->parent->type == _ui_widget_type_column_list))
				{
					child->parent->parameters.list.selected_index = (short)index;
				}

				return;
			}
		}
	}
	else
	{
		/* with no index the found widget takes the focus itself, unless it is a
		multiple-item spinner list, which owns the focus of its own items */
		if (widget_instance_can_receive_events(parent) &&
			!(parent->type == _ui_widget_type_spinner_list &&
			ui_widget_definition_get(parent->definition_tag_index)->child_widgets.count > 1))
		{
			widget_instance_give_focus_directly(widget, parent);
		}
	}

	return;
}

static void widget_instance_go_back_to_previous(
	struct widget_instance *widget)
{
	struct widget_stack_data data;
	short widget_stack = (widget->local_player_index == NONE) ? 0 : widget->local_player_index;
	short previous_local_player_index = NONE;
	long previous_widget_tag;

	if (widget_globals.widget_stack[widget_stack])
	{
		pop_widget(&widget_globals.widget_stack[widget_stack], &data);
		previous_local_player_index = data.local_player_index;
		previous_widget_tag = data.previous_widget_tag;
	}
	else
	{
		previous_widget_tag = NONE;
	}
	ui_widget_delete(widget_instance_get_topmost_parent(widget));
	if (previous_widget_tag != NONE)
	{
		struct widget_instance *new_widget = ui_widget_load_by_name_or_tag(
			NULL,
			previous_widget_tag,
			NULL,
			previous_local_player_index,
			NONE,
			NONE,
			NONE);

		if (new_widget)
		{
			widget_instance_set_focused_child_by_index(
				data.focused_child_parent_widget_tag,
				new_widget,
				data.focused_child_index);
		}
	}

	return;
}

static __inline struct widget_instance *widget_instance_find_by_tag_index(
	long tag_index)
{
	struct widget_instance *result = NULL;
	long widget_index;

	for (widget_index = 0;
		widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS && !result;
		widget_index++)
	{
		if (widget_globals.active_widgets[widget_index])
		{
			result = widget_instance_find_by_tag_index_recursive(
				widget_globals.active_widgets[widget_index],
				tag_index);
		}
	}

	return result;
}

static void event_handler_dispatch(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	struct event_record *event,
	struct ui_widget_event_handler_reference *handler,
	boolean *calling_widget_deleted)
{
	boolean widget_deleted = FALSE;
	boolean success = TRUE;
	boolean function_failed = FALSE;
	long audio_feedback = _ui_audio_feedback_none;
	boolean close_widget_after = FALSE;
	boolean close_current = FALSE;
	boolean close_all = FALSE;

	if (TEST_FLAG(handler->flags, _event_handler_run_scenario_script_bit) &&
		handler->script[0])
	{
		if (!hs_evaluate_by_name(handler->script))
			error(_error_silent, "failed to run ui widget event script '%s'", handler->script);
	}
	if (TEST_FLAG(handler->flags, _event_handler_run_function_bit) &&
		!widget_deleted &&
		!ui_widget_event_handler_function_invoke(
			widget,
			event,
			handler->function,
			&widget_deleted))
	{
		error(_error_silent, "event handler function failed");
		function_failed = TRUE;
	}
	else
	{
		if (TEST_FLAG(handler->flags, _event_handler_give_focus_to_widget_bit) &&
			!widget_deleted)
		{
			if (handler->widget_tag.index != NONE)
			{
				widget_instance_give_focus_by_tag(
					widget,
					handler->widget_tag.index,
					widget->local_player_index);
				audio_feedback = _ui_audio_feedback_cursor;
			}
			else
			{
				error(
					_error_silent,
					"failed to give focus to a widget because event_handler->ui_widget_tag == NONE");
				success = FALSE;
			}
		}
		if (TEST_FLAG(handler->flags, _event_handler_reload_self_bit) &&
			!widget_deleted)
		{
			widget_instance_reload_recursive(widget);
		}
		if (TEST_FLAG(handler->flags, _event_handler_reload_widget_bit) &&
			!widget_deleted)
		{
			if (handler->widget_tag.index != NONE)
			{
				ui_widget_reload_by_tag(handler->widget_tag.index);
			}
			else
			{
				error(
					_error_silent,
					"failed to reload widget because event_handler->ui_widget_tag == NONE");
				success = FALSE;
			}
		}
		if (TEST_FLAG(handler->flags, _event_handler_close_current_widget_bit) &&
			!widget_deleted)
		{
			close_widget_after = TRUE;
		}
		if (TEST_FLAG(handler->flags, _event_handler_close_other_widget_bit) &&
			!widget_deleted &&
			handler->widget_tag.index != NONE)
		{
			struct widget_instance *other_widget =
				widget_instance_find_by_tag_index(handler->widget_tag.index);

			if (other_widget)
			{
				if (other_widget == widget)
					close_current = TRUE;
				else
					ui_widget_delete(other_widget);
			}
			else
			{
				error(
					_error_silent,
					"failed to close widget because event_handler->ui_widget_tag == NONE");
				success = FALSE;
			}
		}
		if (TEST_FLAG(handler->flags, _event_handler_close_all_widgets_bit) &&
			!widget_deleted)
		{
			close_all = TRUE;
		}
		if (TEST_FLAG(handler->flags, _event_handler_open_widget_bit) &&
			handler->widget_tag.index != NONE)
		{
			if (!ui_widget_launch_widget(widget, handler->widget_tag.index))
			{
				error(_error_silent, "event handler failed to spawn widget");
				success = FALSE;
			}
			else
			{
				if (audio_feedback == _ui_audio_feedback_none)
					audio_feedback = _ui_audio_feedback_forward;
				widget_deleted = TRUE;
			}
		}
		if (TEST_FLAG(handler->flags, _event_handler_replace_with_other_widget_bit) &&
			!widget_deleted &&
			handler->widget_tag.index != NONE)
		{
			struct widget_instance *new_widget = ui_widget_load_by_name_or_tag(
				NULL,
				handler->widget_tag.index,
				widget,
				widget->local_player_index,
				NONE,
				NONE,
				NONE);

			if (new_widget)
			{
				struct widget_instance *parent = widget->parent;
				struct widget_instance *next = widget->next;
				struct widget_instance *previous = widget->previous;
				long widget_index;

				if (new_widget->previous)
					new_widget->previous->next = NULL;
				new_widget->previous = NULL;
				new_widget->parent = NULL;
				new_widget->horizontal_offset += widget->horizontal_offset;
				new_widget->vertical_offset += widget->vertical_offset;
				if (parent)
				{
					new_widget->parent = parent;
					if (parent->child == widget)
						parent->child = new_widget;
					if (parent->focused_child == widget)
						parent->focused_child = new_widget;
				}
				if (next)
				{
					match_assert(
						"c:\\halo\\SOURCE\\interface\\ui_widget.c",
						3721,
						next->previous == widget);
					next->previous = new_widget;
				}
				new_widget->next = next;
				if (previous)
				{
					match_assert(
						"c:\\halo\\SOURCE\\interface\\ui_widget.c",
						3728,
						previous->next == widget);
					previous->next = new_widget;
				}
				new_widget->previous = previous;
				for (widget_index = 0;
					widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
					widget_index++)
				{
					if (widget_globals.active_widgets[widget_index] == new_widget)
					{
						widget_globals.active_widgets[widget_index] = NULL;
						break;
					}
				}
				if (audio_feedback == _ui_audio_feedback_none)
					audio_feedback = _ui_audio_feedback_forward;
				widget->previous = NULL;
				widget->next = NULL;
				widget->parent = NULL;
				close_current = TRUE;
			}
			else
			{
				error(
					_error_silent,
					"failed to open widget because the specified widget tag was not found");
				success = FALSE;
			}
		}
		if (TEST_FLAG(handler->flags, _event_handler_go_back_to_previous_widget_bit))
		{
			widget_instance_go_back_to_previous(widget);
			if (audio_feedback == _ui_audio_feedback_none)
				audio_feedback = _ui_audio_feedback_back;
			widget_deleted = TRUE;
		}
		if (handler->sound_effect.index != NONE)
			unspatialized_impulse_sound_new(handler->sound_effect.index, 1.0f);
		if (close_all)
		{
			long widget_index;

			for (widget_index = 0;
				widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
				widget_index++)
			{
				if (widget_globals.active_widgets[widget_index])
					ui_widget_delete(widget_globals.active_widgets[widget_index]);
				while (widget_globals.widget_stack[widget_index])
				{
					struct widget_stack_data data;

					pop_widget(&widget_globals.widget_stack[widget_index], &data);
				}
			}
			widget_deleted = TRUE;
		}
		else if (close_widget_after)
		{
			ui_widget_delete(widget_instance_get_topmost_parent(widget));
			widget_deleted = TRUE;
		}
		else if (close_current)
		{
			ui_widget_delete(widget);
			widget_deleted = TRUE;
		}
	}
	if (!success || function_failed)
	{
		if (TEST_FLAG(handler->flags, _event_handler_look_for_conditional_widget_on_failure_bit))
		{
			long conditional_index;

			for (conditional_index = 0;
				conditional_index < definition->conditional_widgets.count;
				conditional_index++)
			{
				struct ui_widget_conditional_reference *conditional =
					(struct ui_widget_conditional_reference *)definition->conditional_widgets.address +
					conditional_index;

				if (function_failed == TRUE &&
					TEST_FLAG(
						conditional->flags,
						_conditional_widget_load_if_event_handler_function_fails_bit))
				{
					if (!widget_deleted)
					{
						if (conditional->widget_tag.index != NONE)
						{
							if (!ui_widget_launch_widget(widget, conditional->widget_tag.index))
								error(_error_silent, "condition handler failed to spawn widget");
							else
								widget_deleted = TRUE;
						}
					}
					else
					{
						error(
							_error_silent,
							"couldn't load conditional widget because the calling widget was deleted");
					}
				}
			}
		}
	}
	ui_play_audio_feedback_sound(audio_feedback);
	*calling_widget_deleted = widget_deleted;

	return;
}

static boolean ui_widget_load_children_recursive(
	struct widget_instance *widget,
	struct ui_widget_definition *definition)
{
	boolean result = TRUE;
	long child_index;

	if (TEST_FLAG(definition->list_flags, _list_items_generated_from_string_list_tag))
	{
		struct string_list *string_list;
		long string_index;

		match_vassert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			2592,
			widget->type == _ui_widget_type_spinner_list,
			"_list_items_generated_from_string_list_tag flag should only be set for 1-wide spinner list widgets");
		match_vassert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			2594,
			definition->child_widgets.count == 0,
			"no child widget references are needed to define list items when generating a list from a string list tag");
		match_vassert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			2596,
			definition->text_label_string_list.index != NONE,
			"_list_items_generated_from_string_list_tag flag was set but no string list tag was specified");
		string_list = unicode_string_list_definition_get(definition->text_label_string_list.index);
		widget_globals.dont_load_children_recursive = TRUE;
		for (string_index = 0;
			string_index < string_list->strings.count;
			string_index++)
		{
			struct widget_instance *child = ui_widget_load_by_name_or_tag(
				NULL,
				widget->definition_tag_index,
				widget,
				widget->local_player_index,
				NONE,
				NONE,
				NONE);

			if (!child)
			{
				result = FALSE;
				break;
			}
			ui_widget_add_child(widget, child);
			widget->parameters.list.number_of_items++;
		}
		widget_globals.dont_load_children_recursive = FALSE;
	}
	for (child_index = 0;
		child_index < definition->child_widgets.count;
		child_index++)
	{
		struct ui_widget_child_reference *reference =
			(struct ui_widget_child_reference *)definition->child_widgets.address + child_index;
		short controller_index = widget->local_player_index;

		if (TEST_FLAG(reference->flags, _child_widget_use_custom_controller_index_bit))
		{
			if (reference->custom_controller_index >= 0 &&
				reference->custom_controller_index < MAXIMUM_GAMEPADS)
			{
				controller_index = reference->custom_controller_index;
			}
			else
			{
				error(
					_error_silent,
					"invalid controller index specified for child widget (#%d)",
					reference->custom_controller_index);
			}
		}
		if (reference->widget_tag.index != NONE)
		{
			struct widget_instance *child = ui_widget_load_by_name_or_tag(
				NULL,
				reference->widget_tag.index,
				widget,
				controller_index,
				NONE,
				NONE,
				NONE);

			if (!child)
			{
				result = FALSE;
				break;
			}
			child->horizontal_offset = reference->horizontal_offset + widget->horizontal_offset;
			child->vertical_offset = reference->vertical_offset + widget->vertical_offset;
			ui_widget_add_child(widget, child);
		}
	}
	if (widget->type == _ui_widget_type_column_list &&
		definition->extended_description_widget.index != NONE)
	{
		widget->parameters.list.extended_description = ui_widget_load_by_name_or_tag(
			NULL,
			definition->extended_description_widget.index,
			widget,
			widget->local_player_index,
			NONE,
			NONE,
			NONE);
		if (widget->parameters.list.extended_description)
		{
			if (widget->parameters.list.extended_description->previous)
				widget->parameters.list.extended_description->previous->next = NULL;
			widget->parameters.list.extended_description->previous = NULL;
			widget->parameters.list.extended_description->parent = NULL;
		}
	}
	if (!TEST_FLAG(definition->flags, _widget_dont_focus_a_specific_child_bit))
	{
		boolean focus_a_child = FALSE;

		if (widget->type == _ui_widget_type_spinner_list ||
			widget->type == _ui_widget_type_column_list)
		{
			widget->parameters.list.selected_index = 0;
			widget->parameters.list.last_list_tab_direction = 0;
			focus_a_child = TRUE;
		}
		else if (TEST_FLAG(definition->flags, _widget_pass_unhandled_events_to_children_bit))
		{
			focus_a_child = TRUE;
		}
		if (focus_a_child)
		{
			struct widget_instance *child;

			for (child = widget->child; child; child = child->next)
			{
				if (widget->type == _ui_widget_type_spinner_list ||
					widget->type == _ui_widget_type_column_list ||
					widget_instance_can_handle_events(child))
				{
					widget->focused_child = child;
					break;
				}
			}
		}
	}

	return result;
}

static void widget_instance_initialize(
	struct widget_instance *widget,
	struct widget_instance *parent,
	struct ui_widget_definition *definition,
	long tag_index,
	short local_player_index,
	short widget_stack)
{
	long handler_index;

	memset(widget, 0, sizeof(struct widget_instance));
	if (TEST_FLAG(definition->list_flags, _list_items_generated_from_string_list_tag) &&
		parent &&
		tag_index == parent->definition_tag_index)
	{
		widget->type = _ui_widget_type_text_box;
	}
	widget->definition_tag_index = tag_index;
	widget->name = definition->name;
	widget->local_player_index = local_player_index;
	widget->type = definition->type;
	widget->visible = TRUE;
	widget->render_regardless_of_controller_index =
		TEST_FLAG(definition->flags, _widget_render_regardless_of_controller_index_bit);
	widget->pause_game_time = TEST_FLAG(definition->flags, _widget_pause_game_time_bit);
	widget->creation_time = widget_globals.current_system_milliseconds;
	widget->milliseconds_to_auto_close = MAX(definition->milliseconds_to_auto_close, 0);
	widget->auto_close_fade_time = MAX(definition->auto_close_fade_time, 0);
	widget->alpha_modifier = 1.0f;
	widget->parent = parent;
	switch (widget->type)
	{
	case _ui_widget_type_text_box:
		widget->parameters.text_box.string_list_index = NONE;
		break;
	}
	if (definition->background_bitmap.index != NONE)
	{
		widget->animation.number_of_sprite_frames = TAG_BLOCK_GET_ELEMENT(
			&bitmap_group_get(definition->background_bitmap.index)->sequences,
			0,
			struct bitmap_group_sequence)->bitmap_count;
	}
	if (!widget_globals.dont_load_children_recursive)
	{
		if (!ui_widget_load_children_recursive(widget, definition))
			error(_error_silent, "failed to load widget children");
	}
	for (handler_index = 0;
		handler_index < definition->event_handlers.count;
		handler_index++)
	{
		struct ui_widget_event_handler_reference *handler =
			(struct ui_widget_event_handler_reference *)definition->event_handlers.address + handler_index;

		if (handler->event_type == _widget_event_created)
		{
			struct event_record event = {0};
			boolean widget_deleted;

			event.controller_index = widget->local_player_index;
			event_handler_dispatch(widget, definition, &event, handler, &widget_deleted);
		}
	}
	if (!widget->focused_child)
	{
		struct widget_instance *child;

		for (child = widget->child; child; child = child->next)
		{
			if (widget_instance_can_handle_events(child))
				widget_instance_give_focus_directly(widget, child);
		}
	}
	if (widget->pause_game_time == TRUE)
	{
		widget_globals.pause_game_time_count++;
		if (!game_time_get_paused())
			game_time_set_paused(TRUE);
		if (!widget_globals.sound_paused && !we_are_at_the_main_menu)
		{
			sound_pause(TRUE);
			widget_globals.sound_paused = TRUE;
		}
	}

	return;
}

struct widget_instance *ui_widget_load_by_name_or_tag(
	char const *name,
	long tag_index,
	struct widget_instance *parent,
	short local_player_index,
	long invoking_widget_tag,
	long focused_child_parent_widget_tag,
	short focused_child_index)
{
	struct ui_widget_definition *definition;
	struct widget_instance *widget = NULL;
	short widget_stack = (local_player_index == NONE) ? 0 : local_player_index;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		377,
		widget_globals.initialized);
	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		378,
		(name != NULL) || (tag_index != NONE));
	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		379,
		(widget_stack>=0) && (widget_stack<MAXIMUM_GAMEPADS));
	if (tag_index == NONE)
		tag_index = tag_loaded(UI_WIDGET_DEFINITION_TAG, name);
	if (tag_index != NONE)
	{
		definition = ui_widget_definition_get(tag_index);
		widget = pool_new_pointer(
			widget_memory_pool,
			sizeof(struct widget_instance),
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			395);
		if (widget)
		{
			if (!parent)
			{
				short previous_local_player_index;

				if (widget_globals.active_widgets[widget_stack])
				{
					previous_local_player_index =
						widget_globals.active_widgets[widget_stack]->local_player_index;
					ui_widget_delete(widget_globals.active_widgets[widget_stack]);
				}
				else
				{
					previous_local_player_index = NONE;
				}
				widget_globals.active_widgets[widget_stack] = widget;
				if (invoking_widget_tag != NONE &&
					!TEST_FLAG(
						ui_widget_definition_get(invoking_widget_tag)->flags,
						_widget_dont_push_history_data_bit))
				{
					struct widget_stack_data data;

					data.previous_widget_tag = invoking_widget_tag;
					data.focused_child_parent_widget_tag = focused_child_parent_widget_tag;
					data.focused_child_index = focused_child_index;
					data.local_player_index = previous_local_player_index;
					push_widget(&widget_globals.widget_stack[widget_stack], &data);
				}
			}
			if (local_player_index == NONE)
			{
				switch (definition->controller_index)
				{
				case _widget_controller0:
					local_player_index = 0;
					break;
				case _widget_controller1:
					local_player_index = 1;
					break;
				case _widget_controller2:
					local_player_index = 2;
					break;
				case _widget_controller3:
					local_player_index = 3;
					break;
				case _widget_controller_any:
					local_player_index = NONE;
					break;
				}
			}
			widget_instance_initialize(
				widget,
				parent,
				definition,
				tag_index,
				local_player_index,
				widget_stack);
		}
		else
		{
			error(_error_silent, "failed to create new widget; out of memory!");
		}
	}
	else
	{
		error(_error_silent, "ui_widget_definition tag '%s'/%d not loaded", name, NONE);
	}

	return widget;
}

static void render_state_text(
	rectangle2d *bounds,
	rectangle2d *cursor_bounds,
	wchar_t const *text)
{
	rectangle2d text_bounds;
	short initial_indent = cursor_bounds->x0 - bounds->x0;

	if (initial_indent < 0)
		error(_error_silent, "initial_indent<0 in render_state_text() and was about to explode");
	initial_indent = MAX(0, initial_indent);
	draw_string_set_indents(initial_indent, 0);
	draw_unicode_string_compute_bounds(bounds, text, &text_bounds, cursor_bounds);
	cursor_bounds->x0 -= 3;
	text_bounds.x0 = bounds->x0;
	rasterizer_draw_unicode_string(&text_bounds, NULL, NULL, 0, text);
	bounds->y0 = cursor_bounds->y0;

	return;
}

static void render_state_bitmap(
	rectangle2d *bounds,
	rectangle2d *cursor_bounds,
	pixel32 color,
	struct icon_hud_element_definition *icon)
{
	struct game_globals *game_globals;
	struct interface_tag_references_definition *interface_tag_references;
	long bitmap_group_index;
	long frame_index;
	struct bitmap_data const *bitmap;
	real_rectangle2d const *clip;
	real scale;
	point2d point;

	global_scenario_get();
	game_globals = scenario_get_game_globals();
	interface_tag_references = game_globals->interface_tag_references.count
		? TAG_BLOCK_GET_ELEMENT(
			&game_globals->interface_tag_references,
			0,
			struct interface_tag_references_definition)
		: NULL;
	bitmap_group_index = interface_tag_references->tags[_interface_bitmap_iface_map2].index;
	frame_index = 0;
	bitmap = NULL;
	clip = NULL;
	if (icon->frame_rate)
		frame_index = system_milliseconds() * 30 / 1000 / icon->frame_rate;
	hud_retrieve_bitmap_and_bounding_rect(
		bitmap_group_index,
		icon->sequence_index,
		frame_index,
		&bitmap,
		&clip);
	if (bitmap && _texture_cache_bitmap_get_hardware_format(
		(struct bitmap_data *)bitmap, FALSE, TRUE))
	{
		scale = hud_globals_get_scale(local_player_count() > 1);
		point.x = (short)(icon->offset.x * scale + cursor_bounds->x0 + 1.0f);
		point.y = (short)(cursor_bounds->y1 - icon->offset.y * scale - 2.0f);
		hud_draw_bitmap_direct(
			bitmap,
			_hud_anchor_bottom_left,
			&point,
			clip,
			scale,
			0.0f,
			TEST_FLAG(icon->flags, _hud_icon_use_color_bit) ? icon->color : color,
			FALSE);
		if (TEST_FLAG(icon->flags, _hud_icon_absolute_width_bit))
			cursor_bounds->x0 = icon->width_offset + point.x;
		else if (clip)
			cursor_bounds->x0 = (short)((clip->x1 - clip->x0) * bitmap->width + icon->width_offset + point.x);
		else
			cursor_bounds->x0 = bitmap->width + icon->width_offset + point.x;
	}

	return;
}

void draw_string_and_hack_in_icons(
	rectangle2d *bounds,
	rectangle2d *clip,
	point2d *cursor_reference,
	short height_adjust,
	wchar_t const *instring,
	boolean ignore_icon_color)
{
	wchar_t *current = string_data;
	rectangle2d cursor_bounds = *bounds;

	wcscpy(string_data, instring);
	while (current)
	{
		wchar_t *icon_spec = wcschr(current, L'%');
		short icon_type;

		if (!icon_spec)
			break;
		*icon_spec = 0;
		icon_spec++;
		render_state_text(bounds, &cursor_bounds, current);
		current = icon_spec;
		icon_type = get_icon_type(icon_spec);
		if (icon_type == NONE)
		{
			render_state_text(bounds, &cursor_bounds, L"%");
		}
		else
		{
			short remapped_icon_type;
			short icon_index;

			current = icon_spec + wcslen(icon_names[icon_type]);
			remapped_icon_type = remap_sticks_for_local_player(
				icon_type,
				local_player_index_for_draw_string_and_hack_in_icons);
			icon_index = NONE;
			if (remapped_icon_type > _icon_right_stick)
			{
				if (remapped_icon_type <= _icon_look)
				{
					if (remapped_icon_type <= _icon_accept)
					{
						struct game_input_preferences preferences;

						input_abstraction_get_local_player_preferences(
							local_player_index_for_draw_string_and_hack_in_icons,
							&preferences);
						icon_index = preferences.game_control_to_xbox_buttons[
							button_mappings[remapped_icon_type - _icon_action]];
					}
					else
					{
						icon_index = button_mappings[remapped_icon_type - _icon_action];
						switch (remapped_icon_type)
						{
						case _icon_accept:
							icon_index = _icon_start_button;
							break;
						case _icon_back:
							icon_index = _icon_back_button;
							break;
						case _icon_move:
							icon_index = _icon_left_stick;
							break;
						case _icon_look:
							icon_index = _icon_right_stick;
							break;
						}
					}
				}
				else
				{
					match_assert(
						"c:\\halo\\SOURCE\\interface\\ui_widget.c",
						4341,
						FALSE);
				}
			}
			else
			{
				icon_index = remapped_icon_type;
			}

			{
				struct icon_hud_element_definition *icon = TAG_BLOCK_GET_ELEMENT(
					&hud_globals->messaging.button_icons,
					icon_index,
					struct icon_hud_element_definition);
				byte saved_flags = icon->flags;
				short saved_width_offset = icon->width_offset;
				real_argb_color icon_color;
				real_argb_color text_color;
				long alpha;

				pixel32_to_real_argb_color(icon->color, &icon_color);
				icon->flags &= ~FLAG(_hud_icon_use_color_bit);
				match_assert(
					"c:\\halo\\SOURCE\\interface\\ui_widget.c",
					4362,
					icon_index>=0 && icon_index<NUM_ICONS);
				if (icon_is_special[icon_index])
				{
					icon->flags &= ~FLAG(_hud_icon_absolute_width_bit);
					icon->width_offset = -5;
				}
				draw_string_get_color(&text_color);
				alpha = (long)(text_color.alpha * 255.0f) << 24;
				if (icon->color == 0 || ignore_icon_color)
					icon_color = text_color;
				icon_color.red *= text_color.alpha;
				icon_color.green *= text_color.alpha;
				icon_color.blue *= text_color.alpha;
				render_state_bitmap(
					bounds,
					&cursor_bounds,
					(real_argb_color_to_pixel32(&icon_color) & 0x00ffffff) | alpha,
					icon);
				bounds->x0++;
				icon->flags = saved_flags;
				icon->width_offset = saved_width_offset;
			}
		}
	}
	if (current)
		render_state_text(bounds, &cursor_bounds, current);
	draw_string_set_indents(0, 0);

	return;
}

void ui_start_main_menu_music(
	void)
{
	if (!widget_globals.main_menu_music_active && !main_menu_fade_active())
	{
		long sound_definition_index = tag_loaded(LOOPING_SOUND_DEFINITION_TAG, "sound\\music\\title1\\title1");

		if (sound_definition_index != NONE)
		{
			error(_error_silent, "starting main menu music");
			scripted_looping_sound_start(sound_definition_index, NONE, 1.0f);
			widget_globals.main_menu_music_active = TRUE;
		}
		else
		{
			error(_error_silent, "title music tag not found");
		}
	}

	return;
}

void ui_stop_main_menu_music(
	void)
{
	if (widget_globals.main_menu_music_active == TRUE)
	{
		long sound_definition_index = tag_loaded(LOOPING_SOUND_DEFINITION_TAG, "sound\\music\\title1\\title1");

		if (sound_definition_index != NONE)
		{
			error(_error_silent, "stopping main menu music");
			scripted_looping_sound_stop(sound_definition_index);
		}
		else
		{
			error(_error_silent, "title music tag not found");
		}
		widget_globals.main_menu_music_active = FALSE;
	}

	return;
}

void display_error_deferred(
	short error_code,
	short local_player_index,
	boolean modal,
	boolean pause_game_time)
{
	long index;

	if (local_player_index == NONE)
	{
		index = 0;
	}
	else
	{
		index = local_player_index;
		match_assert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			2288,
			(index>=0) && (index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));
	}
	if (widget_globals.deferred_errors[index].error_code == NONE)
	{
		widget_globals.deferred_errors[index].error_code = error_code;
		widget_globals.deferred_errors[index].local_player_index = local_player_index;
		widget_globals.deferred_errors[index].modal = modal;
		widget_globals.deferred_errors[index].pause_game_time = pause_game_time;
	}
	else
	{
		error(
			_error_silent,
			"there is already a deferred error message for local player %d; ignoring this one",
			index);
	}

	return;
}

void display_errors_deferred_until_cinematic_stop(
	void)
{
	short local_player_index;

	match_vassert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		2367,
		!cinematic_in_progress(),
		"Noooooooooooooooooo!!!");
	for (local_player_index = 0;
		local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		local_player_index++)
	{
		struct ui_widget_deferred_cinematic_error *deferred_error =
			&widget_globals.deferred_cinematic_errors[local_player_index];

		if (deferred_error->error_code >= 0 && deferred_error->error_code < NUMBER_OF_ERROR_CODES)
		{
			display_error(
				deferred_error->error_code,
				local_player_index,
				deferred_error->modal,
				deferred_error->pause_game_time);
		}
		deferred_error->error_code = NONE;
	}

	return;
}

boolean ui_widgets_active_for_local_player(
	short local_player_index)
{
	boolean result = FALSE;

	match_vassert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		1110,
		local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS,
		"expected a valid local_player_index");
	if (widget_globals.initialized)
	{
		long widget_index;

		for (widget_index = 0;
			widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
			widget_index++)
		{
			if (widget_globals.active_widgets[widget_index] &&
				widget_globals.active_widgets[widget_index]->local_player_index == local_player_index)
			{
				result = TRUE;
				break;
			}
		}
	}

	return result;
}

void display_error(
	short error_code,
	short local_player_index,
	boolean modal,
	boolean pause_game_time)
{
	if (cinematic_in_progress())
	{
		if (local_player_index == NONE)
			local_player_index = 0;
		match_assert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			2077,
			local_player_index>=0 && local_player_index<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS);
		if (widget_globals.deferred_cinematic_errors[local_player_index].error_code == NONE)
		{
			widget_globals.deferred_cinematic_errors[local_player_index].error_code = error_code;
			widget_globals.deferred_cinematic_errors[local_player_index].modal = modal;
			widget_globals.deferred_cinematic_errors[local_player_index].pause_game_time = pause_game_time;
		}
		else
		{
			error(_error_silent, "there is already a deferred-for-cinematic error queued for player #%d; ignoring this one", local_player_index);
		}
	}
	else
	{
		char const *widget_name = NULL;
		short error_local_player_index;
		short local_player_count;
		short widget_stack;
		boolean first_local_player = TRUE;
		struct widget_instance *top_widget;
		long top_widget_tag_index;
		struct widget_instance *widget;

		if (local_player_index != NONE)
		{
			short index;

			local_player_count = 0;
			error_local_player_index = NONE;
			for (index = local_player_get_next(NONE); index != NONE; index = local_player_get_next(index))
			{
				if (index == local_player_index)
				{
					error_local_player_index = local_player_index;
					if (local_player_count > 0)
						first_local_player = FALSE;
				}
				local_player_count++;
			}
		}
		else
		{
			error_local_player_index = NONE;
			local_player_count = 0;
		}
		if (error_local_player_index == NONE && !we_are_at_the_main_menu)
			local_player_index = NONE;
		switch (local_player_count)
		{
		case 0:
		case 1:
			widget_name = modal
				? "ui\\shell\\error\\error_modal_fullscreen"
				: "ui\\shell\\error\\error_nonmodal_fullscreen";
			break;
		case 2:
			widget_name = modal
				? "ui\\shell\\error\\error_modal_halfscreen"
				: "ui\\shell\\error\\error_nonmodal_halfscreen";
			break;
		case 3:
			if (first_local_player == TRUE)
			{
				widget_name = modal
					? "ui\\shell\\error\\error_modal_halfscreen"
					: "ui\\shell\\error\\error_nonmodal_halfscreen";
			}
			else
			{
				widget_name = modal
					? "ui\\shell\\error\\error_modal_qtrscreen"
					: "ui\\shell\\error\\error_nonmodal_qtrscreen";
			}
			break;
		case 4:
			widget_name = modal
				? "ui\\shell\\error\\error_modal_qtrscreen"
				: "ui\\shell\\error\\error_nonmodal_qtrscreen";
			break;
		default:
			match_vassert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				2161,
				FALSE,
				"invalid local player count");
			break;
		}
		if (widget_name)
		{
			widget_stack = local_player_index == NONE ? 0 : local_player_index;
			match_assert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				2168,
				(widget_stack>=0) && (widget_stack<MAXIMUM_NUMBER_OF_LOCAL_PLAYERS));
			if (transition_to_game_in_progress())
			{
				error(_error_silent, "aborting to the main menu root, for safety's sake");
				main_screen_shell_load();
				main_defer_map_map_change();
				ui_widgets_set_fade_value(-1.0f);
			}
			top_widget = widget_globals.active_widgets[widget_stack];
			top_widget_tag_index = top_widget ? top_widget->definition_tag_index : NONE;
			if (top_widget && top_widget->widget_is_error_dialog == TRUE)
			{
				error(_error_silent, "there is already an error message displayed for this local player index");
				widget = NULL;
			}
			else
			{
				widget = ui_widget_load_by_name_or_tag(
					widget_name,
					NONE,
					NULL,
					local_player_index,
					top_widget_tag_index,
					NONE,
					NONE);
			}
			if (widget)
			{
				struct widget_instance *text_box;

				match_vassert(
					"c:\\halo\\SOURCE\\interface\\ui_widget.c",
					2206,
					widget->child && widget->child->child,
					"error screen widget tag not layed out as expected");
				text_box = widget->child->child;
				match_vassert(
					"c:\\halo\\SOURCE\\interface\\ui_widget.c",
					2208,
					text_box->type == _ui_widget_type_text_box,
					"expected a text box widget in the error widget");
				text_box->parameters.text_box.string_list_index = PIN(error_code, 0, NUMBER_OF_ERROR_CODES - 1);
				widget->widget_is_error_dialog = TRUE;
				if (!widget->pause_game_time)
				{
					widget->pause_game_time = pause_game_time;
					if (widget->pause_game_time == TRUE)
					{
						match_vassert(
							"c:\\halo\\SOURCE\\interface\\ui_widget.c",
							2217,
							widget_globals.pause_game_time_count>=0,
							"widget pause counter is out of whack");
						widget_globals.pause_game_time_count++;
						if (!game_time_get_paused())
							game_time_set_paused(TRUE);
						if (!widget_globals.sound_paused && !we_are_at_the_main_menu)
						{
							sound_pause(TRUE);
							widget_globals.sound_paused = TRUE;
						}
					}
				}
				switch (error_code)
				{
				case _error_controller_unplugged_start_to_continue:
					widget->milliseconds_to_auto_close = 0;
					widget->auto_close_fade_time = 0;
					break;
				case _error_controller_unplugged:
					widget->close_if_local_player_controller_present = TRUE;
					widget->milliseconds_to_auto_close = 0;
					widget->auto_close_fade_time = 0;
					break;
				default:
					widget->close_if_local_player_controller_present = FALSE;
					break;
				}
			}
			else
			{
				error(_error_silent, "failed to display error message");
			}
		}
	}

	return;
}

void display_error_abort_to_dashboard(
	short error_code,
	boolean optional)
{
	char const *widget_name;
	struct widget_instance *widget;

	if (optional == TRUE)
		widget_name = "ui\\shell\\error\\error_abort_to_dashboard";
	else
		widget_name = "ui\\shell\\error\\error_abort_to_dashboard_you_have_no_choice";
	if (!optional)
		ui_widgets_close_all();
	widget = ui_widget_load_by_name_or_tag(widget_name, NONE, NULL, NONE, NONE, NONE, NONE);
	if (widget)
	{
		match_vassert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			2319,
			widget->type == _ui_widget_type_text_box,
			"expected a text box widget");
		widget->parameters.text_box.string_list_index = error_code;
		widget->widget_is_error_dialog = TRUE;
		dashboard_abort_error = error_code;
	}
	else
	{
		error(_error_silent, "failed to load '%s' widget", widget_name);
	}

	return;
}

void display_error_damaged_media(
	void)
{
	display_error_abort_to_dashboard(_error_media_damaged, FALSE);
	input_frame_end();
	main_loop_of_death();

	return;
}

void network_game_reset_to_pregame_ui(
	void)
{
	ui_widgets_close_all();
	if (network_game_is_splitscreen_local())
	{
		if (network_game_is_quickstart_local())
		{
			if (!ui_widget_load_by_name_or_tag(
				"ui\\shell\\main_menu\\multiplayer_type_select\\split_screen\\pregame\\splitscreen_pregame_wrapper_normal",
				NONE, NULL, NONE, NONE, NONE, NONE))
			{
				error(_error_silent, "failed to load pregame screen after quickstart match");
			}
		}
		else
		{
			if (!ui_widget_load_by_name_or_tag(
				"ui\\shell\\main_menu\\multiplayer_type_select\\split_screen\\splitscreen_map_select_postgame_wrapper",
				NONE, NULL, NONE, NONE, NONE, NONE))
			{
				error(_error_silent, "failed to load map select postgame screen");
			}
		}
	}
	else
	{
		if (global_network_game_server_get())
		{
			network_game_server_pause_countdown(global_network_game_server_get(), TRUE);
			if (!ui_widget_load_by_name_or_tag(
				"ui\\shell\\main_menu\\multiplayer_type_select\\connected\\connected_map_select_postgame_wrapper",
				NONE, NULL, NONE, NONE, NONE, NONE))
			{
				error(_error_silent, "failed to load map select postgame screen");
			}
		}
		else
		{
			if (!ui_widget_load_by_name_or_tag(
				"ui\\shell\\main_menu\\multiplayer_type_select\\connected\\pregame\\connected_pregame_screen",
				NONE, NULL, NONE, NONE, NONE, NONE))
			{
				error(_error_silent, "failed to load networked pregame status screen");
			}
		}
	}

	return;
}

void display_scenario_help(
	short string_index)
{
	char scenario_name[256];

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		2407,
		string_index>=0);
	if (global_scenario_index != NONE)
	{
		char const *widget_name;
		short local_player_index;
		struct widget_instance *widget;

		csstrncpy(scenario_name, tag_get_name(global_scenario_index), sizeof(scenario_name) - 1);
		scenario_name[sizeof(scenario_name) - 1] = 0;
		strlwr(scenario_name);
		if (strstr(scenario_name, "a10"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_a10";
		else if (strstr(scenario_name, "a30"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_a30";
		else if (strstr(scenario_name, "a50"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_a50";
		else if (strstr(scenario_name, "b30"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_b30";
		else if (strstr(scenario_name, "b40"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_b40";
		else if (strstr(scenario_name, "c10"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_c10";
		else if (strstr(scenario_name, "c20"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_c20";
		else if (strstr(scenario_name, "c40"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_c40";
		else if (strstr(scenario_name, "d20"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_d20";
		else if (strstr(scenario_name, "d40"))
			widget_name = "ui\\shell\\solo_game\\player_help\\player_help_screen_d40";
		else
		{
			error(_error_silent, "can't display scenario help; unknown scenario is active '%s'", scenario_name);

			return;
		}
		local_player_index = player_ui_get_single_player_local_player_controller(0);
		widget = ui_widget_load_by_name_or_tag(
			widget_name,
			NONE,
			NULL,
			local_player_index,
			NONE,
			NONE,
			NONE);
		if (widget)
		{
			struct widget_instance *text_box;

			for (text_box = widget->child; text_box; text_box = text_box->next)
			{
				if (text_box->type == _ui_widget_type_text_box)
					break;
			}
			match_vassert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				2438,
				text_box && text_box->type == _ui_widget_type_text_box,
				"expected text box widget in player help screen");
			text_box->parameters.text_box.string_list_index = string_index;
		}
		else
		{
			error(_error_silent, "failed to load in-game help dialog");
		}
	}
	else
	{
		error(_error_silent, "can't display scenario help because no scenario is loaded");
	}

	return;
}

static boolean transition_to_game_in_progress(
	void)
{
	if (
		we_are_at_the_main_menu &&
		widget_globals.fade_to_black <= 1.0f &&
		widget_globals.fade_to_black >= 0.0f)
	{
		return TRUE;
	}

	return FALSE;
}

static short get_icon_type(
	wchar_t const *string)
{
	short icon_index;

	for (icon_index = 0; icon_index < NUMBEROF(icon_names); icon_index++)
	{
		if (_wcsnicmp(string, icon_names[icon_index], wcslen(icon_names[icon_index])) == 0)
			break;
	}

	if (icon_index == NUMBEROF(icon_names))
		icon_index = NONE;

	return icon_index;
}

static boolean should_flip_sticks_for_local_player(
	short local_player_index)
{
	struct game_input_preferences preferences;

	if (local_player_index == NONE)
		local_player_index = local_player_get_next(NONE);
	memset(&preferences, 0, sizeof(preferences));
	if (local_player_index != NONE)
		input_abstraction_get_local_player_preferences(local_player_index, &preferences);
	switch (preferences.joystick_controls)
	{
	case _joystick_preset_south_paw:
	case _joystick_preset_legacy_south_paw:
		return TRUE;
	}

	return FALSE;
}

short remap_sticks_for_local_player(
	short icon,
	short local_player_index)
{
	switch (icon)
	{
	case _icon_left_stick:
	case _icon_move:
		match_assert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			4243,
			16 == get_icon_type(L"left-stick"));
		match_assert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			4244,
			30 == get_icon_type(L"move"));
		icon = should_flip_sticks_for_local_player(local_player_index) ? _icon_right_stick : _icon_left_stick;
		break;
	case _icon_right_stick:
	case _icon_look:
		match_assert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			4250,
			17 == get_icon_type(L"right-stick"));
		match_assert(
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			4251,
			31 == get_icon_type(L"look"));
		icon = should_flip_sticks_for_local_player(local_player_index) ? _icon_left_stick : _icon_right_stick;
		break;
	}

	return icon;
}

static unsigned long __stdcall filesystem_initialization_thread_proc(
	void *input)
{
	widget_globals.filesystem_check_result = saved_game_perform_file_system_checks();
	if (!widget_globals.filesystem_check_result)
	{
		word number_of_profiles = 1;
		long profile_index;

		playlist_profiles_enumerate_available_to_local_player_index(NONE, &number_of_profiles, &profile_index);
		player_profiles_enumerate_available_to_local_player_index(NONE, &number_of_profiles, &profile_index, TRUE);
		player_ui_get_player1_last_used_profile_index();
	}

	return 0;
}

static void perform_filesystem_initialization(
	void)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		5439,
		widget_globals.initialization_thread==NULL);
	error(_error_silent, "begining filesystem checks & saved game file enumeration...");
	ui_widgets_inhibit_processing(TRUE);
	widget_globals.filesystem_check_result = 0;
	if (!create_thread(0, filesystem_initialization_thread_proc, NULL, &widget_globals.initialization_thread))
	{
		error(_error_silent, "failed to spawn thread for filesystem checks - running synchronously!");
		widget_globals.initialization_thread = NULL;
		filesystem_initialization_thread_proc(NULL);
		ui_widgets_inhibit_processing(FALSE);
	}

	return;
}

void main_screen_shell_load(
	void)
{
	boolean load_main_menu = TRUE;

	ui_widgets_inhibit_processing(FALSE);
	if (main_screen_shell_first_load == TRUE)
	{
		char const *command_line = shell_get_command_line();

		if (command_line && _stricmp(command_line, "xdemo") == 0)
		{
			error(_error_silent, "xbox command line= '%s'", command_line);
		}
		else
		{
			bink_playback_start(
				attract_mode_get_localized_movie_path(_bink_intro_movie),
				FLAG(_bink_playback_button_click_stops_movie_bit) |
					FLAG(_bink_playback_prevent_events_to_ui_bit) |
					FLAG(_bink_playback_return_to_main_menu_when_finished_bit) |
					FLAG(_bink_playback_dont_allow_skipping_if_filesystem_check_thread_is_active_bit) |
					FLAG(_bink_playback_eat_up_memory_like_a_goddamn_beaver_bit));
			load_main_menu = FALSE;
			if (!bink_playback_active())
				load_main_menu = TRUE;
		}
		perform_filesystem_initialization();
		input_abstraction_reset_controller_detection_timer();
	}
	if (load_main_menu)
	{
		attract_mode_reset_timer();
		ui_widgets_close_all();
		if (!ui_widget_load_by_name_or_tag("ui\\shell\\main_menu\\main_menu", NONE, NULL, NONE, NONE, NONE, NONE))
			error(_error_silent, "failed to load main screen shell window");
		if (widget_globals.main_menu_deferred_error_code != NONE)
		{
			display_error(widget_globals.main_menu_deferred_error_code, NONE, TRUE, FALSE);
			widget_globals.main_menu_deferred_error_code = NONE;
		}
		if (!widget_globals.main_menu_music_active)
			ui_start_main_menu_music();
		reset_last_player1_profile_index();
	}
	if (!virtual_keyboard_initialize())
		error(_error_silent, "failed to initialize the virtual keyboard");
	main_screen_shell_first_load = FALSE;

	return;
}

static void widget_instance_reload_recursive(
	struct widget_instance *widget)
{
	return;
}

static void ui_widget_reload_by_tag(
	long tag_index)
{
	return;
}

real_rgb_color get_ui_rgb_white(
	void)
{
	real_rgb_color result;

	result = *global_real_rgb_white;
	result.red = global_ui_white_red;
	result.green = global_ui_white_green;
	result.blue = global_ui_white_blue;

	return result;
}

real_argb_color get_ui_argb_white(
	void)
{
	real_argb_color result;

	result = *global_real_argb_white;
	result.red = global_ui_white_red;
	result.green = global_ui_white_green;
	result.blue = global_ui_white_blue;

	return result;
}

static __inline real widget_instance_get_cumulative_alpha_modifier(
	struct widget_instance *widget)
{
	real alpha_modifier = widget->alpha_modifier;

	widget = widget->parent;
	while (widget)
	{
		alpha_modifier *= widget->alpha_modifier;
		widget = widget->parent;
	}

	return alpha_modifier;
}

static boolean widget_instance_text_box_is_focused(
	struct widget_instance *widget)
{
	struct widget_instance *parent = widget->parent;
	boolean focused = parent ? parent->focused_child == widget : TRUE;

	if (!focused && parent)
	{
		struct widget_instance *ancestor;

		do
		{
			ancestor = parent->parent;
			if (ancestor)
			{
				if (ancestor->focused_child != parent)
					break;
				focused = (ancestor->type == _ui_widget_type_spinner_list ||
					ancestor->type == _ui_widget_type_column_list) &&
					ancestor->focused_child == parent;
			}
			parent = ancestor;
		}
		while (ancestor);
	}

	return focused;
}

static boolean string_has_icons_to_draw(
	wchar_t *string)
{
	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		4181,
		string);
	while (string)
	{
		wchar_t *icon_spec = wcschr(string, L'%');

		if (!icon_spec)
			break;
		icon_spec++;
		if (get_icon_type(icon_spec) != NONE)
			return TRUE;
		string = icon_spec;
	}

	return FALSE;
}

static long search_and_replace(
	wchar_t *search,
	wchar_t *replace,
	wchar_t **string)
{
	long replacements = 0;

	if (string && *string)
	{
		wchar_t *buffer = *string;
		long search_length = ustrlen(search);
		long replace_length = ustrlen(replace);
		long length = ustrlen(buffer) + 1;
		long delta;
		wchar_t *match;

		if (replace_length <= search_length)
		{
			delta = search_length - replace_length;
			match = ustrstr(buffer, search);
			if (match)
			{
				do
				{
					replacements++;
					csmemcpy(match, replace, 2 * replace_length);
					if (delta > 0)
					{
						csmemmove(
							&match[replace_length],
							&match[search_length],
							2 * (length - (match - buffer) - replace_length));
						length -= delta;
					}
					match = ustrstr(buffer, search);
				}
				while (match);
			}
		}
		else
		{
			delta = replace_length - search_length;
			for (match = ustrstr(buffer, search);
				match;
				match = ustrstr(&match[search_length], search))
			{
				replacements++;
			}
			if (replacements > 0)
			{
				buffer = pool_resize_pointer(
					widget_memory_pool,
					buffer,
					2 * (delta * replacements + length),
					"c:\\halo\\SOURCE\\interface\\ui_widget.c",
					4994);
				if (!buffer)
				{
					replacements = NONE;
				}
				else
				{
					for (match = ustrstr(buffer, search);
						match;
						match = ustrstr(buffer, search))
					{
						csmemmove(
							&match[replace_length],
							&match[search_length],
							2 * (length - (match - buffer) - search_length));
						csmemcpy(match, replace, 2 * replace_length);
						length += delta;
					}
					*string = buffer;
				}
			}
		}
	}

	return replacements;
}

static void widget_instance_render_text_box(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	rectangle2d *clip_rect,
	point2d offset,
	boolean focus)
{
	wchar_t **text;
	long search_index;
	long font_index;
	short justification;
	real alpha_modifier;
	real color_alpha;
	real_argb_color color;
	rectangle2d bounds;
	rectangle2d clip;

	if (definition->text_label_string_list.index != NONE)
	{
		short string_list_index;
		wchar_t *string;
		unsigned long length;

		if (widget->parameters.text_box.string_list_index == NONE)
			string_list_index = definition->string_list_index;
		else
			string_list_index = widget->parameters.text_box.string_list_index;
		string = unicode_string_list_get_string(
			definition->text_label_string_list.index,
			string_list_index);
		length = ustrlen(string);
		widget->parameters.text_box.text = pool_resize_pointer(
			widget_memory_pool,
			widget->parameters.text_box.text,
			2 * length + 2,
			"c:\\halo\\SOURCE\\interface\\ui_widget.c",
			4421);
		if (widget->parameters.text_box.text)
		{
			csmemcpy(widget->parameters.text_box.text, string, 2 * length);
			widget->parameters.text_box.text[length] = 0;
		}
		else
		{
			widget->parameters.text_box.text = L"<out of memory>";
		}
	}
	text = &widget->parameters.text_box.text;
	if (!*text || !**text)
		return;
	for (search_index = 0;
		search_index < definition->search_and_replace_functions.count;
		search_index++)
	{
		struct ui_widget_search_and_replace_reference *reference =
			(struct ui_widget_search_and_replace_reference *)
				definition->search_and_replace_functions.address +
			search_index;

		if (reference && reference->search_string[0])
		{
			wchar_t search_string[32];
			wchar_t *replace = ui_widget_search_and_replace_invoke(
				widget,
				reference->replace_function);

			search_and_replace(
				ascii_to_wide(
					reference->search_string,
					search_string,
					sizeof(search_string)),
				replace,
				text);
		}
	}
	font_index = definition->text_font.index;
	if (font_index == NONE)
	{
		error(
			_error_silent,
			"failed to render text box widget because the font tag was invalid");

		return;
	}
	justification = definition->justification;
	if (justification < 0 || justification >= NUMBER_OF_TEXT_JUSTIFICATIONS)
	{
		error(
			_error_silent,
			"failed to render text box widget because the justification was invalid");

		return;
	}
	if (!widget->visible)
		return;
	alpha_modifier = widget_instance_get_cumulative_alpha_modifier(widget);
	bounds = definition->bounds;
	clip = clip_rect ? *clip_rect : definition->bounds;
	bounds.x1 += offset.x;
	bounds.y1 += offset.y;
	bounds.x0 += offset.x;
	bounds.y0 += offset.y;
	bounds.x0 += definition->horizontal_offset;
	bounds.y0 += definition->vertical_offset;
	if (focus)
	{
		color = get_ui_argb_white();
		color_alpha = definition->text_color.alpha;
	}
	else
	{
		color = definition->text_color;
		if (1.0f == color.red && 1.0f == color.green && 1.0f == color.blue)
		{
			color = get_ui_argb_white();
			color_alpha = definition->text_color.alpha;
		}
		else
		{
			color_alpha = color.alpha;
		}
	}
	color.alpha = alpha_modifier * color_alpha;
	if (TEST_FLAG(definition->text_box_flags, _text_box_flashing_text_bit))
	{
		color.alpha = (((real)cos(
			widget_globals.current_system_milliseconds *
				SECONDS_PER_MILLISECOND * 3.0f) + 1.5f) * 0.4f) * color.alpha;
	}
	draw_string_set_draw_mode(font_index, NONE, justification, 0, &color);
	if (string_has_icons_to_draw(*text))
		draw_string_and_hack_in_icons(&bounds, &clip, NULL, 0, *text, FALSE);
	else
		rasterizer_draw_unicode_string(&bounds, &clip, NULL, 0, *text);

	return;
}

static void widget_instance_render_spinner_list(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	rectangle2d *clip_rect,
	point2d offset,
	boolean focus)
{
	long header_frame_index = 0;
	long footer_frame_index = 0;
	real alpha_modifier = widget_instance_get_cumulative_alpha_modifier(widget);
	short last_list_tab_direction;
	struct bitmap_data *bitmap;
	rectangle2d bounds;

	if (!widget->visible)
		return;
	last_list_tab_direction = widget->parameters.list.last_list_tab_direction;
	if (last_list_tab_direction)
	{
		switch (SIGN(last_list_tab_direction))
		{
		case -1:
			widget->parameters.list.last_list_tab_direction =
				last_list_tab_direction + 1;
			header_frame_index = 1;
			break;

		case 1:
			widget->parameters.list.last_list_tab_direction =
				last_list_tab_direction - 1;
			footer_frame_index = 1;
			break;
		}
	}
	bitmap = bitmap_group_get_bitmap_from_sequence(
		definition->list_header_bitmap.index,
		0,
		header_frame_index);
	if (bitmap)
	{
		struct rasterizer_dynamic_screen_geometry_parameters parameters;
		long alpha = fast_ftol(alpha_modifier * 255.0f);

		csmemset(&parameters, 0, sizeof(parameters));
		bounds = definition->list_header_bounds;
		bounds.x0 += offset.x;
		bounds.y0 += offset.y;
		bounds.x1 += offset.x;
		bounds.y1 += offset.y;
		draw_bitmap_in_rect(
			bitmap,
			&bounds,
			&bounds,
			clip_rect,
			(alpha << 24) | 0x00FFFFFF,
			&parameters,
			FALSE);
	}
	bitmap = bitmap_group_get_bitmap_from_sequence(
		definition->list_footer_bitmap.index,
		0,
		footer_frame_index);
	if (bitmap)
	{
		struct rasterizer_dynamic_screen_geometry_parameters parameters;
		long alpha = fast_ftol(alpha_modifier * 255.0f);

		csmemset(&parameters, 0, sizeof(parameters));
		bounds = definition->list_footer_bounds;
		bounds.x0 += offset.x;
		bounds.y0 += offset.y;
		bounds.x1 += offset.x;
		bounds.y1 += offset.y;
		draw_bitmap_in_rect(
			bitmap,
			&bounds,
			&bounds,
			clip_rect,
			(alpha << 24) | 0x00FFFFFF,
			&parameters,
			FALSE);
	}
	if (definition->child_widgets.count == 0)
	{
		wchar_t *item_text;

		if (definition->text_label_string_list.index != NONE)
		{
			short string_index = widget->parameters.list.selected_index;
			wchar_t *string = unicode_string_list_get_string(
				definition->text_label_string_list.index,
				string_index);
			unsigned long length = ustrlen(string);

			item_text = pool_new_pointer(
				widget_memory_pool,
				2 * length + 2,
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				4610);
			if (item_text)
			{
				long search_index;

				csmemcpy(item_text, string, 2 * length);
				item_text[length] = 0;
				for (search_index = 0;
					search_index < definition->search_and_replace_functions.count;
					search_index++)
				{
					struct ui_widget_search_and_replace_reference *reference =
						(struct ui_widget_search_and_replace_reference *)
							definition->search_and_replace_functions.address +
						search_index;

					if (reference && reference->search_string[0])
					{
						wchar_t search_string[32];
						wchar_t *replace = ui_widget_search_and_replace_invoke(
							widget,
							reference->replace_function);

						search_and_replace(
							ascii_to_wide(
								reference->search_string,
								search_string,
								sizeof(search_string)),
							replace,
							&item_text);
					}
				}
			}
		}
		else
		{
			item_text = widget->parameters.list.item_text;
		}
		if (item_text)
		{
			if (definition->text_font.index == NONE)
			{
				error(
					_error_silent,
					"failed to render spinner list item because the font tag was invalid");
			}
			else if (definition->justification < 0 ||
				definition->justification >= NUMBER_OF_TEXT_JUSTIFICATIONS)
			{
				error(
					_error_silent,
					"failed to render spinner list item because the justification was invalid");
			}
			else
			{
				real text_alpha_modifier =
					widget_instance_get_cumulative_alpha_modifier(widget);
				real color_alpha;
				real_argb_color color;
				rectangle2d clip = clip_rect ? *clip_rect : definition->bounds;

				bounds = definition->bounds;
				bounds.x1 += offset.x;
				bounds.y1 += offset.y;
				bounds.x0 += offset.x;
				bounds.y0 += offset.y;
				if (focus)
				{
					color.alpha = definition->text_color.alpha;
					color.rgb = get_ui_rgb_white();
					color_alpha = color.alpha;
				}
				else
				{
					color = definition->text_color;
					if (1.0f == color.red &&
						1.0f == color.green &&
						1.0f == color.blue)
					{
						color_alpha = definition->text_color.alpha;
					}
					else
					{
						color_alpha = color.alpha;
					}
				}
				color.alpha = text_alpha_modifier * color_alpha;
				if (TEST_FLAG(
						definition->text_box_flags,
						_text_box_flashing_text_bit))
				{
					color.alpha = (((real)sin(
						widget_globals.current_system_milliseconds *
							SECONDS_PER_MILLISECOND * 3.0f) + 1.0f) * 0.5f) *
						color.alpha;
				}
				draw_string_set_draw_mode(
					definition->text_font.index,
					NONE,
					definition->justification,
					0,
					&color);
				rasterizer_draw_unicode_string(&bounds, &clip, NULL, 0, item_text);
			}
		}
		if (definition->text_label_string_list.index != NONE)
			dispose_pointer(widget_memory_pool, item_text);
	}

	return;
}

#ifdef HALO_LINUX
/* ---------- the mouse (desktop builds)

The menus were made for a controller: the d-pad moves the focus through a
screen's items and A activates the focused one. With the mouse
(port/linux/include/halo_ui_pointer.h) the item under the pointer takes the
focus, a left click presses A on it (on a spinner list, left or right by the
half clicked), a right click presses B and the wheel the d-pad. The items and
where they are drawn are noted while the menus draw (ui_mouse_note_target),
for the next frame's processing (ui_widgets_process_mouse), which forgets
them: widgets can be deleted from then on. */

#define UI_MOUSE_MAXIMUM_TARGETS 96

enum ui_mouse_target_kind
{
	/* an item the d-pad moves the focus to: a click presses A on it */
	_ui_mouse_target_item,
	/* a list showing one value at a time: a click on either half steps it
	that way */
	_ui_mouse_target_value,
	/* one of the items a list shows side by side (profiles, levels): the
	list steps to it, and a click then presses A */
	_ui_mouse_target_list_slot,
	/* a button's icon and label in a screen's key: a click presses it */
	_ui_mouse_target_button
};

struct ui_mouse_target
{
	struct widget_instance *widget;
	rectangle2d bounds;
	short kind;
	short button_index;
};

static struct ui_mouse_target ui_mouse_targets[UI_MOUSE_MAXIMUM_TARGETS];
static long ui_mouse_target_count = 0;
static boolean ui_mouse_noting_targets = FALSE;

/* The presses the mouse makes, posted one a frame: the event queue keeps
only the latest event posted between two frames (queue_event). What they
lead to (the focus, a list's position) is only known once they have been
processed, so the pointer's next hover or click waits for them. */
#define UI_MOUSE_MAXIMUM_PRESSES 16

static short ui_mouse_presses[UI_MOUSE_MAXIMUM_PRESSES];
static long ui_mouse_press_count = 0;
static boolean ui_mouse_hover_pending = FALSE;
static boolean ui_mouse_click_pending = FALSE;
static short ui_mouse_hover_x, ui_mouse_hover_y;
static short ui_mouse_click_x, ui_mouse_click_y;

static void ui_mouse_press(
	short button_index)
{
	if (ui_mouse_press_count < UI_MOUSE_MAXIMUM_PRESSES)
		ui_mouse_presses[ui_mouse_press_count++] = button_index;

	return;
}

/* the widget's place among its parent's children, or NONE */
static long ui_mouse_child_index(
	struct widget_instance *widget)
{
	struct widget_instance *child;
	long index = 0;

	for (child = widget->parent ? widget->parent->child : NULL; child; child = child->next, index++)
	{
		if (child == widget)
			return index;
	}

	return NONE;
}

/* a list that shows several of its items at once */
static boolean ui_mouse_list_shows_several(
	struct widget_instance *widget)
{
	return widget->type == _ui_widget_type_spinner_list &&
		ui_widget_definition_get(widget->definition_tag_index)->child_widgets.count > 1;
}

/* the button a screen key's icon stands for ("a_butn" and so on), or NONE */
static short ui_mouse_key_button(
	struct widget_instance *widget)
{
	static struct
	{
		char const *name;
		short button_index;
	} const keys[] =
	{
		{ "a_butn", _gamepad_analog_button_a },
		{ "b_butn", _gamepad_analog_button_b },
		{ "x_butn", _gamepad_analog_button_x },
		{ "y_butn", _gamepad_analog_button_y },
		{ "black_butn", _gamepad_analog_button_black },
		{ "white_butn", _gamepad_analog_button_white },
		{ "start_butn", _gamepad_binary_button_start },
		{ "back_butn", _gamepad_binary_button_back },
	};
	char const *name = tag_get_name(widget->definition_tag_index);
	char const *leaf = name ? strrchr(name, '\\') : NULL;
	long key_index;

	leaf = leaf ? leaf + 1 : name;
	for (key_index = 0; leaf && key_index < NUMBEROF(keys); key_index++)
	{
		long length = (long)strlen(keys[key_index].name);

		/* and the smaller icons of the dialogs, "a_butn_sm" */
		if (!strncmp(leaf, keys[key_index].name, length) &&
			(leaf[length] == '\0' || leaf[length] == '_'))
		{
			return keys[key_index].button_index;
		}
	}

	return NONE;
}

/* whether the d-pad moves the focus to a widget: an item of a column list,
or a child its parent tabs through */
static boolean ui_mouse_widget_is_item(
	struct widget_instance *widget)
{
	struct widget_instance *parent = widget->parent;
	struct ui_widget_definition *definition;
	long index;

	if (!parent || parent->type == _ui_widget_type_spinner_list)
		return FALSE;
	index = ui_mouse_child_index(widget);
	if (index == NONE || !widget_instance_can_receive_events(widget))
		return FALSE;
	if (parent->type == _ui_widget_type_column_list)
	{
		return !parent->parameters.list.list_items ||
			index < parent->parameters.list.number_of_items;
	}
	definition = ui_widget_definition_get(parent->definition_tag_index);
	if (!TEST_FLAG(definition->flags, _widget_dpad_updown_tabs_thru_children_bit) &&
		!TEST_FLAG(definition->flags, _widget_dpad_leftright_tabs_thru_children_bit))
	{
		return FALSE;
	}
	definition = ui_widget_definition_get(widget->definition_tag_index);

	return definition->event_handlers.count > 0 ||
		TEST_FLAG(definition->flags, _widget_pass_unhandled_events_to_children_bit) ||
		widget->type == _ui_widget_type_spinner_list ||
		widget->type == _ui_widget_type_column_list;
}

static void ui_mouse_note_target(
	struct widget_instance *widget,
	struct ui_widget_definition const *definition,
	point2d offset)
{
	struct widget_instance *parent = widget->parent;
	struct ui_mouse_target *target;
	rectangle2d bounds = definition->bounds;
	short button_index;
	short kind;

	if (!ui_mouse_noting_targets ||
		ui_mouse_target_count >= UI_MOUSE_MAXIMUM_TARGETS ||
		widget->disabled)
	{
		return;
	}
	bounds.x0 += offset.x;
	bounds.x1 += offset.x;
	bounds.y0 += offset.y;
	bounds.y1 += offset.y;
	button_index = ui_mouse_key_button(widget);
	if (button_index != NONE)
	{
		/* the icon and its label: the text beginning just right of it */
		struct widget_instance *sibling;
		rectangle2d label_bounds;
		short best_distance = 17;

		kind = _ui_mouse_target_button;
		for (sibling = parent ? parent->child : NULL; sibling; sibling = sibling->next)
		{
			struct ui_widget_definition *sibling_definition;
			rectangle2d sibling_bounds;
			short distance;

			if (sibling->type != _ui_widget_type_text_box || !sibling->visible)
				continue;
			sibling_definition = ui_widget_definition_get(sibling->definition_tag_index);
			sibling_bounds = sibling_definition->bounds;
			sibling_bounds.x0 += offset.x - widget->horizontal_offset + sibling->horizontal_offset;
			sibling_bounds.x1 += offset.x - widget->horizontal_offset + sibling->horizontal_offset;
			sibling_bounds.y0 += offset.y - widget->vertical_offset + sibling->vertical_offset;
			sibling_bounds.y1 += offset.y - widget->vertical_offset + sibling->vertical_offset;
			distance = (short)ABS(sibling_bounds.x0 - bounds.x1);
			if (distance < best_distance &&
				sibling_bounds.y0 < bounds.y1 && sibling_bounds.y1 > bounds.y0)
			{
				best_distance = distance;
				label_bounds = sibling_bounds;
			}
		}
		if (best_distance < 17)
		{
			bounds.x1 = MAX(bounds.x1, label_bounds.x1);
			bounds.y0 = MIN(bounds.y0, label_bounds.y0);
			bounds.y1 = MAX(bounds.y1, label_bounds.y1);
		}
	}
	else if (parent && ui_mouse_list_shows_several(parent))
	{
		if (parent->disabled ||
			ui_mouse_child_index(widget) == NONE ||
			!widget_instance_can_receive_events(parent))
		{
			return;
		}
		kind = _ui_mouse_target_list_slot;
	}
	else if (widget->type == _ui_widget_type_spinner_list)
	{
		/* a list showing several items is picked through them */
		if (!parent || ui_mouse_list_shows_several(widget) || !widget_instance_can_receive_events(widget))
			return;
		kind = _ui_mouse_target_value;
	}
	else if (ui_mouse_widget_is_item(widget))
	{
		kind = _ui_mouse_target_item;
	}
	else
	{
		return;
	}
	target = &ui_mouse_targets[ui_mouse_target_count++];
	target->widget = widget;
	target->bounds = bounds;
	target->kind = kind;
	target->button_index = button_index;

	return;
}

/* the target drawn last (so on top, and the innermost) under a point */
static struct ui_mouse_target *ui_mouse_target_at(
	short x,
	short y)
{
	long index;

	for (index = ui_mouse_target_count - 1; index >= 0; index--)
	{
		rectangle2d const *bounds = &ui_mouse_targets[index].bounds;

		if (x >= bounds->x0 && x < bounds->x1 && y >= bounds->y0 && y < bounds->y1)
			return &ui_mouse_targets[index];
	}

	return NULL;
}

static boolean ui_mouse_widget_has_focus(
	struct widget_instance *widget)
{
	for (; widget->parent; widget = widget->parent)
	{
		if (widget->parent->focused_child != widget)
			return FALSE;
	}

	return TRUE;
}

/* moves the focus to the widget as the d-pad would, with the selection of
the column lists on the way (widget_event_function_list_widget_goto_next_item) */
static void ui_mouse_give_focus(
	struct widget_instance *widget)
{
	struct widget_instance *ancestor;

	if (ui_mouse_widget_has_focus(widget))
		return;
	widget_instance_give_focus_directly(widget_instance_get_topmost_parent(widget), widget);
	for (ancestor = widget; ancestor->parent; ancestor = ancestor->parent)
	{
		if (ancestor->parent->type == _ui_widget_type_column_list)
			ancestor->parent->parameters.list.selected_index = (short)ui_mouse_child_index(ancestor);
	}
	ui_play_audio_feedback_sound(_ui_audio_feedback_cursor);

	return;
}

/* the d-pad buttons that step a widget back and forward */
static void ui_mouse_list_directions(
	struct widget_instance *widget,
	short *back,
	short *forward)
{
	struct ui_widget_definition *definition = ui_widget_definition_get(widget->definition_tag_index);

	if (TEST_FLAG(definition->flags, _widget_dpad_leftright_tabs_thru_list_items_bit) ||
		TEST_FLAG(definition->flags, _widget_dpad_leftright_tabs_thru_children_bit))
	{
		*back = _widget_event_dpad_left;
		*forward = _widget_event_dpad_right;
	}
	else
	{
		*back = _widget_event_dpad_up;
		*forward = _widget_event_dpad_down;
	}

	return;
}

/* steps a list that shows several items to the one shown in a slot, as
pressing the d-pad that many times would */
static void ui_mouse_step_list_to_slot(
	struct widget_instance *slot)
{
	struct widget_instance *list = slot->parent;
	long steps;
	short back, forward;

	ui_mouse_give_focus(list);
	if (!list->focused_child)
		return;
	steps = ui_mouse_child_index(slot) - ui_mouse_child_index(list->focused_child);
	ui_mouse_list_directions(list, &back, &forward);
	for (; steps > 0; steps--)
		ui_mouse_press(forward);
	for (; steps < 0; steps++)
		ui_mouse_press(back);

	return;
}

/* the widget the wheel steps: the innermost on the focus's way that the
d-pad steps through */
static struct widget_instance *ui_mouse_wheel_widget(
	struct widget_instance *root)
{
	struct widget_instance *result = NULL;
	struct widget_instance *widget;

	for (widget = root; widget; widget = widget->focused_child)
	{
		struct ui_widget_definition *definition = ui_widget_definition_get(widget->definition_tag_index);

		if (definition->flags & (FLAG(_widget_dpad_updown_tabs_thru_children_bit) |
			FLAG(_widget_dpad_leftright_tabs_thru_children_bit) |
			FLAG(_widget_dpad_updown_tabs_thru_list_items_bit) |
			FLAG(_widget_dpad_leftright_tabs_thru_list_items_bit)))
		{
			result = widget;
		}
	}

	return result;
}

/* the menu the mouse drives: the first player's, or everyone's */
static struct widget_instance *ui_mouse_menu(
	void)
{
	long widget_index;

	for (widget_index = 0; widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS; widget_index++)
	{
		struct widget_instance *widget = widget_globals.active_widgets[widget_index];

		if (widget && (widget->local_player_index == NONE || widget->local_player_index == 0))
			return widget;
	}

	return NULL;
}

static boolean ui_mouse_menus_active(
	void)
{
	if (widget_globals.initialization_thread || progress_bar_is_active())
		return FALSE;

	return virtual_keyboard_active() || ui_mouse_menu() != NULL;
}

/* the pointer's motion, clicks and wheel since the last frame, as the first
player's controller events */
static void ui_widgets_process_mouse(
	void)
{
	struct halo_ui_pointer pointer;
	struct ui_mouse_target *target;
	short controller_index = 0;

	if (!halo_ui_pointer_update(ui_mouse_menus_active(), &pointer) ||
		virtual_keyboard_active())
	{
		ui_mouse_press_count = 0;
		ui_mouse_hover_pending = FALSE;
		ui_mouse_click_pending = FALSE;
	}
	else
	{
		if (pointer.moved)
		{
			ui_mouse_hover_pending = TRUE;
			ui_mouse_hover_x = pointer.x;
			ui_mouse_hover_y = pointer.y;
		}
		if (pointer.left_clicks)
		{
			ui_mouse_click_pending = TRUE;
			ui_mouse_click_x = pointer.click_x;
			ui_mouse_click_y = pointer.click_y;
		}
		if (pointer.right_clicks)
			ui_mouse_press(_widget_event_b_button);
		if (pointer.wheel_steps && ui_mouse_menu())
		{
			struct widget_instance *widget = ui_mouse_wheel_widget(ui_mouse_menu());
			short back = _widget_event_dpad_up, forward = _widget_event_dpad_down;
			long step;

			if (widget)
				ui_mouse_list_directions(widget, &back, &forward);
			for (step = 0; step < ABS(pointer.wheel_steps) && step < 4; step++)
				ui_mouse_press(pointer.wheel_steps > 0 ? back : forward);
		}
		if (!ui_mouse_press_count && ui_mouse_hover_pending)
		{
			ui_mouse_hover_pending = FALSE;
			target = ui_mouse_target_at(ui_mouse_hover_x, ui_mouse_hover_y);
			if (target)
			{
				switch (target->kind)
				{
				case _ui_mouse_target_item:
				case _ui_mouse_target_value:
					ui_mouse_give_focus(target->widget);
					break;
				case _ui_mouse_target_list_slot:
					ui_mouse_step_list_to_slot(target->widget);
					break;
				}
			}
		}
		if (!ui_mouse_press_count && ui_mouse_click_pending)
		{
			ui_mouse_click_pending = FALSE;
			target = ui_mouse_target_at(ui_mouse_click_x, ui_mouse_click_y);
			if (target)
			{
				switch (target->kind)
				{
				case _ui_mouse_target_item:
					ui_mouse_give_focus(target->widget);
					ui_mouse_press(_gamepad_analog_button_a);
					break;
				case _ui_mouse_target_value:
				{
					short back, forward;
					boolean first_half;

					ui_mouse_give_focus(target->widget);
					ui_mouse_list_directions(target->widget, &back, &forward);
					first_half = back == _widget_event_dpad_left ?
						ui_mouse_click_x < (target->bounds.x0 + target->bounds.x1) / 2 :
						ui_mouse_click_y < (target->bounds.y0 + target->bounds.y1) / 2;
					ui_mouse_press(first_half ? back : forward);
					break;
				}
				case _ui_mouse_target_list_slot:
					ui_mouse_step_list_to_slot(target->widget);
					ui_mouse_press(_gamepad_analog_button_a);
					break;
				case _ui_mouse_target_button:
					ui_mouse_press(target->button_index);
					break;
				}
			}
			else
			{
				long index;

				/* a screen with nothing to pick (a message to dismiss): the
				click is its A */
				for (index = 0; index < ui_mouse_target_count; index++)
				{
					if (ui_mouse_targets[index].kind != _ui_mouse_target_button)
						break;
				}
				if (index == ui_mouse_target_count)
					ui_mouse_press(_gamepad_analog_button_a);
			}
		}
		if (ui_mouse_press_count)
		{
			event_manager_post_button(controller_index, ui_mouse_presses[0]);
			csmemmove(ui_mouse_presses, ui_mouse_presses + 1, --ui_mouse_press_count * sizeof(ui_mouse_presses[0]));
		}
	}
	ui_mouse_target_count = 0;

	return;
}

#endif

static void widget_instance_render_recursive(
	struct widget_instance *widget,
	rectangle2d *clip_rect,
	point2d offset,
	boolean focus,
	boolean use_nifty_plasma_fx)
{
	struct ui_widget_definition *definition =
		ui_widget_definition_get(widget->definition_tag_index);
	real alpha_modifier = widget_instance_get_cumulative_alpha_modifier(widget);
	boolean render_children = TRUE;
	long input_index;
	struct widget_instance *child;
	struct bitmap_data *bitmap;

	if (!use_nifty_plasma_fx &&
		TEST_FLAG(definition->flags, _widget_always_render_with_nifty_fx_bit))
	{
		use_nifty_plasma_fx = TRUE;
	}
	offset.x += widget->horizontal_offset;
	offset.y += widget->vertical_offset;
	for (input_index = 0;
		input_index < definition->game_data_inputs.count;
		input_index++)
	{
		struct ui_widget_game_data_input_reference *input =
			(struct ui_widget_game_data_input_reference *)
				definition->game_data_inputs.address +
			input_index;

		ui_widget_game_data_function_invoke(widget, input->function);
	}
	if (!widget->visible)
		return;
#ifdef HALO_LINUX
	ui_mouse_note_target(widget, definition, offset);
#endif
	bitmap = bitmap_group_get_bitmap_from_sequence(
		definition->background_bitmap.index,
		0,
		widget->animation.current_frame_index);
	if (bitmap)
	{
		real alpha = alpha_modifier;
		rectangle2d bounds = definition->bounds;
		rectangle2d *clip = clip_rect;
		rectangle2d local_clip;
		pixel32 color;
		struct rasterizer_dynamic_screen_geometry_parameters multitexture_params;
		struct bitmap_group *bitmap_group =
			bitmap_group_get(definition->background_bitmap.index);
		struct bitmap_group_sequence *sequence = TAG_BLOCK_GET_ELEMENT(
			&bitmap_group->sequences,
			0,
			struct bitmap_group_sequence);

		if (use_nifty_plasma_fx)
		{
			ui_plasma_effect_color.alpha = 0.0f;
			ui_plasma_effect_color.red = 0.05f;
			ui_plasma_effect_color.green = 0.05f;
			ui_plasma_effect_color.blue = 0.05f;
		}
		bounds.x0 += offset.x;
		bounds.x1 += offset.x;
		bounds.y0 += offset.y;
		bounds.y1 += offset.y;
		if (clip)
		{
			local_clip = *clip;
			clip = &local_clip;
			clip->x0 += offset.x;
			clip->x1 += offset.x;
			clip->y0 += offset.y;
			clip->y1 += offset.y;
		}
		if (TEST_FLAG(definition->flags, _widget_flash_background_bitmap_bit))
		{
			alpha = (((real)cos(
				widget_globals.current_system_milliseconds *
					SECONDS_PER_MILLISECOND * 3.0f) + 1.0f) * 0.5f) *
				alpha_modifier;
		}
		color = modulate_pixel32_by_real_alpha(0xFFFFFFFF, alpha);
		draw_bitmap_in_rect(
			bitmap,
			&bounds,
			&bounds,
			clip,
			color,
			&multitexture_params,
			FALSE);
		if (use_nifty_plasma_fx)
		{
			ui_plasma_effect_color.alpha = 0.0f;
			ui_plasma_effect_color.red = 0.0f;
			ui_plasma_effect_color.green = 0.0f;
			ui_plasma_effect_color.blue = 0.0f;
		}
	}
	switch (widget->type)
	{
	case _ui_widget_type_text_box:
		widget_instance_render_text_box(
			widget,
			definition,
			clip_rect,
			offset,
			widget_instance_text_box_is_focused(widget));
		break;

	case _ui_widget_type_spinner_list:
		widget_instance_render_spinner_list(
			widget,
			definition,
			clip_rect,
			offset,
			focus);
		if (TEST_FLAG(
				definition->list_flags,
				_list_items_generated_from_string_list_tag) &&
			definition->child_widgets.count == 0)
		{
			render_children = FALSE;
		}
		break;

	case _ui_widget_type_column_list:
		widget_instance_render_column_list(
			widget,
			definition,
			clip_rect,
			offset,
			focus);
		render_children = !TEST_FLAG(
			definition->list_flags,
			_list_items_generated_in_code);
		break;
	}
	if (render_children)
	{
		for (child = widget->child; child; child = child->next)
		{
			focus = child == widget->focused_child;
			use_nifty_plasma_fx = focus &&
				(widget->type == _ui_widget_type_spinner_list ||
				widget->type == _ui_widget_type_column_list);
			widget_instance_render_recursive(
				child,
				clip_rect,
				offset,
				focus,
				use_nifty_plasma_fx);
		}
	}

	return;
}

void render_ui_widgets_postgame(
	short local_player_index,
	rectangle2d *window_bounds)
{
	point2d const offsets[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS][MAXIMUM_NUMBER_OF_LOCAL_PLAYERS] =
	{
		{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
		{ { 0, 0 }, { 0, 240 }, { 0, 0 }, { 0, 0 } },
		{ { 0, 0 }, { 0, 240 }, { 320, 240 }, { 0, 0 } },
		{ { 0, 0 }, { 320, 0 }, { 0, 240 }, { 320, 240 } }
	};
	rectangle2d bounds;
	long widget_index;

	if (virtual_keyboard_active())
		return;
	local_player_index = PIN(
		local_player_index,
		0,
		MAXIMUM_NUMBER_OF_LOCAL_PLAYERS - 1);
	for (widget_index = 0;
		widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		widget_index++)
	{
		struct widget_instance *widget = widget_globals.active_widgets[widget_index];
		boolean should_render = FALSE;

		if (widget)
		{
			if (widget->render_regardless_of_controller_index == TRUE)
			{
				should_render = TRUE;
			}
			else if (widget->widget_is_error_dialog == TRUE)
			{
				if (widget->local_player_index == local_player_index ||
					widget->local_player_index == NONE ||
					local_player_index == NONE ||
					we_are_at_the_main_menu)
				{
					should_render = TRUE;
				}
			}
			else if (widget->local_player_index == NONE && widget_index == 0)
			{
				should_render = TRUE;
			}
			else if (widget->local_player_index == local_player_index)
			{
				should_render = TRUE;
			}
		}
		if (should_render)
		{
			bounds.x0 = 0;
			bounds.y0 = 0;
			bounds.x1 = window_bounds->x1 - window_bounds->x0;
			bounds.y1 = window_bounds->y1 - window_bounds->y0;
			widget_instance_render_recursive(
				widget_globals.active_widgets[widget_index],
				&bounds,
				offsets[local_player_count() - 1][local_player_index],
				TRUE,
				FALSE);
		}
	}

	return;
}

void render_ui_widgets(
	short local_player_index,
	rectangle2d const *window_bounds)
{
	rectangle2d bounds;
	long widget_index;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		911,
		window_bounds != NULL);
	local_player_index_for_draw_string_and_hack_in_icons =
		local_player_index == NONE ? 0 : local_player_index;
	if (bink_playback_ui_rendering_inhibited())
		return;
	if (!virtual_keyboard_active())
	{
		local_player_index = PIN(
			local_player_index,
			0,
			MAXIMUM_NUMBER_OF_LOCAL_PLAYERS - 1);
		for (widget_index = 0;
			widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
			widget_index++)
		{
			struct widget_instance *widget = widget_globals.active_widgets[widget_index];
			boolean should_render = FALSE;

			if (widget)
			{
				if (widget->render_regardless_of_controller_index == TRUE)
				{
					should_render = TRUE;
				}
				else if (widget->widget_is_error_dialog == TRUE)
				{
					if (widget->local_player_index == local_player_index ||
						widget->local_player_index == NONE ||
						local_player_index == NONE ||
						we_are_at_the_main_menu)
					{
						should_render = TRUE;
					}
				}
				else if (widget->local_player_index == NONE && widget_index == 0)
				{
					should_render = TRUE;
				}
				else if (widget->local_player_index == local_player_index)
				{
					should_render = TRUE;
				}
			}
			if (should_render)
			{
				point2d offset;

				bounds.x0 = 0;
				bounds.y0 = 0;
				bounds.x1 = window_bounds->x1 - window_bounds->x0;
				bounds.y1 = window_bounds->y1 - window_bounds->y0;
				offset.x = 0;
				offset.y = 0;
#ifdef HALO_LINUX
				/* the mouse drives the first player's menus */
				ui_mouse_noting_targets = widget->local_player_index == NONE ||
					widget->local_player_index == 0;
#endif
				widget_instance_render_recursive(
					widget_globals.active_widgets[widget_index],
					&bounds,
					offset,
					TRUE,
					FALSE);
#ifdef HALO_LINUX
				ui_mouse_noting_targets = FALSE;
#endif
				if (widget_globals.debug_show_path)
				{
					real_argb_color color = { 1.0f, 1.0f, 1.0f, 1.0f };

					bounds.x0 += 32;
					bounds.x1 += 32;
					bounds.y0 += 32;
					bounds.y1 += 32;
					draw_string_set_draw_mode(
						tag_loaded(FONT_GROUP_TAG, "ui\\small_ui"),
						NONE,
						0,
						0,
						&color);
					rasterizer_draw_string(
						&bounds,
						NULL,
						NULL,
						0,
						tag_get_name(
							widget_globals.active_widgets[widget_index]->definition_tag_index));
				}
			}
		}
		if (widget_globals.fade_to_black >= 0.0f &&
			widget_globals.fade_to_black <= 1.0f)
		{
			real alpha;

#ifdef HALO_LINUX
			/* the whole screen, around the centered 640 columns */
			bounds.x0 = (short)(-(halo_screen_width() - 640) / 2);
			bounds.x1 = (short)(640 + (halo_screen_width() - 640) / 2);
#else
			bounds.x0 = 0;
			bounds.x1 = 640;
#endif
			bounds.y0 = 0;
			bounds.y1 = 480;
			if (widget_globals.fade_to_black >= 0.95f)
				widget_globals.fade_to_black = 1.0f;
			alpha = widget_globals.fade_to_black * 255.0f;
			draw_quad(&bounds, fast_ftol(alpha) << 24);
		}
	}
	else
	{
		virtual_keyboard_render();
	}

	return;
}

/* ---------- private code */

static void widget_instance_render_column_list(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	rectangle2d *clip_rect,
	point2d offset,
	boolean focus)
{
	if (widget->parameters.list.extended_description)
	{
		widget->parameters.list.extended_description->alpha_modifier =
			widget_instance_get_cumulative_alpha_modifier(widget);
		widget_instance_render_recursive(
			widget->parameters.list.extended_description,
			clip_rect,
			offset,
			FALSE,
			TRUE);
	}
	if (TEST_FLAG(definition->list_flags, _list_items_generated_in_code))
	{
		struct widget_instance *child;
		long item_index = 0;

		for (child = widget->child; child; child = child->next)
		{
			if (item_index >= widget->parameters.list.number_of_items)
				break;
			widget_instance_render_recursive(
				child,
				clip_rect,
				offset,
				focus,
				item_index == widget->parameters.list.selected_index);
			item_index++;
		}
	}
	widget->parameters.list.last_list_tab_direction = 0;

	return;
}

static __inline void widget_instance_update_animation_parameters(
	struct widget_instance *widget)
{
	widget->animation.first_frame_index =
		FLOOR(widget->animation.first_frame_index, 0);
	widget->animation.last_frame_index =
		FLOOR(widget->animation.last_frame_index, 0);

	return;
}

static __inline void spinner_list_update(
	struct widget_instance *widget)
{
	struct widget_instance *child;

	for (child = widget->child; child; child = child->next)
	{
		child->animation.current_frame_index = 0;
		if (child == widget->focused_child &&
			child->animation.number_of_sprite_frames == 2)
		{
			child->animation.current_frame_index = 1;
		}
	}

	return;
}

/* the definition is passed by January's only call site but is not read; the
spinner list and the column list keep separate updaters rather than sharing one
parameterised helper */

static void column_list_update(
	struct widget_instance *widget,
	struct ui_widget_definition *definition)
{
	struct widget_instance *child;

	for (child = widget->child; child; child = child->next)
	{
		if (child == widget->focused_child)
		{
			if (child->animation.number_of_sprite_frames == 2)
				child->animation.current_frame_index = 1;
		}
		else if (child->animation.number_of_sprite_frames == 2)
		{
			child->animation.current_frame_index = 0;
		}
	}

	return;
}

static void widget_instance_tab_to_next_valid_widget(
	struct widget_instance *widget)
{
	struct widget_instance *child;

	if (widget->focused_child && widget->focused_child->next)
		child = widget->focused_child->next;
	else
		child = widget->child;
	while (child && child != widget->focused_child)
	{
		struct ui_widget_definition *definition =
			ui_widget_definition_get(child->definition_tag_index);

		if (definition->event_handlers.count > 0 ||
			TEST_FLAG(definition->flags, _widget_pass_unhandled_events_to_children_bit) ||
			widget->type == _ui_widget_type_spinner_list ||
			widget->type == _ui_widget_type_column_list)
		{
			widget->focused_child = child;
			break;
		}
		child = child->next;
		if (!child)
			child = widget->child;
	}

	return;
}

static void widget_instance_tab_to_previous_valid_widget(
	struct widget_instance *widget)
{
	struct widget_instance *child;

	if (widget->focused_child)
	{
		if (widget->focused_child->previous)
			child = widget->focused_child->previous;
		else
			child = widget_instance_get_tail_child_widget(widget);
	}
	else
	{
		child = widget->child->previous;
		if (!child)
			child = widget->child;
	}
	while (child && child != widget->focused_child)
	{
		struct ui_widget_definition *definition =
			ui_widget_definition_get(child->definition_tag_index);

		if (definition->event_handlers.count > 0 ||
			TEST_FLAG(definition->flags, _widget_pass_unhandled_events_to_children_bit) ||
			widget->type == _ui_widget_type_spinner_list ||
			widget->type == _ui_widget_type_column_list)
		{
			widget->focused_child = child;
			break;
		}
		if (child->previous)
			child = child->previous;
		else
			child = widget_instance_get_tail_child_widget(widget);
	}

	return;
}

static void widget_instance_process_one_event_recursive(
	struct widget_instance *widget,
	struct ui_widget_definition *definition,
	struct event_record *event,
	boolean *return_widget_deleted)
{
	boolean event_handled = FALSE;
	boolean widget_deleted = FALSE;
	boolean event_for_this_widget = widget->local_player_index == NONE ||
		widget->local_player_index == event->controller_index;
	long audio_feedback = _ui_audio_feedback_none;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		3067,
		widget && definition && event && return_widget_deleted);
	if (event->type == _event_type_button &&
		event->data.button.value > 1 &&
		event->controller_index >= 0 &&
		event->controller_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS &&
		event->data.button.index >= _widget_event_dpad_up &&
		event->data.button.index <= _widget_event_dpad_right &&
		widget_globals.current_system_milliseconds -
			dpad_event_times[event->controller_index]
				[event->data.button.index - _widget_event_dpad_up] >=
			DPAD_EVENT_REPEAT_MILLISECONDS)
	{
		event->data.button.value = 1;
	}
	if (widget->close_if_local_player_controller_present == TRUE)
	{
		if (widget->local_player_index >= 0 &&
			widget->local_player_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS)
		{
			if (input_has_gamepad(widget->local_player_index))
			{
				ui_widget_delete(widget_instance_get_topmost_parent(widget));
				widget_deleted = TRUE;
			}
		}
		else
		{
			short controller_index;

			match_assert(
				"c:\\halo\\SOURCE\\interface\\ui_widget.c",
				3107,
				widget->local_player_index==NONE);
			for (controller_index = 0;
				controller_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
				controller_index++)
			{
				if (input_has_gamepad(controller_index))
				{
					ui_widget_delete(widget_instance_get_topmost_parent(widget));
					widget_deleted = TRUE;
					break;
				}
			}
		}
	}
	if (event_for_this_widget && !widget_deleted)
	{
		if (event->type == _event_type_button &&
			event->data.button.value == 1)
		{
			boolean handled_by_event_handler = FALSE;
			long handler_index;

			if (event->data.button.index == _widget_event_back_button)
			{
				for (handler_index = 0;
					handler_index < definition->event_handlers.count;
					handler_index++)
				{
					struct ui_widget_event_handler_reference *handler =
						(struct ui_widget_event_handler_reference *)
							definition->event_handlers.address + handler_index;

					if (handler->event_type == _widget_event_back_button)
					{
						handled_by_event_handler = TRUE;
						break;
					}
				}
			}
			else if (event->data.button.index == _widget_event_b_button)
			{
				for (handler_index = 0;
					handler_index < definition->event_handlers.count;
					handler_index++)
				{
					struct ui_widget_event_handler_reference *handler =
						(struct ui_widget_event_handler_reference *)
							definition->event_handlers.address + handler_index;

					if (handler->event_type == _widget_event_b_button)
					{
						handled_by_event_handler = TRUE;
						break;
					}
				}
			}
			else
			{
				handled_by_event_handler = TRUE;
			}
			if (!handled_by_event_handler)
			{
				widget_instance_go_back_to_previous(widget);
				audio_feedback = _ui_audio_feedback_back;
				widget_deleted = TRUE;
				event_handled = TRUE;
			}
		}
	}
	if (!widget_deleted)
	{
		if (widget->milliseconds_to_auto_close > 0)
		{
			if (widget_globals.current_system_milliseconds - widget->creation_time >=
				widget->auto_close_fade_time + widget->milliseconds_to_auto_close)
			{
				ui_widget_delete(widget_instance_get_topmost_parent(widget));
				widget_deleted = TRUE;
			}
			else if (widget->auto_close_fade_time > 0)
			{
				long faded_milliseconds =
					widget_globals.current_system_milliseconds -
						widget->creation_time -
						widget->milliseconds_to_auto_close;

				if (faded_milliseconds > 0)
				{
					widget->alpha_modifier = 1.0f - (real)faded_milliseconds /
						(real)widget->auto_close_fade_time;
				}
			}
		}
	}
	if (!widget_deleted)
	{
		widget_instance_update_animation_parameters(widget);
		if (widget->type == _ui_widget_type_spinner_list)
			spinner_list_update(widget);
		else if (widget->type == _ui_widget_type_column_list)
			column_list_update(widget, definition);
		if (event_for_this_widget)
		{
			if (!event_handled &&
				TEST_FLAG(definition->flags, _widget_dpad_updown_tabs_thru_children_bit) &&
				widget->focused_child &&
				!widget_deleted)
			{
				if (event->type == _event_type_button &&
					event->data.button.value == 1)
				{
					switch (event->data.button.index)
					{
					case _widget_event_dpad_up:
						widget_instance_tab_to_previous_valid_widget(widget);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					case _widget_event_dpad_down:
						widget_instance_tab_to_next_valid_widget(widget);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					}
				}
				else if (event->type == _event_type_left_stick)
				{
					switch (event->data.stick.y)
					{
					case SHORT_MIN:
						widget_instance_tab_to_next_valid_widget(widget);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					case SHORT_MAX:
						widget_instance_tab_to_previous_valid_widget(widget);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					}
				}
			}
			if (!event_handled &&
				TEST_FLAG(definition->flags, _widget_dpad_leftright_tabs_thru_children_bit) &&
				widget->focused_child &&
				!widget_deleted)
			{
				if (event->type == _event_type_button &&
					event->data.button.value == 1)
				{
					switch (event->data.button.index)
					{
					case _widget_event_dpad_left:
						widget_instance_tab_to_previous_valid_widget(widget);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					case _widget_event_dpad_right:
						widget_instance_tab_to_next_valid_widget(widget);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					}
				}
				else if (event->type == _event_type_left_stick)
				{
					switch (event->data.stick.x)
					{
					case SHORT_MIN:
						widget_instance_tab_to_previous_valid_widget(widget);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					case SHORT_MAX:
						widget_instance_tab_to_next_valid_widget(widget);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					}
				}
			}
			if (TEST_FLAG(definition->flags, _widget_dpad_updown_tabs_thru_list_items_bit) &&
				(widget->type == _ui_widget_type_spinner_list ||
					widget->type == _ui_widget_type_column_list) &&
				!event_handled &&
				!widget_deleted)
			{
				if (event->type == _event_type_button &&
					event->data.button.value == 1)
				{
					switch (event->data.button.index)
					{
					case _widget_event_dpad_up:
						widget_event_function_list_widget_goto_previous_item(
							widget,
							event,
							&widget_deleted);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					case _widget_event_dpad_down:
						widget_event_function_list_widget_goto_next_item(
							widget,
							event,
							&widget_deleted);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					}
				}
				else if (event->type == _event_type_left_stick)
				{
					switch (event->data.stick.y)
					{
					case SHORT_MIN:
						widget_event_function_list_widget_goto_next_item(
							widget,
							event,
							&widget_deleted);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					case SHORT_MAX:
						widget_event_function_list_widget_goto_previous_item(
							widget,
							event,
							&widget_deleted);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					}
				}
			}
			if (TEST_FLAG(definition->flags, _widget_dpad_leftright_tabs_thru_list_items_bit) &&
				(widget->type == _ui_widget_type_spinner_list ||
					widget->type == _ui_widget_type_column_list) &&
				!event_handled &&
				!widget_deleted)
			{
				if (event->type == _event_type_button &&
					event->data.button.value == 1)
				{
					switch (event->data.button.index)
					{
					case _widget_event_dpad_left:
						widget_event_function_list_widget_goto_previous_item(
							widget,
							event,
							&widget_deleted);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					case _widget_event_dpad_right:
						widget_event_function_list_widget_goto_next_item(
							widget,
							event,
							&widget_deleted);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					}
				}
				else if (event->type == _event_type_left_stick)
				{
					switch (event->data.stick.x)
					{
					case SHORT_MIN:
						widget_event_function_list_widget_goto_previous_item(
							widget,
							event,
							&widget_deleted);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					case SHORT_MAX:
						widget_event_function_list_widget_goto_next_item(
							widget,
							event,
							&widget_deleted);
						if (audio_feedback == _ui_audio_feedback_none)
							audio_feedback = _ui_audio_feedback_cursor;
						event_handled = TRUE;
						break;
					}
				}
			}
		}
	}
	if (event_for_this_widget)
	{
		long handler_index;

		for (handler_index = 0;
			handler_index < definition->event_handlers.count;
			handler_index++)
		{
			struct ui_widget_event_handler_reference *handler;
			boolean handler_matches = FALSE;

			if (widget_deleted)
				break;
			handler = (struct ui_widget_event_handler_reference *)
				definition->event_handlers.address + handler_index;
			switch (event->type)
			{
			case _event_type_left_stick:
				switch (handler->event_type)
				{
				case _widget_event_left_stick_up:
					handler_matches = event->data.stick.y == SHORT_MAX;
					break;
				case _widget_event_left_stick_down:
					handler_matches = event->data.stick.y == SHORT_MIN;
					break;
				case _widget_event_left_stick_left:
					handler_matches = event->data.stick.x == SHORT_MIN;
					break;
				case _widget_event_left_stick_right:
					handler_matches = event->data.stick.x == SHORT_MAX;
					break;
				}
				break;
			case _event_type_right_stick:
				switch (handler->event_type)
				{
				case _widget_event_right_stick_up:
					handler_matches = event->data.stick.y == SHORT_MAX;
					break;
				case _widget_event_right_stick_down:
					handler_matches = event->data.stick.y == SHORT_MIN;
					break;
				case _widget_event_right_stick_left:
					handler_matches = event->data.stick.x == SHORT_MIN;
					break;
				case _widget_event_right_stick_right:
					handler_matches = event->data.stick.x == SHORT_MAX;
					break;
				}
				break;
			case _event_type_button:
				handler_matches = handler->event_type == event->data.button.index &&
					event->data.button.value == 1;
				break;
			}
			if (handler_matches)
			{
				event_handled = TRUE;
				event_handler_dispatch(
					widget,
					definition,
					event,
					handler,
					&widget_deleted);
			}
		}
	}
	match_vwarn(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		3477,
		!TEST_FLAG(definition->flags, _widget_pass_handled_events_to_all_children_bit) ||
			TEST_FLAG(definition->flags, _widget_pass_unhandled_events_to_children_bit),
		"if the _widget_pass_handled_events_to_all_children_bit flag is checked, _widget_pass_unhandled_events_to_children_bit must also be checked for it to work");
	if ((TEST_FLAG(definition->flags, _widget_pass_handled_events_to_all_children_bit) ||
			!event_handled) &&
		(TEST_FLAG(definition->flags, _widget_pass_unhandled_events_to_children_bit) ||
			TEST_FLAG(definition->flags, _widget_pass_unhandled_events_to_all_children_bit)) &&
		!widget_deleted)
	{
		if (TEST_FLAG(definition->flags, _widget_pass_unhandled_events_to_all_children_bit))
		{
			struct widget_instance *child;

			for (child = widget->child; child; child = child->next)
			{
				if (child->local_player_index == NONE ||
					child->local_player_index == event->controller_index)
				{
					widget_instance_process_one_event_recursive(
						child,
						ui_widget_definition_get(child->definition_tag_index),
						event,
						&widget_deleted);
					if (widget_deleted == TRUE)
						break;
				}
			}
		}
		else if (widget->focused_child)
		{
			if (widget->focused_child->local_player_index == NONE ||
				widget->focused_child->local_player_index == event->controller_index)
			{
				widget_instance_process_one_event_recursive(
					widget->focused_child,
					ui_widget_definition_get(
						widget->focused_child->definition_tag_index),
					event,
					&widget_deleted);
			}
		}
	}
	if (widget_deleted == TRUE &&
		TEST_FLAG(definition->flags, _widget_return_to_main_menu_if_no_history_bit))
	{
		long widget_index;

		for (widget_index = 0;
			widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
			widget_index++)
		{
			if (widget_globals.active_widgets[widget_index])
				break;
		}
		if (widget_index == MAXIMUM_NUMBER_OF_LOCAL_PLAYERS)
			main_goto_main_menu();
	}
	if (event->type == _event_type_button &&
		event->data.button.value == 1 &&
		event->controller_index >= 0 &&
		event->controller_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS &&
		event->data.button.index >= _widget_event_dpad_up &&
		event->data.button.index <= _widget_event_dpad_right)
	{
		dpad_event_times[event->controller_index]
			[event->data.button.index - _widget_event_dpad_up] =
			widget_globals.current_system_milliseconds;
	}
	ui_play_audio_feedback_sound(audio_feedback);
	*return_widget_deleted = widget_deleted;

	return;
}

static boolean ui_check_for_pause_game(
	void)
{
	boolean pause_pressed = FALSE;
	boolean network_game = network_game_is_active();

	if (game_in_progress() &&
		!cinematic_in_progress() &&
		game_connection() != _game_connection_film_playback &&
		!we_are_at_the_main_menu &&
		widget_globals.pause_disabled_ticks == 0)
	{
		short controller_index;

		for (controller_index = 0;
			controller_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
			controller_index++)
		{
			if (input_has_gamepad(controller_index) &&
				local_player_exists(controller_index) &&
				input_get_gamepad_state(controller_index)->
					buttons[_gamepad_binary_button_start] == 1)
			{
				boolean pressed_by_first_local_player = TRUE;
				short local_player_count = 0;
				short pressing_local_player_index = NONE;
				short local_player_index;

				pause_pressed = TRUE;
				for (local_player_index = local_player_get_next(NONE);
					local_player_index != NONE;
					local_player_index = local_player_get_next(local_player_index))
				{
					if (local_player_index == controller_index)
					{
						pressing_local_player_index = controller_index;
						if (local_player_count >= 1)
							pressed_by_first_local_player = FALSE;
					}
					local_player_count++;
				}
				if (network_game)
				{
					if (game_engine_allow_pause() &&
						pressing_local_player_index == controller_index)
					{
						if (!widget_globals.active_widgets[controller_index])
						{
							struct network_game_client *client = global_network_game_client_get();
							struct network_game *network_game_data =
								network_game_client_get_game(client);
							short machine_index =
								network_game_client_get_machine_index(client);
							char const *widget_name;

							switch (local_player_count)
							{
							case 1:
								widget_name =
									"ui\\shell\\multiplayer_game\\pause_game\\1p_pause_game";
								break;
							case 2:
								widget_name =
									"ui\\shell\\multiplayer_game\\pause_game\\2p_pause_game";
								break;
							case 3:
								widget_name = pressed_by_first_local_player == TRUE
									? "ui\\shell\\multiplayer_game\\pause_game\\2p_pause_game"
									: "ui\\shell\\multiplayer_game\\pause_game\\4p_pause_game";
								break;
							case 4:
								widget_name =
									"ui\\shell\\multiplayer_game\\pause_game\\4p_pause_game";
								break;
							default:
								error(
									_error_silent,
									"invalid local player count for multiplayer game");
								widget_name = NULL;
								break;
							}
							if (widget_name &&
								!ui_widget_load_by_name_or_tag(
									widget_name,
									NONE,
									NULL,
									controller_index,
									NONE,
									NONE,
									NONE))
							{
								error(
									_error_silent,
									"failed to load multiplayer pause game window");
							}
						}
						else
						{
							ui_widget_delete(widget_globals.active_widgets[controller_index]);
						}
					}
				}
				else
				{
					switch (local_player_count)
					{
					case 0:
					case 1:
						if (widget_globals.active_widgets[controller_index])
						{
							if (game_time_get_paused() == TRUE)
								ui_widgets_close_all();
						}
						else if (!ui_widget_load_by_name_or_tag(
							"ui\\shell\\solo_game\\pause_game\\pause_game",
							NONE,
							NULL,
							controller_index,
							NONE,
							NONE,
							NONE))
						{
							error(
								_error_silent,
								"failed to load full screen pause game window");
						}
						break;
					case 2:
						if (widget_globals.active_widgets[controller_index])
						{
							if (game_time_get_paused() == TRUE)
								ui_widgets_close_all();
						}
						else if (!game_time_get_paused())
						{
							if (!ui_widget_load_by_name_or_tag(
								"ui\\shell\\solo_game\\pause_game\\pause_game_split_screen",
								NONE,
								NULL,
								controller_index,
								NONE,
								NONE,
								NONE))
							{
								error(
									_error_silent,
									"failed to load split screen pause game window");
							}
						}
						break;
					default:
						error(
							_error_silent,
							"the ui seems to be confused... assuming you are playing full-screen single player?");
						if (widget_globals.initialized)
						{
							boolean widgets_active = FALSE;
							long widget_index;

							for (widget_index = 0;
								widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
								widget_index++)
							{
								if (widget_globals.active_widgets[widget_index])
								{
									widgets_active = TRUE;
									break;
								}
							}
							if (widgets_active)
							{
								ui_widgets_close_all();
								break;
							}
						}
						if (!ui_widget_load_by_name_or_tag(
							"ui\\shell\\solo_game\\pause_game\\pause_game",
							NONE,
							NULL,
							controller_index,
							NONE,
							NONE,
							NONE))
						{
							error(
								_error_silent,
								"failed to load full screen pause game window");
						}
						break;
					}
				}
				break;
			}
		}
	}
#ifdef HALO_LINUX
	/* This runs once a frame, several frames per tick on the native builds
	(port/linux/game/render_interpolation.c): count the lock down in 30 Hz
	ticks of real time, not in frames. */
	{
		static real leftover_ticks = 0.f;
		long ticks;

		leftover_ticks += main_get_seconds_elapsed() * TICKS_PER_SECOND;
		ticks = (long)leftover_ticks;
		leftover_ticks -= (real)ticks;
		widget_globals.pause_disabled_ticks =
			FLOOR(widget_globals.pause_disabled_ticks - ticks, 0);
	}
#else
	widget_globals.pause_disabled_ticks =
		FLOOR(widget_globals.pause_disabled_ticks - 1, 0);
#endif

	return pause_pressed;
}

void process_ui_widgets(
	void)
{
	boolean widgets_processed = FALSE;
	boolean widget_deleted;
	boolean any_modal_widget_active;
	boolean pause_pressed;
	boolean modal_widget_active[MAXIMUM_NUMBER_OF_LOCAL_PLAYERS];
	long widget_index;

	match_assert(
		"c:\\halo\\SOURCE\\interface\\ui_widget.c",
		644,
		widget_globals.initialized);
	widget_globals.current_system_milliseconds = system_milliseconds();
#ifdef HALO_LINUX
	ui_widgets_process_mouse();
#endif
	if (widget_globals.initialization_thread)
	{
		if (!thread_has_exited(widget_globals.initialization_thread))
			return;
		dispose_thread(widget_globals.initialization_thread);
		widget_globals.initialization_thread = NULL;
		ui_widgets_inhibit_processing(FALSE);
		switch (widget_globals.filesystem_check_result)
		{
		case _file_system_check_result_not_enough_free_space:
			if (bink_playback_in_progress())
				bink_playback_stop();
			display_error_abort_to_dashboard(
				_error_hard_drive_not_enough_free_space,
				TRUE);
			return;
		case _file_system_check_result_too_many_saved_games:
			if (bink_playback_in_progress())
				bink_playback_stop();
			display_error_abort_to_dashboard(
				_error_hard_drive_maximum_saved_game_files,
				TRUE);
			return;
		}

		return;
	}
	if (progress_bar_is_active())
		return;
	if (virtual_keyboard_active())
	{
		virtual_keyboard_process();
		event_manager_flush();

		return;
	}
	if (attract_mode_should_start())
	{
		attract_mode_start();

		return;
	}
	if (widget_globals.deferred_dashboard_error_code != NONE)
	{
		display_error_abort_to_dashboard(
			widget_globals.deferred_dashboard_error_code,
			widget_globals.deferred_dashboard_optional);
		widget_globals.deferred_dashboard_error_code = NONE;

		return;
	}
	if (widget_globals.deferred_errors[0].error_code != NONE ||
		widget_globals.deferred_errors[1].error_code != NONE ||
		widget_globals.deferred_errors[2].error_code != NONE ||
		widget_globals.deferred_errors[3].error_code != NONE)
	{
		for (widget_index = 0;
			widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
			widget_index++)
		{
			struct ui_widget_deferred_error *deferred_error =
				&widget_globals.deferred_errors[widget_index];

			if (deferred_error->error_code != NONE)
			{
				if (!we_are_at_the_main_menu &&
					!network_game_is_active() &&
					game_time_get() < DEFERRED_ERROR_DELAY_TICKS)
				{
					error(
						_error_silent,
						"waiting for %d ticks before displaying deferred errors",
						DEFERRED_ERROR_DELAY_TICKS);
				}
				else
				{
					display_error(
						deferred_error->error_code,
						deferred_error->local_player_index,
						deferred_error->modal,
						deferred_error->pause_game_time);
					deferred_error->error_code = NONE;
				}
			}
		}

		return;
	}
	pause_pressed = ui_check_for_pause_game();
	any_modal_widget_active = FALSE;
	for (widget_index = 0;
		widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		widget_index++)
	{
		modal_widget_active[widget_index] =
			widget_globals.active_widgets[widget_index] &&
			widget_globals.active_widgets[widget_index]->widget_is_error_dialog == TRUE;
		any_modal_widget_active |= modal_widget_active[widget_index];
	}
	for (widget_index = 0;
		widget_index < MAXIMUM_NUMBER_OF_LOCAL_PLAYERS;
		widget_index++)
	{
		struct widget_instance *widget = widget_globals.active_widgets[widget_index];
		boolean process_widget;

		if (modal_widget_active[widget_index] == TRUE)
			process_widget = widget && widget->widget_is_error_dialog == TRUE;
		else if (we_are_at_the_main_menu)
			process_widget = widget && !any_modal_widget_active;
		else
			process_widget = widget != NULL;
		if (process_widget == TRUE)
		{
			struct ui_widget_definition *definition =
				ui_widget_definition_get(widget->definition_tag_index);
			struct event_record event = {0};

			if (widget_globals.processing_inhibited ||
				!get_next_event(&event, widget->local_player_index))
			{
				/* the widget still gets one empty event so that its animation,
				auto-close timer and fade keep running */
				if (!pause_pressed)
				{
					event.controller_index = widget->local_player_index;
					widget_instance_process_one_event_recursive(
						widget,
						definition,
						&event,
						&widget_deleted);
				}
			}
			else
			{
				do
				{
					if (!pause_pressed)
					{
						widget_instance_process_one_event_recursive(
							widget,
							definition,
							&event,
							&widget_deleted);
						if (widget_deleted == TRUE)
							break;
					}
					if (widget != widget_globals.active_widgets[widget_index])
						break;
				}
				while (get_next_event(&event, widget->local_player_index));
			}
			widgets_processed = TRUE;
			if (!widget_globals.active_widgets[widget_index] &&
				widget_globals.widget_stack[widget_index])
			{
				struct widget_stack_data data;

				pop_widget(&widget_globals.widget_stack[widget_index], &data);
				if (data.previous_widget_tag != NONE)
				{
					struct widget_instance *new_widget = ui_widget_load_by_name_or_tag(
						NULL,
						data.previous_widget_tag,
						NULL,
						data.local_player_index,
						NONE,
						NONE,
						NONE);

					if (new_widget)
					{
						widget_instance_set_focused_child_by_index(
							data.focused_child_parent_widget_tag,
							new_widget,
							data.focused_child_index);
					}
				}
			}
		}
	}
	if (widgets_processed)
		event_manager_flush();

	return;
}
