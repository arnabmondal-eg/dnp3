package ui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
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
    private JPanel packetPanel;
    private JLabel packetPanelTitle;
    private JTextArea packetInfo;
    private JScrollPane packetScroller;
    
    private JScrollPane serverScroller;
    private JPanel serverPanel;
    private JPanel[] servers;
    private int totalServers;
    

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
        packetPanel = new JPanel();
        packetPanel.setLayout(new BorderLayout());
        packetPanelTitle = new JLabel("Recent Packets");
        packetPanelTitle.setAlignmentY(LEFT_ALIGNMENT);
        packetInfo = new JTextArea();
        packetInfo.setEditable(false);
        packetScroller = new JScrollPane(
            packetInfo, 
            JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
            JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED
        );
        packetPanel.add(packetScroller, BorderLayout.CENTER);
        packetPanel.add(packetPanelTitle, BorderLayout.NORTH);
        splitPane = new JSplitPane(
            JSplitPane.VERTICAL_SPLIT, 
            infoPanel, 
            packetPanel
        );

        // right pane
        serverPanel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        serverPanel.
        servers = new JPanel[16];
        serverScroller = new JScrollPane(serverPanel);



        // add all components
        add(splitPane, BorderLayout.CENTER);
        add(serverScroller, BorderLayout.EAST);


        setVisible(true);

        splitPane.setDividerLocation(0.7);
    }

    public void updateRecentPackets(String packet) {
        this.packetInfo.append("\n" + packet);
    }

    public void addNewServer(int serverNumber) {
        if(this.totalServers > 15) {
            System.out.println("Too many servers!");
            return;
        }
        servers[totalServers] = new JPanel();
        servers[totalServers].setBackground(Color.GREEN);
        servers[totalServers].setPreferredSize(new Dimension(150, 50));
        servers[totalServers].add(new JLabel("Server " + serverNumber));

        this.serverPanel.add(servers[totalServers]);
        repaint();

        this.totalServers++;
    }

    public void removeServer(int serverNumber) {
        servers[totalServers].setBackground(Color.RED);
        totalServers--;
    }

}
