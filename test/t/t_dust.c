#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_dust *dst;
} udata;

int t_dust(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    SPFLOAT in = 0, out = 0;
    udata ud;
    
    sp_dust_create(&ud.dst);
    sp_dust_init(sp, ud.dst, 0.3, 10);
    sp->len = 44100 * 5;
    ud.dst->bipolar = 1.0;

    for(n = 0; n < tst->size; n++) {
        in = 0;
        out = 0;
        sp_dust_compute(sp, ud.dst, NULL, &out);
        sp_test_add_sample(tst, out);
    }

    if(sp_test_compare(tst, hash) == SP_NOT_OK) {
        printf("Generated hash %s does not match reference hash %s\n", 
                tst->md5string, hash);
        fail = 1;
    }
    
    sp_dust_destroy(&ud.dst);
     
    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
