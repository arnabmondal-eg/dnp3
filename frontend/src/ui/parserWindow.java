package ui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.JTree;
import javax.swing.border.Border;
import javax.swing.table.DefaultTableModel;

public class parserWindow extends JFrame {
    private final String title;
    private final int[] INITAL_SIZE;
    private boolean visible;    // may be used later

    private JSplitPane splitPane;

    // left panel
    private JPanel leftPanel;   // panel for source hex
    private JScrollPane sourceScrollPane;
    private JTextArea sourceInput;  // input for source hex
    private JButton parseButton;    // parse source hex
    private JLabel leftLabel;

    // right panel
    private JPanel rightPanel;  // panel for parsed hex
    private JTree parsedTree;       // tree for parsed hex
    private JPanel vaildIndicator;  // indicator for validity of source
    private JLabel vaildLabel;      // label for vaildIndicator
    private JLabel rightLabel;

    public parserWindow() {
        title = "Parser";
        INITAL_SIZE = new int[] { 800, 600 };
        setTitle(title);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(INITAL_SIZE[0], INITAL_SIZE[1]);

        // left side components
        leftPanel = new JPanel();
        leftPanel.setLayout(new BorderLayout());

        leftLabel = new JLabel("Source");
        leftLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        sourceInput = new JTextArea();
        sourceInput.setLineWrap(true);
        sourceInput.setWrapStyleWord(true);
        sourceScrollPane = new JScrollPane(sourceInput);
        
        parseButton = new JButton("Parse Source");  //TODO: Setup Action Handler
        parseButton.setAlignmentX(Component.CENTER_ALIGNMENT);


        // right side components
        rightPanel = new JPanel();
        rightPanel.setLayout(new BorderLayout());

        rightLabel = new JLabel("Parsed");
        rightLabel.setAlignmentX(Component.CENTER_ALIGNMENT);

        parsedTree = new JTree();   //TODO: Replace with tree modle

        vaildIndicator = new JPanel();  //TODO: Setup color change based on bool, and text change between Vaild/Not
        vaildIndicator.setBackground(Color.GREEN);
        vaildLabel = new JLabel("VALID");
        vaildIndicator.add(vaildLabel);


        // add all components to main frame
        leftPanel.add(leftLabel, BorderLayout.NORTH);
        leftPanel.add(sourceScrollPane, BorderLayout.CENTER);
        leftPanel.add(parseButton, BorderLayout.SOUTH);

        rightPanel.add(rightLabel, BorderLayout.NORTH);
        rightPanel.add(parsedTree, BorderLayout.CENTER);
        rightPanel.add(vaildIndicator, BorderLayout.SOUTH);

        splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, leftPanel, rightPanel);

        add(splitPane, BorderLayout.CENTER);
        
        setVisible(true);

        splitPane.setDividerLocation(0.5);
    }
}
