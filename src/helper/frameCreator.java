package helper;

import javax.swing.JFrame;

/** Method to easily create windows. Mainly made to save some time */
public class frameCreator extends JFrame {
    /** Default Window Size. int array of order [width, height] */
    private int[] defaultWindowSize = new int[] { 600, 400 }; // random size i chose

    /**
     * Window Constructor. Defaults behavior to exit on close.
     * <p>
     * 
     * @param visible Controls wether or not the new Window will imeditaly become
     *                visible
     * @param name    Title of Window
     * @param size    int array containing initial window size. Order by [width,
     *                height]
     */
    public frameCreator(Boolean visible, String name, int[] size) {
        this.setTitle(name);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(size[0], size[1]);
        this.setVisible(visible);
    }

    /**
     * Window Constructor. Defaults to size set in class. Defaults to exit on close.
     * <p>
     * 
     * @param visible Controls wether or not the new Window will imeditaly become
     *                visible
     * @param name    Title of Window
     *                <p>
     */
    public frameCreator(Boolean visible, String name) {
        this.setTitle(name);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(defaultWindowSize[0], defaultWindowSize[1]);
        this.setVisible(visible);
    }

}
