/*
 * PJLIB settings.
 */
/* Optimizations */
#define PJ_AUTOCONF 1
#define PJ_HAS_PENTIUM 0
#define PJ_IS_LITTLE_ENDIAN 1
#define PJ_IS_BIG_ENDIAN 0
#define PJ_JNI_HAS_JNI_ONLOAD 0
#define PJ_CONFIG_ANDROID 1
#define DEPRECATED_FOR_TICKET_1902 0
#define PJ_ENABLE_EXTRA_CHECK   0

#define PJ_OS_HAS_CHECK_STACK		0

#define PJ_HAS_IPV6 1

/* Enable TCP support. */
#define PJ_HAS_TCP 1

#define PJ_HAS_SSL_SOCK 1
#define PJSIP_HAS_TLS_TRANSPORT 1

#define PJ_QOS_IMPLEMENTATION    PJ_QOS_BSD

#define PJ_STUN_KEEP_ALIVE_SEC 40

/*
 * PJMEDIA settings
 */
//Conference ports?
#define PJMEDIA_CONF_USE_SWITCH_BOARD 0

/* Select codecs to disable */
#define PJMEDIA_HAS_L16_CODEC               0

/* Fine tune Speex's default settings for best performance/quality */
#define PJMEDIA_CODEC_SPEEX_DEFAULT_QUALITY 5

/* For CPU reason, disable speex AEC and use the echo suppressor. */
#define PJMEDIA_HAS_SPEEX_AEC               0

/* Previously, resampling is disabled due to performance reason and
 * this condition prevented some 'light' wideband codecs (e.g: G722.1)
 * to work along with narrowband codecs. Lately, some tests showed
 * that 16kHz <-> 8kHz resampling using libresample small filter was
 * affordable on ARM9 222 MHz, so here we decided to enable resampling.
 * Note that it is important to make sure that libresample is created
 * using small filter. For example PJSUA_DEFAULT_CODEC_QUALITY must
 * be set to 3 or 4 so pjsua-lib will apply small filter resampling.
 */
//#define PJMEDIA_RESAMPLE_IMP              PJMEDIA_RESAMPLE_NONE
#define PJMEDIA_RESAMPLE_IMP                PJMEDIA_RESAMPLE_LIBRESAMPLE

/* Use the lighter WSOLA implementation */
#define PJMEDIA_WSOLA_IMP                   PJMEDIA_WSOLA_IMP_WSOLA_LITE

/* We probably need more buffers especially if MDA audio backend
 * is used, so increase the limit
 */
//  #define PJMEDIA_SOUND_BUFFER_COUNT          32

//  #define PJMEDIA_SND_DEFAULT_REC_LATENCY 150
//  #define PJMEDIA_SND_DEFAULT_PLAY_LATENCY 200
/* Prefer local codec when negotiate SDP. */
#define PJMEDIA_SDP_NEG_PREFER_REMOTE_CODEC_ORDER    0

/* Include raw jitter statistics in RTCP statistics. */
#define PJMEDIA_RTCP_STAT_HAS_RAW_JITTER 1

/* Include IP Delay Variation in RTCP statistics. */
#define PJMEDIA_RTCP_STAT_HAS_IPDV  1

/* Enable rtcp-xr */
#define PJMEDIA_HAS_RTCP_XR 1
#define PJMEDIA_STREAM_ENABLE_XR    1

/* Enable log zero frame */
#define PJMEDIA_HAS_STREAM_LOG_JB_ZERO_FRAME    1

/* Fix google voice problem with RTP DTMF events */
#define PJMEDIA_RTP_PT_TELEPHONE_EVENTS          101
#define PJMEDIA_RTP_PT_TELEPHONE_EVENTS_STR      "101"

/*
 * PJSIP settings.
 */

#define PJSIP_CHECK_VIA_SENT_BY 0

#define PJSIP_HAS_DIGEST_MJMD5_AUTH 1

/* Enable TCP socket keep-alive. */
#if defined(PJ_HAS_TCP) && PJ_HAS_TCP!=0
    #define PJSIP_HAS_TCP_SOCKET_KEEPALIVE  1
#endif

#if defined(PJSIP_HAS_TCP_SOCKET_KEEPALIVE) && PJSIP_HAS_TCP_SOCKET_KEEPALIVE!=0
/* Enable TCP socket keep-alive. */
#define PJSIP_TCP_SOCKET_KEEP_ALIVE 1
/* Disable application keep-alive for TCP transport. */
#define PJSIP_TCP_KEEP_ALIVE_INTERVAL 0

#define PJSIP_TCP_SOCKET_KEEP_ALIVE_IDLE 60
#define PJSIP_TCP_SOCKET_KEEP_ALIVE_INTERVAL 1
#define PJSIP_TCP_SOCKET_KEEP_ALIVE_CNT 3
#endif

/*
 * PJSUA settings.
 */

/* Default codec quality, previously was set to 5, however it is now
 * set to 4 to make sure pjsua instantiates resampler with small filter.
 */
#define PJSUA_DEFAULT_CODEC_QUALITY         4

#define PJSUA_DEFAULT_ILBC_MODE 20

//We need hi availablitly so retry each minute if network fails -
//all the more so as we unregister if network not available for us
#define PJSUA_REG_RETRY_INTERVAL	60

/* Set maximum number of dialog/transaction/calls to minimum */
#define PJSIP_MAX_TSX_COUNT 		31
#define PJSIP_MAX_DIALOG_COUNT 		31
#define PJSUA_MAX_CALLS			4

/* Other pjsua settings */
#define PJSUA_MAX_ACC			10
#define PJSUA_MAX_PLAYERS			4
#define PJSUA_MAX_RECORDERS			4
#define PJSUA_MAX_CONF_PORTS		(PJSUA_MAX_CALLS+2*PJSUA_MAX_PLAYERS)
#define PJSUA_MAX_BUDDIES			32

/* AB-10721: SBC only support legacy contact rewrite method. */
#define PJSUA_CONTACT_REWRITE_METHOD     1

#define PJ_SOCK_HAS_GETADDRINFO 1

#define PJ_GETHOSTIP_DISABLE_LOCAL_RESOLUTION 1

#define PJ_ICE_MAX_CAND 32
#define PJ_ICE_MAX_CHECKS (PJ_ICE_MAX_CAND * 16)
#define PJSIP_MAX_PKT_LEN			40000

#define PJ_AUTOCONF 1