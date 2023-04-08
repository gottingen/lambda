
#include "lambda/core/ann_index.h"
#include "lambda/hnsw/hnswlib_index.h"
#include <turbo/log/logging.h>

namespace lambda {

    AnnIndex::AnnIndex(){
    }

    AnnIndex::~AnnIndex() {
    }

    // create index
    ann_index_ptr AnnIndex::create_ann_index(const IndexConfig &index_conf) {
        ann_index_ptr p = nullptr;
        if (!index_conf.is_double_switch) {
            p = create_single_index(index_conf);
        } else {
            TURBO_CHECK(false)<<"not impl now";
        }
        return p;
    }

    ann_index_ptr AnnIndex::create_single_index(const IndexConfig &index_conf) {
        ann_index_ptr p = nullptr;
        switch (index_conf.index_type) {
        case EngineType::ENGINE_HNSWLIB: {
                p.reset(new hnsw_index);
                break;
            }
            default: {
                TURBO_LOG(ERROR) << "Unsupported engine type:" << static_cast<size_t>(index_conf.index_type);
                break;
            }
        }
        if (p != nullptr) {
            p->set_conf(index_conf);
        }
        return p;
    }
}  // namespace lambda
