#include "stdafx.h"


namespace {
	static const GUID id_copy_group = { 0xa4180e66, 0x8a01, 0x41ea, { 0x81, 0xb7, 0xb3, 0x10, 0x43, 0xcc, 0x9d, 0x24 } };

	static contextmenu_group_popup_factory copy_group_factory(id_copy_group, contextmenu_groups::root, "Copy metadata", 1);

	class copy_items : public contextmenu_item_simple {
	public:
		enum {
			cmd_song = 0,
			cmd_artist_album,
			cmd_album,
			cmd_directory,
			cmd_path,
			cmd_total
		};

		GUID get_parent() override {
			return id_copy_group;
		}

		unsigned get_num_items() override {
			return cmd_total;
		}

		void get_item_name(unsigned p_index, pfc::string_base& p_out) override {
			switch (p_index) {
			case cmd_song: p_out = "Song"; break;
			case cmd_artist_album: p_out = "Artist Album"; break;
			case cmd_album: p_out = "Album"; break;
			case cmd_directory: p_out = "Directory"; break;
			case cmd_path: p_out = "Path"; break;
			default: uBugCheck();
			}
		}

		GUID get_item_guid(unsigned p_index) override {
			static const GUID id_cmd_song = { 0x3bc114ac, 0x207d, 0x4a3c, { 0xa1, 0x6e, 0xfe, 0xe0, 0x62, 0x39, 0xb1, 0x6a } };
			static const GUID id_cmd_artist_album = { 0x6798346d, 0xdf7f, 0x4d4a, { 0x8b, 0x20, 0xea, 0xd3, 0x69, 0xb2, 0xb2, 0xe3 } };
			static const GUID id_cmd_album = { 0x2e21e5fd, 0xf5d1, 0x40c4, { 0x81, 0x70, 0xe2, 0x80, 0x4f, 0xf9, 0x18, 0xdd } };
			static const GUID id_cmd_directory = { 0x7484b70f, 0x90c1, 0x453c, { 0x80, 0x77, 0xbf, 0x9a, 0xb0, 0x51, 0x4f, 0xba } };
			static const GUID id_cmd_path = { 0x44d6a8c6, 0x313e, 0x45a9, { 0x8e, 0x98, 0xc6, 0x9c, 0x15, 0x5d, 0x31, 0x17 } };

			switch (p_index) {
			case cmd_song: return id_cmd_song;
			case cmd_artist_album: return id_cmd_artist_album;
			case cmd_album: return id_cmd_album;
			case cmd_directory: return id_cmd_directory;
			case cmd_path: return id_cmd_path;
			default: uBugCheck();
			}
		}

		bool get_item_description(unsigned p_index, pfc::string_base& p_out) override {
			switch (p_index) {
			case cmd_song: p_out = "Copies the song name."; return true;
			case cmd_artist_album: p_out = "Copies artist & album names."; return true;
			case cmd_album: p_out = "Copies the album name."; return true;
			case cmd_directory: p_out = "Copies the directory."; return true;
			case cmd_path: p_out = "Copies the file path."; return true;
			default:
				uBugCheck();
			}
		}

		void context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) override {
			switch (p_index) {
			case cmd_song: metaCopy(p_data, "[[%artist%] [%title%]]"); break;
			case cmd_artist_album: metaCopy(p_data, "[[%artist%] [%album%]]"); break;
			case cmd_album: metaCopy(p_data, "[%album%]"); break;
			case cmd_directory: metaCopy(p_data, "%directory%"); break;
			case cmd_path: metaCopy(p_data, "%path%"); break;
			default:
				uBugCheck();
			}
		}

		void metaCopy(metadb_handle_list_cref p_data, const char* script) {
			pfc::avltree_t<pfc::string> dedups;
			for (t_size i = 0; i < p_data.get_count(); ++i) {
				auto fmt = fb2k::formatTrackTitle(p_data[i], script);
				if (!fmt.is_empty()) {
					dedups += fmt;
				}
			}

			pfc::string_formatter data;
			for (auto iter = dedups.first(); iter.is_valid(); ++iter) {
				if (!data.is_empty()) {
					data << "\n";
				}
				data << *iter;
			}

			ClipboardHelper::OpenScope scope;
			scope.Open(core_api::get_main_window());

			ClipboardHelper::SetString(data.c_str());
		};
	};

	static contextmenu_item_factory_t<copy_items> copy_items_factory;
}
