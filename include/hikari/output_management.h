/* Script function and purpose: zwlr_output_manager_v1 -- the protocol
wlr-randr, kanshi and wdisplays speak to reconfigure displays at runtime. */

#if !defined(HIKARI_OUTPUT_MANAGEMENT_H)
#define HIKARI_OUTPUT_MANAGEMENT_H

struct hikari_server;

void
hikari_output_management_init(struct hikari_server *server);

void
hikari_output_management_fini(struct hikari_server *server);

/* Function purpose: Republish the compositor's current output configuration to
every bound client.

This is the compositor's half of the protocol's contract: wlroots tracks nothing
on its own, so anything that changes an output's geometry, mode, scale or
presence has to say so here or clients keep acting on a stale picture. Worse
than stale, in fact -- a configuration carrying an out-of-date serial is
cancelled outright before the compositor ever sees it, so a manager that falls
behind makes every subsequent client request fail.

Safe to call at any time, including before the manager exists and while an apply
is in flight; both cases return without doing anything. */
void
hikari_output_management_broadcast(void);

#endif
