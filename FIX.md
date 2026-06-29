# Fix Soon
* Fix all warnings and notes from compiler
* Rewrite all mk Structs to check size of packet
* Add comments to more things
* Fix naming
* Organize src and inc files better maybe
* Clarify assumptions made (ie. All input packets are arrays with values 1 byte in size)

# Fixed
* June 25, 2026
    * Rewrite any structs that are using whole uint8_t for a single bit (dlc, transport header)
    * Convert All print Functions into logging into a file
    * Convert large byte to function code defs into pointer tables

* June 29, 2026
    * Error in Trasnport Header and other logic resulting in unintilized return
        * Temp fix