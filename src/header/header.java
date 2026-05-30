package header;

public class header {
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

}
