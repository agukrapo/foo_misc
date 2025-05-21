#include "stdafx.h"

namespace {
	static const GUID id_group = { 0xc7bef687, 0x56c5, 0x43df, { 0xb7, 0xca, 0xd7, 0xbd, 0x4b, 0xa8, 0x31, 0x5a } };

	static contextmenu_group_popup_factory _group_factory(id_group, contextmenu_groups::root, "Search more of the same", 0);

	class _items : public contextmenu_item_simple {
	public:
		enum {
			cmd_song = 0,
			cmd_artist,
			cmd_title,
			cmd_album,
			cmd_date,
			cmd_genre,
			cmd_directory,
			cmd_total
		};
		GUID get_parent() override {
			return id_group;
		}

		unsigned get_num_items() override {
			return cmd_total;
		}

		void get_item_name(unsigned p_index, pfc::string_base& p_out) override {
			switch (p_index) {
			case cmd_song: p_out = "Song"; break;
			case cmd_artist: p_out = "Artist"; break;
			case cmd_title: p_out = "Title"; break;
			case cmd_album: p_out = "Album"; break;
			case cmd_date: p_out = "Date"; break;
			case cmd_genre: p_out = "Genre"; break;
			case cmd_directory: p_out = "Directory"; break;
			default: uBugCheck();
			}
		}

		GUID get_item_guid(unsigned p_index) override {
			static const GUID id_song = { 0x609bd371, 0x80d9, 0x4557, { 0x91, 0xbe, 0x18, 0x2, 0x0, 0x53, 0xc9, 0xf0 } };
			static const GUID id_artist = { 0x9248b12c, 0xb88, 0x4870, { 0xbe, 0x88, 0x99, 0x6e, 0x92, 0x32, 0x7d, 0x74 } };
			static const GUID id_title = { 0x7036c22e, 0xbcf2, 0x4711, { 0xba, 0x7d, 0xce, 0x2b, 0xf4, 0xcd, 0x86, 0xdb } };
			static const GUID id_album = { 0x400144ef, 0xc0a9, 0x43e5, { 0x96, 0x64, 0x97, 0x9b, 0xa1, 0x85, 0xad, 0xbc } };
			static const GUID id_date = { 0x7f0da23d, 0xb26, 0x4f76, { 0xad, 0x89, 0x79, 0x10, 0xb4, 0x11, 0xdd, 0xea } };
			static const GUID id_genre = { 0x320fa698, 0x21dc, 0x4fd4, { 0xab, 0x0, 0x32, 0xf5, 0xfc, 0x6b, 0x23, 0x69 } };
			static const GUID id_directory = { 0x68a09497, 0x3e65, 0x429d, { 0x9c, 0x1b, 0x57, 0xd7, 0xc1, 0x3d, 0x6a, 0xbb } };

			switch (p_index) {
			case cmd_song: return id_song;
			case cmd_artist: return id_artist;
			case cmd_title: return id_title;
			case cmd_album: return id_album;
			case cmd_date: return id_date;
			case cmd_genre: return id_genre;
			case cmd_directory: return id_directory;
			default: uBugCheck();
			}

		}

		bool get_item_description(unsigned p_index, pfc::string_base& p_out) override {
			switch (p_index) {
			case cmd_song: p_out = "Searches the library for similar songs."; return true;
			case cmd_artist: p_out = "Searches the library for similar artists."; return true;
			case cmd_title: p_out = "Searches the library for similar titles."; return true;
			case cmd_album: p_out = "Searches the library for similar albums."; return true;
			case cmd_date: p_out = "Searches the library for similar dates."; return true;
			case cmd_genre: p_out = "Searches the library for similar genres."; return true;
			case cmd_directory: p_out = "Searches the library for similar directories."; return true;
			default:
				uBugCheck();
			}
		}

		void context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) override {
			switch (p_index) {
			case cmd_song: songSearch(p_data); break;
			case cmd_artist: metaSearch(p_data, "artist"); break;
			case cmd_title: metaSearch(p_data, "title"); break;
			case cmd_album: metaSearch(p_data, "album"); break;
			case cmd_date: metaSearch(p_data, "date"); break;
			case cmd_genre: metaSearch(p_data, "genre"); break;
			case cmd_directory: dirSearch(p_data); break;
			default:
				uBugCheck();
			}
		}

		void songSearch(metadb_handle_list_cref p_data) {
			pfc::string_formatter query;

			for (t_size i = 0; i < p_data.get_count(); ++i) {
				const file_info* info = &p_data[i]->get_full_info_ref(fb2k::noAbort)->info();

				auto song = song_name(info);
				if (song.empty()) {
					continue;
				}

				if (!query.empty()) {
					query.add_string(" OR ");
				}

				query << "* HAS " << song;
			}

			library_search_ui::get()->show(query);
		}

		void metaSearch(metadb_handle_list_cref p_data, const char* name) {
			pfc::string_formatter query;

			for (t_size i = 0; i < p_data.get_count(); ++i) {
				const file_info* info = &p_data[i]->get_full_info_ref(fb2k::noAbort)->info();

				auto data = get_all_meta(info, name);
				if (data.empty()) {
					continue;
				}

				if (!query.empty()) {
					query.add_string(" OR ");
				}

				query << "%" << name << "% HAS " << data;
			}

			library_search_ui::get()->show(query);
		}

		void dirSearch(metadb_handle_list_cref p_data) {
			pfc::string_formatter query;

			for (t_size i = 0; i < p_data.get_count(); ++i) {
				pfc::string path(p_data[i]->get_path());

				auto fullDir = pfc::io::path::getDirectory(path);

				if (pfc::io::path::isDirectoryRoot(fullDir)) {
					continue;
				}

				if (!query.empty()) {
					query.add_string(" OR ");
				}

				auto parent = pfc::io::path::getParent(fullDir);

				query << "%directory% HAS " << fullDir.subString(parent.length() + 1);
			}

			library_search_ui::get()->show(query);
		}
	};

	static contextmenu_item_factory_t<_items> _contextmenu_factory;
}
