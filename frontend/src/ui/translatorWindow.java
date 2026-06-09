package ui;

import java.awt.BorderLayout;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

public class translatorWindow extends JFrame {
    private final String title;
    private final int[] INITAL_SIZE;

    private JPanel menuBar;
    private JPanel packetReciver;
    private JPanel packetViewer;

    private DefaultTableModel tableModel;
    private JTable table;

    private boolean visible;

    public translatorWindow() {
        title = "translator";
        INITAL_SIZE = new int[] { 800, 600 };
        setTitle(title);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(INITAL_SIZE[0], INITAL_SIZE[1]);

        menuBar = new JPanel();
        JLabel menuLable = new JLabel("Upper");
        menuLable.setToolTipText("Menu Bar");
        menuBar.add(menuLable);

        packetReciver = new JPanel();
        JLabel reciverLabel = new JLabel("Middle");
        reciverLabel.setToolTipText("Packet Reciver");
        String[] columnNames = { "Timestamp", "Destination", "Function", "Length", "Hex" };
        tableModel = new DefaultTableModel(columnNames, 0);
        table = new JTable(tableModel);
        packetReciver.add(reciverLabel);
        JScrollPane tableScrollPane = new JScrollPane(table);

        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });
        tableModel.addRow(new Object[] { "120", "H", "Read", "5", "05 64 sd sj kdf" });

        packetViewer = new JPanel();
        JLabel viewerLabel = new JLabel("Bottom");
        viewerLabel.setToolTipText("Packet Viewer");
        packetViewer.add(viewerLabel);

        JSplitPane flexArea = new JSplitPane(JSplitPane.VERTICAL_SPLIT, tableScrollPane, packetViewer);
        flexArea.setDividerLocation(300);

        add(flexArea, BorderLayout.CENTER);
        add(menuBar, BorderLayout.NORTH);

        setVisible(true);

    }
}
