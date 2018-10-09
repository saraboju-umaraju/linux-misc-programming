/* programming ISA DMA
 * Using these functions, a driver can implement a function like
 * the following to prepare for a DMA transfer.
*/

int dad_dma_prepare(int channel, int mode, unsigned int buf,
unsigned int count)
{
  unsigned long flags;

  flags = claim_dma_lock( );
  disable_dma(channel);
  clear_dma_ff(channel);
  set_dma_mode(channel, mode);
  set_dma_addr(channel, virt_to_bus(buf));
  set_dma_count(channel, count);
  enable_dma(channel);
  release_dma_lock(flags);
  return 0;
}
/*
 * Then, a function like the next one is used to check for successful
 * completion of DMA
*/

int dad_dma_isdone(int channel)
{
  int residue;
  unsigned long flags = claim_dma_lock ( );

  residue = get_dma_residue(channel);
  release_dma_lock(flags);
  return (residue = = 0);
}
