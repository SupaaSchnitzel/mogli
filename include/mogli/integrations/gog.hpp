#pragma once

#include "../logging.hpp"

namespace mogli::integrations {
    class GOGIntegration final {
    private:
        mogli::log::LoggerPtr logger;
    
    public:
        GOGIntegration();
        void init();
        void deinit();
    };
}