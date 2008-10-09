/*
   mkvmerge -- utility for splicing together matroska files
   from component media subtypes

   Distributed under the GPL
   see the file COPYING for details
   or visit http://www.gnu.org/copyleft/gpl.html

   $Id$

   output handling

   Written by Moritz Bunkus <moritz@bunkus.org>.
   Modified by Steve Lhomme <steve.lhomme@free.fr>.
*/

#ifndef __OUTPUT_CONTROL_H
#define __OUTPUT_CONTROL_H

#include "os.h"

#include "mkvmerge.h"
#include "pr_generic.h"
#include "smart_pointers.h"

namespace libmatroska {
  class KaxChapters;
  class KaxCues;
  class KaxSeekHead;
  class KaxSegment;
  class KaxTag;
  class KaxTags;
  class KaxTrackEntry;
  class KaxTracks;
  class KaxSegmentFamily;
  class KaxInfo;
};

using namespace std;
using namespace libmatroska;

class mm_io_c;
class generic_packetizer_c;
class generic_reader_c;

struct append_spec_t {
  int64_t src_file_id;
  int64_t src_track_id;
  int64_t dst_file_id;
  int64_t dst_track_id;
};

struct packetizer_t {
  file_status_e status, old_status;
  packet_cptr pack;
  generic_packetizer_c *packetizer, *orig_packetizer;
  int64_t file, orig_file;
  bool deferred;

  packetizer_t():
    status(FILE_STATUS_MOREDATA), old_status(FILE_STATUS_MOREDATA),
    packetizer(NULL), orig_packetizer(NULL),
    file(0), orig_file(0),
    deferred(false) { }
};

struct deferred_connection_t {
  append_spec_t amap;
  packetizer_t *ptzr;
};

struct filelist_t {
  string name;
  int64_t size;

  file_type_e type;

  packet_cptr pack;

  generic_reader_c *reader;

  track_info_c *ti;
  bool appending, appended_to, done;

  int num_unfinished_packetizers, old_num_unfinished_packetizers;
  vector<deferred_connection_t> deferred_connections;
  int64_t deferred_max_timecode_seen;

  filelist_t():
    size(0), type(FILE_TYPE_IS_UNKNOWN),
    reader(NULL),
    ti(NULL), appending(false), appended_to(false), done(false),
    num_unfinished_packetizers(0), old_num_unfinished_packetizers(0),
    deferred_max_timecode_seen(-1) {}
};

struct attachment_t {
  string name, stored_name, mime_type, description;
  int64_t id;
  bool to_all_files;
  counted_ptr<buffer_t> data;

  attachment_t() {
    clear();
  }
  void clear() {
    name = "";
    stored_name = "";
    mime_type = "";
    description = "";
    id = 0;
    to_all_files = false;
    data = counted_ptr<buffer_t>(NULL);
  }
};

struct track_order_t {
  int64_t file_id;
  int64_t track_id;
};

enum timecode_scale_mode_e {
  TIMECODE_SCALE_MODE_NORMAL = 0,
  TIMECODE_SCALE_MODE_FIXED,
  TIMECODE_SCALE_MODE_AUTO
};

class family_uids_c: public vector<bitvalue_c> {
public:
  bool add_family_uid(const KaxSegmentFamily &family);
};

extern vector<packetizer_t> g_packetizers;
extern vector<filelist_t> g_files;
extern vector<attachment_t> g_attachments;
extern vector<track_order_t> g_track_order;
extern vector<append_spec_t> g_append_mapping;

extern string g_outfile;

extern double g_timecode_scale;
extern timecode_scale_mode_e g_timecode_scale_mode;

typedef counted_ptr<bitvalue_c> g_bitvalue_cptr;

extern bitvalue_cptr g_seguid_link_previous, g_seguid_link_next;
extern vector<bitvalue_cptr> g_forced_seguids;
extern family_uids_c g_segfamily_uids;

extern KaxInfo *g_kax_info_chap;

extern bool g_write_meta_seek_for_clusters;

extern string g_chapter_file_name;
extern string g_chapter_language;
extern string g_chapter_charset;

extern string g_segmentinfo_file_name;

extern KaxTags *g_tags_from_cue_chapters;

extern KaxSegment *g_kax_segment;
extern KaxTracks g_kax_tracks;
extern KaxTrackEntry *g_kax_last_entry;
extern KaxCues *g_kax_cues;
extern KaxSeekHead *g_kax_sh_main, *g_kax_sh_cues;
extern KaxChapters *g_kax_chapters;
extern int64_t g_tags_size;
extern string g_segment_title;
extern bool g_segment_title_set;
extern string g_default_language;

extern float g_video_fps;
extern generic_packetizer_c *g_video_packetizer;

extern bool g_write_cues, g_cue_writing_requested;
extern bool g_no_lacing, g_no_linking, g_use_durations;

extern bool g_identifying, g_identify_verbose, g_identify_for_mmg;

extern int g_file_num;

extern int64_t g_max_ns_per_cluster;
extern int g_max_blocks_per_cluster;
extern int g_default_tracks[3], g_default_tracks_priority[3];

extern bool g_splitting;
extern int g_split_max_num_files;

void get_file_type(filelist_t &file);
void create_readers();

void setup();
void cleanup();
void main_loop();

void add_packetizer_globally(generic_packetizer_c *packetizer);
void add_tags(KaxTag *tags);

void create_next_output_file();
void finish_file(bool last_file = false);
void rerender_track_headers();
string create_output_name();

int64_t add_attachment(attachment_t attachment);

#endif // __OUTPUT_CONTROL_H
