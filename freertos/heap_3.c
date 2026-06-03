/* Implementation of malloc/free for FreeRTOS heap_3 */
void *pvPortMalloc( size_t xWantedSize )
{
    void *pvReturn = NULL;
    static uint8_t *pucAlignedHeap = NULL;
    static size_t xNextFreeByte = 0;
    
    if( pucAlignedHeap == NULL )
    {
        /* First call - initialize the heap */
        pucAlignedHeap = ( uint8_t * ) pvPortMalloc( configTOTAL_HEAP_SIZE );
        xNextFreeByte = 0;
    }
    
    /* Simple bump allocator implementation */
    if( xWantedSize > 0 )
    {
        /* Align the requested size to 4-byte boundary */
        size_t xAlignedSize = ( xWantedSize + 3 ) & ~0x3;
        
        if( ( xNextFreeByte + xAlignedSize ) <= configTOTAL_HEAP_SIZE )
        {
            pvReturn = ( void * ) &pucAlignedHeap[ xNextFreeByte ];
            xNextFreeByte += xAlignedSize;
        }
    }
    
    return pvReturn;
}

/* Free function for heap_3 */
vPortFree( void *pv )
{
    /* heap_3 is a simple bump allocator - no actual free needed */
    /* This function exists for API compatibility but does nothing */
    ( void ) pv;
}