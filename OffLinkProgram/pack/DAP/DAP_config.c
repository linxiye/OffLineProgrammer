#include "DAP_config.h"


static struct SWD_Function *fun;

void PORT_SWD_SETUP(void)
{
    // Set SWCLK HIGH
    fun->swd_pin_set_out(fun->swclk);
    fun->swd_pin_set_out(fun->swdio_out);
    fun->swd_pin_set_in(fun->swdio_in);
    fun->swd_pin_set_out(fun->reset);

    fun->swd_pin_set_val(fun->swclk,1);
    fun->swd_pin_set_val(fun->swdio_out,1);
    fun->swd_pin_set_val(fun->reset,1);
}

void PORT_OFF(void)
{
    fun->swd_pin_set_in(fun->swdio_in);
    fun->swd_pin_set_in(fun->swdio_out);
    fun->swd_pin_set_in(fun->swclk);
}

uint32_t PIN_SWCLK_TCK_IN(void)
{
    return fun->swd_pin_get_val(fun->swclk);
}

void PIN_SWCLK_TCK_SET(void)
{
    fun->swd_pin_set_val(fun->swclk, 1);
}

void PIN_SWCLK_TCK_CLR(void)
{
    fun->swd_pin_set_val(fun->swclk, 0);
}

uint32_t PIN_SWDIO_TMS_IN(void)
{
    return fun->swd_pin_get_val(fun->swdio_in);
}

void PIN_SWDIO_TMS_SET(void)
{
    fun->swd_pin_set_val(fun->swdio_out, 1);
}

void PIN_SWDIO_TMS_CLR(void)
{
    fun->swd_pin_set_val(fun->swdio_out, 0);
}

uint32_t PIN_SWDIO_IN(void)
{
    return fun->swd_pin_get_val(fun->swdio_in);
}

void PIN_SWDIO_OUT(uint32_t bit)
{
    fun->swd_pin_set_val(fun->swdio_out, bit);
}

void PIN_SWDIO_OUT_ENABLE(void)
{
    fun->swd_pin_set_out(fun->swdio_out);
    fun->swd_pin_set_val(fun->swdio_out, 0);
}

void PIN_SWDIO_OUT_DISABLE(void)
{
    fun->swd_pin_set_val(fun->swdio_out, 1);
    fun->swd_pin_set_in(fun->swdio_out);
}

uint32_t PIN_nRESET_IN(void)
{
    return fun->swd_pin_get_val(fun->reset);
}

void PIN_nRESET_OUT(uint32_t bit)
{
    fun->swd_pin_set_val(fun->reset, bit);
}

void DAP_SETUP(void)
{

    fun->swd_pin_set_out(fun->swclk);
    fun->swd_pin_set_out(fun->swdio_out);
    fun->swd_pin_set_in(fun->swdio_in);
    fun->swd_pin_set_out(fun->reset);

    fun->swd_pin_set_val(fun->swclk,1);
    fun->swd_pin_set_val(fun->swdio_out,1);
    fun->swd_pin_set_val(fun->reset,1);

}

void DAP_Init(struct SWD_Function *p)
{
    fun = p;
}
