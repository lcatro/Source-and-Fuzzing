

#include <stdio.h>

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/qdev-properties.h"
#include "qemu/event_notifier.h"
#include "qemu/module.h"
#include "sysemu/kvm.h"

#define DIY_PCI_DEVICE_VENDOR_ID 0x1234
#define DIY_PCI_DEVICE_DEVICE_ID 0x6666
#define DIY_PCI_DEVICE_BUFFER_MAX 512

#define IOTEST_IOSIZE  1024 * 4
#define IOTEST_MEMSIZE 2048

typedef struct PCITestDevState {
    /*< private >*/
    PCIDevice parent_obj;
    /*< public >*/

    MemoryRegion mmio;
    MemoryRegion portio;
    uint64_t buffer[DIY_PCI_DEVICE_BUFFER_MAX];
    MemoryRegion membar;
} PCITestDevState;

#define TYPE_DIY_DEV "vuln-device"

#define DIY_PCI_DEV(obj) \
    OBJECT_CHECK(PCITestDevState, (obj), TYPE_DIY_DEV)



static Property diy_pci_device_properties[] = {
    DEFINE_PROP_END_OF_LIST(),
};


int foo1(int b) {
    printf("foo1 !!\n");

    if (b==1) {
        ;
    } else {
        foo2(b);
    }

    return 0;
}

int foo2(int b) {
    printf("foo1 !!\n");

    if (b==1) {
        ;
    } else if (b==3) {
        ;
    } else {
        foo3(b);
    }

    return 0;
}

int foo3(int b) {
    printf("foo1 !!\n");

    if (b==1) {
        ;
    } else if (b==3) {
        ;
    } else if (b==4) {
        ;
    } else {
        foo4(b);
    }

    return 0;
}

int foo4(int b) {
    printf("foo1 !!\n");

    if (b==2) {
        foo5(1);
    } else if (b==4) {
        ;
    } else if (b==6) {
        ;
    } else if (b==5) {
        ;
    } else if (b==1) {
        ;
    }

    return 0;
}

int foo5(int b) {
    return 1;
}


static void
pci_testdev_write(void *opaque, hwaddr addr, uint64_t val,
                  unsigned size, int type)
{
    PCITestDevState* dev_object = (PCITestDevState*)opaque;

    __sanitizer_enter();
    printf("!!!!!\n");

    /*
    if (size == 1) {
        dev_object->buffer[addr] = (uint8_t)val;
    } else if (size == 2) {
        dev_object->buffer[addr] = (uint16_t)val;
    } else if (size == 4) {
        dev_object->buffer[addr] = (uint32_t)val;
    } else {
        dev_object->buffer[addr] = val;
    }*/

    foo1(val);

    __sanitizer_exit();
}

static uint64_t
pci_testdev_read(void *opaque, hwaddr addr, unsigned size)
{
    PCITestDevState* dev_object = (PCITestDevState*)opaque;
    
    return dev_object->buffer[addr];
}

static void
pci_testdev_mmio_write(void *opaque, hwaddr addr, uint64_t val,
                       unsigned size)
{
    pci_testdev_write(opaque, addr, val, size, 0);
}

static void
pci_testdev_pio_write(void *opaque, hwaddr addr, uint64_t val,
                       unsigned size)
{
    pci_testdev_write(opaque, addr, val, size, 1);
}

static const MemoryRegionOps pci_testdev_mmio_ops = {
    .read = pci_testdev_read,
    .write = pci_testdev_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static const MemoryRegionOps pci_testdev_pio_ops = {
    .read = pci_testdev_read,
    .write = pci_testdev_pio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static void pci_testdev_realize(PCIDevice *pci_dev, Error **errp)
{
    PCITestDevState *d = DIY_PCI_DEV(pci_dev);
    uint8_t *pci_conf;
    char *name;
    int r, i;
    bool fastmmio = kvm_ioeventfd_any_length_enabled();

    pci_conf = pci_dev->config;

    pci_conf[PCI_INTERRUPT_PIN] = 0; /* no interrupt pin */

    memory_region_init_io(&d->mmio, OBJECT(d), &pci_testdev_mmio_ops, d,
                          "pci-testdev-mmio", IOTEST_MEMSIZE * 2);
    memory_region_init_io(&d->portio, OBJECT(d), &pci_testdev_pio_ops, d,
                          "pci-testdev-portio", IOTEST_IOSIZE * 2);
    pci_register_bar(pci_dev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &d->mmio);
    pci_register_bar(pci_dev, 1, PCI_BASE_ADDRESS_SPACE_IO, &d->portio);
}

static void
pci_testdev_uninit(PCIDevice *dev)
{
}


static void
pci_testdev_reset(PCITestDevState *d)
{
}

static void diy_pci_device_reset(DeviceState *dev)
{
    PCITestDevState *d = DIY_PCI_DEV(dev);
    pci_testdev_reset(d);
}

static void diy_pci_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->realize = pci_testdev_realize;
    k->exit = pci_testdev_uninit;
    k->vendor_id = DIY_PCI_DEVICE_VENDOR_ID;
    k->device_id = DIY_PCI_DEVICE_DEVICE_ID;
    k->revision = 0x00;
    k->class_id = PCI_CLASS_OTHERS;
    dc->desc = "DIY PCI Device";
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
    dc->reset = diy_pci_device_reset;
    device_class_set_props(dc, diy_pci_device_properties);
}

static const TypeInfo diy_pci_info = {
    .name          = TYPE_DIY_DEV,
    .parent        = "pci-device",
    .instance_size = sizeof(PCITestDevState),
    .class_init    = diy_pci_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    },
};

static void diy_pci_register_types(void)
{
    type_register_static(&diy_pci_info);
}

type_init(diy_pci_register_types)



