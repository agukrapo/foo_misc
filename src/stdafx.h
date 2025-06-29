#include <helpers/foobar2000+atl.h>
#include <libPPUI/clipboard.h>

pfc::string song_name(const file_info* info);

pfc::string get_all_meta(const file_info* info, const char* name);

void clear_metadata(metadb_handle_list_cref p_data, service_ptr_t<file_info_filter> p_filter);

class clear_album_filter : public file_info_filter {
public:
	bool apply_filter(metadb_handle_ptr p_location, t_filestats p_stats, file_info& p_info);
};