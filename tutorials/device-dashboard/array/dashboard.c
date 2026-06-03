// SPDX-FileCopyrightText: 2026 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial

#include "mongoose.h"

// Array elements are stored in a dynamically reallocated array
struct entry {
  char text[20];  // Modifiable via GUI
  bool checked;   // Modifiable via GUI
};

static struct entry *s_arr;   // Backing storage, grown/shrunk via realloc()
static size_t s_count;        // Number of elements in s_arr
static struct entry s_entry;  // Staging area: holds values for read/write/add
static int s_index;           // Current element index, see struct mg_field_set

static struct mg_field fields_arr[] = {
    {"index", MG_VAL_INT, &s_index, sizeof(s_index)},
    {"text", MG_VAL_STR, &s_entry.text, sizeof(s_entry.text)},
    {"checked", MG_VAL_BOOL, &s_entry.checked, sizeof(s_entry.checked)},
    {NULL, MG_VAL_INT, NULL, 0},
};

// Grow s_arr by one slot and append the staged entry. False on OOM
static bool entry_append(void) {
  struct entry *arr =
      (struct entry *) realloc(s_arr, (s_count + 1) * sizeof(*arr));
  if (arr == NULL) return false;
  s_arr = arr;
  s_arr[s_count++] = s_entry;
  return true;
}

// Remove the entry at the given index, shifting the tail down and shrinking
// s_arr by one slot. False if the index is out of range
static bool entry_remove(int index) {
  if (index < 0 || (size_t) index >= s_count) return false;
  memmove(&s_arr[index], &s_arr[index + 1],
          (s_count - (size_t) index - 1) * sizeof(*s_arr));
  s_count--;
  if (s_count == 0) {
    free(s_arr);
    s_arr = NULL;
  } else {
    struct entry *arr = (struct entry *) realloc(s_arr, s_count * sizeof(*arr));
    if (arr != NULL) s_arr = arr;
  }
  return true;
}

// Read, write, delete or append an array element. The framework sets s_index
// before the call: a negative value asks for the total count, an
// out-of-range value means "end of iteration" - both are answered by
// setting s_index accordingly. MG_DASH_ADD builds a new entry from the
// staged fields and appends it regardless of s_index
static bool arr_fn(enum mg_dash_op op, struct mg_dash_user *u) {
  (void) u;
  if (op == MG_DASH_READ) {
    if (s_index < 0) {
      s_index = (int) s_count;
    } else if ((size_t) s_index >= s_count) {
      s_index = -1;
    } else {
      s_entry = s_arr[s_index];
    }
    return true;
  }
  if (op == MG_DASH_WRITE && s_index >= 0 && (size_t) s_index < s_count) {
    s_arr[s_index] = s_entry;
    return true;
  }
  if (op == MG_DASH_DELETE) return entry_remove(s_index);
  if (op == MG_DASH_ADD) return entry_append();
  return false;
}

static struct mg_field_set field_set_arr = {
    "event", fields_arr, arr_fn, &s_index, NULL, NULL,
};

void mg_dash_init(struct mg_mgr *mgr) {
  static struct mg_dash dash;  // Important: keep it static!
  static const char *demo[] = {"Wake up", "Make coffee", "Write code",
                               "Run tests", "Ship it", "Relax", "Sleep"};
  size_t i;
  for (i = 0; i < sizeof(demo) / sizeof(demo[0]); i++) {
    mg_snprintf(s_entry.text, sizeof(s_entry.text), "%s", demo[i]);
    s_entry.checked = (i == 0);
    entry_append();
  }
  MG_DASH_ADD_FIELD_SET(&dash, &field_set_arr);
  mg_mem_files = mg_packed_files;
  mg_http_listen(mgr, MG_HTTP_ADDR, mg_dash_ev_handler, &dash);
}

void mg_dash_poll(struct mg_mgr *mgr) {
  (void) mgr;
}

// On desktop, build with -DMAIN flag to include main().
// On embedded system, run this code in your main() function
#ifdef MAIN
int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_dash_init(&mgr);

  for (;;) {
    mg_mgr_poll(&mgr, 1);
    mg_dash_poll(&mgr);
  }

  return 0;
}
#endif
