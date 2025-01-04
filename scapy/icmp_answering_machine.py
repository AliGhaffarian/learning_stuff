from scapy.all import *
class icmp4_answering_machine(AnsweringMachine):
    filter="icmp" #i believe it is passed to sniff()

    # is sendp() by default, but i wanted to work on network layer, so i changed it to send(), which does the routing for me
    send_function=staticmethod(send) 
    def is_request(self, req: Packet) -> int:
        #redundant, as we are filting for "icmp" in the sniffing phase
        if ICMP not in req\
                or IP not in req:
            return False


        #if i'm not the dst
        if req[IP].dst != get_if_addr(req.sniffed_on)\
                or req[IP].src == "0.0.0.0": #0.0.0.0 is err value of get_if_addr()
                    return False


        return True

    def make_reply(self, req: Packet):
        #remove headers below network layer
        rep = req[IP:]

        #swap dst and src ip
        rep[IP].dst, rep[IP].src = req[IP].src, req[IP].dst

        #prints a detailed view of rep
        rep.show()
        return rep
a = icmp4_answering_machine()

a()
