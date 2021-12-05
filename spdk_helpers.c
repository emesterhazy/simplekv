#include <stdio.h>

#include "spdk_helpers.h"
#include "db_types.h"


#include "spdk/env.h"
#include "spdk/stdinc.h"
#include "spdk/nvme.h"

struct spdk_nvme_ctrlr *skv_spdk_global_ctrlr = NULL;
struct spdk_nvme_ns	   *skv_spdk_global_ns    = NULL;
struct spdk_nvme_qpair *skv_spdk_global_qpair = NULL;
size_t                 *skv_spdk_global_counter = NULL;

u_int64_t skv_spdk_g_tsc_rate;
u_int32_t skv_spdk_max_xfer_size;
u_int32_t skv_spdk_entries;

static bool _probe_cb(void *cb_ctx, const struct spdk_nvme_transport_id *trid,
	           struct spdk_nvme_ctrlr_opts *opts) {
    /* TODO: Why is this device hardcoded? What even is it? */
	return strcmp(trid->traddr, "0000:01:00.0") == 0;
}

static void _attach_cb(void *cb_ctx, const struct spdk_nvme_transport_id *trid,
	           struct spdk_nvme_ctrlr *ctrlr, const struct spdk_nvme_ctrlr_opts *opts) {
	printf("Attached to %s\n", trid->traddr);

    skv_spdk_global_ctrlr = ctrlr;
	skv_spdk_global_ns = spdk_nvme_ctrlr_get_ns(ctrlr, 1);
	skv_spdk_global_qpair = spdk_nvme_ctrlr_alloc_io_qpair(ctrlr, NULL, 0);
	skv_spdk_max_xfer_size = spdk_nvme_ns_get_max_io_xfer_size(skv_spdk_global_ns);
	skv_spdk_entries = (BLK_SIZE - 1) / skv_spdk_max_xfer_size + 3;
    printf("entries %u\n", skv_spdk_entries);
}

void cleanup(void) {
	struct spdk_nvme_detach_ctx *detach_ctx = NULL;

    spdk_nvme_detach_async(skv_spdk_global_ctrlr, &detach_ctx);

	while (detach_ctx && spdk_nvme_detach_poll_async(detach_ctx) == -EAGAIN) {
		;
	}
}

int spdk_init(void) {
    struct spdk_env_opts opts;
    spdk_env_opts_init(&opts);
   	opts.name = "simple_kv";
	opts.shm_id = 0;
    opts.main_core = 0;
    // TODO: mask should be determined programatically, mars only has 6 cores
    opts.core_mask = "[0-5]"; 
    if (spdk_env_init(&opts) < 0) {
        fprintf(stderr, "Unable to initialized SPDK env\n");
        return 1;
    }


	printf("Initializing NVMe Controllers\n");

	if(spdk_nvme_probe(NULL, NULL, _probe_cb, _attach_cb, NULL) != 0) {
		fprintf(stderr, "spdk_nvme_probe() failed\n");
		cleanup();
		return 1;
	}

	if (skv_spdk_global_ctrlr == NULL) {
		fprintf(stderr, "no NVMe controllers found\n");
		cleanup();
		return 1;
	}

    skv_spdk_g_tsc_rate = spdk_get_ticks_hz();

    return 0;
}

// static int _skv_spdk_get_leaf_containing(keyt__t key, Node *node, ptr__t *node_offset) {
//     Request *req = init_request
// }

// static int skv_spdk_get_leaf_containing(keyt__t key, Node *node) {
//     ptr__t x = 0;
//     return _skv_spdk_get_leaf_containing(key, node, &x);
// }

// static void skv_spdk_lookup_key(struct Query *query) {
//     Node *node = spdk_zmalloc(BLK_SIZE, sizof(Node), SPDK_ENV_SOCKET_ID_ANY, SPDK_MALLOC_DMA);
//     if (node == NULL) {
//         /* TODO: handle better */
//         cleanup();
//         exit(1);
//     }

// }