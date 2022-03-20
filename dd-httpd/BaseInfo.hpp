#pragma once

#ifndef PROTOCOL
#define PROTOCOL "HTTP/1.1"
#endif

#ifndef SERVER_NAME
#define SERVER_NAME "dd-http/0.0"
#endif

#ifndef NOT_FOUNT
#define NOT_FOUNT 404
#endif

#ifndef OK
#define OK 200
#endif

#ifndef FORBIDDEN
#define FORBIDDEN 403
#endif

#ifndef RESPONSE_DESC_FORBIDDEN
#define RESPONSE_DESC_FORBIDDEN "Forbidden"
#endif

#ifndef CONTENT_LENGTH
#define CONTENT_LENGTH "Content-Length"
#endif

#ifndef CONTENT_TYPE
#define CONTENT_TYPE "Content-Type"
#endif

#ifndef RESPONSE_DESC_OK
#define RESPONSE_DESC_OK "OK"
#endif

#ifndef RESPONSE_DESC_NOT_FOUNT
#define RESPONSE_DESC_NOT_FOUND "Not Found"
#endif


#ifndef RESPONSE_HEAD_DATE
#define RESPONSE_HEAD_DATE "Date"
#endif

#ifndef RESPONSE_HEAD_SERVER
#define RESPONSE_HEAD_SERVER "Server"
#endif

#ifndef CONNECTION
#define CONNECTION "Connection"
#endif

#ifndef KEEP_ALIVE
#define KEEP_ALIVE "Keep-Alive"
#endif

#ifndef CLOSE
#define CLOSE "Close"
#endif

#ifndef TEXT_PLAIN
#define TEXT_PLAIN "text/plain"
#endif

#ifndef RANGE
#define RANGE "Range"
#endif

#ifndef PARTIAL_CONTENT
#define PARTIAL_CONTENT "Partial Content"
#endif

#ifndef PARTIAL_CONTENT_CODE
#define PARTIAL_CONTENT_CODE 206
#endif

#ifndef REQUESTED_RANGE_NOT_SATISFIABLE
#define REQUESTED_RANGE_NOT_SATISFIABLE "Requested Range Not Satisfiable"
#endif

#ifndef REQUESTED_RANGE_NOT_SATISFIABLE_CODE
#define REQUESTED_RANGE_NOT_SATISFIABLE_CODE 416
#endif

#ifndef MULTIPART_BYTERANGES
#define MULTIPART_BYTERANGES "multipart/byteranges"
#endif

#ifndef ACCEPT_RANGES
#define ACCEPT_RANGES "Accept-Ranges"
#endif

#ifndef BYTES
#define BYTES "bytes"
#endif

#ifndef CONTENT_RANGE
#define CONTENT_RANGE "Content-Range"
#endif
