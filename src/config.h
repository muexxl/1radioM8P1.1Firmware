
#ifndef CONFIG_H
#define CONFIG_H

namespace config{
    const int REGULAR_CHECK_INTERVAL = 300;
    const int CONNECTION_TIMEOUT = 1000;
    const int SEND_BUFFER_SIZE = 700;
    const int RTCM_BUFFER_SIZE = 400;
    const int MAX_RTCM_MSG_SIZE = 0xff;
    const uint32_t UBLOX_MIN_TIME_BETWEEN_SUBMISSION_IN_MS = 50;
}

#endif //CONFIG_H