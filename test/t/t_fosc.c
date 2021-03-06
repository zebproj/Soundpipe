#include "soundpipe.h"
#include "md5.h"
#include "tap.h"
#include "test.h"

typedef struct {
    sp_fosc *osc;
    sp_ftbl *ft; 
    int counter;
} udata;

int t_fosc(sp_test *tst, sp_data *sp, const char *hash) 
{
    uint32_t n;
    int fail = 0;
    udata ud;
    ud.counter = 0;
    sp_ftbl_create(sp, &ud.ft, 2048);
    sp_fosc_create(&ud.osc);

    sp_gen_sine(sp, ud.ft);
    sp_fosc_init(sp, ud.osc, ud.ft);
    ud.osc->freq = 500;

    for(n = 0; n < tst->size; n++) {
        sp_test_add_sample(tst, 0);

        if(ud.counter == 0){
            ud.osc->freq = 500 + sp_rand(sp) % 2000;
        }

        sp_fosc_compute(sp, ud.osc, NULL, &sp->out[0]);
        ud.counter = (ud.counter + 1) % 4410;
    }



    if(sp_test_compare(tst, hash) == SP_NOT_OK) {
        printf("Generated hash %s does not match reference hash %s\n", 
                tst->md5string, hash);
        fail = 1;
    }
     

    sp_ftbl_destroy(&ud.ft);
    sp_fosc_destroy(&ud.osc);

    if(fail) return SP_NOT_OK;
    else return SP_OK;
}
