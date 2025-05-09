#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main() {
    int dev = hci_open_dev(hci_get_route(0));
    if (dev < 0) return 1;

    uint8_t data[31] = { 0x02, 0x01, 0x06 };
    const char *name = "DogStove";
    size_t len = strlen(name);
    if (len > 29) len = 29;
    data[3] = len + 1;
    data[4] = 0x09;
    memcpy(&data[5], name, len);

    le_set_advertising_parameters_cp adv_params = {0};
    adv_params.min_interval = htobs(0x0800);
    adv_params.max_interval = htobs(0x0800);
    adv_params.advtype = 0;
    adv_params.own_bdaddr_type = 0;
    adv_params.channel_map = 7;
    adv_params.filter = 0;

    hci_le_set_advertising_parameters(dev, &adv_params, 1000);
    hci_le_set_advertise_enable(dev, 0x01, 1000);
    hci_le_set_advertising_data(dev, sizeof(data), data, 1000);

    while (1) sleep(10);
    return 0;
}