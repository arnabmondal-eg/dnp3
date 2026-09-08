package ui;

import java.awt.BorderLayout;
import java.awt.GridBagLayout;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JSplitPane;

public class ClientWindow extends JFrame{
    private final String title;
    private final int[] INITAL_SIZE;
    private boolean visible = true;    // may be used later

    private final int clientNum;

    private JSplitPane splitPane;
    
    private JPanel leftPanel;
    
    private JPanel rightPanel;
    private JSplitPane rightSplitPane;
    

    public ClientWindow(int num, int[] size) {
        this.clientNum = Math.abs(num);
        this.title = "Client " + this.clientNum;
        this.INITAL_SIZE = size;

        setTitle(this.title);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(INITAL_SIZE[0], INITAL_SIZE[1]);
        setLayout(new BorderLayout());

        setVisible(true);
    }

    public ClientWindow() {
        this.clientNum = 1;
        this.title = "Client " + this.clientNum;
        this.INITAL_SIZE = new int[] {800, 600};

        setTitle(this.title);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(INITAL_SIZE[0], INITAL_SIZE[1]);
        setLayout(new BorderLayout());

        setVisible(true);
    }
    
}
