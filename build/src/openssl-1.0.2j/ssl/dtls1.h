#ifndef HEADER_DTLS1_H
#define HEADER_DTLS1_H

#include <openssl/buffer.h>
#include <openssl/pqueue.h>
#ifdef OPENSSL_SYS_VMS
#include <resource.h>
#include <sys/timeb.h>
#endif
#ifdef OPENSSL_SYS_WIN32
#include <winsock.h>
#elif defined(OPENSSL_SYS_NETWARE) && !defined(_WINSOCK2API_)
#include <sys/timeval.h>
#else
#if defined(OPENSSL_SYS_VXWORKS)
#include <sys/times.h>
#else
#include <sys/time.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DTLS1_VERSION             0xFEFF
#define DTLS1_2_VERSION           0xFEFD
#define DTLS_MAX_VERSION          DTLS1_2_VERSION
#define DTLS1_VERSION_MAJOR       0xFE

#define DTLS1_BAD_VER             0x0100

#define DTLS1_COOKIE_LENGTH       256

#define DTLS1_RT_HEADER_LENGTH    13

#define DTLS1_HM_HEADER_LENGTH    12

#define DTLS1_HM_BAD_FRAGMENT     -2
#define DTLS1_HM_FRAGMENT_RETRY   -3

#define DTLS1_CCS_HEADER_LENGTH   1

#ifdef DTLS1_AD_MISSING_HANDSHAKE_MESSAGE
#define DTLS1_AL_HEADER_LENGTH    7
#else
#define DTLS1_AL_HEADER_LENGTH    2
#endif

#ifndef OPENSSL_NO_SSL_INTERN

#ifndef OPENSSL_NO_SCTP
#define DTLS1_SCTP_AUTH_LABEL     "EXPORTER_DTLS_OVER_SCTP"
#endif

#define DTLS1_MAX_MTU_OVERHEAD    48

typedef struct dtls1_bitmap_st {
    unsigned long map;
    unsigned char max_seq_num[8];
} DTLS1_BITMAP;

struct dtls1_retransmit_state {
    EVP_CIPHER_CTX *enc_write_ctx;
    EVP_MD_CTX *write_hash;
#ifndef OPENSSL_NO_COMP
    COMP_CTX *compress;
#else
    char *compress;
#endif
    SSL_SESSION *session;
    unsigned short epoch;
};

struct hm_header_st {
    unsigned char type;
    unsigned long msg_len;
    unsigned short seq;
    unsigned long frag_off;
    unsigned long frag_len;
    unsigned int is_ccs;
    struct dtls1_retransmit_state saved_retransmit_state;
};

struct ccs_header_st {
    unsigned char type;
    unsigned short seq;
};

struct dtls1_timeout_st {
    unsigned int read_timeouts;
    unsigned int write_timeouts;
    unsigned int num_alerts;
};

typedef struct record_pqueue_st {
    unsigned short epoch;
    pqueue q;
} record_pqueue;

typedef struct hm_fragment_st {
    struct hm_header_st msg_header;
    unsigned char *fragment;
    unsigned char *reassembly;
} hm_fragment;

typedef struct dtls1_state_st {
    unsigned int send_cookie;
    unsigned char cookie[DTLS1_COOKIE_LENGTH];
    unsigned char rcvd_cookie[DTLS1_COOKIE_LENGTH];
    unsigned int cookie_len;
    unsigned short r_epoch;
    unsigned short w_epoch;
    DTLS1_BITMAP bitmap;
    DTLS1_BITMAP next_bitmap;
    unsigned short handshake_write_seq;
    unsigned short next_handshake_write_seq;
    unsigned short handshake_read_seq;
    unsigned char last_write_sequence[8];
    record_pqueue unprocessed_rcds;
    record_pqueue processed_rcds;
    pqueue buffered_messages;
    pqueue sent_messages;
    record_pqueue buffered_app_data;
    unsigned int listen;
    unsigned int link_mtu;
    unsigned int mtu;
    struct hm_header_st w_msg_hdr;
    struct hm_header_st r_msg_hdr;
    struct dtls1_timeout_st timeout;
    struct timeval next_timeout;
    unsigned short timeout_duration;
    unsigned char alert_fragment[DTLS1_AL_HEADER_LENGTH];
    unsigned int alert_fragment_len;
    unsigned char handshake_fragment[DTLS1_HM_HEADER_LENGTH];
    unsigned int handshake_fragment_len;
    unsigned int retransmitting;
    unsigned int change_cipher_spec_ok;
#ifndef OPENSSL_NO_SCTP
    int next_state;
    int shutdown_received;
#endif
} DTLS1_STATE;

typedef struct dtls1_record_data_st {
    unsigned char *packet;
    unsigned int packet_length;
    SSL3_BUFFER rbuf;
    SSL3_RECORD rrec;
#ifndef OPENSSL_NO_SCTP
    struct bio_dgram_sctp_rcvinfo recordinfo;
#endif
} DTLS1_RECORD_DATA;

#endif

#define DTLS1_TMO_READ_COUNT    2
#define DTLS1_TMO_WRITE_COUNT   2
#define DTLS1_TMO_ALERT_COUNT   12

#ifdef __cplusplus
}
#endif
#endif
