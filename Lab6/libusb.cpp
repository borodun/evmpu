#include <iostream>
#include "libusb.h"

using namespace std;

void printEndpoint(const libusb_endpoint_descriptor *endpoint, int num) {
    cout << "      ENDPOINT #" << num << endl;
    cout << "        bDescriptorType:   " << hex << (int) endpoint->bDescriptorType << endl;
    cout << "        bEndpointAddress:  " << (int) endpoint->bEndpointAddress << dec << endl;
}

void printAltsetting(const libusb_interface_descriptor *interface, int num) {
    cout << "    INTERFACE #" << num << endl;
    cout << "      bInterfaceClass:     " << hex << (int) interface->bInterfaceClass << dec << endl;
    cout << "      bNumEndpoints:       " << (int) interface->bNumEndpoints << endl;
    for (int i = 0; i < interface->bNumEndpoints; i++) {
        printEndpoint(&interface->endpoint[i], i + 1);
    }
}

void printConfiguration(libusb_config_descriptor *config, int num) {
    cout << "  CONFIGURATION #" << num << endl;
    cout << "    bNumInterfaces:        " << (int) config->bNumInterfaces << endl;
    for (int i = 0; i < (int) config->bNumInterfaces; i++) {
        const libusb_interface *inter = &config->interface[i];
        for (int t = 0; t < inter->num_altsetting; t++) {
            printAltsetting(&inter->altsetting[t], i + 1);
        }
    }

}

void printInfo(libusb_device *dev, libusb_device_descriptor desc) {
    libusb_device_handle *handle = nullptr;
    unsigned char text[128];
    libusb_open(dev, &handle);
    if (handle) {
        if (desc.iManufacturer) {
            libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, text, sizeof(text));
            cout << "  Manufacturer:            " << text << endl;
        }
        if (desc.iProduct) {
            libusb_get_string_descriptor_ascii(handle, desc.iProduct, text, sizeof(text));
            cout << "  Product:                 " << text << endl;
        }
        if (desc.iSerialNumber) {
            libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, text, sizeof(text));
            cout << "  Serial Number:           " << text << endl;
        } else {
            cout << "  No Serial Number" << endl;
        }
        libusb_close(handle);
    }
}

void printDevice(libusb_device *dev, int num) {
    libusb_device_descriptor desc{};

    int check = libusb_get_device_descriptor(dev, &desc);
    if (check < 0) {
        cout << "No device descriptor" << endl;
        return;
    }

    cout << endl << "Device #" << num << " (DeviceClass = " << hex << (int) desc.bDeviceClass << "): "
         << desc.idVendor << " - " << desc.idProduct << dec << endl;

    printInfo(dev, desc);

    for (int i = 0; i < desc.bNumConfigurations; i++) {
        libusb_config_descriptor *config;

        cout << "  bNumConfigurations:      " << (int) desc.bNumConfigurations << endl;
        check = libusb_get_config_descriptor(dev, i, &config);
        if (check != LIBUSB_SUCCESS) {
            cout << "  No config descriptor" << endl;
            continue;
        }

        printConfiguration(config, i + 1);

        libusb_free_config_descriptor(config);
    }
}

int main() {
    libusb_device **devs;

    int initCheck = libusb_init(nullptr);
    if (initCheck < 0) {
        return 1;
    }

    libusb_set_option(nullptr, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_NONE);
    int deviceCount = libusb_get_device_list(nullptr, &devs);
    if (deviceCount < 0) {
        libusb_exit(nullptr);
        return 2;
    }
    cout << "Device count: " << deviceCount << endl;
    for (int i = 0; devs[i]; i++) {
        printDevice(devs[i], i + 1);
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(nullptr);
    return 0;
}