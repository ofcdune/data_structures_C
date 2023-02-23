IPAM: 
- Verbundwarte und evtl. Jürgens Adressen entfernen
- andere, externe Adressen nachpflegen => Subnetz hinzufügen => 

Subnetz im CIDR laut FortiNet + Beschreibung "Internetnetz Standort ..." + VRF + im Wurzelordner
+ IP an sich anlegen (keine Gateways)

Router -> Static Route -> Gateway anlegen

Raisecom:
Projektieren mit externem DHCP Server plus Script aus der Datenbank, per SSH mit den Geräten verbinden und automatisch Befehle absetzen

def ip_to_bin(ip: str):
    ip_split = ip.split(".")
    ip_split_int = list(map(int, ip_split))

    for index, element in enumerate(ip_split_int):
        if element > 255:
            raise NotImplementedError(
                "Invalid IPv4 Address (Cannot be above 255)"
            )
        ip_split_int[index] = bin(element).split("b")[1].rjust(8, "0")

    return "".join(ip_split_int)


def bin_to_ip(bin_ip: str):
    bin_ip = [bin_ip[i:i + 8] for i in range(0, len(bin_ip), 8)]
    for index, element in enumerate(bin_ip):
        bin_ip[index] = str(int(element, 2))
    return ".".join(bin_ip)


class NetworkMask:

    def __init__(self):
        # the base ip address, often ending with a 0
        self.network_address = None

        # the amount of bits that are used to determine the host address
        self.network_range = None

        # the network mask in decimal notation used to determine the network address
        self.network_mask = None

        # the amount of devices fitting into this network (2^(32-network_range)-2)
        self.device_amount = None

        # the very first ip address available in this network
        self.min_ip = None

        # the very last ip address available in this network (not the broadcast address)
        self.max_ip = None

    @classmethod
    def from_cidr(cls, base_ip: str, network_range: int):
        nm = NetworkMask()
        nm.network_range = network_range

        binary_ip = ip_to_bin(base_ip)
        network_address = binary_ip[:network_range].ljust(32, "0")

        nm.min_ip = bin_to_ip(network_address[:31] + "1")
        nm.max_ip = bin_to_ip(binary_ip[:network_range].ljust(31, "1") + "0")
        nm.network_address = bin_to_ip(network_address)

        nm.network_mask = bin_to_ip(("1" * network_range).ljust(32, "0"))
        nm.device_amount = pow(2, 32 - network_range) - 2

        return nm

    @classmethod
    def from_decimal(cls, base_ip: str, network_mask: str):
        nm = NetworkMask()
        nm.network_mask = network_mask

        binary_ip = ip_to_bin(base_ip)
        binary_range = ip_to_bin(network_mask)
        set_bits = 0

        for index, bit in enumerate(binary_range):
            if bit == "1" and index != 0 and binary_range[index - 1] == "0":
                raise NotImplementedError(
                    "Invalid network range (Cannot be alternating 0 and 1"
                )
            if bit == "1":
                set_bits += 1

        network_address = binary_ip[:set_bits].ljust(32, "0")

        nm.network_range = set_bits
        nm.device_amount = pow(2, 32-set_bits) - 2
        nm.min_ip = bin_to_ip(network_address[:31] + "1")
        nm.max_ip = bin_to_ip(binary_ip[:set_bits].ljust(31, "1") + "0")
        nm.network_address = bin_to_ip(network_address)

        return nm

    @classmethod
    def from_range(cls, network_address: str, broadcast_address: str):
        nm = NetworkMask()
        nm.network_address = network_address
        nm.max_ip = bin_to_ip(ip_to_bin(broadcast_address)[:31] + "0")

        first_ip_bin_int = ip_to_bin(network_address)
        broadcast_ip_bin_int = int(ip_to_bin(broadcast_address), 2)

        nm.network_range = 32 - (len(bin(int(first_ip_bin_int, 2) ^ broadcast_ip_bin_int)) - 2)
        nm.device_amount = pow(2, 32 - nm.network_range) - 2
        nm.network_mask = bin_to_ip(("1" * nm.network_range).ljust(32, "0"))
        nm.min_ip = bin_to_ip(first_ip_bin_int[:31] + "1")

        return nm

    def info(self):
        return f"{self.network_address}/{self.network_range} ({self.network_mask})\n" \
               f"Range: From {self.min_ip} to {self.max_ip} ({self.device_amount} devices)"

    def __contains__(self, item: str):
        item = ip_to_bin(item)[:self.network_range].ljust(32, "0")
        return bin_to_ip(item) == self.network_address

    def __str__(self):
        return f"{self.network_address}/{self.network_range}"


def ip_in_subnet(ip_to_check, subnet_info):
    network_address = ip_to_bin(ip_to_check)[:subnet_info["network_range"]].ljust(32, "0")

    if bin_to_ip(network_address) == subnet_info["network_address"]:
        return True
    else:
        return False


if __name__ == '__main__':
    print(NetworkMask.from_cidr("", 30).info())
