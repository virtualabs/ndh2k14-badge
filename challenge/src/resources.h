#ifndef __INC_RESOURCES_H
#define __INC_RESOURCES_H

/* Stage 1 hash for 'HelpPlz!' */
#define STAGE1_HASH         "\xF7\xCB\x69\xA7\x06\x01\x46\xFC"
#define STAGE1_WELCOME      0x00
#define STAGE1_WELCOME_STR  "\x82\x93\x38\xf7\x05\x08\x5f\xa8\xe6\xa6\x8c\x17\xbd\xb2\xc0\xaa" \
                            "\x29\xb3\xa2\xfd\xfc\xa6\xf1\x96\x4b\xaa\xc6\x76\xe7\x66\x2a\x1c" \
                            "\x39\x69\xb2\x3e\x71\x65\x99\xc1\xf6\x5b\x06\xb7\x2f\x03\xdc\x22" \
                            "\x16\xf6\xbe\xfc\x0d\xdd\x88\xad\xdc\xf8\xb9\xf7\xb2\x26\x77\x6c" \
                            "\xe4\x5a\x1a\x8f\xc8\xed\xde\x41\x35\x51\xde\x12\x0e\x2e\xec\x3c" \
                            "\xf6\x5f\x0e\x0d\x6b\xe1\x83\x08\xbd\xc3\xc9\x0b\x4a\xf0\x8a\x1d" \
                            "\x4a\xe0\xae\xad\x92\x64"
#define STAGE1_WELCOME_LEN  102

/* Stage 2 hash for 'AvEjUlEs' */
#define STAGE2_HASH         "\x44\x84\x73\xA8\x64\x40\x6C\x34"
#define STAGE2_WELCOME      0x01
#define STAGE2_WELCOME_STR  "\x38\xcf\x0b\xe2\x62\x49\x4a\x32\x2b\xb1\x95\xb2\xe2\xe9\x08\x6b" \
                            "\x72\x5d\xad\xe1\x51\xc1\x83\xf7\xa4\xc2\x20\xcb\x63\xf9\xcf\x3f" \
                            "\xb7\xf5\x4b\x26\x45\x95\x77\x96\x36\x93\xab\x00\x30\x81\x99\x54" \
                            "\x4e\x2d\x07\x33\x36\xf1\x50\x9d\xad\x4c\x78\x2a\xc0\xce\xde\x10" \
                            "\x6a\xc2\x0e\xd1\x4d\xe6\xfa\xa8\x19\xf0\x15\x4a\x99\xfe\xb1\x89" \
                            "\x60\xa2\x05\x9d\x72\x32\x5f\xa7\xd9\x70\xc1\x8a\x21\xb1\xc9\x19" \
                            "\x63\x41\x80\x60\xf0\x01\x14\x43\xf0\x84\xa3\x31\x73\x60\x49\xe7"
#define STAGE2_WELCOME_LEN  112
#define STAGE2_PAYLOAD      0x02
#define STAGE2_PAYLOAD_STR  "\x4b\x97\x4e\xb6\x11\x4d\x4a\x24\x3c\xab\x92\xeb\xa1\xea\x09\x78" \
                            "\x37\x58\xfe\xe9\x51\xac\xcb\xfb\xc2\xa6\x3d\x9c\x45"
#define STAGE2_PAYLOAD_LEN  29


/* Stage 3 hash for 'Pu1lD0wn' */
#define STAGE3_HASH         "\xA7\x67\x80\x64\x81\x97\x0F\x54"
#define STAGE3_WELCOME      0x03
#define STAGE3_WELCOME_STR  "\xca\x2f\x8c\x28\x96\xc2\x1b\x4f\xc1\x61\xee\x58\x20\x5c\x95\x9b" \
                            "\x5d\x1b\x44\x9f\x20\x83\xbe\xf3\x39\xb9\x19\x05\xac\x9b\x0d\xd1" \
                            "\xb4\x6a\xd2\xad\xe7\x81\xbe\xdb\x05\xd0\x7f\x83\xe1\x21\xb3\x88" \
                            "\x3d\x0f\x0c\xb3\x20\xbb\xdc\xb6\x59\x3f"
#define STAGE3_WELCOME_LEN  58
#define STAGE3_PAYLOAD      0x04
#define STAGE3_PAYLOAD_STR  "\xb9\x77\xc9\x7c\xe5\xc6\x1b\x59\xd6\x7b\xe9\x01\x63\x5f\x94\x88" \
                            "\x18\x1e\x17\x96\x20\xfc\xb2\xbe\x12\xa1\x58\x75\xcf"
#define STAGE3_PAYLOAD_LEN  29

/* Stage 3 hash for 'B1p!BlP!' */
#define STAGE4_HASH         "\xF0\x64\xB9\x87\x08\xB4\x89\xB2"

#define NB_RSRC 5

typedef struct {
    int id;
    char raw[112];
    int len;
} RAWRES,PRAWRES;


/*****************************
 * Hardcoded resources
 ****************************/

static RAWRES gRsrc[NB_RSRC] = {
    {STAGE1_WELCOME, STAGE1_WELCOME_STR, STAGE1_WELCOME_LEN},
    {STAGE2_WELCOME, STAGE2_WELCOME_STR, STAGE2_WELCOME_LEN},
    {STAGE2_PAYLOAD, STAGE2_PAYLOAD_STR, STAGE2_PAYLOAD_LEN},
    {STAGE3_WELCOME, STAGE3_WELCOME_STR, STAGE3_WELCOME_LEN},
    {STAGE3_PAYLOAD, STAGE3_PAYLOAD_STR, STAGE3_PAYLOAD_LEN}/*,
    {STAGE4_WELCOME, STAGE4_WELCOME_STR, STAGE4_WELCOME_LEN},
    {STAGE4_PAYLOAD, STAGE4_PAYLOAD_STR, STAGE4_PAYLOAD_LEN}*/
};

/*****************************
 * Resources access routines
 ****************************/

int rsrc_find(int id, char *buffer, int len);


#endif /* __INC_RESOURCES_H */
