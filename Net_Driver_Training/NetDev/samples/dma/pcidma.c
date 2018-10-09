/*
 * The dma operations on the PCI device depends on the device.
 * The following code shows an example for dma transfer and interrupt handling
*/

int dma_transfer(struct dad_dev, *dev, int write, void *buffer, size_t count)
{
    dma_addr_t bus_addr;
    unsigned long flags;

    /* map the buffer for DMA */
    dev->dma_dir = (write ? PCI_DMA_TODEVICE : PCI_DMA_FROMDEVICE);
    dev->dma_size = count;
    bus_addr = pci_map_single(dev->pci_dev, buffer, count, , dev->dma_dir);
    dev->dma_addr = bus_addr;

    /* setup the device */
    writeb(dev->registers.command, DAD_CMD_DISABLEDMA);
    writeb(dev->registers.command, write ? DAD_CMD_WR : DAD_CMD_RD);
    writeb(dev->registers.addr, bus_addr);
    writeb(dev->registers.len, count);

    /* start the operation */
    writeb(dev->registers.command, DAD_CMD_ENABLEDMA);
    return 0;
}

void dad_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
    struct dad_dev *dev = (struct dad_dev *)dev_id;

    /* make sure it's really our device interrupting */

    /* unmap the dma buffer */
    pci_unmap_single(dev->pci_dev, dev->dma_addr, dev->dma_size, dev->dma_dir);

    /* only now it is safe to access the buffer, copy to user etc. */

}

