#include "stdafx.h"


pfc::string song_name(const file_info* info) {
	auto artist = get_all_meta(info, "artist");
	auto title = get_all_meta(info, "title");

	pfc::string middle;
	if (!artist.is_empty() && !title.is_empty()) {
		middle = " ";
	}

	return artist << middle << title;
}

pfc::string get_all_meta(const file_info* info, const char* name) {
	pfc::string_formatter out;

	for (t_size i = 0; i < info->meta_get_count_by_name(name); ++i) {
		if (i > 0) {
			out.add_string(" ");
		}
		out.add_string(info->meta_get(name, i));
	}

	return out;
}