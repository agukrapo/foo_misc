#include "stdafx.h"

namespace {
	static const GUID id_group = { 0xe27c7c95, 0x6ed4, 0x43df, { 0xa3, 0xd8, 0xa7, 0x57, 0x5d, 0x07, 0x94, 0x5d } };
	static const GUID id_pl_rem = { 0x674c0096, 0xb9fb, 0x4f4d, { 0xa0, 0xf6, 0x93, 0x7b, 0x35, 0x3f, 0x99, 0xaf } };
	static const GUID id_del = { 0x94eb3400, 0x0f40, 0x4c18, { 0xa8, 0x1b, 0x2b, 0xf2, 0x7d, 0xd0, 0xca, 0xa9 } };

	static mainmenu_group_popup_factory g_mainmenu_group(id_group, mainmenu_groups::file, mainmenu_commands::sort_priority_dontcare, "! MISC");

	class _commands : public mainmenu_commands {
	public:
		enum {
			cmd_pl_rem = 0,
			cmd_del_file,
			cmd_total
		};
		t_uint32 get_command_count() override {
			return cmd_total;
		}
		GUID get_command(t_uint32 p_index) override {
			switch (p_index) {
			case cmd_pl_rem: return id_pl_rem;
			case cmd_del_file: return id_del;
			default: uBugCheck(); // should never happen
			}
		}
		void get_name(t_uint32 p_index, pfc::string_base& p_out) override {
			switch (p_index) {
			case cmd_pl_rem: p_out = "! remove from playlist"; break;
			case cmd_del_file: p_out = "! delete file"; break;
			default: uBugCheck(); // should never happen
			}
		}
		bool get_description(t_uint32 p_index, pfc::string_base& p_out) override {
			switch (p_index) {
			case cmd_pl_rem: p_out = "Removes a song from playlist and plays the next one."; return true;
			case cmd_del_file: p_out = "Deletes a song file and plays the next one."; return true;
			default: return false;
			}
		}
		GUID get_parent() override {
			return id_group;
		}
		void execute(t_uint32 p_index, service_ptr_t<service_base> p_callback) override {
			auto pm = playlist_manager_v6::get();

			t_size pl_idx, item_idx;
			if (!pm->get_playing_item_location(&pl_idx, &item_idx)) {
				return;
			}

			switch (p_index) {

			case cmd_pl_rem: {
				bit_array_one arr(item_idx);
				pm->playlist_undo_backup(pl_idx);
				pm->playlist_remove_items(pl_idx, arr);

				playback_control_v3::get()->next();
				break;
			}

			case cmd_del_file: {
				playback_control_v3::get()->next();

				auto item = pm->playlist_get_item_handle(pl_idx, item_idx);

				auto fs = filesystem::getLocalFS();

				abort_callback_impl abort;
				fs->remove(item->get_path(), abort);

				break;
			}
			default:
				uBugCheck(); // should never happen
			}
		}
	};

	static mainmenu_commands_factory_t<_commands> g_mainmenu_commands_sample_factory;
}