/*
AI_COMMUNICATION.C

symbols in this file:
00031040 0150:
	_ai_communication_initialize (0000)
00031190 0010:
	_ai_communication_dispose (0000)
000311A0 0110:
	_ai_communication_initialize_for_new_map (0000)
000312B0 0010:
	_ai_communication_dispose_from_old_map (0000)
000312C0 0030:
	_ai_communication_get_type_name (0000)
000312F0 0040:
	_ai_communication_get_type_by_name (0000)
00031330 0060:
	_ai_communication_packet_new (0000)
00031390 0070:
	_reply_filter_close (0000)
00031400 0070:
	_reply_filter_not_close (0000)
00031470 0050:
	_reply_filter_searching (0000)
000314C0 0090:
	_reply_filter_same_platoon (0000)
00031550 0020:
	_reply_filter_fighting (0000)
00031570 0040:
	_reply_filter_fighting_close (0000)
000315B0 00b0:
	_reply_filter_same_target (0000)
00031660 0040:
	_reply_filter_no_certain_target (0000)
000316A0 0040:
	_reply_filter_flee_leader (0000)
000316E0 01a0:
	_ai_communication_consider_speech (0000)
00031880 0040:
	_actor_communication_team (0000)
000318C0 00b0:
	_ai_communication_look_secondary_at_unit (0000)
00031970 0050:
	_ai_communication_look_secondary_at_object (0000)
000319C0 0110:
	_ai_conversation_status (0000)
00031AD0 0060:
	_ai_conversation_line (0000)
00031B30 0090:
	_ai_conversation_advance (0000)
00031BC0 0190:
	_ai_conversation_finish (0000)
00031D50 0130:
	_ai_conversation_new (0000)
00031E80 01b0:
	_ai_conversation_line_begin (0000)
00032030 0290:
	_ai_conversation_line_perform (0000)
000322C0 0030:
	_ai_conversation_line_end (0000)
000322F0 00d0:
	_actor_reset_idle_vocalization_timer (0000)
000323C0 00f0:
	_actor_communication_update (0000)
000324B0 0080:
	_ai_communication_handle_received_looking (0000)
00032530 02a0:
	_ai_communication_update_speech_timers (0000)
000327D0 0340:
	_ai_communication_get_player_rating (0000)
00032B10 0090:
	_ai_conversation_stop (0000)
00032BA0 00d0:
	_ai_conversation_actor_deleted (0000)
00032C70 0170:
	_ai_conversation_unit_died (0000)
00032DE0 0800:
	_ai_conversation_find_participant (0000)
000335E0 02c0:
	_ai_communication_started (0000)
000338A0 0210:
	_ai_communication_notify (0000)
00033AB0 0390:
	_ai_communication_actor_talk_weight (0000)
00033E40 00c0:
	_ai_communication_find_specific_actor_to_talk (0000)
00033F00 0120:
	_ai_communication_find_global_actor_to_talk (0000)
00034020 06d0:
	_ai_conversation_begin (0000)
000346F0 0450:
	_ai_communication_find_actor_to_reply_to_player (0000)
00034B40 0630:
	_ai_communication_finished (0000)
00035170 0150:
	_ai_conversation (0000)
000352C0 0260:
	_ai_conversation_update (0000)
00035520 1f80:
	_ai_communication_event (0000)
00247808 18c0:
	_communication_speech_priorities (0000)
	_communication_notification_delays (0010)
	_communication_protagonist_default_look_priorities (0030)
	_communication_recipient_default_look_priorities (0040)
	_communication_player_speaking_priorities (0050)
	_communication_unit_prefer_silent_time (0060)
	_communication_timer_tolerances (0068)
	_communication_play_delays (01a8)
	_communication_player_additional_delay (01bc)
	_communication_overlap_time_modifier (01c0)
	_communication_timeout_low_priority_modifier (01c4)
	_communication_repeat_selection_time (01c8)
	_communication_player_absolute_range (01cc)
	_communication_player_ideal_range_min (01d0)
	_communication_player_ideal_range_max (01d4)
	_communication_player_ideal_fov (01d8)
	_communication_player_rating_low_priority (01dc)
	_global_dialogue_table (01e0)
	_global_reply_table (1248)
002490C8 0015:
	??_C@_0BF@PLOFNBGK@postcombat_celebrate?$AA@ (0000)
002490E0 0018:
	??_C@_0BI@BNIOPALA@postcombat_shoot_corpse?$AA@ (0000)
002490F8 0018:
	??_C@_0BI@LCABFNHO@postcombat_check_friend?$AA@ (0000)
00249110 0017:
	??_C@_0BH@JEELJBED@postcombat_check_enemy?$AA@ (0000)
00249128 0013:
	??_C@_0BD@MKOGOGNM@postcombat_triumph?$AA@ (0000)
0024913C 0014:
	??_C@_0BE@OGGIDGFC@postcombat_massacre?$AA@ (0000)
00249150 0013:
	??_C@_0BD@OKBEBJFP@postcombat_wounded?$AA@ (0000)
00249164 0015:
	??_C@_0BF@OFGIJICH@postcombat_unscathed?$AA@ (0000)
0024917C 0011:
	??_C@_0BB@LGDDKHGB@postcombat_alone?$AA@ (0000)
00249190 0005:
	??_C@_04PMNENHO@leap?$AA@ (0000)
00249198 0008:
	??_C@_07KMLHDIFF@falling?$AA@ (0000)
002491A0 0014:
	??_C@_0BE@DIJKKGAP@uncover_exclamation?$AA@ (0000)
002491B4 0005:
	??_C@_04FLLHOKOG@dive?$AA@ (0000)
002491BC 0006:
	??_C@_05IMODGEHH@melee?$AA@ (0000)
002491C4 0009:
	??_C@_08MPLMAFLL@surprise?$AA@ (0000)
002491D0 0010:
	??_C@_0BA@HBKFDHMB@vehicle_falling?$AA@ (0000)
002491E0 000f:
	??_C@_0P@EOJFLCO@vehicle_scared?$AA@ (0000)
002491F0 000f:
	??_C@_0P@OJJDOKMB@vehicle_woohoo?$AA@ (0000)
00249200 000d:
	??_C@_0N@KIHOLEBN@vehicle_exit?$AA@ (0000)
00249210 000e:
	??_C@_0O@NEANEBAA@vehicle_entry?$AA@ (0000)
00249220 0010:
	??_C@_0BA@JKBMANAN@hiding_finished?$AA@ (0000)
00249230 000f:
	??_C@_0P@BOOLPEN@attempted_flee?$AA@ (0000)
00249240 000a:
	??_C@_09LPKHDOHP@flee_idle?$AA@ (0000)
0024924C 0011:
	??_C@_0BB@GDNLPHOF@flee_leader_died?$AA@ (0000)
00249260 0011:
	??_C@_0BB@GKOPGPMO@shooting_traitor?$AA@ (0000)
00249274 000f:
	??_C@_0P@IDHEPNJE@shooting_group?$AA@ (0000)
00249284 0011:
	??_C@_0BB@LKALKDPF@shooting_berserk?$AA@ (0000)
00249298 0011:
	??_C@_0BB@GHLHMONC@shooting_vehicle?$AA@ (0000)
002492AC 0009:
	??_C@_08HFADMLAA@shooting?$AA@ (0000)
002492B8 0016:
	??_C@_0BG@IIEIONGD@sighted_friend_player?$AA@ (0000)
002492D0 0008:
	??_C@_07LADJDDJF@retreat?$AA@ (0000)
002492D8 0008:
	??_C@_07JKODCMP@advance?$AA@ (0000)
002492E0 000e:
	??_C@_0O@OAGOMGLF@uncover_start?$AA@ (0000)
002492F0 0015:
	??_C@_0BF@HNJCJGJJ@search_group_abandon?$AA@ (0000)
00249308 000f:
	??_C@_0P@GDPJMJHJ@search_abandon?$AA@ (0000)
00249318 000e:
	??_C@_0O@EKDNJHKG@search_report?$AA@ (0000)
00249328 000d:
	??_C@_0N@MDODPJCI@search_query?$AA@ (0000)
00249338 000d:
	??_C@_0N@IJIHJIIJ@search_start?$AA@ (0000)
00249348 0010:
	??_C@_0BA@KFNIENAA@alert_noncombat?$AA@ (0000)
00249358 0008:
	??_C@_07MAJPMGLJ@blocked?$AA@ (0000)
00249360 000d:
	??_C@_0N@JEODFMIK@lost_contact?$AA@ (0000)
00249370 000f:
	??_C@_0P@JELOHPAB@grenade_danger?$AA@ (0000)
00249380 0010:
	??_C@_0BA@GHAABFAJ@grenade_sighted?$AA@ (0000)
00249390 0010:
	??_C@_0BA@ILMKIOPH@grenade_startle?$AA@ (0000)
002493A0 0011:
	??_C@_0BB@NDMOOLOJ@grenade_throwing?$AA@ (0000)
002493B4 0013:
	??_C@_0BD@JHAEBNEM@allegiance_changed?$AA@ (0000)
002493C8 0012:
	??_C@_0BC@LJDIENED@found_dead_friend?$AA@ (0000)
002493DC 0011:
	??_C@_0BB@HACOIKAB@unexpected_enemy?$AA@ (0000)
002493F0 000c:
	??_C@_0M@EJKFHLEL@found_enemy?$AA@ (0000)
002493FC 000e:
	??_C@_0O@CDKKCOME@sighted_enemy?$AA@ (0000)
0024940C 0005:
	??_C@_04NAFIGEAM@hurt?$AA@ (0000)
00249414 000e:
	??_C@_0O@EGHFODNC@killing_spree?$AA@ (0000)
00249424 0006:
	??_C@_05PBMMFKGI@death?$AA@ (0000)
0024942C 0004:
	??_C@_03MADNOHBP@COV?$AA@ (0000)
00249430 0009:
	??_C@_08ONAJMEN@covenant?$AA@ (0000)
0024943C 0004:
	??_C@_03KPLLINCC@HUM?$AA@ (0000)
00249440 0006:
	??_C@_05OAIPHEDE@human?$AA@ (0000)
00249448 0008:
	??_C@_07BLDMICJO@exclaim?$AA@ (0000)
00249450 0005:
	??_C@_04EJGCJLIC@yell?$AA@ (0000)
00249458 000c:
	??_C@_0M@JHEHDIOO@communicate?$AA@ (0000)
00249464 0005:
	??_C@_04HBNBFOKD@talk?$AA@ (0000)
0024946C 0008:
	??_C@_07EFCDKMI@chatter?$AA@ (0000)
00249474 0007:
	??_C@_06NLCJMHIE@filler?$AA@ (0000)
0024947C 0012:
	??_C@_0BC@ONDNPAEP@conversation_data?$AA@ (0000)
00249490 0010:
	??_C@_0BA@HJGPPPBN@ai?5conversation?$AA@ (0000)
002494A0 0048:
	??_C@_0EI@HLDOMHAD@ai_communication_initialize?3?5una@ (0000)
002494E8 0019:
	??_C@_0BJ@DEJOBHIO@ai?5communication?5replies?$AA@ (0000)
00249508 004b:
	??_C@_0EL@IJCKEONL@ai_communication_initialize?3?5una@ (0000)
00249554 0025:
	??_C@_0CF@ENENAALK@c?3?2halo?2SOURCE?2ai?2ai_communicati@ (0000)
0024957C 001a:
	??_C@_0BK@HKKGFHIF@ai?5communication?5dialogue?$AA@ (0000)
00249598 000c:
	??_C@_0M@FJGFLLPA@information?$AA@ (0000)
002495A4 003a:
	??_C@_0DK@JAPPOFEL@?$CIplay_type?5?$DN?$DN?5_unit_play_speech_@ (0000)
002495E0 000f:
	??_C@_0P@LFGNPAFB@spk?$CFd?$DMtol?$CFd?$CL?$CFd?$AA@ (0000)
002495F0 000a:
	??_C@_09NBNIDDMC@nospch?9?$CFs?$AA@ (0000)
00249600 0040:
	??_C@_0EA@HGHJJMBM@vocalization_type?5?$CG?$CG?5sound_defin@ (0000)
00249640 0021:
	??_C@_0CB@JPNJDDBL@?$CFs?3?5told?5to?5advance?5by?5scripting@ (0000)
00249664 0012:
	??_C@_0BC@NMDPGBEB@?$CFs?3?5finished?5?$CFs?$CFs?$AA@ (0000)
00249678 000c:
	??_C@_0M@PDJPHGJL@prematurely?$AA@ (0000)
00249684 000d:
	??_C@_0N@HAEOLDAP@successfully?$AA@ (0000)
00249694 0013:
	??_C@_0BD@FOMCINHP@?5?$CIunable?5to?5begin?$CJ?$AA@ (0000)
002496A8 0048:
	??_C@_0EI@KMONEJAM@?$CFs?3?5this?5conversation?5is?5already@ (0000)
002496F0 00b5:
	??_C@_0LF@IOBNLMGM@?$CIconversation?9?$DOdialogue_indices?$FL@ (0000)
002497A8 000d:
	??_C@_0N@IDIKJELG@?$CFs?3?5speak?5?$CFs?$AA@ (0000)
002497B8 004b:
	??_C@_0EL@EAKMIEAH@?$CIreply_table_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIrep@ (0000)
00249808 0052:
	??_C@_0FC@DCLNMHJF@?$CIdialogue_type_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CId@ (0000)
0024985C 0013:
	??_C@_0BD@NKCKMGKJ@?$CFs?5?$CFs?5?$CFd?1?$CFs?3?5?$CFs?5?$CFd?$AA@ (0000)
00249870 0004:
	__real@3eb33333 (0000)
00249874 001e:
	??_C@_0BO@PGLKMLMB@?$CFs?3?5told?5to?5stop?5by?5scripting?$AA@ (0000)
00249894 0018:
	??_C@_0BI@MOODPKBH@?$CFs?3?5unit?5died?0?5aborting?$AA@ (0000)
002498AC 0008:
	??_C@_07CEJJIKE@?$CFs?$CI?$CFd?$CJ?5?$AA@ (0000)
002498B4 0012:
	??_C@_0BC@LGHGCJKF@no?9dialogue?9match?$AA@ (0000)
002498C8 0010:
	??_C@_0BA@LGNJEIF@not?9near?9player?$AA@ (0000)
002498D8 000a:
	??_C@_09DFNMPCAJ@selection?$AA@ (0000)
002498E4 0014:
	??_C@_0BE@FCFFGOIN@nowhere?9near?9player?$AA@ (0000)
002498F8 0013:
	??_C@_0BD@FMPJDKHJ@already?9conversing?$AA@ (0000)
0024990C 000b:
	??_C@_0L@FEFLJKBC@wrong?9type?$AA@ (0000)
00249918 000c:
	??_C@_0M@CFFMKJFH@?5?5reasons?3?5?$AA@ (0000)
00249924 0031:
	??_C@_0DB@PGEHKHFC@?$CFs?3?5didn?8t?5find?5?$CFd?1?$CFs?5in?5?$CFs?5?$CI?$CFd?5@ (0000)
00249958 000b:
	??_C@_0L@COMKDGN@?$DMeveryone?$DO?$AA@ (0000)
00249964 0010:
	??_C@_0BA@ELKHLFDD@?$DMspecific?5unit?$DO?$AA@ (0000)
00249978 006d:
	??_C@_0GN@FHAECEPL@?$CIactor_variant_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIa@ (0000)
002499E8 001c:
	??_C@_0BM@CBPILECH@found_variant_index?5?$CB?$DN?5NONE?$AA@ (0000)
00249A08 0056:
	??_C@_0FG@DGHHBBJL@change_variant_indices_count?5?$DM?5M@ (0000)
00249A60 0004:
	__real@3eaaaaab (0000)
00249A64 0004:
	__real@41a20000 (0000)
00249A68 003a:
	??_C@_0DK@CMCFHJEF@nearby_unit_count?5?$DM?5MAXIMUM_PART@ (0000)
00249AA4 0028:
	??_C@_0CI@ICHMPFMM@conversation_definition?5?$CG?$CG?5conve@ (0000)
00249ACC 0007:
	??_C@_06DJHOIPC@?$CFs?3?5?$CFs?$AA@ (0000)
00249AD4 0009:
	??_C@_08BAHEMPNM@?5?$FL?$CFd?1?$CFs?$FN?$AA@ (0000)
00249AE0 0006:
	??_C@_05DNIIFBMG@?$CFs?5?$CFs?$AA@ (0000)
00249AE8 0008:
	??_C@_07GKOCGALF@non?9voc?$AA@ (0000)
00249AF0 000c:
	??_C@_0M@DMDFMBAG@unit?5?$CF04X?3?5?$AA@ (0000)
00249AFC 000c:
	??_C@_0M@HNMGEIHE@player?5?$CFd?3?5?$AA@ (0000)
00249B08 0005:
	??_C@_04PFAOODAA@?$CFs?3?5?$AA@ (0000)
00249B10 0008:
	??_C@_07MMOCNKBD@?$CFs?1?$CFs?3?5?$AA@ (0000)
00249B18 0072:
	??_C@_0HC@PEHCJGNI@?$CIai_information?9?$DOdialogue_type_i@ (0000)
00249B90 0091:
	??_C@_0JB@OPDEEEOP@?$CIconversation?9?$DOdialogue_indices?$FL@ (0000)
00249C24 0030:
	??_C@_0DA@LIHNLMLM@?$CFs?3?5cannot?5start?0?5players?5are?5no@ (0000)
00249C54 0028:
	??_C@_0CI@LELOFLBG@?$CFs?3?5cannot?5start?0?5nobody?5can?5see@ (0000)
00249C80 0047:
	??_C@_0EH@ODDEEICK@?$CFs?3?5participants?5currently?5outsi@ (0000)
00249CC8 0025:
	??_C@_0CF@BJPALGNB@?$CFs?3?5could?5not?5find?5participant?5?$CF@ (0000)
00249CF0 0036:
	??_C@_0DG@BNDGFHIE@?$CFs?3?5found?5participant?5?$CFd?1?$CFs?5but?5@ (0000)
00249D28 0016:
	??_C@_0BG@KFPPJIJM@?$FL?$CFs?50?9player?9chance?$FN?5?$AA@ (0000)
00249D40 0014:
	??_C@_0BE@DLAFFNAP@?$FL?$CFs?5rand?$CF?42f?$DO?$CF?42f?$FN?5?$AA@ (0000)
00249D54 0012:
	??_C@_0BC@GGBDH@?$FL?$CFs?5found?9actor?$FN?5?$AA@ (0000)
00249D68 0012:
	??_C@_0BC@GOGGHGJI@?$FL?$CFs?5?$CFs?9d?9dis?1?$CFd?$FN?5?$AA@ (0000)
00249D7C 000d:
	??_C@_0N@EFEDAFNN@?$FL?$CFs?5nobody?$FN?5?$AA@ (0000)
00249D8C 0015:
	??_C@_0BF@DPPMFKIF@?$FLscripted?9override?$FN?5?$AA@ (0000)
00249DA4 0011:
	??_C@_0BB@HDBKEACA@PLAYER?9REPLY?5?$CFs?5?$AA@ (0000)
00249DB8 0007:
	??_C@_06IKJGFCFO@?$DO?$DO?$CFs?$DM?$DM?$AA@ (0000)
00249DC0 0008:
	??_C@_07GPMPHPH@nobody?5?$AA@ (0000)
00249DC8 000c:
	??_C@_0M@EMMAIOJB@playercant?5?$AA@ (0000)
00249DD4 0009:
	??_C@_08LMAMDFAK@u?9?$CFs?9?$CFs?5?$AA@ (0000)
00249DE0 0004:
	??_C@_03EKLFPLOI@n?1a?$AA@ (0000)
00249DE4 0004:
	??_C@_03CJOLJKCN@dis?$AA@ (0000)
00249DE8 0008:
	??_C@_07CPBKJGBP@filter?5?$AA@ (0000)
00249DF0 000a:
	??_C@_09PGKELFMI@0?9chance?5?$AA@ (0000)
00249DFC 000d:
	??_C@_0N@KOBKEGMJ@rand?9failed?5?$AA@ (0000)
00249E0C 0010:
	??_C@_0BA@MDFGCECJ@rand?5?$CF?42f?$DO?$CF?42f?5?$AA@ (0000)
00249E1C 0011:
	??_C@_0BB@BILJOOCN@?$CBreply_to_player?$AA@ (0000)
00249E30 0013:
	??_C@_0BD@PDIGFKID@override?9scripted?5?$AA@ (0000)
00249E44 000b:
	??_C@_0L@HPKHMDON@wrong?9dmg?5?$AA@ (0000)
00249E50 0004:
	??_C@_03DKEJHHJP@?$CFs?3?$AA@ (0000)
00249E54 000b:
	??_C@_0L@LNGLDCHG@REPLY?5?$CFs?3?5?$AA@ (0000)
00249E60 0047:
	??_C@_0EH@NNNDPBEK@?$CFs?3?5could?5not?5start?0?5and?5not?5set@ (0000)
00249EA8 0039:
	??_C@_0DJ@ONAPPANJ@?$CFs?3?5can?8t?5begin?5yet?5but?5will?5rem@ (0000)
00249EE4 0017:
	??_C@_0BH@MJIAPKFJ@?$CFs?3?5begun?5successfully?$AA@ (0000)
00249F00 0058:
	??_C@_0FI@CHGNAOEL@WARNING?3?5too?5many?5executing?5conv@ (0000)
00249F58 0027:
	??_C@_0CH@FMECNBHL@?$CFs?3?5script?5tried?5to?5start?5conver@ (0000)
00249F80 001a:
	??_C@_0BK@ELJOEOCB@?$CFs?3?5no?5more?5lines?5to?5play?$AA@ (0000)
00249F9C 0030:
	??_C@_0DA@PPIFADMI@?$CFs?3?5unable?5to?5begin?0?5and?5no?5poin@ (0000)
00249FCC 0014:
	??_C@_0BE@KPFKNBML@?$CFs?3?5trying?5to?5begin?$AA@ (0000)
00249FE0 003c:
	??_C@_0DM@GFNDBPMM@selected_possibility?9?$DOpreselecte@ (0000)
0024A01C 001e:
	??_C@_0BO@LJOGJGHI@?$FLrnd?$CF?41f?5tot?$CF?41f?5cum?$CF?41f?$EA?$CFd?$FN?5?$AA@ (0000)
0024A03C 0019:
	??_C@_0BJ@DMOBOMPI@?$FL?$CFd?1?$CF?41f?5force?5?$CFd?1?$CF?41f?$FN?5?$AA@ (0000)
0024A058 0020:
	??_C@_0CA@DKHGIOGG@total_possibility_weight?5?$DO?50?40f?$AA@ (0000)
0024A078 0059:
	??_C@_0FJ@IBKDMIEO@ai_communication_event?3?5type?5?$CFd?5@ (0000)
0024A0D8 0043:
	??_C@_0ED@GJAAIMME@?$FL?$CFs?1?$CFd?5?$CFs?5del?$CFd?5w?3?$CF?41f?$CFs?5s?$CF?41f?5p@ (0000)
0024A11C 0002:
	??_C@_01BIAFAFID@F?$AA@ (0000)
0024A120 0007:
	??_C@_06BEJGECAN@PLAYER?$AA@ (0000)
0024A128 0012:
	??_C@_0BC@FOAHPEHM@protagonist_actor?$AA@ (0000)
0024A13C 0011:
	??_C@_0BB@BOBCLBAE@?$FL?$CFs?1?$CFd?5u?9?$CFs?9?$CFs?$FN?5?$AA@ (0000)
0024A150 0030:
	??_C@_0DA@CPDLIKLO@subject_actor?9?$DOdanger_zone?4objec@ (0000)
0024A180 0015:
	??_C@_0BF@JKGAIKPI@?$FL?$CFs?1?$CFd?5?$CFs?9d?9dis?1?$CFd?$FN?5?$AA@ (0000)
0024A198 000f:
	??_C@_0P@BLNKGMGL@?$FL?$CFs?1?$CFd?5?$CFs?5?$CFs?$FN?5?$AA@ (0000)
0024A1A8 0010:
	??_C@_0BA@JGHKCAMO@shout?3?$CF?41f?$DM?$CF?41f?$AA@ (0000)
0024A1B8 000f:
	??_C@_0P@NMKONCAD@talk?3?$CF?41f?$DM?$CF?41f?$AA@ (0000)
0024A1C8 000f:
	??_C@_0P@JEDLKPOF@chat?3?$CF?41f?$DM?$CF?41f?$AA@ (0000)
0024A1D8 0039:
	??_C@_0DJ@KAPPHHBA@communication_priority?5?$DM?5_ai_com@ (0000)
0024A214 000c:
	??_C@_0M@EGBKHEDG@?$CFs?9?$CFc?$CFc?$CFc?$CFs?$AA@ (0000)
0024A220 0004:
	??_C@_03HOBIBNHG@?9hi?$AA@ (0000)
0024A224 0004:
	??_C@_03CPELBCCM@?9lo?$AA@ (0000)
0024A228 007f:
	??_C@_0HP@PLIGCELE@?$CIcommunication_priority?5?$DO?5_ai_co@ (0000)
0024A2A8 0055:
	??_C@_0FF@NKMCJBLB@?$CIcommunication_team?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIco@ (0000)
0024A300 0009:
	??_C@_08ECOGDNI@unteamed?$AA@ (0000)
0024A30C 0013:
	??_C@_0BD@NMMLILLH@?$FL?$CFs?1?$CFd?50?9playrat?$FN?5?$AA@ (0000)
0024A320 0012:
	??_C@_0BC@OMHOAJOH@?$FL?$CFs?1?$CFd?5nogrp?9?$CFc?$FN?5?$AA@ (0000)
0024A334 0014:
	??_C@_0BE@OMABELMA@?$FL?$CFs?1?$CFd?5noplyreply?$FN?5?$AA@ (0000)
0024A348 0013:
	??_C@_0BD@GACNBALB@?$FL?$CFs?1?$CFd?5nounit?9?$CFc?$FN?5?$AA@ (0000)
0024A35C 0006:
	??_C@_05LLNBKOOD@?$DMerr?$DO?$AA@ (0000)
0024A364 000f:
	??_C@_0P@OMGCNPFA@?$FL?$CFs?1?$CFd?5nodmg?$FN?5?$AA@ (0000)
0024A374 0014:
	??_C@_0BE@PGBJJNLI@?$FL?$CFs?1?$CFd?5nocausrace?$FN?5?$AA@ (0000)
0024A388 0013:
	??_C@_0BD@PHFOKNBE@?$FL?$CFs?1?$CFd?5nosubrace?$FN?5?$AA@ (0000)
0024A39C 0013:
	??_C@_0BD@NGEMBLCE@?$FL?$CFs?1?$CFd?5status?9?$CFs?$FN?5?$AA@ (0000)
0024A3B0 0004:
	??_C@_03NDPLAFDJ@vis?$AA@ (0000)
0024A3B4 0009:
	??_C@_08OLCLKDFA@nodanger?$AA@ (0000)
0024A3C0 0007:
	??_C@_06EBKKMJGD@notvis?$AA@ (0000)
0024A3C8 0005:
	??_C@_04BLLOBPAA@lost?$AA@ (0000)
0024A3D0 0005:
	??_C@_04BBBDNBOK@dead?$AA@ (0000)
0024A3D8 0011:
	??_C@_0BB@GKMIFEBC@?$FL?$CFs?1?$CFd?5host?9?$CFs?$FN?5?$AA@ (0000)
0024A3EC 0008:
	??_C@_07CEGFBHJP@traitor?$AA@ (0000)
0024A3F4 0006:
	??_C@_05ICAMJEGE@enemy?$AA@ (0000)
0024A3FC 0007:
	??_C@_06BHBNKPBA@friend?$AA@ (0000)
0024A404 0005:
	??_C@_04PJDNGLKJ@self?$AA@ (0000)
0024A410 0048:
	??_C@_0EI@GINMJEIA@?$CIdialogue_index?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIdialog@ (0000)
0024A458 0009:
	??_C@_08GLNGGDPJ@DISABLED?$AA@ (0000)
0024A464 0007:
	??_C@_06DAIKMLH@?$CFs?9?$CFc?5?$AA@ (0000)
0024A46C 002b:
	??_C@_0CL@IOMHDOAM@allegiance?5?$CFs?0?5?$CFd?5incidents?5?$CIthr@ (0000)
0024A498 000c:
	??_C@_0M@IDKAPGFO@still?5holds?$AA@ (0000)
0024A4A4 0029:
	??_C@_0CJ@GKFDOEJ@incident?5between?5teams?5?$CFs?5and?5?$CFs@ (0000)
0024A4D0 0009:
	??_C@_08PAHPFNGA@accident?$AA@ (0000)
0024A4DC 0009:
	??_C@_08JPJNIAAJ@betrayal?$AA@ (0000)
0024A4E8 000b:
	??_C@_0L@MCEFNCAK@unobserved?$AA@ (0000)
0024A4F4 0009:
	??_C@_08PJALNDK@observed?$AA@ (0000)
0024A500 0055:
	??_C@_0FF@FCIFJAOH@?$CIcommunication_type?5?$DO?$DN?50?$CJ?5?$CG?$CG?5?$CIco@ (0000)
002B7308 0114:
	_global_communication_priority_names (0000)
	_global_communication_team_names (0020)
	_global_communication_type_names (0030)
00319CA8 0010:
	_global_dialogue_event_count (0000)
	_global_dialogue_events (0004)
	_global_reply_event_count (0008)
	_global_reply_events (000c)
*/

/* ---------- headers */

#include "cseries.h"
#include "ai_communication.h"

#include "ai.h"
#include "ai_debug.h"
#include "ai_profile.h"
#include "ai_script.h"
#include "actions.h"
#include "action_converse.h"
#include "actor_definitions.h"
#include "actor_perception.h"
#include "actors.h"
#include "actor_types.h"
#include "ai_scenario_definitions.h"
#include "cseries/errors.h"
#include "encounters.h"
#include "game/game.h"
#include "game/game_allegiance.h"
#include "game/players.h"
#include "main/console.h"
#include "memory/data.h"
#include "physics/collision_usage.h"
#include "physics/collisions.h"
#include "props.h"
#include "saved games/game_state.h"
#include "scenario/scenario.h"
#include "scenario/scenario_definitions.h"
#include "sound/game_sound.h"
#include "tag_files/tag_files.h"
#include "units/dialogue_definitions.h"
#include "units/units.h"

/* ---------- constants */

enum
{
	NUMBER_OF_COMMUNICATION_PRIORITIES = 8,
	NUMBER_OF_AI_COMMUNICATION_PRIORITIES = NUMBER_OF_COMMUNICATION_PRIORITIES,
	MAXIMUM_COMMUNICATION_POSSIBILITIES = 16,
	NUMBER_OF_COMMUNICATION_TEAMS = 4,
	NUMBER_OF_COMMUNICATION_TYPES = 57,
	NUMBER_OF_COMMUNICATION_TIMER_TYPES = 5,
	NUMBER_OF_DIALOGUE_USAGES = 105,
	NUMBER_OF_REPLY_USAGES = 46,
	MAXIMUM_PARTICIPANTS_PER_CONVERSATION = 8,
	MAXIMUM_DIALOGUE_VARIANTS_PER_CONVERSATION_PARTICIPANT = 6,
	MAXIMUM_RECENT_CONVERSATIONS = 16,
	_ai_conversation_stop_if_anyone_dies_bit = 0,
	_ai_conversation_stop_if_damaged_bit = 1,
	_ai_conversation_stop_if_visible_enemy_bit = 2,
	_ai_conversation_stop_if_alerted_to_enemy_bit = 3,
	_ai_conversation_player_must_be_visible_bit = 4,
	_ai_conversation_stop_other_actions_bit = 5,
	_ai_conversation_keep_trying_to_play_bit = 6,
	_ai_conversation_player_must_be_looking_at_bit = 7,
	_ai_conversation_participant_optional_bit = 0,
	_ai_conversation_participant_has_alternate_bit = 1,
	_ai_conversation_participant_is_alternate_bit = 2,
	_ai_conversation_line_addressee_look_back_bit = 0,
	_ai_conversation_line_everyone_look_at_speaker_bit = 1,
	_ai_conversation_line_everyone_look_at_addressee_bit = 2,
	_ai_conversation_line_wait_after_until_told_to_advance_bit = 3,
	_ai_conversation_line_wait_until_speaker_nearby_bit = 4,
	_ai_conversation_line_wait_until_everyone_nearby_bit = 5,
	_ai_conversation_address_player = 1,
	_ai_conversation_address_participant = 2,
	_ai_conversation_selection_friendly_actor = 0,
	_ai_conversation_selection_disembodied = 1,
	_ai_conversation_selection_in_player_vehicle = 2,
	_ai_conversation_selection_not_in_vehicle = 3,
	_ai_conversation_selection_sargeant = 4,
	_ai_conversation_selection_any_actor = 5,
	_ai_conversation_selection_radio = 6,
	_ai_conversation_selection_radio_sargeant = 7,
	_find_actor_mode_same_team = 0,
	_find_actor_mode_friend = 1,
	_find_actor_mode_enemy = 2,
	_find_actor_allow_lookup_bit = 0,
	_find_actor_near_to_players_bit = 1,
	_find_actor_same_vehicle_bit = 2,
	_find_actor_allow_subject_bit = 3,
	_find_actor_allow_cause_bit = 4,
	_actor_mode_braindead = 0,
	_actor_mode_asleep = 1,
	_actor_mode_alert = 2,
	_actor_mode_combat = 3,
	_actor_combat_status_definite = 3,
	_actor_combat_status_certain = 4,
	_actor_combat_status_dangerous = 6,
	_ai_information_none = 0,
	_ai_information_look_unit = 1,
	_ai_information_look_object = 2,
	_ai_information_allegiance = 1,
	_dialogue_usage_lookup_bit = 0,
	_dialogue_usage_force_bit = 1,
	_dialogue_usage_immediate_notify_bit = 2,
	_dialogue_usage_player_bit = 3,
	_dialogue_usage_same_vehicle_bit = 4,
	_dialogue_usage_allow_subject_bit = 5,
	_dialogue_usage_override_scripted_bit = 6,
	_comm_look_direction_subject = 1,
	_comm_look_direction_protagonist = 2,
	_comm_look_direction_target = 3,
	_comm_look_direction_danger = 4,
	_comm_protagonist_subject = 0,
	_comm_protagonist_cause = 1,
	_comm_protagonist_friend = 2,
	_comm_protagonist_target = 3,
	_comm_protagonist_enemy = 4,
	_ai_communication_priority_none = 0,
	_ai_communication_priority_communicate = 4,
	_ai_communication_priority_shout = 5,
	_ai_communication_priority_yell = 6,
	_ai_communication_priority_exclaim = 7,
	_ai_sound_volume_medium = 1,
	_ai_sound_volume_loud = 2,
	_ai_sound_volume_shout = 3,
	_ai_communication_team_human = 0,
	_ai_communication_team_covenant = 1,
	NUMBER_OF_AI_COMMUNICATION_TEAMS = 2,
	_unit_play_speech_none = 0,
	_unit_play_speech_queue = 1,
};

#define COMMUNICATION_CLOSE_DISTANCE 5.0f

/* ---------- macros */

#define ai_print_conversations (ai_debug.print_conversations)
#define ai_conversation_driver_get(index) \
	((struct ai_conversation_driver_datum_view *)datum_get(conversation_data, (index)))

/* ---------- structures */

struct ai_information_packet;

struct dialogue_usage
{
	short communication_type;
	short communication_priority;
	short vocalization_type;
	short animation_type;
	short protagonist_type;
	short protagonist_look_priority;
	short recipient_look_direction;
	short recipient_look_priority;
	real weight;
	real repeat_delay;
	short flags;
	short required_group;
	short required_hostility;
	short required_enemy_status;
	short required_subject_race;
	short required_cause_race;
	short required_damage;
};

struct reply_usage
{
	short original_vocalization_type;
	short original_damage_category;
	short protagonist_type;
	short vocalization_type;
	short animation_type;
	short communication_priority;
	word flags;
	real chance;
	real player_chance;
	real delay_time;
	real repeat_delay;
	boolean (*reply_filter)(
		long original_unit_index,
		struct ai_information_packet *communication,
		long reply_actor_index);
};

typedef char dialogue_usage_size_assert[
	sizeof(struct dialogue_usage) == 0x28 ? 1 : -1];
typedef char reply_usage_size_assert[
	sizeof(struct reply_usage) == 0x24 ? 1 : -1];

struct ai_conversation_line_view
{
	struct ai_conversation_datum_header header;
	byte __unknown14[0x34];
	short current_line;
};

typedef char ai_conversation_line_view_current_line_offset_assert[
	offsetof(struct ai_conversation_line_view, current_line) == 0x48 ? 1 : -1];

struct ai_conversation_driver_datum_view
{
	struct ai_conversation_datum_header header;
	unsigned long participant_bitmask;
	short dialogue_indices[MAXIMUM_PARTICIPANTS_PER_CONVERSATION];
	long actor_indices[MAXIMUM_PARTICIPANTS_PER_CONVERSATION];
	short current_line;
	short line_participant_index;
	short line_delay_timer;
	word line_flags;
	long line_actor_index;
	long line_unit_index;
	long line_address_unit_index;
	long line_sound_index;
	boolean line_unspatialized;
	boolean line_spoken;
	boolean line_finished;
	boolean line_advance;
};

struct ai_conversation_datum_view
{
	short identifier;
	short scenario_conversation_index;
	boolean scripted;
	boolean any_line_spoken;
	boolean begun;
	boolean finished;
	boolean waiting_to_advance;
	boolean told_to_advance;
	byte reserved0A[2];
	long creation_time;
	long triggering_player_unit_index;
	unsigned long participant_bitmask;
	short dialogue_indices[MAXIMUM_PARTICIPANTS_PER_CONVERSATION];
	long actor_indices[MAXIMUM_PARTICIPANTS_PER_CONVERSATION];
	short line_index;
	short line_participant_index;
	short line_delay_timer;
	word line_flags;
	long line_actor_index;
	long line_unit_index;
	long line_address_unit_index;
	long line_sound_index;
	boolean line_unspatialized;
	boolean line_spoken;
	boolean line_finished;
	boolean line_advance;
};

struct scenario_conversation_definition_view
{
	char name[32];
	word flags;
	word pad22;
	real trigger_distance;
	real run_to_player_distance;
	byte __unknown2C[0x24];
	struct tag_block participants;
	struct tag_block lines;
	struct tag_block unused;
};

struct scenario_conversation_participant_view
{
	word pad00;
	word flags;
	short selection_type;
	short actor_type;
	short preexisting_object_name_index;
	short new_attach_object_name_index;
	byte __unknown0C[0x0C];
	short dialogue_variants[MAXIMUM_DIALOGUE_VARIANTS_PER_CONVERSATION_PARTICIPANT];
	char ai_index_name[32];
	long runtime_ai_index;
	byte __unknown48[0x0C];
};

struct scenario_conversation_line_view
{
	word flags;
	short participant_index;
	short address_type;
	short address_participant_index;
	long unknown08;
	real delay_time;
	byte __unknown10[0x0C];
	struct tag_reference dialogue[MAXIMUM_DIALOGUE_VARIANTS_PER_CONVERSATION_PARTICIPANT];
};

struct recent_conversation_view
{
	short scenario_conversation_index;
	boolean unable_to_begin;
	boolean finished_successfully;
	long finish_time;
	byte __unknown08[8];
};

struct dialogue_event_status
{
	long last_time_spoken;
	long disable_until_time;
};

/* Function-local in the original; its construction code establishes this
 * 0x38-byte layout.  January's assertion strings name the selected record
 * `selected_possibility` and its reply field `preselected_reply_actor_index`. */
struct ai_communication_possibility
{
	real weight;
	boolean interrupts;
	boolean is_reply;
	short vocalization_type;
	short priority;
	short animation_type;
	short play_type;
	short delay_time;
	short ai_delay_time;
	word pad12;
	long protagonist_unit_index;
	long protagonist_actor_index;
	long recipient_unit_index;
	long preselected_reply_actor_index;
	short protagonist_look_priority;
	short recipient_look_priority;
	short look_type;
	word pad2A;
	long look_unit_index;
	long sound_definition_index;
	short dialogue_type_index;
	word pad36;
};

typedef char ai_communication_possibility_size_assert[
	sizeof(struct ai_communication_possibility) == 0x38 ? 1 : -1];

struct ai_communication_globals_view
{
	byte __unknown00[0x10];
	boolean dialogue_triggers_enabled;
	byte __unknown11[3];
	long last_chatter_time[2];
	long last_talk_time[2];
	long last_shout_time[2];
	short recent_conversation_count;
	short recent_conversation_next_index;
	struct recent_conversation_view
		recent_conversations[MAXIMUM_RECENT_CONVERSATIONS];
};

struct actor_iterator
{
	struct data_iterator encounter_iterator;
	boolean iterated_encounterless_list;
	boolean active_only;
	byte pad[2];
	long index;
	long next_index;
};

typedef char ai_conversation_datum_header_size_assert[
	sizeof(struct ai_conversation_datum_header) == 0x14 ? 1 : -1];
typedef char ai_conversation_datum_view_size_assert[
	sizeof(struct ai_conversation_datum_view) == 0x64 ? 1 : -1];
typedef char ai_conversation_datum_view_line_index_offset_assert[
	offsetof(struct ai_conversation_datum_view, line_index) == 0x48 ? 1 : -1];
typedef char ai_conversation_datum_view_line_advance_offset_assert[
	offsetof(struct ai_conversation_datum_view, line_advance) == 0x63 ? 1 : -1];
typedef char ai_conversation_datum_header_any_line_spoken_offset_assert[
	offsetof(struct ai_conversation_datum_header, any_line_spoken) == 0x5 ? 1 : -1];
typedef char ai_conversation_datum_header_begun_offset_assert[
	offsetof(struct ai_conversation_datum_header, begun) == 0x6 ? 1 : -1];
typedef char ai_conversation_datum_header_waiting_to_advance_offset_assert[
	offsetof(struct ai_conversation_datum_header, waiting_to_advance) == 0x8 ? 1 : -1];
typedef char ai_conversation_datum_header_told_to_advance_offset_assert[
	offsetof(struct ai_conversation_datum_header, told_to_advance) == 0x9 ? 1 : -1];
typedef char scenario_conversation_definition_view_size_assert[
	sizeof(struct scenario_conversation_definition_view) == 0x74 ? 1 : -1];
typedef char scenario_conversation_definition_participants_offset_assert[
	offsetof(struct scenario_conversation_definition_view, participants) == 0x50 ? 1 : -1];
typedef char scenario_conversation_definition_lines_offset_assert[
	offsetof(struct scenario_conversation_definition_view, lines) == 0x5C ? 1 : -1];
typedef char scenario_conversation_participant_view_size_assert[
	sizeof(struct scenario_conversation_participant_view) == 0x54 ? 1 : -1];
typedef char scenario_conversation_line_view_size_assert[
	sizeof(struct scenario_conversation_line_view) == 0x7C ? 1 : -1];
typedef char ai_conversation_driver_datum_view_size_assert[
	sizeof(struct ai_conversation_driver_datum_view) == 0x64 ? 1 : -1];
typedef char ai_conversation_driver_participant_bitmask_offset_assert[
	offsetof(struct ai_conversation_driver_datum_view, participant_bitmask) == 0x14 ? 1 : -1];
typedef char ai_conversation_driver_actor_indices_offset_assert[
	offsetof(struct ai_conversation_driver_datum_view, actor_indices) == 0x28 ? 1 : -1];
typedef char ai_conversation_driver_line_flags_offset_assert[
	offsetof(struct ai_conversation_driver_datum_view, line_flags) == 0x4E ? 1 : -1];
typedef char ai_conversation_driver_line_unit_index_offset_assert[
	offsetof(struct ai_conversation_driver_datum_view, line_unit_index) == 0x54 ? 1 : -1];
typedef char ai_conversation_driver_line_address_unit_index_offset_assert[
	offsetof(struct ai_conversation_driver_datum_view, line_address_unit_index) == 0x58 ? 1 : -1];
typedef char recent_conversation_view_size_assert[
	sizeof(struct recent_conversation_view) == 0x10 ? 1 : -1];
typedef char ai_print_conversations_offset_assert[
	offsetof(struct ai_debug_state, print_conversations) == 0x9F ? 1 : -1];
typedef char ai_communication_unit_speech_item_size_assert[
	sizeof(struct unit_speech_item) == 0x30 ? 1 : -1];
typedef char ai_communication_unit_speech_item_ai_offset_assert[
	offsetof(struct unit_speech_item, ai) == 0x10 ? 1 : -1];
typedef char ai_communication_actor_mode_offset_assert[
	offsetof(struct actor_datum, state.mode) == 0x6A ? 1 : -1];
typedef char ai_communication_actor_unit_index_offset_assert[
	offsetof(struct actor_datum, meta.unit_index) == 0x18 ? 1 : -1];
typedef char ai_communication_actor_idle_combat_offset_assert[
	offsetof(struct actor_datum, control.idle_vocalization_combat) == 0x6CC ? 1 : -1];
typedef char ai_communication_actor_idle_timer_offset_assert[
	offsetof(struct actor_datum, control.idle_vocalization_timer) == 0x6CE ? 1 : -1];
typedef char ai_communication_actor_iterator_size_assert[
	sizeof(struct actor_iterator) == 0x1C ? 1 : -1];
typedef char ai_communication_actor_iterator_index_offset_assert[
	offsetof(struct actor_iterator, index) == 0x14 ? 1 : -1];

/* ---------- prototypes */

static boolean reply_filter_close(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index);
static boolean reply_filter_not_close(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index);
static boolean reply_filter_searching(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index);
static boolean reply_filter_same_platoon(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index);
static boolean reply_filter_fighting(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index);
static boolean reply_filter_fighting_close(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index);
static boolean reply_filter_same_target(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index);
static boolean reply_filter_no_certain_target(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index);
static boolean reply_filter_flee_leader(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index);
static short ai_communication_consider_speech(
	long unit_index,
	short communication_priority,
	short speech_priority,
	short delay_ticks,
	boolean allow_vocalization_lookup,
	boolean allow_recent_disabling,
	short *vocalization_type,
	real *weight,
	long *sound_definition_index_reference,
	char *debug_string);
static real ai_communication_actor_talk_weight(
	long actor_index,
	long subject_unit_index,
	real_point3d const *subject_point,
	long cause_unit_index,
	real_point3d const *cause_point,
	real stimulus_range,
	short communication_type,
	short communication_priority,
	short speech_priority,
	short vocalization_type,
	short animation_type,
	short flags);
static long ai_communication_find_specific_actor_to_talk(
	long ai_index,
	long subject_unit_index,
	long cause_unit_index,
	real maximum_distance,
	short communication_type,
	short communication_priority,
	short speech_priority,
	short vocalization_type,
	short animation_type,
	short flags);
static long ai_communication_find_global_actor_to_talk(
	short team_index,
	short find_actor_mode,
	long subject_unit_index,
	long cause_unit_index,
	real maximum_distance,
	short communication_type,
	short communication_priority,
	short speech_priority,
	short vocalization_type,
	short animation_type,
	short flags);
static void ai_communication_look_secondary_at_unit(
	long actor_index,
	short type,
	short priority,
	long look_unit_index,
	long prop_index);
static void ai_communication_look_secondary_at_object(
	long actor_index,
	short type,
	short priority,
	long object_index);
static boolean ai_conversation_line_begin(
	long conversation_index);
static boolean ai_conversation_line_perform(
	long conversation_index);
static void ai_conversation_line_end(
	long conversation_index);
boolean actor_is_fighting(
	long actor_index);
static void actor_reset_idle_vocalization_timer(
	long actor_index);
static void ai_communication_handle_received_looking(
	long actor_index,
	long prop_index,
	struct ai_information_packet *information);
static void ai_communication_update_speech_timers(
	long unit_index,
	short priority,
	short vocalization_type,
	short dialogue_type_index,
	short reply_table_index);
short unit_test_speech(
	long unit_index,
	short priority,
	boolean allow_recursive_lookup,
	boolean allow_queue,
	long *unit_last_speech_time,
	short *vocalization_type_reference,
	long *sound_definition_index_reference);
void unit_speak(
	long unit_index,
	short play_type,
	struct unit_speech_item const *speech_item);
boolean sound_scripted_dialog_is_playing(
	void);
void ai_handle_allegiance_broken_notification(
	short team1_index,
	short team2_index,
	boolean broken);
void actor_iterator_new(
	struct actor_iterator *iterator,
	boolean active_only);
struct actor_datum *actor_iterator_next(
	struct actor_iterator *iterator);
static boolean ai_conversation_find_participant(
	long conversation_index,
	short participant_index,
	boolean *found_specific_unit_reference,
	boolean *try_alternate_reference,
	boolean *success_with_better_player_rating_reference,
	real *best_distance_reference);
static boolean ai_conversation_begin(
	long conversation_index,
	boolean *continue_trying);
static long ai_communication_find_actor_to_reply_to_player(
	long unit_index,
	long target_unit_index,
	short vocalization_type,
	short damage_category,
	real *reply_rating_reference);
void actor_perception_find_sense_position(
	long actor_index,
	real_point3d const *position,
	long prop_index,
	struct actor_position_data *sense_position);
short actor_audibility_at_point(
	long actor_index,
	struct actor_position_data const *position,
	real_point3d const *source_position,
	struct location const *source_location,
	short source_type,
	real scale,
	short line_of_sight);
void actor_handle_communication(
	long actor_index,
	long prop_index,
	struct ai_information_packet *information);

extern short global_communication_table_indices[NUMBER_OF_COMMUNICATION_TYPES];
extern struct ai_communication_globals_view *ai_globals;

/* ---------- globals */

short const communication_speech_priorities[NUMBER_OF_COMMUNICATION_PRIORITIES] =
{
	0, 3, 3, 3, 4, 5, 5, 8,
};

real const communication_notification_delays[NUMBER_OF_COMMUNICATION_PRIORITIES] =
{
	0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.3f,
};

short const communication_protagonist_default_look_priorities
	[NUMBER_OF_COMMUNICATION_PRIORITIES] =
{
	0, 4, 4, 5, 6, 6, 6, 4,
};

short const communication_recipient_default_look_priorities
	[NUMBER_OF_COMMUNICATION_PRIORITIES] =
{
	0, 3, 3, 4, 5, 5, 6, 4,
};

short const communication_player_speaking_priorities
	[NUMBER_OF_COMMUNICATION_PRIORITIES] =
{
	0, 2, 3, 4, 4, 6, 6, 7,
};

short const communication_unit_prefer_silent_time = 60;

real const communication_timer_tolerances
	[NUMBER_OF_COMMUNICATION_PRIORITIES][2][NUMBER_OF_COMMUNICATION_TIMER_TYPES] =
{
	{
		{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	},
	{
		{ 4.0f, 4.5f, 2.5f, 2.0f, 0.0f },
		{ 5.0f, 5.0f, 3.0f, 2.5f, 0.0f },
	},
	{
		{ 3.0f, 3.5f, 2.0f, 1.5f, 0.0f },
		{ 4.5f, 4.5f, 2.5f, 2.0f, 0.0f },
	},
	{
		{ 0.3f, 1.0f, 1.0f, 1.0f, 0.8f },
		{ 1.0f, 2.0f, 1.5f, 1.5f, 0.3f },
	},
	{
		{ 0.0f, 0.5f, 0.5f, 0.5f, 1.3f },
		{ 0.2f, 1.0f, 1.0f, 1.0f, 0.8f },
	},
	{
		{ 0.0f, 0.0f, 1.0f, 1.0f, 1.5f },
		{ 0.0f, 0.0f, 1.5f, 1.5f, 1.0f },
	},
	{
		{ 0.0f, 0.0f, 0.5f, 0.0f, 1.5f },
		{ 0.0f, 0.0f, 0.5f, 0.0f, 1.5f },
	},
	{
		{ 0.0f, 0.0f, 0.5f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.5f, 0.0f, 0.0f },
	},
};

real const communication_play_delays[NUMBER_OF_COMMUNICATION_TIMER_TYPES] =
{
	0.0f, 0.5f, 0.8f, 0.5f, 0.8f,
};

short const communication_player_additional_delay = 30;
short const communication_overlap_time_modifier = 45;
short const communication_timeout_low_priority_modifier = 30;
short const communication_repeat_selection_time = 900;
real const communication_player_absolute_range = 30.0f;
real const communication_player_ideal_range_min = 3.0f;
real const communication_player_ideal_range_max = 15.0f;
real const communication_player_ideal_fov = 0.70710677f;
real const communication_player_rating_low_priority = 2.0f;

struct dialogue_usage const global_dialogue_table[NUMBER_OF_DIALOGUE_USAGES] =
{
	/* death */
	{ 0, 3, 49, -1, 1, 6, 2, 1, 10.0f, 0.0f,
		0, -1, 2, -1, -1, -1, -1 },
	{ 0, 6, 51, -1, 1, 6, 3, 6, 20.0f, 0.0f,
		66, -1, 2, -1, 1, -1, -1 },
	{ 0, 2, 53, -1, 1, 1, 2, 1, 10.0f, 0.0f,
		8, -1, 3, -1, -1, -1, -1 },
	{ 0, 6, 55, -1, 1, 6, 3, 6, 20.0f, 0.0f,
		74, -1, 3, -1, 1, -1, -1 },
	{ 0, 2, 57, -1, 1, 1, 2, 1, 10.0f, 0.0f,
		8, -1, 3, -1, 4, -1, -1 },
	{ 0, 2, 59, -1, 1, 1, 2, 1, 40.0f, 0.0f,
		8, -1, 3, -1, 8, -1, -1 },
	{ 0, 2, 61, -1, 1, 1, 2, 1, 40.0f, 0.0f,
		8, -1, 3, -1, 16, -1, -1 },
	{ 0, 2, 63, -1, 1, 1, 2, 1, 40.0f, 0.0f,
		8, -1, 3, -1, 64, -1, -1 },
	{ 0, 2, 65, -1, 1, 1, 2, 1, 10.0f, 10.0f,
		0, -1, 3, -1, -1, -1, 2 },
	{ 0, 2, 67, -1, 1, 1, 2, 1, 10.0f, 10.0f,
		0, -1, 3, -1, -1, -1, 10 },
	{ 0, 2, 66, -1, 1, 1, 2, 1, 10.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 11 },
	{ 0, 3, 68, -1, 1, 1, 2, 1, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 5 },
	{ 0, 3, 69, -1, 1, 1, 2, 1, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 3 },
	{ 0, 3, 70, -1, 1, 1, 2, 1, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 4 },
	{ 0, 3, 71, -1, 1, 1, 2, 1, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 6 },
	{ 0, 2, 72, -1, 1, 1, 2, 1, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 7 },
	{ 0, 2, 73, -1, 1, 1, 2, 1, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 12 },
	{ 0, 3, 74, -1, 1, 1, 2, 1, 30.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 9 },
	{ 0, 2, 75, -1, 1, 1, 2, 1, 30.0f, 10.0f,
		0, -1, 3, -1, -1, -1, 8 },
	{ 0, 2, 96, -1, 2, 1, 3, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	{ 0, 6, 97, -1, 2, 6, 3, 6, 15.0f, 0.0f,
		66, -1, -1, -1, 1, -1, -1 },
	{ 0, 3, 98, -1, 2, 5, 1, 1, 10.0f, 0.0f,
		0, -1, 2, -1, -1, -1, -1 },
	{ 0, 4, 99, -1, 2, 6, 3, 5, 20.0f, 0.0f,
		66, -1, 2, -1, -1, 1, -1 },
	{ 0, 2, 100, -1, 2, 1, 3, 1, 10.0f, 0.0f,
		0, -1, 3, -1, -1, -1, -1 },
	{ 0, 2, 101, -1, 2, 1, 3, 1, 10.0f, 0.0f,
		0, -1, 3, -1, -1, 1, -1 },
	{ 0, 2, 102, -1, 2, 1, 3, 1, 10.0f, 0.0f,
		0, -1, 3, -1, -1, 4, -1 },
	{ 0, 2, 103, -1, 2, 1, 3, 1, 40.0f, 0.0f,
		0, -1, 3, -1, -1, 56, -1 },
	{ 0, 2, 104, -1, 2, 1, 3, 1, 40.0f, 0.0f,
		0, -1, 3, -1, -1, 64, -1 },
	{ 0, 5, 105, -1, 2, 6, 3, 6, 30.0f, 0.0f,
		2, -1, 4, -1, -1, -1, -1 },
	/* killing_spree */
	{ 1, 4, 76, -1, 0, 1, 2, 1, 30.0f, 0.0f,
		8, -1, -1, -1, -1, -1, -1 },
	/* damage */
	{ 3, 3, 21, -1, 1, 1, 2, 1, 10.0f, 0.0f,
		0, -1, 2, -1, -1, -1, -1 },
	{ 3, 3, 22, -1, 1, 1, 2, 1, 10.0f, 0.0f,
		2, -1, 2, -1, 1, -1, -1 },
	{ 3, 3, 29, -1, 0, 5, 3, 1, 10.0f, 0.0f,
		8, -1, 2, -1, -1, -1, -1 },
	{ 3, 3, 31, -1, 0, 6, 3, 1, 10.0f, 5.0f,
		0, -1, 2, -1, -1, 1, -1 },
	/* hurt */
	{ 2, 1, 23, -1, 1, 1, 0, 0, 10.0f, 10.0f,
		0, -1, 3, -1, -1, -1, -1 },
	{ 2, 2, 32, -1, 0, 5, 0, 0, 10.0f, 10.0f,
		0, -1, 3, -1, -1, -1, -1 },
	{ 2, 2, 35, -1, 0, 5, 0, 0, 10.0f, 10.0f,
		0, -1, 3, -1, -1, -1, 2 },
	{ 2, 2, 36, -1, 0, 5, 0, 0, 10.0f, 10.0f,
		0, -1, 3, -1, -1, -1, 11 },
	{ 2, 2, 37, -1, 0, 5, 0, 0, 10.0f, 10.0f,
		0, -1, 3, -1, -1, -1, 10 },
	{ 2, 3, 38, -1, 0, 5, 0, 0, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 5 },
	{ 2, 2, 40, -1, 0, 5, 0, 0, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 4 },
	{ 2, 3, 41, -1, 0, 5, 0, 0, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 6 },
	{ 2, 2, 42, -1, 0, 5, 0, 0, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 7 },
	{ 2, 2, 43, -1, 0, 5, 0, 0, 20.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 12 },
	{ 2, 3, 44, -1, 0, 5, 0, 0, 30.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 9 },
	{ 2, 2, 45, -1, 0, 5, 0, 0, 30.0f, 0.0f,
		0, -1, 3, -1, -1, -1, 8 },
	/* sighted_enemy */
	{ 4, 5, 108, 3, 0, 6, 3, 6, 10.0f, 10.0f,
		0, -1, -1, 0, -1, -1, -1 },
	{ 4, 5, 109, 3, 0, 6, 3, 6, 10.0f, 25.0f,
		0, -1, -1, 2, -1, -1, -1 },
	/* found_enemy */
	{ 5, 5, 110, 3, 0, 6, 3, 6, 10.0f, 25.0f,
		0, -1, -1, 2, -1, -1, -1 },
	/* unexpected_enemy */
	{ 6, 3, 111, -1, 0, 6, 3, 4, 10.0f, 8.0f,
		1, -1, -1, -1, -1, -1, -1 },
	/* found_dead_friend */
	{ 7, 5, 112, -1, 0, 6, 3, 6, 10.0f, 20.0f,
		0, -1, -1, 4, -1, -1, -1 },
	/* allegiance_changed */
	{ 8, 6, 113, -1, 0, 6, 3, 6, 10.0f, 0.0f,
		68, -1, 4, -1, -1, -1, -1 },
	{ 8, 4, 114, -1, 0, 6, 3, 4, 10.0f, 0.0f,
		64, -1, 2, -1, -1, -1, -1 },
	/* sighted_friend_player */
	{ 25, 4, 148, 3, 0, 6, 3, 5, 10.0f, 15.0f,
		0, -1, -1, 4, -1, -1, -1 },
	/* lost_contact */
	{ 13, 4, 161, -1, 0, 0, 2, 1, 10.0f, 20.0f,
		0, -1, -1, 3, -1, -1, -1 },
	{ 13, 4, 127, -1, 0, 0, 2, 1, 15.0f, 20.0f,
		0, 0, -1, 3, -1, -1, -1 },
	/* alert_noncombat */
	{ 15, 4, 125, -1, 0, 4, 2, 5, 10.0f, 20.0f,
		0, -1, -1, 2, -1, -1, -1 },
	/* blocked */
	{ 14, 2, 129, 1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, 1, -1, -1, -1, -1, 0 },
	/* search_start */
	{ 16, 3, 131, -1, 0, 0, 2, 1, 10.0f, 8.0f,
		0, 0, -1, 3, -1, -1, -1 },
	/* search_query */
	{ 17, 3, 132, -1, 0, 5, 2, 1, 10.0f, 25.0f,
		0, 0, -1, 3, -1, -1, -1 },
	/* search_report */
	{ 18, 3, 134, -1, 0, 0, 2, 1, 10.0f, 25.0f,
		0, 0, -1, 3, -1, -1, -1 },
	/* search_abandon */
	{ 19, 3, 135, -1, 0, 0, 2, 1, 10.0f, 8.0f,
		0, 0, -1, 3, -1, -1, -1 },
	/* search_group_abandon */
	{ 20, 4, 136, -1, 0, 0, 2, 1, 10.0f, 0.0f,
		0, 0, -1, 3, -1, -1, -1 },
	/* uncover_start */
	{ 21, 3, 137, 2, 0, 0, 2, 1, 10.0f, 30.0f,
		0, 1, -1, 3, -1, -1, -1 },
	/* cover */
	{ 24, 2, 143, -1, 0, 0, 2, 1, 10.0f, 30.0f,
		0, 1, -1, -1, -1, -1, -1 },
	/* advance */
	{ 22, 4, 139, 2, 0, 0, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* retreat */
	{ 23, 4, 141, -1, 0, 0, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* shooting */
	{ 26, 1, 149, -1, 0, 0, 0, 0, 10.0f, 10.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* shooting_vehicle */
	{ 27, 1, 150, -1, 0, 0, 0, 0, 20.0f, 10.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* shooting_berserk */
	{ 28, 2, 151, -1, 0, 0, 0, 0, 20.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* shooting_group */
	{ 29, 2, 152, -1, 0, 0, 0, 0, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* shooting_traitor */
	{ 30, 1, 153, -1, 0, 0, 0, 0, 30.0f, 10.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* flee */
	{ 31, 3, 156, -1, 0, 0, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* flee_leader_died */
	{ 32, 4, 158, -1, 0, 0, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* flee_idle */
	{ 33, 3, 2, -1, 0, 0, 0, 0, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* attempted_flee */
	{ 34, 2, 159, -1, 0, 4, 2, 5, 10.0f, 10.0f,
		0, 1, -1, -1, -1, -1, -1 },
	/* hiding_finished */
	{ 35, 3, 162, -1, 0, 0, 2, 1, 10.0f, 30.0f,
		0, 1, -1, 3, -1, -1, -1 },
	/* vehicle_entry */
	{ 36, 3, 163, -1, 0, 0, 0, 0, 10.0f, 10.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* vehicle_exit */
	{ 37, 3, 164, -1, 0, 0, 0, 0, 10.0f, 10.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* vehicle_woohoo */
	{ 38, 2, 165, 12, 2, 0, 0, 0, 10.0f, 25.0f,
		48, -1, -1, -1, -1, -1, -1 },
	/* vehicle_scared */
	{ 39, 2, 166, 13, 2, 0, 0, 0, 10.0f, 25.0f,
		48, -1, -1, -1, -1, -1, -1 },
	/* vehicle_falling */
	{ 40, 7, 10, 13, 2, 0, 0, 0, 10.0f, 0.0f,
		48, -1, -1, -1, -1, -1, -1 },
	/* grenade_sighted */
	{ 11, 3, 117, -1, 0, 6, 4, 6, 10.0f, 4.0f,
		0, 0, 3, -1, -1, -1, -1 },
	/* grenade_startle */
	{ 10, 3, 116, -1, 0, 6, 4, 6, 10.0f, 4.0f,
		0, -1, -1, 4, -1, -1, -1 },
	/* grenade_danger */
	{ 12, 5, 118, -1, 0, 5, 4, 5, 10.0f, 4.0f,
		0, 0, 3, -1, -1, -1, -1 },
	{ 12, 5, 120, -1, 0, 5, 4, 5, 10.0f, 0.0f,
		0, 0, 2, -1, -1, -1, -1 },
	{ 12, 6, 119, -1, 0, 6, 4, 5, 10.0f, 0.0f,
		0, -1, 1, -1, -1, -1, -1 },
	/* surprise */
	{ 41, 7, 177, -1, 0, 6, 2, 4, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* berserk */
	{ 42, 7, 178, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* melee */
	{ 43, 7, 179, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* grenade_throwing */
	{ 9, 7, 115, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* dive */
	{ 44, 7, 180, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* leap */
	{ 47, 7, 182, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* falling */
	{ 46, 7, 10, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* postcombat_alone */
	{ 48, 3, 197, -1, 0, 5, 0, 0, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* postcombat_unscathed */
	{ 49, 3, 198, -1, 0, 0, 0, 0, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* postcombat_wounded */
	{ 50, 3, 199, -1, 0, 0, 0, 0, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* postcombat_massacre */
	{ 51, 3, 201, -1, 0, 0, 0, 0, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* postcombat_triumph */
	{ 52, 3, 203, -1, 0, 0, 0, 0, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* postcombat_check_enemy */
	{ 53, 3, 189, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, 3, -1, -1, -1, -1 },
	/* postcombat_check_friend */
	{ 54, 3, 190, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, 2, -1, -1, -1, -1 },
	/* postcombat_shoot_corpse */
	{ 55, 3, 191, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	{ 55, 6, 192, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		2, -1, -1, -1, -1, 1, -1 },
	/* postcombat_celebrate */
	{ 56, 3, 188, -1, 0, 4, 2, 1, 10.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
	/* sentinel */
	{ -1, -1, -1, -1, -1, -1, -1, -1, 0.0f, 0.0f,
		0, -1, -1, -1, -1, -1, -1 },
};

struct reply_usage const global_reply_table[NUMBER_OF_REPLY_USAGES] =
{
	{ 76, -1, 2, 92, -1, 2, 0, 0.0f, 1.0f, 0.7f, 30.0f,
		reply_filter_close },
	{ 53, 2, 2, 81, -1, 2, 0, 0.0f, 0.5f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 11, 2, 82, -1, 2, 0, 0.0f, 0.6f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 10, 2, 83, -1, 2, 0, 0.0f, 0.6f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 5, 2, 84, -1, 2, 0, 0.0f, 0.8f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 3, 2, 85, -1, 2, 0, 0.5f, 0.9f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 4, 2, 86, -1, 2, 0, 0.0f, 1.0f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 6, 2, 87, -1, 2, 0, 0.0f, 1.0f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 7, 2, 88, -1, 2, 0, 0.0f, 0.9f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 12, 2, 89, -1, 2, 0, 0.0f, 0.6f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 9, 2, 90, -1, 2, 0, 0.0f, 0.8f, 0.7f, 60.0f,
		reply_filter_close },
	{ 53, 8, 2, 91, -1, 2, 0, 0.0f, 1.0f, 0.7f, 60.0f,
		reply_filter_close },
	{ 55, -1, 2, 56, -1, 4, 1, 1.0f, 1.0f, 0.3f, 0.0f,
		reply_filter_close },
	{ 57, -1, 2, 58, -1, 2, 0, 0.8f, 0.6f, 0.5f, 30.0f,
		reply_filter_close },
	{ 59, -1, 2, 60, -1, 2, 0, 0.8f, 0.6f, 0.5f, 20.0f,
		reply_filter_close },
	{ 61, -1, 2, 62, -1, 2, 0, 0.8f, 0.6f, 0.5f, 20.0f,
		reply_filter_close },
	{ 63, -1, 2, 64, -1, 2, 0, 0.8f, 0.6f, 0.5f, 20.0f,
		reply_filter_close },
	{ 53, -1, 2, 54, -1, 2, 0, 0.6f, 0.4f, 0.5f, 30.0f,
		reply_filter_close },
	{ 53, -1, 2, 80, -1, 2, 0, 0.0f, 0.4f, 0.7f, 40.0f,
		reply_filter_close },
	{ 23, -1, 2, 24, -1, 1, 0, 0.8f, 0.0f, 0.7f, 20.0f,
		NULL },
	{ 32, -1, 4, 33, -1, 1, 0, 0.8f, 0.0f, 0.7f, 20.0f,
		NULL },
	{ 32, -1, 2, 34, -1, 1, 0, 0.8f, 0.0f, 0.7f, 20.0f,
		NULL },
	{ 51, -1, 2, 52, -1, 4, 1, 1.0f, 0.0f, 0.3f, 0.0f,
		reply_filter_close },
	{ 49, -1, 2, 50, -1, 3, 0, 0.7f, 0.0f, 0.3f, 20.0f,
		reply_filter_close },
	{ 29, -1, 3, 30, -1, 2, 0, 0.7f, 0.4f, 0.5f, 20.0f,
		NULL },
	{ 108, -1, 2, 123, -1, 3, 0, 0.8f, 0.0f, 0.7f, 20.0f,
		reply_filter_close },
	{ 108, -1, 2, 124, -1, 3, 0, 0.8f, 0.0f, 0.7f, 20.0f,
		reply_filter_not_close },
	{ 109, -1, 2, 123, -1, 3, 0, 0.8f, 0.0f, 0.7f, 20.0f,
		reply_filter_close },
	{ 109, -1, 2, 124, -1, 3, 0, 0.8f, 0.0f, 0.7f, 20.0f,
		reply_filter_not_close },
	{ 110, -1, 2, 123, -1, 3, 0, 0.8f, 0.0f, 0.7f, 20.0f,
		reply_filter_close },
	{ 110, -1, 2, 124, -1, 3, 0, 0.8f, 0.0f, 0.7f, 20.0f,
		reply_filter_not_close },
	{ 125, -1, 3, 126, -1, 3, 0, 0.7f, 0.0f, 0.5f, 15.0f,
		NULL },
	{ 127, -1, 2, 128, -1, 3, 0, 0.7f, 0.0f, 0.5f, 30.0f,
		reply_filter_no_certain_target },
	{ 129, -1, 3, 130, -1, 3, 0, 0.5f, 0.0f, 0.5f, 20.0f,
		NULL },
	{ 132, -1, 2, 133, -1, 3, 0, 1.0f, 0.0f, 0.3f, 20.0f,
		reply_filter_searching },
	{ 137, -1, 2, 138, -1, 3, 0, 1.0f, 0.0f, 0.3f, 20.0f,
		reply_filter_same_target },
	{ 139, -1, 2, 140, -1, 4, 0, 0.7f, 0.0f, 0.7f, 20.0f,
		reply_filter_same_platoon },
	{ 141, -1, 2, 142, -1, 4, 0, 0.7f, 0.0f, 0.7f, 20.0f,
		reply_filter_same_platoon },
	{ 156, -1, 2, 157, -1, 3, 0, 0.5f, 0.0f, 0.7f, 30.0f,
		reply_filter_fighting_close },
	{ 156, -1, 4, 154, -1, 3, 0, 0.5f, 0.0f, 0.7f, 30.0f,
		reply_filter_fighting },
	{ 158, -1, 4, 154, -1, 3, 0, 0.5f, 0.0f, 0.7f, 30.0f,
		reply_filter_fighting },
	{ 159, -1, 2, 160, -1, 3, 0, 0.5f, 0.0f, 0.7f, 20.0f,
		reply_filter_flee_leader },
	{ 199, -1, 2, 200, -1, 3, 0, 0.8f, 0.0f, 0.5f, 0.0f,
		NULL },
	{ 201, -1, 2, 202, -1, 3, 0, 0.8f, 0.0f, 0.5f, 0.0f,
		NULL },
	{ 203, -1, 2, 204, -1, 3, 0, 0.8f, 0.0f, 0.5f, 0.0f,
		NULL },
	{ -1, -1, -1, -1, -1, -1, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		NULL },
};

char const *global_communication_priority_names[NUMBER_OF_COMMUNICATION_PRIORITIES] =
{
	"none",
	"filler",
	"chatter",
	"talk",
	"communicate",
	"shout",
	"yell",
	"exclaim",
};

char const *global_communication_team_names[NUMBER_OF_COMMUNICATION_TEAMS] =
{
	"human",
	"HUM",
	"covenant",
	"COV",
};

char const *global_communication_type_names[NUMBER_OF_COMMUNICATION_TYPES] =
{
	"death",
	"killing_spree",
	"hurt",
	"damage",
	"sighted_enemy",
	"found_enemy",
	"unexpected_enemy",
	"found_dead_friend",
	"allegiance_changed",
	"grenade_throwing",
	"grenade_startle",
	"grenade_sighted",
	"grenade_danger",
	"lost_contact",
	"blocked",
	"alert_noncombat",
	"search_start",
	"search_query",
	"search_report",
	"search_abandon",
	"search_group_abandon",
	"uncover_start",
	"advance",
	"retreat",
	"cover",
	"sighted_friend_player",
	"shooting",
	"shooting_vehicle",
	"shooting_berserk",
	"shooting_group",
	"shooting_traitor",
	"flee",
	"flee_leader_died",
	"flee_idle",
	"attempted_flee",
	"hiding_finished",
	"vehicle_entry",
	"vehicle_exit",
	"vehicle_woohoo",
	"vehicle_scared",
	"vehicle_falling",
	"surprise",
	"berserk",
	"melee",
	"dive",
	"uncover_exclamation",
	"falling",
	"leap",
	"postcombat_alone",
	"postcombat_unscathed",
	"postcombat_wounded",
	"postcombat_massacre",
	"postcombat_triumph",
	"postcombat_check_enemy",
	"postcombat_check_friend",
	"postcombat_shoot_corpse",
	"postcombat_celebrate",
};

short global_dialogue_event_count = 0;
struct dialogue_event_status *global_dialogue_events = NULL;
short global_reply_event_count = 0;
struct dialogue_event_status *global_reply_events = NULL;

/* ---------- public code */

void ai_communication_initialize(
	void)
{
	struct dialogue_usage const *dialogue;
	struct reply_usage const *reply;
	short *communication_table_index;
	short communication_type;
	short communication_type_index;
	short dialogue_index;

	global_dialogue_event_count = 0;
	dialogue = global_dialogue_table;
	do
	{
		dialogue++;
		global_dialogue_event_count++;
	}
	while (dialogue->communication_type != NONE);

	if (global_dialogue_events == NULL)
	{
		global_dialogue_events = (struct dialogue_event_status *)game_state_malloc(
			"ai communication dialogue",
			NULL,
			16 * global_dialogue_event_count);
		match_vassert(
			"c:\\halo\\SOURCE\\ai\\ai_communication.c",
			0x286,
			global_dialogue_events,
			"ai_communication_initialize: unable to allocate comm dialogue status table");
	}

	global_reply_event_count = 0;
	reply = global_reply_table;
	do
	{
		reply++;
		global_reply_event_count++;
	}
	while (reply->original_vocalization_type != NONE);

	if (global_reply_events == NULL)
	{
		global_reply_events = (struct dialogue_event_status *)game_state_malloc(
			"ai communication replies",
			NULL,
			16 * global_reply_event_count);
		match_vassert(
			"c:\\halo\\SOURCE\\ai\\ai_communication.c",
			0x293,
			global_reply_events,
			"ai_communication_initialize: unable to allocate comm reply status table");
	}

	communication_type_index = 0;
	communication_table_index = global_communication_table_indices;
	while (communication_type_index < NUMBER_OF_COMMUNICATION_TYPES)
	{
		dialogue_index = 0;
		*communication_table_index = NONE;
		dialogue = global_dialogue_table;
		communication_type = 0;

		while (communication_type != NONE)
		{
			if (communication_type == communication_type_index)
			{
				*communication_table_index = dialogue_index;
				break;
			}

			dialogue++;
			communication_type = dialogue->communication_type;
			dialogue_index++;
		}

		communication_type_index++;
		communication_table_index++;
	}

	conversation_data = game_state_data_new("ai conversation", 8, 100);
	match_assert(
		"c:\\halo\\SOURCE\\ai\\ai_communication.c",
		0x2A8,
		conversation_data);
	return;
}

void ai_communication_dispose(
	void)
{
	return;
}

void ai_communication_initialize_for_new_map(
	void)
{
	short event_index;

	ai_globals->dialogue_triggers_enabled = TRUE;
	csmemset(ai_globals->last_chatter_time, 0, sizeof(ai_globals->last_chatter_time));
	csmemset(ai_globals->last_talk_time, 0, sizeof(ai_globals->last_talk_time));
	csmemset(ai_globals->last_shout_time, 0, sizeof(ai_globals->last_shout_time));

	event_index = 0;
	if (((long)global_dialogue_event_count << 1) > 0)
	{
		do
		{
			global_dialogue_events[event_index].disable_until_time = NONE;
			global_dialogue_events[event_index].last_time_spoken = NONE;
			event_index = (short)(event_index + 1);
		}
		while ((long)event_index < (long)global_dialogue_event_count << 1);
	}

	event_index = 0;
	if (((long)global_reply_event_count << 1) > 0)
	{
		do
		{
			global_reply_events[event_index].disable_until_time = NONE;
			global_reply_events[event_index].last_time_spoken = NONE;
			event_index = (short)(event_index + 1);
		}
		while ((long)event_index < (long)global_reply_event_count << 1);
	}

	ai_globals->recent_conversation_count = 0;
	ai_globals->recent_conversation_next_index = 0;
	csmemset(
		ai_globals->recent_conversations,
		0,
		sizeof(ai_globals->recent_conversations));
	data_make_valid(conversation_data);
	return;
}

void ai_communication_dispose_from_old_map(
	void)
{
	data_make_invalid(conversation_data);
	return;
}

void ai_communication_packet_new(
	struct ai_information_packet *information)
{
	match_assert(
		"c:\\halo\\SOURCE\\ai\\ai_communication.c",
		0x300,
		information);
	csmemset(information, 0, sizeof(*information));
	information->target_unit_index = NONE;
	information->communication_type = NONE;
	information->dialogue_type_index = NONE;
	information->damage_category = NONE;
	return;
}

char const *ai_communication_get_type_name(
	short communication_type)
{
	char const *name = "<error>";

	if (communication_type >= 0 &&
		communication_type < NUMBER_OF_COMMUNICATION_TYPES)
	{
		name = global_communication_type_names[communication_type];
	}

	return name;
}

short ai_communication_get_type_by_name(
	char const *name)
{
	short communication_type = NONE;
	short index;

	for (index = 0; index < NUMBER_OF_COMMUNICATION_TYPES; index++)
	{
		if (csstrcmp(global_communication_type_names[index], name) == 0)
		{
			communication_type = index;
		}
	}

	return communication_type;
}

static boolean reply_filter_close(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index)
{
	struct prop_datum *prop;
	long prop_index;
	boolean result;

	result = FALSE;
	if (reply_actor_index != NONE)
	{
		prop_index = prop_get_base_by_unit_index(
			reply_actor_index,
			original_unit_index,
			TRUE,
			TRUE);
		if (prop_index != NONE)
		{
			prop = prop_get(prop_index);
			if (prop->distance < COMMUNICATION_CLOSE_DISTANCE &&
				(prop->line_of_sight == _ai_line_of_sight_clear ||
				prop->line_of_sight == _ai_line_of_sight_occluded))
			{
				result = TRUE;
			}
		}
	}

	return result;
}

static boolean reply_filter_not_close(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index)
{
	struct prop_datum *prop;
	long prop_index;
	boolean result;

	result = FALSE;
	if (reply_actor_index != NONE)
	{
		prop_index = prop_get_base_by_unit_index(
			reply_actor_index,
			original_unit_index,
			TRUE,
			TRUE);
		if (prop_index != NONE)
		{
			prop = prop_get(prop_index);
			if (prop->distance > COMMUNICATION_CLOSE_DISTANCE ||
				(prop->line_of_sight != _ai_line_of_sight_clear &&
				prop->line_of_sight != _ai_line_of_sight_occluded))
			{
				result = TRUE;
			}
		}
	}

	return result;
}

static boolean reply_filter_searching(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index)
{
	struct actor_datum *reply_actor;
	boolean result;

	result = FALSE;
	if (reply_actor_index != NONE)
	{
		reply_actor = actor_get(reply_actor_index);
		switch (reply_actor->state.action)
		{
			case _actor_action_uncover:
				result = (boolean)(reply_actor->state.action_data.uncover.pursuit_location.type ==
					_pursuit_location_position);
				break;

			case _actor_action_search:
				result = TRUE;
				break;
		}
	}

	return result;
}

static boolean reply_filter_same_platoon(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index)
{
	struct unit_datum *original_unit;
	struct actor_datum *original_actor;
	struct actor_datum *reply_actor;
	long original_actor_index;
	boolean result;

	result = FALSE;
	if (reply_filter_close(
		original_unit_index,
		communication,
		reply_actor_index))
	{
		original_unit = unit_get(original_unit_index);
		original_actor_index = original_unit->unit.actor_index;
		if (original_actor_index != NONE && reply_actor_index != NONE)
		{
			original_actor = actor_get(original_actor_index);
			reply_actor = actor_get(reply_actor_index);
			result = original_actor->meta.encounter_index != NONE &&
				original_actor->meta.encounter_index == reply_actor->meta.encounter_index &&
				original_actor->meta.platoon_index == reply_actor->meta.platoon_index;
		}
	}

	return result;
}

static boolean reply_filter_fighting(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index)
{
	return actor_is_fighting(reply_actor_index);
}

static boolean reply_filter_fighting_close(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index)
{
	boolean result;

	result = FALSE;
	if (reply_filter_close(
		original_unit_index,
		communication,
		reply_actor_index) &&
		actor_is_fighting(reply_actor_index))
	{
		result = TRUE;
	}

	return result;
}

static boolean reply_filter_same_target(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index)
{
	struct unit_datum *original_unit;
	struct actor_datum *original_actor;
	struct actor_datum *reply_actor;
	struct prop_datum *original_prop;
	struct prop_datum *reply_prop;
	long original_actor_index;
	boolean result;

	result = FALSE;
	if (reply_filter_close(
		original_unit_index,
		communication,
		reply_actor_index))
	{
		original_unit = unit_get(original_unit_index);
		original_actor_index = original_unit->unit.actor_index;
		if (original_actor_index != NONE && reply_actor_index != NONE)
		{
			original_actor = actor_get(original_actor_index);
			reply_actor = actor_get(reply_actor_index);
			if (original_actor->target.target_prop_index != NONE &&
				reply_actor->target.target_prop_index != NONE)
			{
				original_prop = prop_get(original_actor->target.target_prop_index);
				reply_prop = prop_get(reply_actor->target.target_prop_index);
				result = original_prop->unit_index == reply_prop->unit_index;
			}
		}
	}

	return result;
}

static boolean reply_filter_no_certain_target(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index)
{
	struct actor_datum *reply_actor;
	boolean result;

	result = FALSE;
	if (reply_actor_index != NONE)
	{
		reply_actor = actor_get(reply_actor_index);
		if (reply_actor->state.mode == _actor_mode_combat &&
			reply_actor->state.combat_status < _actor_combat_status_certain)
		{
			result = TRUE;
		}
	}

	return result;
}

static boolean reply_filter_flee_leader(
	long original_unit_index,
	struct ai_information_packet *communication,
	long reply_actor_index)
{
	struct actor_datum *reply_actor;
	boolean result;

	result = FALSE;
	if (actor_is_fighting(reply_actor_index))
	{
		reply_actor = actor_get(reply_actor_index);
		if (reply_actor->meta.type == _actor_elite)
		{
			result = TRUE;
		}
	}

	return result;
}

static short ai_communication_consider_speech(
	long unit_index,
	short communication_priority,
	short speech_priority,
	short delay_ticks,
	boolean allow_vocalization_lookup,
	boolean allow_recent_disabling,
	short *vocalization_type,
	real *weight,
	long *sound_definition_index_reference,
	char *debug_string)
{
	long last_speech_time;
	long elapsed_time;
	short play_type;
	short timer_tolerance;
	short speech_time;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\ai_communication.c",
		0xC1A,
		vocalization_type && sound_definition_index_reference && weight);
	play_type = unit_test_speech(
		unit_index,
		speech_priority,
		allow_vocalization_lookup,
		TRUE,
		&last_speech_time,
		vocalization_type,
		sound_definition_index_reference);

	if (play_type == _unit_play_speech_none)
	{
		if (debug_string)
		{
			sprintf(
				debug_string,
				"nospch-%s",
				unit_get_speech_priority_name(speech_priority));
		}
	}
	else if (play_type == _unit_play_speech_queue)
	{
		*weight *= 0.3f;
	}

	if ((game_connection() != _game_connection_local ||
		!ai_debug.communication_unit_repeat_disabled) &&
		allow_recent_disabling &&
		communication_priority < _ai_communication_priority_shout &&
		last_speech_time != NONE)
	{
		elapsed_time = game_time_get() - last_speech_time;
		speech_time = (short)(elapsed_time < 0 ? 0 : elapsed_time);
		timer_tolerance = (short)(long)(
			communication_timer_tolerances[communication_priority][0][2] * 30.0f +
			(real)delay_ticks);
		if (speech_time <= timer_tolerance)
		{
			play_type = _unit_play_speech_none;
			*weight = 0.0f;
			if (debug_string)
			{
				sprintf(
					debug_string,
					"spk%d<tol%d+%d",
					speech_time,
					delay_ticks,
					timer_tolerance - delay_ticks);
			}
		}
		else if (speech_time < timer_tolerance + 60)
		{
			*weight =
				(real)(speech_time - timer_tolerance) *
				*weight *
				0.016666668f;
		}
	}

	match_assert(
		"c:\\halo\\SOURCE\\ai\\ai_communication.c",
		0xC49,
		(play_type == _unit_play_speech_none) || (*weight > 0.0f));
	return play_type;
}

static real ai_communication_actor_talk_weight(
	long actor_index,
	long subject_unit_index,
	real_point3d const *subject_point,
	long cause_unit_index,
	real_point3d const *cause_point,
	real stimulus_range,
	short communication_type,
	short communication_priority,
	short speech_priority,
	short vocalization_type,
	short animation_type,
	short flags)
{
	struct actor_datum *actor;
	boolean has_target;
	boolean passes;
	real weight;

	actor = actor_get(actor_index);
	has_target = subject_unit_index != NONE || cause_unit_index != NONE;
	passes = TRUE;
	weight = 10.0f;
	if (actor->meta.unit_index == NONE)
	{
		passes = FALSE;
	}

	if (actor->state.mode > _actor_mode_asleep && passes)
	{
		if (has_target)
		{
			if (subject_unit_index != NONE)
			{
				real dx;
				real dy;
				real dz;
				real distance_squared;

				dx = subject_point->x - actor->input.position.head_position.x;
				dy = subject_point->y - actor->input.position.head_position.y;
				dz = subject_point->z - actor->input.position.head_position.z;
				distance_squared = dx * dx;
				distance_squared += dy * dy;
				distance_squared += dz * dz;
				passes =
					distance_squared <
					stimulus_range * stimulus_range;
			}
			else
			{
				passes = FALSE;
			}

			if (!passes && cause_unit_index != NONE)
			{
				passes = distance_squared3d(
					&actor->input.position.head_position,
					cause_point) <
					stimulus_range * stimulus_range;
			}
		}

		if (!passes)
		{
			goto done;
		}

		if (TEST_FLAG(flags, _find_actor_near_to_players_bit))
		{
			real player_rating;

			player_rating = ai_communication_get_player_rating(
				actor->meta.unit_index,
				FALSE,
				NULL,
				NULL);
			if (player_rating == 0.0f)
			{
				passes = FALSE;
				goto done;
			}
			else
			{
				weight = player_rating * 5.0f + 10.0f;
			}
		}

		if (TEST_FLAG(flags, _find_actor_same_vehicle_bit) &&
			subject_unit_index != NONE)
		{
			struct unit_datum *subject_unit;

			subject_unit = unit_get(subject_unit_index);
			if (subject_unit->object.parent_object_index !=
				actor->input.vehicle_index)
			{
				passes = FALSE;
				goto done;
			}
		}

		if ((short)animation_type != NONE &&
			unit_test_animation_impulse(
				actor->meta.unit_index,
				animation_type))
		{
			weight += 5.0f;
		}

		if (vocalization_type != NONE)
		{
			short line_vocalization_type = vocalization_type;
			long sound_definition_index = NONE;

			if (ai_communication_consider_speech(
				actor->meta.unit_index,
				communication_priority,
				speech_priority,
				0,
				TEST_FLAG(flags, _find_actor_allow_lookup_bit),
				TRUE,
				&line_vocalization_type,
				&weight,
				&sound_definition_index,
				NULL) == _unit_play_speech_none)
			{
				passes = FALSE;
				goto done;
			}
		}

		if (has_target)
		{
			boolean cause_matches;
			boolean subject_matches;

			cause_matches = subject_matches = FALSE;
			if (subject_unit_index != NONE)
			{
				if (actor->meta.unit_index == subject_unit_index)
				{
					if (TEST_FLAG(flags, _find_actor_allow_subject_bit))
					{
						subject_matches = TRUE;
					}
					else
					{
						passes = FALSE;
					}
				}
				else
				{
					long prop_index;

					prop_index = prop_get_base_by_unit_index(
						actor_index,
						subject_unit_index,
						TRUE,
						FALSE);
					if (prop_index != NONE)
					{
						struct prop_datum *prop;

						prop = prop_get(prop_index);
						if (prop->distance > stimulus_range)
						{
							subject_matches = FALSE;
						}
						else if ((prop->state >=
									_prop_state_becoming_unacknowledged &&
								 prop->state <= _prop_state_acknowledged) ||
								(!prop->enemy &&
									(communication_type != 0 ||
									 prop->audibility >= _ai_sound_volume_loud ||
									 prop->ineffability >= 2 ||
									 actor_visibility_at_point(
										actor_index,
										&actor->input.position,
										&prop->head_position,
										prop->flashlight ?
											_prop_lighting_bright :
											prop->lighting,
										(word)prop->line_of_sight,
										TRUE,
										FALSE,
										actor_get_perception_knowledge(
											actor_index,
											prop_index)) >= 2)))
						{
							subject_matches = TRUE;
						}
						if (subject_matches)
						{
							weight +=
								(1.0f - prop->distance / stimulus_range) *
								10.0f;
						}
					}
				}
			}

			if (cause_unit_index != NONE)
			{
				if (actor->meta.unit_index == cause_unit_index)
				{
					if (TEST_FLAG(flags, _find_actor_allow_cause_bit))
					{
						cause_matches = TRUE;
					}
					else
					{
						passes = FALSE;
					}
				}
				else
				{
					long prop_index;

					prop_index = prop_get_active_by_unit_index(
						actor_index,
						cause_unit_index);
					if (prop_index != NONE)
					{
						struct prop_datum *prop;

						prop = prop_get(prop_index);
						if (prop->distance > stimulus_range)
						{
							cause_matches = FALSE;
						}
						else if ((prop->state >=
									_prop_state_becoming_unacknowledged &&
								 prop->state <= _prop_state_acknowledged) ||
								!prop->enemy)
						{
							cause_matches = TRUE;
						}
						if (cause_matches)
						{
							weight +=
								(1.0f - prop->distance / stimulus_range) *
								10.0f;
						}
					}
				}
			}

			if (passes)
			{
				passes = cause_matches | subject_matches;
			}
		}
	}
	else
	{
		passes = FALSE;
	}

done:
	return passes ? weight : 0.0f;
}

static long ai_communication_find_specific_actor_to_talk(
	long ai_index,
	long subject_unit_index,
	long cause_unit_index,
	real maximum_distance,
	short communication_type,
	short communication_priority,
	short speech_priority,
	short vocalization_type,
	short animation_type,
	short flags)
{
	struct ai_script_actor_reference_iterator iterator;
	real_point3d subject_point;
	real_point3d cause_point;
	long best_actor_index = NONE;
	real best_weight = 0.0f;

	if (ai_index != NONE)
	{
		if (subject_unit_index != NONE)
		{
			unit_get_head_position(subject_unit_index, &subject_point);
		}
		if (cause_unit_index != NONE)
		{
			unit_get_head_position(cause_unit_index, &cause_point);
		}

		ai_index_actor_iterator_new(ai_index, &iterator);
		while (ai_index_actor_iterator_next(&iterator))
		{
			real weight = ai_communication_actor_talk_weight(
				iterator.actor_index,
				subject_unit_index,
				&subject_point,
				cause_unit_index,
				&cause_point,
				maximum_distance,
				communication_type,
				communication_priority,
				speech_priority,
				vocalization_type,
				animation_type,
				flags);

			if (weight > best_weight)
			{
				best_actor_index = iterator.actor_index;
				best_weight = weight;
			}
		}
	}

	return best_actor_index;
}

static long ai_communication_find_global_actor_to_talk(
	short team_index,
	short find_actor_mode,
	long subject_unit_index,
	long cause_unit_index,
	real maximum_distance,
	short communication_type,
	short communication_priority,
	short speech_priority,
	short vocalization_type,
	short animation_type,
	short flags)
{
	struct actor_iterator iterator;
	struct actor_datum *actor;
	real_point3d subject_point;
	real_point3d cause_point;
	long best_actor_index = NONE;
	real best_weight = 0.0f;

	if (subject_unit_index != NONE)
	{
		unit_get_head_position(subject_unit_index, &subject_point);
	}
	if (cause_unit_index != NONE)
	{
		/* BUG (preserved for exact matching): January asks for the subject
		 * position again here and leaves cause_point uninitialized. A corrected
		 * build should call unit_get_head_position(cause_unit_index, &cause_point). */
		unit_get_head_position(subject_unit_index, &subject_point);
	}

	actor_iterator_new(&iterator, TRUE);
	actor = actor_iterator_next(&iterator);
	while (actor)
	{
		boolean eligible = TRUE;

		if (team_index != NONE)
		{
			boolean enemy = game_team_is_enemy(
				team_index,
				actor->meta.team_index);

			switch (find_actor_mode)
			{
			case _find_actor_mode_same_team:
				eligible = actor->meta.team_index == team_index;
				break;

			case _find_actor_mode_friend:
				eligible = !enemy;
				break;

			case _find_actor_mode_enemy:
				eligible = enemy;
				break;

			default:
				match_assert(
					"c:\\halo\\SOURCE\\ai\\ai_communication.c",
					0xDFD,
					!"unreachable");
				eligible = FALSE;
				break;
			}
		}

		if (eligible)
		{
			real weight = ai_communication_actor_talk_weight(
				iterator.index,
				subject_unit_index,
				&subject_point,
				cause_unit_index,
				&cause_point,
				maximum_distance,
				communication_type,
				communication_priority,
				speech_priority,
				vocalization_type,
				animation_type,
				flags);

			if (weight > best_weight)
			{
				best_actor_index = iterator.index;
				best_weight = weight;
			}
		}

		actor = actor_iterator_next(&iterator);
	}

	return best_actor_index;
}

short actor_communication_team(
	long actor_index)
{
	struct actor_datum *actor = actor_get(actor_index);
	short race = actor_type_get_race(actor->meta.type);
	short communication_team = NONE;

	if (TEST_FLAG(race, _race_human_bit))
	{
		communication_team = _ai_communication_team_human;
	}
	else if (TEST_FLAG(race, _race_covenant_bit))
	{
		communication_team = _ai_communication_team_covenant;
	}

	return communication_team;
}

static void ai_communication_look_secondary_at_unit(
	long actor_index,
	short type,
	short priority,
	long look_unit_index,
	long prop_index)
{
	struct direction_specification direction;
	short prop_state;

	if (actor_index != NONE &&
		priority > 0 &&
		look_unit_index != NONE &&
		unit_try_and_get(look_unit_index))
	{
		if (prop_index == NONE)
		{
			prop_index = prop_get_active_by_unit_index(
				actor_index,
				look_unit_index);
			if (prop_index == NONE)
			{
				goto look_at_point;
			}
		}

		prop_state = prop_get(prop_index)->state;
		if (prop_state >= _prop_state_becoming_unacknowledged &&
			prop_state <= _prop_state_acknowledged &&
			prop_index != NONE)
		{
			direction.type = _direction_specification_prop;
			direction.prop_index = prop_index;
		}
		else
		{
		look_at_point:
			direction.type = _direction_specification_point;
			unit_get_head_position(
				look_unit_index,
				&direction.point);
		}

		actor_look_secondary(
			actor_index,
			type,
			priority,
			&direction);
	}

	return;
}

static void ai_communication_look_secondary_at_object(
	long actor_index,
	short type,
	short priority,
	long object_index)
{
	struct direction_specification direction;

	if (actor_index != NONE &&
		priority > 0 &&
		object_index != NONE &&
		object_try_and_get(object_index))
	{
		direction.type = _direction_specification_object;
		direction.object_index = object_index;
		actor_look_secondary(
			actor_index,
			type,
			priority,
			&direction);
	}

	return;
}

short ai_conversation_status(
	short scenario_conversation_index)
{
	struct data_iterator iterator;
	struct ai_conversation_datum_header *conversation;
	short status = 0;

	data_iterator_new(&iterator, conversation_data);
	while ((conversation = (struct ai_conversation_datum_header *)
		data_iterator_next(&iterator)) != NULL)
	{
		if (conversation->scenario_conversation_index == scenario_conversation_index)
		{
			short conversation_status;

			if (!conversation->begun)
			{
				conversation_status = 1;
			}
			else if (!conversation->any_line_spoken)
			{
				conversation_status = 2;
			}
			else
			{
				conversation_status = conversation->waiting_to_advance ? 4 : 3;
			}

			status = MAX(status, conversation_status);
		}
	}

	if (!status)
	{
		struct recent_conversation_view *recent_conversation;
		long latest_finish_time = NONE;
		short latest_index = NONE;
		short index;

		for (index = 0; index < ai_globals->recent_conversation_count; index++)
		{
			if (ai_globals->recent_conversations[index].scenario_conversation_index ==
				scenario_conversation_index &&
				ai_globals->recent_conversations[index].finish_time > latest_finish_time)
			{
				latest_index = index;
				latest_finish_time = ai_globals->recent_conversations[index].finish_time;
			}
		}

		if (latest_index != NONE)
		{
			recent_conversation = &ai_globals->recent_conversations[latest_index];
			if (recent_conversation->unable_to_begin)
			{
				status = 5;
			}
			else
			{
				status = recent_conversation->finished_successfully ? 6 : 7;
			}
		}
	}

	return status;
}

static void actor_reset_idle_vocalization_timer(
	long actor_index)
{
	struct actor_datum *actor;
	struct actor_definition *definition;
	struct unit_datum *unit;
	boolean in_combat;
	short speech_offset;
	real delay;

	actor = actor_get(actor_index);
	definition = actor_definition_get(actor->meta.definition_index);
	in_combat = actor_in_combat(actor_index);
	speech_offset = 0;
	if (actor->meta.unit_index != NONE)
	{
		unit = unit_get(actor->meta.unit_index);
		if (unit->unit.speech.current.priority > 0)
		{
			speech_offset = unit->unit.speech.sound_timer;
		}
	}

	if (in_combat)
	{
		real maximum_delay;
		real minimum_delay;

		maximum_delay = definition->communication.idle_combat_time_upper_bound;
		minimum_delay = definition->communication.idle_combat_time_lower_bound;
		delay = real_seed_random_range(
			get_global_random_seed_address(),
			minimum_delay,
			maximum_delay);
	}
	else
	{
		real maximum_delay;
		real minimum_delay;

		maximum_delay = definition->communication.idle_noncombat_time_upper_bound;
		minimum_delay = definition->communication.idle_noncombat_time_lower_bound;
		delay = real_seed_random_range(
			get_global_random_seed_address(),
			minimum_delay,
			maximum_delay);
	}

	actor->control.idle_vocalization_combat = in_combat;
	actor->control.idle_vocalization_timer =
		(short)(delay * 30.0f + (real)speech_offset);
	return;
}

short ai_conversation_line(
	short scenario_conversation_index)
{
	struct data_iterator iterator;
	struct ai_conversation_line_view *conversation;
	short line = 999;

	data_iterator_new(&iterator, conversation_data);
	while ((conversation = (struct ai_conversation_line_view *)data_iterator_next(&iterator)) != NULL)
	{
		if (conversation->header.scenario_conversation_index == scenario_conversation_index)
		{
			line = conversation->current_line;
			break;
		}
	}

	return line;
}

void ai_conversation_advance(
	short scenario_conversation_index)
{
	struct data_iterator iterator;
	struct ai_conversation_datum_header *conversation;

	data_iterator_new(&iterator, conversation_data);
	while ((conversation = (struct ai_conversation_datum_header *)
		data_iterator_next(&iterator)) != NULL)
	{
		if (conversation->scenario_conversation_index == scenario_conversation_index)
		{
			if (ai_print_conversations)
			{
				struct scenario_conversation_definition_view *definition =
					TAG_BLOCK_GET_ELEMENT(
						&global_scenario_get()->ai_conversations,
						scenario_conversation_index,
						struct scenario_conversation_definition_view);

				console_printf(
					FALSE,
					"%s: told to advance by scripting",
					definition->name);
			}

			conversation->told_to_advance = TRUE;
		}
	}

	return;
}

void ai_conversation_finish(
	long conversation_index,
	boolean unable_to_begin,
	boolean success)
{
	struct ai_conversation_datum_view *conversation;
	struct ai_conversation_datum_view *recent_conversation;
	struct scenario_conversation_definition_view *definition;
	struct actor_datum *actor;
	short recent_conversation_index;
	long recent_conversation_count;
	unsigned long participant_bitmask;
	short participant_index;
	short actor_action;

	if (conversation_index != NONE)
	{
		conversation = (struct ai_conversation_datum_view *)datum_get(
			conversation_data,
			conversation_index);
		definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_conversations,
			conversation->scenario_conversation_index,
			struct scenario_conversation_definition_view);

		if (ai_print_conversations)
		{
			console_printf(
				FALSE,
				"%s: finished %s%s",
				definition->name,
				success ? "successfully" : "prematurely",
				unable_to_begin ? " (unable to begin)" : "");
		}

		recent_conversation = (struct ai_conversation_datum_view *)datum_get(
			conversation_data,
			conversation_index);
		recent_conversation_index =
			(word)ai_globals->recent_conversation_next_index;
		ai_globals->recent_conversation_next_index =
			(short)(recent_conversation_index + 1);
		ai_globals->recent_conversation_next_index %=
			MAXIMUM_RECENT_CONVERSATIONS;

		recent_conversation_count = ai_globals->recent_conversation_count;
		if (recent_conversation_count <= recent_conversation_index + 1)
		{
			recent_conversation_count = recent_conversation_index + 1;
		}
		ai_globals->recent_conversation_count =
			(short)recent_conversation_count;

		ai_globals->recent_conversations[recent_conversation_index]
			.scenario_conversation_index =
			recent_conversation->scenario_conversation_index;
		ai_globals->recent_conversations[recent_conversation_index]
			.unable_to_begin = unable_to_begin;
		ai_globals->recent_conversations[recent_conversation_index]
			.finished_successfully = success;
		ai_globals->recent_conversations[recent_conversation_index]
			.finish_time = game_time_get();

		participant_index = 0;
		while ((long)participant_index < definition->participants.count)
		{
			participant_bitmask = conversation->participant_bitmask;
			if ((participant_bitmask & FLAG(participant_index)) != 0 &&
				conversation->actor_indices[participant_index] != NONE)
			{
				actor = actor_get(conversation->actor_indices[participant_index]);
				actor_action = actor->state.action;
				actor->external_orders.conversation_index = NONE;
				actor->external_orders.conversation_attention_unit_index = NONE;
				if (actor_action == _actor_action_converse)
				{
					actor->state.action_data.converse.conversation_index = NONE;
				}
			}

			participant_index = (short)(participant_index + 1);
		}

		datum_delete(conversation_data, conversation_index);
	}

	return;
}

real ai_communication_get_player_rating(
	long unit_index,
	boolean test_line_of_sight,
	long *unit_index_reference,
	real *distance_reference)
{
	struct data_iterator iterator;
	struct player_datum *player;
	real_point3d position;
	real_point3d player_position;
	/* INFERRED FROM JANUARY'S BYTES - not attested in any surviving source.
	 * Naming the two endpoints before the vector is taken is what fixes the
	 * order in which the compiler squares the vector's components: an
	 * address-of expression and a pointer value are different operand forms
	 * to this compiler.  Measured on this function with everything else held
	 * constant, squaring order of the components:
	 *
	 *     &player_position, &position          k j i   (the untouched source)
	 *     player_head_position, &position      k j i
	 *     &player_position, head_position      k j i
	 *     player_head_position, head_position  k i j   <- this spelling
	 *     any of the above plus &vector bound  k j i
	 *
	 * January squares them k, i, j.  Section size, relocation count,
	 * instruction count and frame size are identical in all five. */
	real_point3d const *head_position = &position;
	real_point3d const *player_head_position = &player_position;
	real_vector3d vector;
	real_vector3d aiming_vector;
	long closest_unit_index = NONE;
	real best_rating = 0.0f;
	real closest_distance = REAL_MAX;
	boolean any_players = FALSE;
	real rating;

	unit_get_head_position(unit_index, &position);
	data_iterator_new(&iterator, player_data);
	while ((player = (struct player_datum *)data_iterator_next(&iterator)) != NULL)
	{
		if (player->unit_index != NONE)
		{
			real distance_squared;

			any_players = TRUE;
			unit_get_head_position(player->unit_index, &player_position);
			vector_from_points3d(player_head_position, head_position, &vector);
			distance_squared = magnitude_squared3d(&vector);
			if (distance_squared <
				communication_player_absolute_range * communication_player_absolute_range)
			{
				boolean clear_line_of_sight = FALSE;
				real distance;

				if (test_line_of_sight)
				{
					struct collision_result collision;
					real_vector3d line_of_sight_vector;
					boolean blocked;
					short cluster_index = object_get(object_get_ultimate_parent(unit_index))->
						object.location.cluster_index;
					short player_cluster_index =
						object_get(object_get_ultimate_parent(player->unit_index))->
							object.location.cluster_index;

					if (cluster_index != NONE &&
						player_cluster_index != NONE &&
						!scenario_test_pvs(cluster_index, player_cluster_index))
					{
						continue;
					}

					ai_profile.meters[_ai_meter_collisions].accumulator++;
					match_assert(
						"c:\\halo\\SOURCE\\ai\\ai_communication.c",
						0xE91,
						global_current_collision_user_depth <
							MAXIMUM_COLLISION_USER_STACK_DEPTH);
					global_current_collision_users[global_current_collision_user_depth++] =
						_collision_user_ai_comms;

					vector_from_points3d(
						player_head_position,
						head_position,
						&line_of_sight_vector);
					blocked = collision_test_vector(
						FLAG(_collision_test_front_facing_surfaces_bit) |
							FLAG(_collision_test_back_facing_surfaces_bit) |
							FLAG(_collision_test_ignore_two_sided_surfaces_bit) |
							FLAG(_collision_test_structure_bit),
						&player_position,
						&line_of_sight_vector,
						NONE,
						&collision);

					match_assert(
						"c:\\halo\\SOURCE\\ai\\ai_communication.c",
						0xE97,
						global_current_collision_user_depth > 1);
					--global_current_collision_user_depth;

					clear_line_of_sight = (boolean)(distance_squared <
						communication_player_ideal_range_min *
							communication_player_ideal_range_min ||
						!blocked);
				}

				rating = 1.0f;
				distance = square_root(distance_squared);
				if (distance < communication_player_ideal_range_max)
				{
					if (distance < communication_player_ideal_range_min)
					{
						rating = 2.0f;
					}
					else
					{
						rating = 1.0f +
							(communication_player_ideal_range_max - distance) /
							(communication_player_ideal_range_max -
								communication_player_ideal_range_min);
					}

					if (clear_line_of_sight)
					{
						rating += 0.5f;
					}

					if (distance > _real_epsilon)
					{
						real facing;

						unit_get_aiming_vector(player->unit_index, &aiming_vector);
						facing = dot_product3d(&aiming_vector, &vector) / distance;
						if (facing > communication_player_ideal_fov)
						{
							rating += 0.7f - (1.0f - facing) /
								(1.0f - communication_player_ideal_fov) * 0.35f;
						}
					}
				}

				if (rating > best_rating)
				{
					best_rating = rating;
					closest_unit_index = player->unit_index;
					closest_distance = distance;
				}
			}
		}
	}

	rating = !any_players ? 1.0f : best_rating;

	if (distance_reference)
	{
		*distance_reference = closest_distance;
	}

	if (unit_index_reference)
	{
		*unit_index_reference = closest_unit_index;
	}

	return rating;
}

void ai_conversation_stop(
	short scenario_conversation_index)
{
	struct data_iterator iterator;
	struct ai_conversation_datum_header *conversation;

	data_iterator_new(&iterator, conversation_data);
	while ((conversation = (struct ai_conversation_datum_header *)
		data_iterator_next(&iterator)) != NULL)
	{
		if (conversation->scenario_conversation_index == scenario_conversation_index)
		{
			if (ai_print_conversations)
			{
				struct scenario_conversation_definition_view *definition =
					TAG_BLOCK_GET_ELEMENT(
						&global_scenario_get()->ai_conversations,
						scenario_conversation_index,
						struct scenario_conversation_definition_view);

				console_printf(
					FALSE,
					"%s: told to stop by scripting",
					definition->name);
			}

			ai_conversation_finish(iterator.datum_index, FALSE, FALSE);
		}
	}

	return;
}

void actor_communication_update(
	long actor_index)
{
	struct actor_datum *actor;
	boolean in_combat;

	actor = actor_get(actor_index);
	if (actor->state.mode >= _actor_mode_alert &&
		ai_globals->dialogue_triggers_enabled)
	{
		in_combat = actor_in_combat(actor_index);
		if (actor->control.idle_vocalization_timer == 0 ||
			actor->control.idle_vocalization_combat != in_combat)
		{
			actor_reset_idle_vocalization_timer(actor_index);
		}

		if (actor->control.idle_vocalization_timer > 0 &&
			--actor->control.idle_vocalization_timer == 0)
		{
			long sound_definition_index = NONE;
			long vocalization_type = in_combat != FALSE;
			short play_type = unit_test_speech(
				actor->meta.unit_index,
				_unit_speech_idle,
				TRUE,
				FALSE,
				NULL,
				(short *)&vocalization_type,
				&sound_definition_index);
			if (play_type > 0)
			{
				struct unit_speech_item speech_item;

				csmemset(&speech_item, 0, sizeof(speech_item));
				speech_item.vocalization_type = (short)vocalization_type;
				speech_item.sound_definition_index = sound_definition_index;
				speech_item.priority = _unit_speech_idle;
				ai_communication_packet_new(&speech_item.ai);
				unit_speak(actor->meta.unit_index, play_type, &speech_item);
			}
		}
	}

	return;
}

static void ai_communication_handle_received_looking(
	long actor_index,
	long prop_index,
	struct ai_information_packet *information)
{
	struct prop_datum *prop;
	short type;

	if (information->look_type > 0)
	{
		prop = prop_get(prop_index);
		type = _secondary_look_communicated_direction;
		if (information->look_type == _ai_information_look_unit &&
			information->look_data.unit.unit_index == prop->unit_index)
		{
			type = _secondary_look_communicating_prop;
		}

		switch (information->look_type)
		{
		case _ai_information_look_unit:
			ai_communication_look_secondary_at_unit(
				actor_index,
				type,
				information->look_priority,
				information->look_data.unit.unit_index,
				NONE);
			break;

		case _ai_information_look_object:
			ai_communication_look_secondary_at_object(
				actor_index,
				type,
				information->look_priority,
				information->look_data.object.object_index);
			break;
		}
	}

	return;
}

static void ai_communication_update_speech_timers(
	long unit_index,
	short priority,
	short vocalization_type,
	short dialogue_type_index,
	short reply_table_index)
{
	struct unit_datum *unit = unit_get(unit_index);
	struct actor_datum *actor;
	long time;
	long notification_time;

	if (unit->unit.actor_index == NONE)
	{
		actor = NULL;
	}
	else
	{
		actor = actor_get(unit->unit.actor_index);
	}

	time = game_time_get();
	notification_time = time +
		MAX(0, unit->unit.speech.sound_timer - communication_overlap_time_modifier);
	unit->unit.speech.last_speech_finished_time = notification_time;

	if (actor)
	{
		short communication_team;

		actor_reset_idle_vocalization_timer(unit->unit.actor_index);
		communication_team = actor_communication_team(unit->unit.actor_index);
		if (communication_team != NONE)
		{
			if (priority <= _unit_speech_shout)
			{
				ai_globals->last_chatter_time[communication_team] = MAX(
					ai_globals->last_chatter_time[communication_team],
					notification_time);
				if (priority >= _unit_speech_talk)
				{
					ai_globals->last_talk_time[communication_team] = MAX(
						ai_globals->last_talk_time[communication_team],
						notification_time);
				}
				if (priority >= _unit_speech_shout)
				{
					ai_globals->last_shout_time[communication_team] = MAX(
						ai_globals->last_shout_time[communication_team],
						notification_time);
				}

				if (ai_debug.print_speech_timers)
				{
					char const *timer_name = priority >= _unit_speech_talk ? "talk" : "chatter";
					char const *team_name =
						global_communication_team_names[communication_team * 2];

					error(
						2,
						"%s %s %d/%s: %s %d",
						team_name,
						unit_get_speech_priority_name(priority),
						dialogue_type_index,
						dialogue_get_vocalization_name(vocalization_type, TRUE),
						timer_name,
						notification_time - time);
				}
			}

			if (dialogue_type_index != NONE)
			{
				struct dialogue_event_status *event;
				struct dialogue_usage const *usage;

				match_assert(
					"c:\\halo\\SOURCE\\ai\\ai_communication.c",
					0xC9C,
					(dialogue_type_index >= 0) && (dialogue_type_index < global_dialogue_event_count));
				usage = &global_dialogue_table[dialogue_type_index];
				event = &global_dialogue_events[
					dialogue_type_index * NUMBER_OF_AI_COMMUNICATION_TEAMS + communication_team];
				event->last_time_spoken = time;
				if (game_connection() ||
					!ai_debug.communication_timeout_disabled)
				{
					if (usage->repeat_delay > 0.0f)
					{
						event->disable_until_time = (long)(
							usage->repeat_delay * TICKS_PER_SECOND +
							notification_time);
					}
				}
			}

			if (reply_table_index != NONE)
			{
				struct dialogue_event_status *event;
				struct reply_usage const *usage;

				match_assert(
					"c:\\halo\\SOURCE\\ai\\ai_communication.c",
					0xCAF,
					(reply_table_index >= 0) && (reply_table_index < global_reply_event_count));
				usage = &global_reply_table[reply_table_index];
				event = &global_reply_events[
					reply_table_index * NUMBER_OF_AI_COMMUNICATION_TEAMS + communication_team];
				event->last_time_spoken = time;
				if (game_connection() ||
					!ai_debug.communication_timeout_disabled)
				{
					if (usage->repeat_delay > 0.0f)
					{
						event->disable_until_time = (long)(
							usage->repeat_delay * TICKS_PER_SECOND +
							notification_time);
					}
				}
			}
		}
	}

	return;
}

static boolean ai_conversation_find_participant(
	long conversation_index,
	short participant_index,
	boolean *found_specific_unit_reference,
	boolean *try_alternate_reference,
	boolean *success_with_better_player_rating_reference,
	real *best_distance_reference)
{
	struct ai_conversation_driver_datum_view *conversation =
		ai_conversation_driver_get(conversation_index);
	struct scenario_conversation_definition_view *conversation_definition =
		TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_conversations,
			conversation->header.scenario_conversation_index,
			struct scenario_conversation_definition_view);
	struct scenario_conversation_participant_view *participant =
		TAG_BLOCK_GET_ELEMENT(
			&conversation_definition->participants,
			participant_index,
			struct scenario_conversation_participant_view);
	short selected_variant_index = NONE;
	long selected_actor_index = NONE;
	real best_player_distance = REAL_MAX;
	boolean better_player_rating_found = FALSE;
	boolean participant_found = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\ai_communication.c",
		0x127A,
		conversation_definition && conversation);

	if (participant->selection_type == _ai_conversation_selection_disembodied)
	{
		selected_variant_index = 0;
		participant_found = TRUE;
	}
	else
	{
		struct actor_iterator actor_iterator;
		struct ai_script_actor_reference_iterator ai_actor_iterator;
		real_point3d nearby_unit_positions[MAXIMUM_PARTICIPANTS_PER_CONVERSATION];
		short rejection_counts[7];
		boolean player_selection = FALSE;
		boolean use_specific_object = FALSE;
		boolean use_ai_iterator = FALSE;
		boolean first_participant;
		long candidate_object_index = NONE;
		real best_rating = 0.0f;
		short possible_actor_count = 0;
		short nearby_unit_count;
		short slot_index;

		csmemset(rejection_counts, 0, sizeof(rejection_counts));
		if (participant->selection_type == _ai_conversation_selection_radio ||
			participant->selection_type == _ai_conversation_selection_radio_sargeant)
		{
			player_selection = TRUE;
		}

		nearby_unit_count = 0;
		for (slot_index = 0;
			slot_index < conversation_definition->participants.count;
			slot_index = (short)(slot_index + 1))
		{
			long actor_index = conversation->actor_indices[slot_index];

			if (actor_index != NONE)
			{
				struct actor_datum *actor = actor_get(actor_index);

				match_assert(
					"c:\\halo\\SOURCE\\ai\\ai_communication.c",
					0x12AD,
					nearby_unit_count < MAXIMUM_PARTICIPANTS_PER_CONVERSATION);
				nearby_unit_positions[nearby_unit_count] =
					actor->input.position.body_position;
				nearby_unit_count = (short)(nearby_unit_count + 1);
			}
		}

		first_participant = nearby_unit_count == 0;
		if (participant->preexisting_object_name_index != NONE)
		{
			candidate_object_index = object_index_from_name_index(
				participant->preexisting_object_name_index);
			use_specific_object = TRUE;
		}
		else if (participant->runtime_ai_index != NONE)
		{
			ai_index_actor_iterator_new(
				participant->runtime_ai_index,
				&ai_actor_iterator);
			use_ai_iterator = TRUE;
		}
		else
		{
			actor_iterator_new(&actor_iterator, TRUE);
		}

		while (TRUE)
		{
			real candidate_rating = 0.0f;
			real candidate_player_distance = REAL_MAX;
			struct unit_datum *player_target_unit = NULL;
			real player_rating;
			boolean selection_valid = TRUE;
			struct actor_datum *candidate_actor;
			long candidate_actor_index;
			long player_target_unit_index;
			short actor_variant_index;

			if (use_specific_object)
			{
				struct unit_datum *candidate_unit = unit_try_and_get(
					candidate_object_index);

				candidate_actor_index = NONE;
				candidate_actor = NULL;
				if (candidate_unit && candidate_unit->unit.actor_index != NONE)
				{
					candidate_actor_index = candidate_unit->unit.actor_index;
					candidate_actor = actor_get(candidate_actor_index);
				}
				candidate_object_index = NONE;
			}
			else if (use_ai_iterator)
			{
				candidate_actor = ai_index_actor_iterator_next(&ai_actor_iterator);
				candidate_actor_index = ai_actor_iterator.actor_index;
			}
			else
			{
				candidate_actor = actor_iterator_next(&actor_iterator);
				candidate_actor_index = actor_iterator.index;
			}

			if (candidate_actor == NULL)
			{
				break;
			}

			possible_actor_count = (short)(possible_actor_count + 1);
			if (candidate_actor->meta.unit_index == NONE)
			{
				rejection_counts[0]++;
				continue;
			}
			if (candidate_actor->meta.type != participant->actor_type)
			{
				rejection_counts[1]++;
				continue;
			}

			for (slot_index = 0;
				slot_index < conversation_definition->participants.count;
				slot_index = (short)(slot_index + 1))
			{
				if (candidate_actor_index == conversation->actor_indices[slot_index])
				{
					break;
				}
			}
			if (slot_index < conversation_definition->participants.count)
			{
				rejection_counts[2]++;
				continue;
			}

			player_rating = ai_communication_get_player_rating(
				candidate_actor->meta.unit_index,
				first_participant,
				&player_target_unit_index,
				&candidate_player_distance);
			if (player_target_unit_index == NONE)
			{
				if (!player_selection)
				{
					rejection_counts[3]++;
					better_player_rating_found = TRUE;
					continue;
				}
			}
			else
			{
				candidate_rating = player_rating;
				player_target_unit = unit_get(player_target_unit_index);
			}

			switch (participant->selection_type)
			{
			case _ai_conversation_selection_friendly_actor:
			case _ai_conversation_selection_radio:
				if (player_target_unit &&
					game_team_is_enemy(
						candidate_actor->meta.team_index,
						player_target_unit->object.owner_team_index))
				{
					selection_valid = FALSE;
				}
				break;

			case _ai_conversation_selection_in_player_vehicle:
				if (!player_target_unit ||
					player_target_unit->object.parent_object_index == NONE ||
					candidate_actor->input.vehicle_index !=
						player_target_unit->object.parent_object_index)
				{
					selection_valid = FALSE;
				}
				else if (candidate_actor->input.vehicle_gunner)
				{
					candidate_rating += 1.0f;
				}
				break;

			case _ai_conversation_selection_not_in_vehicle:
				if (candidate_actor->input.vehicle_index != NONE)
				{
					selection_valid = FALSE;
				}
				break;

			case _ai_conversation_selection_sargeant:
			case _ai_conversation_selection_radio_sargeant:
				if (candidate_actor->meta.unique_leader)
				{
					candidate_rating += 1.5f;
				}
				break;

			case _ai_conversation_selection_disembodied:
			case _ai_conversation_selection_any_actor:
			default:
				break;
			}

			if (!selection_valid)
			{
				rejection_counts[4]++;
				continue;
			}

			if (first_participant &&
				!player_selection &&
				player_rating < 2.0f &&
				conversation_definition->run_to_player_distance == 0.0f)
			{
				rejection_counts[5]++;
				better_player_rating_found = TRUE;
				continue;
			}

			if (nearby_unit_count > 0)
			{
				real nearest_distance_squared = REAL_MAX;

				for (slot_index = 0;
					slot_index < nearby_unit_count;
					slot_index = (short)(slot_index + 1))
				{
					real distance_squared = distance_squared3d(
						&candidate_actor->input.position.body_position,
						&nearby_unit_positions[slot_index]);

					if (nearest_distance_squared > distance_squared)
					{
						nearest_distance_squared = distance_squared;
					}
				}

				if (nearest_distance_squared < 20.25f)
				{
					candidate_rating += 1.0f -
						(square_root(nearest_distance_squared) - 1.5f) *
						0.33333334f;
				}
			}

			{
				struct unit_datum *candidate_unit = unit_get(
					candidate_actor->meta.unit_index);
				short change_variant_indices[
					MAXIMUM_DIALOGUE_VARIANTS_PER_CONVERSATION_PARTICIPANT];
				short unit_variant_index = candidate_unit->object.variant_number;
				short change_variant_indices_count = 0;
				short found_variant_index = NONE;
				boolean found_variant = FALSE;
				short variant_index;

				for (variant_index = 0;
					variant_index <
						MAXIMUM_DIALOGUE_VARIANTS_PER_CONVERSATION_PARTICIPANT;
					variant_index = (short)(variant_index + 1))
				{
					short dialogue_variant =
						participant->dialogue_variants[variant_index];

					if (dialogue_variant == NONE)
					{
						continue;
					}
					if (dialogue_variant == unit_variant_index)
					{
						found_variant_index = variant_index;
						found_variant = TRUE;
						break;
					}
					if (dialogue_variant == 0)
					{
						found_variant_index = variant_index;
						found_variant = TRUE;
					}
					else if (unit_variant_index < 100 && dialogue_variant < 100)
					{
						match_assert(
							"c:\\halo\\SOURCE\\ai\\ai_communication.c",
							0x13AF,
							change_variant_indices_count <
								MAXIMUM_DIALOGUE_VARIANTS_PER_CONVERSATION_PARTICIPANT);
						change_variant_indices[change_variant_indices_count++] =
							variant_index;
					}
				}

				if (found_variant)
				{
					match_assert(
						"c:\\halo\\SOURCE\\ai\\ai_communication.c",
						0x13B8,
						found_variant_index != NONE);
					actor_variant_index = found_variant_index;
					candidate_rating += 0.7f;
				}
				else if (change_variant_indices_count > 0)
				{
					if (change_variant_indices_count == 1)
					{
						actor_variant_index = change_variant_indices[0];
					}
					else
					{
						actor_variant_index = change_variant_indices[
							seed_random_range(
								get_global_random_seed_address(),
								0,
								change_variant_indices_count)];
					}
					match_assert(
						"c:\\halo\\SOURCE\\ai\\ai_communication.c",
						0x13C7,
						(actor_variant_index >= 0) &&
						(actor_variant_index <
							MAXIMUM_DIALOGUE_VARIANTS_PER_CONVERSATION_PARTICIPANT));
				}
				else
				{
					rejection_counts[6]++;
					continue;
				}
			}

			if (candidate_rating > best_rating)
			{
				selected_actor_index = candidate_actor_index;
				best_rating = candidate_rating;
				best_player_distance = candidate_player_distance;
				selected_variant_index = actor_variant_index;
				participant_found = TRUE;
			}
		}

		if (!participant_found && ai_print_conversations)
		{
			char ai_name[256];
			char const *object_name = "none";
			short name_index = participant->preexisting_object_name_index != NONE ?
				participant->preexisting_object_name_index :
				participant->new_attach_object_name_index;

			if (VALID_INDEX(name_index, global_scenario_get()->object_names.count))
			{
				object_name = TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->object_names,
					name_index,
					struct scenario_object_name)->name;
			}

			if (use_specific_object)
			{
				csstrcpy(ai_name, "<specific unit>");
			}
			else if (use_ai_iterator)
			{
				ai_index_to_string(
					participant->runtime_ai_index,
					global_scenario_get(),
					ai_name,
					sizeof(ai_name));
			}
			else
			{
				csstrcpy(ai_name, "<everyone>");
			}

			console_printf(
				FALSE,
				"%s: didn't find %d/%s in %s (%d possible actors)",
				conversation_definition->name,
				participant_index,
				object_name,
				ai_name,
				possible_actor_count);

			if (possible_actor_count > 0)
			{
				char reason_string[512];
				char const *reason_names[7] =
				{
					"swarm",
					"wrong-type",
					"already-conversing",
					"nowhere-near-player",
					"selection",
					"not-near-player",
					"no-dialogue-match",
				};

				csstrcpy(reason_string, "  reasons: ");
				for (slot_index = 0; slot_index < 7; slot_index++)
				{
					if (rejection_counts[slot_index] > 0)
					{
						sprintf(
							&reason_string[csstrlen(reason_string)],
							"%s(%d) ",
							reason_names[slot_index],
							rejection_counts[slot_index]);
					}
				}
				console_printf(FALSE, reason_string);
			}
		}
	}

	if (participant_found)
	{
		SET_FLAG(conversation->participant_bitmask, participant_index, TRUE);
		conversation->actor_indices[participant_index] = selected_actor_index;
		conversation->dialogue_indices[participant_index] = selected_variant_index;
		if (found_specific_unit_reference && selected_actor_index != NONE)
		{
			*found_specific_unit_reference = TRUE;
		}
	}
	else if (TEST_FLAG(
		participant->flags,
		_ai_conversation_participant_has_alternate_bit) &&
		try_alternate_reference)
	{
		*try_alternate_reference = TRUE;
	}

	if (better_player_rating_found &&
		success_with_better_player_rating_reference)
	{
		*success_with_better_player_rating_reference = TRUE;
	}
	if (best_distance_reference &&
		*best_distance_reference > best_player_distance)
	{
		*best_distance_reference = best_player_distance;
	}

	return participant_found;
}

void ai_communication_started(
	long unit_index,
	short priority,
	short vocalization_type,
	struct ai_information_packet *information)
{
	switch (priority)
	{
		case _unit_speech_none:
		case _unit_speech_idle:
		case _unit_speech_pain:
		case _unit_speech_involuntary:
		case _unit_speech_death:
			break;

		default:
			if (ai_debug.print_vocalizations)
			{
				char string[1024];
				char name[256];
				struct unit_datum *unit = unit_get(unit_index);

				if (unit->unit.actor_index != NONE)
				{
					struct actor_datum *actor = actor_get(unit->unit.actor_index);

					if (actor->meta.encounter_index == NONE)
					{
						csstrcpy(name, "<no encounter>");
					}
					else
					{
						struct encounter_definition *encounter = TAG_BLOCK_GET_ELEMENT(
							&global_scenario_get()->ai_encounters,
							DATUM_INDEX_TO_ABSOLUTE_INDEX(actor->meta.encounter_index),
							struct encounter_definition);

						sprintf(
							name,
							"%s/%s",
							encounter->name,
							TAG_BLOCK_GET_ELEMENT(
								&encounter->squads,
								actor->meta.squad_index,
								struct squad_definition)->name);
					}

					sprintf(
						string,
						"%s/%s: ",
						name,
						actor_type_get_name(actor->meta.type));
				}
				else if (unit->object.name_index != NONE)
				{
					sprintf(
						string,
						"%s: ",
						TAG_BLOCK_GET_ELEMENT(
							&global_scenario_get()->object_names,
							unit->object.name_index,
							struct scenario_object_name)->name);
				}
				else if (unit->unit.player_index != NONE)
				{
					sprintf(
						string,
						"player %d: ",
						DATUM_INDEX_TO_ABSOLUTE_INDEX(unit->unit.player_index));
				}
				else
				{
					sprintf(
						string,
						"unit %04X: ",
						DATUM_INDEX_TO_ABSOLUTE_INDEX(unit_index));
				}

				sprintf(
					temporary,
					"%s %s",
					unit_get_speech_priority_name(priority),
					vocalization_type == NONE ?
						"non-voc" :
						dialogue_get_vocalization_name(vocalization_type, FALSE));
				csstrcat(string, temporary);

				if (information &&
					information->dialogue_type_index != NONE)
				{
					sprintf(
						temporary,
						" [%d/%s]",
						information->dialogue_type_index,
						ai_communication_get_type_name(
							global_dialogue_table[information->dialogue_type_index].communication_type));
					csstrcat(string, temporary);
				}

				console_printf(FALSE, string);
			}

			if (ai_debug.print_speech)
			{
				struct unit_datum *unit = unit_get(unit_index);

				if (unit->unit.speech.current.priority > _unit_speech_none)
				{
					char actor_string[512];
					char speech_string[512];

					error(
						2,
						"%s: %s",
						ai_debug_describe_actor(
							unit->unit.actor_index,
							unit_index,
							FALSE,
							actor_string,
							sizeof(actor_string)),
						unit_describe_speech(
							unit_index,
							TRUE,
							sizeof(speech_string),
							speech_string));
				}
			}

			if (!information->updated_dialogue_timers)
			{
				ai_communication_update_speech_timers(
					unit_index,
					priority,
					vocalization_type,
					information->dialogue_type_index,
					NONE);
			}
			break;
	}

	return;
}

void ai_communication_notify(
	long unit_index,
	short priority,
	short vocalization_type,
	struct ai_information_packet *ai_information)
{
	long sound_volume;
	struct unit_datum *unit;
	struct location const *sound_location;
	short speaker_team;
	long parent_object_index;
	real_point3d speaker_head;
	struct actor_iterator actors;
	struct actor_datum *actor;
	long prop_index;
	struct prop_datum *prop;
	struct actor_position_data sense_position;

	switch (ai_information->information_type)
	{
	case _ai_information_allegiance:
		ai_handle_allegiance_broken_notification(
			ai_information->information_data.allegiance.team1_index,
			ai_information->information_data.allegiance.team2_index,
			ai_information->information_data.allegiance.broken);
		break;
	}

	if (ai_information->information_type != 0 ||
		ai_information->look_priority > 0)
	{
		unit = unit_get(unit_index);
		sound_location = &unit->object.location;
		speaker_team = unit->object.owner_team_index;
		sound_volume = _ai_sound_volume_medium;
		unit_get_head_position(unit_index, &speaker_head);

		if (ai_information->dialogue_type_index != NONE)
		{
			match_assert(
				"c:\\halo\\SOURCE\\ai\\ai_communication.c",
				0x894,
				(ai_information->dialogue_type_index >= 0) && (ai_information->dialogue_type_index < global_dialogue_event_count));
			if (global_dialogue_table[ai_information->dialogue_type_index].communication_priority >=
				_ai_communication_priority_communicate)
			{
				sound_volume = _ai_sound_volume_shout;
			}
		}

		if (unit->object.parent_object_index != NONE)
		{
			parent_object_index = object_get_ultimate_parent(unit_index);
			sound_location = &object_get(parent_object_index)->object.location;
		}

		actor_iterator_new(&actors, TRUE);
		actor = actor_iterator_next(&actors);
		while (actor)
		{
			if (actor->meta.unit_index != unit_index &&
				!game_team_is_enemy(actor->meta.team_index, speaker_team) &&
				!(distance_squared3d(
					&actor->input.position.head_position,
					&speaker_head) > 900.0f))
			{
				prop_index = prop_get_base_by_unit_index(
					actors.index,
					unit_index,
					TRUE,
					TRUE);
				if (prop_index != NONE)
				{
					prop = prop_get(prop_index);
					actor_perception_find_sense_position(
						actors.index,
						&speaker_head,
						prop_index,
						&sense_position);
					if (actor_audibility_at_point(
						actors.index,
						&sense_position,
						&speaker_head,
						sound_location,
						(short)sound_volume,
						1.0f,
						prop->line_of_sight) >= _ai_sound_volume_loud)
					{
						actor_handle_communication(
							actors.index,
							prop_index,
							ai_information);
						ai_communication_handle_received_looking(
							actors.index,
							prop_index,
							ai_information);
					}
				}
			}

			actor = actor_iterator_next(&actors);
		}
	}

	return;
}

static boolean ai_conversation_begin(
	long conversation_index,
	boolean *continue_trying)
{
	struct ai_conversation_driver_datum_view *conversation =
		ai_conversation_driver_get(conversation_index);
	struct scenario_conversation_definition_view *definition =
		TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_conversations,
			conversation->header.scenario_conversation_index,
			struct scenario_conversation_definition_view);
	unsigned long better_player_rating_mask = 0;
	boolean try_alternate = FALSE;
	boolean found_alternate = FALSE;
	boolean found_specific_unit = FALSE;
	boolean can_begin = TRUE;
	boolean keep_trying = FALSE;
	real best_distance = REAL_MAX;
	boolean participant_not_ready;
	boolean participant_missing;
	short index;

	conversation->participant_bitmask = 0;
	csmemset(conversation->actor_indices, NONE, sizeof(conversation->actor_indices));
	csmemset(conversation->dialogue_indices, NONE, sizeof(conversation->dialogue_indices));

	for (index = 0;
		index < definition->participants.count;
		index = (short)(index + 1))
	{
		struct scenario_conversation_participant_view *participant =
			TAG_BLOCK_GET_ELEMENT(
				&definition->participants,
				index,
				struct scenario_conversation_participant_view);

		if (!TEST_FLAG(
			participant->flags,
			_ai_conversation_participant_is_alternate_bit))
		{
			boolean better_player_rating = FALSE;

			ai_conversation_find_participant(
				conversation_index,
				index,
				&found_specific_unit,
				&try_alternate,
				&better_player_rating,
				&best_distance);
			SET_FLAG(
				better_player_rating_mask,
				index,
				better_player_rating);
		}
	}

	if (try_alternate)
	{
		for (index = 0;
			index < definition->participants.count;
			index = (short)(index + 1))
		{
			struct scenario_conversation_participant_view *participant =
				TAG_BLOCK_GET_ELEMENT(
					&definition->participants,
					index,
					struct scenario_conversation_participant_view);

			if (!TEST_FLAG(conversation->participant_bitmask, index) &&
				TEST_FLAG(
					participant->flags,
					_ai_conversation_participant_is_alternate_bit))
			{
				boolean better_player_rating = FALSE;

				if (ai_conversation_find_participant(
					conversation_index,
					index,
					&found_specific_unit,
					NULL,
					&better_player_rating,
					&best_distance))
				{
					found_alternate = TRUE;
				}
				else
				{
					SET_FLAG(
						better_player_rating_mask,
						index,
						better_player_rating);
				}
			}
		}
	}

	participant_not_ready = FALSE;
	participant_missing = FALSE;
	for (index = 0;
		index < definition->participants.count;
		index = (short)(index + 1))
	{
		struct scenario_conversation_participant_view *participant =
			TAG_BLOCK_GET_ELEMENT(
				&definition->participants,
				index,
				struct scenario_conversation_participant_view);

		if (!TEST_FLAG(
				participant->flags,
				_ai_conversation_participant_optional_bit) &&
			!TEST_FLAG(conversation->participant_bitmask, index) &&
			(!TEST_FLAG(
				participant->flags,
				_ai_conversation_participant_has_alternate_bit) ||
				!found_alternate) &&
			(!TEST_FLAG(
				participant->flags,
				_ai_conversation_participant_is_alternate_bit) ||
				try_alternate))
		{
			if (TEST_FLAG(better_player_rating_mask, index))
			{
				participant_not_ready = TRUE;
			}
			else
			{
				participant_missing = TRUE;
			}

			if (ai_print_conversations)
			{
				char const *object_name = "none";
				short name_index = participant->preexisting_object_name_index != NONE ?
					participant->preexisting_object_name_index :
					participant->new_attach_object_name_index;

				if (VALID_INDEX(name_index, global_scenario_get()->object_names.count))
				{
					object_name = TAG_BLOCK_GET_ELEMENT(
						&global_scenario_get()->object_names,
						name_index,
						struct scenario_object_name)->name;
				}

				if (TEST_FLAG(better_player_rating_mask, index))
				{
					console_printf(
						FALSE,
						"%s: found participant %d/%s but not ready to talk yet",
						definition->name,
						index,
						object_name);
				}
				else
				{
					console_printf(
						FALSE,
						"%s: could not find participant %d/%s",
						definition->name,
						index,
						object_name);
				}
			}
			break;
		}
	}

	if (participant_missing)
	{
		can_begin = FALSE;
	}
	else if (participant_not_ready)
	{
		keep_trying = TRUE;
		can_begin = FALSE;
	}

	if (can_begin &&
		TEST_FLAG(definition->flags, _ai_conversation_keep_trying_to_play_bit) &&
		definition->trigger_distance > 0.0f &&
		found_specific_unit &&
		best_distance > definition->trigger_distance)
	{
		if (ai_print_conversations)
		{
			console_printf(
				FALSE,
				"%s: participants currently outside trigger-dist %.1f > %.1f, must wait",
				definition->name,
				best_distance,
				definition->trigger_distance);
		}
		keep_trying = TRUE;
		can_begin = FALSE;
	}

	conversation->header.triggering_player_unit_index = NONE;
	if (can_begin &&
		TEST_FLAG(definition->flags, _ai_conversation_player_must_be_visible_bit))
	{
		if (!found_specific_unit)
		{
			can_begin = FALSE;
		}
		else
		{
			struct data_iterator player_iterator;
			struct player_datum *player;
			real nearest_player_distance = REAL_MAX;

			data_iterator_new(&player_iterator, player_data);
			while ((player = (struct player_datum *)data_iterator_next(
				&player_iterator)) != NULL)
			{
				if (player->unit_index != NONE)
				{
					real nearest_prop_distance = REAL_MAX;

					for (index = 0;
						index < definition->participants.count;
						index = (short)(index + 1))
					{
						long actor_index = conversation->actor_indices[index];

						if (actor_index != NONE)
						{
							long prop_index = prop_get_active_by_unit_index(
								actor_index,
								player->unit_index);

							if (prop_index != NONE)
							{
								struct prop_datum *prop = prop_get(prop_index);

								if (prop->state >= _prop_state_becoming_unacknowledged &&
									prop->state <= _prop_state_acknowledged &&
									nearest_prop_distance > prop->distance)
								{
									nearest_prop_distance = prop->distance;
								}
							}
						}
					}

					if (nearest_prop_distance < nearest_player_distance)
					{
						conversation->header.triggering_player_unit_index =
							player->unit_index;
						nearest_player_distance = nearest_prop_distance;
					}
				}
			}

			if (conversation->header.triggering_player_unit_index == NONE)
			{
				if (ai_print_conversations)
				{
					console_printf(
						FALSE,
						"%s: cannot start, nobody can see player",
						definition->name);
				}
				can_begin = FALSE;
				if (TEST_FLAG(
					definition->flags,
					_ai_conversation_keep_trying_to_play_bit))
				{
					keep_trying = TRUE;
				}
			}
		}
	}

	if (can_begin &&
		TEST_FLAG(
			definition->flags,
			_ai_conversation_player_must_be_looking_at_bit) &&
		found_specific_unit)
	{
		struct data_iterator player_iterator;
		struct player_datum *player;
		boolean player_can_see_participant = FALSE;

		data_iterator_new(&player_iterator, player_data);
		while ((player = (struct player_datum *)data_iterator_next(
				&player_iterator)) != NULL &&
			!player_can_see_participant)
		{
			if (player->unit_index != NONE)
			{
				for (index = 0;
					index < definition->participants.count;
					index = (short)(index + 1))
				{
					long actor_index = conversation->actor_indices[index];

					if (actor_index != NONE)
					{
						struct actor_datum *actor = actor_get(actor_index);

						if (unit_can_see_point(
							player->unit_index,
							&actor->input.position.head_position,
							0.52359879f))
						{
							player_can_see_participant = TRUE;
							break;
						}
					}
				}
			}
		}

		if (!player_can_see_participant)
		{
			if (ai_print_conversations)
			{
				console_printf(
					FALSE,
					"%s: cannot start, players are not looking at us",
					definition->name);
			}
			can_begin = FALSE;
			if (TEST_FLAG(
				definition->flags,
				_ai_conversation_keep_trying_to_play_bit))
			{
				keep_trying = TRUE;
			}
		}
	}

	if (can_begin)
	{
		for (index = 0;
			index < definition->participants.count;
			index = (short)(index + 1))
		{
			if (TEST_FLAG(conversation->participant_bitmask, index) &&
				conversation->actor_indices[index] != NONE)
			{
				struct scenario_conversation_participant_view *participant =
					TAG_BLOCK_GET_ELEMENT(
						&definition->participants,
						index,
						struct scenario_conversation_participant_view);
				struct actor_datum *actor = actor_get(conversation->actor_indices[index]);
				struct unit_datum *unit = unit_get(actor->meta.unit_index);
				short attach_name_index = participant->new_attach_object_name_index;
				short dialogue_variant;

				if (attach_name_index != NONE)
				{
					object_set_object_index_for_name_index(
						attach_name_index,
						actor->meta.unit_index);
				}

				if (TEST_FLAG(
					definition->flags,
					_ai_conversation_stop_other_actions_bit))
				{
					struct action_state_data action_data;

					if (action_converse_setup(
						conversation->actor_indices[index],
						conversation_index,
						&action_data.converse))
					{
						actor_action_change(
							conversation->actor_indices[index],
							_actor_action_converse,
							&action_data);
					}
				}

				match_assert(
					"c:\\halo\\SOURCE\\ai\\ai_communication.c",
					0x1216,
					(conversation->dialogue_indices[index] >= 0) &&
					(conversation->dialogue_indices[index] <
						MAXIMUM_DIALOGUE_VARIANTS_PER_CONVERSATION_PARTICIPANT));
				dialogue_variant = participant->dialogue_variants[
					conversation->dialogue_indices[index]];
				if (unit->object.variant_number != dialogue_variant)
				{
					unit->object.variant_number = dialogue_variant;
					SET_FLAG(
						unit->unit.flags,
						_unit_must_set_up_dialogue_bit,
						FALSE);
				}
			}
		}

		conversation->header.begun = TRUE;
	}
	else
	{
		*continue_trying = keep_trying &&
			TEST_FLAG(
				definition->flags,
				_ai_conversation_keep_trying_to_play_bit);
	}

	return can_begin;
}

static long ai_communication_find_actor_to_reply_to_player(
	long unit_index,
	long target_unit_index,
	short vocalization_type,
	short damage_category,
	real *reply_rating_reference)
{
	char debug_string[1024];
	long actor_index = NONE;
	real reply_rating = 1.0f;

	if (ai_globals->dialogue_triggers_enabled && vocalization_type != NONE)
	{
		struct reply_usage const *reply;
		short reply_table_index;
		boolean communication_not_focused = ai_debug.communication_focus_enable;

		if (ai_debug.print_communication)
		{
			sprintf(
				debug_string,
				"PLAYER-REPLY %s ",
				dialogue_get_vocalization_name(vocalization_type, TRUE));
		}
		if (ai_debug.communication_focus_enable &&
			BIT_VECTOR_TEST_FLAG(
				ai_debug.communication_focus_vector,
				vocalization_type))
		{
			communication_not_focused = FALSE;
		}

		reply = global_reply_table;
		reply_table_index = 0;
		do
		{
			if (reply->original_vocalization_type == vocalization_type &&
				(reply->original_damage_category == NONE ||
				reply->original_damage_category == damage_category))
			{
				short speech_priority =
					communication_speech_priorities[reply->communication_priority];

				if (ai_debug.communication_focus_enable &&
					BIT_VECTOR_TEST_FLAG(
						ai_debug.communication_focus_vector,
						reply->vocalization_type))
				{
					communication_not_focused = FALSE;
				}

				if (sound_scripted_dialog_is_playing() &&
					!TEST_FLAG(reply->flags, _dialogue_usage_lookup_bit))
				{
					if (ai_debug.print_communication)
					{
						csstrcat(debug_string, "[scripted-override] ");
					}
				}
				else if (reply->player_chance > 0.0f)
				{
					real random_value = real_seed_random(
						get_global_random_seed_address());

					if ((game_connection() == _game_connection_local &&
						ai_debug.communication_random_disabled) ||
						random_value < reply->player_chance)
					{
						switch (reply->protagonist_type)
						{
						case _comm_protagonist_friend:
						{
							struct unit_datum *unit = unit_get(unit_index);

							actor_index = ai_communication_find_global_actor_to_talk(
								unit->object.owner_team_index,
								_find_actor_mode_friend,
								unit_index,
								NONE,
								9.0f,
								NONE,
								reply->communication_priority,
								speech_priority,
								reply->vocalization_type,
								reply->animation_type,
								0);
						}
							break;

						case _comm_protagonist_target:
						{
							struct unit_datum *target_unit = unit_try_and_get(
								target_unit_index);

							if (target_unit)
							{
								actor_index = target_unit->unit.actor_index;
							}
						}
							break;

						case _comm_protagonist_enemy:
						{
							struct unit_datum *unit = unit_get(unit_index);

							actor_index = ai_communication_find_global_actor_to_talk(
								unit->object.owner_team_index,
								_find_actor_mode_enemy,
								unit_index,
								NONE,
								9.0f,
								NONE,
								reply->communication_priority,
								speech_priority,
								reply->vocalization_type,
								reply->animation_type,
								0);
						}
							break;
						}

						if (actor_index == NONE)
						{
							if (ai_debug.print_communication)
							{
								csstrcat(
									debug_string,
									csprintf(
										temporary,
										"[%s nobody] ",
										dialogue_get_vocalization_name(
											reply->vocalization_type,
											TRUE)));
							}
						}
						else
						{
							short communication_team = actor_communication_team(actor_index);

							if (communication_team != NONE)
							{
								struct dialogue_event_status *event = &global_reply_events[
									reply_table_index * NUMBER_OF_AI_COMMUNICATION_TEAMS +
									communication_team];
								long time = game_time_get();

								if (event->last_time_spoken != NONE)
								{
									reply_rating =
										(real)(time - event->last_time_spoken) * 0.0011111111f;
									if (reply_rating < 0.0f)
									{
										reply_rating = 0.0f;
									}
									else if (reply_rating > 1.0f)
									{
										reply_rating = 1.0f;
									}
								}

								if ((game_connection() != _game_connection_local ||
									!ai_debug.communication_timeout_disabled) &&
									event->disable_until_time != NONE)
								{
									long disable_time = event->disable_until_time - time;

									if (disable_time > 0)
									{
										if (ai_debug.print_communication)
										{
											csstrcat(
												debug_string,
												csprintf(
													temporary,
													"[%s %s-d-dis/%d] ",
													dialogue_get_vocalization_name(
														reply->vocalization_type,
														TRUE),
													global_communication_team_names[
														communication_team * 2 + 1],
													disable_time));
										}
										actor_index = NONE;
									}
								}
							}

							if (actor_index != NONE && ai_debug.print_communication)
							{
								csstrcat(
									debug_string,
									csprintf(
										temporary,
										"[%s found-actor] ",
										dialogue_get_vocalization_name(
											reply->vocalization_type,
											TRUE)));
							}
						}
					}
					else
					{
						if (ai_debug.print_communication)
						{
							csstrcat(
								debug_string,
								csprintf(
									temporary,
									"[%s rand%.2f>%.2f] ",
									dialogue_get_vocalization_name(
										reply->vocalization_type,
										TRUE),
									random_value,
									reply->player_chance));
						}
					}
				}
				else
				{
					if (ai_debug.print_communication)
					{
						csstrcat(
							debug_string,
							csprintf(
								temporary,
								"[%s 0-player-chance] ",
								dialogue_get_vocalization_name(
									reply->vocalization_type,
									TRUE)));
					}
				}

				if (actor_index != NONE)
				{
					break;
				}
			}

			reply++;
			reply_table_index++;
		}
		while (reply->original_vocalization_type != NONE);

		if (ai_debug.print_communication &&
			!communication_not_focused)
		{
			error(2, debug_string);
		}
	}

	if (reply_rating_reference)
	{
		*reply_rating_reference = reply_rating;
	}

	return actor_index;
}

void ai_communication_finished(
	long unit_index,
	short priority,
	short vocalization_type,
	boolean reply_to_player,
	long preselected_reply_actor_index,
	struct ai_information_packet *information)
{
	if (ai_globals->dialogue_triggers_enabled && vocalization_type != NONE)
	{
		char consider_string[512];
		char debug_string[1024];
		struct reply_usage const *reply;
		boolean communication_not_focused = FALSE;
		boolean any_replies = FALSE;
		short reply_table_index;

		if (ai_debug.print_communication)
		{
			sprintf(
				debug_string,
				"REPLY %s: ",
				dialogue_get_vocalization_name(vocalization_type, FALSE));
		}
		if (ai_debug.communication_focus_enable)
		{
			communication_not_focused = !BIT_VECTOR_TEST_FLAG(
				ai_debug.communication_focus_vector,
				vocalization_type);
		}

		for (reply = global_reply_table, reply_table_index = 0;
			reply->original_vocalization_type != NONE;
			reply++, reply_table_index++)
		{
			if (reply->original_vocalization_type == vocalization_type)
			{
				struct unit_datum *unit = unit_get(unit_index);
				struct actor_datum *speaker_actor = unit->unit.actor_index == NONE ?
					NULL :
					actor_get(unit->unit.actor_index);
				long reply_unit_index = NONE;
				short speech_priority =
					communication_speech_priorities[reply->communication_priority];
				any_replies = TRUE;

				if (ai_debug.print_communication)
				{
					csstrcat(
						debug_string,
						csprintf(
							temporary,
							"%s:",
							dialogue_get_vocalization_name(reply->vocalization_type, FALSE)));
				}
				if (ai_debug.communication_focus_enable &&
					BIT_VECTOR_TEST_FLAG(
						ai_debug.communication_focus_vector,
						reply->vocalization_type))
				{
					communication_not_focused = FALSE;
				}

				if (reply->original_damage_category != NONE &&
					reply->original_damage_category != information->damage_category)
				{
					if (ai_debug.print_communication)
					{
						csstrcat(debug_string, "wrong-dmg ");
					}
				}
				else if (sound_scripted_dialog_is_playing() &&
					!TEST_FLAG(reply->flags, _dialogue_usage_lookup_bit))
				{
					if (ai_debug.print_communication)
					{
						csstrcat(debug_string, "override-scripted ");
					}
				}
				else
				{
					if (preselected_reply_actor_index != NONE)
					{
						reply_unit_index =
							actor_get(preselected_reply_actor_index)->meta.unit_index;
					}
					else
					{
						match_assert(
							"c:\\halo\\SOURCE\\ai\\ai_communication.c",
							0x9E5,
							!reply_to_player);
						switch (reply->protagonist_type)
						{
						case _comm_protagonist_target:
						{
							struct unit_datum *target_unit = unit_try_and_get(
								information->target_unit_index);

							if (target_unit)
							{
								reply_unit_index = information->target_unit_index;
							}
						}
							break;

						case _comm_protagonist_friend:
						{
							long reply_actor_index;

							if (speaker_actor && speaker_actor->meta.encounter_index != NONE)
							{
								reply_actor_index = ai_communication_find_specific_actor_to_talk(
									DATUM_INDEX_TO_ABSOLUTE_INDEX(speaker_actor->meta.encounter_index),
									unit_index,
									NONE,
									9.0f,
									NONE,
									reply->communication_priority,
									speech_priority,
									reply->vocalization_type,
									reply->animation_type,
									0);
							}
							else
							{
								reply_actor_index = ai_communication_find_global_actor_to_talk(
									unit->object.owner_team_index,
									_find_actor_mode_friend,
									unit_index,
									NONE,
									9.0f,
									NONE,
									reply->communication_priority,
									speech_priority,
									reply->vocalization_type,
									reply->animation_type,
									0);
							}
							if (reply_actor_index != NONE)
							{
								reply_unit_index = actor_get(reply_actor_index)->meta.unit_index;
							}
						}
							break;

						case _comm_protagonist_enemy:
						{
							long reply_actor_index = ai_communication_find_global_actor_to_talk(
								unit->object.owner_team_index,
								_find_actor_mode_enemy,
								unit_index,
								NONE,
								9.0f,
								NONE,
								reply->communication_priority,
								speech_priority,
								reply->vocalization_type,
								reply->animation_type,
								0);

							if (reply_actor_index != NONE)
							{
								reply_unit_index = actor_get(reply_actor_index)->meta.unit_index;
							}
						}
							break;
						}
					}

					if (reply_unit_index != NONE)
					{
						struct unit_datum *reply_unit = unit_get(reply_unit_index);

						if (reply_unit->unit.player_index == NONE)
						{
							boolean play_reply = reply_to_player;

							if (!play_reply)
							{
								if (reply->chance > 0.0f)
								{
									real random_value = real_seed_random(
										get_global_random_seed_address());

									if ((game_connection() == _game_connection_local &&
										ai_debug.communication_random_disabled) ||
										random_value < reply->chance)
									{
										play_reply = TRUE;
									}
									else
									{
										if (ai_debug.print_communication)
										{
											csstrcat(
												debug_string,
												csprintf(
													temporary,
													"rand %.2f>%.2f ",
													random_value,
													reply->chance));
										}
									}
								}
								else
								{
									if (ai_debug.print_communication)
									{
										csstrcat(debug_string, "0-chance ");
									}
								}
							}

							if (play_reply)
							{
								if (!reply->reply_filter ||
									reply->reply_filter(
										unit_index,
										information,
										reply_unit->unit.actor_index))
								{
									short reply_vocalization_type = reply->vocalization_type;
									long sound_definition_index = NONE;
									real weight = 1.0f;
									short delay_ticks = (short)(long)(reply->delay_time * TICKS_PER_SECOND);
									short play_type = ai_communication_consider_speech(
										reply_unit_index,
										reply->communication_priority,
										speech_priority,
										delay_ticks,
										FALSE,
										FALSE,
										&reply_vocalization_type,
										&weight,
										&sound_definition_index,
										consider_string);

									if (play_type > _unit_play_speech_none)
									{
										struct unit_speech_item speech;

										speech.priority = speech_priority;
										speech.vocalization_type = reply_vocalization_type;
										speech.sound_definition_index = sound_definition_index;
										speech.delay_time = delay_ticks;
										speech.ai_notification_delay = (short)(long)(
											communication_notification_delays[reply->communication_priority] *
											TICKS_PER_SECOND);
										speech.pause_time = 24;
										speech.ai.target_unit_index = unit_index;
										speech.ai.communication_type = NONE;
										speech.ai.dialogue_type_index = NONE;
										speech.ai.damage_category = NONE;
										speech.ai.updated_dialogue_timers = TRUE;
										speech.ai.look_priority = 0;
										speech.ai.look_type = _ai_information_none;
										speech.ai.information_type = _ai_information_none;
										csmemset(&speech.ai.information_data, 0, sizeof(speech.ai.information_data));

										unit_speak(reply_unit_index, play_type, &speech);
										ai_communication_update_speech_timers(
											reply_unit_index,
											speech_priority,
											reply_vocalization_type,
											NONE,
											reply_table_index);
										ai_communication_look_secondary_at_unit(
											reply_unit->unit.actor_index,
											_secondary_look_communicating_prop,
											communication_protagonist_default_look_priorities[
												reply->communication_priority],
											unit_index,
											NONE);

										if (ai_debug.print_communication)
										{
											csstrcat(
												debug_string,
												strupr(csprintf(
													temporary,
													">>%s<<",
													dialogue_get_vocalization_name(speech.vocalization_type, TRUE))));
										}
										break;
									}
									else
									{
										if (ai_debug.print_communication)
										{
											csstrcat(
												debug_string,
												csprintf(
													temporary,
													"u-%s-%s ",
													weight > 0.0f ? "dis" : "n/a",
													consider_string));
										}
									}
								}
								else
								{
									if (ai_debug.print_communication)
									{
										csstrcat(debug_string, "filter ");
									}
								}
							}
							else
							{
								if (ai_debug.print_communication)
								{
									csstrcat(debug_string, "rand-failed ");
								}
							}
						}
						else
						{
							if (ai_debug.print_communication)
							{
								csstrcat(debug_string, "playercant ");
							}
						}
					}
					else
					{
						if (ai_debug.print_communication)
						{
							csstrcat(debug_string, "nobody ");
						}
					}
				}
			}
		}

		if (any_replies &&
			ai_debug.print_communication &&
			!communication_not_focused)
		{
			error(2, debug_string);
		}
	}

	return;
}

void ai_conversation_actor_deleted(
	long actor_index)
{
	struct data_iterator iterator;
	struct ai_conversation_datum_view *conversation;
	struct scenario_conversation_definition_view *definition;
	short participant_index;

	data_iterator_new(&iterator, conversation_data);
	conversation = (struct ai_conversation_datum_view *)
		data_iterator_next(&iterator);
	while (conversation != NULL)
	{
		definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_conversations,
			conversation->scenario_conversation_index,
			struct scenario_conversation_definition_view);
		participant_index = 0;
		while ((long)participant_index < definition->participants.count)
		{
			if (conversation->actor_indices[participant_index] == actor_index)
			{
				if ((definition->flags & FLAG(
					_ai_conversation_stop_if_anyone_dies_bit)) != 0)
				{
					ai_conversation_finish(
						iterator.datum_index,
						FALSE,
						FALSE);
					break;
				}

				conversation->participant_bitmask &= ~FLAG(participant_index);
				conversation->actor_indices[participant_index] = NONE;
				if (conversation->line_participant_index == participant_index)
				{
					conversation->line_advance = TRUE;
				}
			}

			participant_index = (short)(participant_index + 1);
		}

		conversation = (struct ai_conversation_datum_view *)
			data_iterator_next(&iterator);
	}

	return;
}

void ai_conversation_unit_died(
	long unit_index,
	boolean deleted)
{
	struct data_iterator iterator;
	short participant_index;
	struct ai_conversation_datum_view *conversation;
	struct scenario_conversation_definition_view *definition;
	struct actor_datum *actor;
	boolean referenced;

	data_iterator_new(&iterator, conversation_data);
	conversation = (struct ai_conversation_datum_view *)
		data_iterator_next(&iterator);
	if (conversation == NULL)
	{
		return;
	}
	while (TRUE)
	{
		definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_conversations,
			conversation->scenario_conversation_index,
			struct scenario_conversation_definition_view);
		referenced = FALSE;

		if (conversation->line_unit_index == unit_index)
		{
			referenced = TRUE;
			conversation->line_advance = TRUE;
			conversation->line_unit_index = NONE;
		}
		if (conversation->line_address_unit_index == unit_index)
		{
			referenced = TRUE;
			conversation->line_address_unit_index = NONE;
		}
		if (conversation->triggering_player_unit_index == unit_index)
		{
			referenced = TRUE;
			conversation->triggering_player_unit_index = NONE;
		}

		if (deleted ||
			(definition->flags & FLAG(
				_ai_conversation_stop_if_anyone_dies_bit)) != 0)
		{
			participant_index = 0;
			if (participant_index < definition->participants.count)
			{
				do
				{
					if ((conversation->participant_bitmask &
						FLAG(participant_index)) != 0 &&
						conversation->actor_indices[participant_index] != NONE)
					{
						actor = actor_get(
							conversation->actor_indices[participant_index]);
						if (actor->meta.unit_index == unit_index)
						{
							referenced = TRUE;
						}

						if (deleted)
						{
							if (actor->state.action == _actor_action_converse &&
								actor->state.action_data.converse.run_to_unit_index ==
									unit_index)
							{
								actor->state.action_data.converse.run_to_unit_index = NONE;
							}
							if (actor->external_orders.conversation_attention_unit_index ==
								unit_index)
							{
								actor->external_orders.conversation_attention_unit_index = NONE;
							}
						}
					}

					participant_index = (short)(participant_index + 1);
				}
				while (participant_index < definition->participants.count);
			}

			if (referenced)
			{
				break;
			}
		}

		conversation = (struct ai_conversation_datum_view *)
			data_iterator_next(&iterator);
		if (conversation == NULL)
		{
			return;
		}
	}

	if (ai_print_conversations)
	{
		console_printf(
			FALSE,
			"%s: unit died, aborting",
			definition->name);
	}
	ai_conversation_finish(iterator.datum_index, FALSE, FALSE);
	return;
}

long ai_conversation_new(
	short scenario_conversation_index,
	boolean scripted)
{
	struct data_iterator iterator;
	struct ai_conversation_datum_view *conversation;
	long conversation_index = datum_new(conversation_data);

	if (conversation_index == NONE && scripted)
	{
		boolean overwrite_scripted = TRUE;
		long overwrite_creation_time = LONG_MAX;
		long overwrite_conversation_index = NONE;

		data_iterator_new(&iterator, conversation_data);
		while ((conversation = (struct ai_conversation_datum_view *)
			data_iterator_next(&iterator)) != NULL)
		{
			if (conversation->scripted < overwrite_scripted ||
				conversation->creation_time < overwrite_creation_time)
			{
				overwrite_creation_time = conversation->creation_time;
				overwrite_conversation_index = iterator.datum_index;
				overwrite_scripted = conversation->scripted;
			}
		}

		if (overwrite_conversation_index != NONE)
		{
			if (ai_print_conversations)
			{
				struct scenario_conversation_definition_view *definition =
					TAG_BLOCK_GET_ELEMENT(
						&global_scenario_get()->ai_conversations,
						scenario_conversation_index,
						struct scenario_conversation_definition_view);

				console_printf(
					FALSE,
					"%s: this conversation is already running or trying to run, overwrite it",
					definition->name);
			}

			ai_conversation_finish(
				overwrite_conversation_index,
				FALSE,
				FALSE);
			conversation_index = datum_new_at_index(
				conversation_data,
				overwrite_conversation_index);
		}
	}

	if (conversation_index != NONE)
	{
		conversation = (struct ai_conversation_datum_view *)datum_get(
			conversation_data,
			conversation_index);
		conversation->scenario_conversation_index = scenario_conversation_index;
		conversation->line_index = NONE;
		conversation->scripted = scripted;
		conversation->creation_time = game_time_get();
	}

	return conversation_index;
}

static boolean ai_conversation_line_begin(
	long conversation_index)
{
	struct ai_conversation_driver_datum_view *conversation;
	struct scenario_conversation_definition_view *definition;
	struct scenario_conversation_participant_view *participant;
	struct scenario_conversation_line_view *line;
	struct actor_datum *actor;
	short participant_index;
	long speaker_actor_index;
	boolean result;

	conversation = ai_conversation_driver_get(conversation_index);
	definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_conversations,
		conversation->header.scenario_conversation_index,
		struct scenario_conversation_definition_view);
	line = TAG_BLOCK_GET_ELEMENT(
		&definition->lines,
		conversation->current_line,
		struct scenario_conversation_line_view);
	participant_index = line->participant_index;
	result = FALSE;
	if (participant_index >= 0 &&
		participant_index < definition->participants.count &&
		(conversation->participant_bitmask & FLAG(participant_index)) != 0)
	{
		participant = TAG_BLOCK_GET_ELEMENT(
			&definition->participants,
			participant_index,
			struct scenario_conversation_participant_view);
		speaker_actor_index =
			conversation->actor_indices[line->participant_index];
		conversation->line_participant_index = line->participant_index;
		if (speaker_actor_index == NONE)
		{
			conversation->line_actor_index = NONE;
			conversation->line_unit_index = NONE;
			conversation->line_address_unit_index = NONE;
			conversation->line_unspatialized = TRUE;
		}
		else
		{
			actor = actor_get(speaker_actor_index);
			conversation->line_actor_index = speaker_actor_index;
			conversation->line_unit_index = actor->meta.unit_index;
			conversation->line_address_unit_index = NONE;

			switch (line->address_type)
			{
			case _ai_conversation_address_player:
				conversation->line_address_unit_index =
					conversation->header.triggering_player_unit_index;
				break;

			case _ai_conversation_address_participant:
				if (line->address_participant_index >= 0 &&
					line->address_participant_index < definition->participants.count &&
					conversation->actor_indices[line->address_participant_index] != NONE)
				{
					actor = actor_get(
						conversation->actor_indices[line->address_participant_index]);
					conversation->line_address_unit_index = actor->meta.unit_index;
				}
				break;
			}

			conversation->line_unspatialized =
				participant->selection_type == _ai_conversation_selection_radio ||
				participant->selection_type ==
					_ai_conversation_selection_radio_sargeant;
		}

		match_assert(
			"c:\\halo\\SOURCE\\ai\\ai_communication.c",
			0x146B,
			(conversation->dialogue_indices[line->participant_index] >= 0) &&
			(conversation->dialogue_indices[line->participant_index] <
				MAXIMUM_DIALOGUE_VARIANTS_PER_CONVERSATION_PARTICIPANT));
		conversation->line_sound_index = line->dialogue[
			conversation->dialogue_indices[line->participant_index]].index;
		conversation->line_delay_timer = (short)(long)(line->delay_time * 30.0f);
		conversation->line_flags = line->flags;
		conversation->line_advance = FALSE;
		conversation->line_finished = FALSE;
		conversation->line_spoken = FALSE;
		result = TRUE;
	}

	return result;
}

static boolean ai_conversation_line_perform(
	long conversation_index)
{
	struct ai_conversation_driver_datum_view *conversation;
	struct scenario_conversation_definition_view *definition;
	struct actor_datum *actor;
	struct unit_datum *unit;
	long sound_definition_index;
	long speaking_unit_index;
	long speech_sound_definition_index;
	long participant_actor_index;
	short vocalization_type;
	short play_type;
	short participant_index;
	short flags;
	short delay_timer;
	boolean busy;

	conversation = ai_conversation_driver_get(conversation_index);
	definition = TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_conversations,
		conversation->header.scenario_conversation_index,
		struct scenario_conversation_definition_view);
	if (!conversation->line_advance)
	{
		if (!conversation->line_spoken)
		{
			busy = FALSE;
			sound_definition_index = conversation->line_sound_index;
			if (sound_definition_index != NONE)
			{
				flags = conversation->line_flags;
				if ((flags &
					(FLAG(_ai_conversation_line_wait_until_speaker_nearby_bit) |
					FLAG(_ai_conversation_line_wait_until_everyone_nearby_bit))) != 0)
				{
					participant_index = 0;
					while ((long)participant_index < definition->participants.count)
					{
						participant_actor_index =
							conversation->actor_indices[participant_index];
						if (participant_actor_index != NONE)
						{
							actor = actor_get(participant_actor_index);
							flags = conversation->line_flags;
							if (((flags & FLAG(
								_ai_conversation_line_wait_until_everyone_nearby_bit)) != 0 ||
								((flags & FLAG(
									_ai_conversation_line_wait_until_speaker_nearby_bit)) != 0 &&
								participant_actor_index == conversation->line_actor_index)) &&
								actor->state.action == _actor_action_converse &&
								actor->state.action_data.converse.run_to_unit_index != NONE &&
								!actor->state.action_data.converse.in_range &&
								!actor->state.action_data.converse.failed)
							{
								busy = TRUE;
							}
						}

						participant_index = (short)(participant_index + 1);
					}
				}

				if (sound_scripted_dialog_is_playing())
				{
					busy = TRUE;
				}

				if (!busy)
				{
					speaking_unit_index = conversation->line_unit_index;
					if (speaking_unit_index == NONE ||
						conversation->line_unspatialized)
					{
						scripted_sound_new(
							conversation->line_sound_index,
							NONE,
							1.0f);
					}
					else
					{
						speech_sound_definition_index = conversation->line_sound_index;
						vocalization_type = NONE;
						play_type = unit_test_speech(
							speaking_unit_index,
							_ai_communication_priority_yell,
							FALSE,
							TRUE,
							NULL,
							&vocalization_type,
							&speech_sound_definition_index);
						if (play_type == TRUE)
						{
							busy = TRUE;
						}
						else if (play_type > 0)
						{
							struct unit_speech_item speech_item;
							long line_sound_index;

							csmemset(&speech_item, 0, sizeof(speech_item));
							line_sound_index = conversation->line_sound_index;
							speech_item.vocalization_type = NONE;
							speech_item.ai.communication_type = NONE;
							speech_item.ai.damage_category = NONE;
							speech_item.ai.dialogue_type_index = NONE;
							speech_item.ai.target_unit_index =
								conversation->line_address_unit_index;
							speech_item.priority = _ai_communication_priority_yell;
							speech_item.sound_definition_index = line_sound_index;
							speech_item.ai.look_priority = TRUE;
							speech_item.ai.look_type = _ai_information_look_unit;
							speech_item.ai.look_data.unit.unit_index =
								conversation->line_unit_index;
							speech_item.ai.information_type = _ai_information_none;

							if (ai_print_conversations)
							{
								console_printf(
									FALSE,
									"%s: speak %s",
									definition->name,
									tag_get_name(line_sound_index));
							}

							unit_speak(
								conversation->line_unit_index,
								play_type,
								&speech_item);
						}
					}
				}
			}

			if (!busy)
			{
				conversation->line_spoken = TRUE;
				conversation->header.any_line_spoken = TRUE;
			}
		}

		if (conversation->line_spoken)
		{
			if (!conversation->line_finished)
			{
				if (conversation->line_unit_index == NONE)
				{
					sound_definition_index = conversation->line_sound_index;
					conversation->line_finished =
						sound_definition_index == NONE ||
							scripted_sound_time(sound_definition_index) == 0;
				}
				else
				{
					unit = unit_get(conversation->line_unit_index);
					conversation->line_finished =
						unit->unit.speech.current.priority !=
							_ai_communication_priority_yell;
				}
			}

			if (conversation->line_finished)
			{
				delay_timer = conversation->line_delay_timer;
				if (delay_timer > 0)
				{
					conversation->line_delay_timer = (short)(delay_timer - 1);
					return conversation->line_advance;
				}

				conversation->line_advance = TRUE;
				if ((conversation->line_flags & FLAG(
					_ai_conversation_line_wait_after_until_told_to_advance_bit)) != 0)
				{
					if (!conversation->header.waiting_to_advance)
					{
						conversation->header.waiting_to_advance = TRUE;
						conversation->header.told_to_advance = FALSE;
					}

					if (conversation->header.told_to_advance)
					{
						conversation->header.waiting_to_advance = FALSE;
					}
					else
					{
						conversation->line_advance = FALSE;
					}
				}
			}
		}
	}

	return conversation->line_advance;
}

static void ai_conversation_line_end(
	long conversation_index)
{
	struct ai_conversation_driver_datum_view *conversation;

	conversation = ai_conversation_driver_get(conversation_index);
	TAG_BLOCK_GET_ELEMENT(
		&global_scenario_get()->ai_conversations,
		conversation->header.scenario_conversation_index,
		struct scenario_conversation_definition_view);
	return;
}

boolean ai_conversation(
	short scenario_conversation_index,
	boolean scripted)
{
	struct scenario *scenario;
	long conversation_index;
	boolean result;

	scenario = global_scenario_get();
	result = FALSE;
	if (scenario_conversation_index >= 0 &&
		scenario_conversation_index < scenario->ai_conversations.count)
	{
		conversation_index = ai_conversation_new(
			scenario_conversation_index,
			scripted);
		if (ai_print_conversations)
		{
			console_printf(
				FALSE,
				"%s: script tried to start conversation",
				TAG_BLOCK_GET_ELEMENT(
					&global_scenario_get()->ai_conversations,
					scenario_conversation_index,
					struct scenario_conversation_definition_view)->name);
		}

		if (conversation_index == NONE)
		{
			error(
				2,
				"WARNING: too many executing conversations (ran out of MAXIMUM_CONVERSATIONS_PER_MAP %d)",
				128);
		}
		else
		{
			boolean continue_trying;

			continue_trying = FALSE;
			if (ai_conversation_begin(conversation_index, &continue_trying))
			{
				if (ai_print_conversations)
				{
					console_printf(
						FALSE,
						"%s: begun successfully",
						TAG_BLOCK_GET_ELEMENT(
							&global_scenario_get()->ai_conversations,
							scenario_conversation_index,
							struct scenario_conversation_definition_view)->name);
				}

				result = TRUE;
			}
			else if (continue_trying)
			{
				if (ai_print_conversations)
				{
					console_printf(
						FALSE,
						"%s: can't begin yet but will remember and keep trying it",
						TAG_BLOCK_GET_ELEMENT(
							&global_scenario_get()->ai_conversations,
							scenario_conversation_index,
							struct scenario_conversation_definition_view)->name);
				}

				result = TRUE;
			}
			else
			{
				if (ai_print_conversations)
				{
					console_printf(
						FALSE,
						"%s: could not start, and not set to keep trying... aborting (status 5)",
						TAG_BLOCK_GET_ELEMENT(
							&global_scenario_get()->ai_conversations,
							scenario_conversation_index,
							struct scenario_conversation_definition_view)->name);
				}

				ai_conversation_finish(conversation_index, TRUE, FALSE);
			}
		}
	}

	return result;
}

void ai_conversation_update(
	void)
{
	struct data_iterator iterator;
	struct ai_conversation_driver_datum_view *conversation;
	long time;

	time = game_time_get();
	data_iterator_new(&iterator, conversation_data);
	conversation = (struct ai_conversation_driver_datum_view *)data_iterator_next(&iterator);
	while (conversation)
	{
		struct scenario_conversation_definition_view *definition;

		definition = TAG_BLOCK_GET_ELEMENT(
			&global_scenario_get()->ai_conversations,
			conversation->header.scenario_conversation_index,
			struct scenario_conversation_definition_view);
		if (!conversation->header.begun)
		{
			boolean continue_trying;

			continue_trying = TRUE;
			if ((time - conversation->header.creation_time) % 30 == 0)
			{
				if (ai_print_conversations)
				{
					console_printf(
						FALSE,
						"%s: trying to begin",
						definition->name);
				}

				ai_conversation_begin(iterator.datum_index, &continue_trying);
			}

			if (!conversation->header.begun && !continue_trying)
			{
				if (ai_print_conversations)
				{
					console_printf(
						FALSE,
						"%s: unable to begin, and no point in continuing",
						definition->name);
				}

				ai_conversation_finish(iterator.datum_index, TRUE, FALSE);
			}
		}

		if (conversation->header.begun && !conversation->header.finished)
		{
			boolean perform;

			perform = conversation->current_line >= 0 &&
				conversation->current_line < definition->lines.count;
			while (!perform || ai_conversation_line_perform(iterator.datum_index))
			{
				if (perform)
				{
					ai_conversation_line_end(iterator.datum_index);
				}

				conversation->current_line++;
				if (conversation->current_line < definition->lines.count)
				{
					perform = ai_conversation_line_begin(iterator.datum_index);
				}
				else
				{
					if (ai_print_conversations)
					{
						console_printf(
							FALSE,
							"%s: no more lines to play",
							definition->name);
					}

					conversation->header.finished = TRUE;
					break;
				}
			}
		}

		if (conversation->header.finished)
		{
			ai_conversation_finish(iterator.datum_index, FALSE, TRUE);
		}
		else if (conversation->header.begun)
		{
			short participant_index;

			for (participant_index = 0;
				participant_index < definition->participants.count;
				participant_index++)
			{
				if (TEST_FLAG(conversation->participant_bitmask, participant_index) &&
					conversation->actor_indices[participant_index] != NONE)
				{
					struct actor_datum *actor;
					long actor_unit_index;
					long speaker_unit_index;

					actor = actor_get(conversation->actor_indices[participant_index]);
					actor->external_orders.conversation_index = iterator.datum_index;
					actor_unit_index = actor->meta.unit_index;
					actor->external_orders.conversation_attention_unit_index = NONE;
					speaker_unit_index = conversation->line_unit_index;
					if (actor_unit_index == speaker_unit_index)
					{
						actor->external_orders.conversation_attention_unit_index =
							conversation->line_address_unit_index;
					}
					else
					{
						long address_unit_index;

						address_unit_index = conversation->line_address_unit_index;
						if (actor_unit_index == address_unit_index &&
							TEST_FLAG(
								conversation->line_flags,
								_ai_conversation_line_addressee_look_back_bit))
						{
							actor->external_orders.conversation_attention_unit_index =
								speaker_unit_index;
						}
						else if (TEST_FLAG(
							conversation->line_flags,
							_ai_conversation_line_everyone_look_at_speaker_bit))
						{
							actor->external_orders.conversation_attention_unit_index =
								speaker_unit_index;
						}
						else if (TEST_FLAG(
							conversation->line_flags,
							_ai_conversation_line_everyone_look_at_addressee_bit))
						{
							actor->external_orders.conversation_attention_unit_index =
								address_unit_index;
						}
					}
				}
			}
		}

		conversation = (struct ai_conversation_driver_datum_view *)data_iterator_next(&iterator);
	}

	return;
}

void ai_communication_event(
	short communication_type,
	long subject_unit_index,
	long cause_unit_index,
	short hostility,
	short damage_type,
	short information_type,
	struct ai_information_data *information_data)
{
	long event_time = game_time_get();
	char requirements_string[1024];
	char debug_string[1024];
	char team_string[256];
	char timer_string[512];
	char speech_string[512];
	short possibility_count = 0;
	real total_possibility_weight = 0.0f;
	boolean any_forced_possibility = FALSE;
	struct unit_datum *subject_unit = NULL;
	struct unit_datum *cause_unit = NULL;
	long subject_encounter_index = NONE;
	struct encounter_datum *subject_encounter = NULL;
	long subject_actor_index = NONE;
	struct actor_datum *subject_actor = NULL;
	long cause_actor_index = NONE;
	struct actor_datum *cause_actor = NULL;
	long friend_actor_index = NONE;
	long other_actor_index = NONE;
	short subject_team = NONE;
	short cause_team = NONE;
	short subject_race = 0;
	short cause_race = 0;
	boolean find_friend_actor = TRUE;
	boolean find_other_actor = TRUE;
	boolean subject_groups[2];
	boolean cause_groups[2];
	boolean enemy_status[6];
	boolean hostility_matches[NUMBER_OF_AI_COMMUNICATION_HOSTILITIES];
	short chatter_ticks[NUMBER_OF_AI_COMMUNICATION_TEAMS];
	short talk_ticks[NUMBER_OF_AI_COMMUNICATION_TEAMS];
	short shout_ticks[NUMBER_OF_AI_COMMUNICATION_TEAMS];
	real chatter_seconds[NUMBER_OF_AI_COMMUNICATION_TEAMS];
	real talk_seconds[NUMBER_OF_AI_COMMUNICATION_TEAMS];
	real shout_seconds[NUMBER_OF_AI_COMMUNICATION_TEAMS];
	short speech_disabled_reason[NUMBER_OF_AI_COMMUNICATION_TEAMS][NUMBER_OF_COMMUNICATION_PRIORITIES][2];
	short speech_delay[NUMBER_OF_AI_COMMUNICATION_TEAMS][NUMBER_OF_COMMUNICATION_PRIORITIES][2];
	boolean speech_disabled[NUMBER_OF_AI_COMMUNICATION_TEAMS][NUMBER_OF_COMMUNICATION_PRIORITIES][2];
	struct ai_communication_possibility possibilities[MAXIMUM_COMMUNICATION_POSSIBILITIES];
	struct ai_communication_possibility *selected_possibility;
	struct dialogue_usage const *usage;
	short dialogue_index;
	boolean player_involved = FALSE;
	boolean suppress_output = FALSE;
	boolean any_requirements_failed = FALSE;
	boolean any_protagonist_considered = FALSE;
	short team_index;
	short priority;
	short distance_group;

	match_assert(
		"c:\\halo\\SOURCE\\ai\\ai_communication.c",
		0x34E,
		(communication_type >= 0) && (communication_type < NUMBER_OF_AI_COMMUNICATION_TYPES));

	if (hostility == NONE)
	{
		hostility = _comm_hostility_none;
	}
	if (damage_type == NONE)
	{
		damage_type = 0;
	}

	for (distance_group = 0; distance_group < 2; distance_group++)
	{
		subject_groups[distance_group] = FALSE;
		cause_groups[distance_group] = FALSE;
	}

	if (subject_unit_index != NONE)
	{
		subject_unit = unit_get(subject_unit_index);
		subject_actor_index = subject_unit->unit.actor_index;
		subject_team = subject_unit->object.owner_team_index;
		subject_race = (short)ai_get_race_from_team_index(subject_team);

		if (subject_actor_index != NONE)
		{
			subject_actor = actor_get(subject_actor_index);
			subject_encounter_index = subject_actor->meta.encounter_index;
			subject_race = actor_type_get_race(subject_actor->meta.type);
			if (subject_actor->situation.close_friends > 0)
			{
				subject_groups[0] = TRUE;
				subject_groups[1] = TRUE;
			}
			else if (subject_actor->situation.area_friends > 0)
			{
				subject_groups[0] = TRUE;
				subject_groups[1] = FALSE;
			}

			if (subject_encounter_index != NONE)
			{
				subject_encounter = encounter_get(subject_encounter_index);
			}
		}
		else if (subject_unit->unit.player_index != NONE)
		{
			subject_race = _race_player;
		}
	}

	if (cause_unit_index != NONE)
	{
		cause_unit = unit_get(cause_unit_index);
		cause_actor_index = cause_unit->unit.actor_index;
		cause_team = cause_unit->object.owner_team_index;
		cause_race = (short)ai_get_race_from_team_index(cause_team);

		if (cause_actor_index != NONE)
		{
			cause_actor = actor_get(cause_actor_index);
			cause_race = actor_type_get_race(cause_actor->meta.type);
			if (cause_actor->situation.close_friends > 0)
			{
				cause_groups[0] = TRUE;
				cause_groups[1] = TRUE;
			}
			else if (cause_actor->situation.area_friends > 0)
			{
				cause_groups[0] = TRUE;
				cause_groups[1] = FALSE;
			}
		}
		else if (cause_unit->unit.player_index != NONE)
		{
			cause_race = _race_player;
		}
	}

	if (subject_unit && cause_unit &&
		subject_team != cause_team &&
		game_team_is_ally(subject_team, cause_team))
	{
		boolean betrayal = FALSE;
		boolean observed = FALSE;

		if (communication_type == _ai_communication_death)
		{
			if (hostility == _comm_hostility_enemy)
			{
				betrayal = TRUE;
				observed = TRUE;
			}
			else
			{
				if (subject_encounter)
				{
					betrayal =
						subject_encounter->enemy_traitor ||
						subject_encounter->enemy_visible_timer == NONE ||
						subject_encounter->enemy_visible_timer >= 270;
				}

				friend_actor_index = ai_communication_find_global_actor_to_talk(
					subject_team,
					_find_actor_mode_same_team,
					subject_unit_index,
					cause_unit_index,
					18.0f,
					_ai_communication_death,
					_ai_communication_priority_yell,
					NONE,
					NONE,
					NONE,
					0);
				if (friend_actor_index != NONE)
				{
					find_friend_actor = FALSE;
					observed = TRUE;
				}

				switch (damage_type)
				{
				case 3:
				case 4:
				case 9:
					if (!betrayal)
					{
						observed = FALSE;
					}
					break;
				}
				if (damage_type == 3)
				{
					betrayal = FALSE;
				}
			}

			if (betrayal)
			{
				hostility = _comm_hostility_traitor;
			}

			if (ai_debug.print_allegiance)
			{
				console_printf(
					FALSE,
					"incident between teams %s and %s: %s, %s",
					global_game_team_names[subject_team],
					global_game_team_names[cause_team],
					betrayal ? "betrayal" : "accident",
					observed ? "observed" : "unobserved");
			}

			if (observed)
			{
				boolean notify_immediately = FALSE;
				boolean broken = game_allegiance_incident(
					cause_team,
					subject_team,
					betrayal ? _allegiance_incident_betrayal : _allegiance_incident_accident,
					&notify_immediately);

				if (notify_immediately)
				{
					ai_handle_allegiance_broken_notification(
						cause_team,
						subject_team,
						broken);
				}

				if (ai_debug.print_allegiance && !broken)
				{
					short incident_threshold;
					short incidents = game_allegiance_get_incidents(
						cause_team,
						subject_team,
						&incident_threshold);

					console_printf(
						FALSE,
						"allegiance %s, %d incidents (threshold %d)",
						"still holds",
						incidents,
						incident_threshold == NONE ? 999 : incident_threshold);
				}
			}
		}

		if (game_team_is_enemy(subject_team, cause_team))
		{
			hostility = _comm_hostility_traitor;
		}
	}

	if (ai_debug.print_communication)
	{
		char hostility_characters[NUMBER_OF_AI_COMMUNICATION_HOSTILITIES] =
		{
			'n', 's', 'f', 'e', 't'
		};

		sprintf(
			debug_string,
			"%s-%c ",
			global_communication_type_names[communication_type],
			hostility_characters[hostility]);
		strupr(debug_string);
		csstrcpy(requirements_string, "");
	}

	if (!subject_actor)
	{
		short index;

		for (index = 0; index < NUMBEROF(enemy_status); index++)
		{
			enemy_status[index] = TRUE;
		}
	}
	else if (!subject_encounter)
	{
		enemy_status[0] = !subject_actor->target.any_target_ever;
		enemy_status[1] =
			!subject_actor->target.target_really_alive &&
			subject_actor->target.since_any_target_visible_timer != NONE;
		enemy_status[2] =
			subject_actor->target.target_prop_index == NONE ||
			subject_actor->target.since_any_target_visible_timer == NONE ||
			subject_actor->target.since_any_target_visible_timer >= 180;
		enemy_status[3] =
			subject_actor->state.combat_status < _actor_combat_status_definite &&
			(subject_actor->target.since_any_target_visible_timer == NONE ||
			subject_actor->target.since_any_target_visible_timer >= 75) &&
			(subject_actor->target.target_really_alive || subject_actor->state.combat_status > 0);
		enemy_status[4] = subject_actor->state.combat_status < _actor_combat_status_dangerous;
		enemy_status[5] =
			subject_actor->target.target_type >= _actor_target_visible_enemy &&
			subject_actor->target.target_really_alive;
	}
	else
	{
		enemy_status[0] = !subject_actor->target.any_target_ever;
		enemy_status[1] =
			subject_encounter->enemy_visible_timer != NONE &&
			!subject_encounter->enemy_alive;
		enemy_status[2] =
			(subject_encounter->enemy_visible_timer == NONE ||
			subject_encounter->enemy_visible_timer >= 180) &&
			subject_encounter->enemy_alive;
		enemy_status[3] =
			subject_actor->state.combat_status < _actor_combat_status_definite &&
			(subject_encounter->enemy_visible_timer == NONE ||
			subject_encounter->enemy_visible_timer >= 75) &&
			(subject_encounter->enemy_alive || subject_actor->state.combat_status > 0);
		enemy_status[4] =
			subject_actor->state.combat_status < _actor_combat_status_dangerous &&
			(subject_encounter->enemy_visible_timer == NONE ||
			subject_encounter->enemy_visible_timer >= 75);
		enemy_status[5] =
			subject_encounter->enemy_visible &&
			subject_encounter->enemy_alive;
	}

	csmemset(hostility_matches, 0, sizeof(hostility_matches));
	if (hostility != NONE)
	{
		hostility_matches[hostility] = TRUE;
		if (hostility == _comm_hostility_traitor)
		{
			hostility_matches[_comm_hostility_enemy] = TRUE;
		}
	}

	csmemset(speech_disabled_reason, NONE, sizeof(speech_disabled_reason));
	csmemset(speech_disabled, 0, sizeof(speech_disabled));
	csmemset(speech_delay, 0, sizeof(speech_delay));

	for (team_index = 0;
		team_index < NUMBER_OF_AI_COMMUNICATION_TEAMS;
		team_index++)
	{
		shout_ticks[team_index] = (short)MAX(0,
			event_time - ai_globals->last_shout_time[team_index]);
		talk_ticks[team_index] = (short)MAX(0,
			event_time - ai_globals->last_talk_time[team_index]);
		chatter_ticks[team_index] = (short)MAX(0,
			event_time - ai_globals->last_chatter_time[team_index]);
		shout_seconds[team_index] =
			shout_ticks[team_index] * (1.0f / TICKS_PER_SECOND);
		talk_seconds[team_index] =
			talk_ticks[team_index] * (1.0f / TICKS_PER_SECOND);
		chatter_seconds[team_index] =
			chatter_ticks[team_index] * (1.0f / TICKS_PER_SECOND);

		/* nothing may be spoken at priority none */
		speech_disabled[team_index][_ai_communication_priority_none][0] = TRUE;
		speech_disabled[team_index][_ai_communication_priority_none][1] = TRUE;

		for (priority = 1; priority < _ai_communication_priority_yell; priority++)
		{
			for (distance_group = 0; distance_group < 2; distance_group++)
			{
				boolean disabled = FALSE;
				short delay = 0;
				short reason = NONE;

				if (communication_timer_tolerances[priority][distance_group][0] > 0.0f)
				{
					short remaining = (short)(
						communication_timer_tolerances[priority][distance_group][0] * TICKS_PER_SECOND -
						chatter_ticks[team_index]);

					if (remaining > 0)
					{
						reason = 0;
						disabled = TRUE;
						delay = MAX(delay, remaining);
					}
				}
				if (communication_timer_tolerances[priority][distance_group][1] > 0.0f)
				{
					short remaining = (short)(
						communication_timer_tolerances[priority][distance_group][1] * TICKS_PER_SECOND -
						talk_ticks[team_index]);

					if (remaining > 0)
					{
						disabled = TRUE;
						reason = 1;
						delay = MAX(delay, remaining);
					}
				}
				if (communication_timer_tolerances[priority][distance_group][3] > 0.0f)
				{
					short remaining = (short)(
						communication_timer_tolerances[priority][distance_group][3] * TICKS_PER_SECOND -
						shout_ticks[team_index]);

					if (remaining > 0)
					{
						disabled = TRUE;
						reason = 2;
						delay = MAX(delay, remaining);
					}
				}

				/* BUG (original): January reads this slot's stored delay before it
				 * is written below (still zero from the clear), so a positive
				 * minimum tolerance re-enables every disabled slot; comparing
				 * `delay` was probably intended. */
				if (disabled &&
					communication_timer_tolerances[priority][distance_group][4] > 0.0f &&
					speech_delay[team_index][priority][distance_group] <
						communication_timer_tolerances[priority][distance_group][4] * TICKS_PER_SECOND)
				{
					disabled = FALSE;
				}

				speech_disabled[team_index][priority][distance_group] = disabled;
				speech_delay[team_index][priority][distance_group] = delay;
				speech_disabled_reason[team_index][priority][distance_group] = reason;
			}
		}
	}

	if (!ai_globals->dialogue_triggers_enabled)
	{
		if (ai_debug.print_communication)
		{
			csstrcat(debug_string, "DISABLED");
			error(2, debug_string);
		}
		return;
	}

	dialogue_index = global_communication_table_indices[communication_type];
	if (game_connection() == _game_connection_local &&
		BIT_VECTOR_TEST_FLAG(ai_debug.communication_suppress_vector, communication_type))
	{
		dialogue_index = NONE;
		suppress_output = TRUE;
	}
	else
	{
		suppress_output = BIT_VECTOR_TEST_FLAG(
			ai_debug.communication_ignore_vector,
			communication_type);
	}
	if (ai_debug.communication_focus_enable)
	{
		suppress_output = TRUE;
	}

	if (dialogue_index != NONE)
	{
		match_assert(
			"c:\\halo\\SOURCE\\ai\\ai_communication.c",
			0x4C3,
			(dialogue_index >= 0) && (dialogue_index < global_dialogue_event_count));

		for (usage = &global_dialogue_table[dialogue_index];
			usage->communication_type == communication_type;
			usage++, dialogue_index++)
		{
			short communication_priority = usage->communication_priority;

			if (ai_debug.communication_focus_enable &&
				BIT_VECTOR_TEST_FLAG(
					ai_debug.communication_focus_vector,
					usage->vocalization_type))
			{
				suppress_output = FALSE;
			}

			if (usage->required_hostility != NONE &&
				!hostility_matches[usage->required_hostility])
			{
				if (ai_debug.print_communication)
				{
					char const *hostility_names[NUMBER_OF_AI_COMMUNICATION_HOSTILITIES] =
					{
						"none", "self", "friend", "enemy", "traitor"
					};

					csstrcat(
						requirements_string,
						csprintf(
							temporary,
							"[%s/%d host-%s] ",
							dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
							dialogue_index,
							hostility_names[usage->required_hostility]));
				}
				any_requirements_failed = TRUE;
				continue;
			}
			else if (sound_scripted_dialog_is_playing() &&
				usage->communication_priority < _ai_communication_priority_yell &&
				!TEST_FLAG(usage->flags, _dialogue_usage_override_scripted_bit))
			{
				if (ai_debug.print_communication)
				{
					csstrcat(requirements_string, "[scripted-override] ");
				}
				any_requirements_failed = TRUE;
				continue;
			}
			else if (usage->required_enemy_status != NONE &&
				!enemy_status[usage->required_enemy_status])
			{
				if (ai_debug.print_communication)
				{
					char const *enemy_status_names[NUMBEROF(enemy_status)] =
					{
						"never", "dead", "lost", "notvis", "nodanger", "vis"
					};

					csstrcat(
						requirements_string,
						csprintf(
							temporary,
							"[%s/%d status-%s] ",
							dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
							dialogue_index,
							enemy_status_names[usage->required_enemy_status]));
				}
				any_requirements_failed = TRUE;
				continue;
			}
			else if (usage->required_subject_race != NONE &&
				(subject_unit_index == NONE ||
				!(usage->required_subject_race & subject_race)))
			{
				if (ai_debug.print_communication)
				{
					csstrcat(
						requirements_string,
						csprintf(
							temporary,
							"[%s/%d nosubrace] ",
							dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
							dialogue_index));
				}
				any_requirements_failed = TRUE;
				continue;
			}
			else if (usage->required_cause_race != NONE &&
				(cause_unit_index == NONE ||
				!(usage->required_cause_race & cause_race)))
			{
				if (ai_debug.print_communication)
				{
					csstrcat(
						requirements_string,
						csprintf(
							temporary,
							"[%s/%d nocausrace] ",
							dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
							dialogue_index));
				}
				any_requirements_failed = TRUE;
				continue;
			}
			else if (usage->required_damage != NONE &&
				usage->required_damage != damage_type)
			{
				if (ai_debug.print_communication)
				{
					csstrcat(
						requirements_string,
						csprintf(
							temporary,
							"[%s/%d nodmg] ",
							dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
							dialogue_index));
				}
				any_requirements_failed = TRUE;
				continue;
			}
			else
			{
				short speech_priority = communication_speech_priorities[communication_priority];
				long protagonist_actor_index = NONE;
				long protagonist_unit_index = NONE;
				struct actor_datum *protagonist_actor = NULL;
				long recipient_unit_index = NONE;
				short look_type = _ai_information_none;
				long look_unit_index = NONE;
				short protagonist_look_priority;
				short recipient_look_priority = 0;
				boolean *protagonist_groups = NULL;
				boolean is_reply = FALSE;
				boolean near_player = FALSE;
				long reply_actor_index = NONE;
				short delay_time;
				short ai_delay_time;
				short candidate_delay = 0;
				real player_rating;
				real repeat_rating = 1.0f;
				real reply_rating = 1.0f;

				csstrcpy(team_string, "<err>");

				switch (usage->protagonist_type)
				{
				case _comm_protagonist_subject:
					protagonist_groups = subject_groups;
					protagonist_unit_index = subject_unit_index;
					protagonist_actor_index = subject_actor_index;
					protagonist_actor = subject_actor;
					recipient_unit_index = cause_unit_index;
					break;

				case _comm_protagonist_cause:
					protagonist_groups = cause_groups;
					protagonist_unit_index = cause_unit_index;
					protagonist_actor_index = cause_actor_index;
					protagonist_actor = cause_actor;
					recipient_unit_index = subject_unit_index;
					break;

				case _comm_protagonist_friend:
					recipient_unit_index = cause_unit_index;
					if (find_friend_actor)
					{
						short find_actor_flags = 0;

						SET_FLAG(find_actor_flags, _find_actor_allow_lookup_bit,
							TEST_FLAG(usage->flags, _dialogue_usage_lookup_bit));
						SET_FLAG(find_actor_flags, _find_actor_near_to_players_bit, TRUE);
						SET_FLAG(find_actor_flags, _find_actor_same_vehicle_bit,
							TEST_FLAG(usage->flags, _dialogue_usage_same_vehicle_bit));
						SET_FLAG(find_actor_flags, _find_actor_allow_subject_bit,
							TEST_FLAG(usage->flags, _dialogue_usage_allow_subject_bit));
						SET_FLAG(find_actor_flags, _find_actor_allow_cause_bit, TRUE);

						if (subject_encounter_index != NONE)
						{
							friend_actor_index = ai_communication_find_specific_actor_to_talk(
								DATUM_INDEX_TO_ABSOLUTE_INDEX(subject_encounter_index),
								subject_unit_index,
								cause_unit_index,
								10.0f,
								communication_type,
								communication_priority,
								speech_priority,
								usage->vocalization_type,
								usage->animation_type,
								find_actor_flags);
						}
						else
						{
							friend_actor_index = ai_communication_find_global_actor_to_talk(
								subject_team,
								_find_actor_mode_friend,
								subject_unit_index,
								cause_unit_index,
								10.0f,
								communication_type,
								communication_priority,
								speech_priority,
								usage->vocalization_type,
								usage->animation_type,
								find_actor_flags);
						}
						find_friend_actor = FALSE;
					}

					protagonist_actor_index = friend_actor_index;
					if (protagonist_actor_index != NONE)
					{
						protagonist_actor = actor_get(protagonist_actor_index);
						protagonist_unit_index = protagonist_actor->meta.unit_index;
					}
					break;

				case _comm_protagonist_enemy:
					recipient_unit_index = cause_unit_index;
					if (find_other_actor)
					{
						short find_actor_flags = 0;

						SET_FLAG(find_actor_flags, _find_actor_allow_lookup_bit,
							TEST_FLAG(usage->flags, _dialogue_usage_lookup_bit));
						SET_FLAG(find_actor_flags, _find_actor_near_to_players_bit, TRUE);
						SET_FLAG(find_actor_flags, _find_actor_same_vehicle_bit,
							TEST_FLAG(usage->flags, _dialogue_usage_same_vehicle_bit));
						SET_FLAG(find_actor_flags, _find_actor_allow_subject_bit,
							TEST_FLAG(usage->flags, _dialogue_usage_allow_subject_bit));

						other_actor_index = ai_communication_find_global_actor_to_talk(
							subject_team,
							_find_actor_mode_enemy,
							subject_unit_index,
							cause_unit_index,
							12.0f,
							communication_type,
							communication_priority,
							speech_priority,
							usage->vocalization_type,
							usage->animation_type,
							find_actor_flags);
						find_other_actor = FALSE;
					}

					protagonist_actor_index = other_actor_index;
					if (protagonist_actor_index != NONE)
					{
						protagonist_actor = actor_get(protagonist_actor_index);
						protagonist_unit_index = protagonist_actor->meta.unit_index;
					}
					break;

				default:
					match_vassert("c:\\halo\\SOURCE\\ai\\ai_communication.c", 0x594, FALSE, NULL);
					break;
				}

				{
					boolean protagonist_invalid = FALSE;

					if (protagonist_unit_index == NONE)
					{
						protagonist_invalid = TRUE;
					}
					else
					{
						struct unit_datum *protagonist_unit = unit_get(protagonist_unit_index);

						if (TEST_FLAG(protagonist_unit->object.damage_flags, _object_dead_bit))
						{
							protagonist_invalid = TRUE;
						}
						else if (protagonist_unit->object.type == _object_type_vehicle)
						{
							protagonist_invalid = TRUE;
						}
						else if (protagonist_unit->unit.player_index != NONE &&
							protagonist_unit->unit.actor_index == NONE)
						{
							if (TEST_FLAG(usage->flags, _dialogue_usage_player_bit))
							{
								is_reply = TRUE;
								player_involved = TRUE;
							}
							else
							{
								protagonist_invalid = TRUE;
							}
						}
					}

					if (protagonist_actor)
					{
						if (protagonist_actor->state.mode == _actor_mode_braindead)
						{
							protagonist_invalid = TRUE;
						}
						else if (protagonist_actor->state.action == _actor_action_obey &&
							!protagonist_actor->state.action_data.obey.allow_communication)
						{
							protagonist_invalid = TRUE;
						}
					}

					if (protagonist_invalid)
					{
						if (ai_debug.print_communication)
						{
							char protagonist_code =
								usage->protagonist_type == _comm_protagonist_subject ? 's' :
									usage->protagonist_type == _comm_protagonist_cause ? 'c' :
									usage->protagonist_type == _comm_protagonist_friend ? 'f' :
									usage->protagonist_type == _comm_protagonist_enemy ? 'e' : '?';

							csstrcat(
								debug_string,
								csprintf(
									temporary,
									"[%s/%d nounit-%c] ",
									dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
									dialogue_index,
									protagonist_code));
						}
						any_protagonist_considered = TRUE;
						continue;
					}
				}

				if (is_reply)
				{
					reply_actor_index = ai_communication_find_actor_to_reply_to_player(
						protagonist_unit_index,
						recipient_unit_index,
						usage->vocalization_type,
						damage_type,
						&reply_rating);
					if (protagonist_unit_index == subject_unit_index)
					{
						find_friend_actor = FALSE;
						friend_actor_index = reply_actor_index;
					}

					if (reply_actor_index == NONE)
					{
						if (ai_debug.print_communication)
						{
							csstrcat(
								debug_string,
								csprintf(
									temporary,
									"[%s/%d noplyreply] ",
									dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
									dialogue_index));
						}
						any_protagonist_considered = TRUE;
						continue;
					}
				}

				if (usage->required_group != NONE &&
					protagonist_groups &&
					!protagonist_groups[usage->required_group])
				{
					if (ai_debug.print_communication)
					{
						char group_code =
							usage->required_group == 0 ? 'e' :
								usage->required_group == 1 ? 't' : '?';

						csstrcat(
							debug_string,
							csprintf(
								temporary,
								"[%s/%d nogrp-%c] ",
								dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
								dialogue_index,
								group_code));
					}
					any_protagonist_considered = TRUE;
					continue;
				}

				if (is_reply)
				{
					communication_priority =
						communication_player_speaking_priorities[communication_priority];
					player_rating = 2.0f;
					csstrcpy(team_string, "player");
				}
				else
				{
					player_rating = ai_communication_get_player_rating(
						protagonist_unit_index,
						TRUE,
						NULL,
						NULL);
					if (player_rating == 0.0f)
					{
						if (ai_debug.print_communication)
						{
							csstrcat(
								debug_string,
								csprintf(
									temporary,
									"[%s/%d 0-playrat] ",
									dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
									dialogue_index));
						}
						any_protagonist_considered = TRUE;
						continue;
					}
					else
					{
						short communication_team;

						near_player = player_rating < communication_player_rating_low_priority;
						communication_team = protagonist_actor_index == NONE ?
							NONE : actor_communication_team(protagonist_actor_index);

						if (communication_team == NONE)
						{
							csstrcpy(team_string, "unteamed");
						}
						else
						{
							match_assert(
								"c:\\halo\\SOURCE\\ai\\ai_communication.c",
								0x62B,
								(communication_team >= 0) && (communication_team < NUMBER_OF_AI_COMMUNICATION_TEAMS));
							match_assert(
								"c:\\halo\\SOURCE\\ai\\ai_communication.c",
								0x62E,
								(communication_priority > _ai_communication_priority_none) && (communication_priority < NUMBER_OF_AI_COMMUNICATION_PRIORITIES));

							sprintf(
								team_string,
								"%s-%c%c%c%s",
								global_communication_team_names[2 * communication_team + 1],
								global_communication_priority_names[communication_priority][0],
								global_communication_priority_names[communication_priority][1],
								global_communication_priority_names[communication_priority][2],
								near_player ? "-lo" : "-hi");

							if (speech_disabled[communication_team][communication_priority][near_player])
							{
								match_assert(
									"c:\\halo\\SOURCE\\ai\\ai_communication.c",
									0x639,
									communication_priority < _ai_communication_priority_yell);

								if (ai_debug.print_communication)
								{
									if (speech_disabled_reason[communication_team][communication_priority][near_player] == 0)
									{
										sprintf(
											timer_string,
											"chat:%.1f<%.1f",
											chatter_seconds[communication_team],
											communication_timer_tolerances[communication_priority][near_player][0]);
									}
									else if (speech_disabled_reason[communication_team][communication_priority][near_player] == 1)
									{
										sprintf(
											timer_string,
											"talk:%.1f<%.1f",
											talk_seconds[communication_team],
											communication_timer_tolerances[communication_priority][near_player][1]);
									}
									else if (speech_disabled_reason[communication_team][communication_priority][near_player] == 2)
									{
										sprintf(
											timer_string,
											"shout:%.1f<%.1f",
											shout_seconds[communication_team],
											communication_timer_tolerances[communication_priority][near_player][3]);
									}
									else
									{
										sprintf(timer_string, "<err>");
									}

									csstrcat(
										debug_string,
										csprintf(
											temporary,
											"[%s/%d %s %s] ",
											dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
											dialogue_index,
											team_string,
											timer_string));
								}
								any_protagonist_considered = TRUE;
								continue;
							}
							else
							{
								candidate_delay = speech_delay[communication_team][communication_priority][near_player];

								if (communication_priority < _ai_communication_priority_exclaim)
								{
									struct dialogue_event_status *event = &global_dialogue_events[
										dialogue_index * NUMBER_OF_AI_COMMUNICATION_TEAMS +
										communication_team];

									if (event->last_time_spoken != NONE)
									{
										repeat_rating =
											(real)(event_time - event->last_time_spoken) *
											0.0011111111f;
										if (repeat_rating < 0.0f)
										{
											repeat_rating = 0.0f;
										}
										else if (repeat_rating > 1.0f)
										{
											repeat_rating = 1.0f;
										}
									}

									if ((game_connection() != _game_connection_local ||
										!ai_debug.communication_timeout_disabled) &&
										event->disable_until_time != NONE)
									{
										long remaining = event->disable_until_time - event_time;

										if (near_player)
										{
											remaining += communication_timeout_low_priority_modifier;
										}
										if (remaining > 0)
										{
											if (ai_debug.print_communication)
											{
												csstrcat(
													debug_string,
													csprintf(
														temporary,
														"[%s/%d %s-d-dis/%d] ",
														dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
														dialogue_index,
														global_communication_team_names[2 * communication_team + 1],
														remaining));
											}
											any_protagonist_considered = TRUE;
											continue;
										}
									}
								}
							}
						}
					}
				}

				delay_time = (short)(
					communication_play_delays[usage->protagonist_type] * TICKS_PER_SECOND);
				if (subject_race == _race_player && !is_reply)
				{
					delay_time = (short)(delay_time + communication_player_additional_delay);
				}
				ai_delay_time = (short)(TEST_FLAG(usage->flags, _dialogue_usage_immediate_notify_bit) ?
					0.0f :
					communication_notification_delays[communication_priority] * TICKS_PER_SECOND);
				delay_time = (short)(delay_time + candidate_delay);
				ai_delay_time = (short)(ai_delay_time + candidate_delay);

				switch (usage->recipient_look_direction)
				{
				case _comm_look_direction_subject:
					if (subject_unit_index != NONE)
					{
						look_type = _ai_information_look_unit;
						look_unit_index = subject_unit_index;
					}
					break;

				case _comm_look_direction_protagonist:
					if (protagonist_unit_index != NONE)
					{
						look_type = _ai_information_look_unit;
						look_unit_index = protagonist_unit_index;
					}
					break;

				case _comm_look_direction_target:
					if (recipient_unit_index != NONE)
					{
						look_type = _ai_information_look_unit;
						look_unit_index = recipient_unit_index;
					}
					break;

				case _comm_look_direction_danger:
					if (subject_actor_index != NONE)
					{
						struct actor_datum *subject_actor = actor_get(subject_actor_index);

						if (subject_actor->danger_zone.danger_type > _actor_danger_zone_none)
						{
							match_assert(
								"c:\\halo\\SOURCE\\ai\\ai_communication.c",
								0x6B0,
								subject_actor->danger_zone.object_index != NONE);
							look_type = _ai_information_look_object;
							look_unit_index = subject_actor->danger_zone.object_index;
						}
					}
					break;
				}

				if (look_type != _ai_information_none)
				{
					recipient_look_priority = usage->recipient_look_priority;
					if (recipient_look_priority == NONE ||
						recipient_look_priority == TRUE)
					{
						recipient_look_priority =
							communication_recipient_default_look_priorities[communication_priority];
					}
				}

				protagonist_look_priority = usage->protagonist_look_priority;
				if (protagonist_look_priority == NONE ||
					protagonist_look_priority == TRUE)
				{
					protagonist_look_priority =
						communication_protagonist_default_look_priorities[communication_priority];
				}

				{
					short vocalization_type = usage->vocalization_type;
					short animation_type = usage->animation_type;
					long sound_definition_index = NONE;
					real speech_weight = 1.0f;
					real animation_weight = 1.0f;
					short play_type = _unit_play_speech_none;

					if (!is_reply)
					{
						play_type = ai_communication_consider_speech(
							protagonist_unit_index,
							communication_priority,
							speech_priority,
							delay_time,
							TEST_FLAG(usage->flags, _dialogue_usage_lookup_bit),
							FALSE,
							&vocalization_type,
							&speech_weight,
							&sound_definition_index,
							speech_string);
						if (play_type == _unit_play_speech_none)
						{
							if (ai_debug.print_communication)
							{
								csstrcat(
									debug_string,
									csprintf(
										temporary,
										"[%s/%d u-%s-%s] ",
										dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
										dialogue_index,
										speech_weight > 0.0f ? "dis" : "n/a",
										speech_string));
							}
							any_protagonist_considered = TRUE;
							continue;
						}
						else if (animation_type != NONE)
						{
							boolean animation_impulse = unit_test_animation_impulse(
								protagonist_unit_index,
								animation_type);

							if (animation_impulse && protagonist_actor_index != NONE)
							{
								match_assert(
									"c:\\halo\\SOURCE\\ai\\ai_communication.c",
									0x6F2,
									protagonist_actor);
								if (actor_action_class(protagonist_actor_index) == _action_class_transitory)
								{
									animation_impulse = FALSE;
								}
								else if (protagonist_actor->state.mode == _actor_mode_asleep)
								{
									animation_impulse = FALSE;
								}
							}

							if (animation_impulse)
							{
								animation_weight = 2.0f;
							}
						}
					}

					{
						real weight = animation_weight * speech_weight * player_rating *
							usage->weight * reply_rating * repeat_rating;

						if (weight > 0.0f)
						{
							if (possibility_count >= MAXIMUM_COMMUNICATION_POSSIBILITIES)
							{
								error(
									2,
									"ai_communication_event: type %d (%s) overflowed MAXIMUM_COMMUNICATION_POSSIBILITIES (%d)",
									communication_type,
									global_communication_type_names[communication_type],
									MAXIMUM_COMMUNICATION_POSSIBILITIES);
								break;
							}

							possibilities[possibility_count].weight = weight;
							possibilities[possibility_count].dialogue_type_index = dialogue_index;
							possibilities[possibility_count].is_reply = is_reply;
							possibilities[possibility_count].protagonist_unit_index = protagonist_unit_index;
							possibilities[possibility_count].protagonist_actor_index = protagonist_actor_index;
							possibilities[possibility_count].animation_type = usage->animation_type;
							possibilities[possibility_count].recipient_unit_index = recipient_unit_index;
							possibilities[possibility_count].preselected_reply_actor_index = reply_actor_index;
							possibilities[possibility_count].priority = speech_priority;
							possibilities[possibility_count].delay_time = delay_time;
							possibilities[possibility_count].ai_delay_time = ai_delay_time;
							possibilities[possibility_count].vocalization_type = vocalization_type;
							possibilities[possibility_count].sound_definition_index = sound_definition_index;
							possibilities[possibility_count].recipient_look_priority = recipient_look_priority;
							possibilities[possibility_count].look_type = look_type;
							possibilities[possibility_count].look_unit_index = look_unit_index;
							possibilities[possibility_count].play_type = play_type;
							possibilities[possibility_count].protagonist_look_priority = protagonist_look_priority;
							possibilities[possibility_count].interrupts =
								TEST_FLAG(usage->flags, _dialogue_usage_force_bit);
							if (possibilities[possibility_count].interrupts)
							{
								any_forced_possibility = TRUE;
							}
							possibility_count++;

							if (ai_debug.print_communication)
							{
								csstrcat(
									debug_string,
									csprintf(
										temporary,
										"[%s/%d %s del%d w:%.1f%s s%.1f p%.1f%s a%.1f rc%.1f rp%.1f t%.1f] ",
										dialogue_get_vocalization_name(usage->vocalization_type, TRUE),
										dialogue_index,
										team_string,
										candidate_delay,
										usage->weight,
										possibilities[possibility_count - 1].interrupts ? "F" : "",
										speech_weight,
										player_rating,
										is_reply ? "PLAYER" : "",
										animation_weight,
										repeat_rating,
										reply_rating,
										possibilities[possibility_count - 1].weight));
							}

							total_possibility_weight += weight;
							any_protagonist_considered = TRUE;
						}
					}
				}
			}
		}
	}

	if (ai_debug.print_communication_player && !player_involved)
	{
		suppress_output = TRUE;
	}

	if (possibility_count > 0)
	{
		struct ai_information_packet information;

		selected_possibility = possibilities;
		if (any_forced_possibility)
		{
			real original_weight = total_possibility_weight;
			short forced_count = 0;
			short index;

			total_possibility_weight = 0.0f;
			for (index = 0; index < possibility_count; index++)
			{
				if (possibilities[index].interrupts)
				{
					forced_count++;
				}
				else
				{
					possibilities[index].weight = 0.0f;
				}
				total_possibility_weight += possibilities[index].weight;
			}

			match_assert(
				"c:\\halo\\SOURCE\\ai\\ai_communication.c",
				0x76E,
				total_possibility_weight > 0.0f);

			if (ai_debug.print_communication)
			{
				csstrcat(
					debug_string,
					csprintf(
						temporary,
						"[%d/%.1f force %d/%.1f] ",
						possibility_count,
						original_weight,
						forced_count,
						total_possibility_weight));
			}
		}

		if (possibility_count > 1)
		{
			real cumulative_weight = 0.0f;
			real random_weight = real_seed_random(get_global_random_seed_address()) *
				total_possibility_weight;
			short index;

			for (index = 0; index < possibility_count - 1; index++)
			{
				cumulative_weight += possibilities[index].weight;
				if (cumulative_weight >= random_weight)
				{
					break;
				}
			}

			selected_possibility = &possibilities[index];
			if (ai_debug.print_communication)
			{
				csstrcat(
					debug_string,
					csprintf(
						temporary,
						"[rnd%.1f tot%.1f cum%.1f@%d] ",
						random_weight,
						total_possibility_weight,
						cumulative_weight,
						index));
			}
		}

		information.communication_type = communication_type;
		information.target_unit_index = selected_possibility->recipient_unit_index;
		information.damage_category = damage_type;
		information.dialogue_type_index = selected_possibility->dialogue_type_index;
		information.updated_dialogue_timers = TRUE;
		information.look_priority = selected_possibility->recipient_look_priority;
		information.look_type = selected_possibility->look_type;
		information.look_data.unit.unit_index = selected_possibility->look_unit_index;
		information.information_type = information_type == NONE ?
			_ai_information_none : information_type;
		if (!information_data)
		{
			csmemset(
				&information.information_data,
				0,
				sizeof(information.information_data));
		}
		else
		{
			information.information_data = *information_data;
		}

		if (selected_possibility->is_reply)
		{
			match_assert(
				"c:\\halo\\SOURCE\\ai\\ai_communication.c",
				0x7A7,
				selected_possibility->preselected_reply_actor_index != NONE);

			ai_communication_started(
				selected_possibility->protagonist_unit_index,
				selected_possibility->priority,
				selected_possibility->vocalization_type,
				&information);
			ai_communication_notify(
				selected_possibility->protagonist_unit_index,
				selected_possibility->priority,
				selected_possibility->vocalization_type,
				&information);
			ai_communication_finished(
				selected_possibility->protagonist_unit_index,
				selected_possibility->priority,
				selected_possibility->vocalization_type,
				TRUE,
				selected_possibility->preselected_reply_actor_index,
				&information);
		}
		else
		{
			struct unit_speech_item speech;

			speech.priority = selected_possibility->priority;
			speech.vocalization_type = selected_possibility->vocalization_type;
			speech.sound_definition_index = selected_possibility->sound_definition_index;
			speech.delay_time = selected_possibility->delay_time;
			speech.ai_notification_delay = selected_possibility->ai_delay_time;
			speech.pause_time = 24;
			speech.ai = information;
			unit_speak(
				selected_possibility->protagonist_unit_index,
				selected_possibility->play_type,
				&speech);

			if (selected_possibility->animation_type != NONE)
			{
				struct unit_datum *speaker_unit = unit_get(selected_possibility->protagonist_unit_index);
				real_vector2d alignment;

				alignment.i = speaker_unit->object.forward.i;
				alignment.j = speaker_unit->object.forward.j;
				if (selected_possibility->recipient_unit_index != NONE)
				{
					real_point3d speaker_head;
					real_point3d target_head;
					real magnitude;

					unit_get_head_position(selected_possibility->protagonist_unit_index, &speaker_head);
					unit_get_head_position(selected_possibility->recipient_unit_index, &target_head);
					alignment.i = target_head.x - speaker_head.x;
					alignment.j = target_head.y - speaker_head.y;
					magnitude = square_root(
						alignment.i * alignment.i + alignment.j * alignment.j);
					if (fabs(magnitude) >= _real_epsilon && magnitude != 0.0f)
					{
						alignment.i /= magnitude;
						alignment.j /= magnitude;
					}
					else
					{
						alignment.i = speaker_unit->object.forward.i;
						alignment.j = speaker_unit->object.forward.j;
					}
				}

				unit_start_animation_impulse(
					selected_possibility->protagonist_unit_index,
					selected_possibility->animation_type,
					&alignment);
			}

			if (selected_possibility->protagonist_actor_index != NONE)
			{
				ai_communication_look_secondary_at_unit(
					selected_possibility->protagonist_actor_index,
					_secondary_look_communicated_direction,
					selected_possibility->protagonist_look_priority,
					selected_possibility->recipient_unit_index,
					NONE);
			}
			ai_communication_update_speech_timers(
				selected_possibility->protagonist_unit_index,
				selected_possibility->priority,
				selected_possibility->vocalization_type,
				selected_possibility->dialogue_type_index,
				NONE);
		}

		if (ai_debug.print_communication)
		{
			csstrcat(
				debug_string,
				strupr(csprintf(
					temporary,
					">>%s<<",
					dialogue_get_vocalization_name(selected_possibility->vocalization_type, TRUE))));
			if (!suppress_output)
			{
				error(2, debug_string);
			}
		}
	}
	else if (ai_debug.print_communication)
	{
		if (any_requirements_failed && !any_protagonist_considered)
		{
			csstrcat(debug_string, requirements_string);
		}
		if (ai_debug.communication_focus_enable ||
			!any_requirements_failed ||
			any_protagonist_considered)
		{
			csstrcat(debug_string, "NONE");
			if (!suppress_output)
			{
				error(2, debug_string);
			}
		}
	}

	return;
}

/* ---------- private code */
