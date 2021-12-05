#ifndef _SPDK_HELPERS_H_
#define _SPDK_HELPERS_H_

#include "spdk/nvme.h"

extern struct spdk_nvme_ctrlr *skv_spdk_global_ctrlr;
extern struct spdk_nvme_ns    *skv_spdk_global_ns;
extern struct spdk_nvme_qpair *skv_spdk_global_qpair;
extern size_t                 *skv_spdk_global_counter;

extern u_int64_t skv_spdk_g_tsc_rate;
extern u_int32_t skv_spdk_max_xfer_size;
extern u_int32_t skv_spdk_entries;


// struct SpdkRequest {
//     key__t key;
//     ptr__t ofs;
//     void *buff;
//     size_t *counter;
//     bool is_value;
//     struct spdk_nvme_ns	*ns;
//     struct spdk_nvme_qpair *qpair;
//     struct timespec start;
//     WorkerArg *warg;
// }

// static inline struct SpdkRequest *init_spdk_request {
//     struct SpdkRequest *req = malloc(sizeof(struct SpdkRequest));
//     if (req == NULL) {
//         return req;
//     }


// }V

void cleanup(void);

int spdk_init(void);

#endif /* _SPDK_HELPERS_H_ */