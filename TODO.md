# write_data
1. Add crc every 16 bytes into packet
2. Calculate and set length of packet

# client
1. Fix poll always returning that all sockets have data
2. Improve checking of revents to include POLLHUP, POLLERR, POLLNVAL
3. Add ability for server to send acknowedlgement before data

# server 
1. Add flag generation to Group 1 Var 2, both group 30
    * transmit said flag along with packet
    * currently just sending 0 for all
2. 