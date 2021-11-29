#ifndef _VLGL_JS_DISPLAY_WS
#define _VLGL_JS_DISPLAY_WS

#include <esp_system.h>
#include <esp_http_server.h>



#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif


/* Max supported HTTP request header length */
#define HTTPD_MAX_REQ_HDR_LEN CONFIG_HTTPD_MAX_REQ_HDR_LEN
/* Max supported HTTP request URI length */
#define HTTPD_MAX_URI_LEN CONFIG_HTTPD_MAX_URI_LEN

#define PARSER_BLOCK_SIZE  128
/* Calculate the maximum size needed for the scratch buffer */
#define HTTPD_SCRATCH_BUF  MAX(HTTPD_MAX_REQ_HDR_LEN, HTTPD_MAX_URI_LEN)

/**
 * @brief A database of all the open sockets in the system.
 */
struct sock_db {
    int fd;                                 /*!< The file descriptor for this socket */
    void *ctx;                              /*!< A custom context for this socket */
    bool ignore_sess_ctx_changes;           /*!< Flag indicating if session context changes should be ignored */
    void *transport_ctx;                    /*!< A custom 'transport' context for this socket, to be used by send/recv/pending */
    httpd_handle_t handle;                  /*!< Server handle */
    httpd_free_ctx_fn_t free_ctx;      /*!< Function for freeing the context */
    httpd_free_ctx_fn_t free_transport_ctx; /*!< Function for freeing the 'transport' context */
    httpd_send_func_t send_fn;              /*!< Send function for this socket */
    httpd_recv_func_t recv_fn;              /*!< Receive function for this socket */
    httpd_pending_func_t pending_fn;        /*!< Pending function for this socket */
    uint64_t lru_counter;                   /*!< LRU Counter indicating when the socket was last used */
    bool lru_socket;                        /*!< Flag indicating LRU socket */
    char pending_data[PARSER_BLOCK_SIZE];   /*!< Buffer for pending data to be received */
    size_t pending_len;                     /*!< Length of pending data to be received */
#ifdef CONFIG_HTTPD_WS_SUPPORT
    bool ws_handshake_done;                 /*!< True if it has done WebSocket handshake (if this socket is a valid WS) */
    bool ws_close;                          /*!< Set to true to close the socket later (when WS Close frame received) */
    esp_err_t (*ws_handler)(httpd_req_t *r);   /*!< WebSocket handler, leave to null if it's not WebSocket */
    bool ws_control_frames;                         /*!< WebSocket flag indicating that control frames should be passed to user handlers */
#endif
};

/**
 * @brief   Auxiliary data structure for use during reception and processing
 *          of requests and temporarily keeping responses
 */
struct httpd_req_aux {
    struct sock_db *sd;                             /*!< Pointer to socket database */
    char            scratch[HTTPD_SCRATCH_BUF + 1]; /*!< Temporary buffer for our operations (1 byte extra for null termination) */
    size_t          remaining_len;                  /*!< Amount of data remaining to be fetched */
    char           *status;                         /*!< HTTP response's status code */
    char           *content_type;                   /*!< HTTP response's content type */
    bool            first_chunk_sent;               /*!< Used to indicate if first chunk sent */
    unsigned        req_hdrs_count;                 /*!< Count of total headers in request packet */
    unsigned        resp_hdrs_count;                /*!< Count of additional headers in response packet */
    struct resp_hdr {
        const char *field;
        const char *value;
    } *resp_hdrs;                                   /*!< Additional headers in response packet */
    struct http_parser_url url_parse_res;           /*!< URL parsing result, used for retrieving URL elements */
#ifdef CONFIG_HTTPD_WS_SUPPORT
    bool ws_handshake_detect;                       /*!< WebSocket handshake detection flag */
    httpd_ws_type_t ws_type;                        /*!< WebSocket frame type */
    bool ws_final;                                  /*!< WebSocket FIN bit (final frame or not) */
#endif
};


void vlgl_js_display_ws_init() ;

#endif