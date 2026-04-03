#pragma once
#include "mongoose.h"

// Encoded video frame provided by user code
struct mg_rtsp_frame {
  const void *data;      // H.264 encoded frame (one access unit)
  size_t len;            // Frame size in bytes
  uint64_t pts_us;       // Presentation timestamp, microseconds
  bool keyframe;         // true for IDR frame
};

// Encoded audio frame provided by user code
struct mg_rtsp_audio {
  const void *data;      // AAC encoded frame, one access unit, no ADTS header
  size_t len;            // Frame size in bytes
  uint64_t pts_us;       // Presentation timestamp, microseconds
};

// Called by Mongoose when it needs the next video frame
// - frame->data must stay valid until the callback returns
// - Mongoose copies/packetizes it before returning
typedef bool (*mg_rtsp_next_frame_fn)(struct mg_rtsp_frame *frame,
                                     void *fn_data);

// Called by Mongoose when it needs the next audio frame
// - audio->data must stay valid until the callback returns
// - Mongoose copies/packetizes it before returning
typedef bool (*mg_rtsp_next_audio_fn)(struct mg_rtsp_audio *audio,
                                      void *fn_data);

// RTSP listener options (H.264 video, optional AAC audio)
struct mg_rtsp_opts {
  unsigned width;                // Optional, 0 if unknown
  unsigned height;               // Optional, 0 if unknown
  unsigned fps;                  // Optional, 0 if unknown/variable

  mg_rtsp_next_frame_fn fn;      // User callback to provide frames
  void *fn_data;                 // User callback data

  mg_rtsp_next_audio_fn audio_fn;  // Optional AAC callback, NULL if no audio
  void *audio_fn_data;             // Audio callback data
  uint8_t aac_lc_config[2];        // AAC-LC config, e.g. {0x12, 0x10};
                                   // sample rate and channels are inferred
                                   // e.g. {0x12, 0x10} is 44.1 kHz, stereo.
};
// Start RTSP server listener
// E.g. mg_rtsp_listen(&mgr, "rtsp://0.0.0.0:8554/cam", &opts);
struct mg_connection *mg_rtsp_listen(struct mg_mgr *mgr, const char *url,
                                     const struct mg_rtsp_opts *opts);

// Helper callbacks that get H.264 video audio from the MP4 file.
// fn_data must be a const char * path to the MP4 file. E.g. "test.mp4"
bool mg_rtsp_get_mp4_frame(struct mg_rtsp_frame *frame, void *fn_data);
bool mg_rtsp_get_mp4_audio(struct mg_rtsp_audio *audio, void *fn_data);
