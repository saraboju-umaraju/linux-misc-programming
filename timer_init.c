static cyg_handle_t  sys_timer_handle;                                           
static cyg_interrupt sys_timer_interrupt;                                        
static cyg_uint64    sys_timer_ticks = 0;  
static void                                                                      
sys_timer_dsr(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data)         
{                                                                                
} 
static void sys_timer_init(void)                                                 
{                                                                                
    HAL_CLOCK_INITIALIZE(CYGNUM_HAL_RTC_PERIOD);                                 
                                                                                 
    cyg_drv_interrupt_create(                                                    
        CYGNUM_HAL_INTERRUPT_RTC,                                                
        0,                                 
        (CYG_ADDRWORD)0,                   
        sys_timer_isr,                     
        sys_timer_dsr,                     
        &sys_timer_handle,                 
        &sys_timer_interrupt );            
                                                                                 
    cyg_drv_interrupt_attach(sys_timer_handle);                                  
                                                                                 
    cyg_drv_interrupt_unmask(CYGNUM_HAL_INTERRUPT_RTC);                          
} 

static cyg_uint32                                                                
sys_timer_isr(cyg_vector_t vector, cyg_addrword_t data)                          
{                                                                                
    ++sys_timer_ticks;                                                           
                                                                                 
    HAL_CLOCK_RESET(CYGNUM_HAL_INTERRUPT_RTC, CYGNUM_HAL_RTC_PERIOD);            
                                                                                 
    cyg_drv_interrupt_acknowledge(CYGNUM_HAL_INTERRUPT_RTC);                     
                                                                                 
    return CYG_ISR_HANDLED;                                                      
}
