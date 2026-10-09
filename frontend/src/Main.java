import ui.ClientWindow;
import ui.ParserWindow;
 
public class Main {
    public static void main(String[] args) {
        // new ParserWindow();
        ClientWindow client = new ClientWindow();
        client.addNewServer(1);
        client.addNewServer(2);
        
    }
}
