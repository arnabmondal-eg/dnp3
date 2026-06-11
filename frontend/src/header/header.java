package header;

public class header {
    public boolean valid;
    // TODO: Add definitions for each byte
    /** Denotes starting byte of dnp3 header; 05-64 */
    private int start;

    /** LEN byte of header */
    private int LEN;
    /** DLC byte of header */
    private int DLC;

    /** Destination byte */
    private int destination;
    /** Source byte */
    private int source;

    /** Cyclic Redundancy Check (CRC) byte of header */
    private int CRC;

    /**
     * Initalizes new Header object. Will save segment header, as well as save string and int version for all byte blocks
     * @param rawHexCode Expects int of hex type 
     */
    public header(int rawHexCode) {
    }

    /**Method to check if packet is valid. ALL dnp3 packets must start with 05-64, 
     * and if they do not, they are either malformed, not a packet, or something else. */
    private boolean checkStartByte(int rawHexCode) {
        /*
        Going to use string comparaison
        Know its not the best, but other method I though of used modulo divison and other things.
        I feel like this is the cleanest way to procede, as we can easily itterate through string and preform checks.
        If you have another idea, put in a pull request ;)
        */

        boolean valid = true;  // assume false
        String startSequence = "0564";                          // start sequence of all (valid) dnp3 packets
        String inputSequence = Integer.toString(rawHexCode);

        for (int i = 0; i < 3; i++) {
            if (inputSequence.charAt(i) != startSequence.charAt(i)) {
                valid = false;
                break;
            }
        }
        return valid;
    }
}
