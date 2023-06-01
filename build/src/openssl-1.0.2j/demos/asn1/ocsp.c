
#include <openssl/asn1.h>
#include <openssl/asn1t.h>
#include <openssl/x509v3.h>


/* Request Structures */
typedef struct Request_st {
    ASN1_INTEGER *version;
    GENERAL_NAME *requestorName;
    struct stack_st_Request *requestList;
    struct stack_st_X509_EXTENSION *requestExtensions;
} Request;

typedef struct TBSRequest_st {
    Request *tbsRequest;
    struct Signature_st *optionalSignature;
} TBSRequest;

typedef struct Signature_st {
    X509_ALGOR *signatureAlgorithm;
    ASN1_BIT_STRING *signature;
    struct stack_st_X509 *certs;
} Signature;

typedef struct CertID_st {
    X509_ALGOR *hashAlgorithm;
    ASN1_OCTET_STRING *issuerNameHash;
    ASN1_OCTET_STRING *issuerKeyHash;
    ASN1_INTEGER *certificateSerialNumber;
} CertID;

typedef struct Request_st {
    CertID *reqCert;
    struct stack_st_X509_EXTENSION *singleRequestExtensions;
} Request;

/* Response structures */

typedef struct ResponseBytes_st {
    ASN1_OBJECT *responseType;
    ASN1_OCTET_STRING *response;
} ResponseBytes;

typedef struct OCSPResponse_st {
    ASN1_ENUMERATED *responseStatus;
    ResponseBytes *responseBytes;
} OCSPResponse;

typedef struct ResponderID_st {
    int type;
    union {
        X509_NAME *byName;
        ASN1_OCTET_STRING *byKey;
    } d;
} ResponderID;

typedef struct ResponseData_st {
    ASN1_INTEGER *version;
    ResponderID *responderID;
    ASN1_GENERALIZEDTIME *producedAt;
    struct stack_st_SingleResponse *responses;
    struct stack_st_X509_EXTENSION *responseExtensions;
} ResponseData;

typedef struct BasicOCSPResponse_st {
    ResponseData *tbsResponseData;
    X509_ALGOR *signatureAlgorithm;
    ASN1_BIT_STRING *signature;
    struct stack_st_X509 *certs;
} BasicOCSPResponse;

typedef struct RevokedInfo_st {
    ASN1_GENERALIZEDTIME *revocationTime;
    ASN1_ENUMERATED *revocationReason;
} RevokedInfo;

typedef struct CertStatus_st {
    int type;
    union {
        ASN1_NULL *good;
        RevokedInfo *revoked;
        ASN1_NULL *unknown;
    } d;
} CertStatus;

typedef struct {
    CertID *certID;
    CertStatus *certStatus;
    ASN1_GENERALIZEDTIME *thisUpdate;
    ASN1_GENERALIZEDTIME *nextUpdate;
    STACK_OF(X509_EXTENSION) *singleExtensions;
} SingleResponse;

typedef struct {
    X509_NAME *issuer;
    STACK_OF(ACCESS_DESCRIPTION) *locator;
} ServiceLocator;

/* Now the ASN1 templates */

IMPLEMENT_COMPAT_ASN1(X509);
IMPLEMENT_COMPAT_ASN1(X509_ALGOR);
// IMPLEMENT_COMPAT_ASN1(X509_EXTENSION);
IMPLEMENT_COMPAT_ASN1(GENERAL_NAME);
IMPLEMENT_COMPAT_ASN1(X509_NAME);

ASN1_SEQUENCE(X509_EXTENSION) = {
        ASN1_SIMPLE(X509_EXTENSION, object, ASN1_OBJECT),
        ASN1_OPT(X509_EXTENSION, critical, ASN1_BOOLEAN),
        ASN1_SIMPLE(X509_EXTENSION, value, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(X509_EXTENSION);


ASN1_SEQUENCE(Signature) = {
        ASN1_SIMPLE(Signature, signatureAlgorithm, X509_ALGOR),
        ASN1_SIMPLE(Signature, signature, ASN1_BIT_STRING),
        ASN1_SEQUENCE_OF(Signature, certs, X509)
} ASN1_SEQUENCE_END(Signature);

ASN1_SEQUENCE(CertID) = {
        ASN1_SIMPLE(CertID, hashAlgorithm, X509_ALGOR),
        ASN1_SIMPLE(CertID, issuerNameHash, ASN1_OCTET_STRING),
        ASN1_SIMPLE(CertID, issuerKeyHash, ASN1_OCTET_STRING),
        ASN1_SIMPLE(CertID, certificateSerialNumber, ASN1_INTEGER)
} ASN1_SEQUENCE_END(CertID);

ASN1_SEQUENCE(Request) = {
        ASN1_SIMPLE(Request, reqCert, CertID),
        ASN1_EXP_SEQUENCE_OF_OPT(Request, singleRequestExtensions, X509_EXTENSION, 0)
} ASN1_SEQUENCE_END(Request);

ASN1_SEQUENCE(TBSRequest) = {
        ASN1_EXP_OPT(TBSRequest, version, ASN1_INTEGER, 0),
        ASN1_EXP_OPT(TBSRequest, requestorName, GENERAL_NAME, 1),
        ASN1_SEQUENCE_OF(TBSRequest, requestList, Request),
        ASN1_EXP_SEQUENCE_OF_OPT(TBSRequest, requestExtensions, X509_EXTENSION, 2)
} ASN1_SEQUENCE_END(TBSRequest);

ASN1_SEQUENCE(OCSPRequest) = {
        ASN1_SIMPLE(OCSPRequest, tbsRequest, TBSRequest),
        ASN1_EXP_OPT(OCSPRequest, optionalSignature, Signature, 0)
} ASN1_SEQUENCE_END(OCSPRequest);

/* Response templates */

ASN1_SEQUENCE(ResponseBytes) = {
            ASN1_SIMPLE(ResponseBytes, responseType, ASN1_OBJECT),
            ASN1_SIMPLE(ResponseBytes, response, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(ResponseBytes);

ASN1_SEQUENCE(OCSPResponse) = {
        ASN1_SIMPLE(OCSPResponse, responseStatus, ASN1_ENUMERATED),
        ASN1_EXP_OPT(OCSPResponse, responseBytes, ResponseBytes, 0)
} ASN1_SEQUENCE_END(OCSPResponse);

ASN1_CHOICE(ResponderID) = {
           ASN1_EXP(ResponderID, d.byName, X509_NAME, 1),
           ASN1_IMP(ResponderID, d.byKey, ASN1_OCTET_STRING, 2)
} ASN1_CHOICE_END(ResponderID);

ASN1_SEQUENCE(RevokedInfo) = {
        ASN1_SIMPLE(RevokedInfo, revocationTime, ASN1_GENERALIZEDTIME),
        ASN1_EXP_OPT(RevokedInfo, revocationReason, ASN1_ENUMERATED, 0)
} ASN1_SEQUENCE_END(RevokedInfo);

ASN1_CHOICE(CertStatus) = {
        ASN1_IMP(CertStatus, d.good, ASN1_NULL, 0),
        ASN1_IMP(CertStatus, d.revoked, RevokedInfo, 1),
        ASN1_IMP(CertStatus, d.unknown, ASN1_NULL, 2)
} ASN1_CHOICE_END(CertStatus);

ASN1_SEQUENCE(SingleResponse) = {
           ASN1_SIMPLE(SingleResponse, certID, CertID),
           ASN1_SIMPLE(SingleResponse, certStatus, CertStatus),
           ASN1_SIMPLE(SingleResponse, thisUpdate, ASN1_GENERALIZEDTIME),
           ASN1_EXP_OPT(SingleResponse, nextUpdate, ASN1_GENERALIZEDTIME, 0),
           ASN1_EXP_SEQUENCE_OF_OPT(SingleResponse, singleExtensions, X509_EXTENSION, 1)
} ASN1_SEQUENCE_END(SingleResponse);

ASN1_SEQUENCE(ResponseData) = {
           ASN1_EXP_OPT(ResponseData, version, ASN1_INTEGER, 0),
           ASN1_SIMPLE(ResponseData, responderID, ResponderID),
           ASN1_SIMPLE(ResponseData, producedAt, ASN1_GENERALIZEDTIME),
           ASN1_SEQUENCE_OF(ResponseData, responses, SingleResponse),
           ASN1_EXP_SEQUENCE_OF_OPT(ResponseData, responseExtensions, X509_EXTENSION, 1)
} ASN1_SEQUENCE_END(ResponseData);

ASN1_SEQUENCE(BasicOCSPResponse) = {
           ASN1_SIMPLE(BasicOCSPResponse, tbsResponseData, ResponseData),
           ASN1_SIMPLE(BasicOCSPResponse, signatureAlgorithm, X509_ALGOR),
           ASN1_SIMPLE(BasicOCSPResponse, signature, ASN1_BIT_STRING),
           ASN1_EXP_SEQUENCE_OF_OPT(BasicOCSPResponse, certs, X509, 0)
} ASN1_SEQUENCE_END(BasicOCSPResponse);
