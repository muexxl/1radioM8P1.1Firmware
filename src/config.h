
#ifndef CONFIG_H
#define CONFIG_H

namespace config{
    const int REGULAR_CHECK_INTERVAL = 300;
    const int CONNECTION_TIMEOUT = 1000;
    const int SEND_BUFFER_SIZE = 600;
    const int RTCM_BUFFER_SIZE = 300;
    const int MAX_RTCM_MSG_SIZE = 0xff;
}

#endif //CONFIG_H