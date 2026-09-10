/* Script function and purpose: zwp_pointer_constraints_v1 policy -- which
 * constraint is active, and what a client's hold on the pointer means for the
 * cursor. The protocol implementation itself lives in wlroots; everything here
 * is the compositor-side decision it leaves to us. */

#if !defined(HIKARI_POINTER_CONSTRAINTS_H)
#define HIKARI_POINTER_CONSTRAINTS_H

#include <stdbool.h>

#include <wayland-server-core.h>

struct hikari_server;
struct hikari_view;
struct wlr_surface;
struct wlr_pointer_constraint_v1;

/* Class purpose: Per-constraint wrapper holding the listeners hikari attaches
to one wlr_pointer_constraint_v1. Both MUST be removed before wlroots destroys
the constraint -- it asserts the listener lists are empty, and NDEBUG erases
that assert in release builds, so a missed removal corrupts a freed list
silently rather than aborting. See constraint_destroy_handler. */
struct hikari_pointer_constraint {
  struct wlr_pointer_constraint_v1 *wlr_constraint;

  struct wl_listener set_region;
  struct wl_listener destroy;
};

void
hikari_pointer_constraints_setup(struct hikari_server *server);

void
hikari_pointer_constraints_fini(struct hikari_server *server);

/* Function purpose: Re-decide which constraint should be active, from the
seat's current pointer focus. The single entry point for activation -- call it
wherever seat pointer focus changes. */
void
hikari_pointer_constraint_refresh(void);

/* Function purpose: Take the active constraint down, honouring the client's
cursor position hint. For mode changes and the lock screen. */
void
hikari_pointer_constraint_deactivate(void);

/* Function purpose: Take the active constraint down WITHOUT honouring the
cursor hint, because the caller is about to warp the pointer somewhere of its
own choosing and the hint would fight it. D-028 Ruling 8. */
void
hikari_pointer_constraint_break_for_warp(void);

bool
hikari_pointer_constraint_is_locked(void);

/* Function purpose: Clamp one motion delta against a confined pointer's region,
in layout coordinates. False when nothing is confined, in which case the caller
moves the cursor as it normally would; true means *out_x / *out_y hold where the
cursor should go, which may be exactly where it already is. */
bool
hikari_pointer_constraint_confine(
    double dx, double dy, double *out_x, double *out_y);

/* Function purpose: True when the active constraint belongs to this view's
surface. Per-surface deliberately: a constraint held by some OTHER surface, with
a warp landing on this one, is the pointer going somewhere else and still breaks
it. D-028 Ruling 9. */
bool
hikari_pointer_constraint_holds_view(struct hikari_view *view);

#endif
