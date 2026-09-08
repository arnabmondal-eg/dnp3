package ui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagLayout;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;

public class ClientWindow extends JFrame{
    private final String title;
    private final int[] INITAL_SIZE;
    private boolean visible = true;    // may be used later

    private final int clientNum;

    private JSplitPane splitPane;
    private JPanel infoPanel;
    private JTextArea packetInfo;
    private JScrollPane packetScroller;
    
    private JScrollPane serverScroller;
    private JPanel serverPanel;
    private JPanel server1;
    

    public ClientWindow() {
        this.clientNum = 1;
        this.title = "Client " + this.clientNum;
        this.INITAL_SIZE = new int[2];
        this.INITAL_SIZE[0] = 800;
        this.INITAL_SIZE[1] = 600;

        setTitle(this.title);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(INITAL_SIZE[0], INITAL_SIZE[1]);
        setLayout(new BorderLayout());

        // left pane
        infoPanel = new JPanel();
        infoPanel.setLayout(new BorderLayout());
        packetInfo = new JTextArea();
        packetInfo.setEditable(false);
        packetScroller = new JScrollPane(
            packetInfo, 
            JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
            JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED
        );
        splitPane = new JSplitPane(
            JSplitPane.VERTICAL_SPLIT, 
            infoPanel, 
            packetScroller
        );

        // right pane
        serverPanel = new JPanel();
        server1 = new JPanel();
        server1.setBackground(Color.GREEN);
        server1.setPreferredSize(new Dimension(150, 50));
        server1.add(new JLabel("Server 1"));
        serverPanel.add(server1);
        serverScroller = new JScrollPane(serverPanel);



        // add all components
        add(splitPane, BorderLayout.CENTER);
        add(serverScroller, BorderLayout.EAST);


        setVisible(true);

        splitPane.setDividerLocation(0.7);
    }
    
}
